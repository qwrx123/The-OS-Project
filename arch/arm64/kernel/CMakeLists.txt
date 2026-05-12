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
)

target_compile_options(arch_kernel PRIVATE
  -nostdlib 
  -ffreestanding
  -Wall
  -Wextra
  -Wpedantic
  -Werror)