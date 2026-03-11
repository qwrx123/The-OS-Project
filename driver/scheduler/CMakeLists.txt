#
#Copyright (c) 2026 Jess Knue
#
#SPDX-License-Identifier: BSD-2-Clause

add_library(
    scheduler
    STATIC
    scheduler.c
    scheduleNode.c
)

if(NOT BUILD_TESTING)
    add_library(
        reg_switch.S
    )
endif()

registerFilesToFormat(FILES
                        reg_switch.S
                        scheduler.c
                        scheduleNode.h
                        scheduleNode.c)

target_compile_options(scheduler PRIVATE
    -nostdlib
    -ffreestanding
    -Wall
    -Wextra
    -Wpedantic
    -Werror)

target_include_directories(scheduler
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)