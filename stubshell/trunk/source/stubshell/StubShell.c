/*
  Copyright (c) 2014 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener alex@ludobloom.com
*/

#include "shell/Shell.h"
#include "stubshell/StubShell.h"

#if defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#elif defined(__linux)
#include <time.h>
#elif defined(WIN32)
#include <windows.h>
#endif

#include <stdio.h>

void * StubShell_callbackContext = NULL;

void (* StubShellCallback_mainLoop)(void * context) = NULL;
void (* StubShellCallback_redisplay)(void * context) = NULL;
bool (* StubShellCallback_isFullScreen)(void * context) = NULL;
bool (* StubShellCallback_enterFullScreen)(void * context, unsigned int displayIndex) = NULL;
void (* StubShellCallback_exitFullScreen)(void * context) = NULL;
double (* StubShellCallback_getCurrentTime)(void * context) = NULL;
const char * (* StubShellCallback_getResourcePath)(void * context) = NULL;
const char * (* StubShellCallback_getSupportPath)(void * context, const char * subdirectory) = NULL;
unsigned int (* StubShellCallback_getDisplayCount)(void * context) = NULL;
unsigned int (* StubShellCallback_getDisplayIndexFromWindow)(void * context) = NULL;
void (* StubShellCallback_getDisplayBounds)(void * context, unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight) = NULL;
enum ShellBatteryState (* StubShellCallback_getBatteryState)(void * context) = NULL;
float (* StubShellCallback_getBatteryLevel)(void * context) = NULL;
unsigned int (* StubShellCallback_setTimer)(void * context, double interval, bool repeat, void (* callback)(unsigned int timerID, void * timerContext), void * timerContext) = NULL;
void (* StubShellCallback_cancelTimer)(void * context, unsigned int timerID) = NULL;
void (* StubShellCallback_setCursorVisible)(void * context, bool visible) = NULL;
void (* StubShellCallback_hideCursorUntilMouseMoves)(void * context) = NULL;
void (* StubShellCallback_setCursor)(void * context, int cursor) = NULL;
void (* StubShellCallback_setMouseDeltaMode)(void * context, bool deltaMode) = NULL;
void (* StubShellCallback_openURL)(void * context, const char * url) = NULL;
void (* StubShellCallback_setVSync)(void * context, bool sync, bool fullscreen) = NULL;
bool (* StubShellCallback_openFileDialog)(void * context, const char * basePath, char * outFilePath, unsigned int maxLength) = NULL;
bool (* StubShellCallback_saveFileDialog)(void * context, const char * basePath, const char * baseName, char * outFilePath, unsigned int maxLength) = NULL;
ShellThread (* StubShellCallback_createThread)(void * context, int (* threadFunction)(void * context), void * threadContext) = NULL;
void (* StubShellCallback_exitThread)(void * context, int statusCode) = NULL;
int (* StubShellCallback_joinThread)(void * context, ShellThread thread) = NULL;
ShellThread (* StubShellCallback_getCurrentThread)(void * context) = NULL;
ShellMutex (* StubShellCallback_createMutex)(void * context) = NULL;
void (* StubShellCallback_disposeMutex)(void * context, ShellMutex mutex) = NULL;
void (* StubShellCallback_lockMutex)(void * context, ShellMutex mutex) = NULL;
bool (* StubShellCallback_tryLockMutex)(void * context, ShellMutex mutex) = NULL;
void (* StubShellCallback_unlockMutex)(void * context, ShellMutex mutex) = NULL;

void Shell_mainLoop() {
	if (StubShellCallback_mainLoop != NULL) {
		StubShellCallback_mainLoop(StubShell_callbackContext);
	}
}

void Shell_redisplay() {
	if (StubShellCallback_redisplay != NULL) {
		StubShellCallback_redisplay(StubShell_callbackContext);
	}
}

bool Shell_isFullScreen() {
	if (StubShellCallback_isFullScreen != NULL) {
		return StubShellCallback_isFullScreen(StubShell_callbackContext);
	}
	return false;
}

bool Shell_enterFullScreen(unsigned int displayIndex) {
	if (StubShellCallback_enterFullScreen != NULL) {
		return StubShellCallback_enterFullScreen(StubShell_callbackContext, displayIndex);
	}
	return true;
}

void Shell_exitFullScreen() {
	if (StubShellCallback_exitFullScreen != NULL) {
		StubShellCallback_exitFullScreen(StubShell_callbackContext);
	}
}

#ifndef S_SPLINT_S
double Shell_getCurrentTime() {
	if (StubShellCallback_getCurrentTime != NULL) {
		return StubShellCallback_getCurrentTime(StubShell_callbackContext);
	}
#if defined(__APPLE__)
	static mach_timebase_info_data_t timebaseInfo;
	
	if (timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}
	return mach_absolute_time() * timebaseInfo.numer / timebaseInfo.denom / 1000000000.0;
	
#elif defined(WIN32)
	static LARGE_INTEGER frequency;
	LARGE_INTEGER currentTime;
	
	if (frequency.QuadPart == 0) {
		QueryPerformanceFrequency(&frequency);
	}
	QueryPerformanceCounter(&currentTime);
	
	return (double) currentTime.QuadPart / frequency.QuadPart;
	
#elif defined(__linux)
	struct timespec currentTime;
	
	clock_gettime(CLOCK_MONOTONIC, &currentTime);
	return currentTime.tv_sec + currentTime.tv_nsec * 0.000000001;
#else
#error Unimplemented target OS
#endif
}
#endif

const char * Shell_getResourcePath() {
	if (StubShellCallback_getResourcePath != NULL) {
		return StubShellCallback_getResourcePath(StubShell_callbackContext);
	}
	return "";
}

