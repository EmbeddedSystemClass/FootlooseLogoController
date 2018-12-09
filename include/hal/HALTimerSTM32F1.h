/*************************************************
 * @file HALTimerSTM32F1.h
 * @brief HAL timer
 *
 *************************************************/

#pragma once
#include "HALTimer.h"

#include "stm32f1xx_hal.h"

#include "stdint.h"

/*
 * Serial driver interface
 */
class HALTimerSTM32F1 : public HALTimer
{
public:
    HALTimerSTM32F1(TIM_TypeDef* timer, HALTimer::TimerMode mode, uint32_t channel);

    virtual void start();

    virtual void stop();

    virtual void setInterval(uint32_t us);

    static void callback(uint32_t timer);

private:
    TIM_HandleTypeDef*      m_handle;
    uint32_t                m_channel;
    static HALTimerSTM32F1* m_this[3];
};

extern "C"
{
    void TIM2_IRQHandler(void);
    void TIM3_IRQHandler(void);
    void TIM4_IRQHandler(void);
}
