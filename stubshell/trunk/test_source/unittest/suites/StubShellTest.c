#include <math.h>
#include <string.h>
#include "shell/Shell.h"
#include "stubshell/StubShell.h"
#include "unittest/framework/TestSuite.h"
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#define msleep(ms) Sleep(ms)
#else
#include <unistd.h>
#define msleep(ms) usleep(ms * 1000)
#endif

static unsigned int callbackCalls = 0;
static void * lastContext = NULL;
static bool lastFullScreen = false;
static double lastInterval = 0.0;
static bool lastRepeat = false;
static void (* lastCallback)(unsigned int timerID, void * timerContext) = NULL;
static void * lastTimerContext = NULL;
static unsigned int lastTimerID = 0;

static bool boolReturnValue = false;
static double doubleReturnValue = 0.0;
static const char * constCharPtrReturnValue = false;
static enum ShellBatteryState shellBatteryStateReturnValue = ShellBatteryState_unknown;
static float floatReturnValue = 0.0f;
static unsigned int setTimerReturnValue = 0;

static void voidTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
}

static bool boolTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return boolReturnValue;
}

static bool setFullScreenTestCallback(void * context, bool fullScreen) {
	callbackCalls++;
	lastContext = context;
	lastFullScreen = fullScreen;
	return boolReturnValue;
}

static double doubleTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return doubleReturnValue;
}

static const char * constCharPtrTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return constCharPtrReturnValue;
}

static enum ShellBatteryState shellBatteryStateTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return shellBatteryStateReturnValue;
}

static float floatTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return floatReturnValue;
}

static unsigned int setTimerTestCallback(double interval, bool repeat, void (* callback)(unsigned int timerID, void * timerContext), void * timerContext, void * context) {
	callbackCalls++;
	lastInterval = interval;
	lastRepeat = repeat;
	lastCallback = callback;
	lastTimerContext = timerContext;
	lastContext = context;
	return setTimerReturnValue;
}

static void cancelTimerTestCallback(unsigned int timerID, void * context) {
	callbackCalls++;
	lastContext = context;
	lastTimerID = timerID;
}

static void testMainLoop() {
	callbackCalls = 0;
	lastContext = NULL;
	Shell_mainLoop();
	
	StubShellCallback_mainLoop = voidTestCallback;
	StubShell_callbackContext = (void *) 0x1;
	Shell_mainLoop();
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x1, "Expected 0x1 but got %p", lastContext);
}

static void testRedisplay() {
	callbackCalls = 0;
	lastContext = NULL;
	Shell_redisplay();
	
	StubShellCallback_redisplay = voidTestCallback;
	StubShell_callbackContext = (void *) 0x2;
	Shell_redisplay();
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x2, "Expected 0x2 but got %p", lastContext);
}

static void testIsFullScreen() {
	bool result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_isFullScreen();
	TestCase_assert(!result, "Expected false but got true");
	
	StubShellCallback_isFullScreen = boolTestCallback;
	StubShell_callbackContext = (void *) 0x3;
	boolReturnValue = false;
	result = Shell_isFullScreen();
	TestCase_assert(!result, "Expected false but got true");
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x3, "Expected 0x3 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x4;
	boolReturnValue = true;
	result = Shell_isFullScreen();
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x4, "Expected 0x4 but got %p", lastContext);
}

static void testSetFullScreen() {
	bool result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_setFullScreen(true);
	TestCase_assert(result, "Expected true but got false");
	
	StubShellCallback_setFullScreen = setFullScreenTestCallback;
	StubShell_callbackContext = (void *) 0x5;
	boolReturnValue = true;
	result = Shell_setFullScreen(true);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(lastFullScreen, "Expected true but got false");
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x5, "Expected 0x5 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x6;
	boolReturnValue = false;
	result = Shell_setFullScreen(false);
	TestCase_assert(!result, "Expected false but got true");
	TestCase_assert(!lastFullScreen, "Expected false but got true");
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x6, "Expected 0x6 but got %p", lastContext);
}

#define MAX_DEVIATION_SECONDS 0.25

