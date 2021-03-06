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

    static Color RED() { return Color(255, 0, 0); }
    static Color GREEN() { return Color(0, 255, 0); }
    static Color BLUE() { return Color(0, 0, 255); }
    static Color WHITE() { return Color(255, 255, 255); }
    static Color BLACK() { return Color(0, 0, 0); }

    //    Color operator++(int)
    //    {
    //        Color result(*this);  // make a copy for result
    //        result.m_red++;
    //        result.m_green++;
    //        result.m_blue++;
    //        return result;  // return the copy (the old) value.
    //    }

private:
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
};
