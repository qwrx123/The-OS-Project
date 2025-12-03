#ifndef MMU_H
#define MMU_H

#define MT_NORMAL 0
#define MT_NORMAL_NC 2
#define MT_DEVICE_nGnRnE 3
#define MT_DEVICE_nGnRE 4

//
#define MAIR_ATTR_DEVICE_nGnRnE 0x00
#define MAIR_ATTR_DEVICE_nGnRE 0x04
#define MAIR_ATTR_NORMAL_NC 0x44
#define MAIR_ATTR_NORMAL 0xff

#define PTE_AP_SHIFT 6
#define AP_RW_EL1 (0ULL << 6)
#define AP_RW_EL0 (1ULL << 6)
#define AP_RO_EL1 (2ULL << 6)
#define AP_RO_EL0 (3ULL << 6)

#define PTE_SH_SHIFT 8
#define SH_NONE (0ULL << 8)
#define SH_OUTER (2ULL << 8)
#define SH_INNER (3ULL << 8)

#define PTE_PXN (1ULL << 53)
#define PTE_UXN (1ULL << 54)

#define PTE_SH_SHIFT 8
/**
 * @brief 
 * 
 */
void early_mmu_init();

#endif