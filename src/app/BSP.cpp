/*************************************************
 * @file BSP.h
 * @brief
 *
 *************************************************/
#include "app/BSP.h"

#include <string>

#include "app/BinDecIO.h"
#include "app/DMXReceiver.h"
#include "app/DMXTransmitter.h"
#include "app/Effects.h"
#include "app/EffectsController.h"
#include "app/GPIOBlinker.h"
#include "app/TaskStateMonitor.h"
#include "drv/DRVGPIO.h"
#include "drv/DRVSerialUart.h"
//#include "drv/DRVSerialUsb.h"
#include "hal/HALTimerSTM32F1.h"
#include "hal/HALUartSTM32F1.h"
#include "os/OSError.h"
#include "thread.hpp"

BSP::BSP(const char* name)
    : Thread(name, 1000, 1)
{
}

void BSP::Run()
{
    // Task is created to initialize all software components
    // Make sure our error handler is up before anything else

    // output for error, ideal would be uart as this can also capture startup issues
    //    DRVSerialUsb usbCDC;

    HALUartSTM32F1 serialDebug(USART3, 115200, HALUartSTM32F1::UartModeRxTx);
    DRVSerialUart  serialDebugDrv(serialDebug);

    OSError errorHandler;
    errorHandler.setup(serialDebugDrv);

    Delay(1000);

    REPORTLOG("System up!");
    char datetime[50];
    sprintf(datetime, "Build date %s, %s\n\r", __DATE__, __TIME__);
    REPORTLOG(datetime);

    // HAL
    HALUartSTM32F1  dmxRxUart(USART2, 250000, HALUartSTM32F1::UartModeRx);
    HALUartSTM32F1  dmxTxUart(USART1, 250000, HALUartSTM32F1::UartModeTx);
    HALTimerSTM32F1 dmxBreakCaptureTimer(TIM2, HALTimer::TimerInputCapture, (HALTimer::TimerChannel3 || HALTimer::TimerChannel4));

    REPORTLOG("Initialization of HAL complete");

    // Driver

    // clang-format off
    DRVGPIO gpioA = DRVGPIO(GPIOA,
                            //5432109876453210
                            0b0000111111111111,   // Owner
                            0b0000001000000100,   // Direction 1=out
                            0b1111111111111111);  // Polarity 1=active high
    // clang-format on
    GPIOpin uart2Tx = gpioA.getPin(2);
    GPIOpin uart2Rx = gpioA.getPin(3);
    GPIOpin dip0    = gpioA.getPin(0);
    GPIOpin dip1    = gpioA.getPin(1);
    GPIOpin dip2    = gpioA.getPin(4);
    GPIOpin dip3    = gpioA.getPin(5);
    GPIOpin dip4    = gpioA.getPin(6);
    GPIOpin dip5    = gpioA.getPin(7);
    GPIOpin dip6    = gpioA.getPin(8);
    GPIOpin uart1Tx = gpioA.getPin(9);
    GPIOpin uart1Rx = gpioA.getPin(10);

    GPIOpin dip7 = gpioA.getPin(11);

    // UART2 pins
    //    gpioA.setAlternateFunction(2, 1);
    //    gpioA.setAlternateFunction(3, 0);
    uart1Tx.setAlternateFunction();
    uart1Rx.setAlternateFunction();
    uart2Tx.setAlternateFunction();
    uart2Rx.setAlternateFunction();

    // clang-format off
	DRVGPIO gpioB = DRVGPIO(GPIOB,
							  //5432109876543210
							  0b0000000000110011, //Owner
							  0b0000000000110000, //Direction 1=out
							  0b1111111111111111);//Polarity 1=active high
    // clang-format on
    GPIOpin dip8         = gpioB.getPin(0);
    GPIOpin dip9         = gpioB.getPin(1);
    GPIOpin ledPower     = gpioB.getPin(4);
    GPIOpin ledStatusPin = gpioB.getPin(5);

    GPIOBlinker ledStatus(ledStatusPin);

    //    ledStatus.setFrequency(2);
    //    ledStatus.setDutyCycle(10)

    ledPower = true;

    // clang-format off
	DRVGPIO gpioC = DRVGPIO(GPIOC,
						  //5432109876453210
						  0b0010000000000000,//Owner
						  0b0010000000000000,//Direction 1=out
						  0b1101111111111111);//Polarity 1=active high
    // clang-format on

    // Uart drivers
    DRVSerialUart dmxRxUartDRV(dmxRxUart);
    DRVSerialUart dmxTxUartDRV(dmxTxUart);

    REPORTLOG("Initialization of DRV complete");

    //    APP

    //        task monitor
    TaskStateMonitor taskMonitor("Monitor UI", ledStatus);
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

    // Queue for dmx sending
    cpp_freertos::Queue transmittingQueue(10, sizeof(DMXTransmitter::DMXQueueItem));

    // Receiver
    DMXReceiver receiver(taskMonitor.GetHandle(), 1, dmxRxUartDRV, dmxBreakCaptureTimer, &dmxAddress, &receivingQueue, 4);
    receiver.Start();

    // Transmitter
    DMXTransmitter transmitter(taskMonitor.GetHandle(), 2, uart1Tx, dmxTxUartDRV, 100, &transmittingQueue);
    transmitter.Start();

    // sender

    // effects controller
    EffectsController controller("Controller", taskMonitor.GetHandle(), 2, receivingQueue, transmittingQueue, 20);

    // adding fixtures
    //    RGBFixture logoF(100, 0);
    //    RGBFixture logoO1(103, 1);
    //    RGBFixture logoO2(106, 2);
    //    RGBFixture logoT(109, 3);
    //    RGBFixture logoL(112, 4);
    //    RGBFixture logoO3(115, 5);
    //    RGBFixture logoO4(118, 6);
    //    RGBFixture logoS(121, 7);
    //    RGBFixture logoE(124, 8);

    RGBFixture logoF(1, 0);
    RGBFixture logoO1(4, 1);
    RGBFixture logoO2(7, 2);
    RGBFixture logoT(10, 3);
    RGBFixture logoL(13, 4);
    RGBFixture logoO3(16, 5);
    RGBFixture logoO4(19, 6);
    RGBFixture logoS(22, 7);
    RGBFixture logoE(27, 8);

    controller.addFixture(logoF);
    controller.addFixture(logoO1);
    controller.addFixture(logoO2);
    controller.addFixture(logoT);
    controller.addFixture(logoL);
    controller.addFixture(logoO3);
    controller.addFixture(logoO4);
    controller.addFixture(logoS);
    controller.addFixture(logoE);

    // adding effects
    StaticColorEffect effectStaticColor;
    CycleColorEffect  effectCycleColor;

    controller.addEffect(effectStaticColor, EffectsController::DmxRange(0, 10));
    controller.addEffect(effectCycleColor, EffectsController::DmxRange(11, 20));

    // starting controller
    controller.Start();

    //    receiver.insertTestDataInQueue();

    while (1)
    {

        Delay(200);
    }

    // Suspend this task as we do not want to free memory
    Suspend();

    REPORTLOG("BSP return from suspend, this is not allowed");
}

BSP::~BSP() {}
