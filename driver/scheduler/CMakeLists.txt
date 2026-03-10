#
#Copyright (c) 2026 Jess Knue
#
#SPDX-License-Identifier: BSD-2-Clause

add_library(
    arch_scheduler
    STATIC
    scheduler.c
)

registerFilesToFormat(FILES
                        proc_list.h
                        reg_switch.S
                        scheduler.c
                        scheduleNode.h)

target_compile_options(arch_scheduler PRIVATE
    -nostdlib
    -ffreestanding
    -Wall
    -Wextra
    -Wpedantic
    -Werror)

target_include_directories(arch_scheduler
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)