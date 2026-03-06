/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <gtest/gtest.h>
#include <iostream>
extern "C"
{
#include "memallc.h"
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

TEST(memallc_test, get_heap_s)
{
    uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
    void* start = (void*)0x1000;
    init_memallc(start, 0x200000);
	EXPECT_EQ(get_heap_s(), start);
}

TEST(memallc_test, memallc_in_range)
{
    uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
    init_memallc((void*)0x1000, 0x200000);
    uint64_t bytes = 0x1000;
    memallc(bytes);
    EXPECT_EQ(get_allc_memblk()->size, bytes);
}

TEST(memallc_test, memallc_max)
{
    uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
    init_memallc((void*)0x1000, 0x200000);
    uint64_t range = 0x200000;
    uint64_t max_bytes = range - (uintptr_t)get_heap_s();
    memallc(max_bytes);
    EXPECT_EQ(get_allc_memblk()->size, max_bytes);
}

TEST(memallc_test, memallc_over_max)
{
    uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
    init_memallc((void*)0x1000, 0x200000);
    uint64_t range = 0x200000;
    memallc(range + range);
    EXPECT_EQ(get_allc_memblk()->size, range);
}

TEST(memallc_test, free_memallc_addr)
{
    uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
    init_memallc((void*)0x1000, 0x200000);
    uint64_t bytes = 0x1000;
    memallc(bytes);
    void* addr = get_heap_s();
	free_memallc(addr);
    EXPECT_EQ(get_free_memblk()->addr, addr);
}

TEST(memallc_test, free_memallc_mid)
{
    uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
    init_memallc((void*)0x1000, 0x200000);
    uint64_t bytes = 0x1000;
    memallc(bytes);
    memallc(bytes);
    memallc(bytes);
    void* addr = (void*)((uintptr_t)get_heap_s() + bytes);
	free_memallc(addr);
    EXPECT_EQ(get_free_memblk()->addr, addr);
    EXPECT_EQ(get_free_memblk()->size, bytes);
    EXPECT_EQ(get_allc_memblk_by_addr(get_heap_s())->addr, get_heap_s());
    EXPECT_EQ(get_allc_memblk_by_addr((void*)((uintptr_t)get_heap_s() + bytes * 2))->addr, (void*)((uintptr_t)get_heap_s() + bytes * 2));
}

TEST(memallc_test, get_and_set_range)
{
    uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
    uint64_t range = 0x100000;
    set_range(range);
    EXPECT_EQ(get_range(), range);
}

TEST(memallc_test, init_memallc)
{
    uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
    void* start = (void*)0x1000;
    uint64_t range = 0x100000;
    init_memallc(start, range);
    EXPECT_EQ(get_heap_s(), start);
    EXPECT_EQ(get_range(), range);
    EXPECT_EQ(get_free_memblk()->addr, start);
}

TEST(memallc_test, defragment_free_memblk)
{
    uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
    init_memallc((void*)0x1000, 0x200000);
    uint64_t bytes = 0x1000;
    memallc(bytes);
    memallc(bytes);
    memallc(bytes);
    void* addr1 = get_heap_s();
    void* addr2 = (void*)((uintptr_t)get_heap_s() + bytes);
    void* addr3 = (void*)((uintptr_t)get_heap_s() + bytes * 2);
    free_memallc(addr2);
    free_memallc(addr1);
    EXPECT_EQ(get_free_memblk()->addr, addr1);
    EXPECT_EQ(get_free_memblk()->size, bytes * 2);
    EXPECT_EQ(get_allc_memblk_by_addr(addr3)->addr, addr3);
}