#
# Copyright (c) 2026 Nathaniel Smith
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(
    init
    STATIC
    main.c
)

registerFilesToFormat(FILES main.c)

target_include_directories(init
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)