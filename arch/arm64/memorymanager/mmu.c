#include "mmu.h"
#include "board_devices.h"
#include "socfunctions.h"
#include "kernel/string.h"
#include "kernel/types.h"
#include "kernel/stddef.h"

/**
 * @brief 
 * 
 * @param pa 
 */
static void init_l1_table(uint64_t *pa);

/**
 * @brief 
 * 
 * @param l1_table 
 * @param l1_index 
 * @param l2_table 
 */
static void init_l2_table(uint64_t *l1_table, uint64_t l1_index,
			  uint64_t *l2_table);

/**
 * @brief 
 * 
 * @param l2_table 
 * @param l2_index 
 * @param pa_block 
 * @param attrindx 
 * @param ap 
 * @param sh 
 * @param pxn 
 * @param uxn 
 */
static void init_l2_block(uint64_t *l2_table, uint64_t l2_index,
			  phys_addr_t pa_block, uint8_t attrindx, uint64_t ap,
			  uint64_t sh, uint64_t pxn, uint64_t uxn);

/**
 * @brief 
 * 
 * @param page_table_start 
 * @param page_table_end 
 */
static uint64_t *map_kernel(uint64_t *page_table_start,
			    uint64_t *page_table_end);
static uint64_t *map_devices(uint64_t *page_table_start,
			     uint64_t *page_table_end);

static inline void *phys_to_virt(phys_addr_t pa);

static inline phys_addr_t virt_to_phys(const void *va);

#define MAIR_ATTRIDX(attr, idx) ((unsigned long long)(attr) << ((idx) * 8))

//
#define MAIR_ATTR_DEVICE_nGnRnE 0x00
#define MAIR_ATTR_DEVICE_nGnRE 0x04
#define MAIR_ATTR_NORMAL_NC 0x44
#define MAIR_ATTR_NORMAL 0xff

#define TCR_T0SZ (25ULL)
#define TCR_TG0_4K (0ULL << 14)
#define TCR_SH0_INNER (3ULL << 12)
#define TCR_ORGN0_WBWA (1ULL << 10)
#define TCR_IRGN0_WBWA (1ULL << 8)
#define TCR_IPS_40BIT (2ULL << 32)

#define TCR_EL1_SET                                               \
	(TCR_T0SZ | TCR_TG0_4K | TCR_SH0_INNER | TCR_ORGN0_WBWA | \
	 TCR_IRGN0_WBWA | TCR_IPS_40BIT)

#define MAIR_EL1_SET                                               \
	(MAIR_ATTRIDX(MAIR_ATTR_DEVICE_nGnRnE, MT_DEVICE_nGnRnE) | \
	 MAIR_ATTRIDX(MAIR_ATTR_DEVICE_nGnRE, MT_DEVICE_nGnRE) |   \
	 MAIR_ATTRIDX(MAIR_ATTR_NORMAL_NC, MT_NORMAL_NC) |         \
	 MAIR_ATTRIDX(MAIR_ATTR_NORMAL, MT_NORMAL))

#define VA_BITS 39ULL
#define PA_BITS 40ULL

#define PAGE_SHIFT 12ULL
#define PAGE_SIZE (1ULL << PAGE_SHIFT)
#define PAGE_OFFSET_MASK (PAGE_SIZE - 1ULL)

#define L1_SHIFT 30ULL
#define L2_SHIFT 21ULL
#define L3_SHIFT 12ULL

#define PT_INDEX_BITS 9ULL
#define PT_ENTRIES (1ULL << PT_INDEX_BITS)
#define PT_INDEX_MASK (PT_ENTRIES - 1ULL)

#define L1_INDEX(va) (((uintptr_t)(va) >> L1_SHIFT) & PT_INDEX_MASK)
#define L2_INDEX(va) (((uintptr_t)(va) >> L2_SHIFT) & PT_INDEX_MASK)
#define L3_INDEX(va) (((uintptr_t)(va) >> L3_SHIFT) & PT_INDEX_MASK)

#define L1_BLOCK_SIZE (1ULL << L1_SHIFT)
#define L2_BLOCK_SIZE (1ULL << L2_SHIFT)

#define PTE_VALID (1ULL << 0)
#define PTE_TABLE (1ULL << 1)
#define PTE_BLOCK (PTE_VALID)
#define PTE_NEXT_TABLE (PTE_VALID | PTE_TABLE)
#define PTE_PAGE (PTE_VALID | PTE_TABLE)

#define PTE_ADDR_MASK (((1ULL << PA_BITS) - 1ULL) & ~((1ULL << 12) - 1ULL))

#define MAKE_DESC_ADDR(pa) ((uintptr_t)(pa) & PTE_ADDR_MASK)

#define PTE_ATTRINDX_SHIFT 2
#define PTE_ATTRINDX(x) (((uint64_t)(x) & 0x7ULL) << PTE_ATTRINDX_SHIFT)

#define PTE_AF (1ULL << 10)
#define PTE_NG (1ULL << 11)

#define SCTLR_M (1ULL << 0)
#define SCTLR_C (1ULL << 2)
#define SCTLR_I (1ULL << 12)
#define SCTLR_E0E (1ULL << 24)
#define SCTLR_EE (1ULL << 25)

#define SCTLR_EL1_SET (SCTLR_M | SCTLR_C | SCTLR_I)
#define SCTLR_EL1_CLEAR (SCTLR_EE | SCTLR_E0E)

/**
 * @brief 
 * 
 * @param pa
 * @param attrindx
 * @param ap
 * @param sh
 * @param af
 * @param ng
 * @param pxn
 * @param uxn
 */
#define PAGE_DESC(pa, attrindx, ap, sh, af, ng, pxn, uxn)                \
	(PTE_PAGE | MAKE_DESC_ADDR(pa) | PTE_ATTRINDX(attrindx) | (ap) | \
	 (sh) | (af) | (ng) | (pxn) | (uxn))

/**
 * @brief 
 * 
 * @param pa
 * @param attrindx
 * @param ap
 * @param sh
 * @param af
 * @param ng
 * @param pxnextern uint64_t __page_tables_size;
 * @param uxn
 */
#define BLOCK_DESC(pa, attrindx, ap, sh, af, ng, pxn, uxn)                \
	(PTE_BLOCK | MAKE_DESC_ADDR(pa) | PTE_ATTRINDX(attrindx) | (ap) | \
	 (sh) | (af) | (ng) | (pxn) | (uxn))

/**
 * @brief 
 * 
 * @param pa
 * @param attrindx
 * @param ap
 * @param sh
 * @param af
 * @param ng
 * @param pxn
 * @param uxn
 */
#define TABLE_DESC(pa) (PTE_NEXT_TABLE | MAKE_DESC_ADDR(pa))

extern uint8_t __page_tables_start[];
extern uint8_t __page_tables_end[];
extern uint8_t __kernel_start[];
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

	uint64_t sctlr = read_sctlr_el1();

	sctlr &= ~SCTLR_EL1_CLEAR;
	sctlr |= SCTLR_EL1_SET;

	write_sctlr_el1(sctlr);
	mmu_isb();
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
						PTE_ADDR_MASK);
		}

		init_l2_block(l2_table, L2_INDEX(device_address_va),
			      device_address_pa, current_device->mem_type,
			      current_device->access_perm,
			      current_device->shareability, current_device->pxn,
			      current_device->uxn);
	}
	return page_table_start;
}