/*
  Copyright (c) 2012 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
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
bool (* StubShellCallback_setFullScreen)(void * context, bool fullScreen) = NULL;
double (* StubShellCallback_getCurrentTime)(void * context) = NULL;
const char * (* StubShellCallback_getResourcePath)(void * context) = NULL;
enum ShellBatteryState (* StubShellCallback_getBatteryState)(void * context) = NULL;
float (* StubShellCallback_getBatteryLevel)(void * context) = NULL;
unsigned int (* StubShellCallback_setTimer)(double interval, bool repeat, void (* callback)(unsigned int timerID, void * timerContext), void * timerContext, void * context) = NULL;
void (* StubShellCallback_cancelTimer)(unsigned int timerID, void * context) = NULL;

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

bool Shell_setFullScreen(bool fullScreen) {
	if (StubShellCallback_setFullScreen != NULL) {
		return StubShellCallback_setFullScreen(StubShell_callbackContext, fullScreen);
	}
	return true;
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
		return StubShellCallback_setTimer(interval, repeat, callback, timerContext, StubShell_callbackContext);
	}
	return 0;
}

void Shell_cancelTimer(unsigned int timerID) {
	if (StubShellCallback_cancelTimer != NULL) {
		StubShellCallback_cancelTimer(timerID, StubShell_callbackContext);
	}
}
