cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME Anton)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_SKETCH Anton.ino)

#### Uncomment below additional settings as needed.
# AUS https://github.com/francoiscampbell/arduino-cmake
set(${CMAKE_PROJECT_NAME}_BOARD nano)
set(${CMAKE_PROJECT_NAME}_PORT COM3)
#set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyACM0)
set(nano.build.mcu atmega328p)
#set(nano.upload.protocol wiring)
set(nano.upload.speed 57600)


#### Uncomment below additional settings as needed.
# set(${CMAKE_PROJECT_NAME}_BOARD mega)
# set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyACM0)
# set(mega.build.mcu atmega2560)
# set(mega.upload.protocol wiring)
# set(mega.upload.speed 115200)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
