add_library(
    arch_memorymanager
    STATIC
    mmu.c
    board_devices.c 
)

registerFilesToFormat(FILES socfunctions.h
                        mmu.c mmu.h
                        board_devices.c
                        board_devices.h)

target_include_directories(arch_memorymanager
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)