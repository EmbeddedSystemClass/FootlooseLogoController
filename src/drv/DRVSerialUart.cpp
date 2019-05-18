/*************************************************
 * @file DRVSerialUsb.h
 * @brief Serial CDC driver interface
 *
 *************************************************/

#include "stdint.h"

#include "drv/DRVSerialUart.h"

DRVSerialUart::DRVSerialUart(HALUart& hal)
    : DRVSerial(true)
    , m_HAL(hal)
{
}

DRVSerialUart::~DRVSerialUart() {}

void DRVSerialUart::sendByte(uint8_t data)
{  // m_HAL.sendByte(data);
    (void)data;
}

uint32_t DRVSerialUart::readByte()
{  // return m_HAL.readByte();
    return 0;
}

uint32_t DRVSerialUart::readBuffer(uint8_t* buf, uint32_t bufferSize)
{
    m_HAL.receive(buf, bufferSize);
    return 0;
}
uint32_t DRVSerialUart::writeBuffer(uint8_t* buf, uint32_t bufferSize)
{
    m_HAL.send(buf, bufferSize);
    return 0;
}

void DRVSerialUart::registerCallback(HALUart::CallbackFunction f, HALUart::CallBack type, void* parameter)
{
    m_HAL.registerCallback(f, type, parameter);
}
