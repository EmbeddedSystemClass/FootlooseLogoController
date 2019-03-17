/*************************************************
 * @file CAT5932.h
 * @brief CAT5932 led driver driver
 *
 *************************************************/
#pragma once

#include "DRVGPIO.h"
#include "DRVGPIORemote.h"
#include "HAL/HALi2c.h"
#include "I2CDevice.h"

class CAT5932 : private I2CDevice, public GPIORemotePinHandler
{
public:
    enum OutputState
    {
        OutputOff      = 0b00,
        OutputOnBlink1 = 0b10,
        OutputOnBlink2 = 0b11,
        OutputOn       = 0b01,
    };
    const uint8_t OUTPUT_STATE_MASK = 0b11;

    CAT5932(HALI2C& i2cHal, uint8_t deviceAddress);

    bool initDevice();

    void setOnState(OutputState state);

    GPIORemotePin getPin(uint8_t pin);

    virtual bool read(uint32_t ID);

    virtual void write(uint32_t ID, bool val);

    virtual void toggle(uint32_t ID);

    virtual bool isOutput(uint32_t ID);

    void sendUpdate();

private:
    enum Registers
    {
        REGInput0 = 0x00,
        REGInput1,
        REGPSC0,
        REGPWM0,
        REGPSC1,
        REGPWM1,
        REGLS0,
        REGLS1,
        REGLS2,
        REGLS3,
        REG_MAX
    };
    const uint8_t REG_AUTO_INCREMENT = 0x10;

    uint8_t     m_registerBuffer[REG_MAX];
    uint8_t     m_I2COutputData[REG_MAX + 1];
    OutputState m_onState;

    OutputState getStateFromReg(uint8_t pin);
    bool        getOnStateFromReg(uint8_t pin);
    void        setStateToReg(uint8_t pin, OutputState state);
    void        setOnStateToReg(uint8_t pin, bool state);
    uint8_t*    getOutputReg(uint8_t pin);
};
