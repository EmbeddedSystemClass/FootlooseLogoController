/*************************************************
 * @file OSError.h
 * @brief Global error handling
 *
 *************************************************/
#pragma once

#include <string>
#include "stdint.h"

#include "drv/DRVSerial.h"

#define REPORT(sev, type, string) OSError::report(sev, type, __PRETTY_FUNCTION__, string);

/*
 * Serial driver interface
 */
class OSError
{
public:
    enum ErrorSeverity
    {
        SevLog,
        SevWarning,
        SevError,
        SevFatal,
        SevNone

    };

    enum ErrorType
    {
        TypeOverflow,
        TypeUnderflow,
        TypeNullPointer,
        TypeParam,
        TypeNone,
    };

    OSError();

    void setup(DRVSerial& output);

    static void report(ErrorSeverity sev, ErrorType type, std::string prefix, std::string str);

    // Make our reference to self public. it's static so no need to check if it's ok.
    static OSError* This;

protected:
private:
    void handleError(ErrorSeverity sev, ErrorType type, std::string prefix, std::string str);

    std::string getStringFromSeverity(ErrorSeverity sev);

    std::string getStringType(ErrorType Type);

    DRVSerial* m_serial;  //!< Serial output instance
};
