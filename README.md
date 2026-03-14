
Build tests ![Build](https://github.com/OscarDavidson/MicroModbus/actions/workflows/unit-tests.yml/badge.svg?job=Build)

Run tests ![Tests](https://github.com/OscarDavidson/MicroModbus/actions/workflows/unit-tests.yml/badge.svg?job=Tests)

# MicroModbus

Minimal Modbus RTU library for Arduino/platformio

Work in progress, but fully functional as a Modbus RTU server

Included messages handlers and function code:

* Read discrete inputs - 0x02
* Real holding register - 0x03
* Write single coil - 0x05
* Write multiple coils - 0x0F

The included example `AdcClientAvr` uses only 4404 bytes of ROM and 588 bytes of RAM when complied for an Arduino Nano. Modbus serial timings are respected. Baud rates up to 115,200 work fine, only real limit is the serial driver (custom driver coming).

RS485 is supported, driving a configurable GPIO for transmit enable. Standard serial without transmit enable is supported, such as the built in USB port.

Network support is planned, and more message handlers.

### Unit tests
Unit tests are provided, using googletest. To build and run the unit tests, run
the following:
```bash
cmake -S tests tests/build
cmake --build tests/build
./tests/build/MicroModbusTest
``` 
Or run the script with `./run_tests.sh`. You may need to make it executable
first with `chmod+x run_tests.sh`.
