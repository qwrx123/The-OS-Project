/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "uart.h"
#include "memallc.h"
#include "timer.h"

int kernel_init(void *name)
{
#ifdef QEMU
	uart_init(0, id_pl011);
#else
	uart_init(0, id_16550);
#endif
	init_timer();
	enable_timer();
	init_memallc((void *)0x400000, 0x200000);
	if (name)
	{
	}
	uart_puts("Hello world\r\n");
	memallc(0x1000);
	free_memallc(memallc(0x1000));
	timer_to_string();
	return 0;
}