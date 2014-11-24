#include <math.h>
#include <string.h>
#include "shell/Shell.h"
#include "stubshell/StubShell.h"
#include "unittest/TestSuite.h"
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#define msleep(ms) Sleep(ms)
#else
#include <unistd.h>
#define msleep(ms) usleep(ms * 1000)
#endif

static unsigned int callbackCalls;
static void * lastContext;
static double lastInterval;
static bool lastRepeat;
static void (* lastTimerCallback)(unsigned int timerID, void * timerContext);
static void * lastPtrArg;
static unsigned int lastTimerID;
static bool lastBoolArg, lastBoolArg2;
static int lastIntArg;
static int lastUIntArg;
static ShellThread lastShellThreadArg;
static ShellMutex lastShellMutexArg;
static int (* lastThreadCallback)(void * threadContext);
static const char * lastConstCharPtrArg, * lastConstCharPtrArg2;
static char * lastCharPtrArg;
static int * lastOffsetXPtrArg;
static int * lastOffsetYPtrArg;
static unsigned int * lastWidthPtrArg;
static unsigned int * lastHeightPtrArg;

static bool boolReturnValue;
static double doubleReturnValue;
static const char * constCharPtrReturnValue;
static enum ShellBatteryState shellBatteryStateReturnValue;
static float floatReturnValue;
static int intReturnValue;
static unsigned int uintReturnValue;
static void * ptrReturnValue;

static void voidTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
}

static bool boolReturnTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return boolReturnValue;
}

static bool enterFullScreenTestCallback(void * context, unsigned int uintArg) {
	callbackCalls++;
	lastContext = context;
	lastUIntArg = uintArg;
	return boolReturnValue;
}

static double doubleReturnTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return doubleReturnValue;
}

static unsigned int uintReturnTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return uintReturnValue;
}

static const char * constCharPtrReturnTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return constCharPtrReturnValue;
}

static const char * getSupportPathTestCallback(void * context, const char * subdirectory) {
	callbackCalls++;
	lastContext = context;
	lastConstCharPtrArg = subdirectory;
	return constCharPtrReturnValue;
}

static enum ShellBatteryState shellBatteryStateReturnTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return shellBatteryStateReturnValue;
}

static float floatReturnTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return floatReturnValue;
}

static unsigned int setTimerTestCallback(void * context, double interval, bool repeat, void (* callback)(unsigned int timerID, void * timerContext), void * timerContext) {
	callbackCalls++;
	lastInterval = interval;
	lastRepeat = repeat;
	lastTimerCallback = callback;
	lastPtrArg = timerContext;
	lastContext = context;
	return uintReturnValue;
}

static void cancelTimerTestCallback(void * context, unsigned int timerID) {
	callbackCalls++;
	lastContext = context;
	lastTimerID = timerID;
}

static void boolArgTestCallback(void * context, bool boolArg) {
	callbackCalls++;
	lastContext = context;
	lastBoolArg = boolArg;
}

static void intArgTestCallback(void * context, int intArg) {
	callbackCalls++;
	lastContext = context;
	lastIntArg = intArg;
}

static void constCharPtrArgTestCallback(void * context, const char * constCharPtrArg) {
	callbackCalls++;
	lastContext = context;
	lastConstCharPtrArg = constCharPtrArg;
}

static void getDisplayBoundsTestCallback(void * context, unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight) {
	callbackCalls++;
	lastUIntArg = displayIndex;
	lastOffsetXPtrArg = outOffsetX;
	lastOffsetYPtrArg = outOffsetY;
	lastWidthPtrArg = outWidth;
	lastHeightPtrArg = outHeight;
	lastContext = context;
}

static ShellThread createThreadTestCallback(void * context, int (* callback)(void * threadContext), void * threadContext) {
	callbackCalls++;
	lastThreadCallback = callback;
	lastPtrArg = threadContext;
	lastContext = context;
	return ptrReturnValue;
}

static int shellThreadArgTestCallback(void * context, ShellThread shellThreadArg) {
	callbackCalls++;
	lastContext = context;
	lastShellThreadArg = shellThreadArg;
	return intReturnValue;
}

