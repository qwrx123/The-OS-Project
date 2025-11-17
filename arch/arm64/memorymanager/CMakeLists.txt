add_library(
    arch_memorymanager
    STATIC
    mmu.c    
)

registerFilesToFormat(FILES socfunctions.h
                        mmu.c mmu.h)

target_include_directories(arch_memorymanager
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)