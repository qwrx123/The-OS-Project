/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdint.h>

extern void uart_puts(const char *s);

void el1_irq_handler(/*struct pt_regs *regs*/)
{
    uart_puts("el1_irq: interrupt\n");
}