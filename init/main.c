/*
 * Copyright (c) [year] [your name]
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "uart.h"
#include "memallc.h"

int kernel_init(void *name)
{
#ifdef QEMU
	uart_init(0, id_pl011);
	init_memallc((void*)0x400000, 0x200000);
#else
	uart_init(0, id_16550);
	init_memallc((void*)0x400000, 0x200000);
#endif
	if (name)
	{
	}
	uart_puts("Hello world\n");
	memallc(0x1000);
	free_memallc(memallc(0x1000));
	return 0;
}