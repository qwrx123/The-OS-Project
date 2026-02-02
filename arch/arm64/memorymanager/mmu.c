/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "mmu.h"
#include "board_devices.h"
#include "socfunctions.h"
#include "kernel/string.h"
#include "kernel/types.h"
#include "kernel/stddef.h"

/**
 * @brief Enables the mmu
 * 
 */
static void enable_mmu();

/**
 * @brief Initialize the l1 translation table
 * 
 * @param pa The Physical memory address that points to the l1 table
 */
static void init_l1_table(uint64_t *pa);

/**
 * @brief Initialize the l2 translation table
 * 
 * @param l1_table The physical memory address that points to the l1_table
 * @param l1_index The index to write the memory address of the l2_table
 * @param l2_table The physical memory address of the l2_table
 */
static void init_l2_table(uint64_t *l1_table, uint64_t l1_index,
			  uint64_t *l2_table);

/**
 * @brief Writes a mapped block of memory to the l2_table with
 * all permissions and attributes
 * 
 * @param l2_table The physical memory address of the l2_table
 * @param l2_index The index of the l2_table to write the descriptor of the block to
 * @param pa_block The physical memory address of the block
 * @param attrindx The attribute index from the MAIR register to what type of memory is being used
 * @param ap The access permissions saying what can read or write to the block
 * @param sh The shareability of the block for interprocess communication
 * @param pxn Privileged Execute Never, prevents privileged execution if set
 * @param uxn Unprivileged Execute Never, prevents unprivileged execution if set
 */
static void init_l2_block(uint64_t *l2_table, uint64_t l2_index,
			  phys_addr_t pa_block, uint8_t attrindx, uint64_t ap,
			  uint64_t sh, uint64_t pxn, uint64_t uxn);

/**
 * @brief Maps the whole kernel before the MMU is fully initalized
 * 
 * @param page_table_start The physical memory address of the block of memory that page tables can be written to
 * @param page_table_end The physical memory address of the block of memory end that page tables can be written to
 * @return uint64_t* The physical memory address of the start of the block unused by this funtion
 */
static uint64_t *map_kernel(uint64_t *page_table_start,
			    uint64_t *page_table_end);
/**
 * @brief Maps all devices in the kenrel before the MMU is fully initalized
 * 
 * @param page_table_start The physical memory address of the block of memory that page tables can be written to
 * @param page_table_end The physical memory address of the block of memory end that page tables can be written to
 * @return uint64_t* The physical memory address of the start of the block unused by this funtion
 */
static uint64_t *map_devices(uint64_t *page_table_start,
			     uint64_t *page_table_end);

/**
 * @brief Translates a physical memory address to a virtual memory address
 * 
 * @param pa The physical memory address
 * @return void* The virtual memory address after translation
 */
static inline void *phys_to_virt(phys_addr_t pa);

/**
 * @brief Translates a virtual memory address to a physical one
 * 
 * @param va The virtual memory address
 * @return phys_addr_t The translated physical memory addresss
 */
static inline phys_addr_t virt_to_phys(const void *va);

/**
 * @brief Translation Control Register (TCR) configuration parameters
 * 
 * @param TCR_T0SZ The size offset of the memory region addressed
 * 
 * @param TCR_TG0_4K Granule size
 * 
 * @param TCR_SH0_INNER Shareability attribute
 * 
 * @param TCR_ORGN0_WBWA Outer cacheability
 * 
 * @param TCR_IRGN0_WBWA Inner cacheability
 * 
 * @param TCR_IPS_40BIT Intermediate Physical Address size
 */
#define TCR_T0SZ (25ULL)
#define TCR_TG0_4K (0ULL << 14)
#define TCR_SH0_INNER (3ULL << 12)
#define TCR_ORGN0_WBWA (1ULL << 10)
#define TCR_IRGN0_WBWA (1ULL << 8)
#define TCR_IPS_40BIT (2ULL << 32)

/**
 * @brief Helper macro to set the correct bits of the TCR_EL1 register
 */
#define TCR_EL1_SET                                               \
	(TCR_T0SZ | TCR_TG0_4K | TCR_SH0_INNER | TCR_ORGN0_WBWA | \
	 TCR_IRGN0_WBWA | TCR_IPS_40BIT)

