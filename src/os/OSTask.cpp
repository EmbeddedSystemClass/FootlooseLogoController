/*************************************************
 * @file OSTask.h
 * @brief implementation c++ wrapper for FREERTOS
 *
 *************************************************/
#include "OSTask.h"

//#include "FreeRTOS.h"
#include "cmsis_os.h"

OSTask::OSTask(const char* name, uint16_t stackDepth, osPriority priority)
{
    osThreadDef(name, taskHandler, priority, 0, 128);
    m_taskInfo.This = this;
    m_taskInfo.taskHandle = osThreadCreate(osThread(name), &m_taskInfo);
}

OSTask::~OSTask() {}

void OSTask::taskHandler(void const* pvParameters)
{
    static_cast<const OSTaskInfo*>(pvParameters)->This->task();
}

void OSTask::delay(uint32_t ticks) { osDelay(ticks); }
