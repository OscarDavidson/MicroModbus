#include "WriteMultiCoilsHandler.h"

bool WriteMultiCoilsHandler::Handle(uint8_t *data, size_t &len)
{
    if (FUNC_CODE != data[0])
    {
        data[0] = FUNC_CODE;
        return SendException(UNSUPPORTED_FUNC, data, len);
    }
    const uint16_t addr = (data[ADDR_HI] << 8) | data[ADDR_LO];
    const uint16_t quantity = (data[QUANT_HI] << 8) | data[QUANT_LO];
    const uint8_t byteCount = data[BYTE_COUNT];
    if ((byteCount != ((quantity / 8) + (quantity & 7)?1:0))
            || (gpioLen < quantity))
    {
        return SendException(INVALID_QUANTITY, data, len);
    }

    if (gpioLen < (addr + quantity))
    {
        return SendException(INVALID_ADDRESS, data, len);
    }

    for (uint8_t ix = 0; ix < quantity; ++ix)
    {
        gpio.Write(gpioTable[ix + addr], (data[(ix / 8) + DATA_OFFSET]
                                            & (1 << (ix & 7)))?1:0);
    }

    len = QUANT_LO + 1;
    return true;
}
