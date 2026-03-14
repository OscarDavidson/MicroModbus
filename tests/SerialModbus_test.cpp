#include <gtest/gtest.h>
#include <SerialModbus.h>

#include "ModbusFake.h"
#include "SerialFake.h"

class SerialModbusSuite : public ::testing::Test
{
protected:
    SerialFake serial;
    ModbusFake modbus;
    static constexpr uint8_t ADDRESS = 123;
    ModbusCounters counters;
    SerialModbus serialModbus{serial, modbus, ADDRESS, counters};

    static constexpr auto TIMEOUT = CharTimeoutUs[(uint8_t)DEFAULT_BAUD];
};

TEST_F(SerialModbusSuite, NoMessage_noActionTaken)
{
    //Given no serial data, when the clocktick is called
    serialModbus.Clocktick(0);
    //Then the Modbus fake will not have been called
    ASSERT_EQ(0, modbus.callCount);
}

TEST_F(SerialModbusSuite, ReadHoldingRegisterId_callModbus_sendReply)
{
    //Given a request message is added to the serial read buffer
    static constexpr uint8_t REQUEST_DATA[] =
    {
        0x7B, 0x03, 0x04, 0xD2, 0x00, 0x34, 0xEE, 0x8E
    };
    serial.AddBlockToRead(REQUEST_DATA, sizeof(REQUEST_DATA));
    modbus.returnSize = sizeof(REQUEST_DATA) - 3;
    modbus.returnValue = true;
    //When the object is clockticked once at time 0
    serialModbus.Clocktick(0);
    //And again after the timeout period
    serialModbus.Clocktick(TIMEOUT);
    //Then the Modbus fake will have been called
    ASSERT_EQ(1, modbus.callCount);
    //With just the PDU (function code and data) passed to Modbus
    EXPECT_EQ(modbus.sizeCalled, sizeof(REQUEST_DATA) - 3);
    for (size_t ix = 0; ix < (sizeof(REQUEST_DATA) - 3); ++ix)
    {
        EXPECT_EQ(REQUEST_DATA[ix + 1], modbus.dataCopy[ix]) << ix;
    }
    //And a copy of the message (reply) will be sent to the serial driver
    EXPECT_EQ(serial.writeData.size(), sizeof(REQUEST_DATA));

    for (size_t ix = 0; (ix < (sizeof(REQUEST_DATA) - 3))
                        && serial.writeData.size(); ++ix)
    {
        EXPECT_EQ(serial.writeData.front(), REQUEST_DATA[ix]);
        serial.writeData.pop();
    }
    EXPECT_EQ(counters.busMessages, 1);
    EXPECT_EQ(counters.slaveCount, 1);
}

TEST_F(SerialModbusSuite, MessageForOtherDevice_incrementCounter)
{
    //Given a request message to a different address
    static constexpr uint8_t REQUEST_DATA[] =
    {
        0x64, 0x03, 0x04, 0xD2, 0x00, 0x34, 0xEC, 0xE1
    };
    serial.AddBlockToRead(REQUEST_DATA, sizeof(REQUEST_DATA));
    modbus.returnSize = sizeof(REQUEST_DATA) - 3;
    modbus.returnValue = true;
    //When the object is clockticked once at time 0
    serialModbus.Clocktick(0);
    //And again after the timeout period
    serialModbus.Clocktick(TIMEOUT);
    //Then the Modbus fake will not have been called
    ASSERT_EQ(0, modbus.callCount);
    //And no reply will be sent
    EXPECT_EQ(serial.writeData.size(), 0);
    //And the message will be counted
    EXPECT_EQ(counters.busMessages, 1);
    //But not to the slave
    EXPECT_EQ(counters.slaveCount, 0);
    EXPECT_EQ(counters.commsErrors, 0);
}

TEST_F(SerialModbusSuite, ReadRegisters_CrcError)
{
    //Given a request message with a CRC error
    static constexpr uint8_t REQUEST_DATA[] =
    {
        0x7B, 0x03, 0x04, 0xD2, 0x00, 0x35, 0xEE, 0x8E
    };
    serial.AddBlockToRead(REQUEST_DATA, sizeof(REQUEST_DATA));
    modbus.returnSize = sizeof(REQUEST_DATA) - 3;
    modbus.returnValue = true;
    //When the object is clockticked once at time 0
    serialModbus.Clocktick(0);
    //And again after the timeout period
    serialModbus.Clocktick(TIMEOUT);
    //Then the Modbus fake will not have been called
    ASSERT_EQ(0, modbus.callCount);
    //And no reply will be sent
    EXPECT_EQ(serial.writeData.size(), 0);
    //And the message will be counted
    EXPECT_EQ(counters.busMessages, 1);
    //But not addressed to the slave, even though the address matches, CRC error
    EXPECT_EQ(counters.slaveCount, 0);
    //And the comms error is counted
    EXPECT_EQ(counters.commsErrors, 1);
}

