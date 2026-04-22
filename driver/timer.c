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
	uart_puts("timer: kernal has been running for ");
	timer_to_string_imp();
	uart_puts(" seconds\r\n");
}
