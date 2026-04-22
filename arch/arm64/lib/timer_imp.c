/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "kernel/timer_imp.h"
#include "kernel/string.h"
#include "kernel/stddef.h"
#include "kernel/stdint.h"
#include "kernel/types.h"

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


void init_timer_imp()
{
	freq = read_cntfrq_el0();
	time = read_cntpct_el0();
	sec = time / freq;
	write_cntps_cval_el1(time + freq);
}

void enable_timer_imp()
{
	uint64_t ctl = read_cntps_ctl_el1();
	write_cntps_ctl_el1(ctl ^ 5);
}

void disable_timer_imp()
{
	uint64_t ctl = read_cntps_ctl_el1();
	write_cntps_ctl_el1(ctl ^ 4);
}

void timer_imp_interrupt()
{
	time = read_cntpct_el0();
	interrupt = read_cntps_cval_el1() + freq;
	write_cntps_cval_el1(interrupt);
	timer_to_string_imp();
}

uint64_t get_time_imp()
{
	return read_cntpct_el0();
}

uint64_t get_timer_freq_imp()
{
	return read_cntfrq_el0();
}

uint64_t get_timer_ctl_imp()
{
	return read_cntps_ctl_el1();
}

uint64_t get_timer_sec_imp()
{
	time = read_cntpct_el0();
	return (time / freq);
}

void timer_to_string_imp()
{
	// uint64_t sec = (time / freq);
	// TODO: Implement a more robust integer to string conversion function that can handle larger numbers and edge cases.
	/*
	uint64_t sec = (time / freq);
	uint64_t temp = sec;
	uint64_t length = 0;
	while (temp > 0)
	{
		length++;
		temp /= 10;
	}
	char* seconds[20];
	for (uint64_t i = length - 1; i >= 0; i--)
	{
		seconds[i] = sec % 10 + '0';
		sec /= 10;
	}
	seconds[length] = '\0';
	uart_puts(seconds);
	*/
}