/**
 * @brief Helper macro to help set the MAIR register
 * 
 * @param attr the attribute to set
 * @param idx the index into the mair register to set
 */
#define MAIR_ATTRIDX(attr, idx) ((unsigned long long)(attr) << ((idx) * 8))

/**
 * @brief Helper macro to set the correct bits of the MAIR_EL1 register
 */
#define MAIR_EL1_SET                                               \
	(MAIR_ATTRIDX(MAIR_ATTR_DEVICE_nGnRnE, MT_DEVICE_nGnRnE) | \
	 MAIR_ATTRIDX(MAIR_ATTR_DEVICE_nGnRE, MT_DEVICE_nGnRE) |   \
	 MAIR_ATTRIDX(MAIR_ATTR_NORMAL_NC, MT_NORMAL_NC) |         \
	 MAIR_ATTRIDX(MAIR_ATTR_NORMAL, MT_NORMAL))

/**
 * @brief The virtual address space available to the kernel
 */
#define VA_BITS 39ULL
/**
 * @brief The physical address space the hardware supports
 */
#define PA_BITS 40ULL

/**
 * @brief Helper parameters describing the page (4KiB)
 * 
 * @param PAGE_SHIFT Number of bits to shift to calculate page size
 * 
 * @param PAGE_SIZE The size of a single page in bytes (4096 bytes)
 * 
 * @param PAGE_OFFSET_MASK Bitmask for extracting the offset within a page
 */
#define PAGE_SHIFT 12ULL
#define PAGE_SIZE (1ULL << PAGE_SHIFT)
#define PAGE_OFFSET_MASK (PAGE_SIZE - 1ULL)

/**
 * @brief Helper paramaters describing how much each translation page
 * needs to be shifted to get to the real value
 * 
 * @param L1_SHIFT Number of bits to shift for Level 1 page table translation (1GB granularity)
 * 
 * @param L2_SHIFT Number of bits to shift for Level 2 page table translation (2MB granularity)
 * 
 * @param L3_SHIFT Number of bits to shift for Level 3 page table translation (4KB granularity)
 */
#define L1_SHIFT 30ULL
#define L2_SHIFT 21ULL
#define L3_SHIFT 12ULL

/**
 * @brief Helper macros to describe how to index into a translation table
 * 
 * @param PT_INDEX_BITS Number of bits used to index into a page table level
 * 
 * @param PT_ENTRIES Total number of entries in a single page table level
 * 
 * @param PT_INDEX_MASK Bitmask to extract the relevant index bits from a virtual address
 */
#define PT_INDEX_BITS 9ULL
#define PT_ENTRIES (1ULL << PT_INDEX_BITS)
#define PT_INDEX_MASK (PT_ENTRIES - 1ULL)

/**
 * @brief Returns the number to index into a L1, L2, or L3 page table
 */
#define L1_INDEX(va) (((uintptr_t)(va) >> L1_SHIFT) & PT_INDEX_MASK)
#define L2_INDEX(va) (((uintptr_t)(va) >> L2_SHIFT) & PT_INDEX_MASK)
#define L3_INDEX(va) (((uintptr_t)(va) >> L3_SHIFT) & PT_INDEX_MASK)

/**
 * @brief How many bytes a L1 or L2 block hold
 */
#define L1_BLOCK_SIZE (1ULL << L1_SHIFT)
#define L2_BLOCK_SIZE (1ULL << L2_SHIFT)

/**
 * @brief Paramaters describing a page table
 */
#define PTE_VALID (1ULL << 0)
#define PTE_TABLE (1ULL << 1)
#define PTE_BLOCK (PTE_VALID)
#define PTE_NEXT_TABLE (PTE_VALID | PTE_TABLE)
#define PTE_PAGE (PTE_VALID)

/**
 * @brief Returns the physical addres mask to grab the address from a block or page
 */
#define PTE_ADDR_MASK_PA (((1ULL << PA_BITS) - 1ULL))
/**
 * @brief Grabs the physical address of a page or block
 */
#define MAKE_DESC_ADDR_PAGE(pa) \
	((uintptr_t)(pa) & (PTE_ADDR_MASK_PA & ~((1ULL << 12) - 1ULL)))
