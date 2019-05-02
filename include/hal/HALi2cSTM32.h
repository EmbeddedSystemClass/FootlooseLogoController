/*************************************************
 * @file HALi2cSTM32.h
 * @brief HAL I2C
 *
 *************************************************/

#pragma once
#include "HALi2c.h"
#include "semaphore.hpp"
#include "stdint.h"
#include "stm32f3xx.h"

/*
 * Serial driver interface
 */
class HALI2CSTM32 : public HALI2C, private cpp_freertos::BinarySemaphore
{
public:
    enum CallbackEvent
    {
        HAL_I2C_Complete
    };

    HALI2CSTM32(I2C_TypeDef* interface, Frequency speed);

    void write(uint8_t slaveAddress, uint8_t* data, uint8_t dataLength);

    static void callBack(I2C_HandleTypeDef* uart, CallbackEvent type);

protected:
    I2C_HandleTypeDef* m_handle;

private:
    static HALI2CSTM32* m_this[2];
};
