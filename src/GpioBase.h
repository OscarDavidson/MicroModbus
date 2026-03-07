#ifndef GPIOBASE_H
#define GPIOBASE_H

#include <stdint.h>

class GpioBase
{
public:
    enum class Mode : uint8_t
    {
        Input = 0,
        Ouput,
        InputPullup
    };
    virtual void SetMode(uint8_t pin, Mode mode) = 0;
    virtual void Write(uint8_t pin, bool value) = 0;
    virtual bool Read(uint8_t pin) = 0;
};

#endif // GPIOBASE_H
