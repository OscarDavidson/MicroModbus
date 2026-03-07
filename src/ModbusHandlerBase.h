#ifndef MODBUSHANDLERBASE_H
#define MODBUSHANDLERBASE_H

#include <stdint.h>
#include <stddef.h>

class ModbusHandlerBase
{
public:
    virtual bool Handle(uint8_t* data, size_t& len) = 0;
    virtual uint8_t FunctionCode() = 0;
    virtual ~ModbusHandlerBase() { }

    bool SendException(const uint8_t code, uint8_t* data, size_t& len)
    {
        data[0] |= 0x80;
        data[1] = code;
        len = 2;
        return true;
    }

    static constexpr uint8_t UNSUPPORTED_FUNC = 0x01;
};

#endif // MODBUSHANDLERBASE_H
