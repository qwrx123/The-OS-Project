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
    uintptr_t start = 0x1000;
	EXPECT_EQ(get_heap_s(), start);
}

TEST(memallc_test, memallc_under_min_bytes)
{
    uint64_t min_bytes = 0x00000000000001F0;
    memallc(0x000000000000000A0);
}

TEST(memallc_test, memallc_min_bytes)
{
    uint64_t min_bytes = 0x00000000000001F0;
    memallc(min_bytes);
}

TEST(memallc_test, memallc_in_range)
{
    uint64_t bytes = 0x1000;
    memallc(bytes);
}

TEST(memallc_test, memallc_max)
{
    uint64_t range = 0x200000;
    uint64_t max_bytes = range - get_heap_s();
    memallc(max_bytes);
}

TEST(memallc_test, memallc_over_max)
{
    uint64_t range = 0x200000;
    memallc(range + range);
}

TEST(memallc_test, free_memallc_addr)
{
	uintptr_t end = 0x4000;
    uintptr_t addr = get_heap_s();
	free_memallc(addr);
}

TEST(memallc_test, get_and_set_range)
{
    uint64_t range = 0x100000;
    set_range(range);
    EXPECT_EQ(get_range(), range);
}

TEST(memallc_test, init_memallc)
{
    uintptr_t start = 0x1000;
    uint64_t range = 0x100000;
    init_memallc(start, range);
    EXPECT_EQ(get_heap_s(), start);
    EXPECT_EQ(get_range(), range);
    EXPECT_EQ(get_free_memblk()->addr, start);
}