static void shellMutexArgTestCallback(void * context, ShellMutex shellMutexArg) {
	callbackCalls++;
	lastContext = context;
	lastShellMutexArg = shellMutexArg;
}

static bool tryLockMutexTestCallback(void * context, ShellMutex shellMutexArg) {
	callbackCalls++;
	lastContext = context;
	lastShellMutexArg = shellMutexArg;
	return boolReturnValue;
}

static ShellThread shellThreadReturnTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return ptrReturnValue;
}

static ShellMutex shellMutexReturnTestCallback(void * context) {
	callbackCalls++;
	lastContext = context;
	return ptrReturnValue;
}

static void setVSyncTestCallback(void * context, bool boolArg, bool boolArg2) {
	callbackCalls++;
	lastContext = context;
	lastBoolArg = boolArg;
	lastBoolArg2 = boolArg2;
}

static bool openFileDialogTestCallback(void * context, const char * basePath, char * outFilePath, unsigned int maxLength) {
	callbackCalls++;
	lastContext = context;
	lastConstCharPtrArg = basePath;
	lastCharPtrArg = outFilePath;
	lastUIntArg = maxLength;
	return boolReturnValue;
}

static bool saveFileDialogTestCallback(void * context, const char * basePath, const char * baseName, char * outFilePath, unsigned int maxLength) {
	callbackCalls++;
	lastContext = context;
	lastConstCharPtrArg = basePath;
	lastConstCharPtrArg2 = baseName;
	lastCharPtrArg = outFilePath;
	lastUIntArg = maxLength;
	return boolReturnValue;
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
	
	StubShellCallback_isFullScreen = boolReturnTestCallback;
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

#define MAX_DEVIATION_SECONDS 0.25

static void testGetCurrentTime() {
	double startTime, endTime, result;
	
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_getCurrentTime = doubleReturnTestCallback;
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
	
	StubShellCallback_getResourcePath = constCharPtrReturnTestCallback;
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

static void testGetSupportPath() {
	const char * result;
	
	callbackCalls = 0;
	lastContext = NULL;
	lastConstCharPtrArg = NULL;
	result = Shell_getSupportPath(NULL);
	TestCase_assert(!strcmp("", result), "Expected \"\" but got \"%s\"", result);
	
	StubShellCallback_getSupportPath = getSupportPathTestCallback;
	StubShell_callbackContext = (void *) 0x9;
	constCharPtrReturnValue = "foo";
	result = Shell_getSupportPath("bar");
	TestCase_assert(!strcmp("foo", result), "Expected \"foo\" but got \"%s\"", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x9, "Expected 0x9 but got %p", lastContext);
	TestCase_assert(!strcmp("bar", lastConstCharPtrArg), "Expected \"bar\" but got \"%s\"", lastConstCharPtrArg);
	
	StubShell_callbackContext = (void *) 0xA;
	constCharPtrReturnValue = "bar";
	result = Shell_getSupportPath("baz");
	TestCase_assert(!strcmp("bar", result), "Expected \"bar\" but got \"%s\"", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0xA, "Expected 0xA but got %p", lastContext);
	TestCase_assert(!strcmp("baz", lastConstCharPtrArg), "Expected \"baz\" but got \"%s\"", lastConstCharPtrArg);
}

static void testGetBatteryState() {
	enum ShellBatteryState result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_getBatteryState();
	TestCase_assert(result == ShellBatteryState_unknown, "Expected ShellBatteryState_unknown but got %d", result);
	
	StubShellCallback_getBatteryState = shellBatteryStateReturnTestCallback;
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
	
	StubShellCallback_getBatteryLevel = floatReturnTestCallback;
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
	uintReturnValue = 1;
	result = Shell_setTimer(1.0, true, (void (*)(unsigned int, void *)) 0x1, (void *) 0x2);
	TestCase_assert(result == 1, "Expected 1 but got %u", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0xF, "Expected 0xF but got %p", lastContext);
	TestCase_assert(lastInterval == 1.0, "Expected 1.0 but got %f", lastInterval);
	TestCase_assert(lastRepeat, "Expected true but got false");
	TestCase_assert(lastTimerCallback == (void (*)(unsigned int, void *)) 0x1, "Expected 0x1 but got %p", lastTimerCallback);
	TestCase_assert(lastPtrArg == (void *) 0x2, "Expected 0x2 but got %p", lastPtrArg);
	
	StubShell_callbackContext = (void *) 0x10;
	uintReturnValue = 2;
	result = Shell_setTimer(0.0, false, (void (*)(unsigned int, void *)) 0x4, (void *) 0x3);
	TestCase_assert(result == 2, "Expected 2 but got %u", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x10, "Expected 0x10 but got %p", lastContext);
	TestCase_assert(lastInterval == 0.0, "Expected 0.0 but got %f", lastInterval);
	TestCase_assert(!lastRepeat, "Expected false but got true");
	TestCase_assert(lastTimerCallback == (void (*)(unsigned int, void *)) 0x4, "Expected 0x4 but got %p", lastTimerCallback);
	TestCase_assert(lastPtrArg == (void *) 0x3, "Expected 0x3 but got %p", lastPtrArg);
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

static void testSetCursorVisible() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_setCursorVisible = boolArgTestCallback;
	StubShell_callbackContext = (void *) 0x13;
	Shell_setCursorVisible(true);
	TestCase_assert(lastBoolArg, "Expected true but got false");
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x13, "Expected 0x13 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x14;
	Shell_setCursorVisible(false);
	TestCase_assert(!lastBoolArg, "Expected false but got true");
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x14, "Expected 0x14 but got %p", lastContext);
}

static void testHideCursorUntilMouseMoves() {
	callbackCalls = 0;
	lastContext = NULL;
	Shell_hideCursorUntilMouseMoves();
	
	StubShellCallback_hideCursorUntilMouseMoves = voidTestCallback;
	StubShell_callbackContext = (void *) 0x15;
	Shell_hideCursorUntilMouseMoves();
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x15, "Expected 0x15 but got %p", lastContext);
}

