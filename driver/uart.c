/*
 * Copyright (c) [year] [your name]
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "uart.h"
#include "kernel/stddef.h"

#ifdef TESTING
void (*uart_fr_callback)() = NULL;
void (*uart_dr_callback)() = NULL;
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
#ifdef TESTING
		// Call the test callback if it's set
		if (uart_fr_callback != NULL)
		{
			uart_fr_callback();
		}
#endif
	}
	UART->DR = (unsigned int)c;
#ifdef TESTING
	// Call the test callback if it's set
	if (uart_dr_callback != NULL)
	{
		uart_dr_callback();
	}
#endif
}

void uart_puts(const char *s)
{
	if (!s)
	{
		return;
	}
	while (*s)
	{
		uart_putc(*s++);
	}
}