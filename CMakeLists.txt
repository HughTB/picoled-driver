cmake_minimum_required(VERSION 3.26)
project(picoled_driver_linux)

set(CMAKE_CXX_STANDARD 17)

add_executable(picoled_driver_linux
    src/main.cxx
    src/serial.cxx
    src/serial.hxx
    src/system_info.cxx
    src/system_info.hxx
)
