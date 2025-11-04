add_library(
    driver
    STATIC
    uart.c
)

registerFilesToFormat(FILES uart.c)

target_include_directories(driver
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)