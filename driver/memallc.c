/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

 #include "memallc.h"
 #include "stddef.h"

uintptr_t heap_s = 0x1000;
uint64_t min_bytes = 0x00000000000001F0;
uint64_t range = 0x200000;
memblk_t* free_memblk_list;
memblk_t* allc_memblk_list;

void init_memallc(uintptr_t s, uint64_t r)
{
    heap_s = s;
    range = r;
    free_memblk_list = new_memblk(heap_s, range);
}

void set_range(uint64_t r)
{
    range = r;
}

uint64_t get_range()
{
    return range;
}

uintptr_t get_heap_s()
{
    return heap_s;
}

memblk_t* get_free_memblk()
{
    return free_memblk_list;
}

memblk_t* get_allc_memblk()
{
    return allc_memblk_list;
}

void memallc(uint64_t size)
{
    if (size < min_bytes)
    {
        allc_memblk(min_bytes);
    }
    else if ((heap_s + size) > (heap_s + range))
    {
        allc_memblk(range - free_memblk_list->addr);
    }
    else
    {
        allc_memblk(size);
    }
}

void free_memallc(uintptr_t addr)
{
    
}

memblk_t* allc_memblk(uint64_t size)
{
	
}

void free_memblk(uintptr_t addr, uint64_t size)
{
    
}

memblk_t* new_memblk(uintptr_t addr, uint64_t size)
{
    memblk_t* block;
    block->addr = addr;
    block->size = size;
    block->next = NULL;
    return block;
}