static void testSetCursor() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_setCursor = intArgTestCallback;
	StubShell_callbackContext = (void *) 0x16;
	Shell_setCursor(2);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x16, "Expected 0x16 but got %p", lastContext);
	TestCase_assert(lastIntArg == 2, "Expected 2 but got %d", lastIntArg);
	
	StubShell_callbackContext = (void *) 0x17;
	Shell_setCursor(1);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x17, "Expected 0x17 but got %p", lastContext);
	TestCase_assert(lastIntArg == 1, "Expected 1 but got %d", lastIntArg);
}

static void testSetMouseDeltaMode() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_setMouseDeltaMode = boolArgTestCallback;
	StubShell_callbackContext = (void *) 0x18;
	Shell_setMouseDeltaMode(true);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x18, "Expected 0x18 but got %p", lastContext);
	TestCase_assert(lastBoolArg, "Expected true but got false");
	
	StubShell_callbackContext = (void *) 0x19;
	Shell_setMouseDeltaMode(false);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x19, "Expected 0x19 but got %p", lastContext);
	TestCase_assert(!lastBoolArg == 1, "Expected false but got true");
}

static void testCreateThread() {
	ShellThread result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_createThread(NULL, NULL);
	TestCase_assert(result == NULL, "Expected NULL but got %p", result);
	
	StubShellCallback_createThread = createThreadTestCallback;
	StubShell_callbackContext = (void *) 0x1A;
	ptrReturnValue = (ShellThread) 0x3;
	result = Shell_createThread((int (*)(void *)) 0x1, (void *) 0x2);
	TestCase_assert(result == (ShellThread) 0x3, "Expected 0x3 but got %p", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x1A, "Expected 0x1A but got %p", lastContext);
	TestCase_assert(lastThreadCallback == (int (*)(void *)) 0x1, "Expected 0x1 but got %p", lastThreadCallback);
	TestCase_assert(lastPtrArg == (void *) 0x2, "Expected 0x2 but got %p", lastPtrArg);
	
	StubShell_callbackContext = (void *) 0x1B;
	ptrReturnValue = (ShellThread) 0x2;
	result = Shell_createThread((int (*)(void *)) 0x4, (void *) 0x3);
	TestCase_assert(result == (ShellThread) 0x2, "Expected 0x2 but got %p", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x1B, "Expected 0x1B but got %p", lastContext);
	TestCase_assert(lastThreadCallback == (int (*)(void *)) 0x4, "Expected 0x4 but got %p", lastThreadCallback);
	TestCase_assert(lastPtrArg == (void *) 0x3, "Expected 0x3 but got %p", lastPtrArg);
}

