/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

 #include "memallc.h"

uintptr_t heap_s = 0x1000;
uintptr_t heap_e;
uint64_t min_bytes = 0x00000000000001F0;
uint64_t range = 0x400000;

void set_heap_e(phys_addr_t e)
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
    
}

void free_memallc()
{
    heap_e = heap_s;
}