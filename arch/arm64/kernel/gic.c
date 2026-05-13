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

#define GICR_WAKER (*(volatile uint32_t *)(GICR_BASE_PHYS + 0x0014))

static inline void gic_isb()
{
	__asm__ volatile("isb" ::: "memory");
}

void gic_init()
{
	uint32_t waker_val;

	write_icc_sre_el1(1UL);
	gic_isb();

	write_icc_pmr_el1(0xFFUL);

	write_icc_igrpen1_el1(1UL);
	gic_isb();

	waker_val = GICR_WAKER;
	waker_val &= ~(1u << 1);
	GICR_WAKER = waker_val;
	while (GICR_WAKER & (1u << 2))
	{
	}

	GICD_CTLR = 1;
	gic_isb();
}

void gic_enable_irq(uint32_t irq)
{
	uint32_t reg = (irq / 32);
	uint32_t bit = 1u << (irq % 32);
	GICD_ISENABLER(reg) = bit;
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