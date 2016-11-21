cmake_minimum_required(VERSION 3.6)
project(sntp_coursework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/sntp-client.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)

add_executable(sntp_coursework ${SOURCE_FILES})