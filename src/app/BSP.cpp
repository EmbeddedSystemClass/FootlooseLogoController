/*************************************************
 * @file BSP.h
 * @brief
 *
 *************************************************/
#include "app/BSP.h"

#include <string>

#include "app/BinDecIO.h"
#include "app/DMXReceiver.h"
#include "app/Effects.h"
#include "app/EffectsController.h"
#include "app/GPIOBlinker.h"
#include "app/TaskStateMonitor.h"
#include "drv/DRVGPIO.h"
#include "drv/DRVSerialUart.h"
#include "drv/DRVSerialUsb.h"
#include "hal/HALUartSTM32F1.h"
#include "os/OSError.h"
#include "thread.hpp"

BSP::BSP(const char* name)
    : Thread(name, 600, 1)
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
    HALUartSTM32F1 dmxRxUart(USART1, 250000, HALUartSTM32F1::UartModeRx);
    HALUartSTM32F1 dmxTxUart(USART2, 250000, HALUartSTM32F1::UartModeTx);

    REPORTLOG("Initialization of HAL complete");

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

    // Uart drivers
    DRVSerialUart dmxRxUartDRV(dmxRxUart);

    REPORTLOG("Initialization of DRV complete");

    // APP

    // task monitor
    TaskStateMonitor taskMonitor("Monitor UI", LED);
    taskMonitor.Start();

    // DMX dip switch decoder
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
    cpp_freertos::Queue sendingQueue(1, 4);

    // Receiver
    DMXReceiver receiver(taskMonitor.GetHandle(), 1, dmxRxUartDRV, &dmxAddress, &receivingQueue, 4);

    // sender

    // effects controller
    EffectsController controller("Controller", taskMonitor.GetHandle(), 2, receivingQueue, sendingQueue, 20);

    // adding fixtures
    RGBFixture logoF(100, 0);

    controller.addFixture(logoF);

    // adding effects
    StaticColorEffect effectStaticColor;

    controller.addEffect(effectStaticColor, EffectsController::DmxRange(0, 120));
    controller.addEffect(effectStaticColor, EffectsController::DmxRange(150, 255));
    controller.addEffect(effectStaticColor, EffectsController::DmxRange(121, 149));

    // starting controller
    controller.Start();

    while (1)
    {
        receiver.insertTestDataInQueue();
        Delay(5000);
    }

    // Suspend this task as we do not want to free memory
    Suspend();

    REPORTLOG("BSP return from suspend, this is not allowed");
}

BSP::~BSP() {}
