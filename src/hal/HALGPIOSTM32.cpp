/*************************************************
 * @file GPIOpinSTM32.h
 * @brief GPIO driver interface
 *
 *************************************************/
#include "hal/HALGPIOSTM32.h"
#include "hal/HALGPIO.h"

#include <string>
#include "stdint.h"

#include "stm32f3xx.h"

GPIOpinSTM32::GPIOpinSTM32(GPIO_TypeDef* port, GPIO_InitTypeDef initSettings, bool polarity)
    : GPIOpin(polarity)
    , m_initialSettings(initSettings)
    , m_port(port)
    , m_state(GPIO_PIN_RESET)
{
}

void GPIOpinSTM32::setAlternateFunction()
{
    GPIO_InitTypeDef pinSettings = m_initialSettings;

    if (isOutput())
    {
        pinSettings.Mode = GPIO_MODE_AF_PP;
    }
    else
    {
        //        pinSettings.Mode = GPIO_MODE_AF_INPUT;
        pinSettings.Mode = GPIO_MODE_INPUT;
    }

    HAL_GPIO_Init(m_port, &pinSettings);
}

void GPIOpinSTM32::setNormalFunction()
{
    GPIO_InitTypeDef pinSettings = m_initialSettings;

    if (isOutput())
    {
        pinSettings.Mode = GPIO_MODE_OUTPUT_PP;
    }
    else
    {
        pinSettings.Mode = GPIO_MODE_INPUT;
    }

    HAL_GPIO_Init(m_port, &pinSettings);
}

GPIOpinSTM32& GPIOpinSTM32::operator=(const bool state)
{
    if (isOutput())
    {
        m_state = static_cast<GPIO_PinState>(applyPolarity(state));
        HAL_GPIO_WritePin(m_port, m_initialSettings.Pin, m_state);
    }
    else
    {
        m_state = HAL_GPIO_ReadPin(m_port, m_initialSettings.Pin);
    }
    return *this;
}

GPIOpinSTM32::operator bool()
{
    m_state = HAL_GPIO_ReadPin(m_port, m_initialSettings.Pin);
    return applyPolarity(m_state == GPIO_PIN_SET);
}

void GPIOpinSTM32::toggle()
{
    if (isOutput())
    {
        HAL_GPIO_TogglePin(m_port, m_initialSettings.Pin);
    }
}

bool GPIOpinSTM32::isOutput()
{
    bool retVal = false;

    if (m_initialSettings.Mode == GPIO_MODE_OUTPUT_PP || m_initialSettings.Mode == GPIO_MODE_OUTPUT_OD || m_initialSettings.Mode == GPIO_MODE_AF_PP ||
        m_initialSettings.Mode == GPIO_MODE_AF_OD)
    {
        retVal = true;
    }

    return retVal;
}
