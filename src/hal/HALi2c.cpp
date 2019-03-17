/*************************************************
 * @file HALi2c.cpp
 * @brief HAL I2C
 *
 *************************************************/

#include "hal/HALi2c.h"

#include "stddef.h"
#include "stdint.h"

HALI2C::HALI2C()
    : m_callback(NULL)
    , m_callbackParamerter(NULL)
{
}

void HALI2C::setCallback(CallbackFunction function, void* parameter)
{
    m_callback           = function;
    m_callbackParamerter = parameter;
}

void HALI2C::callCallback(CallbackEvent event, uint32_t value)
{
    if (m_callback != NULL)
    {
        m_callback(event, value, m_callbackParamerter);
    }
}
