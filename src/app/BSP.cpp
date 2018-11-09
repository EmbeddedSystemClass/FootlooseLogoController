/*************************************************
 * @file BSP.h
 * @brief
 *
 *************************************************/
#include "app/BSP.h"

#include <string>

#include "drv/DRVSerialUsb.h"
#include "os/OSError.h"
#include "thread.hpp"

BSP::BSP(const char* name)
    : Thread(name, 200, 1)
{
}

void BSP::Run()
{
    // Task is created to initialize all software components
    // Make sure our error handler is up before anything else

    // output for error, ideal would be uart as this can also capture startup issues
    DRVSerialUsb usbCDC;

    OSError errorHandler(usbCDC);
    REPORT(OSError::SevLog, OSError::TypeNone, "System up!");

    // HAL

    REPORT(OSError::SevLog, OSError::TypeNone, "Initialization of HAL complete");

    // Driver

    REPORT(OSError::SevLog, OSError::TypeNone, "Initialization of DRV complete");

    // APP

    REPORT(OSError::SevLog, OSError::TypeNone, "Initialization of APP complete");

    // Suspend this task as we do not want to free memory
    Suspend();

    REPORT(OSError::SevFatal, OSError::TypeNone, "BSP return from suspend, this is not allowed");
}

BSP::~BSP() {}
