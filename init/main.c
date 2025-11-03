#include "uart.h"

int kernel_init(void *)
{
	uart_puts("Hello world");
	return 0;
}