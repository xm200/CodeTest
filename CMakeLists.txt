cmake_minimum_required(VERSION 3.25)

project(CodeTest)

add_compile_definitions(typeInt=long\ long)
add_compile_definitions(typeFloat=long\ double)

set(CMAKE_CXX_STANDARD 17)
option(DEBUG "Provide debug mode" ON)

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

add_test(
        NAME test_reading_file
        COMMAND $<TARGET_FILE:test_checker> 1 none
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_test(
        NAME test_cfg_tree_dfs
        COMMAND $<TARGET_FILE:test_checker> 2 dfs
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_test(
        NAME test_cfg_tree_bfs
        COMMAND $<TARGET_FILE:test_checker> 2 bfs
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_test(
        NAME test_simple_generation_dfs
        COMMAND $<TARGET_FILE:test_checker> 3 dfs
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_test(
        NAME test_simple_generation_bfs
        COMMAND $<TARGET_FILE:test_checker> 3 bfs
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

target_link_libraries(codetest -static)

install(TARGETS codetest DESTINATION bin)
