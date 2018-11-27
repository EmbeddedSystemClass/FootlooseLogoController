/*************************************************
 * @file DMXReceiver.h
 * @brief DMX receiver interface
 *
 *************************************************/
#include "app/DMXReceiver.h"

#include <cstring>
#include "stdint.h"

#include "app/BinDecIO.h"
#include "drv/DRVSerial.h"
#include "queue.hpp"

DMXReceiver::DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, HALTimer& timer, uint8_t channelCount)
    : TaskState(taskToNotify, ID)
    , m_uart(uart)
    , m_timer(timer)
    , m_address(NULL)
    , m_queue(NULL)
    , m_channelCount(channelCount)
    , m_startTime(0)
    , m_stopTime(0)
{
}

DMXReceiver::DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, HALTimer& timer, BinDecIO* dmxAddress, uint8_t channelCount)
    : TaskState(taskToNotify, ID)
    , m_uart(uart)
    , m_timer(timer)
    , m_address(dmxAddress)
    , m_queue(NULL)
    , m_channelCount(channelCount)
    , m_startTime(0)
    , m_stopTime(0)
{
}

DMXReceiver::DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, HALTimer& timer, BinDecIO* dmxAddress, cpp_freertos::Queue* queue,
                         uint8_t channelCount)
    : TaskState(taskToNotify, ID)
    , m_uart(uart)
    , m_timer(timer)
    , m_address(dmxAddress)
    , m_queue(queue)
    , m_channelCount(channelCount)
    , m_startTime(0)
    , m_stopTime(0)
{
}

void DMXReceiver::getChannels(uint8_t* data) {}

bool DMXReceiver::isNewDataReceived() { return false; }

void DMXReceiver::insertTestDataInQueue()
{
    if (m_queue->IsEmpty())
    {
        uint8_t data[m_channelCount];

        memset(data, 0xAA, m_channelCount);

        m_queue->Enqueue(data);
    }
}

HALTimer::CallbackFunction DMXReceiver::timerCallback(HALTimer::CallbackEvent event, HALTimer::TimerChannel channel, uint32_t value, void* This)
{
    // do something
    uint32_t timeDiff = 0;

    if (event == HALTimer::CallbackTrigger)
    {
        switch (channel)
        {
        case HALTimer::TimerChannel3:

            static_cast<DMXReceiver*>(This)->m_stopTime = value;

            timeDiff = m_stopTime - m_startTime;
            if (timeDiff > 100)
            {
            }

            break;

        case HALTimer::TimerChannel4:
            static_cast<DMXReceiver*>(This)->m_startTime = value;
            break;
        default:
            break;
        }
    }
}
