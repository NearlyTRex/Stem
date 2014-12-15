#include "unittest/TestSuite.h"
#include "utilities/FixedIntervalRunLoop.h"

static void runCallback(void * context) {
	*(unsigned int *) context += 1;
}

static double timeValue;

static double getTime() {
	return timeValue;
}

static void testCallbackInterval() {
	FixedIntervalRunLoop * runLoop;
	unsigned int callbackCalls = 0, reportedCallbackCalls;
	
	timeValue = 0.0;
	runLoop = FixedIntervalRunLoop_create(getTime, 1, runCallback, &callbackCalls);
	timeValue = 0.9999;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 0, "Expected 0 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 0, "Expected 0 but got %u", reportedCallbackCalls);
	
	timeValue = 1.0;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 1, "Expected 1 but got %u", reportedCallbackCalls);
	
	timeValue = 4.9999;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 4, "Expected 4 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 3, "Expected 3 but got %u", reportedCallbackCalls);
	
	runLoop->dispose(runLoop);
}

void testPauseResume() {
	FixedIntervalRunLoop * runLoop;
	unsigned int callbackCalls = 0, reportedCallbackCalls;
	
	timeValue = 5.0;
	runLoop = FixedIntervalRunLoop_create(getTime, 2, runCallback, &callbackCalls);
	FixedIntervalRunLoop_pause(runLoop);
	timeValue = 9.0;
	FixedIntervalRunLoop_resume(runLoop);
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 0, "Loop ran %u times while paused (expected 0)", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 0, "Expected 0 but got %u", reportedCallbackCalls);
	
	timeValue = 13.5;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 2, "Expected 2 but got %u", reportedCallbackCalls);
	
	timeValue = 15.5;
	FixedIntervalRunLoop_resume(runLoop);
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 3, "Expected 3 but got %d", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 1, "Expected 1 but got %u", reportedCallbackCalls);
	
	FixedIntervalRunLoop_pause(runLoop);
	timeValue = 17.5;
	FixedIntervalRunLoop_pause(runLoop);
	FixedIntervalRunLoop_resume(runLoop);
	timeValue = 19.5;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 4, "Expected 4 but got %d", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 1, "Expected 1 but got %u", reportedCallbackCalls);
	
	timeValue = 22.0;
	FixedIntervalRunLoop_pause(runLoop);
	FixedIntervalRunLoop_resume(runLoop);
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 5, "Expected 5 but got %d", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 1, "Expected 1 but got %u", reportedCallbackCalls);
	
	runLoop->dispose(runLoop);
}

static void testTolerance() {
	FixedIntervalRunLoop * runLoop;
	unsigned int callbackCalls = 0, reportedCallbackCalls;
	
	timeValue = 0.0;
	runLoop = FixedIntervalRunLoop_create(getTime, 1, runCallback, &callbackCalls);
	FixedIntervalRunLoop_setTolerance(runLoop, 0.25);
	timeValue = 0.7499;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 0, "Expected 0 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 0, "Expected 0 but got %u", reportedCallbackCalls);
	
	timeValue = 0.75;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 1, "Expected 1 but got %u", reportedCallbackCalls);
	
	timeValue = 1.0;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 0, "Expected 0 but got %u", reportedCallbackCalls);
	
	timeValue = 2.25;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 1, "Expected 1 but got %u", reportedCallbackCalls);
	
	timeValue = 2.75;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 3, "Expected 3 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 1, "Expected 1 but got %u", reportedCallbackCalls);
	
	timeValue = 5.25;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 4, "Expected 4 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 1, "Expected 1 but got %u", reportedCallbackCalls);
	
	timeValue = 5.25;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 5, "Expected 5 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 1, "Expected 1 but got %u", reportedCallbackCalls);
	
	timeValue = 7.251;
	reportedCallbackCalls = FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 7, "Expected 7 but got %u", callbackCalls);
	TestCase_assert(reportedCallbackCalls == 2, "Expected 2 but got %u", reportedCallbackCalls);
	
	runLoop->dispose(runLoop);
}

TEST_SUITE(FixedIntervalRunLoopTest,
           testCallbackInterval,
           testPauseResume,
           testTolerance)
