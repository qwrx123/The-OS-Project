/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "gic.h"
#include "socfunctions_gic.h"
#include <kernel/stdint.h>

#define GICD_CTLR (*(volatile uint32_t *)(GICD_BASE_PHYS + 0x000))
#define GICD_ISENABLER(n) \
	(*(volatile uint32_t *)(GICD_BASE_PHYS + 0x100 + ((n) * 4)))
#define GICD_ICENABLER(n) \
	(*(volatile uint32_t *)(GICD_BASE_PHYS + 0x180 + ((n) * 4)))

#define GICD_IPRIORITYR_BYTE(n) \
	(*(volatile uint8_t *)(GICD_BASE_PHYS + 0x400 + (n)))
#define GICD_ITARGETSR_BYTE(n) \
	(*(volatile uint8_t *)(GICD_BASE_PHYS + 0x800 + (n)))

#define GICD_IROUTER(n) \
	(*(volatile uint64_t *)(GICD_BASE_PHYS + 0x6000 + ((n) * 8)))

#define GICD_IGROUPR(n) \
	(*(volatile uint32_t *)(GICD_BASE_PHYS + 0x080 + ((n) * 4)))

#define GICR_WAKER (*(volatile uint32_t *)(GICR_BASE_PHYS + 0x0014))

static inline void gic_isb()
{
	__asm__ volatile("isb" ::: "memory");
}

#define GICD_IROUTER(n) \
	(*(volatile uint64_t *)(GICD_BASE_PHYS + 0x6000 + ((n) * 8)))

void gic_init()
{
	uint32_t waker_val;

	write_icc_sre_el1(1UL);
	gic_isb();

	write_icc_pmr_el1(0xFFUL);

	// Enable Group 1 interrupts at CPU interface
	write_icc_igrpen1_el1(1UL);
	gic_isb();

	waker_val = GICR_WAKER;
	waker_val &= ~(1u << 1); // clear ProcessorSleep
	GICR_WAKER = waker_val;
	while (GICR_WAKER & (1u << 2))
	{
	}

	// Enable Group 1 Non-secure (bit1) and Group 0 (bit0) in Distributor
	GICD_CTLR = (1u << 1) | (1u << 0);
	gic_isb();
}

static inline uint64_t mpidr_affinity(void)
{
	uint64_t mpidr;
	asm volatile("mrs %0, mpidr_el1" : "=r"(mpidr));
	return mpidr & 0x00FFFFFF00FFFFFFULL;
}

void gic_enable_irq(uint32_t irq)
{
	uint32_t reg = irq / 32;
	uint32_t bit = 1u << (irq % 32);

	/* mark SPI as Group 1 non-secure */
	if (irq >= 32)
	{
		GICD_IGROUPR(irq / 32) |= bit;
		GICD_IROUTER(irq) = mpidr_affinity();
		// Set a sane priority (lower value = higher priority)
		GICD_IPRIORITYR_BYTE(irq) = 0x80;
	}

	GICD_ISENABLER(reg) |= bit;
	gic_isb();
}
void gic_disable_irq(uint32_t irq)
{
	uint32_t reg = (irq / 32);
	uint32_t bit = 1u << (irq % 32);
	GICD_ICENABLER(reg) = bit;
}

uint32_t gic_get_active_irq()
{
	uint64_t iar = read_icc_iar1_el1();
	return (uint32_t)(iar & 0x3FFFFFFF);
}

void gic_eoi(uint32_t irq)
{
	write_icc_eoir1_el1((uint64_t)irq);
}