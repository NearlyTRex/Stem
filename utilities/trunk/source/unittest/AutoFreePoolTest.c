#include "unittest/TestSuite.h"
#include "utilities/AutoFreePool.h"

static void incrementCallback(void * context) {
	*(int *) context += 1;
}

static void testCallbacks() {
	int calls = 0;
	
	AutoFreePool_addCallback(incrementCallback, &calls);
	AutoFreePool_empty();
	TestCase_assert(calls == 1, "Callback not called the expected number of times (called %d, expected 1)", calls);
	
	AutoFreePool_addCallback(incrementCallback, &calls);
	AutoFreePool_addCallback(incrementCallback, &calls);
	AutoFreePool_empty();
	TestCase_assert(calls == 3, "Callback not called the expected number of times (called %d, expected 3)", calls);
}

static void testPushPop() {
	int calls = 0;
	
	AutoFreePool_addCallback(incrementCallback, &calls);
	AutoFreePool_push();
	AutoFreePool_empty();
	TestCase_assert(calls == 0, "Callback not called the expected number of times (called %d, expected 0)", calls);
	
	AutoFreePool_addCallback(incrementCallback, &calls);
	AutoFreePool_empty();
	TestCase_assert(calls == 1, "Callback not called the expected number of times (called %d, expected 1)", calls);
	
	AutoFreePool_addCallback(incrementCallback, &calls);
	AutoFreePool_pop();
	TestCase_assert(calls == 2, "Callback not called the expected number of times (called %d, expected 2)", calls);
	
	AutoFreePool_empty();
	TestCase_assert(calls == 3, "Callback not called the expected number of times (called %d, expected 3)", calls);
}

TEST_SUITE(AutoFreePoolTest, testCallbacks, testPushPop)
