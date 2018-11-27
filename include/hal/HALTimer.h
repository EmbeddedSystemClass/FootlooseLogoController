/*************************************************
 * @file HALTimer.h
 * @brief HAL timer
 *
 *************************************************/

#pragma once

#include "stdint.h"

/*
 * Serial driver interface
 */
class HALTimer
{
public:
    enum CallbackEvent
    {
        CallbackOverflow,
        CallbackUnderflow,
        CallbackTrigger
    };

    enum TimerMode
    {
        TimerDountUp,
        TimerCountDown,
        TimerInputCapture,
        TimerPWM
    };

    enum TimerChannel
    {
        TimerChannel1   = 0x01,
        TimerChannel2   = 0x02,
        TimerChannel3   = 0x04,
        TimerChannel4   = 0x08,
        TimerChannelAll = 0x0F
    };

    HALTimer(TimerMode mode, TimerChannel channel = TimerChannel1);

    virtual ~HALTimer();

    typedef void (*CallbackFunction)(CallbackEvent, TimerChannel, uint32_t, void*);

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void setInterval(uint32_t us) = 0;

    virtual void setCallback(CallbackFunction function, void* parameter);

protected:
    CallbackFunction m_callback;
    void*            m_callbackParamerter;
    const TimerMode  m_mode;
    TimerChannel     m_channels;

    void callCallback(CallbackEvent event, TimerChannel channel, uint32_t value);

private:
};
