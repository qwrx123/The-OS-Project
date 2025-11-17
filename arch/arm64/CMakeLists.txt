file(WRITE null.c "") #Needed to make a executable without a source

add_library(
    arch_aarch64
    STATIC
    null.c
    $<TARGET_OBJECTS:arch_kernel>
    $<TARGET_OBJECTS:arch_memorymanager>
)

add_subdirectory(kernel)
add_subdirectory(memorymanager)