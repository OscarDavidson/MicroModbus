#ifndef SERIALBASE_H
#define SERIALBASE_H

#include <stddef.h>
#include <stdint.h>

class SerialBase
{
public:
    virtual bool Write(const uint8_t* data, const size_t len) = 0;
    virtual bool Read(uint8_t& value) = 0;
    virtual size_t Available() = 0;
};

#endif // SERIALBASE_H
