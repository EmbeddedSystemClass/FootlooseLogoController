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
#include "app/UserInterface.h"
#include "drv/CAT5932.h"
#include "drv/DRV7Segment.h"
#include "drv/DRV7SegmentDisplay.h"
#include "drv/DRVGPIO.h"
#include "drv/DRVSerialUart.h"
#include "hal/HALTimerSTM32F1.h"
#include "hal/HALUartSTM32F1.h"
#include "hal/HALi2cSTM32.h"
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

    HALI2CSTM32 uiI2C(I2C1, HALI2C::Frequency100k);

    REPORTLOG("Initialization of HAL complete");

    // Driver

    // clang-format off
    DRVGPIO gpioA = DRVGPIO(GPIOA,
                            //5432109876453210
                            0b0000111111111111,   // Owner
                            0b0000001000000100,   // Direction 1=out
                            0b1111111111111111);  // Polarity 1=active high
    // clang-format on
    GPIOpinSTM32 uart2Tx = gpioA.getPin(2);
    GPIOpinSTM32 uart2Rx = gpioA.getPin(3);
    GPIOpinSTM32 dip0    = gpioA.getPin(0);
    GPIOpinSTM32 dip1    = gpioA.getPin(1);
    GPIOpinSTM32 dip2    = gpioA.getPin(4);
    GPIOpinSTM32 dip3    = gpioA.getPin(5);
    GPIOpinSTM32 dip4    = gpioA.getPin(6);
    GPIOpinSTM32 dip5    = gpioA.getPin(7);
    GPIOpinSTM32 dip6    = gpioA.getPin(8);
    GPIOpinSTM32 uart1Tx = gpioA.getPin(9);
    GPIOpinSTM32 uart1Rx = gpioA.getPin(10);

    GPIOpinSTM32 dip7 = gpioA.getPin(11);

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
    GPIOpinSTM32 dip8         = gpioB.getPin(0);
    GPIOpinSTM32 dip9         = gpioB.getPin(1);
    GPIOpinSTM32 ledPower     = gpioB.getPin(4);
    GPIOpinSTM32 ledStatusPin = gpioB.getPin(5);

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

    CAT5932 ledDriver1(uiI2C, 0b01100000);
    CAT5932 ledDriver2(uiI2C, 0b01100001);
    ledDriver1.initDevice();
    ledDriver2.initDevice();

    GPIORemotePin uiLedStatus = ledDriver2.getPin(14);
    GPIORemotePin uiLedPower  = ledDriver2.getPin(15);

    GPIORemotePin uiSeg1A   = ledDriver1.getPin(1);
    GPIORemotePin uiSeg1B   = ledDriver1.getPin(0);
    GPIORemotePin uiSeg1C   = ledDriver2.getPin(6);
    GPIORemotePin uiSeg1D   = ledDriver2.getPin(5);
    GPIORemotePin uiSeg1E   = ledDriver2.getPin(4);
    GPIORemotePin uiSeg1F   = ledDriver1.getPin(2);
    GPIORemotePin uiSeg1G   = ledDriver1.getPin(3);
    GPIORemotePin uiSeg1Dot = ledDriver2.getPin(7);
    GPIORemotePin uiSeg2A   = ledDriver1.getPin(5);
    GPIORemotePin uiSeg2B   = ledDriver1.getPin(4);
    GPIORemotePin uiSeg2C   = ledDriver1.getPin(1);
    GPIORemotePin uiSeg2D   = ledDriver1.getPin(2);
    GPIORemotePin uiSeg2E   = ledDriver1.getPin(0);
    GPIORemotePin uiSeg2F   = ledDriver1.getPin(6);
    GPIORemotePin uiSeg2G   = ledDriver1.getPin(7);
    GPIORemotePin uiSeg2Dot = ledDriver1.getPin(3);
    GPIORemotePin uiSeg3A   = ledDriver1.getPin(9);
    GPIORemotePin uiSeg3B   = ledDriver1.getPin(8);
    GPIORemotePin uiSeg3C   = ledDriver1.getPin(14);
    GPIORemotePin uiSeg3D   = ledDriver1.getPin(13);
    GPIORemotePin uiSeg3E   = ledDriver1.getPin(12);
    GPIORemotePin uiSeg3F   = ledDriver1.getPin(10);
    GPIORemotePin uiSeg3G   = ledDriver1.getPin(11);
    GPIORemotePin uiSeg3Dot = ledDriver1.getPin(15);

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

    // UI
    DRV7Segment uiSegment1(uiSeg1A, uiSeg1B, uiSeg1C, uiSeg1D, uiSeg1E, uiSeg1F, uiSeg1G, &uiSeg1Dot);
    DRV7Segment uiSegment2(uiSeg2A, uiSeg2B, uiSeg2C, uiSeg2D, uiSeg2E, uiSeg2F, uiSeg2G, &uiSeg2Dot);
    DRV7Segment uiSegment3(uiSeg3A, uiSeg3B, uiSeg3C, uiSeg3D, uiSeg3E, uiSeg3F, uiSeg3G, &uiSeg3Dot);

    DRV7SegmentDisplay uiDisplay;
    uiDisplay.addSegment(&uiSegment1);
    uiDisplay.addSegment(&uiSegment2);
    uiDisplay.addSegment(&uiSegment3);

    UserInterface ui;
    ui.Start();

    uint16_t cnt = 0;
    while (1)
    {
        uiLedPower.toggle();
        uiLedStatus = !uiLedPower;

        uiDisplay.setNumber(cnt);

        ledDriver1.sendUpdate();
        ledDriver2.sendUpdate();
        cnt++;
        if (cnt > 999) cnt = 0;
        Delay(10);
    }

    // Suspend this task as we do not want to free memory
    Suspend();

    REPORTLOG("BSP return from suspend, this is not allowed");
}

BSP::~BSP() {}
