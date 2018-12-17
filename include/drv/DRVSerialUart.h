/*************************************************
 * @file DRVSerialUart.h
 * @brief Serial CDC driver interface
 *
 *************************************************/
#pragma once

#include "stdint.h"

#include "DRVSerial.h"

#include "hal/HALUart.h"

/*
 * Serial driver interface
 */
class DRVSerialUart : public DRVSerial
{
public:
    DRVSerialUart(HALUart& hal);
    ~DRVSerialUart();

    uint32_t readByte();

    uint32_t readBuffer(uint8_t* buf, uint32_t bufferSize);
    uint32_t writeBuffer(uint8_t* buf, uint32_t bufferSize);

    void registerCallback(HALUart::CallbackFunction f, HALUart::CallBack type, void* parameter);

protected:
    void sendByte(uint8_t);

private:
    HALUart& m_HAL;
};
