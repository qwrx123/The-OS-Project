/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "uart.h"
#include "kernel/stddef.h"

#ifdef TESTING
void (*uart_fr_callback)() = NULL;
void (*uart_dr_callback)() = NULL;
void (*uart_lsr_callback)() = NULL;
void (*uart_tbr_callback)() = NULL;
#endif

void (*uart_putc_impl)(char) = NULL;
void (*uart_puts_impl)(const char *s) = NULL;

void uart_putc_pl011(char c);
void uart_puts_pl011(const char *s);

void uart_putc_16550(char c);
void uart_puts_16550(const char *s);

static uart_regs_t *UART = (uart_regs_t *)UARTADDRESS;

void uart_init(uart_regs_t *uart_device, uart_type init_type)
{
	if (uart_device != NULL)
	{
		UART = uart_device;
	}
	else
	{
		UART = (uart_regs_t *)UARTADDRESS;
	}
	switch (init_type)
	{
#ifndef QEMU
	case id_default:
	default:
#endif
	case id_16550:
		UART->id_16550.IIR_FCR = 0x01;
		uart_putc_impl = &uart_putc_16550;
		uart_puts_impl = &uart_puts_16550;
		break;
	case id_pl011:
#ifdef QEMU
	case id_default:
	default:
#endif
		uart_putc_impl = &uart_putc_pl011;
		uart_puts_impl = &uart_puts_pl011;
		break;
	}
}

void uart_putc(char c)
{
	if (uart_putc_impl)
	{
		uart_putc_impl(c);
	}
}

void uart_puts(const char *s)
{
	if (uart_puts_impl)
	{
		uart_puts_impl(s);
	}
}

void uart_putc_pl011(char c)
{
	while (UART->id_pl011.FR & UARTPL011FLAGFULL)
	{
#ifdef TESTING
		// Call the test callback if it's set
		if (uart_fr_callback != NULL)
		{
			uart_fr_callback();
		}
#endif
	}
	UART->id_pl011.DR = (unsigned int)c;
#ifdef TESTING
	// Call the test callback if it's set
	if (uart_dr_callback != NULL)
	{
		uart_dr_callback();
	}
#endif
}

void uart_puts_pl011(const char *s)
{
	if (!s)
	{
		return;
	}
	while (*s)
	{
		uart_putc_pl011(*s++);
	}
}

void uart_putc_16550(char c)
{
	while (!(UART->id_16550.LSR & (UART16550FLAGFULL)))
	{
#ifdef TESTING
		if (uart_lsr_callback != NULL)
		{
			uart_lsr_callback();
		}
#endif
	}
	UART->id_16550.RBR_TBR = (unsigned int)c;
#ifdef TESTING
	if (uart_tbr_callback != NULL)
	{
		uart_tbr_callback();
	}
#endif
}

void uart_puts_16550(const char *s)
{
	if (!s)
	{
		return;
	}
	while (*s)
	{
		uart_putc_16550(*s++);
	}
}