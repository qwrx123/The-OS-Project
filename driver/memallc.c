/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "memallc.h"
#include "stddef.h"
#include "uart.h"

#define MAX_MEMBLK_COUNT 1024
static memblk_t memblk_pool[MAX_MEMBLK_COUNT];
static int memblk_pool_index = 0;
void *heap_s = NULL;
uint64_t range = 0x200000;
memblk_t *free_memblk_list;
memblk_t *allc_memblk_list;
static memblk_t *allc_memblk_impl(uint64_t size, memblk_t *prev,
				  memblk_t *block);
static memblk_t *allc_memblk_impl_split(uint64_t size, memblk_t *prev,
					memblk_t *block);
static void defragment_memblk(memblk_t *block);

void init_memallc(void *s, uint64_t r)
{
	heap_s = s;
	range = r;
	memblk_pool_index = 0;
	free_memblk_list = new_memblk(heap_s, range);
	allc_memblk_list = NULL;
	uart_puts("memallc: Memory allocator initialized.\r\n");
}

void set_range(uint64_t r)
{
	range = r;
	uart_puts("memallc: Memory range updated.\r\n");
}

uint64_t get_range()
{
	uart_puts("memallc: Current memory range retrieved.\r\n");
	return range;
}

void *get_heap_s()
{
	uart_puts("memallc: Heap start address retrieved.\r\n");
	return heap_s;
}

memblk_t *get_free_memblk()
{
	uart_puts("memallc: Free memory blocks retrieved.\r\n");
	return free_memblk_list;
}

memblk_t *get_allc_memblk()
{
	uart_puts("memallc: Allocated memory blocks retrieved.\r\n");
	return allc_memblk_list;
}

void *memallc(uint64_t size)
{
	if (!free_memblk_list)
	{
		uart_puts("memallc: No free memory blocks available.\r\n");
		return NULL;
	}
	memblk_t *block = NULL;
	if (size > range)
	{
		uart_puts(
			"memallc: Requested size exceeds range. Allocating maximum available block.\r\n");
		block = allc_memblk(range);
	}
	else
	{
		uart_puts("memallc: Allocating memory block.\r\n");
		block = allc_memblk(size);
	}
	uart_puts("memallc: Memory block allocated.\r\n");
	return block ? block->addr : NULL;
}

void free_memallc(void *addr)
{
	if (!addr || !allc_memblk_list)
	{
		uart_puts(
			"memallc: Invalid address or no allocated memory blocks.\r\n");
		return;
	}
	memblk_t *block = NULL;
	memblk_t *prev = NULL;
	memblk_t *current = allc_memblk_list;
	uart_puts("memallc: Attempting to free memory.\r\n");
	while (current)
	{
		if (current->addr == addr)
		{
			uart_puts(
				"memallc: Found allocated block. Freeing it.\r\n");
			block = current;
			break;
		}
		prev = current;
		current = current->next;
	}
	if (!block)
	{
		uart_puts(
			"memallc: Address not found in allocated memory blocks.\r\n");
		return;
	}
	if (prev)
	{
		uart_puts("memallc: Removing block from allocated list.\r\n");
		prev->next = block->next;
	}
	else
	{
		uart_puts(
			"memallc: Removing block from allocated list. This block is the head of the allocated list.\r\n");
		allc_memblk_list = block->next;
	}
	block->next = free_memblk_list;
	free_memblk_list = block;
	uart_puts(
		"memallc: Block added to free list. Attempting to defragment.\r\n");
	defragment_memblk(block);
}

memblk_t *allc_memblk(uint64_t size)
{
	if (!size || !free_memblk_list)
	{
		uart_puts(
			"memallc: Invalid size or no free memory blocks available.\r\n");
		return NULL;
	}
	memblk_t *prev = NULL;
	memblk_t *block = free_memblk_list;
	uart_puts("memallc: Searching for a free block.\r\n");
	return allc_memblk_impl(size, prev, block);
}

static memblk_t *allc_memblk_impl(uint64_t size, memblk_t *prev,
				  memblk_t *block)
{
	while (block)
	{
		if (block->size >= size)
		{
			uart_puts("memallc: Found suitable block.\r\n");
			return allc_memblk_impl_split(size, prev, block);
		}
		prev = block;
		block = block->next;
	}
	uart_puts("memallc: No suitable block found.\r\n");
	return NULL;
}

static memblk_t *allc_memblk_impl_split(uint64_t size, memblk_t *prev,
					memblk_t *block)
{
	if (block->size > size)
	{
		memblk_t *new_block = new_memblk((char *)block->addr + size,
						 block->size - size);
		if (new_block)
		{
			uart_puts(
				"memallc: Splitting free block and creating new block.\r\n");
			new_block->next = block->next;
			block->next = new_block;
			block->size = size;
		}
	}
	if (prev)
	{
		uart_puts("memallc: Removing block from free list.\r\n");
		prev->next = block->next;
	}
	else
	{
		uart_puts(
			"memallc: Removing block from free list. This block is the head of the free list.\r\n");
		free_memblk_list = block->next;
	}
	block->next = allc_memblk_list;
	allc_memblk_list = block;
	uart_puts("memallc: Block allocated and added to allocated list.\r\n");
	return block;
}

static void defragment_memblk(memblk_t *block)
{
	memblk_t *current = free_memblk_list;
	memblk_t *prev_current = NULL;
	while (current)
	{
		if ((char *)current->addr + current->size == block->addr)
		{
			uart_puts("memallc: Merging adjacent blocks.\r\n");
			current->size += block->size;
			current->next = block->next;
			uart_puts("memallc: Free list defragmented.\r\n");
			return;
		}
		else if ((char *)block->addr + block->size == current->addr)
		{
			uart_puts("memallc: Merging adjacent blocks.\r\n");
			block->size += current->size;
			block->next = current->next;
			if (current == free_memblk_list)
			{
				uart_puts(
					"memallc: Current block is head of free list. Updating head.\r\n");
				free_memblk_list = block;
			}
			else if (prev_current)
			{
				uart_puts(
					"memallc: Updating previous block to point to merged block.\r\n");
				prev_current->next = block;
			}
			uart_puts("memallc: Free list defragmented.\r\n");
			return;
		}
		prev_current = current;
		current = current->next;
	}
	block->next = free_memblk_list;
	free_memblk_list = block;
	uart_puts("memallc: No adjacent blocks found for defragmentation.\r\n");
}

memblk_t *new_memblk(void *addr, uint64_t size)
{
	if (memblk_pool_index >= MAX_MEMBLK_COUNT)
	{
		uart_puts("memallc: Memory block pool exhausted.\r\n");
		return NULL;
	}
	memblk_t *block = &memblk_pool[memblk_pool_index];
	memblk_pool_index++;
	block->addr = addr;
	block->size = size;
	block->next = NULL;
	uart_puts("memallc: Created new memory block.\r\n");
	return block;
}

memblk_t *get_allc_memblk_by_addr(void *addr)
{
	memblk_t *current = get_allc_memblk();
	while (current)
	{
		if (current->addr == addr)
		{
			uart_puts("memallc: Found memory block.\r\n");
			return current;
		}
		current = current->next;
	}
	uart_puts("memallc: No allocated memory block found.\r\n");
	return NULL;
}