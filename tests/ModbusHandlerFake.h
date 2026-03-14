#ifndef MODBUSHANDLERFAKE_H
#define MODBUSHANDLERFAKE_H

#include <ModbusHandlerBase.h>

class ModbusHandlerFake : public ModbusHandlerBase
{
public:
    ModbusHandlerFake(uint8_t func) : funcCode(func) { }

    bool Handle(uint8_t* data, size_t& len)
    {
        ++callCount;
        return false;
    }

    uint8_t FunctionCode()
    {
        return funcCode;
    }

    uint8_t funcCode = 0;

    uint8_t* replyData = nullptr;
    size_t replyLen = 0;

    size_t callCount = 0;
};

#endif // MODBUSHANDLERFAKE_H
