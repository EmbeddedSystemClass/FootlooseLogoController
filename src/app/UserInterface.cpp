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

UserInterface::UserInterface()
    : cpp_freertos::Thread("UI", 500, 2)
    , m_fsm(*this)
{
}

void UserInterface::Run()
{
    m_fsm.Ready();

    while (1)
    {
        Delay(100);
    }
}