static void testExitThread() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_exitThread = intArgTestCallback;
	StubShell_callbackContext = (void *) 0x1C;
	Shell_exitThread(2);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x1C, "Expected 0x1C but got %p", lastContext);
	TestCase_assert(lastIntArg == 2, "Expected 2 but got %d", lastIntArg);
	
	StubShell_callbackContext = (void *) 0x1D;
	Shell_exitThread(1);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x1D, "Expected 0x1D but got %p", lastContext);
	TestCase_assert(lastIntArg == 1, "Expected 1 but got %d", lastIntArg);
}

static void testJoinThread() {
	int result;
	
	callbackCalls = 0;
	lastContext = NULL;
	
	intReturnValue = 1;
	StubShellCallback_joinThread = shellThreadArgTestCallback;
	StubShell_callbackContext = (void *) 0x20;
	result = Shell_joinThread((ShellThread) 0x5);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x20, "Expected 0x20 but got %p", lastContext);
	TestCase_assert(lastShellThreadArg == (ShellThread) 0x5, "Expected 0x5 but got %p", lastShellThreadArg);
	TestCase_assert(result == 1, "Expected 1 but got %d", result);
	
	intReturnValue = 3;
	StubShell_callbackContext = (void *) 0x21;
	result = Shell_joinThread((ShellThread) 0x6);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x21, "Expected 0x21 but got %p", lastContext);
	TestCase_assert(lastShellThreadArg == (ShellThread) 0x6, "Expected 0x6 but got %p", lastShellThreadArg);
	TestCase_assert(result == 3, "Expected 3 but got %d", result);
}

static void testGetCurrentThread() {
	ShellThread result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_getCurrentThread();
	TestCase_assert(result == NULL, "Expected NULL but got %p", result);
	
	StubShellCallback_getCurrentThread = shellThreadReturnTestCallback;
	StubShell_callbackContext = (void *) 0x22;
	ptrReturnValue = (void *) 0x7;
	result = Shell_getCurrentThread();
	TestCase_assert(result == (void *) 0x7, "Expected 0x7 but got %p", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x22, "Expected 0x22 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x23;
	ptrReturnValue = (void *) 0x8;
	result = Shell_getCurrentThread();
	TestCase_assert(result == (void *) 0x8, "Expected 0x8 but got %p", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x23, "Expected 0x23 but got %p", lastContext);
}

static void testCreateMutex() {
	ShellMutex result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_createMutex();
	TestCase_assert(result == NULL, "Expected NULL but got %p", result);
	
	StubShellCallback_createMutex = shellMutexReturnTestCallback;
	StubShell_callbackContext = (void *) 0x24;
	ptrReturnValue = (void *) 0x9;
	result = Shell_createMutex();
	TestCase_assert(result == (void *) 0x9, "Expected 0x9 but got %p", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x24, "Expected 0x24 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x25;
	ptrReturnValue = (void *) 0xA;
	result = Shell_createMutex();
	TestCase_assert(result == (void *) 0xA, "Expected 0xA but got %p", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x25, "Expected 0x25 but got %p", lastContext);
}

static void testDisposeMutex() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_disposeMutex = shellMutexArgTestCallback;
	StubShell_callbackContext = (void *) 0x26;
	Shell_disposeMutex((ShellMutex) 0x1);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x26, "Expected 0x26 but got %p", lastContext);
	TestCase_assert(lastShellMutexArg == (ShellMutex) 0x1, "Expected 0x1 but got %p", lastShellMutexArg);
	
	StubShell_callbackContext = (void *) 0x27;
	Shell_disposeMutex((ShellMutex) 0x2);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x27, "Expected 0x27 but got %p", lastContext);
	TestCase_assert(lastShellMutexArg == (ShellMutex) 0x2, "Expected 0x2 but got %p", lastShellMutexArg);
}

