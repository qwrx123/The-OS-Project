/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "timer.h"

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

void init_timer()
{
	freq = read_cntfrq_el0();
	time = read_cntpct_el0();
}

void enable_timer()
{
	uint64_t ctl = read_cntps_ctl_el1();
	write_cntps_ctl_el1(ctl ^ 5);
}

void disable_timer()
{
    uint64_t ctl = read_cntps_ctl_el1();
    write_cntps_ctl_el1(ctl ^ 4);
}

void timer_interrupt()
{
	// Handle the timer interrupt
	// This function should be called when the timer interrupt occurs
	// You can add your interrupt handling code here
}

uint64_t get_time()
{
	return read_cntpct_el0();
}

uint64_t get_timer_freq()
{
	return read_cntfrq_el0();
}
