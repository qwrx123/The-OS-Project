#include "uart.h"

void uart_putc(char c)
{
	while (UART->FR & UARTFLAGFULL)
	{
	}
	UART->DR = (unsigned int)c;
}

void uart_puts(char *s)
{
	while (*s)
	{
		uart_putc(*s++);
	}
}