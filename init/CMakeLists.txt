#
# Copyright (c) 2026 Nathaniel Smith
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(
    init
    STATIC
    main.c
    demo.c
)

registerFilesToFormat(FILES main.c
                        demo.c)

target_compile_options(init PRIVATE
  -nostdlib 
  -ffreestanding
  -Wall
  -Wextra
  -Wpedantic
  -Werror)

target_include_directories(init
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)