/*************************************************
 * @file TaskState.h
 * @brief Task state interface
 *
 *************************************************/
#pragma once

#include "stdint.h"

#include "GPIOBlinker.h"
#include "TaskState.h"
#include "drv/DRVGPIO.h"
#include "thread.hpp"

class TaskStateMonitor : public cpp_freertos::Thread
{
public:
    TaskStateMonitor(const char* name, GPIOpin led);

    ~TaskStateMonitor();

    void Run();

private:
    TaskState::State m_taskStates[16];

    GPIOBlinker m_led;

    // Force empty constructor to not be used
    TaskStateMonitor();

    TaskState::State getHighestState();
};
