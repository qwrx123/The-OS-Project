/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <gtest/gtest.h>
#include <iostream>
extern "C"
{
#include "timer.h"
#include <uart.h>
}

typedef struct
{
	volatile unsigned int DR;
	volatile unsigned int RSR_ECR;
	volatile unsigned int reserved1[4];
	volatile unsigned int FR;
	volatile unsigned int reserved2;
	volatile unsigned int ILPR;
	volatile unsigned int IBRD;
	volatile unsigned int FBRD;
	volatile unsigned int LCR_H;
	volatile unsigned int CR;
	volatile unsigned int IFLS;
	volatile unsigned int IMSC;
	volatile unsigned int RIS;
	volatile unsigned int MIS;
	volatile unsigned int ICR;
	volatile unsigned int DMACR;
	volatile unsigned int reserved3[997];
	volatile unsigned int PeriphID0;
	volatile unsigned int PeriphID1;
	volatile unsigned int PeriphID2;
	volatile unsigned int PeriphID3;
	volatile unsigned int PCellID0;
	volatile unsigned int PCellID1;
	volatile unsigned int PCellID2;
	volatile unsigned int PCellID3;
} uart_mock_regs_t;

static uart_mock_regs_t UARTMOCK = { 0 };
#define UARTDEFAULTRESET 100;
static int UARTRESETCYCLES = 0;
static int UARTFRREAD = 0;
static std::string console;
extern void (*uart_fr_callback)();
extern void (*uart_dr_callback)();
extern void (*uart_lsr_callback)();
extern void (*uart_tbr_callback)();

TEST(timer_test, init_timer)
{
	uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
	init_timer();
	EXPECT_GT(get_timer_freq(), 0);
	EXPECT_GT(get_time(), 0);
	EXPECT_GT(get_timer_sec(), 0);
	EXPECT_GT(get_timer_ctl(), 0);
}

TEST(timer_test, enable_timer)
{
	uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
	init_timer();
	enable_timer();
	uint64_t ctl = get_timer_ctl();
	EXPECT_EQ(ctl & 5, 5);
}

TEST(timer_test, disable_timer)
{
	uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
	init_timer();
	enable_timer();
	disable_timer();
	uint64_t ctl = get_timer_ctl();
	EXPECT_EQ(ctl & 5, 0);
}

TEST(timer_test, get_time)
{
	uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
	init_timer();
	uint64_t t1 = get_time();
	uint64_t t2 = get_time();
	EXPECT_GT(t2, t1);
}

TEST(timer_test, get_timer_freq)
{
	uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
	init_timer();
	uint64_t f = get_timer_freq();
	EXPECT_GT(f, 0);
}

TEST(timer_test, get_timer_ctl)
{
	uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
	init_timer();
	uint64_t ctl = get_timer_ctl();
	EXPECT_EQ(ctl & 5, 0);
}

TEST(timer_test, get_timer_sec)
{
	uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
	init_timer();
	uint64_t sec1 = get_timer_sec();
	uint64_t sec2 = get_timer_sec();
	EXPECT_GE(sec2, sec1);
}