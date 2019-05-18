/*************************************************
 * @file I2CDevice.cpp
 * @brief I2CDevice helper class
 *
 *************************************************/

#include "DRV/I2CDevice.h"

#include "HAL/HALi2c.h"

I2CDevice::I2CDevice(HALI2C& i2cHal, uint8_t deviceAddress)
    : m_hal(i2cHal)
    , m_address(deviceAddress)
{
}

void I2CDevice::writeToDevice(uint8_t* data, uint8_t dataLength) { m_hal.write(m_address, data, dataLength); }