#define MAKE_DESC_ADDR_L2(pa) \
	((uintptr_t)(pa) & (PTE_ADDR_MASK_PA & ~((1ULL << 21) - 1ULL)))
#define MAKE_DESC_ADDR_L1(pa) \
	((uintptr_t)(pa) & (PTE_ADDR_MASK_PA & ~((1ULL << 30) - 1ULL)))

#define TABLE_ADDR_MASK MAKE_DESC_ADDR_PAGE(~0ULL)

/**
 * @brief Helper macros to help get the Page table Attribute index
 */
#define PTE_ATTRINDX_SHIFT 2
#define PTE_ATTRINDX(x) (((uint64_t)(x) & 0x7ULL) << PTE_ATTRINDX_SHIFT)

/**
 * @brief Helper macros to set the Proper flags in a page table
 */
#define PTE_AF (1ULL << 10)
#define PTE_NG (1ULL << 11)

/**
 * @brief Values to set to the System Control Register (SCTLR)
 * which helps enable the MMU
 */
#define SCTLR_M (1ULL << 0)
#define SCTLR_C (1ULL << 2)
#define SCTLR_I (1ULL << 12)
#define SCTLR_E0E (1ULL << 24)
#define SCTLR_EE (1ULL << 25)

/**
 * @brief Bits to SET to 1 and CLEAR to 0
 */
#define SCTLR_EL1_SET (SCTLR_M | SCTLR_C | SCTLR_I)
#define SCTLR_EL1_CLEAR (SCTLR_EE | SCTLR_E0E)

/**
 * @brief Generates a page description to map a page of memory
 * 
 * @param pa The physical address of the page to map
 * @param attrindx The attribute index from the MAIR register to what type of memory is being used
 * @param ap The access permissions saying what can read or write to the block
 * @param sh The shareability of the block for interprocess communication
 * @param af The Access Flag which is used to tell if a page has been accessed
 * @param ng The flag that tells if a page is global or not
 * @param pxn Privileged Execute Never, prevents privileged execution if set
 * @param uxn Unprivileged Execute Never, prevents unprivileged execution if set
 */
#define PAGE_DESC(pa, attrindx, ap, sh, af, ng, pxn, uxn)                     \
	(PTE_PAGE | MAKE_DESC_ADDR_PAGE(pa) | PTE_ATTRINDX(attrindx) | (ap) | \
	 (sh) | (af) | (ng) | (pxn) | (uxn))

/**
 * @brief Generates a block description to map a block to memory
 * 
 * @param pa The physical address of the block to map
 * @param attrindx The attribute index from the MAIR register to what type of memory is being used
 * @param ap The access permissions saying what can read or write to the block
 * @param sh The shareability of the block for interprocess communication
 * @param af The Access Flag which is used to tell if a block has been accessed
 * @param ng The flag that tells if a block is global or not
 * @param pxn Privileged Execute Never, prevents privileged execution if set
 * @param uxn Unprivileged Execute Never, prevents unprivileged execution if set
 */
#define BLOCK_DESC(pa, attrindx, ap, sh, af, ng, pxn, uxn)                   \
	(PTE_BLOCK | MAKE_DESC_ADDR_L2(pa) | PTE_ATTRINDX(attrindx) | (ap) | \
	 (sh) | (af) | (ng) | (pxn) | (uxn))

/**
 * @brief Creates a description for the next table
 * 
 * @param pa the physical address of the next table
 */
#define TABLE_DESC(pa) (PTE_NEXT_TABLE | MAKE_DESC_ADDR_PAGE(pa))

/**
 * @brief The physical address of the start of the page tables
 */
extern uint8_t __page_tables_start[];
/**
 * @brief The physical address of the end of the page tables
 */
extern uint8_t __page_tables_end[];
/**
 * @brief The physical address of the start of the kernel memory
 */
extern uint8_t __kernel_start[];
/**
 * @brief The physical address of the end of the kernel memory
 */
extern uint8_t __kernel_end[];

