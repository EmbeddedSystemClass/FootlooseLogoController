/*************************************************
 * @file DRVGPIO.cpp
 * @brief GPIO driver interface
 *
 *************************************************/
#include "drv/DRVGPIO.h"

#include <string>
#include "stdint.h"

#include "OSError.h"

#include "stm32f3xx.h"

DRVGPIO::DRVGPIO(GPIO_TypeDef* gpio, uint32_t owner, uint32_t dir, uint32_t polarity)
    : m_gpio(gpio)
    , m_owner(owner)
    , m_dir(dir)
    , m_polarity(polarity)
{
    for (uint32_t pin = 1; pin != 0; pin = ((pin << 1) & GPIO_PIN_MASK))
    {
        if ((m_owner & pin) == pin)
        {
            // If we own this pin, then we can change the settings
            GPIO_InitTypeDef pinSettings;
            pinSettings.Pin   = pin;
            pinSettings.Pull  = GPIO_NOPULL;
            pinSettings.Speed = GPIO_SPEED_FREQ_HIGH;

            if ((dir & pin) == pin)
            {
                pinSettings.Mode = GPIO_MODE_OUTPUT_PP;
            }
            else
            {
                pinSettings.Mode = GPIO_MODE_INPUT;
            }

            HAL_GPIO_Init(m_gpio, &pinSettings);
        }
    }
}

GPIOpin DRVGPIO::getPin(uint32_t pin)
{
    uint32_t selectedPin = (1 << pin);
    if ((m_owner & selectedPin) != selectedPin)
    {
        REPORTFATAL("non owned pin requested")
    }

    bool    polarity  = (m_polarity & (1 << pin));
    GPIOpin returnPin = GPIOpin(m_gpio, pin, true, polarity);
    return returnPin;
}

void DRVGPIO::setAlternateFunction(uint32_t pin, bool output)
{
    uint32_t selectedPin = (1 << pin);
    if ((m_owner & selectedPin) == selectedPin)
    {
        REPORTFATAL(" owned pin requested for alternate function")
    }

    GPIO_InitTypeDef pinSettings;
    pinSettings.Pin   = selectedPin;
    pinSettings.Pull  = GPIO_NOPULL;
    pinSettings.Speed = GPIO_SPEED_FREQ_HIGH;

    if (output)
    {
        pinSettings.Mode = GPIO_MODE_AF_PP;
    }
    else
    {
        //        pinSettings.Mode = GPIO_MODE_AF_INPUT;
        pinSettings.Mode = GPIO_MODE_INPUT;
    }
    HAL_GPIO_Init(m_gpio, &pinSettings);
}

GPIOpin::GPIOpin(GPIO_TypeDef* port, uint8_t pin, bool output, bool polarity)
    : m_port(port)
    , m_polarity(polarity)
    , m_output(output)
    , m_state(GPIO_PIN_RESET)
{
    m_pin = (1 << pin) & GPIO_PIN_MASK;
    if (!m_pin) REPORTFATAL("PIN assigment failed")
}

GPIOpin& GPIOpin::operator=(const bool state)
{
    if (m_output)
    {
        m_state = static_cast<GPIO_PinState>(applyPolarity(state));
        HAL_GPIO_WritePin(m_port, m_pin, m_state);
    }
    else
    {
        m_state = HAL_GPIO_ReadPin(m_port, m_pin);
    }
    return *this;
}

GPIOpin::operator bool()
{
    m_state = HAL_GPIO_ReadPin(m_port, m_pin);
    return applyPolarity(m_state == GPIO_PIN_SET);
}

void GPIOpin::toggle()
{
    if (m_output)
    {
        HAL_GPIO_TogglePin(m_port, m_pin);
    }
}

bool GPIOpin::applyPolarity(bool state)
{
    bool retVal = state;
    if (!m_polarity) retVal = !retVal;

    return retVal;
}
