/*************************************************
 * @file DMXReceiver.h
 * @brief DMX receiver interface
 *
 *************************************************/
#pragma once

#include <string>
#include "stdint.h"

#include "BinDecIO.h"
#include "TaskState.h"
#include "drv/DRVSerial.h"
#include "hal/HALTimer.h"
#include "queue.hpp"

/**
 * DMXReceiver
 */
class DMXReceiver : public TaskState
{
public:
    DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, HALTimer& timer, uint8_t channelCount);
    DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, HALTimer& timer, BinDecIO* dmxAddress, uint8_t channelCount);
    DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, HALTimer& timer, BinDecIO* dmxAddress, cpp_freertos::Queue* queue,
                uint8_t channelCount);

    void getChannels(uint8_t* data);

    bool isNewDataReceived();

    void insertTestDataInQueue();

private:
    DRVSerial&           m_uart;
    HALTimer&            m_timer;
    BinDecIO*            m_address;
    cpp_freertos::Queue* m_queue;
    const uint8_t        m_channelCount;
    uint32_t             m_startTime;
    uint32_t             m_stopTime;

    HALTimer::CallbackFunction timerCallback(HALTimer::CallbackEvent event, HALTimer::TimerChannel channel, uint32_t value, void* This);

    DMXReceiver();  // not allowed
};
