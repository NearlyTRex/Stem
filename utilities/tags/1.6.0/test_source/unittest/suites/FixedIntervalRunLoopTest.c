#include "unittest/framework/TestSuite.h"
#include "utilities/FixedIntervalRunLoop.h"

static void runCallback(void * context) {
	*(int *) context += 1;
}

static double timeValue;

static double getTime() {
	return timeValue;
}

static void testCallbackInterval() {
	FixedIntervalRunLoop * runLoop;
	int callbackCalls = 0;
	
	timeValue = 0.0;
	runLoop = FixedIntervalRunLoop_create(getTime, 1, runCallback, &callbackCalls);
	timeValue = 0.9999;
	runLoop->run(runLoop);
	TestCase_assert(callbackCalls == 0, "Expected 0 but got %d", callbackCalls);
	
	timeValue = 1.0;
	runLoop->run(runLoop);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	
	timeValue = 4.9999;
	runLoop->run(runLoop);
	TestCase_assert(callbackCalls == 4, "Expected 4 but got %d", callbackCalls);
	
	runLoop->dispose(runLoop);
}

void testPauseResume() {
	FixedIntervalRunLoop * runLoop;
	int callbackCalls = 0;
	
	timeValue = 5.0;
	runLoop = FixedIntervalRunLoop_create(getTime, 2, runCallback, &callbackCalls);
	runLoop->pause(runLoop);
	timeValue = 9.0;
	runLoop->resume(runLoop);
	runLoop->run(runLoop);
	TestCase_assert(callbackCalls == 0, "Loop ran %d times while paused...", callbackCalls);
	
	timeValue = 13.5;
	runLoop->run(runLoop);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	
	timeValue = 15.5;
	runLoop->resume(runLoop);
	runLoop->run(runLoop);
	TestCase_assert(callbackCalls == 3, "Expected 3 but got %d", callbackCalls);
	
	runLoop->pause(runLoop);
	timeValue = 17.5;
	runLoop->pause(runLoop);
	runLoop->resume(runLoop);
	timeValue = 19.5;
	runLoop->run(runLoop);
	TestCase_assert(callbackCalls == 4, "Expected 4 but got %d", callbackCalls);
	
	timeValue = 22.0;
	runLoop->pause(runLoop);
	runLoop->resume(runLoop);
	runLoop->run(runLoop);
	TestCase_assert(callbackCalls == 5, "Expected 5 but got %d", callbackCalls);
	
	runLoop->dispose(runLoop);
}

TEST_SUITE(FixedIntervalRunLoopTest, testCallbackInterval, testPauseResume)
