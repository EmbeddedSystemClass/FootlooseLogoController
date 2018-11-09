/*************************************************
 * @file OSError.h
 * @brief Global error handling
 *
 *************************************************/

#include <string>
#include "stdint.h"

#include "os/OSError.h"

#include "drv/DRVSerial.h"

OSError* OSError::This = NULL;

OSError::OSError(DRVSerial& output)
    : m_serial(NULL)

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

void OSError::report(ErrorSeverity sev, ErrorType type, std::string prefix, std::string str) { This->handleError(sev, type, prefix, str); }

void OSError::handleError(ErrorSeverity sev, ErrorType type, std::string prefix, std::string str)
{
    std::string severityString = getStringFromSeverity(sev);
    std::string typeString     = getStringType(type);
    std::string output;

    output = severityString + "_" + typeString + ": " + prefix + ":" + str + "\n\r";

    m_serial->send(output);

    if (sev == SevFatal)
    {
        while (1)
            ;
    }
}

std::string OSError::getStringFromSeverity(ErrorSeverity sev)
{
    std::string retVal;
    switch (sev)
    {
    case SevLog:
        retVal = "LOG";
        break;
    case SevWarning:
        retVal = "WAR";
        break;
    case SevError:
        retVal = "ERR";
        break;
    case SevFatal:
        retVal = "FAT";
        break;
    case SevNone:
    default:
        retVal = "---";
        break;
    }
    return retVal;
}

std::string OSError::getStringType(ErrorType type)
{
    std::string retVal;
    switch (type)
    {
    case TypeParam:
        retVal = "PAR";
        break;
    case TypeUnderflow:
        retVal = "UFL";
        break;
    case TypeOverflow:
        retVal = "OFL";
        break;
    case TypeNullPointer:
        retVal = "NUL";
        break;
    case TypeNone:
    default:
        retVal = "---";
        break;
    }
    return retVal;
}
