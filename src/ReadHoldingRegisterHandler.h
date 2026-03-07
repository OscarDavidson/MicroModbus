#ifndef READHOLDINGREGISTERHANDLER_H
#define READHOLDINGREGISTERHANDLER_H

#include "ModbusHandlerBase.h"
#include "ModbusCounterType.h"

class ReadHoldingRegisterHandler : public ModbusHandlerBase
{
public:
    ReadHoldingRegisterHandler(uint16_t* _registers, size_t _regLen,
                               ModbusCounters& _counters)
        : registers(_registers), regLen(_regLen), counters(_counters)
    {

    }

    bool Handle(uint8_t* data, size_t& len) override;

    uint8_t FunctionCode() override;

    ~ReadHoldingRegisterHandler() {}

private:
    const uint16_t* registers = nullptr;
    const size_t regLen = 0;
    ModbusCounters& counters;

    static constexpr uint8_t REQUEST_LENGTH = 5;
    static constexpr uint8_t FuncCode = 0x03;
    static constexpr uint8_t MAX_REG_COUNT = 125;
};

#endif // READHOLDINGREGISTERHANDLER_H
