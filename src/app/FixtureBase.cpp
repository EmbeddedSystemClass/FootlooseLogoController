/*************************************************
 * @file FixtureBase.cpp
 * @brief Fixture base interface
 *
 *************************************************/
#include "app/FixtureBase.h"

#include <string>
#include "stdint.h"

FixtureBase::FixtureBase(uint16_t dmxStartAddress, uint8_t channels, uint8_t x)
    : m_dmxStartAddress(dmxStartAddress)
    , m_dmxChannelCount(channels)
    , m_xPosition(x)
{
}

FixtureBase::~FixtureBase() {}

uint16_t FixtureBase::getDmxStartAddress() { return m_dmxStartAddress; }

uint8_t FixtureBase::getDmxChannelCount() { return m_dmxChannelCount; }

uint8_t FixtureBase::getXLocation() { return m_xPosition; }
