#ifndef ARDUINOGPIO_H
#define ARDUINOGPIO_H

#include "GpioBase.h"
#include <Arduino.h>

class ArduinoGpio : public GpioBase
{
public:
    void SetMode(uint8_t pin, Mode mode) override
    {
        pinMode(pin, (uint8_t)mode);
    }

    void Write(uint8_t pin, bool value) override
    {
        digitalWrite(pin, value);
    }

    bool Read(uint8_t pin) override
    {
        return digitalRead(pin);
    }
};

#endif // ARDUINOGPIO_H
