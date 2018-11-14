/*************************************************
 * @file Color.cpp
 * @brief color interface
 *
 *************************************************/
#include "app/Color.h"

#include <string>
#include "stdint.h"

Color::Color()
    : m_red(0)
    , m_green(0)
    , m_blue(0)
{
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue)
    : m_red(red)
    , m_green(green)
    , m_blue(blue)
{
}

void Color::setRed(uint8_t red) { m_red = red; }

void Color::setGreen(uint8_t green) { m_green = green; }

void Color::setBlue(uint8_t blue) { m_blue = blue; }

void Color::setRedPercentage(uint8_t percent) { m_red = (255 * percent) / 100; }

void Color::setGreenPercentage(uint8_t percent) { m_green = (255 * percent) / 100; }

void Color::setBluePercentage(uint8_t percent) { m_blue = (255 * percent) / 100; }

uint8_t Color::getRed() { return m_red; }

uint8_t Color::getGreen() { return m_green; }

uint8_t Color::getBlue() { return m_blue; }
