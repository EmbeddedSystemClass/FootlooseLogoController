/*************************************************
 * @file OSSema.h
 * @brief c++ wrapper for FREERTOS
 *
 *************************************************/

#include "stdint.h"

#include "cmsis_os.h"

/*
 * OS task wrapper
 */
class OSSema
{
public:
    OSSema(int32_t count);

    ~OSSema();

    int32_t wait(uint32_t ms);

    void release();

protected:
private:
    osSemaphoreId m_semaphore;
};
