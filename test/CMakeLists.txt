cmake_minimum_required(VERSION 3.1)
include_directories(${CMAKE_HOME_DIRECTORY}/src)

add_executable(unittest "unittest.cpp")

add_test(PersonnummerTest unittest)
target_link_libraries(unittest Personnummer)
