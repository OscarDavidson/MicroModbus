#ifndef MODBUSBASE_H
#define MODBUSBASE_H

#include <stdint.h>
#include <stddef.h>

class ModbusBase
{
public:
    virtual bool HandleMessage(uint8_t* data, size_t& length) = 0;
};

#endif // MODBUSBASE_H
