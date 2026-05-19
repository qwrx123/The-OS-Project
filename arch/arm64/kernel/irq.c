/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <kernel/stdint.h>
#include "gic.h"

extern void uart_puts(const char *s);
extern void uart_putc(char c);

void el1_irq_handler(/*struct pt_regs *regs*/)
{
	uart_putc('*'); // Just landed in handler
	uint32_t irq = gic_get_active_irq();
	uart_puts("el1_irq: interrupt\r\n");
	gic_eoi(irq);
}