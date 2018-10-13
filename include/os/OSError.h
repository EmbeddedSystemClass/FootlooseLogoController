/*************************************************
 * @file OSError.h
 * @brief Global error handling
 *
 *************************************************/
#pragma once

#include "stdint.h"

#include "drv/DRVSerial.h"

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

    static void report(ErrorSeverity sev, ErrorType type, uint8_t user);

    // Make our reference to self public. it's static so no need to check if
    // it's ok.
    static OSError* This;

protected:
private:
    void handleError(ErrorSeverity sev, ErrorType type, uint8_t user);

    DRVSerial* m_serial;
};
