/*************************************************
 * @file DRVGPIO.h
 * @brief GPIO driver interface
 *
 *************************************************/
#pragma once

#include <string>
#include "stdint.h"

#include "stm32f1xx.h"

/**
 * PIN
 */
class GPIOpin
{
public:
    GPIOpin(GPIO_TypeDef*, uint8_t pin, bool output, bool polarity);

    GPIOpin& operator=(const bool);

    operator bool();

    void toggle();

private:
    GPIOpin();  // not allowed

    bool applyPolarity(bool state);

    GPIO_TypeDef* m_port;
    uint16_t      m_pin;
    bool          m_output;
    GPIO_PinState m_state;
    bool          m_polarity;  //!< 1 means normal
};

/*
 * Serial driver interface
 */
class DRVGPIO
{
public:
    DRVGPIO(GPIO_TypeDef* gpio, uint32_t owner, uint32_t dir, uint32_t polarity);

    GPIOpin getPin(uint32_t pin);

private:
    DRVGPIO();  // Not allowed

    GPIO_TypeDef*  m_gpio;
    const uint32_t m_owner;
    const uint32_t m_polarity;
    uint32_t       m_dir;
};
