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

protected:
    void sendByte(uint8_t);

private:
    HALUart& m_HAL;
};
