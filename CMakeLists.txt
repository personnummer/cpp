cmake_minimum_required(VERSION 3.0)
project(cpp-personnummer)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/personnummer.cpp)

add_library(Personnummer ${SOURCE_FILES})
target_link_libraries(Personnummer -lpthread)

add_executable(unittest test/unittest.cpp)
target_link_libraries(unittest Personnummer)
