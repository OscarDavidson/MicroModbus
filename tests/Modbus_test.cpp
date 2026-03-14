#include <gtest/gtest.h>
#include <ReadHoldingRegisterHandler.h>
#include <Modbus.h>
#include <ModbusHandlerBase.h>
#include <ModbusHandlerFake.h>
#include <SerialModbus.h>
#include "SerialFake.h"

class ModbusSuite : public ::testing::Test
{
protected:
    static constexpr size_t REG_LEN = 16;
    static constexpr uint8_t ADDRESS = 0x12;

    static constexpr auto TIMEOUT = CharTimeoutUs[(uint8_t)DEFAULT_BAUD];

    uint16_t registers[REG_LEN] = { 0 };
    ReadHoldingRegisterHandler handler{registers, REG_LEN, counters};
    ModbusHandlerFake handlerFake1{0x05};
    ModbusHandlerFake handlerFake2{0x06};

    ModbusCounters counters;

    static constexpr size_t HANDLER_COUNT = 4;
    ModbusHandlerBase* handlers[HANDLER_COUNT] =
    {
        &handler,
        &handlerFake1,
        &handlerFake2,
        nullptr
    };

    Modbus modbus{handlers, counters};

    SerialFake serial;
    SerialModbus serialModbus{serial, modbus, ADDRESS, counters};
};

TEST_F(ModbusSuite, HandleMessage)
{
    //Given a valid request to read the first register
    size_t requestSize = 5;
    uint8_t buffer[256] =
    {
        0x12, 0x05, 0x00, 0xD2, 0x95
    };
    ASSERT_EQ(handlerFake1.callCount, 0);
    serial.AddBlockToRead(buffer, 5);
    serialModbus.Clocktick(0);
    serialModbus.Clocktick(TIMEOUT * 2);
    ASSERT_EQ(counters.busMessages, 1);
    ASSERT_EQ(counters.slaveCount, 1);
    ASSERT_EQ(counters.commsErrors, 0);
    ASSERT_EQ(handlerFake1.callCount, 1);
}

TEST_F(ModbusSuite, UnhandledMessages_ExceptionSent)
{
    //Given a valid request to read the first register
    size_t requestSize = 5;
    uint8_t buffer[256] =
    {
        0x12, 0x23, 0x00, 0xC8, 0xF5
    };
    ASSERT_EQ(handlerFake1.callCount, 0);
    serial.AddBlockToRead(buffer, 5);
    serialModbus.Clocktick(0);
    serialModbus.Clocktick(TIMEOUT * 2);
    ASSERT_EQ(counters.busMessages, 1);
    ASSERT_EQ(counters.slaveCount, 1);
    ASSERT_EQ(counters.commsErrors, 0);
    ASSERT_EQ(handlerFake1.callCount, 0);
    EXPECT_EQ(counters.exceptions, 1);

    ASSERT_EQ(5, serial.writeData.size());
    ASSERT_EQ(0x12, serial.writeData.front());
    serial.writeData.pop();
    ASSERT_EQ(0xA3, serial.writeData.front());
    serial.writeData.pop();
    ASSERT_EQ(0x01, serial.writeData.front());
    serial.writeData.pop();
    ASSERT_EQ(0x68, serial.writeData.front());
    serial.writeData.pop();
    ASSERT_EQ(0xF5, serial.writeData.front());
    serial.writeData.pop();
}
