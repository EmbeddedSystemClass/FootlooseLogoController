/*************************************************
 * @file GPIOOutputDuplicate.cpp
 * @brief GPIO output duplicator implementation
 *
 *************************************************/
#include "app/GPIOOutputDuplicate.h"

#include <string>
#include "stdint.h"

#include "OSError.h"
#include "drv/DRVGPIO.h"

GPIOOutputDuplicate::GPIOOutputDuplicate()
    : GPIOpin(false)
    , m_state(false)
{
}

GPIOOutputDuplicate::~GPIOOutputDuplicate() {}

void GPIOOutputDuplicate::addOutput(GPIOpin* pin) { m_gpioPins.push_front(pin); }

GPIOpin& GPIOOutputDuplicate::operator=(const bool value)
{
    m_state = value;
    for (auto& i : m_gpioPins)
    {
        *i = value;
    }
}

GPIOOutputDuplicate::operator bool() { return m_state; }

void GPIOOutputDuplicate::toggle()
{
    for (auto const& i : m_gpioPins)
    {
        i->toggle();
    }
}

void GPIOOutputDuplicate::setAlternateFunction() { REPORTFATAL("Function unsupported") }

void GPIOOutputDuplicate::setNormalFunction() { REPORTFATAL("Function unsupported") }

bool GPIOOutputDuplicate::isOutput() { return true; }
