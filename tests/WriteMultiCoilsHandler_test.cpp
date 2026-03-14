#include <gtest/gtest.h>
#include "ModbusCounterType.h"
#include "WriteMultiCoilsHandler.h"
#include "GpioFake.h"

class WriteMultiCoilsSuite : public ::testing::Test
{
protected:
    static constexpr size_t REG_LEN = 16;
    uint16_t registers[REG_LEN] = { 0 };
    ModbusCounters counters;
    static constexpr size_t GPIO_LEN = 32;
    GpioFake<GPIO_LEN> gpio;

    static constexpr uint8_t GPIO_LOOKUP[] =
    {
        0,
        12,
        14,
        2,
        1,
        5,
        23,
        3
    };

    WriteMultiCoilsHandler handler{gpio, GPIO_LOOKUP, sizeof(GPIO_LOOKUP)};
};

TEST_F(WriteMultiCoilsSuite, WriteSingleCoil_0)
{
    //Given a valid request to set coil 0 on
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo, ByteCount, value
        0x0F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01
    };
    size_t requestSize = 7;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the output is turned on
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[0]]);
    static constexpr uint8_t REPLY_REF[] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x01
    };
    //And the reply is the correct length and data
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteMultiCoilsSuite, WriteTooManyCoils_invalid)
{
    //Given an invalid request to set 16 coils
    uint8_t buffer[256] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x10, 0x02, 0xFF, 0xFF
    };
    size_t requestSize = 8;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the correct error exception reply is sent
    static constexpr uint8_t REPLY_REF[] =
    {
        0x8F, 0x03
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteMultiCoilsSuite, InvalidStartingAddress)
{
    //Given an invalid request, starting at address 8
    uint8_t buffer[256] =
    {
        0x0F, 0x00, 0x08, 0x00, 0x01, 0x01, 0x01
    };
    size_t requestSize = 7;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the correct error exception reply is sent
    static constexpr uint8_t REPLY_REF[] =
    {
        0x8F, 0x02
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteMultiCoilsSuite, WrongFunctionCode)
{
    //Given a request with the wrong function code
    uint8_t buffer[256] =
    {
        0x01, 0x00, 0x08, 0x00, 0x01, 0x01, 0x01
    };
    size_t requestSize = 7;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the correct error exception reply is sent
    static constexpr uint8_t REPLY_REF[] =
    {
        0x8F, 0x01
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteMultiCoilsSuite, WriteOnlyLastOutput)
{
    //Given a valid request to write only the last output, output 7
    uint8_t buffer[256] =
    {
        0x0F, 0x00, 0x07, 0x00, 0x01, 0x01, 0x01
    };
    size_t requestSize = 7;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the output is turned on
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[7]]);
    //And the correct reply is sent
    static constexpr uint8_t REPLY_REF[] =
    {
        0x0F, 0x00, 0x07, 0x00, 0x01
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteMultiCoilsSuite, WriteSingleCoil_1)
{
    //Given a valid request to set coil 1 on
    uint8_t buffer[256] =
    {
        0x0F, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01
    };
    size_t requestSize = 7;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the output is turned on
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[1]]);
    //And the correct reply is sent
    static constexpr uint8_t REPLY_REF[] =
    {
        0x0F, 0x00, 0x01, 0x00, 0x01
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteMultiCoilsSuite, WriteTwoCoils)
{
    //Given a valid request to set two coils
    uint8_t buffer[256] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x02, 0x01, 0x03
    };
    size_t requestSize = 7;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then both outputs are turned on
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[0]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[1]]);
    //And the correct reply is sent
    static constexpr uint8_t REPLY_REF[] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x02
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteMultiCoilsSuite, WriteAll8Coils_0x55)
{
    //Given a valid request to set all 8 coils
    uint8_t buffer[256] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x08, 0x01, 0x55
    };
    size_t requestSize = 7;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then only the correct outputs are turned on
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[0]]);
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[1]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[2]]);
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[3]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[4]]);
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[5]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[6]]);
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[7]]);
    //And the correct reply is sent
    static constexpr uint8_t REPLY_REF[] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x08
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteMultiCoilsSuite, WriteAll8Coils_0xAA)
{
    //Given a valid request to set all 8 coils
    uint8_t buffer[256] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x08, 0x01, 0xAA
    };
    size_t requestSize = 7;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then only the correct outputs are turned on
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[0]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[1]]);
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[2]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[3]]);
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[4]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[5]]);
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[6]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[7]]);
    //And the correct reply is sent
    static constexpr uint8_t REPLY_REF[] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x08
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteMultiCoilsSuite, WriteAll8Coils_0xFF)
{
    //Given a valid request to set all 8 coils
    uint8_t buffer[256] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x08, 0x01, 0xFF
    };
    size_t requestSize = 7;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then all the outputs are turned on
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[0]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[1]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[2]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[3]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[4]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[5]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[6]]);
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[7]]);
    //And the correct reply is sent
    static constexpr uint8_t REPLY_REF[] =
    {
        0x0F, 0x00, 0x00, 0x00, 0x08
    };
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}
