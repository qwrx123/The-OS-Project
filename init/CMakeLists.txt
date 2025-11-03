add_library(
    init
    STATIC
    main.c
    uart.c
)

registerFilesToFormat(FILES main.c uart.c)

target_include_directories(init
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)