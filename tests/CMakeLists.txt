#cmake_minimum_required(VERSION 3.14)

#project(MicroModbusTests)

#set(CMAKE_CXX_STANDARD 17)

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

#enable_testing()

#include_directories(.)
#include_directories(../src)

#add_executable(
#    MicroModbusTest
##    main.cpp
#    tst_case.cpp
#)

#target_link_libraries(
#  MicroModbusTest
#  gtest_main
#)

##target_compile_options(MicroModbusTest PRIVATE
##  -Wall
##)

#include(GoogleTest)
#gtest_discover_tests(MicroModbusTest)

cmake_minimum_required(VERSION 3.14)
include(FetchContent)
project(MicroModbusTests)
set(CMAKE_CXX_STANDARD 17)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include_directories(.)
include_directories(../src)

add_executable(
    MicroModbusTest
    SerialModbus_test.cpp
    ../src/SerialModbus.h
    ../src/SerialModbus.cpp
    ../src/SerialBase.h
    SerialFake.h
    SerialFake_test.cpp
    Crc16_test.cpp
    ../src/ModbusCrc16.h
    ../src/Crc16Lookup.h
    ../src/Crc16Lookup.cpp
    Crc16Lookup_test.cpp
    ../src/ModbusBase.h
    ModbusFake.h
    ReadHoldingRegisterHandler_test.cpp
    ../src/ReadHoldingRegisterHandler.h
    ../src/ReadHoldingRegisterHandler.cpp
    Modbus_test.cpp
    ../src/Modbus.h
    ../src/Modbus.cpp
    ModbusHandlerFake.h
    WriteMultiCoilsHandler_test.cpp
    ../src/WriteMultiCoilsHandler.h
    ../src/WriteMultiCoilsHandler.cpp
    WriteSingleCoilHandler_test.cpp
    ../src/WriteSingleCoilHandler.h
    ../src/WriteSingleCoilHandler.cpp
    ReadInputsHandler_test.cpp
    ../src/ReadInputsHandler.h
    ../src/ReadInputsHandler.cpp
)

target_link_libraries(
    MicroModbusTest
    PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(MicroModbusTest)
