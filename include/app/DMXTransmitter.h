/*************************************************
 * @file DMXTransmitter.h
 * @brief DMX transmitter interface
 *
 *************************************************/
#pragma once

#include <string>
#include <vector>
#include "stdint.h"

#include "TaskState.h"
#include "drv/DRVGPIO.h"
#include "drv/DRVSerial.h"
#include "queue.hpp"

/**
 * DMXTransmitter
 */
class DMXTransmitter : public TaskState, public cpp_freertos::Thread
{
public:
    enum TransmitterState
    {
        StateInit,
        StateIdle,
        StateBreak,
        StateSending,
    };

    struct DMXQueueItem
    {
        uint16_t             startAddress;
        uint8_t              channelCount;
        static const uint8_t channelDataLength = 10;
        uint8_t              channeldata[channelDataLength];
    };

    DMXTransmitter(TaskHandle_t taskToNotify, uint8_t ID, GPIOpin& uartPin, DRVSerial& uart, uint8_t lastChannel);
    DMXTransmitter(TaskHandle_t taskToNotify, uint8_t ID, GPIOpin& uartPin, DRVSerial& uart, uint8_t lastChannel, cpp_freertos::Queue* queue);

    virtual void Run();

    void writeChannels(uint8_t* data, uint16_t startAddress, uint8_t channelCount);

private:
    DRVSerial&           m_uart;
    GPIOpin&             m_uartPin;
    cpp_freertos::Queue* m_queue;
    const uint8_t        m_channelCount;
    TransmitterState     m_state;
    uint8_t*             m_dmxBuffer;
    uint32_t             m_lastSendTime;
    bool                 m_dataWasUpdated;

    static void uartCallback(HALUart::CallBack type, void* This);

    DMXTransmitter();  // not allowed
};