static void testGetCurrentTime() {
	double startTime, endTime, result;
	
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_getCurrentTime = doubleTestCallback;
	StubShell_callbackContext = (void *) 0x7;
	doubleReturnValue = 1.0;
	result = Shell_getCurrentTime();
	TestCase_assert(result == 1.0, "Expected 1.0 but got %f", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x7, "Expected 0x7 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x8;
	doubleReturnValue = -0.5;
	result = Shell_getCurrentTime();
	TestCase_assert(result == -0.5, "Expected -0.5 but got %f", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x8, "Expected 0x8 but got %p", lastContext);
	
	StubShellCallback_getCurrentTime = NULL;
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

static void testGetResourcePath() {
	const char * result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_getResourcePath();
	TestCase_assert(!strcmp("", result), "Expected \"\" but got \"%s\"", result);
	
	StubShellCallback_getResourcePath = constCharPtrTestCallback;
	StubShell_callbackContext = (void *) 0x9;
	constCharPtrReturnValue = "foo";
	result = Shell_getResourcePath();
	TestCase_assert(!strcmp("foo", result), "Expected \"foo\" but got \"%s\"", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x9, "Expected 0x9 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0xA;
	constCharPtrReturnValue = "bar";
	result = Shell_getResourcePath();
	TestCase_assert(!strcmp("bar", result), "Expected \"bar\" but got \"%s\"", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0xA, "Expected 0xA but got %p", lastContext);
}

static void testGetBatteryState() {
	enum ShellBatteryState result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_getBatteryState();
	TestCase_assert(result == ShellBatteryState_unknown, "Expected ShellBatteryState_unknown but got %d", result);
	
	StubShellCallback_getBatteryState = shellBatteryStateTestCallback;
	StubShell_callbackContext = (void *) 0xB;
	shellBatteryStateReturnValue = ShellBatteryState_full;
	result = Shell_getBatteryState();
	TestCase_assert(result == ShellBatteryState_full, "Expected ShellBatteryState_full but got %d", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0xB, "Expected 0xB but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0xC;
	shellBatteryStateReturnValue = ShellBatteryState_charging;
	result = Shell_getBatteryState();
	TestCase_assert(result == ShellBatteryState_charging, "Expected ShellBatteryState_charging but got %d", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0xC, "Expected 0xC but got %p", lastContext);
}

static void testGetBatteryLevel() {
	float result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_getBatteryLevel();
	TestCase_assert(result == -1.0f, "Expected -1.0 but got %f", result);
	
	StubShellCallback_getBatteryLevel = floatTestCallback;
	StubShell_callbackContext = (void *) 0xD;
	floatReturnValue = 1.0f;
	result = Shell_getBatteryLevel();
	TestCase_assert(result == 1.0f, "Expected 1.0 but got %f", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0xD, "Expected 0xD but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0xE;
	floatReturnValue = 0.25;
	result = Shell_getBatteryLevel();
	TestCase_assert(result == 0.25f, "Expected 0.25 but got %f", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0xE, "Expected 0xE but got %p", lastContext);
}

static void testSetTimer() {
	unsigned int result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_setTimer(0.0, false, NULL, NULL);
	TestCase_assert(result == 0, "Expected 0 but got %u", result);
	
	StubShellCallback_setTimer = setTimerTestCallback;
	StubShell_callbackContext = (void *) 0xF;
	setTimerReturnValue = 1;
	result = Shell_setTimer(1.0, true, (void (*)(unsigned int, void *)) 0x1, (void *) 0x2);
	TestCase_assert(result == 1, "Expected 1 but got %u", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0xF, "Expected 0xF but got %p", lastContext);
	TestCase_assert(lastInterval == 1.0, "Expected 1.0 but got %f", lastInterval);
	TestCase_assert(lastRepeat, "Expected true but got false");
	TestCase_assert(lastCallback == (void (*)(unsigned int, void *)) 0x1, "Expected 0x1 but got %p", lastCallback);
	TestCase_assert(lastTimerContext == (void *) 0x2, "Expected 0x2 but got %p", lastTimerContext);
	
	StubShell_callbackContext = (void *) 0x10;
	setTimerReturnValue = 2;
	result = Shell_setTimer(0.0, false, (void (*)(unsigned int, void *)) 0x4, (void *) 0x3);
	TestCase_assert(result == 2, "Expected 2 but got %u", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x10, "Expected 0x10 but got %p", lastContext);
	TestCase_assert(lastInterval == 0.0, "Expected 0.0 but got %f", lastInterval);
	TestCase_assert(!lastRepeat, "Expected false but got true");
	TestCase_assert(lastCallback == (void (*)(unsigned int, void *)) 0x4, "Expected 0x4 but got %p", lastCallback);
	TestCase_assert(lastTimerContext == (void *) 0x3, "Expected 0x3 but got %p", lastTimerContext);
}

static void testCancelTimer() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_cancelTimer = cancelTimerTestCallback;
	StubShell_callbackContext = (void *) 0x11;
	Shell_cancelTimer(2);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x11, "Expected 0x11 but got %p", lastContext);
	TestCase_assert(lastTimerID == 2, "Expected 2 but got %u", lastTimerID);
	
	StubShell_callbackContext = (void *) 0x12;
	Shell_cancelTimer(1);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x12, "Expected 0x12 but got %p", lastContext);
	TestCase_assert(lastTimerID == 1, "Expected 1 but got %u", lastTimerID);
}

TEST_SUITE(StubShellTest,
           testMainLoop,
           testRedisplay,
           testIsFullScreen,
           testSetFullScreen,
           testGetCurrentTime,
           testGetResourcePath,
           testGetBatteryState,
           testGetBatteryLevel,
           testSetTimer,
           testCancelTimer)