void early_mmu_init()
{
	write_mair_el1(MAIR_EL1_SET);
	write_tcr_el1(TCR_EL1_SET);
	mmu_isb();

	uint64_t *page_tables_start = (uint64_t *)__page_tables_start;
	uint64_t *page_tables_end = (uint64_t *)__page_tables_end;

	uint64_t *next_map = map_kernel(page_tables_start, page_tables_end);

	map_devices(next_map, page_tables_end);

	write_ttbr0_el1((uintptr_t)page_tables_start);

	mmu_dsb_ish();
	mmu_isb();
	enable_mmu();
}

static void init_l1_table(uint64_t *pa)
{
	memset(pa, 0, PT_ENTRIES * sizeof(uint64_t));
}

static void init_l2_table(uint64_t *l1_table, uint64_t l1_index,
			  uint64_t *l2_table)
{
	memset(l2_table, 0, PT_ENTRIES * sizeof(uint64_t));
	l1_table[l1_index] = TABLE_DESC(l2_table);
}

static void init_l2_block(uint64_t *l2_table, uint64_t l2_index,
			  phys_addr_t pa_block, uint8_t attrindx, uint64_t ap,
			  uint64_t sh, uint64_t pxn, uint64_t uxn)
{
	l2_table[l2_index] =
		BLOCK_DESC(pa_block, attrindx, ap, sh, PTE_AF, 0, pxn, uxn);
}

static inline void *phys_to_virt(phys_addr_t pa)
{
	return (void *)pa;
}

static inline phys_addr_t virt_to_phys(const void *va)
{
	return (phys_addr_t)va;
}

//Currently only supports <= 1GB of ram
static uint64_t *map_kernel(uint64_t *page_table_start,
			    uint64_t *page_table_end)
{
	size_t kernel_size = __kernel_end - __kernel_start;

	size_t block_num = (kernel_size + (L2_BLOCK_SIZE - 1)) >> L2_SHIFT;

	uint64_t *l1_table = page_table_start;
	uint64_t *l2_table = page_table_start + PT_ENTRIES;

	init_l1_table(l1_table);

	phys_addr_t kernel_block_pa = (phys_addr_t)__kernel_start;
	uintptr_t kernel_block_va = (uintptr_t)phys_to_virt(kernel_block_pa);

	uint64_t l1_index = L1_INDEX(kernel_block_va);
	init_l2_table(l1_table, l1_index, l2_table);

	for (size_t i = 0; i < block_num; i++)
	{
		init_l2_block(l2_table, L2_INDEX(kernel_block_va),
			      kernel_block_pa, MT_NORMAL, AP_RW_EL1, SH_INNER,
			      0, PTE_UXN);
		kernel_block_va += L2_BLOCK_SIZE;
		kernel_block_pa += L2_BLOCK_SIZE;
	}

	return l2_table + PT_ENTRIES;
}

static uint64_t *map_devices(uint64_t *page_table_start,
			     uint64_t *page_table_end)
{
	uint64_t *l1_table = (uint64_t *)__page_tables_start;

	for (int i = 0; i < early_device_map_count; i++)
	{
		uint64_t *l2_table;
		const device_config_t *current_device = &early_device_map[i];
		phys_addr_t device_address_pa = current_device->base_addr;
		uintptr_t device_address_va =
			(uintptr_t)phys_to_virt(device_address_pa);

		uint64_t l1_index = L1_INDEX(device_address_va);

		if (!(l1_table[l1_index] & PTE_VALID))
		{
			l2_table = page_table_start;
			page_table_start += PT_ENTRIES;
			init_l2_table(l1_table, l1_index, l2_table);
		}
		else
		{
			l2_table = (uint64_t *)(l1_table[l1_index] &
						TABLE_ADDR_MASK);
		}

		init_l2_block(l2_table, L2_INDEX(device_address_va),
			      device_address_pa, current_device->mem_type,
			      current_device->access_perm,
			      current_device->shareability, current_device->pxn,
			      current_device->uxn);
	}
	return page_table_start;
}

static void enable_mmu()
{
	uint64_t sctlr = read_sctlr_el1();
	sctlr &= ~SCTLR_EL1_CLEAR;
	sctlr |= SCTLR_EL1_SET;
	write_sctlr_el1(sctlr);
	mmu_isb();
}