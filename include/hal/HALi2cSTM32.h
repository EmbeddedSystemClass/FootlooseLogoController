/*************************************************
 * @file HALi2cSTM32.h
 * @brief HAL I2C
 *
 *************************************************/

#pragma once
#include "HALi2c.h"
#include "stdint.h"
#include "stm32f3xx.h"

/*
 * Serial driver interface
 */
class HALI2CSTM32 : public HALI2C
{
public:
    enum CallbackEvent
    {

    };

    HALI2CSTM32(I2C_TypeDef* interface, Frequency speed);

    void write(uint8_t slaveAddress, uint8_t* data, uint8_t dataLength);

protected:
    I2C_HandleTypeDef* m_handle;

private:
};
