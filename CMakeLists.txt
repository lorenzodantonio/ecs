cmake_minimum_required(VERSION 3.10)
project(storage C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra -O2 -pedantic)

file(GLOB ALL_SOURCES "*.c")
list(REMOVE_ITEM ALL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
)

add_executable(storage main.c ${ALL_SOURCES})

add_executable(test_runner test.c ${ALL_SOURCES})
target_compile_options(test_runner PRIVATE --coverage)
target_link_options(test_runner PRIVATE --coverage)
