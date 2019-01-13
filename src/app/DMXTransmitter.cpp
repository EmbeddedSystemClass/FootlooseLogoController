/*************************************************
 * @file DMXTransmitter.h
 * @brief DMX transmitter interface
 *
 *************************************************/
#include "app/DMXTransmitter.h"

#include <cstring>
#include <vector>
#include "stdint.h"

#include "app/TaskState.h"
#include "drv/DRVSerial.h"
#include "os/queue.hpp"
#include "os/ticks.hpp"

using namespace cpp_freertos;

DMXTransmitter::DMXTransmitter(TaskHandle_t taskToNotify, uint8_t ID, GPIOpin& uartPin, DRVSerial& uart, uint8_t lastChannel)
    : TaskState(taskToNotify, ID)
    , cpp_freertos::Thread(":DMX trx", 200 + (lastChannel / 4), 5)
    , m_uart(uart)
    , m_uartPin(uartPin)
    , m_queue(NULL)
    , m_channelCount(lastChannel)
    , m_state(StateInit)
    , m_lastSendTime(0)
    , m_dataWasUpdated(false)
{

    m_dmxBuffer = static_cast<uint8_t*>(malloc(lastChannel + 1));

    memset(m_dmxBuffer, 0, lastChannel + 1);
}

DMXTransmitter::DMXTransmitter(TaskHandle_t taskToNotify, uint8_t ID, GPIOpin& uartPin, DRVSerial& uart, uint8_t lastChannel,
                               cpp_freertos::Queue* queue)
    : TaskState(taskToNotify, ID)
    , cpp_freertos::Thread(":DMX trx", 200 + (lastChannel / 4), 5)
    , m_uart(uart)
    , m_uartPin(uartPin)
    , m_queue(queue)
    , m_channelCount(lastChannel)
    , m_state(StateInit)
    , m_lastSendTime(0)
    , m_dataWasUpdated(false)
{
    m_dmxBuffer = static_cast<uint8_t*>(malloc(lastChannel + 1));

    memset(m_dmxBuffer, 0, lastChannel + 1);
}

void DMXTransmitter::Run()
{

    // setup
    m_uart.registerCallback(uartCallback, HALUart::Send, static_cast<void*>(this));

    while (1)
    {
        bool     sendDMX     = false;
        uint32_t currentTime = Ticks::TicksToMs(Ticks::GetTicks());

        // check for data update
        if (m_queue != NULL)
        {
            // process all waiting updates
            while (!m_queue->IsEmpty())
            {
                // update data
                DMXQueueItem item;
                m_queue->Dequeue(&item, 10);
                writeChannels(item.channeldata, item.startAddress, item.channelCount);
                sendDMX = true;
            }
        }

        if (m_dataWasUpdated)
        {
            sendDMX          = true;
            m_dataWasUpdated = false;
        }

        // check for periodic update
        if ((currentTime - m_lastSendTime) > 500)
        {
            // Send an update every 500ms, don't care about the overflow of the time values
            sendDMX = true;
        }

        if (sendDMX)
        {
            m_lastSendTime = Ticks::TicksToMs(Ticks::GetTicks());
            m_state        = StateBreak;
            setTaskStateFromISR(TaskStateRunning);
            m_uartPin.setNormalFunction();
            m_uartPin = 0;
            Delay(1);
            m_uartPin = 1;
            m_uartPin.setAlternateFunction();
            m_uart.writeBuffer(m_dmxBuffer, m_channelCount);
        }

        Delay(1);
    }
}

void DMXTransmitter::writeChannels(uint8_t* data, uint16_t startAddress, uint8_t channelCount)
{
    if ((startAddress + channelCount) < m_channelCount)
    {
        memcpy(&m_dmxBuffer[startAddress], data, channelCount);
        m_dataWasUpdated = true;
    }
}

void DMXTransmitter::uartCallback(HALUart::CallBack type, void* parameter)
{
    DMXTransmitter* This = static_cast<DMXTransmitter*>(parameter);

    switch (type)
    {
    case HALUart::Send:
        This->m_state = StateIdle;
        This->setTaskStateFromISR(TaskStateWaiting);
        break;
    case HALUart::Error:
        This->m_state = StateIdle;
        This->setTaskStateFromISR(TaskStateError);
        break;
    default:
        // dont care
        break;
    }
}
