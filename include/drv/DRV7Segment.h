/*************************************************
 * @file DRV7Segment.h
 * @brief 7- segment driver
 *
 *************************************************/
#pragma once

#include "hal/HALGPIO.h"

#include <string>
#include "stdint.h"

class DRV7Segment
{
public:
    DRV7Segment(GPIOpin& segmentA, GPIOpin& segmentB, GPIOpin& segmentC, GPIOpin& segmentD, GPIOpin& segmentE, GPIOpin& segmentF, GPIOpin& segmentG,
                GPIOpin* segmentDot);

    void setChar(char c);
    void setNumber(uint8_t);
    void setDot(bool enable);

private:
    enum Segment
    {
        SegmentA,
        SegmentB,
        SegmentC,
        SegmentD,
        SegmentE,
        SegmentF,
        SegmentG,
        SegmentDot,
        SegmentMax,
    };
    GPIOpin* m_segments[SegmentMax];
};
