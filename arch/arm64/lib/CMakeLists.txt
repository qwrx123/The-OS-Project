#
# Copyright (c) 2026 Nathaniel Smith
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(
    arch_lib
    STATIC
    memset.c 
)

registerFilesToFormat(FILES memset.c)

target_include_directories(arch_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)