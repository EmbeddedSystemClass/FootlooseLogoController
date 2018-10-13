/*************************************************
 * @file DRVSerialUsb.h
 * @brief Serial CDC driver interface
 *
 *************************************************/
#pragma once

#include "stdint.h"

#include "DRVSerial.h"

/*
 * Serial driver interface
 */
class DRVSerialUsb : public DRVSerial
{
public:
    DRVSerialUsb();
    ~DRVSerialUsb();

    //    void send(char* buf);
    void send(uint8_t* buf, uint32_t length);

    uint32_t getBytesPending();
    uint32_t readLine(char* buf, uint32_t bufferSize);
    uint32_t readBuffer(uint8_t* buf, uint32_t bufferSize);
    uint32_t readByte();

protected:
    void sendByte(uint8_t);

private:
};
