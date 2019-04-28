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

UserInterface::UserInterface(DRV7SegmentDisplay& display, GPIOpin& power, GPIOpin& status, BinDecIO& dmxAddress)
    : cpp_freertos::Thread("UI", 250, 2)
    , m_fsm(*this)
    , m_display(display)
    , m_uiLedPower(power)
    , m_uiLedStatus(status)
    , m_dmxAddress(dmxAddress)
{
    m_timer.subscribe(&timerElapsed, this);
}

void UserInterface::Run()
{
    m_fsm.Ready();

    while (1)
    {
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
uint8_t UserInterface::getSpeed() {}
void    UserInterface::incSpeed() {}
void    UserInterface::decSpeed() {}
