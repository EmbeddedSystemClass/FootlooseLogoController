/*************************************************
 * @file DRVGPIORemote.cpp
 * @brief GPIO driver interface
 *
 *************************************************/
#include "drv/DRVGPIORemote.h"

#include <string>
#include "stdint.h"

GPIORemotePin::GPIORemotePin(uint32_t ID, GPIORemotePinHandler& parent, bool polarity)
    : GPIOpin(polarity)
    , m_parent(parent)
    , m_ID(ID)
{
}

GPIORemotePin& GPIORemotePin::operator=(const bool val)
{
    m_parent.write(m_ID, applyPolarity(val));
    return *this;
}
GPIORemotePin::operator bool() { return applyPolarity(m_parent.read(m_ID)); }

void GPIORemotePin::toggle() { m_parent.toggle(m_ID); }

bool GPIORemotePin::isOutput() { m_parent.isOutput(m_ID); }

void GPIORemotePin::setAlternateFunction() {}
void GPIORemotePin::setNormalFunction() {}
