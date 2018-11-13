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
    enum UartMode
    {
        UartModeRx   = UART_MODE_RX,
        UartModeTx   = UART_MODE_TX,
        UartModeRxTx = UART_MODE_TX_RX
    };

    HALUartSTM32F1(USART_TypeDef* uart, uint32_t baudRate, UartMode mode);

    virtual void open();

    virtual void close();

    virtual void sendByte(uint8_t);

    virtual uint32_t readByte();

private:
    UART_HandleTypeDef m_handle;
};
