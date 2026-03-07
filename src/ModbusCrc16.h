#ifndef MODBUSCRC16_H
#define MODBUSCRC16_H

#include <cstddef>
#include <stdint.h>

class CRC16
{
public:

    static constexpr uint16_t CalculateCRC(const uint8_t* data, size_t length)
    {
        uint16_t crc = 0xFFFF;

        for (size_t i = 0; i < length; ++i)
        {
            crc ^= data[i]; // XOR byte into CRC

            for (int j = 0; j < 8; ++j) // Process each bit
            {
                if (crc & 0x0001) // If the LSB is set
                {
                    crc = (crc >> 1) ^ 0xA001; // Shift and XOR with polynomial
                }
                else
                {
                    crc >>= 1;
                }
            }
        }

        ConvertToLittleEndian(crc);

        return crc;
    }

    // Convert CRC from host-endian to little-endian
    static void ConvertToLittleEndian(uint16_t& value)
    {
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            // No conversion needed if system is little-endian
        value = __builtin_bswap16(value); // Swap bytes if system is big-endian
        #else

        #endif
    }

    // Convert CRC from little-endian to host-endian
    static constexpr void ConvertFromLittleEndian(uint16_t& value)
    {
        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            // No conversion needed if system is little-endian
        value = __builtin_bswap16(value); // Swap bytes if system is big-endian
        #else

        #endif
    }
};

#endif // MODBUSCRC16_H
