/*************************************************
 * @file UserInterface.h
 * @brief User interface
 *
 *************************************************/
#pragma once

#include "UserInterfaceFSM.h"
#include "os/thread.hpp"

using namespace std;
using namespace statemap;

class UserInterface : public cpp_freertos::Thread
{
public:
    UserInterface();

    virtual void Run();

private:
    UserInterfaceFSM m_fsm;
};
