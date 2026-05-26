#
#Copyright (c) 2026 Jess Knue
#
#SPDX-License-Identifier: BSD-2-Clause

add_library(
    scheduler
    STATIC
    scheduler.c
)

registerFilesToFormat(FILES
                        scheduler.c)

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