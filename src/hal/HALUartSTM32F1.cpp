/*************************************************
 * @file HALUartSTM32F1.cpp
 * @brief HAL uart
 *
 *************************************************/

#include "hal/HALUartSTM32F1.h"
#include "hal/HALUart.h"

#include "OSError.h"

#include "stm32f3xx.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
HALUartSTM32F1 *          HALUartSTM32F1::m_this[3] = {0};

HALUartSTM32F1::HALUartSTM32F1(USART_TypeDef *uart, uint32_t baudRate, UartMode mode)
    : m_callbackFunction(NULL)
    , m_callbackParameters(NULL)
{

    if (uart == USART1)
    {
        m_handle  = &huart1;
        m_this[0] = this;
        __HAL_RCC_USART1_CLK_ENABLE();
        HAL_NVIC_SetPriority(USART1_IRQn, 15, 14);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (uart == USART2)
    {
        m_handle  = &huart2;
        m_this[1] = this;
        __HAL_RCC_USART2_CLK_ENABLE();
        HAL_NVIC_SetPriority(USART2_IRQn, 15, 14);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    else if (uart == USART3)
    {
        m_handle  = &huart3;
        m_this[2] = this;
    }
    else
    {
        REPORTFATAL("uart unkonwn")
    }

    m_handle->Instance = uart;

    m_handle->Init.BaudRate     = baudRate;
    m_handle->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    m_handle->Init.Mode         = mode;
    m_handle->Init.OverSampling = UART_OVERSAMPLING_16;
    m_handle->Init.Parity       = UART_PARITY_NONE;
    m_handle->Init.StopBits     = UART_STOPBITS_2;
    //    m_handle->Init.StopBits   = UART_STOPBITS_1;
    m_handle->Init.WordLength = UART_WORDLENGTH_8B;

    HAL_UART_Init(m_handle);

    //    __HAL_UART_ENABLE(m_handle);
}

HALUartSTM32F1::~HALUartSTM32F1() {}

void HALUartSTM32F1::open() {}

void HALUartSTM32F1::close() {}

void HALUartSTM32F1::send(uint8_t *data, uint8_t length, uint32_t timeout)
{
    if (timeout > 0)
    {
        HAL_UART_Transmit(m_handle, data, length, timeout);
    }
    else
    {
        HAL_UART_Transmit_IT(m_handle, data, length);
    }
}

void HALUartSTM32F1::receive(uint8_t *data, uint16_t bufferLength, uint32_t timeout)
{
    if (timeout > 0)
    {
        HAL_UART_Receive(m_handle, data, bufferLength, timeout);
    }
    else
    {
        HAL_UART_Receive_IT(m_handle, data, bufferLength);
    }
}

void HALUartSTM32F1::sendByte(uint8_t) {}

uint32_t HALUartSTM32F1::readByte() { return 0; }

void HALUartSTM32F1::callBack(UART_HandleTypeDef *uart, CallBackType type)
{
    HALUartSTM32F1 *This = NULL;
    // finding instance
    if (uart == &huart1)
    {
        This = m_this[0];
    }
    else if (uart == &huart2)
    {
        This = m_this[1];
    }
    else if (uart == &huart3)
    {
        This = m_this[2];
    }
    if (This->m_callbackFunction != NULL)
    {
        This->m_callbackFunction(getCallback(type), This->m_callbackParameters);
    }
}

HALUart::CallBack HALUartSTM32F1::getCallback(CallBackType callback)
{
    HALUart::CallBack retVal = HALUart::Error;

    switch (callback)
    {
    case HAL_UART_AbortCpltCallback:
    case HAL_UART_TxHalfCpltCallback:
    case HAL_UART_TxCpltCallback:
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

void HALUartSTM32F1::registerCallback(CallbackFunction f, CallBack type, void *parameter)
{
    (void)type;
    m_callbackFunction   = f;
    m_callbackParameters = parameter;
}

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
