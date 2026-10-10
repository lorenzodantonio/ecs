cmake_minimum_required(VERSION 3.14...3.28)
project(ecs C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(ECS_BUILD_DEMO "Build the demo" ON)
option(ECS_BUILD_TESTS "Build the tests" ON)
option(ECS_SANITIZE "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(ECS_COVERAGE "Build with coverage instrumentation" OFF)
option(ECS_WERROR "Treat warnings as errors" OFF)

add_compile_options(-Wall -Wextra -pedantic)
if(ECS_WERROR)
    add_compile_options(-Werror)
endif()
if(ECS_SANITIZE)
    add_compile_options(-fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

add_library(ecs STATIC
        src/component_pool.c
        src/component_registry.c
        src/sparse_set.c
        src/entity_registry.c
        src/storage.c
)

target_include_directories(ecs
        PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if(ECS_COVERAGE)
    target_compile_options(ecs PUBLIC --coverage -O0 -g)
    target_link_options(ecs PUBLIC --coverage)
endif()

if(ECS_BUILD_DEMO)
    add_executable(ecs_demo demo.c)
    target_link_libraries(ecs_demo PRIVATE ecs)
    if(UNIX AND NOT APPLE)
        target_link_libraries(ecs_demo PRIVATE m)
    endif()
endif()

if(ECS_BUILD_TESTS)
    enable_testing()
    foreach(TEST_NAME
            test_component_registry
            test_entity_registry
            test_entity
            test_storage)
        add_executable(${TEST_NAME} tests/${TEST_NAME}.c)
        target_link_libraries(${TEST_NAME} PRIVATE ecs)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()

    if(ECS_COVERAGE)
        add_custom_target(coverage
                COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
                COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --filter ${CMAKE_SOURCE_DIR}/src
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        add_dependencies(coverage test_component_registry test_entity_registry test_entity test_storage)
    endif()
endif()
