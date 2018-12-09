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

    enum CallBack
    {
        Send,
        Received,
        Error
    };

    typedef void (*CallbackFunction)(CallBack, void* parameter);

    //    HALUart();
    //    virtual ~HALUart();

    virtual void send(uint8_t* data, uint8_t length, uint32_t timeout = 0) = 0;

    virtual void receive(uint8_t* data, uint16_t bufferLength, uint32_t timeout = 0) = 0;

    virtual void registerCallback(CallbackFunction f, CallBack type, void* parameter) = 0;

    /**
     * get generic status
     */
    UartState getState();

    /**
     * Get specific error code
     */
    uint32_t getErrorCode();

protected:
private:
};
