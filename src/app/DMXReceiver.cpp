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

DMXReceiver::DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, uint8_t channelCount)
    : TaskState(taskToNotify, ID)
    , m_uart(uart)
    , m_address(NULL)
    , m_queue(NULL)
    , m_channelCount(channelCount)
{
}
DMXReceiver::DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, BinDecIO* dmxAddress, uint8_t channelCount)
    : TaskState(taskToNotify, ID)
    , m_uart(uart)
    , m_address(dmxAddress)
    , m_queue(NULL)
    , m_channelCount(channelCount)
{
}
DMXReceiver::DMXReceiver(TaskHandle_t taskToNotify, uint8_t ID, DRVSerial& uart, BinDecIO* dmxAddress, cpp_freertos::Queue* queue,
                         uint8_t channelCount)
    : TaskState(taskToNotify, ID)
    , m_uart(uart)
    , m_address(dmxAddress)
    , m_queue(queue)
    , m_channelCount(channelCount)
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
