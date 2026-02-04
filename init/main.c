/*
 * Copyright (c) [year] [your name]
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "uart.h"

int kernel_init(void *)
{
	uart_init(0, id_16550);
	uart_puts("Hello world");
	return 0;
}