#ifndef ARDUINOSERIAL_H
#define ARDUINOSERIAL_H

#ifdef ARDUINO

#include <Arduino.h>
#include <SerialBase.h>

class ArduinoSerial : public SerialBase
{
public:
    ArduinoSerial(HardwareSerial& _serial) : serial(_serial) { }

    bool Write(const uint8_t* data, const size_t len)
    {
        EnableTx();
        const bool result = (0 < serial.write(data, len));
        serial.flush();
        DisableTx();
        return result;
    }

    bool Read(uint8_t& value)
    {
        if (serial.available())
        {
            const int intRead = serial.read();
            if (0 > intRead) { return false; }
            value = (uint8_t)(intRead & 0xFF);
            return true;
        }
        return false;
    }

    size_t Available()
    {
        const int available = serial.available();
        return (0 > available)?0:available;
    }

protected:
    virtual void EnableTx() { }

    virtual void DisableTx() { }

private:
    HardwareSerial& serial;
};


#ifdef ARDUINO_ARCH_AVR
class ArduinoSerialPortPins : public ArduinoSerial
{
public:
    ArduinoSerialPortPins(HardwareSerial& _serial, volatile uint8_t* _port,
                          uint8_t _pin)
        : ArduinoSerial(_serial), port(_port), pin(1 << _pin) { }

protected:
    void EnableTx() override    { _MMIO_BYTE(port) |= pin; }
    void DisableTx() override   { _MMIO_BYTE(port) &= ~pin; }

private:
    volatile uint8_t* port = nullptr;
    uint8_t pin = 0;
};
#endif //ARDUINO_ARCH_AVR

class ArduinoSerialDigitalWrite : public ArduinoSerial
{
public:
    ArduinoSerialDigitalWrite(HardwareSerial& _serial, uint8_t _pin)
        : ArduinoSerial(_serial), pin(_pin) { }

protected:
    void EnableTx() override    { digitalWrite(pin, true); }
    void DisableTx() override   { digitalWrite(pin, false); }

private:
    const uint8_t pin = 0;
};

#endif // ARDUINO

#endif // ARDUINOSERIAL_H
