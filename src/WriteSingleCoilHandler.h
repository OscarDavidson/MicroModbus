#ifndef WRITESINGLECOILHANDLER_H
#define WRITESINGLECOILHANDLER_H

#include <ModbusHandlerBase.h>
#include <GpioBase.h>

class WriteSingleCoilHandler : public ModbusHandlerBase
{
public:
    WriteSingleCoilHandler(GpioBase& _gpio, const uint8_t* const _table,
                           const uint8_t _len)
        : gpio(_gpio), gpioTable(_table), gpioLen(_len) { }

    bool Handle(uint8_t* data, size_t& len) override
    {
        if (FUNC_CODE != data[FUNC_CODE_OFFSET])
        {
            data[FUNC_CODE_OFFSET] = FUNC_CODE;
            return SendException(UNSUPPORTED_FUNC, data, len);
        }

        if (((data[VALUE_HI] != 0xFF) && (data[VALUE_HI] != 0x00)) ||
                (data[VALUE_LO] != 0x00))
        {
            return SendException(INVALID_VALUE_ERROR, data, len);
        }

        const bool value = (data[VALUE_HI] == 0xFF);

        const uint16_t addr = (data[ADDR_HI] << 8) | data[ADDR_LO];
        if (gpioLen <= addr)
        {
            return SendException(INVALID_ADDR_ERROR, data, len);
        }

        gpio.Write(gpioTable[addr], value);

        return true;
    }

    uint8_t FunctionCode() override     { return FUNC_CODE; }

    static constexpr uint8_t FUNC_CODE = 0x05;
    static constexpr uint8_t INVALID_ADDR_ERROR = 0x02;
    static constexpr uint8_t INVALID_VALUE_ERROR = 0x03;

private:
    GpioBase& gpio;
    const uint8_t* const gpioTable = nullptr;
    const uint8_t gpioLen = 0;

    static constexpr uint8_t FUNC_CODE_OFFSET = 0;
    static constexpr uint8_t ADDR_HI = 1;
    static constexpr uint8_t ADDR_LO = 2;
    static constexpr uint8_t VALUE_HI = 3;
    static constexpr uint8_t VALUE_LO = 4;
};

#endif // WRITESINGLECOILHANDLER_H