TEST_F(SerialModbusSuite, HandleRequest_multipleClockTicks)
{
    //Given a request message is added to the serial read buffer
    static constexpr uint8_t REQUEST_DATA[] =
    {
        0x7B, 0x03, 0x04, 0xD2, 0x00, 0x34, 0xEE, 0x8E
    };
    serial.AddBlockToRead(REQUEST_DATA, sizeof(REQUEST_DATA));
    modbus.returnSize = sizeof(REQUEST_DATA) - 3;
    modbus.returnValue = true;
    //When the object is clockticked once at time 0
    serialModbus.Clocktick(0);
    //And again after the timeout period
    serialModbus.Clocktick(TIMEOUT);
    serialModbus.Clocktick(TIMEOUT * 2);
    serialModbus.Clocktick(TIMEOUT * 3);
    //Then the Modbus fake will have been called
    ASSERT_EQ(1, modbus.callCount);
    //With just the PDU (function code and data) passed to Modbus
    EXPECT_EQ(modbus.sizeCalled, sizeof(REQUEST_DATA) - 3);
    for (size_t ix = 0; ix < (sizeof(REQUEST_DATA) - 3); ++ix)
    {
        EXPECT_EQ(REQUEST_DATA[ix + 1], modbus.dataCopy[ix]) << ix;
    }
    //And a copy of the message (reply) will be sent to the serial driver
    EXPECT_EQ(serial.writeData.size(), sizeof(REQUEST_DATA));

    for (size_t ix = 0; (ix < (sizeof(REQUEST_DATA) - 3))
                        && serial.writeData.size(); ++ix)
    {
        EXPECT_EQ(serial.writeData.front(), REQUEST_DATA[ix]);
        serial.writeData.pop();
    }
    EXPECT_EQ(counters.busMessages, 1);
    EXPECT_EQ(counters.slaveCount, 1);
}

TEST_F(SerialModbusSuite, HandleRequest_multipleValidRequests)
{
    //Given a request message is added to the serial read buffer
    static constexpr uint8_t REQUEST_DATA[] =
    {
        0x7B, 0x03, 0x04, 0xD2, 0x00, 0x34, 0xEE, 0x8E
    };
    serial.AddBlockToRead(REQUEST_DATA, sizeof(REQUEST_DATA));
    modbus.returnSize = sizeof(REQUEST_DATA) - 3;
    modbus.returnValue = true;
    //When the object is clockticked once at time 0
    serialModbus.Clocktick(0);
    //And again after the timeout period
    serialModbus.Clocktick(TIMEOUT);
    serialModbus.Clocktick(TIMEOUT * 2);
    serialModbus.Clocktick(TIMEOUT * 3);
    //Then the Modbus fake will have been called
    ASSERT_EQ(1, modbus.callCount);
    //With just the PDU (function code and data) passed to Modbus
    EXPECT_EQ(modbus.sizeCalled, sizeof(REQUEST_DATA) - 3);
    for (size_t ix = 0; ix < (sizeof(REQUEST_DATA) - 3); ++ix)
    {
        EXPECT_EQ(REQUEST_DATA[ix + 1], modbus.dataCopy[ix]) << ix;
    }
    //And a copy of the message (reply) will be sent to the serial driver
    EXPECT_EQ(serial.writeData.size(), sizeof(REQUEST_DATA));

    for (size_t ix = 0; (ix < sizeof(REQUEST_DATA))
                        && serial.writeData.size(); ++ix)
    {
        EXPECT_EQ(serial.writeData.front(), REQUEST_DATA[ix]);
        serial.writeData.pop();
    }
    EXPECT_EQ(counters.busMessages, 1);
    EXPECT_EQ(counters.slaveCount, 1);
    serial.AddBlockToRead(REQUEST_DATA, sizeof(REQUEST_DATA));
    serialModbus.Clocktick(TIMEOUT * 4);
    serialModbus.Clocktick(TIMEOUT * 5);
    serialModbus.Clocktick(TIMEOUT * 6);
    ASSERT_EQ(2, modbus.callCount);
    EXPECT_EQ(serial.writeData.size(), sizeof(REQUEST_DATA));
}
