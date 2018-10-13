/*************************************************
 * @file DRVSerial.h
 * @brief Serial driver interface implementation
 *
 *************************************************/

#include "drv/DRVSerial.h"

DRVSerial::DRVSerial()
    : m_status(Closed)
    , m_useSemaphore(false)
    , m_semaphore(1)
{
}

DRVSerial::DRVSerial(bool useSemaphore)
    : m_status(Closed)
    , m_useSemaphore(useSemaphore)
    , m_semaphore(1)
{
}

DRVSerial::~DRVSerial() {}

DRVSerial::Status DRVSerial::open(uint32_t ms)
{
    if (m_useSemaphore)
    {
        if (m_semaphore.wait(ms) > 0)
        {
            m_status = Open;
        }
        // check if we got is
    }
    else
    {
        m_status = Open;
    }
    return m_status;
}

DRVSerial::Status DRVSerial::close()
{
    if (m_useSemaphore)
    {
        m_semaphore.release();
    }
    else
    {
        m_status = Closed;
    }
    return m_status;
}

void DRVSerial::send(const char* buf)
{
    const char* p;

    for (p = buf; *p != '\0'; p++)
    {

        send(*p);
    }
}

void DRVSerial::send(uint8_t* buf, uint32_t length)
{
    uint32_t bufferIdx = 0;

    for (bufferIdx = 0; bufferIdx < length; bufferIdx++)
    {
        send(buf[bufferIdx]);
    }
}

void DRVSerial::send(uint8_t data)
{
    if (m_status == Open)
    {
        sendByte(data);
    }
}

uint32_t DRVSerial::readLine(char* buf, uint32_t bufferSize) {}

uint32_t DRVSerial::readBuffer(uint8_t* buf, uint32_t bufferSize) {}
