/*************************************************
 * @file DRVSerial.h
 * @brief Serial driver interface
 *
 *************************************************/
#pragma once

#include "stdint.h"

#include "OSSema.h"

/*
 * Serial driver interface
 */
class DRVSerial
{
public:
    enum Status
    {
        Closed,
        Open,
        Error
    };

    DRVSerial();
    DRVSerial(bool useSemaphore);
    virtual ~DRVSerial();

    virtual Status open(uint32_t ms);
    virtual Status close();

    virtual void send(const char* buf);
    virtual void send(uint8_t* buf, uint32_t length);
    virtual void send(uint8_t);

    virtual uint32_t getBytesPending() = 0;
    virtual uint32_t readLine(char* buf, uint32_t bufferSize);
    virtual uint32_t readBuffer(uint8_t* buf, uint32_t bufferSize);
    virtual uint32_t readByte() = 0;

protected:
    virtual void sendByte(uint8_t) = 0;

    Status m_status;

private:
    bool   m_useSemaphore;
    OSSema m_semaphore;
};
