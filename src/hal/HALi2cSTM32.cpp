/*************************************************
 * @file HALi2cSTM32.cpp
 * @brief HAL I2C
 *
 *************************************************/
#include "hal/HALi2cSTM32.h"
#include "os/OSError.h"

#include "stm32f3xx.h"

#include "stddef.h"
#include "stdint.h"
#include "string.h"

extern I2C_HandleTypeDef hi2c1;

HALI2CSTM32::HALI2CSTM32(I2C_TypeDef* interface, Frequency speed)
{
    if (interface == I2C1)
    {
        m_handle = &hi2c1;
    }
    else
    {
        REPORTFATAL("could not map I2C instance")
    }

    m_handle->Instance = interface;

    m_handle->Init.OwnAddress1      = 0;
    m_handle->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    m_handle->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    m_handle->Init.OwnAddress2      = 0;
    m_handle->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    m_handle->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    m_handle->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    switch (speed)
    {
    case HALI2C::Frequency100k:
        m_handle->Init.Timing = 0x2000090E;
        break;
    case HALI2C::Frequency400k:
        REPORTERROR("I2C 400kHz not supported for STM32")
        break;
    case HALI2C::Frequency1000k:
        REPORTERROR("I2C 1MHz not supported for STM32")
        break;
    case HALI2C::Frequency3200k:
        REPORTERROR("I2C 3.2MHz not supported for STM32")
        break;
    default:
        REPORTERROR("unkown speed requested")
        break;
    }
    if (HAL_I2C_Init(m_handle) != HAL_OK)
    {
        REPORTERROR("I2C init failed")
    }
    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(m_handle, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        REPORTERROR("I2C init failed")
    }
    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(m_handle, 0) != HAL_OK)
    {
        REPORTERROR("I2C init failed")
    }

    HAL_I2C_Init(m_handle);
}

void HALI2CSTM32::write(uint8_t slaveAddress, uint8_t* data, uint8_t dataLength)
{
    HAL_I2C_Master_Transmit_IT(m_handle, slaveAddress << 1, data, dataLength);
}
