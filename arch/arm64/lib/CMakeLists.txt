#
# Copyright (c) 2026 Nathaniel Smith
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(
    arch_lib
    STATIC
    memset.c 
    timer_imp.c
)

registerFilesToFormat(FILES memset.c timer_imp.c)

target_compile_options(arch_lib PRIVATE
  -nostdlib 
  -ffreestanding
  -Wall
  -Wextra
  -Wpedantic
  -Werror)

target_include_directories(arch_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)