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

class Scheduler : public ::testing::Test
{
    protected:
    sched* testScheduler;
    proc testProcessRun;
    proc testProcessReady;
    proc testProcessReady2;

    Scheduler()
    {
        std::cout << "initiallize test scheduler" << std::endl;
        sched Sched = {0, 0, 0};
        testScheduler = schedulerInit(&Sched);

        context testContext0 = { 1000, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };
        testProcessRun = {RUNNING, testContext0};
        testScheduler->currentProc = &testProcessRun;

        context testContext1 = { 2000, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
        testProcessReady = {READY, testContext1};
        std::cout << "finished initiallization" << std::endl;
    }

    virtual ~Scheduler()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(Scheduler, addProcessEmptyQueue)
{
    testScheduler = scheduleProcess(testScheduler, &testProcessReady);
    ASSERT_EQ(&testProcessReady, getNextProcess(testScheduler));
}

TEST_F(Scheduler, addProcessQueueLine)
{
    testScheduler = scheduleProcess(testScheduler, &testProcessReady);
    context newContext = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    testProcessReady2 = {READY, newContext};
    testScheduler = scheduleProcess(testScheduler, &testProcessReady2);
    std::cout << getNextProcess(testScheduler) << std::endl;
    std::cout << getLastProcess(testScheduler) << std::endl;
    ASSERT_EQ(&testProcessReady, getNextProcess(testScheduler));
    ASSERT_EQ(&testProcessReady2, getLastProcess(testScheduler));
}

TEST_F(Scheduler, switchProcess)
{
    testScheduler = scheduleProcess(testScheduler, &testProcessReady);
    procSwitch(testScheduler);
    ASSERT_EQ(&testProcessReady, testScheduler->currentProc);
}

TEST_F(Scheduler, sleepProcess)
{
    testProcessReady.proc_state = SLEEPING;
    testScheduler = scheduleProcess(testScheduler, &testProcessReady);
    ASSERT_EQ(&testProcessReady, getNextSleepProcess(testScheduler));
}