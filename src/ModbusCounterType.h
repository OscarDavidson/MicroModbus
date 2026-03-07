#ifndef MODUSCOUNTERTYPE_H
#define MODUSCOUNTERTYPE_H

#include <stdint.h>

struct ModbusCounters
{
    uint16_t busMessages = 0;
    uint16_t commsErrors = 0;
    uint16_t exceptions = 0;
    uint16_t slaveCount = 0;
    uint16_t noResponse = 0;
    uint16_t nakCount = 0;
    uint16_t busyCount = 0;
    uint16_t overrunCount = 0;
};

#endif // MODUSCOUNTERTYPE_H
