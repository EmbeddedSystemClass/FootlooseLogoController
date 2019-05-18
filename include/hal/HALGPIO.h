/*************************************************
 * @file GPIOpin.h
 * @brief GPIO driver interface
 *
 *************************************************/
#pragma once

#include <string>
#include "stdint.h"

/**
 * PIN
 */
class GPIOpin
{
public:
    GPIOpin(bool polarity);
    virtual GPIOpin& operator=(const bool)  = 0;
    virtual          operator bool()        = 0;
    virtual void     toggle()               = 0;
    virtual void     setAlternateFunction() = 0;
    virtual void     setNormalFunction()    = 0;

protected:
    bool m_polarity;  //!< 1 means normal

    bool         applyPolarity(bool state);
    virtual bool isOutput() = 0;
};
