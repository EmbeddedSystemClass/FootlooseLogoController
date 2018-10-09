/*************************************************
 * @file OSSema.cpp
 * @brief c++ wrapper for FREERTOS
 *
 *************************************************/

#include "stdint.h"

#include "os/OSSema.h"

#include "cmsis_os.h"

OSSema::OSSema(int32_t count)
{
    const osSemaphoreDef_t dummy = {0};
    m_semaphore = osSemaphoreCreate(&dummy, count);
}

OSSema::~OSSema() { osSemaphoreDelete(m_semaphore); }

int32_t OSSema::wait(uint32_t ms) { return osSemaphoreWait(m_semaphore, ms); }

void OSSema::release() { osSemaphoreRelease(m_semaphore); }
