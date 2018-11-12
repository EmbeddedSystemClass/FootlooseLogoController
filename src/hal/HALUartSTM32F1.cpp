/*************************************************
 * @file HALUartSTM32F1.cpp
 * @brief HAL uart
 *
 *************************************************/

#include "hal/HALUartSTM32F1.h"

#include "stm32f1xx.h"

HALUartSTM32F1::HALUartSTM32F1(UART_HandleTypeDef* uart) { HAL_UART_Init(uart); }

void HALUartSTM32F1::open(uint32_t baudRate, uint8_t dataBits, uint8_t stopBits) {}

void HALUartSTM32F1::close() {}

void HALUartSTM32F1::sendByte(uint8_t) {}

uint32_t HALUartSTM32F1::readByte() {}
