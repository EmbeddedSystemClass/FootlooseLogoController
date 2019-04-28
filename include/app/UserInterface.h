/*************************************************
 * @file UserInterface.h
 * @brief User interface
 *
 *************************************************/
#pragma once

#include "UserInterfaceFSM.h"
#include "app/BinDecIO.h"
#include "drv/DRV7SegmentDisplay.h"
#include "hal/HALGPIO.h"
#include "os/OSTimer.h"
#include "os/thread.hpp"

using namespace std;
using namespace statemap;

class UserInterface : public cpp_freertos::Thread
{
public:
    enum OperationModes
    {
        OperationDmx,
        OperationRainbow,
        OperationTest
    };
    UserInterface(DRV7SegmentDisplay& display, GPIOpin& power, GPIOpin& status, BinDecIO& dmxAddress);

    virtual void Run();

    void    uiLedPower(bool state);
    void    uiLedStatus(bool state);
    void    uiDisplay(uint16_t value);
    void    uiDisplay(const char* str);
    void    setMode(OperationModes mode);
    void    setSpeed(uint8_t speed);
    uint8_t getSpeed();
    void    incSpeed();
    void    decSpeed();

    static void timerElapsed(bool running, void* ptr);

    OSTimer   m_timer;
    GPIOpin&  m_uiLedPower;
    GPIOpin&  m_uiLedStatus;
    BinDecIO& m_dmxAddress;

private:
    UserInterfaceFSM    m_fsm;
    DRV7SegmentDisplay& m_display;
};
