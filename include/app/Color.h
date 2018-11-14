/*************************************************
 * @file Color.h
 * @brief color interface
 *
 *************************************************/
#pragma once

#include <string>
#include "stdint.h"

/**
 * Color
 */
class Color
{
public:
    Color();

    Color(uint8_t red, uint8_t green, uint8_t blue);

    void setRed(uint8_t red);

    void setGreen(uint8_t green);

    void setBlue(uint8_t blue);

    void setRedPercentage(uint8_t percent);

    void setGreenPercentage(uint8_t percent);

    void setBluePercentage(uint8_t percent);

    uint8_t getRed();

    uint8_t getGreen();

    uint8_t getBlue();

private:
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
};
