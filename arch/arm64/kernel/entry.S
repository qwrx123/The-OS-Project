/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

.global vector_table
.balign 0x800
vector_table:
    // Current EL, SP_EL0
    b sync_entry
    .balign 0x80
    b irq_entry
    .balign 0x80
    b .
    .balign 0x80
    b .
    .balign 0x80

    // Current EL, SP_ELx
    b sync_entry
    .balign 0x80
    b irq_entry
    .balign 0x80
    b .
    .balign 0x80
    b .
    .balign 0x80

    // Lower EL, AArch64
    b sync_entry
    .balign 0x80
    b irq_entry
    .balign 0x80
    b .
    .balign 0x80
    b .
    .balign 0x80

    // Lower EL, AArch32
    b sync_entry
    .balign 0x80
    b irq_entry
    .balign 0x80
    b .
    .balign 0x80
    b .

sync_entry:
    sub sp, sp, #272
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    stp x16, x17, [sp, #128]
    stp x18, x19, [sp, #144]
    stp x20, x21, [sp, #160]
    stp x22, x23, [sp, #176]
    stp x24, x25, [sp, #192]
    stp x26, x27, [sp, #208]
    stp x28, x29, [sp, #224]
    str x30, [sp, #240]

    mrs x0, ESR_EL1
    str x0, [sp, #248]
    mrs x0, FAR_EL1
    str x0, [sp, #256]
    mrs x0, ELR_EL1
    str x0, [sp, #264]

    mov x0, sp
    bl el1_sync_handler

    ldr x0, [sp, #264]
    msr ELR_EL1, x0

    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #208]
    ldp x28, x29, [sp, #224]
    ldr x30, [sp, #240]
    add sp, sp, #272
    eret

irq_entry:
    sub sp, sp, #272
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    stp x16, x17, [sp, #128]
    stp x18, x19, [sp, #144]
    stp x20, x21, [sp, #160]
    stp x22, x23, [sp, #176]
    stp x24, x25, [sp, #192]
    stp x26, x27, [sp, #208]
    stp x28, x29, [sp, #224]
    str x30, [sp, #240]

    mrs x0, ESR_EL1
    str x0, [sp, #248]
    mrs x0, FAR_EL1
    str x0, [sp, #256]
    mrs x0, ELR_EL1
    str x0, [sp, #264]

    mov x0, sp
    bl el1_irq_handler

    ldr x0, [sp, #264]
    msr ELR_EL1, x0

    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #208]
    ldp x28, x29, [sp, #224]
    ldr x30, [sp, #240]
    add sp, sp, #272
    eret