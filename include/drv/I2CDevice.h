/*************************************************
 * @file I2CDevice.h
 * @brief I2CDevice helper class
 *
 *************************************************/
#pragma once

#include "HAL/HALi2c.h"

class I2CDevice
{
public:
    I2CDevice(HALI2C& i2cHal, uint8_t deviceAddress);

    void writeToDevice(uint8_t* data, uint8_t dataLength);

    virtual bool initDevice() = 0;

protected:
    HALI2C& m_hal;
    uint8_t m_address;
};
