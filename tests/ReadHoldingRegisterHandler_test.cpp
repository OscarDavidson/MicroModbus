#include <gtest/gtest.h>
#include <ModbusCounterType.h>
#include "ReadHoldingRegisterHandler.h"

class ReadRegisterSuite : public ::testing::Test
{
protected:
    static constexpr size_t REG_LEN = 16;
    uint16_t registers[REG_LEN] = { 0 };
    ModbusCounters counters;
    ReadHoldingRegisterHandler handler{registers, REG_LEN, counters};
};

TEST_F(ReadRegisterSuite, ReadFirstRegister)
{
    //Given a valid request to read the first register
    size_t requestSize = 5;
    uint8_t buffer[256] =
    {
        0x03, 0x00, 0x00, 0x00, 0x01
    };
    //And the first register is set to a value
    registers[0] = 0x1234;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the correct reply is placed in the buffer
    static constexpr uint8_t REPLY_REF[] =
    {
        0x03, 0x02, 0x12, 0x34
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
    EXPECT_EQ(counters.exceptions, 0);
}

TEST_F(ReadRegisterSuite, InvalidRegisterQuantity)
{
    //Given an invalid request with a quantity too high
    size_t requestSize = 5;
    uint8_t buffer[256] =
    {
        0x03, 0x00, 0x00, 0x00, 0x7E
    };
    //And the first register is set to a value
    registers[0] = 0x1234;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the correct exception is placed in the buffer
    static constexpr uint8_t REPLY_REF[] =
    {
        0x83, 0x03
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
    EXPECT_EQ(counters.exceptions, 1);
}

TEST_F(ReadRegisterSuite, InvalidStartingAddress)
{
    //Given an invalid request with a quantity too high
    size_t requestSize = 5;
    uint8_t buffer[256] =
    {
        0x03, 0x00, REG_LEN, 0x00, 0x01
    };
    //And the first register is set to a value
    registers[0] = 0x1234;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the correct exception is placed in the buffer
    static constexpr uint8_t REPLY_REF[] =
    {
        0x83, 0x02
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
    EXPECT_EQ(counters.exceptions, 1);
}

TEST_F(ReadRegisterSuite, ReadRegister_3)
{
    //Given a valid request to read the first register
    size_t requestSize = 5;
    uint8_t buffer[256] =
    {
        0x03, 0x00, 0x02, 0x00, 0x01
    };
    //And the first register is set to a value
    registers[0] = 0x1234;
    registers[1] = 0x5678;
    registers[2] = 0x90AB;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the correct reply is placed in the buffer
    static constexpr uint8_t REPLY_REF[] =
    {
        0x03, 0x02, 0x90, 0xAB
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
    EXPECT_EQ(counters.exceptions, 0);
}

TEST_F(ReadRegisterSuite, ReadThreeRegisters)
{
    //Given a valid request to read the first register
    size_t requestSize = 5;
    uint8_t buffer[256] =
    {
        0x03, 0x00, 0x00, 0x00, 0x03
    };
    //And the first register is set to a value
    registers[0] = 0x1234;
    registers[1] = 0x5678;
    registers[2] = 0x90AB;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the correct reply is placed in the buffer
    static constexpr uint8_t REPLY_REF[] =
    {
        0x03, 0x06, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
    EXPECT_EQ(counters.exceptions, 0);
}

TEST_F(ReadRegisterSuite, ReadFiveRegistersWithOffset)
{
    //Given a valid request to read the first register
    size_t requestSize = 5;
    uint8_t buffer[256] =
    {
        0x03, 0x00, 0x03, 0x00, 0x05
    };
    //And the first register is set to a value
    registers[3] = 0xCDEF;
    registers[4] = 0x5678;
    registers[5] = 0x1122;
    registers[6] = 0x90AB;
    registers[7] = 0x1234;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the correct reply is placed in the buffer
    static constexpr uint8_t REPLY_REF[] =
    {
        0x03, 0x0A, 0xCD, 0xEF, 0x56, 0x78, 0x11, 0x22, 0x90, 0xAB, 0x12, 0x34
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
    EXPECT_EQ(counters.exceptions, 0);
}
