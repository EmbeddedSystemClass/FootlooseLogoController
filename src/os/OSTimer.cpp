/*************************************************
 * @file OSTimer.cpp
 * @brief OS timer
 *
 *************************************************/
#include "OSTimer.h"

#include <string>
#include "stdint.h"

#include "util/Notify.h"

#include "timer.hpp"

OSTimer::OSTimer()
    : cpp_freertos::Timer("OSTimer", 1000, false)
    , m_periodic(false)
{
}

// OSTimer::OSTimer(const char* const TimerName, TickType_t PeriodInTicks, bool Periodic)
//    : cpp_freertos::Timer(TimerName, PeriodInTicks, Periodic)
//,m_periodic(Periodic)
//{
//}

void OSTimer::setTimeout(uint16_t value)
{
    m_periodic = false;
    SetPeriod(value);
    Start();
}
void OSTimer::setPeriod(uint16_t value)
{
    m_periodic = true;
    SetPeriod(value);
    Start();
}

void OSTimer::Run()
{
    if (m_periodic) Start();
    notifySubscribers(IsActive());
}
