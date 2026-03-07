#ifndef BAUDRATETYPE_H
#define BAUDRATETYPE_H

#include <stdint.h>

#ifdef ARDUINO_ARCH_AVR
    #include "avr/pgmspace.h"
#endif

enum class BaudRate : uint8_t
{
    B300 = 0,
    B600,
    B1200,
    B2400,
    B4800,
    B9600,
    B19200,
    B38400,
    B57600,
    B76800,
    B115200,
    B230400,
    B250000,

    LENGTH
};

static constexpr uint32_t BAUD_RATES[] =
{
    300,
    600,
    1200,
    2400,
    4800,
    9600,
    19200,
    38400,
    57600,
    115200,
    230400,
    250000
};

enum class Parity : uint8_t
{
    None = 0,
    Even = 0x02,
    Odd = 0x03,
};

static constexpr BaudRate DEFAULT_BAUD = BaudRate::B19200;

struct SerialSettings
{
    BaudRate baud = DEFAULT_BAUD;
    uint8_t parity:2;
    bool stopBits:1;

    static constexpr bool ONE_STOP = 0;
    static constexpr bool TWO_STOP = 1;

    SerialSettings(BaudRate _baud = DEFAULT_BAUD, Parity _parity = Parity::None,
                   bool stop = ONE_STOP)
        : baud(_baud), parity((uint8_t)_parity), stopBits(stop) { }
    uint16_t ToRegister()
    {
        return ((uint8_t)baud) | ((((uint8_t)parity) | (stopBits << 2)) << 8);
    }

    void FromRegister(const uint16_t reg)
    {
        baud = (BaudRate)(reg & 0xFF);
        parity = (reg >> 8) & 0x03;
        stopBits = (reg >> 10) & 0x01;
    }

    uint32_t ToBaud()
    {
        return BAUD_RATES[(uint8_t)baud];
    }
};

static constexpr
#ifdef ARDUINO_ARCH_AVR
    PROGMEM
#endif
    uint32_t CharTimeoutUs[] =
{
    128000, // (1/300) * 11 bits * 3.5 = 128ms
    64000,  // 600 Baud
    32000,  // 1200
    16000,  // 2400
    8000,   // 4800
    4000,   // 9600
    2000,   // 19200
    1750,   // 38400
    1650,   // 57600
    1650,   // 76800
    1650,   // 115200
    1650,   // 230400
    1650,   // 250000
};

#endif // BAUDRATETYPE_H
