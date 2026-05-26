#
# Copyright (c) 2026 Jess Knue
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(
    arch_scheduler
    STATIC
    reg_switch.S
)

target_compile_options(arch_scheduler PRIVATE
  -nostdlib 
  -ffreestanding
  -Wall
  -Wextra
  -Wpedantic
  -Werror)