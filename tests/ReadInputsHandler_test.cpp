#include <gtest/gtest.h>
#include "ReadInputsHandler.h"
#include "GpioFake.h"

class ReadInputsSuite : public ::testing::Test
{
protected:

    static constexpr uint8_t GPIO_LOOKUP[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
        17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
    };

    static constexpr size_t GPIO_TABLE_LEN = sizeof(GPIO_LOOKUP);

    static constexpr size_t GPIO_LEN = 32;
    GpioFake<GPIO_LEN> gpio;

    ReadInputsHandler handler{gpio, GPIO_LOOKUP, GPIO_TABLE_LEN};

    static constexpr uint8_t FUNC = 0x02;
    static constexpr uint8_t ERROR = 0x82;
};

TEST_F(ReadInputsSuite, InvalidFunctionCode)
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

TEST_F(ReadInputsSuite, Invalid_Quantity_0)
{
    //Given an invalid request with a quantity of 0
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x00, 0x00
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

TEST_F(ReadInputsSuite, Invalid_Quantity_9000)
{
    //Given an invalid request with a quantity of 9000
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x23, 0x28
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

TEST_F(ReadInputsSuite, Invalid_Quantity_TableLengthPlus1)
{
    //Given an invalid request with a quantity of the table length plus one
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x00, GPIO_TABLE_LEN + 1
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

TEST_F(ReadInputsSuite, Invalid_Quantity_AddressTooHigh_9000)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x23, 0x28, 0x00, 0x01
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

TEST_F(ReadInputsSuite, Invalid_Quantity_AddressTooHigh_TableLen)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, GPIO_TABLE_LEN, 0x00, 0x01
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

TEST_F(ReadInputsSuite, Invalid_Quantity_AddressPlusQuantityTooHigh_7and2)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, GPIO_TABLE_LEN - 1, 0x00, 0x02
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

TEST_F(ReadInputsSuite, INVALID_FRAME_LENGTH)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC,
    };
    size_t requestSize = 1;
    //When the handler is called, false is returned, so send no reply
    ASSERT_FALSE(handler.Handle(buffer, requestSize));
}

TEST_F(ReadInputsSuite, Valid_ReadFirstInput_off)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x00, 0x01
    };
    size_t requestSize = 5;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x01, 0x00
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadFirstInput_on)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x00, 0x01
    };
    size_t requestSize = 5;
    //And input 0 is on
    gpio.states[GPIO_LOOKUP[0]] = true;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x01, 0x01
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadInput1_off)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x01, 0x00, 0x01
    };
    size_t requestSize = 5;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x01, 0x00
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadInput1_on)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x01, 0x00, 0x01
    };
    size_t requestSize = 5;
    //And input 0 is on
    gpio.states[GPIO_LOOKUP[1]] = true;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x01, 0x01
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadInput_0to1_off)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x00, 0x02
    };
    size_t requestSize = 5;
    //And input 1 is on
    gpio.states[GPIO_LOOKUP[1]] = true;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x01, 0x02
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadInput_0to7_0x59)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x00, 0x08
    };
    size_t requestSize = 5;
    //And input 1 is on
    gpio.states[GPIO_LOOKUP[0]] = true;
    gpio.states[GPIO_LOOKUP[3]] = true;
    gpio.states[GPIO_LOOKUP[4]] = true;
    gpio.states[GPIO_LOOKUP[6]] = true;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x01, 'Y'
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadInput_0to7_0x6A)
{
    //Given an invalid request with an address too high
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x00, 0x08
    };
    size_t requestSize = 5;
    //And input 1 is on
    gpio.states[GPIO_LOOKUP[1]] = true;
    gpio.states[GPIO_LOOKUP[3]] = true;
    gpio.states[GPIO_LOOKUP[5]] = true;
    gpio.states[GPIO_LOOKUP[6]] = true;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x01, 0x6A
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadInput_0to15_0x6E9A)
{
    //Given a valid request to read 16 inputs
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x00, 0x10
    };
    size_t requestSize = 5;
    //And input 1 is on
    gpio.states[GPIO_LOOKUP[1]] = true;
    gpio.states[GPIO_LOOKUP[3]] = true;
    gpio.states[GPIO_LOOKUP[4]] = true;
    gpio.states[GPIO_LOOKUP[7]] = true;
    gpio.states[GPIO_LOOKUP[9]] = true;
    gpio.states[GPIO_LOOKUP[10]] = true;
    gpio.states[GPIO_LOOKUP[11]] = true;
    gpio.states[GPIO_LOOKUP[13]] = true;
    gpio.states[GPIO_LOOKUP[14]] = true;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x02, 0x9A, 0x6E
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadInput_1to16_0xB74D)
{
    //Given a valid request to read 16 inputs starting at 1
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x01, 0x00, 0x10
    };
    size_t requestSize = 5;
    //And input 1 is on
    gpio.states[GPIO_LOOKUP[1]] = true;
    gpio.states[GPIO_LOOKUP[3]] = true;
    gpio.states[GPIO_LOOKUP[4]] = true;
    gpio.states[GPIO_LOOKUP[7]] = true;
    gpio.states[GPIO_LOOKUP[9]] = true;
    gpio.states[GPIO_LOOKUP[10]] = true;
    gpio.states[GPIO_LOOKUP[11]] = true;
    gpio.states[GPIO_LOOKUP[13]] = true;
    gpio.states[GPIO_LOOKUP[14]] = true;
    gpio.states[GPIO_LOOKUP[16]] = true;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x02, 0x4D, 0xB7
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadInput_22to31_0x1BE)
{
    //Given a valid request to read 9 inputs starting at 22
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x16, 0x00, 0x09
    };
    size_t requestSize = 5;
    //And input 1 is on
    gpio.states[GPIO_LOOKUP[23]] = true;
    gpio.states[GPIO_LOOKUP[24]] = true;
    gpio.states[GPIO_LOOKUP[25]] = true;
    gpio.states[GPIO_LOOKUP[26]] = true;
    gpio.states[GPIO_LOOKUP[27]] = true;
    gpio.states[GPIO_LOOKUP[29]] = true;
    gpio.states[GPIO_LOOKUP[30]] = true;
    gpio.states[GPIO_LOOKUP[31]] = true;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x02, 0xBE, 0x01
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}

