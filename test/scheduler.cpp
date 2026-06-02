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

scheduleNode *readyQueue;
scheduleNode *sleepQueue;
scheduleNode *currentProc;

proc *testProcessRun;
proc *testProcessReady;
proc *testProcessReady2;

proc testProc_s1;
proc testProc_s2;

class Scheduler : public ::testing::Test
{
    protected:
	Scheduler()
	{
	}

	virtual ~Scheduler()
	{
		while (readyQueue->next != readyQueue)
		{
			free(readyQueue->next->process);
			free(dequeueProc(readyQueue->next));
		}

		while (sleepQueue->next != sleepQueue)
		{
			free(sleepQueue->next->process);
			free(dequeueProc(sleepQueue->next));
		}

		free(readyQueue);
		free(sleepQueue);
		free(currentProc->process);
		free(currentProc);
	}

	virtual void SetUp()
	{
		uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);

		scheduleNode *ready = new scheduleNode;
		scheduleNode *sleep = new scheduleNode;
		ready->next = ready;
		ready->prev = ready;
		sleep->next = sleep;
		sleep->prev = sleep;
		readyQueue = ready;
		sleepQueue = sleep;

		context testContext0 = { 1000, 19, 20, 21, 22, 23, 24,
					 25,   26, 27, 28, 29, 30 };
		testProcessRun = new proc;
		testProcessRun->proc_state = RUNNING;
		testProcessRun->proc_context = testContext0;
		testProcessRun->proc_ID = 1;

		scheduleNode *runningNode = new scheduleNode;
		scheduleNodeInit(runningNode, testProcessRun);
		currentProc = runningNode;

		context testContext1 = { 2000, 1, 2, 3,	 4,  5, 6,
					 7,    8, 9, 10, 11, 12 };
		testProcessReady = new proc;
		testProcessReady->proc_state = READY;
		testProcessReady->proc_context = testContext1;
		testProcessReady->proc_ID = 2;

		//start static-specific initiallization
		schedulerInit_static();
		context testContext_s1 = { 1000, 19, 20, 21, 22, 23, 24,
					   25,	 26, 27, 28, 29, 30 };
		testProc_s1 = { READY, testContext_s1, 1 };

		context testContext_s2 = { 2000, 1, 2, 3,  4,  5, 6,
					   7,	 8, 9, 10, 11, 12 };
		testProc_s2 = { READY, testContext_s2, 2 };
	}

	virtual void TearDown()
	{
	}
};

TEST_F(Scheduler, addProcessEmptyQueue)
{
	scheduleNode *node = new scheduleNode;
	scheduleNodeInit(node, testProcessReady);
	procToReady(node);
	ASSERT_EQ(testProcessReady, getNextProcess());
}

TEST_F(Scheduler, addProcessQueueLine)
{
	scheduleNode *node = new scheduleNode;
	scheduleNodeInit(node, testProcessReady);
	procToReady(node);

	context newContext = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	testProcessReady2 = new proc;
	testProcessReady2->proc_state = READY;
	testProcessReady2->proc_context = newContext;

	scheduleNode *node2 = new scheduleNode;
	scheduleNodeInit(node2, testProcessReady2);

	procToReady(node2);
	ASSERT_EQ(testProcessReady, getNextProcess());
	ASSERT_EQ(testProcessReady2, getLastProcess());
}

TEST_F(Scheduler, switchProcess)
{
	scheduleNode *node = new scheduleNode;
	scheduleNodeInit(node, testProcessReady);
	procToReady(node);

	procSwitch();
	ASSERT_EQ(testProcessReady, getRunningProcess());
}

TEST_F(Scheduler, sleepProcess)
{
	testProcessReady->proc_state = SLEEPING;
	scheduleNode *node = new scheduleNode;
	scheduleNodeInit(node, testProcessReady);

	procToSleep(node);
	ASSERT_EQ(testProcessReady, getNextSleepProcess());
}

TEST_F(Scheduler, addProcessEmptyQueue_static)
{
	scheduleProcess_static(testProc_s1);
	ASSERT_EQ(testProc_s1.proc_ID, getLastProcess_s().proc_ID);
}

TEST_F(Scheduler, addProcessQueueLine_static)
{
	scheduleProcess_static(testProc_s1);
	scheduleProcess_static(testProc_s2);
	ASSERT_EQ(testProc_s1.proc_ID, getNextProcess_s().proc_ID);
	ASSERT_EQ(testProc_s2.proc_ID, getLastProcess_s().proc_ID);
}

TEST_F(Scheduler, switchProcess_static)
{
	scheduleProcess_static(testProc_s1);
	scheduleProcess_static(testProc_s2);
	procSwitch_static();
	ASSERT_EQ(testProc_s1.proc_ID, getRunningProcess_s().proc_ID);

	procSwitch_static();
	ASSERT_EQ(testProc_s2.proc_ID, getRunningProcess_s().proc_ID);
	ASSERT_EQ(testProc_s1.proc_ID, getLastProcess_s().proc_ID);
}

TEST_F(Scheduler, sleepProcess_static)
{
	testProc_s1.proc_state = SLEEPING;
	scheduleProcess_static(testProc_s1);
	ASSERT_EQ(testProc_s1.proc_ID, getLastSleepProcess_s().proc_ID);
}