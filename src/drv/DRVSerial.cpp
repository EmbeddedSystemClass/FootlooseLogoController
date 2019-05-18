/*************************************************
 * @file DRVSerial.h
 * @brief Serial driver interface implementation
 *
 *************************************************/

#include "drv/DRVSerial.h"

#include <string>

DRVSerial::DRVSerial(bool useSemaphore)
    : m_status(Closed)
    , m_semaphore(NULL)
{
    if (useSemaphore)
    {
        m_semaphore = new cpp_freertos::BinarySemaphore(true);
    }
}

DRVSerial::~DRVSerial() {}

DRVSerial::Status DRVSerial::open(uint32_t ms)
{
    if (m_semaphore != NULL)
    {
        if (ms == 0) ms = 1;
        if (m_semaphore->Take(ms))
        {
            m_status = Open;
        }
        else
        {
            m_status = Error;
        }
    }
    else
    {
        m_status = Open;
    }
    return m_status;
}

DRVSerial::Status DRVSerial::close()
{
    if (m_semaphore != NULL)
    {
        if (m_semaphore->Give())
        {
            m_status = Closed;
        }
        else
        {
            m_status = Error;
        }
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

void DRVSerial::send(std::string buf)
{

    for (std::string::iterator it = buf.begin(); it != buf.end(); ++it)
    {
        send(*it);
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

uint32_t DRVSerial::readLine(char* buf, uint32_t bufferSize)
{
    (void)buf;
    (void)bufferSize;
    return 0;
}

uint32_t DRVSerial::readBuffer(uint8_t* buf, uint32_t bufferSize)
{
    (void)buf;
    (void)bufferSize;
    return 0;
}

uint32_t DRVSerial::writeBuffer(uint8_t* buf, uint32_t bufferSize)
{
    (void)buf;
    (void)bufferSize;
    return 0;
}

void DRVSerial::registerCallback(HALUart::CallbackFunction f, HALUart::CallBack type, void* parameter)
{
    (void)f;
    (void)type;
    (void)parameter;
}
