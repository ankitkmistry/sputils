cmake_minimum_required(VERSION 3.22.1)
project(sputils)

set(CMAKE_CXX_STANDARD 20)

add_library(sputils STATIC
    elpops/elpdef.cpp
    elpops/reader.cpp
    elpops/writer.cpp
    spinfo/opcode.cpp
    spinfo/sign.cpp
    spimp/utils.cpp
)
