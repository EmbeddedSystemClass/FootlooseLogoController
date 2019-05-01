/*************************************************
 * @file BinDecIO.cpp
 * @brief Binair to decimal converter for IO
 *
 *************************************************/

#include "app/BinDecIO.h"

#include "stdint.h"

BinDecIO::BinDecIO(uint32_t offset)
    : m_pinList()
    , m_offset(offset)
{
}

void BinDecIO::addBin(PinValuePair bin) { m_pinList.push_front(bin); }

BinDecIO::operator uint32_t()
{
    uint32_t retVal = 0;

    for (auto const& i : m_pinList)
    {
        bool bitToShift = i.first;
        retVal += (bitToShift << i.second);
    }

    return retVal + m_offset;
}
