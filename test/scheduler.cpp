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
        context testContext = {1000, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
        proc testProc = {RUNNING, testContext};
        testScheduler->currentProc = testProc;

        context testContext2 = {2000, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        testProcessNode->process = {READY, testContext2};
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

TEST_F(Scheduler, addProcess)
{
    testScheduler.procToReady(testProcessNode);
    ASSERT_EQ(testProcessNode, testScheduler->readyQueue->next);
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
    ASSERT_EQ(testProcessNode, testScheduler->sleepQueue->next);
}