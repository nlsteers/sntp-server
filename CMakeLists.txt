cmake_minimum_required(VERSION 3.6)
project(sntp_coursework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CLIENT_FILES
        src/client/sntp-client.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)

set(SERVER_FILES
        src/server/sntp-server.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)

set(DUMMYCLIENT_FILES
        src/server/dummy-client.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)

add_executable(client ${CLIENT_FILES})
add_executable(server ${SERVER_FILES})
add_executable(dummyclient ${DUMMYCLIENT_FILES})