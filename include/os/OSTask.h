/*************************************************
 * @file OSTask.h
 * @brief c++ wrapper for FREERTOS
 *
 *************************************************/

#include "stdint.h"

#include "cmsis_os.h"

/*
 * OS task wrapper
 */
class OSTask
{
public:
    OSTask(const char* name, uint16_t stackDepth, osPriority priority);
    virtual ~OSTask();

    virtual void task() = 0;

protected:
    static void delay(uint32_t ticks);

private:
    struct OSTaskInfo
    {
        osThreadId taskHandle;
        OSTask* This;
    };

    // Force empty constructor to not be used
    OSTask();

    static inline void taskHandler(void const* pvParameters);

    OSTaskInfo m_taskInfo;
};
