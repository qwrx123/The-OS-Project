/*
 * Copyright (c) [year] [your name]
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "uart.h"

int kernel_init(void *name)
{
	if (name)
	{
	}
	uart_puts("Hello world");
	return 0;
}