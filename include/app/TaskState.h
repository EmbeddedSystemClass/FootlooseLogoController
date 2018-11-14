/*************************************************
 * @file TaskState.h
 * @brief Task state interface
 *
 *************************************************/
#pragma once

#include "os/thread.hpp"
#include "stdint.h"

class TaskState
{
public:
    enum State
    {
        StateNone = 0,
        StateInit,
        StateWaiting,
        StateRunning,
        StateError
    };

    TaskState(TaskHandle_t taskToNotify, uint8_t ID);

    ~TaskState();

    State getTaskState();

protected:
    void setTaskState(State state);

private:
    State        m_state;
    TaskHandle_t m_taskToNotify;
    uint8_t      m_ID;
};
