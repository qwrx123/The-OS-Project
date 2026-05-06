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

#ifdef QEMU
uint64_t q_freq = 0;
uint64_t q_time = 0;
uint64_t q_interrupt = 0;
uint64_t q_sec = 0;
uint64_t q_ctl = 0;
#endif

void init_timer()
{
#ifdef QEMU
	uart_puts("timer: initialized timer (QEMU)\r\n");
	q_freq = 1400000000 / 6;
	q_time = 0;
	q_sec = 0;
	q_interrupt = q_time + q_freq;
	q_ctl = 8;

#else
	uart_puts("timer: initialized timer\r\n");
	init_timer_imp();
#endif
}

void enable_timer()
{
#ifdef QEMU
	uart_puts("timer: enabled (QEMU)\r\n");
	q_ctl |= 5u;
#else
	enable_timer_imp();
	uart_puts("timer: enabled\r\n");
#endif
}

void disable_timer()
{
#ifdef QEMU
	uart_puts("timer: disabled (QEMU)\r\n");
	q_ctl &= ~5u;
#else
	disable_timer_imp();
	uart_puts("timer: disabled\r\n");
#endif
}

void timer_interrupt()
{
#ifdef QEMU
	uart_puts("timer: interrupt occurred (QEMU)\r\n");
	timer_to_string();
#else
	timer_imp_interrupt();
	uart_puts("timer: interrupt occurred\r\n");
#endif
}

uint64_t get_time()
{
#ifdef QEMU
	uart_puts("timer: got time (QEMU)\r\n");
	return ++q_time;
#else
	uart_puts("timer: got time\r\n");
#endif
	return get_time_imp();
}

uint64_t get_timer_freq()
{
#ifdef QEMU
	uart_puts("timer: got frequency (QEMU)\r\n");
	return q_freq;
#else
	uart_puts("timer: got frequency \r\n");
	return get_timer_freq_imp();
#endif
}

uint64_t get_timer_ctl()
{
#ifdef QEMU
	uart_puts("timer: got control register value (QEMU)\r\n");
	return q_ctl;
#else
	uart_puts("timer: got control register value\r\n");
	return get_timer_ctl_imp();
#endif
}

uint64_t get_timer_sec()
{
#ifdef QEMU
	uart_puts("timer: got time in seconds (QEMU)\r\n");
	return ++q_sec;
#else
	uart_puts("timer: got time in seconds\r\n");
	return get_timer_sec_imp();
#endif
}

void timer_to_string()
{
#ifdef QEMU
	static char str_sec[20];
	int len = 20;
	uint64_t sec = get_timer_sec();
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

	uart_puts("timer: kernel has been running for ");
	uart_puts(str_sec);
	uart_puts(" seconds(QEMU)\r\n");
	return;
#else
	uart_puts("timer: kernel has been running for ");
	uart_puts(timer_to_string_imp());
	uart_puts(" seconds\r\n");
#endif
}