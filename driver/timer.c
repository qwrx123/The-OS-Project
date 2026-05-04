/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "timer.h"
#include "uart.h"
#include "kernel/timer_imp.h"
#include "kernel/string.h"
#include "kernel/stddef.h"
#include "kernel/stdint.h"

void init_timer()
{
	init_timer_imp();
	uart_puts("timer: initialized\r\n");
}

void enable_timer()
{
	enable_timer_imp();
	uart_puts("timer: enabled\r\n");
}

void disable_timer()
{
	disable_timer_imp();
	uart_puts("timer: disabled\r\n");
}

void timer_interrupt()
{
	timer_imp_interrupt();
	uart_puts("timer: interrupt occurred\r\n");
}

uint64_t get_time()
{
	uart_puts("timer: got time\r\n");
	return get_time_imp();
}

uint64_t get_timer_freq()
{
	uart_puts("timer: got frequency \r\n");
	return get_timer_freq_imp();
}

uint64_t get_timer_ctl()
{
	uart_puts("timer: got control register value\r\n");
	return get_timer_ctl_imp();
}

uint64_t get_timer_sec()
{
	uart_puts("timer: got time in seconds\r\n");
	return get_timer_sec_imp();
}

void timer_to_string()
{
	uart_puts("timer: kernel has been running for ");

	char str_sec[20];
	int len = 20;

	uint64_t sec = get_timer_sec_imp();

	int i = len - 1;
	str_sec[i] = '\0';

	if (sec == 0)
	{
		if (i > 0)
		{
			str_sec[--i] = '0';
		}
	}
	else
	{
		while (sec > 0 && i > 0)
		{
			str_sec[--i] = (char)('0' + (sec % 10));
			sec /= 10;
		}
	}

	int j = 0;
	while (str_sec[i] != '\0')
	{
		str_sec[j++] = str_sec[i++];
	}
	str_sec[j] = '\0';

	uart_puts(str_sec);

	uart_puts(" seconds\r\n");
}
