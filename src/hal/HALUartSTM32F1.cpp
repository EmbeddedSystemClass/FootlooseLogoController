/*************************************************
 * @file HALUartSTM32F1.cpp
 * @brief HAL uart
 *
 *************************************************/

#include "hal/HALUartSTM32F1.h"
#include "hal/HALUart.h"

#include "stm32f1xx.h"

HALUartSTM32F1::HALUartSTM32F1(USART_TypeDef *uart, uint32_t baudRate, UartMode mode)
{
    m_handle.Instance = uart;

    m_handle.Init.BaudRate     = baudRate;
    m_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    m_handle.Init.Mode         = mode;
    m_handle.Init.OverSampling = UART_OVERSAMPLING_16;
    m_handle.Init.Parity       = UART_PARITY_NONE;
    //    m_handle.Init.StopBits     = UART_STOPBITS_2;
    m_handle.Init.StopBits   = UART_STOPBITS_1;
    m_handle.Init.WordLength = UART_WORDLENGTH_8B;

    HAL_UART_Init(&m_handle);

    __HAL_UART_ENABLE(&m_handle);
}

void HALUartSTM32F1::open() {}

void HALUartSTM32F1::close() {}

void HALUartSTM32F1::send(uint8_t *data, uint8_t length, uint32_t timeout)
{
    if (timeout > 0)
    {
        HAL_UART_Transmit(&m_handle, data, length, timeout);
    }
    else
    {
        HAL_UART_Transmit_IT(&m_handle, data, length);
    }
}

void HALUartSTM32F1::receive(uint8_t *data, uint8_t bufferLength, uint32_t timeout)
{
    if (timeout > 0)
    {
        HAL_UART_Receive(&m_handle, data, bufferLength, timeout);
    }
    else
    {
        HAL_UART_Receive_IT(&m_handle, data, bufferLength);
    }
}

void HALUartSTM32F1::sendByte(uint8_t) {}

uint32_t HALUartSTM32F1::readByte() {}

void HALUartSTM32F1::callBack(UART_HandleTypeDef *uart, CallBackType type) {}

HALUart::CallBack HALUartSTM32F1::getCallback(CallBackType callback)
{
    HALUart::CallBack retVal = HALUart::Error;

    switch (callback)
    {
    case HAL_UART_AbortCpltCallback:
    case HAL_UART_TxHalfCpltCallback:
        retVal = HALUart::Send;
        break;
    case HAL_UART_RxCpltCallback:
    case HAL_UART_RxHalfCpltCallback:
        retVal = HALUart::Received;
        break;
    case HAL_UART_ErrorCallback:
    case HAL_UART_AbortTransmitCpltCallback:
    case HAL_UART_AbortReceiveCpltCallback:
    default:
        retVal = HALUart::Error;
        break;
    }

    return retVal;
}

void HALUartSTM32F1::registerCallback(CallbackFunction f, CallBack type) {}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) { HALUartSTM32F1::callBack(huart, HALUartSTM32F1::HAL_UART_TxCpltCallback); }

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart) { HALUartSTM32F1::callBack(huart, HALUartSTM32F1::HAL_UART_TxHalfCpltCallback); }

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) { HALUartSTM32F1::callBack(huart, HALUartSTM32F1::HAL_UART_RxCpltCallback); }

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) { HALUartSTM32F1::callBack(huart, HALUartSTM32F1::HAL_UART_RxHalfCpltCallback); }

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) { HALUartSTM32F1::callBack(huart, HALUartSTM32F1::HAL_UART_ErrorCallback); }

void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart) { HALUartSTM32F1::callBack(huart, HALUartSTM32F1::HAL_UART_AbortCpltCallback); }

void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
{
    HALUartSTM32F1::callBack(huart, HALUartSTM32F1::HAL_UART_AbortTransmitCpltCallback);
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
    HALUartSTM32F1::callBack(huart, HALUartSTM32F1::HAL_UART_AbortReceiveCpltCallback);
}
