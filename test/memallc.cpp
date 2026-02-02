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
    uintptr_t heap_s = 0x1000;
	EXPECT_EQ(get_heap_s(), heap_s);
}

TEST(memallc_test, set_and_get_heap_e)
{
	phys_addr_t end = 0x2000;
	set_heap_e(end);
	EXPECT_EQ(get_heap_e(), end);
}

TEST(memallc_test, set_heap_e_out_of_range)
{
    uint64_t range = 0x400000;
	phys_addr_t end = range + range;
	set_heap_e(end);
	EXPECT_EQ(get_heap_e(), get_heap_s() + range);
}

TEST(memallc_test, memallc_under_min_bytes)
{
}

TEST(memallc_test, memallc_min_bytes)
{
}

TEST(memallc_test, memallc_in_range)
{
}

TEST(memallc_test, memallc_max)
{
}

TEST(memallc_test, memallc_over_max)
{
}

TEST(memallc_test, free_memallc)
{
	phys_addr_t end = 0x4000;
	set_heap_e(end);
	free_memallc();
	EXPECT_EQ(get_heap_e(), get_heap_s());
}