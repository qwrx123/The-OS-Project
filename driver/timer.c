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
#ifdef QEMU
	uart_puts("timer: initialized timer (QEMU)\r\n");
#else
	uart_puts("timer: initialized timer\r\n");
	init_timer_imp();
#endif
}

void enable_timer()
{
#ifdef QEMU
	uart_puts("timer: enabled (QEMU)\r\n");
#else
	enable_timer_imp();
	uart_puts("timer: enabled\r\n");
#endif
}

void disable_timer()
{
#ifdef QEMU
	uart_puts("timer: disabled (QEMU)\r\n");
#else
	disable_timer_imp();
	uart_puts("timer: disabled\r\n");
#endif
}

void timer_interrupt()
{
#ifdef QEMU
	uart_puts("timer: interrupt occurred (QEMU)\r\n");
#else
	timer_imp_interrupt();
	uart_puts("timer: interrupt occurred\r\n");
#endif
}

uint64_t get_time()
{
#ifdef QEMU
	uart_puts("timer: got time (QEMU)\r\n");
	return 0;
#else
	uart_puts("timer: got time\r\n");
#endif
	return get_time_imp();
}

uint64_t get_timer_freq()
{
#ifdef QEMU
	uart_puts("timer: got frequency (QEMU)\r\n");
	return 0;
#else
	uart_puts("timer: got frequency \r\n");
	return get_timer_freq_imp();
#endif
}

uint64_t get_timer_ctl()
{
#ifdef QEMU
	uart_puts("timer: got control register value (QEMU)\r\n");
	return 0;
#else
	uart_puts("timer: got control register value\r\n");
	return get_timer_ctl_imp();
#endif
}

uint64_t get_timer_sec()
{
#ifdef QEMU
	uart_puts("timer: got time in seconds (QEMU)\r\n");
	return 0;
#else
	uart_puts("timer: got time in seconds\r\n");
	return get_timer_sec_imp();
#endif
}

void timer_to_string()
{
#ifdef QEMU
	uart_puts(
		"timer: kernel has been running for [unknown] seconds (QEMU)\r\n");
	return;
#else
	uart_puts("timer: kernel has been running for ");
	uart_puts(timer_to_string_imp());
	uart_puts(" seconds\r\n");
#endif
}