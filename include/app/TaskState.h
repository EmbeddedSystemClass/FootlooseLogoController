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
        TaskStateNone = 0,
        TaskStateInit,
        TaskStateWaiting,
        TaskStateRunning,
        TaskStateError
    };

    TaskState(TaskHandle_t taskToNotify, uint8_t ID);

    ~TaskState();

    State getTaskState();

protected:
    void setTaskState(State state);
    void setTaskStateFromISR(State state);

private:
    State        m_state;
    TaskHandle_t m_taskToNotify;
    uint8_t      m_ID;
};
