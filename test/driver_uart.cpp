#include <gtest/gtest.h>
extern "C" {
    #include "uart.h"
}
TEST(UART, test)
{
    uart_init(NULL);
}