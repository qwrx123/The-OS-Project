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

uint64_t freq = 0;
uint64_t time = 0;
uint64_t interrupt = 0;
uint64_t sec = 0;

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

static inline uint64_t read_cntp_ctl_el0(void)
{
	uint64_t v;
	__asm__ volatile("mrs %0, cntp_ctl_el0" : "=r"(v));
	return v;
}

static inline void write_cntp_ctl_el0(uint64_t v)
{
	__asm__ volatile("msr cntp_ctl_el0, %0" ::"r"(v));
}

static inline uint64_t read_cntp_cval_el0(void)
{
	uint64_t v;
	__asm__ volatile("mrs %0, cntp_cval_el0" : "=r"(v));
	return v;
}

static inline void write_cntp_cval_el0(uint64_t v)
{
	__asm__ volatile("msr cntp_cval_el0, %0" ::"r"(v));
}

void init_timer_imp()
{
	freq = read_cntfrq_el0();
	time = read_cntpct_el0();
	sec = time / freq;
	write_cntp_cval_el0(time + freq);
}

void enable_timer_imp()
{
	uint64_t ctl = read_cntp_ctl_el0();
	ctl |= 5u;
	ctl &= ~2u;
	write_cntp_ctl_el0(ctl ^ 5);
}

void disable_timer_imp()
{
	uint64_t ctl = read_cntp_ctl_el0();
	ctl &= ~5u;
	write_cntp_ctl_el0(ctl);
}

void timer_imp_interrupt()
{
	time = read_cntpct_el0();
	interrupt = read_cntp_cval_el0() + freq;
	write_cntp_cval_el0(interrupt);
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
	return read_cntp_ctl_el0();
}

uint64_t get_timer_sec_imp()
{
	time = read_cntpct_el0();
	return (time / freq);
}

char *timer_to_string_imp()
{
	static char str_sec[20];
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

	return str_sec;
}
