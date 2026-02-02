/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

.global vector_table
.balign 2048
vector_table:
	b .
.balign 0x80
    b .
.balign 0x80
    b .
.balign 0x80
    b .
.balign 0x80