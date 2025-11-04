#include "uart.h"

void uart_putc(char c)
{
	while (UARTFR & UARTFLAGFULL)
	{
	}
	UARTDR = (unsigned int)c;
}

void uart_puts(char *s)
{
	while (*s)
	{
		uart_putc(*s++);
	}
}