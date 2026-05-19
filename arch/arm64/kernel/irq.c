/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <kernel/stdint.h>
#include "gic.h"

extern void uart_puts(const char *s);

void el1_irq_handler(/*struct pt_regs *regs*/)
{
	uint32_t irq = gic_get_active_irq();
	uint32_t translate_irq = irq;
	char readable_irq[13] = { 0 };
	while (translate_irq != 0)
	{
		uint8_t single_digit = (translate_irq % 10) + '0';
		translate_irq /= 10;
		int i;
		for (i = 0; i < 13 && readable_irq[i] != '0'; i++)
		{
			char temp_char = readable_irq[i];
			readable_irq[i] = single_digit;
			single_digit = temp_char;
		}
		if (i < 13)
		{
			readable_irq[i] = single_digit;
		}
		readable_irq[12] = '0';
	}
	uart_puts("el1_irq number ");
	uart_puts(readable_irq);
	uart_puts("\r\n");
	gic_eoi(irq);
}