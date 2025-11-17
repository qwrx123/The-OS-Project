#include "mmu.h"
#include "socfunctions.h"

#define MAIR_ATTRIDX(attr, idx) ((unsigned long long)(attr) << ((idx) * 8))

#define MT_NORMAL 0
#define MT_NORMAL_NC 2
#define MT_DEVICE_nGnRnE 3
#define MT_DEVICE_nGnRE 4

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

void early_mmu_init()
{
	write_mair_el1(MAIR_EL1_SET);

	write_tcr_el1(TCR_EL1_SET);
}