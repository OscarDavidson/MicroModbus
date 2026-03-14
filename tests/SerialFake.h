#ifndef SERIALFAKE_H
#define SERIALFAKE_H

#include <SerialBase.h>
#include <queue>

class SerialFake : public SerialBase
{
public:
    bool Write(const uint8_t* data, const size_t len) override
    {
        if ((nullptr == data ) || (0 >= len)) { return false; }

        for (size_t ix = 0; ix < len; ++ix)
        {
            writeData.push(data[ix]);
        }

        return true;
    }

    bool Read(uint8_t& value) override
    {
        if (0 >= readData.size()) { return false; }
        value = readData.front();
        readData.pop();
        return true;
    }

    size_t Available() override
    {
        return readData.size();
    }

    void AddBlockToRead(const uint8_t* data, const size_t len)
    {
        if ((nullptr == data) || (0 == len)) { return; }
        for (size_t ix = 0; ix < len; ++ix)
        {
            readData.push(data[ix]);
        }
    }

    std::queue<uint8_t> readData;
    std::queue<uint8_t> writeData;
};

#endif // SERIALFAKE_H
