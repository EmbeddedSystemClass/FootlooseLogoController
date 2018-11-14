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
#include "queue.hpp"

/**
 * DMXReceiver
 */
class DMXReceiver : public TaskState
{
public:
    DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, uint8_t channelCount);
    DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, BinDecIO* dmxAddress, uint8_t channelCount);
    DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, BinDecIO* dmxAddress, cpp_freertos::Queue* queue, uint8_t channelCount);

    void getChannels(uint8_t* data);

    bool isNewDataReceived();

    void insertTestDataInQueue();

private:
    DRVSerial&           m_uart;
    BinDecIO*            m_address;
    cpp_freertos::Queue* m_queue;
    const uint8_t        m_channelCount;

    DMXReceiver();  // not allowed
};
