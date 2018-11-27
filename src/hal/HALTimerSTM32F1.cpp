/*************************************************
 * @file HALTimerSTM32F1.cpp
 * @brief HAL timer
 *
 *************************************************/

#include "hal/HALTimerSTM32F1.h"

#include "OSError.h"
#include "stm32f1xx_hal.h"

#include "stdint.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
HALTimerSTM32F1*         HALTimerSTM32F1::m_this[3] = {0, 0, 0};

HALTimerSTM32F1::HALTimerSTM32F1(TIM_TypeDef* timer, HALTimer::TimerMode mode, uint32_t channel)
    : HALTimer(mode)
    , m_channel(m_channel)
{
    if (timer == TIM2)
    {
        m_handle  = &htim2;
        m_this[0] = this;
    }
    else if (timer == TIM2)
    {
        //        m_handle  = &htim3;
        //        m_this[1] = this;
    }
    else if (timer == TIM2)
    {
        //        m_handle  = &htim4;
        //        m_this[2] = this;
    }
    else
    {
        REPORTFATAL("unkown timer")
    }

    TIM_ClockConfigTypeDef  sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_IC_InitTypeDef      sConfigIC;

    m_handle->Instance               = timer;
    m_handle->Init.Prescaler         = 0;
    m_handle->Init.CounterMode       = TIM_COUNTERMODE_UP;
    m_handle->Init.Period            = 0;
    m_handle->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    m_handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(m_handle) != HAL_OK)
    {
        REPORTFATAL("HAL init failed")
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    sClockSourceConfig.ClockFilter
	sClockSourceConfig.ClockPolarity
	sClockSourceConfig.ClockPrescaler
    if (HAL_TIM_ConfigClockSource(m_handle, &sClockSourceConfig) != HAL_OK)
    {
        REPORTFATAL("HAL init failed")
    }

    if (HAL_TIM_IC_Init(m_handle) != HAL_OK)
    {
        REPORTFATAL("HAL init failed")
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(m_handle, &sMasterConfig) != HAL_OK)
    {
        REPORTFATAL("HAL init failed")
    }

    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(m_handle, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
    {
        REPORTFATAL("HAL init failed")
    }
    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(m_handle, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
    {
        REPORTFATAL("HAL init failed")
    }
    __HAL_TIM_ENABLE_IT(m_handle, TIM_IT_TRIGGER);

    //    HAL_NVIC_SetPriority(TIM2_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void HALTimerSTM32F1::start()
{
    switch (m_mode)
    {
    case TimerInputCapture:

        for (uint8_t channel = TimerChannel1; channel <= TimerChannel4; channel = (channel << 1))
        {
            HAL_TIM_IC_Start_IT(m_handle, static_cast<TimerChannel>(channel));
        }
        break;
    default:
        REPORTFATAL("Timer mode not implemented")
        break;
    }
    //    HAL_TIM_Base_Start(m_handle);
}

void HALTimerSTM32F1::stop()
{
    switch (m_mode)
    {
    case TimerInputCapture:
        for (uint8_t channel = TimerChannel1; channel <= TimerChannel4; channel = (channel << 1))
        {
            HAL_TIM_IC_Stop_IT(m_handle, static_cast<TimerChannel>(channel));
        }

        break;

    default:
        REPORTFATAL("Timer mode not implemented")
        break;
    }
    //    HAL_TIM_Base_Stop(m_handle);
}

void HALTimerSTM32F1::setInterval(uint32_t us) {}

void HALTimerSTM32F1::callback(uint32_t timer)
{
    HALTimerSTM32F1* This = m_this[timer];

    if (__HAL_TIM_GET_FLAG(This->m_handle, TIM_FLAG_CC1) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(This->m_handle, TIM_IT_CC1) != RESET)
        {

            This->callCallback(HALTimer::CallbackTrigger, HALTimer::TimerChannel1, This->m_handle->Instance->CCR1);
            __HAL_TIM_CLEAR_IT(This->m_handle, TIM_FLAG_CC1);
        }
    }
    if (__HAL_TIM_GET_FLAG(This->m_handle, TIM_FLAG_CC2) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(This->m_handle, TIM_IT_CC2) != RESET)
        {
            This->callCallback(HALTimer::CallbackTrigger, HALTimer::TimerChannel2, This->m_handle->Instance->CCR2);
            __HAL_TIM_CLEAR_IT(This->m_handle, TIM_FLAG_CC2);
        }
    }
    if (__HAL_TIM_GET_FLAG(This->m_handle, TIM_FLAG_CC3) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(This->m_handle, TIM_IT_CC3) != RESET)
        {

            This->callCallback(HALTimer::CallbackTrigger, HALTimer::TimerChannel3, This->m_handle->Instance->CCR3);
            __HAL_TIM_CLEAR_IT(This->m_handle, TIM_FLAG_CC3);
        }
    }
    if (__HAL_TIM_GET_FLAG(This->m_handle, TIM_FLAG_CC4) != RESET)
    {
        if (__HAL_TIM_GET_IT_SOURCE(This->m_handle, TIM_IT_CC4) != RESET)
        {

            This->callCallback(HALTimer::CallbackTrigger, HALTimer::TimerChannel4, This->m_handle->Instance->CCR4);
            __HAL_TIM_CLEAR_IT(This->m_handle, TIM_FLAG_CC4);
        }
    }

    if (__HAL_TIM_GET_FLAG(This->m_handle, TIM_FLAG_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(This->m_handle, TIM_FLAG_UPDATE);
    }

    if (__HAL_TIM_GET_FLAG(This->m_handle, TIM_FLAG_CC1OF) != RESET)
    {
        __HAL_TIM_CLEAR_IT(This->m_handle, TIM_FLAG_CC1OF);
    }
    if (__HAL_TIM_GET_FLAG(This->m_handle, TIM_FLAG_CC2OF) != RESET)
    {
        __HAL_TIM_CLEAR_IT(This->m_handle, TIM_FLAG_CC2OF);
    }
    if (__HAL_TIM_GET_FLAG(This->m_handle, TIM_FLAG_CC3OF) != RESET)
    {
        __HAL_TIM_CLEAR_IT(This->m_handle, TIM_FLAG_CC3OF);
    }
    if (__HAL_TIM_GET_FLAG(This->m_handle, TIM_FLAG_CC4OF) != RESET)
    {
        __HAL_TIM_CLEAR_IT(This->m_handle, TIM_FLAG_CC4OF);
    }
}

void TIM2_IRQHandler(void) { HALTimerSTM32F1::callback(0); }
void TIM3_IRQHandler(void) { HALTimerSTM32F1::callback(1); }
void TIM4_IRQHandler(void) { HALTimerSTM32F1::callback(2); }
