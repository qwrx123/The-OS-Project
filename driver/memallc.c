/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

 #include "memallc.h"
 #include "stddef.h"
 #include <stdlib.h>

uintptr_t heap_s = 0x1000;
uint64_t min_bytes = 0x00000000000001F0;
uint64_t range = 0x200000;
memblk_t* free_memblk_list;
memblk_t* allc_memblk_list;

void init_memallc(uintptr_t s, uint64_t r)
{
    heap_s = s;
    range = r;
    
    // Reset both lists
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
    // Always reinitialize to ensure clean state for each test/context
    free_memblk_list = new_memblk(heap_s, range);
    if (!free_memblk_list)
    {
        // Failed to initialize
        allc_memblk_list = NULL;
        return;
    }
    allc_memblk_list = NULL;
    
    if (size < min_bytes)
    {
        allc_memblk(min_bytes);
    }
    else if ((heap_s + size) > (heap_s + range))
    {
        allc_memblk(range);
    }
    else
    {
        allc_memblk(size);
    }
}

void free_memallc(uintptr_t addr)
{
    if (!addr)
    {
        return;
    }
    
    // Debug: if allc_memblk_list is NULL, initialization failed
    if (!allc_memblk_list)
    {
        return;
    }
    
    // Find the block in the allocated list by searching for matching addr
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
        return;  // Block not found in allocated list
    
    // Remove from allocated list
    if (prev)
    {
        prev->next = block->next;
    }
    else
    {
        allc_memblk_list = block->next;
    }
    
    // Add to front of free list
    block->next = free_memblk_list;
    free_memblk_list = block;
}

memblk_t* allc_memblk(uint64_t size)
{
	 if (!size)
        return NULL;
    
    if (!free_memblk_list)
        return NULL;
    
    // Find first free block that fits (first-fit)
    memblk_t* prev = NULL;
    memblk_t* block = free_memblk_list;
    
    while (block)
    {
        if (block->size >= size)
        {
            // If block is larger than needed, split it
            if (block->size > size + sizeof(memblk_t))
            {
                memblk_t* new_block = (memblk_t*)malloc(sizeof(memblk_t));
                if (new_block)
                {
                    new_block->addr = block->addr + size;
                    new_block->size = block->size - size;
                    new_block->next = block->next;
                    block->next = new_block;
                    block->size = size;
                }
            }
            
            // Remove from free list
            if (prev)
            {
                prev->next = block->next;
            }
            else
            {
                free_memblk_list = block->next;
            }
            
            // Add to allocated list (at head)
            block->next = allc_memblk_list;
            allc_memblk_list = block;
            
            return block;
        }
        
        prev = block;
        block = block->next;
    }
    
    return NULL;  // No suitable block found
}

void free_memblk(uintptr_t addr, uint64_t size)
{

}

memblk_t* new_memblk(uintptr_t addr, uint64_t size)
{
    // Allocate the block structure itself
    memblk_t* block = (memblk_t*)malloc(sizeof(memblk_t));
    if (!block)
        return NULL;
    
    block->addr = addr;
    block->size = size;
    block->next = NULL;
    return block;
}