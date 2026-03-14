#include <gtest/gtest.h>
#include "SerialFake.h"

class SerialFakeSuite : public ::testing::Test
{
protected:
    SerialFake serial;
};

TEST_F(SerialFakeSuite, Available_zeroAtStart)
{
    //Given an empty read buffer, when asked how many bytes are available
    //Then the result is 0
    EXPECT_EQ(serial.Available(), 0);
}

TEST_F(SerialFakeSuite, Available_oneAfterByteAdded)
{
    //Given a byte has been added to the read buffer
    serial.readData.push(0x42);
    //When asked how many bytes are available, then the result is 1
    EXPECT_EQ(serial.Available(), 1);
}

TEST_F(SerialFakeSuite, ReadOneByte)
{
    //Given a byte has been added to the read buffer
    static constexpr uint8_t VALUE = 0x42;
    serial.readData.push(VALUE);
    uint8_t value = 0;
    //When a byte is read true is returned
    ASSERT_TRUE(serial.Read(value));
    //Then the value is equal to what was added to the read buffer
    EXPECT_EQ(VALUE, value);
    EXPECT_EQ(0, serial.Available());
}

TEST_F(SerialFakeSuite, ReadThreeByesInOrder)
{
    //Given three bytes have been added to the read buffer
    serial.readData.push(0xAB);
    serial.readData.push(0xCD);
    serial.readData.push(0xEF);
    //When asked how many bytes are available, the result is 3
    EXPECT_EQ(serial.Available(), 3);
    //Then when each byte is read, the value is returned in order
    uint8_t value = 0;
    ASSERT_TRUE(serial.Read(value));
    EXPECT_EQ(0xAB, value);
    ASSERT_TRUE(serial.Read(value));
    EXPECT_EQ(0xCD, value);
    ASSERT_TRUE(serial.Read(value));
    EXPECT_EQ(0xEF, value);
    //And available returns 0 when all bytes have been read
    EXPECT_EQ(0, serial.Available());
}

TEST_F(SerialFakeSuite, Write_emptyAtStart)
{
    //Given an empty write buffer, when asked how many bytes are available
    //Then the result is 0
    EXPECT_EQ(serial.writeData.size(), 0);
}

TEST_F(SerialFakeSuite, WriteOneByte)
{
    //Given a byte has been added to the read buffer
    static constexpr uint8_t VALUE = 0xAC;
    ASSERT_TRUE(serial.Write(&VALUE, sizeof(VALUE)));
    serial.readData.push(0x42);
    //When asked how many bytes are available, then the result is 1
    EXPECT_EQ(serial.writeData.size(), 1);
    uint8_t value = 0;
}

TEST_F(SerialFakeSuite, WriteFourByte)
{
    //Given four bytes have been added to the read buffer
    static constexpr uint8_t TEST_DATA[] = { 0xA1, 0xB2, 0xC3, 0xD4 };
    ASSERT_TRUE(serial.Write(TEST_DATA, sizeof(TEST_DATA)));
    //When the read buffer, the size is equal to that of the test data
    EXPECT_EQ(serial.writeData.size(), sizeof(TEST_DATA));
    //Then each byte is read in order
    for (size_t ix = 0; ix < sizeof(TEST_DATA); ++ix)
    {
        ASSERT_EQ(serial.writeData.front(), TEST_DATA[ix]);
        serial.writeData.pop();
    }
}

TEST_F(SerialFakeSuite, ReadEmpty)
{
    uint8_t value = 0;
    //Given an empty buffer, when trying to read a byte, then false is returned
    ASSERT_FALSE(serial.Read(value));
}

TEST_F(SerialFakeSuite, Write_nullptr_fails)
{
    //Given a serial fake, when trying to write nullptr, then false is returned
    ASSERT_FALSE(serial.Write(nullptr, 1));
}

TEST_F(SerialFakeSuite, Write_zeroLength_fails)
{
    uint8_t value = 0;
    //Given a serial fake, when trying to write a length of 0
    //Then false is returned
    ASSERT_FALSE(serial.Write(&value, 0));
}