const char * Shell_getSupportPath(const char * subdirectory) {
	if (StubShellCallback_getSupportPath != NULL) {
		return StubShellCallback_getSupportPath(StubShell_callbackContext, subdirectory);
	}
	return "";
}

unsigned int Shell_getDisplayCount() {
	if (StubShellCallback_getDisplayCount != NULL) {
		return StubShellCallback_getDisplayCount(StubShell_callbackContext);
	}
	return 0;
}

unsigned int Shell_getDisplayIndexFromWindow() {
	if (StubShellCallback_getDisplayIndexFromWindow != NULL) {
		return StubShellCallback_getDisplayIndexFromWindow(StubShell_callbackContext);
	}
	return 0;
}

void Shell_getDisplayBounds(unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight) {
	if (StubShellCallback_getDisplayBounds != NULL) {
		StubShellCallback_getDisplayBounds(StubShell_callbackContext, displayIndex, outOffsetX, outOffsetY, outWidth, outHeight);
	}
}

enum ShellBatteryState Shell_getBatteryState() {
	if (StubShellCallback_getBatteryState != NULL) {
		return StubShellCallback_getBatteryState(StubShell_callbackContext);
	}
	return ShellBatteryState_unknown;
}

float Shell_getBatteryLevel() {
	if (StubShellCallback_getBatteryLevel != NULL) {
		return StubShellCallback_getBatteryLevel(StubShell_callbackContext);
	}
	return -1.0f;
}

unsigned int Shell_setTimer(double interval, bool repeat, void (* callback)(unsigned int timerID, void * timerContext), void * timerContext) {
	if (StubShellCallback_setTimer != NULL) {
		return StubShellCallback_setTimer(StubShell_callbackContext, interval, repeat, callback, timerContext);
	}
	return 0;
}

void Shell_cancelTimer(unsigned int timerID) {
	if (StubShellCallback_cancelTimer != NULL) {
		StubShellCallback_cancelTimer(StubShell_callbackContext, timerID);
	}
}

void Shell_setCursorVisible(bool visible) {
	if (StubShellCallback_setCursorVisible != NULL) {
		StubShellCallback_setCursorVisible(StubShell_callbackContext, visible);
	}
}

void Shell_hideCursorUntilMouseMoves() {
	if (StubShellCallback_hideCursorUntilMouseMoves != NULL) {
		StubShellCallback_hideCursorUntilMouseMoves(StubShell_callbackContext);
	}
}

void Shell_setCursor(int cursor) {
	if (StubShellCallback_setCursor != NULL) {
		StubShellCallback_setCursor(StubShell_callbackContext, cursor);
	}
}

void Shell_setMouseDeltaMode(bool deltaMode) {
	if (StubShellCallback_setMouseDeltaMode != NULL) {
		StubShellCallback_setMouseDeltaMode(StubShell_callbackContext, deltaMode);
	}
}

void Shell_openURL(const char * url) {
	if (StubShellCallback_openURL != NULL) {
		StubShellCallback_openURL(StubShell_callbackContext, url);
	}
}

ShellThread Shell_createThread(int (* threadFunction)(void * context), void * context) {
	if (StubShellCallback_createThread != NULL) {
		return StubShellCallback_createThread(StubShell_callbackContext, threadFunction, context);
	}
	return NULL;
}

void Shell_exitThread(int statusCode) {
	if (StubShellCallback_exitThread != NULL) {
		StubShellCallback_exitThread(StubShell_callbackContext, statusCode);
	}
}

int Shell_joinThread(ShellThread thread) {
	if (StubShellCallback_joinThread != NULL) {
		return StubShellCallback_joinThread(StubShell_callbackContext, thread);
	}
	return 0;
}

ShellThread Shell_getCurrentThread() {
	if (StubShellCallback_getCurrentThread != NULL) {
		return StubShellCallback_getCurrentThread(StubShell_callbackContext);
	}
	return NULL;
}

ShellMutex Shell_createMutex() {
	if (StubShellCallback_createMutex != NULL) {
		return StubShellCallback_createMutex(StubShell_callbackContext);
	}
	return NULL;
}

void Shell_disposeMutex(ShellMutex mutex) {
	if (StubShellCallback_disposeMutex != NULL) {
		StubShellCallback_disposeMutex(StubShell_callbackContext, mutex);
	}
}

void Shell_lockMutex(ShellMutex mutex) {
	if (StubShellCallback_lockMutex != NULL) {
		StubShellCallback_lockMutex(StubShell_callbackContext, mutex);
	}
}

bool Shell_tryLockMutex(ShellMutex mutex) {
	if (StubShellCallback_tryLockMutex != NULL) {
		return StubShellCallback_tryLockMutex(StubShell_callbackContext, mutex);
	}
	return false;
}

void Shell_unlockMutex(ShellMutex mutex) {
	if (StubShellCallback_unlockMutex != NULL) {
		StubShellCallback_unlockMutex(StubShell_callbackContext, mutex);
	}
}

void Shell_setVSync(bool sync, bool fullscreen) {
	if (StubShellCallback_setVSync != NULL) {
		StubShellCallback_setVSync(StubShell_callbackContext, sync, fullscreen);
	}
}

bool Shell_openFileDialog(const char * basePath, char * outFilePath, unsigned int maxLength) {
	if (StubShellCallback_openFileDialog != NULL) {
		return StubShellCallback_openFileDialog(StubShell_callbackContext, basePath, outFilePath, maxLength);
	}
	return false;
}

bool Shell_saveFileDialog(const char * basePath, const char * baseName, char * outFilePath, unsigned int maxLength) {
	if (StubShellCallback_saveFileDialog != NULL) {
		return StubShellCallback_saveFileDialog(StubShell_callbackContext, basePath, baseName, outFilePath, maxLength);
	}
	return false;
}
