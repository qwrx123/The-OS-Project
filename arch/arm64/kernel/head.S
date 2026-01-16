/*
 * Copyright (c) [year] [your name]
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
.global _start

_start:
	//set stack
	ldr x0, =_stack_top
    mov sp, x0
	//clear bss (needed for c)
	ldr x0, =__bss_start
	mov x1, #0
	ldr x2, =__bss_end
	sub x2, x2, x0
	bl memset
	bl early_mmu_init
	bl kernel_init
	b .