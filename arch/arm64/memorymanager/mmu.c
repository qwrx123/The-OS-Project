#include "mmu.h"
#include "socfunctions.h"

#define MAIR_ATTRIDX(attr, idx) ((attr) << ((idx) * 8))

#define MT_NORMAL 0
#define MT_NORMAL_NC 2
#define MT_DEVICE_nGnRnE 3
#define MT_DEVICE_nGnRE 4

#define MAIR_ATTR_DEVICE_nGnRnE 0x00
#define MAIR_ATTR_DEVICE_nGnRE 0x04
#define MAIR_ATTR_NORMAL_NC 0x44
#define MAIR_ATTR_NORMAL 0xff

#define MAIR_EL1_SET											\
	(MAIR_ATTRIDX(MAIR_ATTR_DEVICE_nGnRnE, MT_DEVICE_nGnRnE) |	\
	MAIR_ATTRIDX(MAIR_ATTR_DEVICE_nGnRE, MT_DEVICE_nGnRE) |		\
	MAIR_ATTRIDX(MAIR_ATTR_NORMAL_NC, MT_NORMAL_NC) |			\
	MAIR_ATTRIDX(MAIR_ATTR_NORMAL, MT_NORMAL))

void memory_cpu_setup(void)
{
	write_mair_el1(MAIR_EL1_SET);
}

void early_mmu_init()
{
}