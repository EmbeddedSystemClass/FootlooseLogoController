/*************************************************
 * @file RGBFixture.h
 * @brief Fixture base interface
 *
 *************************************************/
#include "app/RGBFixture.h"

#include <string>
#include "stdint.h"

#include "app/Color.h"
#include "app/FixtureBase.h"

RGBFixture::RGBFixture(uint16_t dmxStartAddress, uint8_t x)
    : FixtureBase(dmxStartAddress, 3, x)
    , m_color()
    , m_brightness(255)
{
}

RGBFixture::~RGBFixture() {}

void RGBFixture::getDmxData(uint8_t* data)
{
    data[0] = (m_color.getRed() * m_brightness) / 255;
    data[1] = (m_color.getGreen() * m_brightness) / 255;
    data[2] = (m_color.getBlue() * m_brightness) / 255;
}

void RGBFixture::setColor(const Color color) { m_color = color; }

void RGBFixture::setBrightness(uint8_t brightness) { m_brightness = brightness; }
