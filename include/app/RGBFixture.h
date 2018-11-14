/*************************************************
 * @file RGBFixture.h
 * @brief Fixture base interface
 *
 *************************************************/
#pragma once

#include <string>
#include "stdint.h"

#include "Color.h"
#include "FixtureBase.h"

/**
 * RGBFixture
 */
class RGBFixture : public FixtureBase
{
public:
    RGBFixture(uint16_t dmxStartAddress, uint8_t x);

    virtual ~RGBFixture();

    virtual void getDmxData(uint8_t* data);

    void setColor(const Color color);

    void setBrightness(uint8_t brightness);

protected:
    Color   m_color;
    uint8_t m_brightness;

private:
    RGBFixture();  // Not allowed
};
