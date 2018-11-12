/*************************************************
 * @file HALUartSTM32F1.h
 * @brief HAL uart
 *
 *************************************************/
#pragma once

#include "HALUart.h"
#include "HALUartSTM32F1.h"

#include "stm32f1xx.h"

#include "stdint.h"

/*
 * Serial driver interface
 */
class HALUartSTM32F1 : public HALUart
{
public:
    HALUartSTM32F1(UART_HandleTypeDef* uart);

    virtual void open(uint32_t baudRate, uint8_t dataBits, uint8_t stopBits);

    virtual void close();

    virtual void sendByte(uint8_t);

    virtual uint32_t readByte();

private:
};
