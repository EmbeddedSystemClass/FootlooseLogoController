/*************************************************
 * @file DRV7SegmentDisplay.h
 * @brief 7- segment display driver
 *
 *************************************************/
#pragma once

#include "drv/DRV7Segment.h"
#include "hal/HALGPIO.h"

#include <string>
#include <vector>
#include "stdint.h"

class DRV7SegmentDisplay
{
public:
    DRV7SegmentDisplay();

    void addSegment(DRV7Segment*);
    void setNumber(uint16_t val);
    void setString(const char* str);
    void clearDisplay();

private:
    std::vector<DRV7Segment*> m_segments;
};
