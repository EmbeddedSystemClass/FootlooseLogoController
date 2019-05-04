/*************************************************
 * @file UserInterface.cpp
 * @brief User interface
 *
 *************************************************/
#include "app/UserInterface.h"

#include "app/UserInterfaceFSM.h"
#include "os/thread.hpp"

using namespace std;
using namespace statemap;

UserInterface::UserInterface(DRV7SegmentDisplay& display, GPIOpin& power, GPIOpin& status, BinDecIO& dmxAddress, GPIOpin& btnOk, GPIOpin& btnMode,
                             CAT5932& driver1, CAT5932& driver2)
    : cpp_freertos::Thread("UI", 250, 2)
    , m_fsm(*this)
    , m_display(display)
    , m_uiLedPower(power)
    , m_uiLedStatus(status)
    , m_dmxAddress(dmxAddress)
    , m_btnOk(btnOk)
    , m_btnMode(btnMode)
    , m_lastBtnStateOk(false)
    , m_lastBtnStateMode(false)
    , m_displayBrightness(10)
    , m_ledDriver1(driver1)
    , m_ledDriver2(driver2)
{
    m_timer.subscribe(&timerElapsed, this);
}

void UserInterface::Run()
{
    m_fsm.Ready();

    while (1)
    {
        if ((m_btnOk && !m_lastBtnStateOk) && !m_btnMode)
        {
            m_fsm.BtnOkPressed();
        }
        if ((m_btnMode && !m_lastBtnStateMode) && !m_btnOk)
        {
            m_fsm.BtnModePressed();
        }
        m_lastBtnStateMode = m_btnMode;
        m_lastBtnStateOk   = m_btnOk;

        Delay(100);
    }
}

void UserInterface::timerElapsed(bool running, void* ptr)
{
    UserInterface* This = static_cast<UserInterface*>(ptr);
    This->m_fsm.timerElapsed();
}

void UserInterface::uiLedPower(bool state) { m_uiLedPower = state; }
void UserInterface::uiLedStatus(bool state) { m_uiLedStatus = state; }
void UserInterface::uiDisplay(uint16_t value) { m_display.setNumber(value); }
void UserInterface::uiDisplay(const char* str) { m_display.setString(str); }

void    UserInterface::setMode(OperationModes mode) {}
void    UserInterface::setSpeed(uint8_t speed) {}
uint8_t UserInterface::getSpeed() { return 0; }
void    UserInterface::incSpeed() {}
void    UserInterface::decSpeed() {}

uint8_t UserInterface::getDisplayBrightness() { return m_displayBrightness; }
void    UserInterface::incDisplayBrightness()
{
    m_displayBrightness++;
    if (m_displayBrightness > 10) m_displayBrightness = 0;
    m_ledDriver1.setBrightness(m_displayBrightness * 25);
    m_ledDriver2.setBrightness(m_displayBrightness * 25);
}
