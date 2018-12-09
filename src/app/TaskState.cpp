/*************************************************
 * @file TaskState.cpp
 * @brief Task state interface
 *
 *************************************************/
#include "app/TaskState.h"

#include "stdint.h"

#include "OSError.h"
#include "task.h"

TaskState::TaskState(TaskHandle_t taskToNotify, uint8_t ID)
    : m_state(TaskStateInit)
    , m_taskToNotify(taskToNotify)
    , m_ID(ID)
{
    if (m_ID > 15) REPORTFATAL("ID above 15 not allowed due to shifting in uint16")
}

TaskState::~TaskState() {}

TaskState::State TaskState::getTaskState() { return m_state; }

void TaskState::setTaskState(State state)
{
    m_state = state;

    uint32_t notifyValue = static_cast<uint32_t>(m_state);
    notifyValue |= ((m_ID << 16) & 0xFFFF0000);

    xTaskNotify(m_taskToNotify, notifyValue, eSetValueWithOverwrite);
}

void TaskState::setTaskStateFromISR(State state)
{
    BaseType_t pxHigherPriorityTaskWoken = 0;
    m_state                              = state;

    uint32_t notifyValue = static_cast<uint32_t>(m_state);
    notifyValue |= ((m_ID << 16) & 0xFFFF0000);

    xTaskNotifyFromISR(m_taskToNotify, notifyValue, eSetValueWithOverwrite, &pxHigherPriorityTaskWoken);
}
