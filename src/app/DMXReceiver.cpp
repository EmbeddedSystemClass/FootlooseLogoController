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
#include "hal/HALTimer.h"
#include "queue.hpp"

DMXReceiver::DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, GPIOpin& pin, HALTimer& timer, uint8_t channelCount)
    : TaskState(taskToNotify, ID)
    , cpp_freertos::Thread(":DMX rec", 300, 5)
    , m_uart(uart)
    , m_timer(timer)
    , m_pin(pin)
    , m_address(NULL)
    , m_queue(NULL)
    , m_channelCount(channelCount)
    , m_startTime(0)
    , m_stopTime(0)
    , m_state(StateInit)
    , m_timeout(0)
    , m_selectedChannels(channelCount)
    , m_newData(false)
{
}

DMXReceiver::DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, GPIOpin& pin, HALTimer& timer, BinDecIO* dmxAddress,
                         uint8_t channelCount)
    : TaskState(taskToNotify, ID)
    , cpp_freertos::Thread(":DMX rec", 200, 5)
    , m_uart(uart)
    , m_timer(timer)
    , m_pin(pin)
    , m_address(dmxAddress)
    , m_queue(NULL)
    , m_channelCount(channelCount)
    , m_startTime(0)
    , m_stopTime(0)
    , m_state(StateInit)
    , m_timeout(0)
    , m_selectedChannels(channelCount)
    , m_newData(false)
{
}

DMXReceiver::DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, GPIOpin& pin, HALTimer& timer, BinDecIO* dmxAddress,
                         cpp_freertos::Queue* queue, uint8_t channelCount)
    : TaskState(taskToNotify, ID)
    , cpp_freertos::Thread(":DMX rec", 200, 5)
    , m_uart(uart)
    , m_timer(timer)
    , m_pin(pin)
    , m_address(dmxAddress)
    , m_queue(queue)
    , m_channelCount(channelCount)
    , m_startTime(0)
    , m_stopTime(0)
    , m_state(StateInit)
    , m_timeout(0)
    , m_selectedChannels(channelCount)
    , m_newData(false)
{
}

void DMXReceiver::Run()
{
    m_pin.setAlternateFunction(1);

    m_timer.setCallback(timerCallback, static_cast<void*>(this));

    m_uart.registerCallback(uartCallback, HALUart::Received, static_cast<void*>(this));

    m_timer.start();

    setTaskState(TaskStateWaiting);

    m_state = StateWaitingBreak;

    while (1)
    {
        while (m_state != StateReceived)
        {
            Delay(10);
            if (m_state == StateReceiving)
            {
                // A break was received, data should be being received now
                m_timeout++;
                if (m_timeout > 100)
                {
                    // No data received after break, try and capture break again
                    m_timeout = 0;

                    m_pin.setAlternateFunction(1);
                    m_timer.start();

                    setTaskState(TaskStateWaiting);
                    m_state = StateWaitingBreak;
                }
            }
        }
        // reset counter
        m_timeout = 0;

        // timer and uart are off now

        if (m_dmxBuffer[0] == 0)
        {
            // first one should be 0
            memcpy(m_selectedChannels.data(), &m_dmxBuffer[*m_address + 1], m_channelCount);
            if (m_queue != NULL)
            {
                if (!m_queue->IsFull())
                {
                    m_queue->Enqueue(m_selectedChannels.data());
                }
            }
            m_newData = true;
        }
        m_pin.setAlternateFunction(1);
        m_timer.start();

        m_state = StateWaitingBreak;
    }
}

void DMXReceiver::getChannels(uint8_t* data)
{
    memcpy(data, m_selectedChannels.data(), m_channelCount);
    m_newData = false;
}

bool DMXReceiver::isNewDataReceived() { return m_newData; }

void DMXReceiver::insertTestDataInQueue()
{
    if (m_queue->IsEmpty())
    {
        uint8_t data[m_channelCount];

        memset(data, 0xAA, m_channelCount);

        data[m_channelCount - 1] = 11;

        m_queue->Enqueue(data);
    }
}

void DMXReceiver::uartCallback(HALUart::CallBack type, void* parameter)
{
    DMXReceiver* This = static_cast<DMXReceiver*>(parameter);
    switch (type)
    {
    case HALUart::Received:

        // now we should have 512 bytes
        This->setTaskStateFromISR(TaskStateWaiting);
        This->m_state = StateReceived;

        //        HAL_GPIO_WritePin(GPIOB, (1 << 8), GPIO_PIN_RESET);

        break;
    default:
        // anything we dont want we treat as an error.
        break;
    }
}

void DMXReceiver::timerCallback(HALTimer::CallbackEvent event, HALTimer::TimerChannel channel, uint32_t value, void* parameter)
{
    DMXReceiver* This = static_cast<DMXReceiver*>(parameter);

    uint32_t timeDiff = 0;

    if (event == HALTimer::CallbackTrigger && This->m_state == StateWaitingBreak)
    {
        switch (channel)
        {
        case HALTimer::TimerChannel4:
            if (This->m_startTime != 0)
            {
                //                HAL_GPIO_WritePin(GPIOA, (1 << 1), GPIO_PIN_RESET);
                This->m_stopTime = value;

                timeDiff = This->m_stopTime - This->m_startTime;

                if (timeDiff > 8 && timeDiff < 1000)  // No larger than 10ms
                {
                    // break detected
                    // start dmx receiver
                    //                HAL_GPIO_WritePin(GPIOA, (1 << 9), GPIO_PIN_RESET);
                    This->m_pin.setAlternateFunction(7);
                    This->setTaskStateFromISR(TaskStateRunning);
                    This->m_state = StateReceiving;
                    This->m_uart.readBuffer(This->m_dmxBuffer, 513);
                    This->m_timer.stop();
                    //                    HAL_GPIO_TogglePin(GPIOA, (1 << 2));
                }
                This->m_startTime = 0;
            }

            break;

        case HALTimer::TimerChannel3:
            static_cast<DMXReceiver*>(This)->m_startTime = value;
            //            HAL_GPIO_WritePin(GPIOA, (1 << 9), GPIO_PIN_SET);
            //            HAL_GPIO_WritePin(GPIOA, (1 << 1), GPIO_PIN_SET);

            break;
        default:
            break;
        }
    }
}
