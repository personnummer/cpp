cmake_minimum_required(VERSION 3.1)
include_directories(${CMAKE_HOME_DIRECTORY}/src)

add_executable(main "main.cpp")
target_link_libraries(main Personnummer)
