#ifndef WRITEMULTICOILSHANDLER_H
#define WRITEMULTICOILSHANDLER_H

#include <ModbusHandlerBase.h>
#include <GpioBase.h>

class WriteMultiCoilsHandler : public ModbusHandlerBase
{
public:

    WriteMultiCoilsHandler(GpioBase& _gpio, const uint8_t* const _table,
                           const uint8_t _len)
        : gpio(_gpio), gpioTable(_table), gpioLen(_len) { }

    bool Handle(uint8_t* data, size_t& len) override;

    uint8_t FunctionCode() override     { return FUNC_CODE; }

    static constexpr uint8_t FUNC_CODE = 0x0F;

private:
    GpioBase& gpio;
    const uint8_t* const gpioTable = nullptr;
    const uint8_t gpioLen = 0;

    static constexpr uint8_t ADDR_LO = 2;
    static constexpr uint8_t ADDR_HI = 1;
    static constexpr uint8_t QUANT_LO = 4;
    static constexpr uint8_t QUANT_HI = 3;
    static constexpr uint8_t BYTE_COUNT = 5;
    static constexpr uint8_t DATA_OFFSET = 6;
    static constexpr uint8_t INVALID_ADDRESS = 0x02;
    static constexpr uint8_t INVALID_QUANTITY = 0x03;

};

#endif // WRITEMULTICOILSHANDLER_H
