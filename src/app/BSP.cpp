/*************************************************
 * @file BSP.h
 * @brief
 *
 *************************************************/
#include "app/BSP.h"

#include <string>

#include "app/BinDecIO.h"
#include "app/DMXReceiver.h"
#include "app/GPIOBlinker.h"
#include "app/TaskStateMonitor.h"
#include "drv/DRVGPIO.h"
#include "drv/DRVSerialUart.h"
#include "drv/DRVSerialUsb.h"
#include "hal/HALUartSTM32F1.h"
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

    REPORTLOG("System up!");
    char datetime[50];
    sprintf(datetime, "Build date %s, %s\n\r", __DATE__, __TIME__);
    REPORTLOG(datetime);

    // HAL
    HALUartSTM32F1 dmxRxUart(USART1, 9600, HALUartSTM32F1::UartModeRx);
    HALUartSTM32F1 dmxTxUart(USART2, 9600, HALUartSTM32F1::UartModeTx);

    REPORTLOG("Initialization of HAL complete");

    // Driver

    // clang-format off
	DRVGPIO gpioA = DRVGPIO(GPIOA,
							  //5432109876453210
							  0b0000001111110011, //Owner
							  0b0000000000000000, //Direction 1=out
							  0b1111111111111111);//Polarity 1=active high
    // clang-format on
    GPIOpin dip0 = gpioA.getPin(0);
    GPIOpin dip1 = gpioA.getPin(1);
    GPIOpin dip2 = gpioA.getPin(4);
    GPIOpin dip3 = gpioA.getPin(5);
    GPIOpin dip4 = gpioA.getPin(6);
    GPIOpin dip5 = gpioA.getPin(7);
    GPIOpin dip6 = gpioA.getPin(8);
    GPIOpin dip7 = gpioA.getPin(9);

    // UART2 pins
    gpioA.setAlternateFunction(2, 1);

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

    // Uart drivers
    DRVSerialUart dmxRxUartDRV(dmxRxUart);

    REPORTLOG("Initialization of DRV complete");

    // APP

    // task monitor
    TaskStateMonitor taskMonitor("Monitor UI", LED);
    taskMonitor.Start();

    // DMX dipswitch decoder
    BinDecIO dmxAddress;
    dmxAddress.addBin(BinDecIO::PinValuePair(dip0, 0));
    dmxAddress.addBin(BinDecIO::PinValuePair(dip1, 1));
    dmxAddress.addBin(BinDecIO::PinValuePair(dip2, 2));
    dmxAddress.addBin(BinDecIO::PinValuePair(dip3, 3));
    dmxAddress.addBin(BinDecIO::PinValuePair(dip4, 4));
    dmxAddress.addBin(BinDecIO::PinValuePair(dip5, 5));
    dmxAddress.addBin(BinDecIO::PinValuePair(dip6, 6));
    dmxAddress.addBin(BinDecIO::PinValuePair(dip7, 7));

    // Queue for received DMX channels
    cpp_freertos::Queue receivingQueue(10, 4);

    // Receiver
    DMXReceiver receiver(taskMonitor.GetHandle(), 1, dmxRxUartDRV, &dmxAddress, &receivingQueue, 4);

    while (1)
    {
        //        receiver.insertTestDataInQueue();
        uint8_t data[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
        dmxTxUart.send(data, 10);
        Delay(5000);
    }

    // Suspend this task as we do not want to free memory
    Suspend();

    REPORTLOG("BSP return from suspend, this is not allowed");
}

BSP::~BSP() {}
