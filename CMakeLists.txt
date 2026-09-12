cmake_minimum_required(VERSION 3.10)
project(ecs C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra -O2 -pedantic)

# lib sources (no GLOB)
set(ECS_SOURCES
        src/component_pool.c
        src/component_registry.c
        src/datastructure.c
        src/entity_registry.c
        src/storage.c
)

add_library(ecs STATIC ${ECS_SOURCES})

target_include_directories(ecs
        PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# Demo
add_executable(ecs_demo demo.c)
target_link_libraries(ecs_demo PRIVATE ecs)

# Test Coverage
enable_testing()

set(TEST_SOURCES
        tests/test.c
        tests/test_component_registry.c
        tests/test_datastructure.c
        tests/test_entity_registry.c
        tests/test_entity.c
        tests/test_storage.c
)

add_executable(test_runner ${TEST_SOURCES})
target_link_libraries(test_runner PRIVATE ecs)
target_compile_options(test_runner PRIVATE --coverage)
target_link_options(test_runner PRIVATE --coverage)

add_test(NAME Tests COMMAND test_runner)
