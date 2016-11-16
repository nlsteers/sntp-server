cmake_minimum_required(VERSION 3.6)
project(sntp_coursework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/include/timeconversion.c
        src/include/ntp-structure.h
    src/sntp-client.c
        )

add_executable(sntp_coursework ${SOURCE_FILES})