static void testLockMutex() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_lockMutex = shellMutexArgTestCallback;
	StubShell_callbackContext = (void *) 0x28;
	Shell_lockMutex((ShellMutex) 0x3);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x28, "Expected 0x28 but got %p", lastContext);
	TestCase_assert(lastShellMutexArg == (ShellMutex) 0x3, "Expected 0x3 but got %p", lastShellMutexArg);
	
	StubShell_callbackContext = (void *) 0x29;
	Shell_disposeMutex((ShellMutex) 0x4);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x29, "Expected 0x29 but got %p", lastContext);
	TestCase_assert(lastShellMutexArg == (ShellMutex) 0x4, "Expected 0x4 but got %p", lastShellMutexArg);
}

static void testTryLockMutex() {
	bool result;

	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_tryLockMutex = tryLockMutexTestCallback;
	StubShell_callbackContext = (void *) 0x2A;
	boolReturnValue = true;
	result = Shell_tryLockMutex((ShellMutex) 0x5);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x2A, "Expected 0x2A but got %p", lastContext);
	TestCase_assert(lastShellMutexArg == (ShellMutex) 0x5, "Expected 0x5 but got %p", lastShellMutexArg);
	TestCase_assert(result, "Expected true but got false");
	
	StubShell_callbackContext = (void *) 0x2B;
	boolReturnValue = false;
	result = Shell_tryLockMutex((ShellMutex) 0x6);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x2B, "Expected 0x2B but got %p", lastContext);
	TestCase_assert(lastShellMutexArg == (ShellMutex) 0x6, "Expected 0x6 but got %p", lastShellMutexArg);
	TestCase_assert(!result, "Expected false but got true");
}

static void testUnlockMutex() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_unlockMutex = shellMutexArgTestCallback;
	StubShell_callbackContext = (void *) 0x2C;
	Shell_unlockMutex((ShellMutex) 0x7);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x2C, "Expected 0x2C but got %p", lastContext);
	TestCase_assert(lastShellMutexArg == (ShellMutex) 0x7, "Expected 0x7 but got %p", lastShellMutexArg);
	
	StubShell_callbackContext = (void *) 0x2D;
	Shell_unlockMutex((ShellMutex) 0x8);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x2D, "Expected 0x2D but got %p", lastContext);
	TestCase_assert(lastShellMutexArg == (ShellMutex) 0x8, "Expected 0x8 but got %p", lastShellMutexArg);
}

static void testOpenURL() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_openURL = constCharPtrArgTestCallback;
	StubShell_callbackContext = (void *) 0x2E;
	Shell_openURL(NULL);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x2E, "Expected 0x2E but got %p", lastContext);
	TestCase_assert(lastConstCharPtrArg == NULL, "Expected NULL but got %p", lastConstCharPtrArg);
	
	StubShell_callbackContext = (void *) 0x2F;
	Shell_openURL((const char *) 0x1);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x2F, "Expected 0x2F but got %p", lastContext);
	TestCase_assert(lastConstCharPtrArg == (const char *) 0x1, "Expected 0x1 but got %p", lastConstCharPtrArg);
}

static void testEnterFullScreen() {
	bool result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_enterFullScreen(0);
	TestCase_assert(result, "Expected true but got false");
	
	StubShellCallback_enterFullScreen = enterFullScreenTestCallback;
	StubShell_callbackContext = (void *) 0x5;
	boolReturnValue = true;
	result = Shell_enterFullScreen(0);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(lastUIntArg == 0, "Expected 0 but got %u", lastUIntArg);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x5, "Expected 0x5 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x6;
	boolReturnValue = false;
	result = Shell_enterFullScreen(1);
	TestCase_assert(!result, "Expected false but got true");
	TestCase_assert(lastUIntArg == 1, "Expected 1 but got %u", lastUIntArg);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x6, "Expected 0x6 but got %p", lastContext);
}

static void testExitFullScreen() {
	callbackCalls = 0;
	lastContext = NULL;
	StubShellCallback_exitFullScreen = voidTestCallback;
	StubShell_callbackContext = (void *) 0x30;
	Shell_exitFullScreen();
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x30, "Expected 0x30 but got %p", lastContext);
}

