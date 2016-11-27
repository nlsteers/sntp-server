include_directories(../lib/googletest/include ../lib/googletest)

add_executable(runBasicTests
        checks.cpp)

target_link_libraries(runBasicTests gtest gtest_main)
