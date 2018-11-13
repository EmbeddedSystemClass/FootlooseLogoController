/*************************************************
 * @file HALUartSTM32F1.cpp
 * @brief HAL uart
 *
 *************************************************/

#include "hal/HALUartSTM32F1.h"

#include "stm32f1xx.h"

HALUartSTM32F1::HALUartSTM32F1(USART_TypeDef* uart, uint32_t baudRate, UartMode mode)
{
    m_handle.Instance = uart;

    m_handle.Init.BaudRate     = baudRate;
    m_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    m_handle.Init.Mode         = mode;
    m_handle.Init.OverSampling = UART_OVERSAMPLING_16;
    m_handle.Init.Parity       = UART_PARITY_NONE;
    m_handle.Init.StopBits     = UART_STOPBITS_2;
    m_handle.Init.WordLength   = UART_WORDLENGTH_8B;

    HAL_UART_Init(m_handle);
}

void HALUartSTM32F1::open() {}

void HALUartSTM32F1::close() {}

void HALUartSTM32F1::sendByte(uint8_t) {}

uint32_t HALUartSTM32F1::readByte() {}
