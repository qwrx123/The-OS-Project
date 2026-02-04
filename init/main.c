/*
 * Copyright (c) [year] [your name]
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "uart.h"

int kernel_init(void *name)
{
#ifdef QEMU
	uart_init(0, id_pl011);
#else
	uart_init(0, id_16550);
#endif
	if (name)
	{
	}
	uart_puts("Hello world");
	return 0;
}