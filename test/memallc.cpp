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
}

TEST(memallc_test, get_heap_s)
{
    void* start = (void*)0x1000;
    init_memallc(start, 0x200000);
	EXPECT_EQ(get_heap_s(), start);
}

TEST(memallc_test, memallc_in_range)
{
    init_memallc((void*)0x1000, 0x200000);
    uint64_t bytes = 0x1000;
    memallc(bytes);
    EXPECT_EQ(get_allc_memblk()->size, bytes);
}

TEST(memallc_test, memallc_max)
{
    init_memallc((void*)0x1000, 0x200000);
    uint64_t range = 0x200000;
    uint64_t max_bytes = range - (uintptr_t)get_heap_s();
    memallc(max_bytes);
    EXPECT_EQ(get_allc_memblk()->size, max_bytes);
}

TEST(memallc_test, memallc_over_max)
{
    init_memallc((void*)0x1000, 0x200000);
    uint64_t range = 0x200000;
    memallc(range + range);
    EXPECT_EQ(get_allc_memblk()->size, range);
}

TEST(memallc_test, free_memallc_addr)
{
    init_memallc((void*)0x1000, 0x200000);
    uint64_t bytes = 0x1000;
    memallc(bytes);
    void* addr = get_heap_s();
	free_memallc(addr);
    EXPECT_EQ(get_free_memblk()->addr, addr);
}

TEST(memallc_test, free_memallc_mid)
{
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
    uint64_t range = 0x100000;
    set_range(range);
    EXPECT_EQ(get_range(), range);
}

TEST(memallc_test, init_memallc)
{
    void* start = (void*)0x1000;
    uint64_t range = 0x100000;
    init_memallc(start, range);
    EXPECT_EQ(get_heap_s(), start);
    EXPECT_EQ(get_range(), range);
    EXPECT_EQ(get_free_memblk()->addr, start);
}

TEST(memallc_test, defragment_free_memblk)
{
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