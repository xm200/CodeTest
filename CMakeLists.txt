cmake_minimum_required(VERSION 3.25)
project(CodeTest)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(VERBOSE)
add_compile_definitions(DEBUG_MODE)
add_compile_definitions(typeInt=long\ long)
add_compile_definitions(typeFloat=long\ double)
#add_compile_definitions(DO_NOT_REMOVE_CACHE)
add_compile_definitions(_GLIBCXX_DEBUG)
add_compile_definitions(_GLIBCXX_DEBUG_PEDANTIC)

add_executable(CodeTest main.cpp interval.h ASTExperiment.h CFGparser.h)

add_executable(test_lib_interval interval.cpp interval.h)
