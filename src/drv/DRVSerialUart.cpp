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

void DRVSerialUart::sendByte(uint8_t data)
{  // m_HAL.sendByte(data);
}

uint32_t DRVSerialUart::readByte()
{  // return m_HAL.readByte();
}

uint32_t DRVSerialUart::readBuffer(uint8_t* buf, uint32_t bufferSize) { m_HAL.receive(buf, bufferSize); }
uint32_t DRVSerialUart::writeBuffer(uint8_t* buf, uint32_t bufferSize) { m_HAL.send(buf, bufferSize); }

void DRVSerialUart::registerCallback(HALUart::CallbackFunction f, HALUart::CallBack type, void* parameter)
{
    m_HAL.registerCallback(f, type, parameter);
}
