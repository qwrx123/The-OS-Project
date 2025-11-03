cmake_minimum_required(VERSION 3.20)

set(CROSS "aarch64-none-elf-" CACHE STRING "Cross-compile prefix (e.g., aarch64-none-elf)")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(NOT CMAKE_C_COMPILER)
  set(CMAKE_C_COMPILER "${CROSS}gcc")
endif()
if(NOT CMAKE_ASM_COMPILER)
  set(CMAKE_ASM_COMPILER "${CROSS}gcc")
endif()

project(TheOSProject LANGUAGES C ASM)

file(WRITE null.c "") #Needed to make a executable without a source

# Include helpful modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(helpfulFunctions)

# Add clang-format support
find_program(CLANG_FORMAT clang-format)

add_executable(kernel
  null.c)

target_compile_options(kernel PRIVATE -nostdlib)
target_link_options(kernel PRIVATE -nostdlib -T ${CMAKE_SOURCE_DIR}/linker.ld)

add_subdirectory(arch/arm64/kernel)
add_subdirectory(init)
add_subdirectory(driver)

target_link_libraries(kernel
  arch_kernel
  init
  driver)
  
set_target_properties(kernel PROPERTIES OUTPUT_NAME kernel SUFFIX .elf)

find_program(OBJCOPY_BIN NAMES ${CROSS}objcopy objcopy REQUIRED)
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
  DEPENDS kernel
  COMMAND ${OBJCOPY_BIN} -O binary $<TARGET_FILE:kernel> ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
  COMMENT "Generating kernel.bin"
)
add_custom_target(kernel_bin ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin)

add_custom_target(run
  DEPENDS kernel_bin
  COMMAND qemu-system-aarch64 -M virt -cpu cortex-a57 -m 2G -nographic
          -device loader,file=${CMAKE_CURRENT_BINARY_DIR}/kernel.bin,addr=0x80000000,cpu-num=0
  USES_TERMINAL
)