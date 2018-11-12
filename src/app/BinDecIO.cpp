/*************************************************
 * @file BinDecIO.cpp
 * @brief Binair to decimal converter for IO
 *
 *************************************************/

#include "app/BinDecIO.h"

#include "stdint.h"

BinDecIO::BinDecIO()
    : m_pinList()
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

    return retVal;
}
