#include "uart.h"
#include "kernel/stddef.h"
#ifdef TESTING

#endif
static uart_regs_t *UART = (uart_regs_t *)UARTADDRESS;

void uart_init(uart_regs_t *uart_device)
{
	if (uart_device != NULL)
	{
		UART = uart_device;
	}
	else
	{
		UART = (uart_regs_t *)UARTADDRESS;
	}
}

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