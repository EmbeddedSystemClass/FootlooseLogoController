/*************************************************
 * @file UserInterface.h
 * @brief User interface
 *
 *************************************************/
#pragma once

#include "UserInterfaceFSM.h"
#include "app/BinDecIO.h"
#include "app/EffectsController.h"
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
        OperationDmx     = EffectsController::EffectModeDmx,
        OperationRainbow = EffectsController::EffectModeRainbow,
        OperationTest    = EffectsController::EffectModeTest
    };
    UserInterface(EffectsController& effectsController, DRV7SegmentDisplay& display, GPIOpin& power, GPIOpin& status, BinDecIO& dmxAddress,
                  GPIOpin& btnOk, GPIOpin& btnMode, GPIOpin& swMode, CAT5932& driver1, CAT5932& driver2);

    virtual void Run();

    void uiLedPower(bool state);
    void uiLedStatus(bool state);
    void uiDisplay(uint16_t value);
    void uiDisplay(const char* str);
    void setMode(OperationModes mode);
    void restorePreviousMode();

    uint8_t getDisplayBrightness();
    void    incDisplayBrightness();

    static void updateTimerElapsed(bool running, void* ptr);
    static void displaySleepTimerElapsed(bool running, void* ptr);

    OSTimer   m_updateTimer;
    OSTimer   m_displaySleepTimer;
    GPIOpin&  m_uiLedPower;
    GPIOpin&  m_uiLedStatus;
    BinDecIO& m_dmxAddress;

private:
    UserInterfaceFSM    m_fsm;
    EffectsController&  m_effectsController;
    DRV7SegmentDisplay& m_display;
    GPIOpin&            m_btnOk;
    GPIOpin&            m_btnMode;
    GPIOpin&            m_swMode;
    bool                m_lastBtnStateOk;
    bool                m_lastBtnStateMode;
    uint8_t             m_displayBrightness;
    CAT5932&            m_ledDriver1;
    CAT5932&            m_ledDriver2;
    uint16_t            m_lastDmxAddress;
    OperationModes      m_previousMode;
};
