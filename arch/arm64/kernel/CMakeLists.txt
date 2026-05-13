#
# Copyright (c) 2026 Nathaniel Smith
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(
    arch_kernel
    STATIC
    head.S
    entry.S
    irq.c
    traps.c
    gic.c
)

registerFilesToFormat(FILES gic.c gic.h 
                    irq.c traps.c
                    socfunctions_gic.h)

target_compile_options(arch_kernel PRIVATE
  -nostdlib 
  -ffreestanding
  -Wall
  -Wextra
  -Wpedantic
  -Werror)

target_include_directories(arch_kernel
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)