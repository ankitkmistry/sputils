cmake_minimum_required(VERSION 3.22.1)
project(sputils)

set(CMAKE_CXX_STANDARD 20)

add_library(sputils STATIC
        src/elpops/elpdef.cpp
        src/elpops/reader.cpp
        src/elpops/writer.cpp
        src/spinfo/opcode.cpp
        src/spinfo/sign.cpp
        src/spimp/utils.cpp
)
