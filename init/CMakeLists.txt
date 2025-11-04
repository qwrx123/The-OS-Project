add_library(
    init
    STATIC
    main.c
)

registerFilesToFormat(FILES main.c)

target_include_directories(init
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)