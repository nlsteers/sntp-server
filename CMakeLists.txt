cmake_minimum_required(VERSION 3.6)
project(sntp_coursework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(GCC_COVERAGE_LINK_FLAGS "-lm")

add_definitions(${GCC_COVERAGE_LINK_FLAGS})

set(MULTICASTCLIENT_FILES
        src/client/sntp-multicast-client.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)

set(MULTICASTSERVER_FILES
        src/server/sntp-multicast-server.c
        src/include/sntp-packet.h src/include/ntp-time.h src/include/time-conversion.h)

add_executable(sntp-client ${MULTICASTCLIENT_FILES})
add_executable(sntp-server ${MULTICASTSERVER_FILES})

target_link_libraries(sntp-client m)
target_link_libraries(sntp-server m)

include_directories(src)
add_subdirectory(src/tests)