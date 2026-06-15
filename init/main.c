/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "uart.h"
#include "memallc.h"
#include "scheduler.h"
#include "proc.h"
#include "memory.h"
#include "timer.h"

int kernel_init(void *name)
{
#ifdef QEMU
	uart_init(0, id_pl011);
#else
	uart_init(0, id_16550);
#endif
	init_timer();
	enable_timer();
	if (name)
	{
	}
	uart_puts("Hello world\r\n");
	char *test = (char *)memallc(0x1000);
	const char *test_output = "Copy to heap";
	int i;
	for (i = 0; test_output[i] != '\0'; i++)
	{
		test[i] = test_output[i];
	}
	test[i] = '\0';
	uart_puts("memallc: Able to write to heap without crash\r\n");
	uart_puts("memallc: Output written \"");
	uart_puts(test);
	uart_puts("\"\r\n");
	free_memallc(memallc(0x1000));

	schedulerInit();
	uart_puts("Creating example process.\r\n");
	context emptyContext =
		(context){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	proc *proc0 = memallc(sizeof(proc));
	proc0->proc_context = emptyContext;
	proc0->proc_ID = 0;
	proc0->proc_state = READY;
	uart_puts("Example process proc0 created.\r\n");
	scheduleProcess(proc0);
	uart_puts("Creating example process.\r\n");
	proc *proc1 = memallc(sizeof(proc));
	proc1->proc_context = emptyContext;
	proc1->proc_ID = 1;
	proc1->proc_state = SLEEPING;
	uart_puts("Example process proc1 created.\r\n");
	scheduleProcess(proc1);
	procSwitch();
	awakenProcess();
	procSwitch();

	return 0;
}