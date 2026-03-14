#include <gtest/gtest.h>
#include "WriteSingleCoilHandler.h"
#include "GpioFake.h"

class WriteSingleCoilSuite : public ::testing::Test
{
protected:
    static constexpr size_t REG_LEN = 16;
    uint16_t registers[REG_LEN] = { 0 };

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

    static constexpr size_t GPIO_LEN = 32;
    GpioFake<GPIO_LEN> gpio;

    WriteSingleCoilHandler handler{gpio, GPIO_LOOKUP, sizeof(GPIO_LOOKUP)};

    static constexpr uint8_t FUNC = 0x05;
    static constexpr uint8_t ERROR = 0x85;
};

TEST_F(WriteSingleCoilSuite, InvalidFunctionCode)
{
    //Given an invalid request with the wrong function code
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo, value hi, value lo
        0x42, 0x00, 0x00, 0x00, 0x00
    };
    size_t requestSize = 5;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        ERROR, 0x01
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, InvalidOutputValue)
{
    //Given an invalid request with an invalid output valie
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo, value hi, value lo
        FUNC, 0x00, 0x00, 0x12, 0x34
    };
    size_t requestSize = 5;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        ERROR, 0x03
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, InvalidOutputValue_0xFFFF)
{
    //Given an invalid request with an invalid output valie
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo, value hi, value lo
        FUNC, 0x00, 0x00, 0xFF, 0xFF
    };
    size_t requestSize = 5;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        ERROR, 0x03
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, InvalidOutputValue_0x00FF)
{
    //Given an invalid request with an invalid output valie
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo, value hi, value lo
        FUNC, 0x00, 0x00, 0x00, 0xFF
    };
    size_t requestSize = 5;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        ERROR, 0x03
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, InvalidOutputAddress_8)
{
    //Given an invalid request with an invalid output valie
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo, value hi, value lo
        FUNC, 0x00, GPIO_LEN, 0xFF, 0x00
    };
    size_t requestSize = 5;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        ERROR, 0x02
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, InvalidOutputAddress_256)
{
    //Given an invalid request with an invalid output valie
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo, value hi, value lo
        FUNC, 0x01, 0x00, 0xFF, 0x00
    };
    size_t requestSize = 5;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        ERROR, 0x02
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, SetOutput0_on)
{
    //Given a valid request to turn output 0 on
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo, value hi, value lo
        FUNC, 0x00, 0x00, 0xFF, 0x00
    };
    size_t requestSize = 5;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the output is turned on
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[0]]);
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x00, 0x00, 0xFF, 0x00
    };
    //And the correct reply is sent (copy of request)
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, SetOutput1_on)
{
    //Given a valid request to turn output 1 on
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x00, 0x01, 0xFF, 0x00
    };
    uint8_t buffer[256];
    size_t requestSize = sizeof(REPLY_REF);
    memcpy(buffer, REPLY_REF, requestSize);
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the output is turned on
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[1]]);
    //And the correct reply is sent (copy of request)
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, SetOutput0_off)
{
    //Given a valid request to turn output 0 on
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t buffer[256];
    //And output 0 is turned on
    gpio.states[GPIO_LOOKUP[0]] = true;
    size_t requestSize = sizeof(REPLY_REF);
    memcpy(buffer, REPLY_REF, requestSize);
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the output is turned on
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[0]]);
    //And the correct reply is sent (copy of request)
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, SetOutput7_on)
{
    //Given a valid request to turn output 7 on
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x00, 0x07, 0xFF, 0x00
    };
    uint8_t buffer[256];
    size_t requestSize = sizeof(REPLY_REF);
    memcpy(buffer, REPLY_REF, requestSize);
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the output is turned on
    EXPECT_TRUE(gpio.states[GPIO_LOOKUP[7]]);
    //And the correct reply is sent (copy of request)
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(WriteSingleCoilSuite, SetOutput7_off)
{
    //Given a valid request to turn output 7 on
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x00, 0x07, 0x00, 0x00
    };
    uint8_t buffer[256];
    //And output 7 is turned on
    gpio.states[GPIO_LOOKUP[7]] = true;
    size_t requestSize = sizeof(REPLY_REF);
    memcpy(buffer, REPLY_REF, requestSize);
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    //Then the output is turned on
    EXPECT_FALSE(gpio.states[GPIO_LOOKUP[7]]);
    //And the correct reply is sent (copy of request)
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}
