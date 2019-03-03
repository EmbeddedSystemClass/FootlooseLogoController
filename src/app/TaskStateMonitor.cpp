/*************************************************
 * @file TaskStateMonitor.cpp
 * @brief Task state monitor
 *
 *************************************************/
#include "stdint.h"

#include "OSError.h"
#include "app/TaskState.h"
#include "app/TaskStateMonitor.h"
#include "drv/DRVGPIO.h"
#include "thread.hpp"

TaskStateMonitor::TaskStateMonitor(const char* name, GPIOpin led)
    : cpp_freertos::Thread(name, 100, 2)
    , m_led(led)
{
    for (uint8_t ii = 0; ii < 16; ii++) m_taskStates[ii] = TaskState::TaskStateNone;
}

TaskStateMonitor::~TaskStateMonitor() {}

void TaskStateMonitor::Run()
{

    while (1)
    {
        uint32_t         notificationValue = 0;
        TaskState::State highestState      = TaskState::TaskStateNone;
        // wait for notification
        WaitForNotification(&notificationValue);

        if (notificationValue)
        {
            uint8_t ID = (notificationValue >> 16) & 0xFF;
            if (ID < 16)
            {
                m_taskStates[ID] = static_cast<TaskState::State>(notificationValue & 0xFFFF);
            }
            else
            {
                REPORTLOG("ID higher than 16 received")
            }

            highestState = getHighestState();

            switch (highestState)
            {
            case TaskState::TaskStateError:
                m_led.setDutyCycle(20);
                m_led.setFrequency(10);
                break;
            case TaskState::TaskStateRunning:
                m_led.setDutyCycle(50);
                m_led.setFrequency(2);
                break;
            case TaskState::TaskStateWaiting:
                m_led = true;
                break;
            case TaskState::TaskStateInit:
                m_led = false;
                break;
            case TaskState::TaskStateNone:
            default:
                m_led = false;
                break;
            }
        }
    }
}

TaskState::State TaskStateMonitor::getHighestState()
{
    TaskState::State retVal = TaskState::TaskStateNone;

    for (uint8_t ii = 0; ii < 16; ii++)
    {
        if (static_cast<uint8_t>(m_taskStates[ii]) > static_cast<uint8_t>(retVal))
        {
            retVal = m_taskStates[ii];
        }
    }

    return retVal;
}
