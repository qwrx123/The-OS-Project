#
# Copyright (c) 2026 Nathaniel Smith
#
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.28.3)
project(TheOSProject LANGUAGES CXX C ASM)

enable_testing()

find_package(GTest REQUIRED)

# use C++20 language standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

registerFilesToFormat(FILES driver_uart.cpp memallc.cpp
                            scheduler.cpp lib_itoa.cpp timer.cpp)

add_executable(projectTests
    driver_uart.cpp
    memallc.cpp
    scheduler.cpp
    lib_itoa.cpp
    timer.cpp)

target_include_directories(projectTests PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_sources(projectTests PRIVATE
    driver_uart.cpp
    memallc.cpp
    timer.cpp
    mocks/mock_timer_imp.c
)

target_link_libraries(projectTests
    PRIVATE
    driver
    scheduler
    lib
    GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(projectTests)