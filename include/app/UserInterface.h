/*************************************************
 * @file UserInterface.h
 * @brief User interface
 *
 *************************************************/
#pragma once

#include "UserInterfaceFSM.h"
#include "app/BinDecIO.h"
#include "drv/CAT5932.h"
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
    UserInterface(DRV7SegmentDisplay& display, GPIOpin& power, GPIOpin& status, BinDecIO& dmxAddress, GPIOpin& btnOk, GPIOpin& btnMode,
                  CAT5932& driver1, CAT5932& driver2);

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

    uint8_t getDisplayBrightness();
    void    incDisplayBrightness();

    static void timerElapsed(bool running, void* ptr);

    OSTimer   m_timer;
    GPIOpin&  m_uiLedPower;
    GPIOpin&  m_uiLedStatus;
    BinDecIO& m_dmxAddress;

private:
    UserInterfaceFSM    m_fsm;
    DRV7SegmentDisplay& m_display;
    GPIOpin&            m_btnOk;
    GPIOpin&            m_btnMode;
    uint8_t             m_displayBrightness;
    CAT5932&            m_ledDriver1;
    CAT5932&            m_ledDriver2;

    bool m_lastBtnStateOk;
    bool m_lastBtnStateMode;
};