TEST_F(ReadInputsSuite, Valid_ReadInput_0to31_0xBEEFCAFE)
{
    //Given a valid request to read 32 inputs starting at 0
    uint8_t buffer[256] =
    {   //Func, StartHi, StartLo, QuantHi, QuantLo
        FUNC, 0x00, 0x00, 0x00, 0x20
    };
    size_t requestSize = 5;
    //And input 1 is on
    gpio.states[GPIO_LOOKUP[1]] = true;
    gpio.states[GPIO_LOOKUP[2]] = true;
    gpio.states[GPIO_LOOKUP[3]] = true;
    gpio.states[GPIO_LOOKUP[4]] = true;
    gpio.states[GPIO_LOOKUP[5]] = true;
    gpio.states[GPIO_LOOKUP[6]] = true;
    gpio.states[GPIO_LOOKUP[7]] = true;
    gpio.states[GPIO_LOOKUP[9]] = true;
    gpio.states[GPIO_LOOKUP[11]] = true;
    gpio.states[GPIO_LOOKUP[14]] = true;
    gpio.states[GPIO_LOOKUP[15]] = true;
    gpio.states[GPIO_LOOKUP[16]] = true;
    gpio.states[GPIO_LOOKUP[17]] = true;
    gpio.states[GPIO_LOOKUP[18]] = true;
    gpio.states[GPIO_LOOKUP[19]] = true;
    gpio.states[GPIO_LOOKUP[21]] = true;
    gpio.states[GPIO_LOOKUP[22]] = true;
    gpio.states[GPIO_LOOKUP[23]] = true;
    gpio.states[GPIO_LOOKUP[25]] = true;
    gpio.states[GPIO_LOOKUP[26]] = true;
    gpio.states[GPIO_LOOKUP[27]] = true;
    gpio.states[GPIO_LOOKUP[28]] = true;
    gpio.states[GPIO_LOOKUP[29]] = true;
    gpio.states[GPIO_LOOKUP[31]] = true;
    //When the handler is called, true is returned
    ASSERT_TRUE(handler.Handle(buffer, requestSize));
    static constexpr uint8_t REPLY_REF[] =
    {
        FUNC, 0x04, 0xFE, 0xCA, 0xEF, 0xBE
    };
    //And the correct exception error reply is sent
    ASSERT_EQ(requestSize, sizeof(REPLY_REF));
    for (size_t ix = 0; ix < sizeof(REPLY_REF); ++ix)
    {
        EXPECT_EQ(buffer[ix], REPLY_REF[ix]) << ix;
    }
}
