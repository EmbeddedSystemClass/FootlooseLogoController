/*************************************************
 * @file HALUart.h
 * @brief HAL uart
 *
 *************************************************/

#include "stdint.h"

/*
 * Serial driver interface
 */
class HALUart
{
public:
    HALUart();
    virtual ~HALUart();

    virtual void open(uint32_t baudRate, uint8_t dataBits, uint8_t stopBits);

    virtual void close();

    virtual void sendByte(uint8_t) = 0;

    virtual uint32_t readByte() = 0;

private:
};
