/*************************************************
 * @file BSP.h
 * @brief
 *
 *************************************************/
#include "app/BSP.h"

#include "thread.hpp"

#include "drv/DRVSerialUsb.h"
#include "os/OSError.h"

// OSError* BSP::m_osError = new OSError();
// OSError* OSError::This  = BSP::m_osError;

BSP::BSP(const char* name)
    : Thread(name, 200, 1)
{
}

void BSP::Run()
{
    // Task is created to initialize all software components

    // HAL

    // Driver
    DRVSerialUsb usbCDC;

    OSError errorhandler;
    errorhandler.setup(usbCDC);

    OSError::report(OSError::SevLog, OSError::TypeNone, 0);

    //    usbCDC.open(0);
    //
    //    usbCDC.open(0);

    // APP
    while (1)
    {
        //        usbCDC.send((uint8_t*)"Hello world, this is a test!\r\n", 30);
        OSError::report(OSError::SevLog, OSError::TypeNone, 1);
        Delay(100);
    }
}

BSP::~BSP() {}
