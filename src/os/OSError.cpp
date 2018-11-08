/*************************************************
 * @file OSError.h
 * @brief Global error handling
 *
 *************************************************/

#include "stdint.h"

#include "os/OSError.h"

#include "drv/DRVSerial.h"

OSError* OSError::This = new OSError();

OSError::OSError()
    : m_serial(NULL)

{
}

void OSError::setup(DRVSerial& output)
{
    m_serial = &output;
    if (m_serial->open(0) != DRVSerial::Open)
    {
        // do not allow the application to start as we have no error
        while (1)
            ;
    }
    This = this;
}

void OSError::report(ErrorSeverity sev, ErrorType type, uint8_t user)
{
    This->handleError(sev, type, user);
}

void OSError::handleError(ErrorSeverity sev, ErrorType type, uint8_t user)
{
    uint8_t msg[10];

    msg[0] = sev;
    msg[1] = ':';
    msg[2] = type;
    msg[3] = ':';
    msg[4] = user;
    msg[5] = '\n';

    m_serial->send(msg, 6);
}
