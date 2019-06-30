/*************************************************
 * @file GPIOpinSTM32.h
 * @brief GPIO driver interface
 *
 *************************************************/
#pragma once
#include "HALGPio.h"

#include <string>
#include "stdint.h"

#include "stm32f3xx.h"

/**
 * PIN
 */
class GPIOpinSTM32 : public GPIOpin
{
public:
    GPIOpinSTM32(GPIO_TypeDef* port, GPIO_InitTypeDef initSettings, bool polarity);
    virtual ~GPIOpinSTM32();

    virtual GPIOpinSTM32& operator=(const bool);

    virtual operator bool();

    virtual void toggle();

    virtual void setAlternateFunction();
    virtual void setAlternateFunction(uint8_t function);
    virtual void setNormalFunction();

private:
    GPIOpinSTM32();  // not allowed

    bool isOutput();

    const GPIO_InitTypeDef m_initialSettings;
    GPIO_TypeDef*          m_port;
    GPIO_PinState          m_state;
};
