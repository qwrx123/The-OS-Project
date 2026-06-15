/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <kernel/stdint.h>
#include "gic.h"
#include "uart.h"
#include "stdlib.h"
#include "timer.h"

extern void uart_puts(const char *s);

/**
 * @brief Handles IRQs for EL1
 * 
 */
void el1_irq_handler(/*struct pt_regs *regs*/)
{
	uint32_t irq = gic_get_active_irq();
	char readable_irq[13] = { 0 };
	itoa(irq, readable_irq, 10);
	uart_puts("el1_irq number ");
	uart_puts(readable_irq);
	uart_puts("\r\n");

	if (irq == UART_IRQ)
	{
		char grabbed_key = uart_getc();
		uart_puts("Key pressed is :");
		uart_putc(grabbed_key);
		uart_puts("\r\n");
	}
	else if (irq == TIMER_IRQ)
	{
		timer_interrupt();
	}
	gic_eoi(irq);
}