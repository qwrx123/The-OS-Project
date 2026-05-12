/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
.global _start

_start:
	//set stack
	msr SPSel, #1
	ldr x0, =_stack_top
    mov sp, x0
	//clear bss (needed for c)
	ldr x0, =__bss_start
	mov x1, #0
	ldr x2, =__bss_end
	sub x2, x2, x0
	bl memset
	mov x0, 0
	mov x1, 0
	bl uart_init
	ldr x0, =clearBSS
	bl uart_puts
	bl early_mmu_init
	ldr x0, =initializedMMU
	bl uart_puts
	//Initalize the Exception Vector Table
	ldr x0, =vector_table
    msr VBAR_EL1, x0
    isb

	msr daifclr, #2

	ldr x0, =aboutToBreak
	bl uart_puts
	brk #0
	
	bl kernel_init
	ldr x0, =initializedKernel
	bl uart_puts
	b .

.data
	clearBSS: .ascii "ASM: Cleared BSS\r\n\0"
	initializedMMU: .ascii "ASM: initialized Memory Management Unit\r\n\0"
	initializedKernel: .ascii "ASM: initialized Kernel\r\n\0"
	 aboutToBreak: .ascii "ASM: About to trigger brk\r\n\0"