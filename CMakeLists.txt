cmake_minimum_required(VERSION 3.6)
project(sntp_coursework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CLIENT_FILES
        src/client/sntp-client.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)

set(SERVER_FILES
        src/server/sntp-server.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)

set(MULTICASTCLIENT_FILES
        src/client/sntp-multicast-client.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)

set(MULTICASTSERVER_FILES
        src/server/sntp-multicast-server.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)


add_executable(client ${CLIENT_FILES})
add_executable(server ${SERVER_FILES})
add_executable(multicast-client ${MULTICASTCLIENT_FILES})
add_executable(multicast-server ${MULTICASTSERVER_FILES})

include_directories(src)
add_subdirectory(src/tests)


