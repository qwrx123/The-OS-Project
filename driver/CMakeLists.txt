#
# Copyright (c) 2026 Nathaniel Smith
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(
    driver
    STATIC
    uart.c
    memallc.c
)

registerFilesToFormat(FILES uart.c)

target_include_directories(driver
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)