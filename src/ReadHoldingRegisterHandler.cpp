#include "ReadHoldingRegisterHandler.h"

bool ReadHoldingRegisterHandler::Handle(uint8_t *data, size_t &len)
{
    if ((nullptr == data) || (REQUEST_LENGTH != len)) { return false; }
    if (FunctionCode() != data[0]) { return false; }

    uint16_t startAddr = (data[1] << 8) | data[2];
    const uint8_t regCount = data[4];

    if ((MAX_REG_COUNT < regCount) || (0 != data[3]) || (0 == regCount))
    {
        data[0] |= 0x80;
        data[1] = 0x03;
        len = 2;
        ++counters.exceptions;
        return true;
    }

    if ((startAddr + data[4]) > regLen)
    {
        data[0] |= 0x80;
        data[1] = 0x02;
        len = 2;
        ++counters.exceptions;
        return true;
    }

    data[1] = 2 * regCount;
    for (uint8_t ix = 0; ix < regCount; ++ix)
    {
        data[(ix * 2) + 2] = registers[startAddr + ix] >> 8;
        data[(ix * 2) + 3] = registers[startAddr + ix] & 0xFF;
    }
    len = data[1] + 2;
    return true;
}

uint8_t ReadHoldingRegisterHandler::FunctionCode()
{
    return FuncCode;
}
