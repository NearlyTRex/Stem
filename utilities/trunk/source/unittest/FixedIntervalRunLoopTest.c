#include "unittest/TestSuite.h"
#include "utilities/FixedIntervalRunLoop.h"
#include <string.h>

static void runCallback(void * context) {
	*(unsigned int *) context += 1;
}

static double timeValue;

static double getTime() {
	return timeValue;
}

static void verifyInit(FixedIntervalRunLoop * runLoop, double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext) {
	TestCase_assert(runLoop->dispose == FixedIntervalRunLoop_dispose, "Expected %p but got %p", FixedIntervalRunLoop_dispose, runLoop->dispose);
	TestCase_assert(runLoop->timeFunction == timeFunction, "Expected %p but got %p", timeFunction, runLoop->timeFunction);
	TestCase_assert(runLoop->stepInterval == stepInterval, "Expected %f but got %f", stepInterval, runLoop->stepInterval);
	TestCase_assert(runLoop->stepCallback == stepCallback, "Expected %p but got %p", stepCallback, runLoop->stepCallback);
	TestCase_assert(runLoop->stepContext == stepContext, "Expected %p but got %p", stepContext, runLoop->stepContext);
	TestCase_assert(runLoop->timeScale == 1.0, "Expected 1.0 but got %f", runLoop->timeScale);
	TestCase_assert(runLoop->tolerance == 0.0, "Expected 0.0 but got %f", runLoop->tolerance);
	TestCase_assert(runLoop->slop == 0.0, "Expected 0.0 but got %f", runLoop->slop);
	TestCase_assert(!runLoop->paused, "Expected false but got true");
}

static void testInit() {
	FixedIntervalRunLoop runLoop, * runLoopPtr;
	bool success;
	
	memset(&runLoop, 0x00, sizeof(runLoop));
	success = FixedIntervalRunLoop_init(&runLoop, getTime, 1, runCallback, NULL);
	TestCase_assert(success, "Expected true but got false");
	verifyInit(&runLoop, getTime, 1, runCallback, NULL);
	runLoop.dispose(&runLoop);
	
	memset(&runLoop, 0xFF, sizeof(runLoop));
	success = FixedIntervalRunLoop_init(&runLoop, getTime, 1, runCallback, NULL);
	TestCase_assert(success, "Expected true but got false");
	verifyInit(&runLoop, getTime, 1, runCallback, NULL);
	runLoop.dispose(&runLoop);
	
	runLoopPtr = FixedIntervalRunLoop_create(getTime, 1, runCallback, NULL);
	TestCase_assert(runLoopPtr != NULL, "Expected non-NULL but got NULL");
	if (runLoopPtr == NULL) { return; } // Suppress clang warning
	verifyInit(runLoopPtr, getTime, 1, runCallback, NULL);
	runLoopPtr->dispose(runLoopPtr);
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

static void testTimeScale() {
	FixedIntervalRunLoop * runLoop;
	unsigned int callbackCalls = 0;
	
	timeValue = 0.0;
	callbackCalls = 0;
	runLoop = FixedIntervalRunLoop_create(getTime, 1.0, runCallback, &callbackCalls);
	
	FixedIntervalRunLoop_setTimeScale(runLoop, 0.5);
	timeValue = 2.0;
	// Called twice to ensure slop tolerance cannot affect number of callbacks
	FixedIntervalRunLoop_run(runLoop);
	FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %u", callbackCalls);
	
	FixedIntervalRunLoop_setTimeScale(runLoop, 2.0);
	timeValue = 3.0;
	// Called twice to ensure slop tolerance cannot affect number of callbacks
	FixedIntervalRunLoop_run(runLoop);
	FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 3, "Expected 3 but got %u", callbackCalls);
	
	timeValue = 3.25;
	FixedIntervalRunLoop_run(runLoop);
	FixedIntervalRunLoop_setTimeScale(runLoop, 1.0);
	timeValue = 3.74;
	FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 3, "Expected 3 but got %u", callbackCalls);
	timeValue = 3.75;
	FixedIntervalRunLoop_run(runLoop);
	TestCase_assert(callbackCalls == 4, "Expected 4 but got %u", callbackCalls);
	
	FixedIntervalRunLoop_dispose(runLoop);
}

TEST_SUITE(FixedIntervalRunLoopTest,
           testInit,
           testCallbackInterval,
           testPauseResume,
           testTolerance,
           testTimeScale)
