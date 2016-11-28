include_directories(../lib/googletest/include ../lib/googletest)
include_directories(../../client)
include_directories(../../server)
include_directories(../../include)

add_executable(runBasicTests
        checks.cpp)

target_link_libraries(runBasicTests gtest gtest_main)
