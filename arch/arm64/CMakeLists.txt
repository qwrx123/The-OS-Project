#
# Copyright (c) 2026 Nathaniel Smith
#
# SPDX-License-Identifier: BSD-2-Clause

file(WRITE null.c "typedef int make_iso_compilers_happy;\n") #Needed to make a executable without a source

add_library(
    arch_aarch64
    STATIC
    null.c
    $<TARGET_OBJECTS:arch_kernel>
    $<TARGET_OBJECTS:arch_memorymanager>
    $<TARGET_OBJECTS:arch_lib>
    $<TARGET_OBJECTS:arch_scheduler>
)

target_link_libraries(arch_aarch64 PRIVATE arch_lib)

target_compile_options(arch_aarch64 PRIVATE
  -nostdlib 
  -ffreestanding
  -Wall
  -Wextra
  -Wpedantic
  -Werror)

add_subdirectory(kernel)
add_subdirectory(memorymanager)
add_subdirectory(lib)
add_subdirectory(scheduler)