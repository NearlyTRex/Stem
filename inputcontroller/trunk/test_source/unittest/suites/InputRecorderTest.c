#include "unittest/framework/TestSuite.h"
#include <stdbool.h>

static void testInit() {
	TestCase_assert(false, "Unimplemented");
}

static void testBasicRecording() {
	TestCase_assert(false, "Unimplemented");
}

static void testWriteFile() {
	TestCase_assert(false, "Unimplemented");
}

static void testWriteData() {
	TestCase_assert(false, "Unimplemented");
}

static void testContinuousOutput() {
	TestCase_assert(false, "Unimplemented");
}

TEST_SUITE(InputRecorderTest,
           testInit,
           testBasicRecording,
           testWriteFile,
           testWriteData,
           testContinuousOutput)
