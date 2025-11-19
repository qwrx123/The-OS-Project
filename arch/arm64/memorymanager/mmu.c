#include "mmu.h"
#include "socfunctions.h"
#include "kernel/string.h"
#include "kernel/types.h"

void init_l1_table(uint64_t *);

void init_l2_table(uint64_t *l1_table, uint64_t l1_index, uint64_t *l2_table);

void init_l2_block(uint64_t *l2_table, uint64_t l2_index, uint64_t pa_block,
		   uint8_t attrindx, uint64_t ap, uint64_t sh, uint64_t pxn,
		   uint64_t uxn);

#define MAIR_ATTRIDX(attr, idx) ((unsigned long long)(attr) << ((idx) * 8))

//Locations in the MAIR for different memory types
#define MT_NORMAL 0
#define MT_NORMAL_NC 2
#define MT_DEVICE_nGnRnE 3
#define MT_DEVICE_nGnRE 4

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

#define L1_INDEX(va) (((uint64_t)(va) >> L1_SHIFT) & PT_INDEX_MASK)
#define L2_INDEX(va) (((uint64_t)(va) >> L2_SHIFT) & PT_INDEX_MASK)
#define L3_INDEX(va) (((uint64_t)(va) >> L3_SHIFT) & PT_INDEX_MASK)

#define L1_BLOCK_SIZE (1ULL << L1_SHIFT)
#define L2_BLOCK_SIZE (1ULL << L2_SHIFT)

#define PTE_VALID (1ULL << 0)
#define PTE_TABLE (1ULL << 1)
#define PTE_BLOCK (PTE_VALID)
#define PTE_NEXT_TABLE (PTE_VALID | PTE_TABLE)
#define PTE_PAGE (PTE_VALID | PTE_TABLE)

#define PTE_ADDR_MASK (((1ULL << PA_BITS) - 1ULL) & ~((1ULL << 12) - 1ULL))

#define MAKE_DESC_ADDR(pa) ((uint64_t)(pa) & PTE_ADDR_MASK)

#define PTE_ATTRINDX_SHIFT 2
#define PTE_ATTRINDX(x) (((uint64_t)(x) & 0x7ULL) << PTE_ATTRINDX_SHIFT)

#define PTE_AP_SHIFT 6
#define AP_RW_EL1 (0ULL << PTE_AP_SHIFT)
#define AP_RW_EL0 (1ULL << PTE_AP_SHIFT)
#define AP_RO_EL1 (2ULL << PTE_AP_SHIFT)
#define AP_RO_EL0 (3ULL << PTE_AP_SHIFT)

#define PTE_SH_SHIFT 8
#define SH_NONE (0ULL << PTE_SH_SHIFT)
#define SH_OUTER (2ULL << PTE_SH_SHIFT)
#define SH_INNER (3ULL << PTE_SH_SHIFT)

#define PTE_AF (1ULL << 10)
#define PTE_NG (1ULL << 11)

#define PTE_PXN (1ULL << 53)
#define PTE_UXN (1ULL << 54)

#define PAGE_DESC(pa, attrindx, ap, sh, af, ng, pxn, uxn)                \
	(PTE_PAGE | MAKE_DESC_ADDR(pa) | PTE_ATTRINDX(attrindx) | (ap) | \
	 (sh) | (af) | (ng) | (pxn) | (uxn))

#define BLOCK_DESC(pa, attrindx, ap, sh, af, ng, pxn, uxn)                \
	(PTE_BLOCK | MAKE_DESC_ADDR(pa) | PTE_ATTRINDX(attrindx) | (ap) | \
	 (sh) | (af) | (ng) | (pxn) | (uxn))

#define TABLE_DESC(pa) (PTE_NEXT_TABLE | MAKE_DESC_ADDR(pa))

void early_mmu_init()
{
	write_mair_el1(MAIR_EL1_SET);

	write_tcr_el1(TCR_EL1_SET);
}

void init_l1_table(uint64_t *pa)
{
	memset(pa, 0, PT_ENTRIES * sizeof(uint64_t));
}

void init_l2_table(uint64_t *l1_table, uint64_t l1_index, uint64_t *l2_table)
{
	memset(l2_table, 0, PT_ENTRIES * sizeof(uint64_t));
	l1_table[l1_index] = TABLE_DESC(l2_table);
}

void init_l2_block(uint64_t *l2_table, uint64_t l2_index, uint64_t pa_block,
		   uint8_t attrindx, uint64_t ap, uint64_t sh, uint64_t pxn,
		   uint64_t uxn)
{
	l2_table[l2_index] =
		BLOCK_DESC(pa_block, attrindx, ap, sh, PTE_AF, 0, pxn, uxn);
}