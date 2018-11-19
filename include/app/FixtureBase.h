/*************************************************
 * @file FixtureBase.h
 * @brief Fixture base interface
 *
 *************************************************/
#pragma once

#include <string>
#include "stdint.h"

/**
 * FixtureBase
 */
class FixtureBase
{
public:
    FixtureBase(uint16_t dmxStartAddress, uint8_t channels, uint8_t x);

    virtual ~FixtureBase();

    uint16_t getDmxStartAddress() const;

    uint8_t getDmxChannelCount() const;

    uint8_t getXLocation() const;

    virtual void getDmxData(uint8_t* data) = 0;

protected:
    const uint16_t m_dmxStartAddress;
    const uint8_t  m_dmxChannelCount;
    const uint8_t  m_xPosition;

private:
    FixtureBase();  // Not allowed
};
