cmake_minimum_required(VERSION 3.25)

get_cmake_property(CACHE_ARGS CACHE_VARIABLES)

if (gcc)
    set(CMAKE_C_COMPILER gcc)
endif ()

if (g++)
    set(CMAKE_CXX_COMPILER g++)
endif()

project(CodeTest)

add_compile_definitions(typeInt=long\ long)
add_compile_definitions(typeFloat=long\ double)

set(CMAKE_CXX_STANDARD 17)
option(DEBUG "Provide debug mode" ON)

if (DEBUG)
    add_compile_definitions(DEBUG_MODE)
    #add_compile_definitions(DO_NOT_REMOVE_CACHE)
    add_compile_definitions(_GLIBCXX_DEBUG)
    add_compile_definitions(_GLIBCXX_DEBUG_PEDANTIC)
endif (DEBUG)
unset(DEBUG CACHE)

add_executable(codetest main.cpp libraries/interval.h libraries/ASTExperiment.h libraries/CFGparser.h)

add_executable(test_lib_interval interval.cpp libraries/interval.h)

target_link_libraries(codetest -static)

install(TARGETS codetest DESTINATION bin)