static void testGetDisplayCount() {
	unsigned int result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_getDisplayCount();
	TestCase_assert(result == 0, "Expected 0 but got %u", result);
	
	StubShellCallback_getDisplayCount = uintReturnTestCallback;
	StubShell_callbackContext = (void *) 0x31;
	uintReturnValue = 1;
	result = Shell_getDisplayCount();
	TestCase_assert(result == 1, "Expected 1 but got %u", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x31, "Expected 0x31 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x32;
	uintReturnValue = 2;
	result = Shell_getDisplayCount();
	TestCase_assert(result == 2, "Expected 2 but got %u", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x32, "Expected 0x32 but got %p", lastContext);
}

static void testGetDisplayIndexFromWindow() {
	unsigned int result;
	
	callbackCalls = 0;
	lastContext = NULL;
	result = Shell_getDisplayIndexFromWindow();
	TestCase_assert(result == 0, "Expected 0 but got %u", result);
	
	StubShellCallback_getDisplayIndexFromWindow = uintReturnTestCallback;
	StubShell_callbackContext = (void *) 0x33;
	uintReturnValue = 1;
	result = Shell_getDisplayIndexFromWindow();
	TestCase_assert(result == 1, "Expected 1 but got %u", result);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x33, "Expected 0x33 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x34;
	uintReturnValue = 2;
	result = Shell_getDisplayIndexFromWindow();
	TestCase_assert(result == 2, "Expected 2 but got %u", result);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x34, "Expected 0x34 but got %p", lastContext);
}

static void testGetDisplayBounds() {
	callbackCalls = 0;
	lastContext = NULL;
	StubShellCallback_getDisplayBounds = getDisplayBoundsTestCallback;
	StubShell_callbackContext = (void *) 0x35;
	Shell_getDisplayBounds(1, (int *) 0x2, (int *) 0x3, (unsigned int *) 0x4, (unsigned int *) 0x5);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x35, "Expected 0x35 but got %p", lastContext);
	TestCase_assert(lastUIntArg == 1, "Expected 1 but got %u", lastUIntArg);
	TestCase_assert(lastOffsetXPtrArg == (int *) 0x2, "Expected 0x2 but got %p", lastOffsetXPtrArg);
	TestCase_assert(lastOffsetYPtrArg == (int *) 0x3, "Expected 0x3 but got %p", lastOffsetYPtrArg);
	TestCase_assert(lastWidthPtrArg == (unsigned int *) 0x4, "Expected 0x4 but got %p", lastWidthPtrArg);
	TestCase_assert(lastHeightPtrArg == (unsigned int *) 0x5, "Expected 0x5 but got %p", lastHeightPtrArg);
	
	StubShell_callbackContext = (void *) 0x36;
	Shell_getDisplayBounds(6, (int *) 0x7, (int *) 0x8, (unsigned int *) 0x9, (unsigned int *) 0xA);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x36, "Expected 0x36 but got %p", lastContext);
	TestCase_assert(lastUIntArg == 6, "Expected 1 but got %u", lastUIntArg);
	TestCase_assert(lastOffsetXPtrArg == (int *) 0x7, "Expected 0x7 but got %p", lastOffsetXPtrArg);
	TestCase_assert(lastOffsetYPtrArg == (int *) 0x8, "Expected 0x8 but got %p", lastOffsetYPtrArg);
	TestCase_assert(lastWidthPtrArg == (unsigned int *) 0x9, "Expected 0x9 but got %p", lastWidthPtrArg);
	TestCase_assert(lastHeightPtrArg == (unsigned int *) 0xA, "Expected 0xA but got %p", lastHeightPtrArg);
}

static void testSetVSync() {
	callbackCalls = 0;
	lastContext = NULL;
	
	StubShellCallback_setVSync = setVSyncTestCallback;
	StubShell_callbackContext = (void *) 0x37;
	Shell_setVSync(true, false);
	TestCase_assert(lastBoolArg, "Expected true but got false");
	TestCase_assert(!lastBoolArg2, "Expected false but got true");
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x37, "Expected 0x37 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x38;
	Shell_setVSync(false, true);
	TestCase_assert(!lastBoolArg, "Expected false but got true");
	TestCase_assert(lastBoolArg2, "Expected true but got false");
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x38, "Expected 0x38 but got %p", lastContext);
}

