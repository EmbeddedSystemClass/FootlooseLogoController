/*************************************************
 * @file BSP.h
 * @brief
 *
 *************************************************/
#include "app/BSP.h"

#include "drv/DRVSerialUsb.h"

BSP::BSP(const char* name) : OSTask(name, 200, osPriorityAboveNormal) {}

void BSP::task()
{
    // Task is created to initialize all software components

    // HAL

    // Driver
    DRVSerialUsb usbCDC;

    // APP
    while (1)
    {
        //        usbCDC.send((uint8_t*)"Hello world, this is a test!\r\n", 30);
        delay(100);
    }
}

BSP::~BSP() {}
