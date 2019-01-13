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
    //    GPIOpin(GPIO_TypeDef*, uint8_t pin, bool output, bool polarity);
    GPIOpin(GPIO_TypeDef* port, GPIO_InitTypeDef initSettings, bool polarity);

    GPIOpin& operator=(const bool);

    operator bool();

    void toggle();

    void setAlternateFunction();
    void setNormalFunction();

private:
    GPIOpin();  // not allowed

    bool applyPolarity(bool state);

    bool isOutput();

    const GPIO_InitTypeDef m_initialSettings;
    GPIO_TypeDef*          m_port;
    GPIO_PinState          m_state;
    bool                   m_polarity;  //!< 1 means normal
};

/*
 * Serial driver interface
 */
class DRVGPIO
{
public:
    DRVGPIO(GPIO_TypeDef* gpio, uint32_t owner, uint32_t dir, uint32_t polarity);

    GPIOpin getPin(uint32_t pin);

    void setAlternateFunction(uint32_t pin, bool output = 0);

private:
    DRVGPIO();  // Not allowed

    GPIO_TypeDef*  m_gpio;
    const uint32_t m_owner;
    const uint32_t m_polarity;
    uint32_t       m_dir;
};
