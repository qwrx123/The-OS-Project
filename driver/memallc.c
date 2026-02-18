/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

 #include "memallc.h"
 #include "stddef.h"

#define MAX_MEMBLK_COUNT 1024
static memblk_t memblk_pool[MAX_MEMBLK_COUNT];
static int memblk_pool_index = 0;
void* heap_s = NULL;
uint64_t range = 0x200000;
memblk_t* free_memblk_list;
memblk_t* allc_memblk_list;
static memblk_t* allc_memblk_impl(uint64_t size, memblk_t* prev, memblk_t* block);
static memblk_t* allc_memblk_impl_split(uint64_t size, memblk_t* prev, memblk_t* block);
static void defragment_memblk(memblk_t* block);

void init_memallc(void* s, uint64_t r)
{
    heap_s = s;
    range = r;
    memblk_pool_index = 0;
    free_memblk_list = new_memblk(heap_s, range);
    allc_memblk_list = NULL;
}

void set_range(uint64_t r)
{
    range = r;
}

uint64_t get_range()
{
    return range;
}

void* get_heap_s()
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

void* memallc(uint64_t size)
{
    if (!free_memblk_list)
    {
        return NULL;
    }
    memblk_t* block = NULL;
    if (size > range)
    {   
        block = allc_memblk(range);
    }
    else
    {
        block = allc_memblk(size);
    }
    return block ? block->addr : NULL;
}

void free_memallc(void* addr)
{
    if (!addr || !allc_memblk_list)
    {
        return;
    } 
    memblk_t* block = NULL;
    memblk_t* prev = NULL;
    memblk_t* current = allc_memblk_list;
    while (current)
    {
        if (current->addr == addr)
        {
            block = current;
            break;
        }
        prev = current;
        current = current->next;
    }
    if (!block)
    {
        return;
    }
    if (prev)
    {
        prev->next = block->next;
    }
    else
    {
        allc_memblk_list = block->next;
    }
    block->next = free_memblk_list;
    free_memblk_list = block;
    defragment_memblk(block);
}

memblk_t* allc_memblk(uint64_t size)
{
    if (!size || !free_memblk_list)
    {
        return NULL;
    }
    memblk_t* prev = NULL;
    memblk_t* block = free_memblk_list;
    return allc_memblk_impl(size, prev, block);
}

static memblk_t* allc_memblk_impl(uint64_t size, memblk_t* prev, memblk_t* block)
{
    while (block)
    {
        if (block->size >= size)
        {
            return allc_memblk_impl_split(size, prev, block);
        }
        prev = block;
        block = block->next;
    }
    return NULL;  
}

static memblk_t* allc_memblk_impl_split(uint64_t size, memblk_t* prev, memblk_t* block)
{
    if (block->size > size)
    {
        memblk_t* new_block = new_memblk((char*)block->addr + size, block->size - size);
        if (new_block)
        {
            new_block->next = block->next;
            block->next = new_block;
            block->size = size;
        }
    }
    if (prev)
    {
        prev->next = block->next;
    }
    else
    {
        free_memblk_list = block->next;
    }
    block->next = allc_memblk_list;
    allc_memblk_list = block;
    return block;
}

static void defragment_memblk(memblk_t* block)
{
    memblk_t* current = free_memblk_list;
    memblk_t* prev_current = NULL;
    while (current)
    {
        if ((char*)current->addr + current->size == block->addr)
        {
            current->size += block->size;
            current->next = block->next;
            return;
        }
        else if ((char*)block->addr + block->size == current->addr)
        {
            block->size += current->size;
            block->next = current->next;
            if (current == free_memblk_list)
            {
                free_memblk_list = block;
            }
            else if (prev_current)
            {
                prev_current->next = block;
            }
            return;
        }
        prev_current = current;
        current = current->next;
    }
    block->next = free_memblk_list;
    free_memblk_list = block;
}

memblk_t* new_memblk(void* addr, uint64_t size)
{
    if (memblk_pool_index >= MAX_MEMBLK_COUNT)
    {
        return NULL;
    }
    memblk_t* block = &memblk_pool[memblk_pool_index];
    memblk_pool_index++;
    block->addr = addr;
    block->size = size;
    block->next = NULL;
    return block;
}

memblk_t* get_allc_memblk_by_addr(void* addr)
{
    memblk_t* current = get_allc_memblk();
    while (current)
    {
        if (current->addr == addr)
            return current;
        current = current->next;
    }
    return NULL;
}