/*************************************************
 * @file HALi2c.h
 * @brief HAL I2C
 *
 *************************************************/

#pragma once

#include "stdint.h"

/*
 * Serial driver interface
 */
class HALI2C
{
public:
    enum CallbackEvent
    {

    };

    enum Frequency
    {
        Frequency100k,
        Frequency400k,
        Frequency1000k,
        Frequency3200k,
    };

    HALI2C();

    typedef void (*CallbackFunction)(CallbackEvent, uint32_t, void*);

    virtual void setCallback(CallbackFunction function, void* parameter);

    virtual void write(uint8_t slaveAddress, uint8_t* data, uint8_t dataLength) = 0;

protected:
    CallbackFunction m_callback;
    void*            m_callbackParamerter;

    void callCallback(CallbackEvent event, uint32_t value);

private:
};
