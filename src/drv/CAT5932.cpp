/*************************************************
 * @file CAT5932.cpp
 * @brief CAT5932 led driver driver
 *
 *************************************************/
#include "drv/CAT5932.h"

#include "HAL/HALi2c.h"
#include "OS/OSError.h"
#include "drv/DRVGPIO.h"
#include "drv/DRVGPIORemote.h"
#include "drv/I2CDevice.h"

#include <cstring>

CAT5932::CAT5932(const char* name, HALI2C& i2cHal, uint8_t deviceAddress)
    : Thread(name, 100, 1)
    , I2CDevice(i2cHal, deviceAddress)
    , m_onState(OutputOn)
    , m_registerUpdated(false)
{
    memset(m_registerBuffer, 0, REG_MAX);
}

bool CAT5932::initDevice() {}

void CAT5932::setOnState(CAT5932::OutputState state)
{
    m_onState = state;
    for (uint8_t i = 0; i < 15; i++)
    {
        if (getStateFromReg(i) != OutputOff)
        {
            setStateToReg(i, state);
        }
    }
    m_registerUpdated = true;
}

GPIORemotePin CAT5932::getPin(uint8_t pin)
{
    if (pin >= 0 && pin <= 15)
    {
        return GPIORemotePin(pin, *this, 1);
    }
    else
    {
        REPORTFATAL("Pin out of range")
        // will never return from reportfatal, call below is to avoid compiler warning
        return GPIORemotePin(pin, *this, 1);
    }
}

bool CAT5932::read(uint32_t ID) { return getOnStateFromReg(ID); }

void CAT5932::write(uint32_t ID, bool val) { setOnStateToReg(ID, val); }

void CAT5932::toggle(uint32_t ID) { setOnStateToReg(ID, !getOnStateFromReg(ID)); }

bool CAT5932::isOutput(uint32_t ID)
{
    (void)ID;
    return true;
}

void CAT5932::setBrightness(uint8_t brightness)
{
    m_registerBuffer[REGPWM0] = brightness;
    m_registerBuffer[REGPSC0] = 0;

    m_I2COutputData[0] = REGPSC0 | REG_AUTO_INCREMENT;
    m_I2COutputData[1] = m_registerBuffer[REGPSC0];
    m_I2COutputData[2] = m_registerBuffer[REGPWM0];

    writeToDevice(m_I2COutputData, 3);

    setOnState(OutputOnBlink1);
}

CAT5932::OutputState CAT5932::getStateFromReg(uint8_t pin)
{
    OutputState retVal = OutputOff;
    uint8_t*    reg    = getOutputReg(pin);
    retVal             = static_cast<OutputState>((*reg >> ((pin % 4) * 2)) & OUTPUT_STATE_MASK);
    return retVal;
}
bool CAT5932::getOnStateFromReg(uint8_t pin)
{
    bool retVal = 0;

    if (getStateFromReg(pin) != OutputOff)
    {
        retVal = 1;
    }

    return retVal;
}
void CAT5932::setStateToReg(uint8_t pin, CAT5932::OutputState state)
{
    uint8_t* reg     = getOutputReg(pin);
    uint8_t  regVal  = *reg;
    uint8_t  bitmask = ~(0x03 << ((pin % 4) * 2));
    uint8_t  setBits = (state << ((pin % 4) * 2));

    *reg &= bitmask;  // clear target bits
    *reg |= setBits;  // set intended bits

    if (*reg != regVal)
    {
        m_registerUpdated = true;
    }
}
void CAT5932::setOnStateToReg(uint8_t pin, bool state)
{
    if (state)
    {
        setStateToReg(pin, m_onState);
    }
    else
    {
        setStateToReg(pin, OutputOff);
    }
}

uint8_t* CAT5932::getOutputReg(uint8_t pin)
{
    uint8_t* retVal = 0;
    if (pin >= 0 && pin <= 3)
    {
        retVal = &m_registerBuffer[REGLS0];
    }
    else if (pin >= 4 && pin <= 7)
    {
        retVal = &m_registerBuffer[REGLS1];
    }
    else if (pin >= 8 && pin <= 11)
    {
        retVal = &m_registerBuffer[REGLS2];
    }
    else if (pin >= 12 && pin <= 15)
    {
        retVal = &m_registerBuffer[REGLS3];
    }
    else
    {
        REPORTFATAL("Wrong pin index")
    }
    return retVal;
}

void CAT5932::sendUpdate()
{
    m_I2COutputData[0] = REGLS0 | REG_AUTO_INCREMENT;
    m_I2COutputData[1] = m_registerBuffer[REGLS0];
    m_I2COutputData[2] = m_registerBuffer[REGLS1];
    m_I2COutputData[3] = m_registerBuffer[REGLS2];
    m_I2COutputData[4] = m_registerBuffer[REGLS3];

    writeToDevice(m_I2COutputData, 5);
}

void CAT5932::Run()
{
    while (1)
    {
        if (m_registerUpdated)
        {
            m_registerUpdated = false;
            sendUpdate();
        }
        Delay(50);
    }
}
