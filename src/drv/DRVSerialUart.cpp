/*************************************************
 * @file DRVSerialUsb.h
 * @brief Serial CDC driver interface
 *
 *************************************************/

#include "stdint.h"

#include "drv/DRVSerialUart.h"

DRVSerialUart::DRVSerialUart(HALUart& hal)
    : m_HAL(hal)
    , DRVSerial(true)
{
}

DRVSerialUart::~DRVSerialUart() {}

void DRVSerialUart::sendByte(uint8_t data) { m_HAL.sendByte(data); }

uint32_t DRVSerialUart::readByte() { return m_HAL.readByte(); }
