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

int kernel_init(void *name)
{
#ifdef QEMU
	uart_init(0, id_pl011);
#else
	uart_init(0, id_16550);
#endif
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
	context emptyContext0 =
		(context){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	proc *proc0 = memallc(sizeof(proc));
	proc0->proc_context = emptyContext0;
	proc0->proc_ID = 0;
	proc0->proc_state = RUNNING;
	scheduleProcess(proc0);

	uart_puts("static\r\n");

	schedulerInit_static();
	context emptyContext =
		(context){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	proc proc1 = (proc){ READY, emptyContext, 1 };
	uart_puts("Example process proc1 created.\r\n");
	scheduleProcess_static(proc1);
	uart_puts("Example process proc2 created.\r\n");
	proc proc2 = (proc){ SLEEPING, emptyContext, 2 };
	scheduleProcess_static(proc2);
	procSwitch_static();
	procAwaken_static();
	killProcess_static();

	return 0;
}