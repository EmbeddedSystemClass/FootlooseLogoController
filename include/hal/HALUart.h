/*************************************************
 * @file HALUart.h
 * @brief HAL uart
 *
 *************************************************/

#pragma once

#include "stdint.h"

/*
 * Serial driver interface
 */
class HALUart
{
public:
    enum UartState
    {
        StateReset,
        StateReady,
        StateBusy,
        StateTimeout,
        StateError
    };

    //    HALUart();
    //    virtual ~HALUart();

    virtual void send(uint8_t* data, uint8_t length, bool blocking = false) = 0;

    virtual void receive(uint8_t* data, uint8_t bufferLength, bool blocking = false) = 0;

    /**
     * get generic status
     */
    UartState getState();

    /**
     * Get specific error code
     */
    uint32_t getErrorCode();

private:
};
