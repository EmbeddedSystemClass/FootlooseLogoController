/*************************************************
 * @file GPIOBlinker.cpp
 * @brief
 *
 *************************************************/
#include "app/GPIOBlinker.h"

#include <string>
#include "stdint.h"

#include "OSError.h"
#include "drv/DRVGPIO.h"
#include "os/timer.hpp"

GPIOBlinker::GPIOBlinker(GPIOpin& pin)
    : cpp_freertos::Timer(1000, false)  // set to 1000 ticks and not periodic
    , m_pin(pin)
    , m_period(1000)
    , m_dutyCycle(50)
{
    m_pin = false;
}

GPIOBlinker& GPIOBlinker::operator=(const bool pinVal)
{
    // When assigned binary, stop using the timer
    m_pin = pinVal;
    if (pinVal)
    {
        m_dutyCycle = 100;
    }
    else
    {
        m_dutyCycle = 0;
    }
    m_period = UINT16_MAX;
    Stop();
    return *this;
}

void GPIOBlinker::setPeriod(uint16_t ms)
{
    if (m_period != ms)
    {
        m_period = ms;
        updateTimer();
    }
}

void GPIOBlinker::setFrequency(uint16_t hz)
{
    uint16_t newHz = 1000 / hz;
    if (m_period != newHz)
    {
        m_period = newHz;
        updateTimer();
    }
}

void GPIOBlinker::setDutyCycle(uint8_t duty)
{
    if (m_dutyCycle != duty)
    {
        m_dutyCycle = duty;
        if (m_dutyCycle > 100)
        {
            m_dutyCycle = 100;
            REPORTLOG("Dutycycle higher than 100 was set, limiting to 100%")
        }
        updateTimer();
    }
}

void GPIOBlinker::updateTimer()
{
    if (IsActive())
    {
        // Force update
        Run();
    }
    else
    {
        m_pin = true;
        SetPeriod((m_period * m_dutyCycle) / 100);
        Start();
    }
}

void GPIOBlinker::Run()
{
    m_pin.toggle();
    bool     state   = m_pin;
    uint32_t newTime = 0;
    if (state)
    {
        // Is now on
        newTime = (m_period * m_dutyCycle) / 100;
    }
    else
    {
        // Is now off
        newTime = (m_period * (100 - m_dutyCycle)) / 100;
    }
    SetPeriod(newTime);
    Start();
}
