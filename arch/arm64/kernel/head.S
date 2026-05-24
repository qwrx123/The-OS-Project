/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
.global _start

_start:
    mov x0, #(1 << 31)   // HCR_EL2.RW = 1, EL1 is AArch64
    msr HCR_EL2, x0
    mov x0, #0x3c5       // SPSR_EL2 = EL1h, DAIF masked
    msr SPSR_EL2, x0
    ldr x0, =el1_start
    msr ELR_EL2, x0
    eret

el1_start:

	//set stack
	//msr SPSel, #1
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

	bl gic_init
    ldr x0, =initializedGIC
    bl uart_puts

	msr daifclr, #2

	ldr x0, =aboutToBreak
	bl uart_puts
	brk #0
	
	MOV X0, #0x00300000
	MSR CPACR_EL1, X0
	bl kernel_init
	ldr x0, =initializedKernel
	bl uart_puts
slow_loop:
    wfi
    b slow_loop

.data
	clearBSS: .ascii "ASM: Cleared BSS\r\n\0"
	initializedMMU: .ascii "ASM: initialized Memory Management Unit\r\n\0"
	initializedKernel: .ascii "ASM: initialized Kernel\r\n\0"
	aboutToBreak: .ascii "ASM: About to trigger brk\r\n\0"
	initializedGIC: .ascii "ASM: initialized GIC\r\n\0"