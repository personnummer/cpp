cmake_minimum_required(VERSION 3.1)
project(Personnummer)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

option(WITH_TEST "Build the test suite" OFF)
add_subdirectory(src)

if (WITH_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
