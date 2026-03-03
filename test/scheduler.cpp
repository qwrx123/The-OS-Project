/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>

extern "C"
{
    #include "scheduler.h"
}

class Scheduler : public ::testing::Test
{
    protected:
    sched* testScheduler;
    scheduleNode* testProcessNode;

    Scheduler()
    {
        schedulerInit(testScheduler);
        context testContext = {SP = 1000, x19 = 19, x20 = 20, x21 = 21, x22 = 22, x23 = 23, x24 = 24, x25 = 25, x26 = 26, x27 = 27, x28 = 28, x29 = 29, x30 = 30};
        proc testProc = {proc_state = state.RUNNING, proc_context = testContext};
        testScheduler->currentProc = testProc;

        context testContext2 = {SP = 2000, x19 = 1, x20 = 2, x21 = 3, x22 = 4, x23 = 5, x24 = 6, x25 = 7, x26 = 8, x27 = 9, x28 = 10, x29 = 11, x30 = 12};
        testProcessNode->process = {proc_state = state.READY, proc_context = testContext2};
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
}

TEST_F(Scheduler, addProcess)
{
    testScheduler.procToReady(testProcessNode);
    ASSERT_EQ(testProcessNode, testScheduler->ReadyQueue->next);
}

TEST_F(Scheduler, switchProcess)
{
    proc expectedProcess = testProcessNode->process;
    testScheduler.procToReady(testProcessNode);
    testScheduler.procSwitch();
    ASSERT_EQ(expectedProcess, testScheduler->currentProc);
}

TEST_F(Scheduler, sleepProcess)
{
    testProcessNode->process->proc_state = state.SLEEPING;
    testScheduler.procToSleep(testProcessNode);
    ASSERT_EQ(testProcessNode, testScheduler->SleepQueue->next);
}