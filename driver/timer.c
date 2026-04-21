/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "timer.h"
#include "uart.h"
#include "kernel/string.h"
#include "kernel/stddef.h"

static inline uint64_t read_cntfrq_el0(void)
{
	uint64_t v;
	__asm__ volatile("mrs %0, cntfrq_el0" : "=r"(v));
	return v;
}

static inline uint64_t read_cntpct_el0(void)
{
	uint64_t v;
	__asm__ volatile("mrs %0, cntpct_el0" : "=r"(v));
	return v;
}

static inline uint64_t read_cntps_ctl_el1(void)
{
	uint64_t v;
	__asm__ volatile("mrs %0, cntps_ctl_el1" : "=r"(v));
	return v;
}

static inline void write_cntps_ctl_el1(uint64_t v)
{
	__asm__ volatile("msr cntps_ctl_el1, %0" ::"r"(v));
}

static inline uint64_t read_cntps_cval_el1(void)
{
	uint64_t v;
	__asm__ volatile("mrs %0, cntps_cval_el1" : "=r"(v));
	return v;
}

static inline void write_cntps_cval_el1(uint64_t v)
{
	__asm__ volatile("msr cntps_cval_el1, %0" ::"r"(v));
}

void init_timer()
{
	freq = read_cntfrq_el0();
	time = read_cntpct_el0();
	write_cntps_cval_el1(time + freq);
	uart_puts("timer: initialized\r\n");
}

void enable_timer()
{
	uint64_t ctl = read_cntps_ctl_el1();
	write_cntps_ctl_el1(ctl ^ 5);
	uart_puts("timer: enabled\r\n");
}

void disable_timer()
{
	uint64_t ctl = read_cntps_ctl_el1();
	write_cntps_ctl_el1(ctl ^ 4);
	uart_puts("timer: disabled\r\n");
}

void timer_interrupt()
{
	time = read_cntpct_el0();
	interrupt = read_cntps_cval_el1() + freq;
	write_cntps_cval_el1(interrupt);
	uart_puts("timer: interrupt occurred\r\n");
	to_string();
}

uint64_t get_time()
{
	uart_puts("timer: got time\r\n");
	return read_cntpct_el0();
}

uint64_t get_timer_freq()
{
	uart_puts("timer: got frequency \r\n");
	return read_cntfrq_el0();
}

char *to_string()
{
	uart_puts("timer: kernal has been running for ");
	// uint64_t seconds = (time / freq);
	// TODO: Implement a more robust integer to string conversion function that can handle larger numbers and edge cases.
	// uart_puts(seconds);
	uart_puts(" seconds\r\n");
	return NULL;
}
