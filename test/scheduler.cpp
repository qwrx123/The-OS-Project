/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>

extern "C"
{
#include "scheduler.h"
#include "uart.h"
#include "memallc.h"
}

typedef struct
{
	volatile unsigned int DR;
	volatile unsigned int RSR_ECR;
	volatile unsigned int reserved1[4];
	volatile unsigned int FR;
	volatile unsigned int reserved2;
	volatile unsigned int ILPR;
	volatile unsigned int IBRD;
	volatile unsigned int FBRD;
	volatile unsigned int LCR_H;
	volatile unsigned int CR;
	volatile unsigned int IFLS;
	volatile unsigned int IMSC;
	volatile unsigned int RIS;
	volatile unsigned int MIS;
	volatile unsigned int ICR;
	volatile unsigned int DMACR;
	volatile unsigned int reserved3[997];
	volatile unsigned int PeriphID0;
	volatile unsigned int PeriphID1;
	volatile unsigned int PeriphID2;
	volatile unsigned int PeriphID3;
	volatile unsigned int PCellID0;
	volatile unsigned int PCellID1;
	volatile unsigned int PCellID2;
	volatile unsigned int PCellID3;
} uart_mock_regs_t;

static uart_mock_regs_t UARTMOCK = { 0 };
#define UARTDEFAULTRESET 100;
static int UARTRESETCYCLES = 0;
static int UARTFRREAD = 0;
static std::string console;
extern void (*uart_fr_callback)();
extern void (*uart_dr_callback)();
extern void (*uart_lsr_callback)();
extern void (*uart_tbr_callback)();

static sched *testScheduler;
proc *testProcessRun;
proc *testProcessReady;
proc *testProcessReady2;

class Scheduler : public ::testing::Test
{
    protected:
	Scheduler()
	{
	}

	virtual ~Scheduler()
	{
		killScheduler(testScheduler);
	}

	virtual void SetUp()
	{
		uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
		init_memallc((void *)0x1000, 0x200000);

		std::cout << "initiallize test scheduler" << std::endl;
		std::cout << sizeof(sched) << std::endl;
		testScheduler = new sched;
		std::cout << "test" << std::endl;
		*testScheduler = (sched){ 0, 0, 0 };
		std::cout << "test2" << std::endl;

		scheduleNode *ready = new scheduleNode;
		scheduleNode *sleep = new scheduleNode;
		ready->next = ready;
		ready->prev = ready;
		sleep->next = sleep;
		sleep->prev = sleep;
		testScheduler->readyQueue = ready;
		testScheduler->sleepQueue = sleep;

		std::cout << "got past schedulerInit" << std::endl;

		context testContext0 = { 1000, 19, 20, 21, 22, 23, 24,
					 25,   26, 27, 28, 29, 30 };
		testProcessRun = new proc;
		testProcessRun->proc_state = RUNNING;
		testProcessRun->proc_context = testContext0;

		scheduleNode *runningNode = new scheduleNode;
		scheduleNodeInit(runningNode, testProcessRun);
		testScheduler->currentProc = runningNode;

		context testContext1 = { 2000, 1, 2, 3,	 4,  5, 6,
					 7,    8, 9, 10, 11, 12 };
		testProcessReady = new proc;
		testProcessReady->proc_state = READY;
		testProcessReady->proc_context = testContext1;
		std::cout << "finished initiallization" << std::endl;
	}

	virtual void TearDown()
	{
	}
};

TEST_F(Scheduler, addProcessEmptyQueue)
{
	scheduleNode *node = new scheduleNode;
	scheduleNodeInit(node, testProcessReady);
	procToReady(testScheduler, node);
	ASSERT_EQ(testProcessReady, getNextProcess(testScheduler));
}

TEST_F(Scheduler, addProcessQueueLine)
{
	scheduleNode *node = new scheduleNode;
	scheduleNodeInit(node, testProcessReady);
	procToReady(testScheduler, node);

	context newContext = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	testProcessReady2 = new proc;
	testProcessReady2->proc_state = READY;
	testProcessReady2->proc_context = newContext;

	scheduleNode *node2 = new scheduleNode;
	scheduleNodeInit(node2, testProcessReady2);

	procToReady(testScheduler, node2);
	std::cout << getNextProcess(testScheduler) << std::endl;
	std::cout << getLastProcess(testScheduler) << std::endl;
	ASSERT_EQ(testProcessReady, getNextProcess(testScheduler));
	ASSERT_EQ(testProcessReady2, getLastProcess(testScheduler));
}

TEST_F(Scheduler, switchProcess)
{
	scheduleNode *node = new scheduleNode;
	scheduleNodeInit(node, testProcessReady);
	procToReady(testScheduler, node);

	procSwitch(testScheduler);
	ASSERT_EQ(testProcessReady, getRunningProcess(testScheduler));
}

TEST_F(Scheduler, sleepProcess)
{
	testProcessReady->proc_state = SLEEPING;
	scheduleNode *node = new scheduleNode;
	scheduleNodeInit(node, testProcessReady);

	procToSleep(testScheduler, node);
	ASSERT_EQ(testProcessReady, getNextSleepProcess(testScheduler));
}