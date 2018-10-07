/*************************************************
 * @file BSP.h
 * @brief
 *
 *************************************************/
#include "app/BSP.h"

BSP::BSP(const char* name) : OSTask(name, 200, osPriorityAboveNormal) {}

void BSP::task()
{
    while (1)
    {
        delay(100);
    }
}

BSP::~BSP() {}
