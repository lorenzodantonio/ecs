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

# Coverage per la libreria ecs (e i suoi consumatori)
target_compile_options(ecs PUBLIC --coverage)
target_link_options(ecs PUBLIC --coverage)

# Demo
add_executable(ecs_demo demo.c)
target_link_libraries(ecs_demo PRIVATE ecs)

# Test Coverage
enable_testing()

set(TEST_FILES
        test_component_registry
        test_datastructure
        test_entity_registry
        test_entity
        test_storage
)

foreach(TEST_NAME ${TEST_FILES})
    add_executable(${TEST_NAME} tests/${TEST_NAME}.c)
    target_link_libraries(${TEST_NAME} PRIVATE ecs)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()