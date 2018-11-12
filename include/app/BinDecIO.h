/*************************************************
 * @file BinDecIO.h
 * @brief Binair to decimal converter for IO
 *
 *************************************************/
#pragma once

#include "drv/DRVGPIO.h"

#include <list>
#include <utility>
#include "stdint.h"

class BinDecIO
{
public:
    typedef std::pair<GPIOpin&, uint8_t> PinValuePair;

    BinDecIO();

    void addBin(PinValuePair bin);

    operator uint32_t();

private:
    std::list<PinValuePair> m_pinList;
};
