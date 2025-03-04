cmake_minimum_required(VERSION 3.25)
project(CodeTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(CodeTest main.cpp interval.h)

add_executable(test_lib_interval interval.cpp interval.h)