static void testOpenFileDialog() {
	bool result;
	
	callbackCalls = 0;
	lastContext = NULL;
	StubShellCallback_openFileDialog = openFileDialogTestCallback;
	StubShell_callbackContext = (void *) 0x39;
	boolReturnValue = false;
	result = Shell_openFileDialog((const char *) 0x1, (char *) 0x2, 3);
	TestCase_assert(!result, "Expected false but got true");
	TestCase_assert(lastConstCharPtrArg == (const char *) 0x1, "Expected 0x1 but got %p", lastConstCharPtrArg);
	TestCase_assert(lastCharPtrArg == (char *) 0x2, "Expected 0x2 but got %p", lastCharPtrArg);
	TestCase_assert(lastUIntArg == 3, "Expected 3 but got %u", lastUIntArg);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x39, "Expected 0x39 but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x3A;
	boolReturnValue = true;
	result = Shell_openFileDialog((const char *) 0x4, (char *) 0x5, 6);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(lastConstCharPtrArg == (const char *) 0x4, "Expected 0x4 but got %p", lastConstCharPtrArg);
	TestCase_assert(lastCharPtrArg == (char *) 0x5, "Expected 0x5 but got %p", lastCharPtrArg);
	TestCase_assert(lastUIntArg == 6, "Expected 6 but got %u", lastUIntArg);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x3A, "Expected 0x3A but got %p", lastContext);
}

static void testSaveFileDialog() {
	bool result;
	
	callbackCalls = 0;
	lastContext = NULL;
	StubShellCallback_saveFileDialog = saveFileDialogTestCallback;
	StubShell_callbackContext = (void *) 0x3B;
	boolReturnValue = false;
	result = Shell_saveFileDialog((const char *) 0x1, (const char *) 0x2, (char *) 0x3, 4);
	TestCase_assert(!result, "Expected false but got true");
	TestCase_assert(lastConstCharPtrArg == (const char *) 0x1, "Expected 0x1 but got %p", lastConstCharPtrArg);
	TestCase_assert(lastConstCharPtrArg2 == (const char *) 0x2, "Expected 0x2 but got %p", lastConstCharPtrArg2);
	TestCase_assert(lastCharPtrArg == (char *) 0x3, "Expected 0x3 but got %p", lastCharPtrArg);
	TestCase_assert(lastUIntArg == 4, "Expected 4 but got %u", lastUIntArg);
	TestCase_assert(callbackCalls == 1, "Expected 1 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x3B, "Expected 0x3B but got %p", lastContext);
	
	StubShell_callbackContext = (void *) 0x3C;
	boolReturnValue = true;
	result = Shell_saveFileDialog((const char *) 0x5, (const char *) 0x6, (char *) 0x7, 8);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(lastConstCharPtrArg == (const char *) 0x5, "Expected 0x5 but got %p", lastConstCharPtrArg);
	TestCase_assert(lastConstCharPtrArg2 == (const char *) 0x6, "Expected 0x6 but got %p", lastConstCharPtrArg2);
	TestCase_assert(lastCharPtrArg == (char *) 0x7, "Expected 0x7 but got %p", lastCharPtrArg);
	TestCase_assert(lastUIntArg == 8, "Expected 8 but got %u", lastUIntArg);
	TestCase_assert(callbackCalls == 2, "Expected 2 but got %d", callbackCalls);
	TestCase_assert(lastContext == (void *) 0x3C, "Expected 0x3C but got %p", lastContext);
}

TEST_SUITE(StubShellTest,
           testMainLoop,
           testRedisplay,
           testIsFullScreen,
           testGetCurrentTime,
           testGetResourcePath,
           testGetSupportPath,
           testGetBatteryState,
           testGetBatteryLevel,
           testSetTimer,
           testCancelTimer,
           testSetCursorVisible,
           testHideCursorUntilMouseMoves,
           testSetCursor,
           testSetMouseDeltaMode,
           testCreateThread,
           testExitThread,
           testJoinThread,
           testGetCurrentThread,
           testCreateMutex,
           testDisposeMutex,
           testLockMutex,
           testTryLockMutex,
           testUnlockMutex,
           testOpenURL,
           testEnterFullScreen,
           testExitFullScreen,
           testGetDisplayCount,
           testGetDisplayIndexFromWindow,
           testGetDisplayBounds,
           testSetVSync,
           testOpenFileDialog,
           testSaveFileDialog)
