/*************************************************
 * @file BSP.h
 * @brief
 *
 *************************************************/
#include "app/BSP.h"

#include <string>

#include "app/BinDecIO.h"
#include "drv/DRVGPIO.h"
#include "drv/DRVSerialUsb.h"
#include "os/OSError.h"
#include "thread.hpp"

BSP::BSP(const char* name)
    : Thread(name, 400, 1)
{
}

void BSP::Run()
{
    // Task is created to initialize all software components
    // Make sure our error handler is up before anything else

    // output for error, ideal would be uart as this can also capture startup issues
    DRVSerialUsb usbCDC;

    OSError errorHandler;
    errorHandler.setup(usbCDC);

    Delay(1000);

    REPORT(OSError::SevLog, OSError::TypeNone, "System up!");
    char datetime[50];
    sprintf(datetime, "Build date %s, %s\n\r", __DATE__, __TIME__);
    REPORT(OSError::SevLog, OSError::TypeNone, datetime);
    REPORT(OSError::SevLog, OSError::TypeNone, "System up!");

    // HAL

    REPORT(OSError::SevLog, OSError::TypeNone, "Initialization of HAL complete");

    // Driver

    // clang-format off
	DRVGPIO gpioA = DRVGPIO(GPIOA,
							  //5432109876453210
							  0b0000000011111111, //Owner
							  0b0000000000000000, //Direction 1=out
							  0b1111111111111111);//Polarity 1=active high
    // clang-format on
    GPIOpin dip0 = gpioA.getPin(0);
    GPIOpin dip1 = gpioA.getPin(1);
    GPIOpin dip2 = gpioA.getPin(2);
    GPIOpin dip3 = gpioA.getPin(3);
    GPIOpin dip4 = gpioA.getPin(4);
    GPIOpin dip5 = gpioA.getPin(5);
    GPIOpin dip6 = gpioA.getPin(6);
    GPIOpin dip7 = gpioA.getPin(7);

    // clang-format off
	DRVGPIO gpioB = DRVGPIO(GPIOB,
							  //5432109876453210
							  0b0000000000000011, //Owner
							  0b0000000000000000, //Direction 1=out
							  0b1111111111111111);//Polarity 1=active high
    // clang-format on
    GPIOpin dip8 = gpioB.getPin(0);
    GPIOpin dip9 = gpioB.getPin(1);

    // clang-format off
    DRVGPIO gpioC = DRVGPIO(GPIOC,
                              //5432109876453210
                              0b0010000000000000,//Owner
							  0b0010000000000000,//Direction 1=out
							  0b1101111111111111);//Polarity 1=active high
    // clang-format on
    GPIOpin LED = gpioC.getPin(13);

    REPORT(OSError::SevLog, OSError::TypeNone, "Initialization of DRV complete");

    // APP
    BinDecIO DMXAdress;

    DMXAdress.addBin(BinDecIO::PinValuePair(dip0, 0));
    DMXAdress.addBin(BinDecIO::PinValuePair(dip1, 1));
    DMXAdress.addBin(BinDecIO::PinValuePair(dip2, 2));
    DMXAdress.addBin(BinDecIO::PinValuePair(dip3, 3));
    DMXAdress.addBin(BinDecIO::PinValuePair(dip4, 4));
    DMXAdress.addBin(BinDecIO::PinValuePair(dip5, 5));
    DMXAdress.addBin(BinDecIO::PinValuePair(dip6, 6));
    DMXAdress.addBin(BinDecIO::PinValuePair(dip7, 7));

    while (1)
    {

        REPORT(OSError::SevLog, OSError::TypeNone, "BSP is alive!");
        //        LED.toggle();

        Delay(1000);

        if (LED && val > 0)
        {
            LED = false;
        }
        else
        {
            LED = true;
        }
    }

    // Suspend this task as we do not want to free memory
    Suspend();

    REPORT(OSError::SevFatal, OSError::TypeNone, "BSP return from suspend, this is not allowed");
}

BSP::~BSP() {}
