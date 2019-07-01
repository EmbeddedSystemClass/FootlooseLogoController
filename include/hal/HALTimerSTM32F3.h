/*************************************************
 * @file HALTimerSTM32F3.h
 * @brief HAL timer
 *
 *************************************************/

#pragma once
#include "HALTimer.h"
#include "HALTimerSTM32F1.h"

#include "stm32f3xx_hal.h"

#include "stdint.h"

/*
 * Serial driver interface
 */
class HALTimerSTM32F3 : public HALTimerSTM32F1
{
public:
    HALTimerSTM32F3(TIM_TypeDef* timer, HALTimer::TimerMode mode, uint32_t channel);

protected:
    virtual uint32_t getTimerChannel(TimerChannel channel);

private:
};
