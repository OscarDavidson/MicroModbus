#include <ArduinoSerial.h>
#include <SerialModbus.h>
#include <Modbus.h>
#include <ModbusHandlerBase.h>
#include <ReadHoldingRegisterHandler.h>
#include <Crc16Lookup.h>

// Choose only one of the serial drivers
ArduinoSerial arduSerial{Serial};   //Or Serial2, Serial3 etc
// Below are drivers for RS485, the digital pin specified is for TX enable
// You must set this pin as an output in setup
//ArduinoSerialDigitalWrite arduSerial{Serial, 13};
//ArduinoSerialPortPins arduSerial{Serial, &PORTB, PB7};

static constexpr uint8_t ADDRESS = 10;  //Modbus address to use
ModbusCounters counters;                //Serial stats/error counts

static constexpr size_t REG_LEN = 2;
uint16_t registers[REG_LEN] = { 0 };    //Registers for ADC values
//Read holding register handler - reads from registers above
ReadHoldingRegisterHandler readHoldingHandler{registers, REG_LEN, counters};

//List of handlers
ModbusHandlerBase* handlers[] =
{
    &readHoldingHandler,
    nullptr //MUST be terminated with nullptr
};

SerialSettings settings(BaudRate::B115200); //Needed for timing calculations
//Modbus object - called by SerialModbus
Modbus modbus{handlers, counters};
//SerialModbus will call Modbus when it reads a message from arduSerial
SerialModbus serialModbus{ arduSerial, modbus, ADDRESS,
                           counters, settings.baud };

void setup()
{
   Serial.begin(settings.ToBaud());
}

void loop()
{
    //Call serialModbus to check for new bytes. It's a good idea to call every
    //loop. If there's no serial data available, it will return immediately
    serialModbus.Clocktick(micros());
    //Update the registers will new analogue samples
    registers[0] = analogRead(A0);
    registers[1] = analogRead(A1);
}
