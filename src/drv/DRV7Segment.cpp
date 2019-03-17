/*************************************************
 * @file DRV7Segment.cpp
 * @brief 7- segment driver
 *
 *************************************************/

#include "drv/DRV7Segment.h"
#include "hal/HALGPIO.h"

#include "third party/7-Segment-ASCII_BIN.h"

#include "stddef.h"
#include "stdint.h"

DRV7Segment::DRV7Segment(GPIOpin& segmentA, GPIOpin& segmentB, GPIOpin& segmentC, GPIOpin& segmentD, GPIOpin& segmentE, GPIOpin& segmentF,
                         GPIOpin& segmentG, GPIOpin* segmentDot)
{
    m_segments[SegmentA]   = &segmentA;
    m_segments[SegmentB]   = &segmentB;
    m_segments[SegmentC]   = &segmentC;
    m_segments[SegmentD]   = &segmentD;
    m_segments[SegmentE]   = &segmentE;
    m_segments[SegmentF]   = &segmentF;
    m_segments[SegmentG]   = &segmentG;
    m_segments[SegmentDot] = segmentDot;
}

void DRV7Segment::setChar(char c)
{
    if (c >= 32 && c <= 127)
    {
        uint8_t segments      = SevenSegmentAscii::SevenSegmentASCII[c - 32];
        *m_segments[SegmentA] = (segments & 0b00000001);
        *m_segments[SegmentB] = (segments & 0b00000010);
        *m_segments[SegmentC] = (segments & 0b00000100);
        *m_segments[SegmentD] = (segments & 0b00001000);
        *m_segments[SegmentE] = (segments & 0b00010000);
        *m_segments[SegmentF] = (segments & 0b00100000);
        *m_segments[SegmentG] = (segments & 0b01000000);
        if (m_segments != NULL) *m_segments[SegmentDot] = (segments & 0b10000000);
    }
}
void DRV7Segment::setNumber(uint8_t val)
{
    if (val >= 0 && val <= 9) setChar(val + 48);
}
void DRV7Segment::setDot(bool enable)
{
    if (m_segments != NULL) *m_segments[SegmentDot] = enable;
}
