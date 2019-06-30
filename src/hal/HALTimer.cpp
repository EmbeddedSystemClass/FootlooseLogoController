/*************************************************
 * @file HALTimer.cpp
 * @brief HAL timer
 *
 *************************************************/
#include "hal/HALTimer.h"

#include "stddef.h"
#include "stdint.h"

HALTimer::HALTimer(TimerMode mode, TimerChannel channels)
    : m_callback(NULL)
    , m_callbackParamerter(NULL)
    , m_mode(mode)
    , m_channels(channels)
{
}

HALTimer::~HALTimer() {}

void HALTimer::setCallback(CallbackFunction function, void* parameter)
{
    m_callback           = function;
    m_callbackParamerter = parameter;
}

void HALTimer::callCallback(CallbackEvent event, TimerChannel channel, uint32_t value)
{
    if (m_callback != NULL)
    {
        m_callback(event, channel, value, m_callbackParamerter);
    }
}

uint32_t HALTimer::getTimerChannel(TimerChannel channel) { return static_cast<uint32_t>(channel); }
