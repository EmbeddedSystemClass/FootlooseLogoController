/*************************************************
 * @file DRV7SegmentDisplay.cpp
 * @brief 7- segment display driver
 *
 *************************************************/

#include "drv/DRV7SegmentDisplay.h"

#include "drv/DRV7Segment.h"
#include "hal/HALGPIO.h"

#include <string>
#include <vector>
#include "stdint.h"
#include "string.h"

DRV7SegmentDisplay::DRV7SegmentDisplay() {}

void DRV7SegmentDisplay::addSegment(DRV7Segment* segment) { m_segments.push_back(segment); }

void DRV7SegmentDisplay::setNumber(uint16_t val)
{
    char buffer[10] = {0};
    snprintf(buffer, 5, "%d", val);
    setString(buffer);
}

void DRV7SegmentDisplay::setString(const char* str)
{
    uint8_t stringLength = strlen(str);

    // currently no support for scrolling display
    if (stringLength > 0)
    {
        uint8_t nextChar = 1;
        for (auto const& value : m_segments)
        {
            value->setChar(str[stringLength - nextChar++]);
            if (nextChar > stringLength) break;
        }
    }
    else
    {
        // clear display
        for (auto const& value : m_segments) value->setChar(' ');
    }
}
