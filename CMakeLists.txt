cmake_minimum_required(VERSION 3.21)

project(
    reaction 
    VERSION 1.0
    LANGUAGES CXX
)

set(BUILD_TESTS ON CACHE BOOL "Build tests")
set(BUILD_BENCHMARKS ON CACHE BOOL "Build benchmark")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()

add_compile_options(-Wall -Wextra -pedantic -fno-rtti)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(
    ${PROJECT_NAME} INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(bench)
endif()
