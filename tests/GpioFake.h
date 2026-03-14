#ifndef GPIOFAKE_H
#define GPIOFAKE_H

#include <GpioBase.h>
#include <cstddef>

template <size_t LEN>
class GpioFake : public GpioBase
{
public:
    void SetMode(uint8_t pin, Mode mode) override
    {
        if (LEN > pin)
        {
            modes[pin] = mode;
        }
    }

    void Write(uint8_t pin, bool value) override
    {
        if (LEN > pin)
        {
            states[pin] = value;
        }
    }

    bool Read(uint8_t pin) override
    {
        if (LEN > pin)
        {
            return states[pin];
        }
        return false;
    }

    bool states[LEN] = { 0 };
    GpioBase::Mode modes[LEN] = { GpioBase::Mode::Input };
};

#endif // GPIOFAKE_H
