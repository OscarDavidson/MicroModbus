#include <ArduinoSerial.h>
#include <SerialModbus.h>
#include <Modbus.h>
#include <ModbusHandlerBase.h>
#include <Crc16Lookup.h>
#include <avr/pgmspace.h>
#include <WriteMultiCoilsHandler.h>
#include <ArduinoGpio.h>

// Choose only one of the serial drivers
ArduinoSerial arduSerial{Serial};   //Or Serial2, Serial3 etc
// Below are drivers for RS485, the digital pin specified is for TX enable
// You must set this pin as an output in setup
//ArduinoSerialDigitalWrite arduSerial{Serial, 13};
//ArduinoSerialPortPins arduSerial{Serial, &PORTB, PB7};

//Map of Modbus coil to Arduino GPIO pin, starting from coil 0
static constexpr uint8_t PIN_MAP[] =
{
    22, //Writing to Modbus coil 0 will set GPIO 22
    23,
    24,
    25,
    26,
    27,
    28,
    29
};

ArduinoGpio gpio;   //Wrapper around Arduino's digitalWrite functions
//This will get called when a write coils message is received
WriteMultiCoilsHandler writeMultiCoils{gpio, PIN_MAP, sizeof(PIN_MAP)};

static constexpr uint8_t ADDRESS = 10;  //Modbus address to use
ModbusCounters counters;                //Serial stats/error counts

//List of handlers
ModbusHandlerBase* handlers[] =
{
    &writeMultiCoils,
    nullptr //MUST be terminated with nullptr
};

SerialSettings settings(BaudRate::B115200); //Needed for timing calculations
//Modbus object - called by SerialModbus
Modbus modbus{handlers, counters};
//SerialModbus will call Modbus when it reads a message from arduSerial
SerialModbus serialModbus{arduSerial, modbus, ADDRESS, counters,
                          settings.baud};

void setup()
{
    //You are responsible for initialising the GPIO as outputs
    for (uint8_t ix = 0; ix < sizeof(PIN_MAP); ++ix)
    {
        pinMode(PIN_MAP[ix], OUTPUT);
        digitalWrite(PIN_MAP[ix], LOW);
    }
    
    Serial.begin(settings.ToBaud());
}

void loop()
{
    //Call serialModbus to check for new bytes. It's a good idea to call every
    //loop. If there's no serial data available, it will return immediately
    serialModbus.Clocktick(micros());
}
