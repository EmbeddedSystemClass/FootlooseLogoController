/*************************************************
 * @file GPIOOutputDuplicate.h
 * @brief GPIO output duplicator
 *
 *************************************************/
#pragma once

#include <list>
#include <string>
#include "stdint.h"

#include "drv/DRVGPIO.h"

/**
 * GPIOOutputDuplicate
 */
class GPIOOutputDuplicate : public GPIOpin
{
public:
    GPIOOutputDuplicate();
    virtual ~GPIOOutputDuplicate();

    void addOutput(GPIOpin* pin);

    GPIOpin& operator=(const bool);
             operator bool();

    void toggle();
    void setAlternateFunction();
    void setNormalFunction();

protected:
    std::list<GPIOpin*> m_gpioPins;
    bool                m_state;
    bool                m_firstToggle;

    bool isOutput();
};
