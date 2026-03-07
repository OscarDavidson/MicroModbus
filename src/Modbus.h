#ifndef MODBUS_H
#define MODBUS_H

#include "ModbusBase.h"
#include "ModbusHandlerBase.h"
#include "ModbusCounterType.h"

class Modbus : public ModbusBase
{
public:
    Modbus(ModbusHandlerBase* const * _handlers, ModbusCounters& _counters)
        : handlers(_handlers), counters(_counters)
    {

    }

    bool HandleMessage(uint8_t* data, size_t& length) override
    {
        size_t ix = 0;
//        bool handled = false;
        while (nullptr != handlers[ix])
        {
            if (handlers[ix]->FunctionCode() == data[0])
            {
                return handlers[ix]->Handle(data, length);
            }
            ++ix;
        }


        data[0] |= 0x80;
        data[1] = 0x01;
        length = 2;
        ++counters.exceptions;

        return true;
    }
private:
    ModbusHandlerBase* const * handlers = nullptr;
    ModbusCounters& counters;
};

#endif // MODBUS_H
