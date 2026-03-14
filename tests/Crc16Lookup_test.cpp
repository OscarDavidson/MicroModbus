#include <gtest/gtest.h>
#include <Crc16Lookup.h>

TEST(Crc16LookupSuite, Test1)
{
    static constexpr uint8_t TEST_DATA[] =
    {
        0x01, 0x03, 0x00, 0x00, 0x00, 0x02
    };
    static constexpr uint16_t CRC_RESULT = 0x0BC4;
    ASSERT_EQ(Crc16Lookup::Crc16(TEST_DATA, sizeof(TEST_DATA)), CRC_RESULT);
}

TEST(Crc16LookupSuite, Test2)
{
    static constexpr uint8_t TEST_DATA[] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
    };
    static constexpr uint16_t CRC_RESULT = 0x7A46;
    ASSERT_EQ(Crc16Lookup::Crc16(TEST_DATA, sizeof(TEST_DATA)), CRC_RESULT);
}

TEST(Crc16LookupSuite, Test3)
{
    static constexpr uint8_t TEST_DATA[] =
    {
        0x7F, 0x03, 0x04, 0xD2, 0x00, 0x34
    };
    static constexpr uint16_t CRC_RESULT = 0x0AEF;
    ASSERT_EQ(Crc16Lookup::Crc16(TEST_DATA, sizeof(TEST_DATA)), CRC_RESULT);
}
