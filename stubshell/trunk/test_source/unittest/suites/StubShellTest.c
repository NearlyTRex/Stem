#include "unittest/suites/StubShellTest.h"

#include <math.h>
#include <string.h>
#include "shell/Shell.h"
#include "unittest/framework/TestSuite.h"
#include <stdio.h>

#ifdef WIN32
#include <winbase.h>
#define msleep(ms) Sleep(ms)
#else
#include <unistd.h>
#define msleep(ms) usleep(ms * 1000)
#endif

#define MAX_DEVIATION_SECONDS 0.25

static void testGetCurrentTime() {
	double startTime, endTime;
	
	startTime = Shell_getCurrentTime();
	msleep(300);
	endTime = Shell_getCurrentTime();
	TestCase_assert(startTime <= endTime, "Clock is counting backward (%f -> %f)!", startTime, endTime);
	TestCase_assert(fabs(0.3 - (endTime - startTime)) < MAX_DEVIATION_SECONDS, "Shell_getCurrentTime delta (%f) not within maximum deviation limit (%f seconds) of 0.3 seconds", endTime - startTime, MAX_DEVIATION_SECONDS);
	
	startTime = Shell_getCurrentTime();
	msleep(500);
	endTime = Shell_getCurrentTime();
	TestCase_assert(startTime <= endTime, "Clock is counting backward (%f -> %f)!", startTime, endTime);
	TestCase_assert(fabs(0.5 - (endTime - startTime)) < MAX_DEVIATION_SECONDS, "Shell_getCurrentTime delta (%f) not within maximum deviation limit (%f seconds) of 0.5 seconds", endTime - startTime, MAX_DEVIATION_SECONDS);
}

struct TestSuite * StubShellTest_suite() {
	return testSuite("StubShellTest", testGetCurrentTime, NULL);
}
