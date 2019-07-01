/*************************************************
 * @file HALTimerSTM32F3.cpp
 * @brief HAL timer
 *
 *************************************************/

#include "hal/HALTimerSTM32F3.h"

#include "OSError.h"
#include "stm32f3xx_hal_tim.h"

#include "stdint.h"

HALTimerSTM32F3::HALTimerSTM32F3(TIM_TypeDef* timer, HALTimer::TimerMode mode, uint32_t channel)
    : HALTimerSTM32F1(timer, mode, channel)
{
}

uint32_t HALTimerSTM32F3::getTimerChannel(TimerChannel channel)
{
    uint32_t val = 0;
    switch (channel)
    {
    case TimerChannel1:
        val = TIM_CHANNEL_1;
        break;
    case TimerChannel2:
        val = TIM_CHANNEL_2;
        break;
    case TimerChannel3:
        val = TIM_CHANNEL_3;
        break;
    case TimerChannel4:
        val = TIM_CHANNEL_4;
        break;
    case TimerChannelAll:
        val = TIM_CHANNEL_ALL;
        break;
    default:
        REPORTERROR("Unsuported timer channel")
        break;
    }

    return val;
}
