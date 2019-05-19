/*************************************************
 * @file DRVGPIORemote.h
 * @brief GPIO driver interface
 *
 *************************************************/
#pragma once
#include "DRVGPIO.h"

#include <string>
#include "stdint.h"

class GPIORemotePinHandler
{
public:
    virtual ~GPIORemotePinHandler() {}
    virtual bool read(uint32_t ID)            = 0;
    virtual void write(uint32_t ID, bool val) = 0;
    virtual void toggle(uint32_t ID)          = 0;
    virtual bool isOutput(uint32_t ID)        = 0;
};

class GPIORemotePin : public GPIOpin
{
public:
    GPIORemotePin(uint32_t ID, GPIORemotePinHandler& parent, bool polarity);
    virtual ~GPIORemotePin();

    virtual GPIORemotePin& operator=(const bool);
    virtual                operator bool();
    virtual void           toggle();
    virtual void           setAlternateFunction();
    virtual void           setNormalFunction();

protected:
    GPIORemotePinHandler& m_parent;
    uint32_t              m_ID;

    virtual bool isOutput();
};
