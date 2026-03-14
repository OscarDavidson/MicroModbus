#ifndef MODBUSFAKE_H
#define MODBUSFAKE_H

#include <ModbusBase.h>
#include <string.h>

class ModbusFake : public ModbusBase
{
public:
    bool HandleMessage(uint8_t* data, size_t& length) override
    {
        dataPtr = data;
        sizeCalled = length;
        memcpy(dataCopy, data, (length > 256)?256:length);
        length = returnSize;
        ++callCount;

        return returnValue;
    }

    uint8_t* dataPtr = nullptr;

    uint8_t dataCopy[256] = { 0 };

    uint8_t replyData[256] = { 0 };

    size_t sizeCalled = 0;
    size_t returnSize = 0;
    bool returnValue = false;

    size_t callCount = 0;
};

#endif // MODBUSFAKE_H
