/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

 #include "memallc.h"

uintptr_t heap_s = 0x1000;
uintptr_t heap_e;
uint64_t min_bytes = 0x00000000000001F0;
uint64_t range = 0x200000;

void init_memallc(uintptr_t s, uintptr_t e, uint64_t r)
{
    heap_s = s;
    range = r;
    set_heap_e(e);
}

void set_heap_bounds(uintptr_t s, uintptr_t e)
{
    heap_s = s;
    set_heap_e(e);
}

void set_range(uint64_t r)
{
    range = r;
}

uint64_t get_range()
{
    return range;
}

void set_heap_e(uintptr_t e)
{
    if (e > (heap_s + range))
    {
        heap_e = heap_s + range;
    }
    else
    {
        heap_e = e;
    }
}

uintptr_t get_heap_s()
{
    return heap_s;
}

uintptr_t get_heap_e()
{
    return heap_e;
}

void memallc(uint64_t size)
{
    if (size < min_bytes)
    {
        heap_e = heap_s + min_bytes;
    }
    else if ((heap_s + size) > (heap_s + range))
    {
        heap_e = heap_s + range;
    }
    else
    {
        heap_e = heap_s + size;
    }
}

void free_memallc()
{
    heap_e = heap_s;
}