//#ifndef SERIALDRIVER3_H
//#define SERIALDRIVER3_H

//#include "SerialBase.h"
//#include "BaudrateType.h"
//#include "wiring_private.h"
//#include <avr/pgmspace.h>

//extern class SerialDriver3 serialDriver3;

///* Settings:
// * - BaudRate
// * - Parity
// * - Stop bits
// * - Reject parity bit failure
// *
// */


//class SerialDriver3 : public SerialBase
//{
//public:
//    SerialDriver3(volatile uint8_t* _port, uint8_t _pin)
//        : port(_port), pin(1 << _pin)
//    {

//    }

//    void Start(const SerialSettings settings)
//    {
////        settings = newSettings;
//        const uint16_t UBR = pgm_read_word(&BAUD_BITS_16MHZ[
//                                           (uint8_t)settings.baud]);
//        UBRR3H = (uint8_t)((UBR >> 8) & 0x0F);
//        UBRR3L = (uint8_t)(UBR & 0x00FF);
//        if (0 == (0x8000 & UBR))
//        {
//            UCSR3A = (1 << U2X3);
//        }
//        else
//        {
//            UCSR3A = 0;
//        }
//        UCSR3B = (1 << RXCIE3)  | (1 << TXCIE3)/* | (1 << UDRIE3)*/
//                | (1 << RXEN3) | (1 << TXEN3);
//        UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);
//        UCSR3C |= (((uint8_t)settings.parity & 0x03) << 4) |
//                    (settings.stopBits << 3);
//    }

//    void Stop()
//    {
//        UCSR3B = 0;
//    }

//    bool Write(const uint8_t* data, const size_t len) override
//    {
//        _MMIO_BYTE(port) |= pin;

//        sendSize = len;
//        sendIndex = 1;

//        dataPtr = (volatile uint8_t*)data;
//        UDR3 = data[0];
//        UCSR3B |= (1 << UDRIE3);
//        return true;
//    }

//    bool Read(uint8_t& value) override
//    {
//        if (rxBufferIndexIn != rxBufferIndexOut)
//        {
//            value = rxBuffer[rxBufferIndexOut];
//            rxBufferIndexOut = (rxBufferIndexOut + 1) & RX_BUFFER_MASK;
//            return true;
//        }
//        return false;
//    }

//    size_t Available() override
//    {
//        return (rxBufferIndexOut - rxBufferIndexIn) & RX_BUFFER_MASK;
//    }

//    inline void HandleRxInterrupt()
//    {
//        rxBuffer[rxBufferIndexIn] = UDR3;
//        rxBufferIndexIn = (rxBufferIndexIn + 1) & RX_BUFFER_MASK;
//        if (rxBufferIndexIn == rxBufferIndexOut)
//        {
//            ++rxBufferIndexOut;
//            rxBufferIndexOut &= RX_BUFFER_MASK;
//        }
//    }

//    inline void HandleUdrInterrupt()
//    {
//        UDR3 = dataPtr[sendIndex++];
//        if (sendIndex >= sendSize)
//        {
//            cbi(UCSR3B, UDRIE3);
////            UCSR3B &= ~(1 << UDRIE3);
//        }
//    }

//    inline void HandleTxCompleteInterrupt()
//    {
//        _MMIO_BYTE(port) &= ~pin;
//    }

//    static const PROGMEM uint16_t BAUD_BITS_16MHZ[];

////private:
//    static constexpr uint8_t RX_BUFFER_SIZE = 16;
//    static constexpr uint8_t RX_BUFFER_MASK = RX_BUFFER_SIZE - 1;
//    volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
//    volatile uint8_t rxBufferIndexIn = 0;
//    volatile uint8_t rxBufferIndexOut = 0;
//    volatile uint8_t* dataPtr = nullptr;
//    volatile size_t sendIndex = 0;
//    volatile size_t sendSize = 0;
//    volatile uint8_t* port = nullptr;
//    uint8_t pin = 0;
////    SerialSettings settings;
//};

//#endif // SERIALDRIVER3_H
