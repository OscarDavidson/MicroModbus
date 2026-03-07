#ifndef READINPUTSHANDLER_H
#define READINPUTSHANDLER_H

#include <ModbusHandlerBase.h>
#include <GpioBase.h>
#include <string.h>

class ReadInputsHandler : public ModbusHandlerBase
{
public:
    ReadInputsHandler(GpioBase& _gpio, const uint8_t* const _table,
                      const uint8_t _len)
   : gpio(_gpio), gpioTable(_table), gpioLen(_len) { }

    bool Handle(uint8_t* data, size_t& len) override
    {
        if (nullptr == gpioTable) { return false; }
        if (REQUEST_LENGTH != len)
        {
            return false;
        }

        if (FUNC_CODE != data[FUNC_CODE_OFFSET])
        {
            data[FUNC_CODE_OFFSET] = FUNC_CODE;
            return SendException(UNSUPPORTED_FUNC, data, len);
        }

        const uint16_t quantity = (data[QUANTITY_HI] << 8) | data[QUANTITY_LO];

        if ((0 == quantity) || (gpioLen < quantity))
        {
            return SendException(INVALID_QUANTITY_ERROR, data, len);
        }

        const uint16_t addr = (data[ADDR_HI] << 8) | data[ADDR_LO];

        if ((gpioLen <= addr) || ((addr + quantity) > gpioLen))
        {
            return SendException(INVALID_ADDR_ERROR, data, len);
        }

        const uint8_t byteSize = (quantity >> 3)
                + (((quantity & 7) == 0)?0:1);
        data[BYTE_COUNT_OFFSET] = byteSize;

        memset(data + 2, 0, byteSize);
        for (uint8_t ix = 0; ix < quantity; ++ix)
        {
            const uint8_t bytePos = 2 + (ix >> 3);
            const uint8_t bitPos = ix & 0x07;
            if (gpio.Read(gpioTable[ix + addr]))
            {
                data[bytePos] |= 1 << bitPos;
            }
        }

        len = byteSize + 2;

        return true;
    }

    uint8_t FunctionCode() override     { return FUNC_CODE; }

    static constexpr uint8_t FUNC_CODE = 0x02;
    static constexpr uint8_t INVALID_ADDR_ERROR = 0x02;
    static constexpr uint8_t INVALID_QUANTITY_ERROR = 0x03;
    static constexpr uint8_t REQUEST_LENGTH = 5;

private:
    GpioBase& gpio;
    const uint8_t* const gpioTable = nullptr;
    const uint8_t gpioLen = 0;

    static constexpr uint8_t FUNC_CODE_OFFSET = 0;
    static constexpr uint8_t ADDR_HI = 1;
    static constexpr uint8_t ADDR_LO = 2;
    static constexpr uint8_t QUANTITY_HI = 3;
    static constexpr uint8_t QUANTITY_LO = 4;

    static constexpr uint8_t BYTE_COUNT_OFFSET = 1;
};

#endif // READINPUTSHANDLER_H
