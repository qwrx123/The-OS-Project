#
# Copyright (c) 2026 Nathaniel Smith
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(
    lib
    STATIC
    itoa.c
)

registerFilesToFormat(FILES itoa.c)

target_compile_options(lib PRIVATE
  -nostdlib 
  -ffreestanding
  -Wall
  -Wextra
  -Wpedantic
  -Werror)

target_include_directories(lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)