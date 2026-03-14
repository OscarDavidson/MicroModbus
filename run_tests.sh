#!/bin/bash
set -e

cmake -S tests tests/build
cmake --build tests/build
./tests/build/MicroModbusTest
