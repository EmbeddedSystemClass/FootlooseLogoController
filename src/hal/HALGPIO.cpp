/*************************************************
 * @file GPIOpin.cpp
 * @brief GPIO driver interface
 *
 *************************************************/
#include <hal/HALGPIO.h>
#include <string>
#include "stdint.h"

GPIOpin::GPIOpin(bool polarity)
    : m_polarity(polarity)
{
}

GPIOpin::~GPIOpin() {}

bool GPIOpin::applyPolarity(bool state)
{
    bool retVal = state;
    if (!m_polarity) retVal = !retVal;

    return retVal;
}
