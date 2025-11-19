add_library(
    arch_lib
    STATIC
    memset.c 
)

registerFilesToFormat(FILES memset.c)

target_include_directories(arch_memorymanager
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)