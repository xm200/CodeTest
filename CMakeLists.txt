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
option(DEBUG "Provide debug mode" OFF)

if (DEBUG)
    add_compile_definitions(DEBUG_MODE)
#    add_compile_definitions(DO_NOT_REMOVE_CACHE)
    add_compile_definitions(_GLIBCXX_DEBUG)
    add_compile_definitions(_GLIBCXX_DEBUG_PEDANTIC)
endif (DEBUG)
unset(DEBUG CACHE)

add_executable(codetest main.cpp libraries/interval.h libraries/ASTExperiment.h libraries/CFGparser.h)

add_executable(test_lib_interval interval.cpp libraries/interval.h)

add_executable(test_checker Testing/test.cpp libraries/interval.h libraries/ASTExperiment.h libraries/CFGparser.h)

enable_testing()

add_test(NAME test_reading_file COMMAND $<TARGET_FILE:test_checker> 1 none WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_test(NAME test_cfg_tree_dfs COMMAND $<TARGET_FILE:test_checker> 2 dfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(NAME test_cfg_tree_bfs COMMAND $<TARGET_FILE:test_checker> 2 bfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_test(NAME test_simple_generation_dfs COMMAND $<TARGET_FILE:test_checker> 3 dfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(NAME test_simple_generation_bfs COMMAND $<TARGET_FILE:test_checker> 3 bfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_test(NAME test_and_generation_dfs COMMAND $<TARGET_FILE:test_checker> 4 dfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(NAME test_and_generation_bfs COMMAND $<TARGET_FILE:test_checker> 4 bfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_test(NAME test_or_generation_dfs COMMAND $<TARGET_FILE:test_checker> 5 dfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(NAME test_or_generation_bfs COMMAND $<TARGET_FILE:test_checker> 5 bfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_test(NAME test_not_generation_dfs COMMAND $<TARGET_FILE:test_checker> 6 dfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(NAME test_not_generation_bfs COMMAND $<TARGET_FILE:test_checker> 6 bfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_test(NAME test_rollback_generation_dfs COMMAND $<TARGET_FILE:test_checker> 7 dfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_test(NAME test_rollback_generation_bfs COMMAND $<TARGET_FILE:test_checker> 7 bfs WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

if (NOT $ENV{MAC})
    target_link_libraries(codetest -static)
    target_link_libraries(test_lib_interval -static)
    target_link_libraries(test_checker -static)
else ()
    target_link_libraries(codetest)
    target_link_libraries(test_lib_interval)
    target_link_libraries(test_checker)
endif ()

install(TARGETS codetest DESTINATION bin)
