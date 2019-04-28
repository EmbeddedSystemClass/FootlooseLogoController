/*************************************************
 * @file OSTimer.h
 * @brief OS timer
 *
 *************************************************/
#pragma once

#include <string>
#include "stdint.h"

#include "util/Notify.h"

#include "timer.hpp"

/*
 * Serial driver interface
 */
class OSTimer : public cpp_freertos::Timer, public Notify<bool>
{
public:
    OSTimer();
    //    OSTimer(const char* const TimerName, TickType_t PeriodInTicks, bool Periodic = true);

    void setTimeout(uint16_t value);
    void setPeriod(uint16_t value);

    //    typedef Notify<bool> subscribe;

    //    typedef void (*subscribeCallback)(bool) Notify<bool>::subscribeCallback(bool);

protected:
    void Run();

private:
    bool m_periodic;
};
