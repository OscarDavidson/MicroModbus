#ifndef SERIALMODBUS_H
#define SERIALMODBUS_H

/* Responsibilities:
 * - Check for incoming data from serial driver
 * - Decide when a complete packet has been received
 * - Check if the packet matches our address
 * - Check the CRC matches the data
 * - When timeout reached to reply, pass the message to ModbusHandler
 * - Send a reply if ModbusHandler returns true
 * - Keep counters for messages and error
 *
 */

#include <stdint.h>
#include <ModbusBase.h>
#include <SerialBase.h>
#include <BaudrateType.h>
#include <Crc16Lookup.h>
#include <ModbusCounterType.h>

class SerialModbus
{
public:
    SerialModbus(SerialBase& _serial, ModbusBase& _modbus, uint8_t address,
                 ModbusCounters& _counters,
                 BaudRate _baud = DEFAULT_BAUD)
        : serial(_serial), modbus(_modbus), slaveAddress(address),
          counters(_counters), baud(_baud)
    {

    }

    void Clocktick(const uint32_t timeUs)
    {
        uint8_t byteRead = 0;
        if ((BUFFER_LENGTH > bufferIndex) && serial.Available())
        {
            lastByteTime = timeUs;
        }
        while ((BUFFER_LENGTH > bufferIndex) && serial.Read(byteRead))
        {
            buffer[bufferIndex++] = byteRead;
        }

        if ((MIN_MESSAGE_LENGTH <= bufferIndex) && TimeoutExpired(timeUs))
        {
            const uint16_t crc16 = Crc16Lookup::Crc16(buffer, bufferIndex - 2);
            ++counters.busMessages;

            if (((crc16 & 0x00FF) == buffer[bufferIndex - 2])
                && (((crc16 >> 8) & 0x00FF) == buffer[bufferIndex - 1]))
            {
                if ((BROADCAST_ADDRESS == buffer[0])
                    || (slaveAddress == buffer[0]))
                {
                    ++counters.slaveCount;
                    size_t replyLength = bufferIndex - 3;
                    if (modbus.HandleMessage(buffer + 1, replyLength))
                    {
                        const uint16_t crc = Crc16Lookup::Crc16(buffer,
                                                                replyLength + 1);
                        buffer[replyLength + 1] = crc & 0x00FF;
                        buffer[replyLength + 2] = (crc >> 8) & 0x00FF;
                        serial.Write(buffer, replyLength + 3);
                    }
                    else
                    {
                        ++counters.noResponse;
                    }

                }

            }
            else
            {
                ++counters.commsErrors;
            }
            bufferIndex = 0;
        }
    }

    void SetBaudRate(BaudRate newBaud)
    {
        if (BaudRate::LENGTH > newBaud)
        {
            baud = newBaud;
        }
    }

private:
    bool TimeoutExpired(const uint32_t newTime)
    {
#ifdef ARDUINO_ARCH_AVR
        return (pgm_read_dword(&CharTimeoutUs[(uint8_t)baud])
                <= (newTime - lastByteTime));
#else
        return (CharTimeoutUs[(uint8_t)baud]
                <= (newTime - lastByteTime));
#endif
    }

    SerialBase& serial;
    ModbusBase& modbus;
    uint8_t slaveAddress = 0;
    ModbusCounters& counters;
    BaudRate baud = DEFAULT_BAUD;

    static constexpr size_t BUFFER_LENGTH = 256;
    static constexpr size_t MIN_MESSAGE_LENGTH = 4;
    static constexpr uint8_t BROADCAST_ADDRESS = 0;
    uint8_t buffer[BUFFER_LENGTH];
    size_t bufferIndex = 0;
    uint32_t lastByteTime = 0;
};

#endif // SERIALMODBUS_H
