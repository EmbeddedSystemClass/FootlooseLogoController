/*************************************************
 * @file GPIOBlinker.h
 * @brief GPIO driver interface
 *
 *************************************************/
#pragma once

#include <string>
#include "stdint.h"

#include "drv/DRVGPIO.h"
#include "os/timer.hpp"

/**
 * GPIOBlinker
 */
class GPIOBlinker : private cpp_freertos::Timer
{
public:
    GPIOBlinker(GPIOpin& pin);

    GPIOBlinker& operator=(const bool);

    void setPeriod(uint16_t ms);

    void setFrequency(uint16_t hz);

    void setDutyCycle(uint8_t ms);

private:
    GPIOpin  m_pin;
    uint16_t m_period;
    uint16_t m_dutyCycle;

    GPIOBlinker();  // not allowed

    void updateTimer();

    void Run();
};
