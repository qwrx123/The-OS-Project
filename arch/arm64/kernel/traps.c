#include <stdint.h>

extern void uart_puts(const char *s);

void el1_sync_handler()
{
    uart_puts("el1_sync\n");
}