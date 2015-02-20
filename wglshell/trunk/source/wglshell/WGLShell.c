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

#include "glgraphics/GLGraphics.h"
#include "glgraphics/GLIncludes.h"
#include "shell/Shell.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"
#include "shell/ShellThreads.h"
#include "wglshell/WGLShell.h"
#include "wglshell/WGLShellCallbacks.h"
#include "wglshell/WGLTarget.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <PowrProf.h>
#include <commdlg.h>

struct WGLShellTimer {
	double interval;
	double nextFireTime;
	bool repeat;
	unsigned int id;
	void (* callback)(unsigned int timerID, void * context);
	void * context;
};

struct threadFuncInvocation {
	int (* threadFunction)(void * context);
	void * context;
};

#define VSYNC_DEFAULT_WINDOW true
#define VSYNC_DEFAULT_FULLSCREEN true

static HWND window;
static unsigned int buttonMask;
static unsigned int modifierFlags;
static bool isFullScreen;
static bool cursorHiddenByHide;
static bool cursorHiddenUntilMouseMoves;
static bool mouseDeltaMode;
static int restoreMouseX, restoreMouseY;
static int lastMouseX, lastMouseY;
static int ignoreX = INT_MAX, ignoreY = INT_MAX;
static bool vsyncWindow = VSYNC_DEFAULT_WINDOW, vsyncFullscreen = VSYNC_DEFAULT_FULLSCREEN;
static unsigned int nextTimerID;
static size_t timerCount;
static struct WGLShellTimer * timers;
static bool redisplayNeeded;
static HDC displayContext;
static HGLRC glContext;
static bool windowShown = false;
static int lastWidth, lastHeight;
static RECT oldWindowRect;
static DWORD oldWindowStyle;
static int accumulatedWheelDelta;

void Shell_mainLoop() {
	MSG message;
	unsigned int timerIndex, timerIndex2;
	double currentTime;
	
	for (;;) {
		while (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage(&message, NULL, 0, 0)) {
				return;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		
		currentTime = Shell_getCurrentTime();
		for (timerIndex = 0; timerIndex < timerCount; timerIndex++) {
			if (currentTime >= timers[timerIndex].nextFireTime) {
				timers[timerIndex].callback(timers[timerIndex].id, timers[timerIndex].context);
				if (timers[timerIndex].repeat) {
					timers[timerIndex].nextFireTime += timers[timerIndex].interval;
				} else {
					timerCount--;
					for (timerIndex2 = timerIndex; timerIndex2 < timerCount; timerIndex2++) {
						timers[timerIndex2] = timers[timerIndex2 + 1];
					}
					timerIndex--;
				}
			}
		}
		
		if (redisplayNeeded) {
			redisplayNeeded = false;
			RedrawWindow(window, NULL, NULL, RDW_NOERASE | RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_UPDATENOW);
		}
		
		// SwapBuffers for some reason doesn't appear to actually swap until the next GL call.
		// wglMakeCurrent isn't otherwise necessary here, but it forces a swap, which avoids a
		// potential delay later down the line at the next run loop iteration.
		wglMakeCurrent(displayContext, glContext);
		if (timerCount == 0 && !redisplayNeeded) {
			MsgWaitForMultipleObjects(0, NULL, FALSE, INFINITE, QS_ALLINPUT);
		}
	}
}

void Shell_redisplay() {
	redisplayNeeded = true;
}

bool Shell_isFullScreen() {
	return isFullScreen;
}

static void setVSync(bool sync) {
	const char * WINAPI (* wglGetExtensionsStringEXT)(void) = (const char * WINAPI (*)(void)) wglGetProcAddress("wglGetExtensionsStringEXT");
	if (strstr((char *) glGetString(GL_EXTENSIONS), "WGL_EXT_swap_control") && strstr(wglGetExtensionsStringEXT(), "WGL_EXT_swap_control")) {
		BOOL WINAPI (* wglSwapIntervalEXT)(int interval) = (BOOL WINAPI (*)(int)) wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT(1);
	}
}

#ifndef MONITOR_DEFAULTTONEAREST
#define MONITOR_DEFAULTTONEAREST 2
#endif
WINUSERAPI HMONITOR WINAPI MonitorFromRect(LPCRECT,DWORD);

struct monitorEnumProcGetMonitorAtIndexContext {
	unsigned int targetDisplayIndex;
	unsigned int displayIndex;
	HMONITOR hMonitor;
};

static BOOL CALLBACK monitorEnumProcGetMonitorAtIndex(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	struct monitorEnumProcGetMonitorAtIndexContext * context = (struct monitorEnumProcGetMonitorAtIndexContext *) dwData;
	
	if (context->displayIndex++ == context->targetDisplayIndex) {
		context->hMonitor = hMonitor;
	}
	return true;
}

bool Shell_enterFullScreen(unsigned int displayIndex) {
	DWORD windowStyle;
	HMONITOR monitor = NULL;
	MONITORINFO monitorInfo;
	RECT screenRect;
	struct monitorEnumProcGetMonitorAtIndexContext context = {displayIndex, 0, NULL};
	
	if (isFullScreen) {
		return true;
	}
	GetWindowRect(window, &oldWindowRect);
	if (EnumDisplayMonitors(NULL, NULL, monitorEnumProcGetMonitorAtIndex, (LPARAM) &context)) {
		monitor = context.hMonitor;
	} else {
		monitor = MonitorFromRect(&oldWindowRect, MONITOR_DEFAULTTONEAREST);
	}
	if (monitor == NULL) {
		return false;
	}
	
	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &monitorInfo);
	
	oldWindowStyle = windowStyle = GetWindowLong(window, GWL_STYLE);
	windowStyle &= ~WS_OVERLAPPEDWINDOW;
	windowStyle |= WS_POPUP;
	SetWindowLong(window, GWL_STYLE, windowStyle);
	screenRect = monitorInfo.rcMonitor;
	SetWindowPos(window, HWND_TOP, screenRect.left, screenRect.top, screenRect.right - screenRect.left, screenRect.bottom - screenRect.top, SWP_NOZORDER | SWP_FRAMECHANGED);
	isFullScreen = true;
	setVSync(vsyncFullscreen);
	
	return true;
}

void Shell_exitFullScreen() {
	if (!isFullScreen) {
		return;
	}
	SetWindowLong(window, GWL_STYLE, oldWindowStyle);
	SetWindowPos(window, HWND_TOP, oldWindowRect.left, oldWindowRect.top, oldWindowRect.right - oldWindowRect.left, oldWindowRect.bottom - oldWindowRect.top, SWP_NOZORDER | SWP_FRAMECHANGED);
	isFullScreen = false;
	setVSync(vsyncWindow);
}

double Shell_getCurrentTime() {
	static LARGE_INTEGER frequency;
	LARGE_INTEGER currentTime;
	
	if (frequency.QuadPart == 0) {
		QueryPerformanceFrequency(&frequency);
	}
	QueryPerformanceCounter(&currentTime);
	
	return (double) currentTime.QuadPart / frequency.QuadPart;
}

const char * Shell_getResourcePath() {
	return "Resources";
}

const char * Shell_getSupportPath(const char * subdirectory) {
	static char supportPath[PATH_MAX];
	
	if (subdirectory == NULL) {
		strncpy(supportPath, getenv("APPDATA"), PATH_MAX);
	} else {
		snprintf(supportPath, PATH_MAX, "%s/%s", getenv("APPDATA"), subdirectory);
	}
	_mkdir(supportPath);
	return supportPath;
}

void Shell_getMainScreenSize(unsigned int * outWidth, unsigned int * outHeight) {
	if (outWidth != NULL) {
		*outWidth = GetSystemMetrics(SM_CXSCREEN);
	}
	if (outHeight != NULL) {
		*outHeight = GetSystemMetrics(SM_CYSCREEN);
	}
}

unsigned int Shell_getDisplayCount() {
	return GetSystemMetrics(SM_CMONITORS);
}

struct monitorEnumProcGetIndexContext {
	HMONITOR hMonitor;
	unsigned int displayIndex;
	bool found;
};

static BOOL CALLBACK monitorEnumProcGetIndex(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	struct monitorEnumProcGetIndexContext * context = (struct monitorEnumProcGetIndexContext *) dwData;
	
	if (!context->found) {
		if (hMonitor == context->hMonitor) {
			context->found = true;
		} else {
			context->displayIndex++;
		}
	}
	return true;
}

unsigned int Shell_getDisplayIndexFromWindow() {
	BOOL success;
	struct monitorEnumProcGetIndexContext context = {NULL, 0, false};
	RECT windowRect;
	
	GetWindowRect(window, &windowRect);
	context.hMonitor = MonitorFromRect(&windowRect, MONITOR_DEFAULTTONEAREST);
	success = EnumDisplayMonitors(NULL, NULL, monitorEnumProcGetIndex, (LPARAM) &context);
	if (success && context.found) {
		return context.displayIndex;
	}
	return 0;
}

struct monitorEnumProcGetBoundsContext {
	unsigned int targetDisplayIndex;
	unsigned int displayIndex;
	bool found;
	RECT bounds;
};

static BOOL CALLBACK monitorEnumProcGetBounds(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	struct monitorEnumProcGetBoundsContext * context = (struct monitorEnumProcGetBoundsContext *) dwData;
	
	if (context->displayIndex++ == context->targetDisplayIndex) {
		context->bounds = *lprcMonitor;
		context->found = true;
	}
	return true;
}

void Shell_getDisplayBounds(unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight) {
	BOOL success;
	struct monitorEnumProcGetBoundsContext context = {displayIndex, 0, false, {0, 0, 0, 0}};
	
	success = EnumDisplayMonitors(NULL, NULL, monitorEnumProcGetBounds, (LPARAM) &context);
	if (success && context.found) {
		if (outOffsetX != NULL) {
			*outOffsetX = context.bounds.left;
		}
		if (outOffsetY != NULL) {
			*outOffsetY = context.bounds.top;
		}
		if (outWidth != NULL) {
			*outWidth = context.bounds.right - context.bounds.left;
		}
		if (outHeight != NULL) {
			*outHeight = context.bounds.bottom - context.bounds.top;
		}
	}
}

static BOOL CALLBACK monitorEnumProcGetWorkArea(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	struct monitorEnumProcGetBoundsContext * context = (struct monitorEnumProcGetBoundsContext *) dwData;
	
	if (context->displayIndex++ == context->targetDisplayIndex) {
		MONITORINFO monitorInfo;
		
		monitorInfo.cbSize = sizeof(monitorInfo);
		if (GetMonitorInfo(hMonitor, &monitorInfo)) {
			context->bounds = monitorInfo.rcWork;
			context->found = true;
		}
	}
	return true;
}

void Shell_getSafeWindowRect(unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight) {
	BOOL success;
	struct monitorEnumProcGetBoundsContext context = {displayIndex, 0, false, {0, 0, 0, 0}};
	
	success = EnumDisplayMonitors(NULL, NULL, monitorEnumProcGetWorkArea, (LPARAM) &context);
	if (success && context.found) {
		RECT adjustedRect = context.bounds;
		AdjustWindowRect(&adjustedRect, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_OVERLAPPEDWINDOW, false);
		context.bounds.left -= adjustedRect.left - context.bounds.left;
		context.bounds.top -= adjustedRect.top - context.bounds.top;
		context.bounds.right -= adjustedRect.right - context.bounds.right;
		context.bounds.bottom -= adjustedRect.bottom - context.bounds.bottom;
		if (outOffsetX != NULL) {
			*outOffsetX = context.bounds.left;
		}
		if (outOffsetY != NULL) {
			*outOffsetY = context.bounds.top;
		}
		if (outWidth != NULL) {
			*outWidth = context.bounds.right - context.bounds.left;
		}
		if (outHeight != NULL) {
			*outHeight = context.bounds.bottom - context.bounds.top;
		}
	}
}

unsigned int Shell_setTimer(double interval, bool repeat, void (* callback)(unsigned int timerID, void * context), void * context) {
	timers = realloc(timers, sizeof(struct WGLShellTimer) * (timerCount + 1));
	timers[timerCount].interval = interval;
	timers[timerCount].nextFireTime = Shell_getCurrentTime() + interval;
	timers[timerCount].repeat = repeat;
	timers[timerCount].id = nextTimerID++;
	timers[timerCount].callback = callback;
	timers[timerCount].context = context;
	PostMessage(window, WM_USER, 0, 0); // Wake up run loop in case it's blocked on GetMessage
	return timers[timerCount++].id;
}

void Shell_cancelTimer(unsigned int timerID) {
	unsigned int timerIndex;
	
	for (timerIndex = 0; timerIndex < timerCount; timerIndex++) {
		if (timers[timerIndex].id == timerID) {
			timerCount--;
			for (; timerIndex < timerCount; timerIndex++) {
				timers[timerIndex] = timers[timerIndex + 1];
			}
			break;
		}
	}
}

void Shell_setCursorVisible(bool visible) {
	if (visible && cursorHiddenByHide) {
		cursorHiddenByHide = false;
		if (!mouseDeltaMode) {
			ShowCursor(true);
		}
	} else if (!visible && !cursorHiddenByHide) {
		cursorHiddenByHide = true;
		ShowCursor(false);
	}
}

void Shell_hideCursorUntilMouseMoves() {
	if (!cursorHiddenUntilMouseMoves) {
		cursorHiddenUntilMouseMoves = true;
		ShowCursor(false);
	}
}

#ifndef GCL_HCURSOR
#define GCL_HCURSOR -12
#endif

void Shell_setCursor(int value) {
	HCURSOR cursor = NULL;
	
	switch (value) {
		case ShellCursor_arrow:
			cursor = LoadCursor(NULL, IDC_ARROW);
			break;
		case ShellCursor_iBeam:
			cursor = LoadCursor(NULL, IDC_IBEAM);
			break;
		case ShellCursor_crosshair:
			cursor = LoadCursor(NULL, IDC_CROSS);
			break;
		case ShellCursor_hand:
			cursor = LoadCursor(NULL, IDC_HAND);
			break;
		case ShellCursor_wait:
			cursor = LoadCursor(NULL, IDC_WAIT);
			break;
		case WGLShellCursor_appStarting:
			cursor = LoadCursor(NULL, IDC_APPSTARTING);
			break;
		case WGLShellCursor_no:
			cursor = LoadCursor(NULL, IDC_NO);
			break;
		case WGLShellCursor_sizeAll:
			cursor = LoadCursor(NULL, IDC_SIZEALL);
			break;
		case WGLShellCursor_sizeNESW:
			cursor = LoadCursor(NULL, IDC_SIZENESW);
			break;
		case WGLShellCursor_sizeNS:
			cursor = LoadCursor(NULL, IDC_SIZENS);
			break;
		case WGLShellCursor_sizeNWSE:
			cursor = LoadCursor(NULL, IDC_SIZENWSE);
			break;
		case WGLShellCursor_sizeWE:
			cursor = LoadCursor(NULL, IDC_SIZEWE);
			break;
		case WGLShellCursor_upArrow:
			cursor = LoadCursor(NULL, IDC_UPARROW);
			break;
		case WGLShellCursor_help:
			cursor = LoadCursor(NULL, IDC_HELP);
			break;
	}
	if (cursor != NULL) {
		SetClassLongPtr(window, GCL_HCURSOR, (LONG_PTR) cursor);
		SetCursor(cursor);
	}
}

static void warpPointerAndIgnoreEvent(int x, int y) {
	POINT point = {x, y};
	
	ignoreX = x;
	ignoreY = y;
	lastMouseX = x;
	lastMouseY = y;
	ClientToScreen(window, &point);
	SetCursorPos(point.x, point.y);
}

static int getWindowCenterX() {
	RECT rect;
	GetClientRect(window, &rect);
	return (rect.right - rect.left) / 2;
}

static int getWindowCenterY() {
	RECT rect;
	GetClientRect(window, &rect);
	return (rect.bottom - rect.top) / 2;
}

void Shell_setMouseDeltaMode(bool deltaMode) {
	if (!mouseDeltaMode && deltaMode) {
		restoreMouseX = lastMouseX;
		restoreMouseY = lastMouseY;
		warpPointerAndIgnoreEvent(getWindowCenterX(), getWindowCenterY());
		ShowCursor(false);
		mouseDeltaMode = true;
		
	} else if (mouseDeltaMode && !deltaMode) {
		warpPointerAndIgnoreEvent(restoreMouseX, restoreMouseY);
		mouseDeltaMode = false;
		if (!cursorHiddenByHide && !cursorHiddenUntilMouseMoves) {
			ShowCursor(true);
		}
	}
}

void Shell_openURL(const char * url) {
	ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

enum ShellBatteryState Shell_getBatteryState() {
	SYSTEM_POWER_STATUS powerStatus;
	SYSTEM_POWER_CAPABILITIES powerCapabilities;
	
	if (GetPwrCapabilities(&powerCapabilities) && !powerCapabilities.SystemBatteriesPresent) {
		return ShellBatteryState_notBatteryPowered;
	}
	if (GetSystemPowerStatus(&powerStatus)) {
		if (powerStatus.ACLineStatus == 0) {
			return ShellBatteryState_unplugged;
		}
		if (powerStatus.ACLineStatus == 1) {
			if (powerStatus.BatteryFlag & 0x80) {
				return ShellBatteryState_batteryMissing;
			}
			if (powerStatus.BatteryFlag & 0x08) {
				return ShellBatteryState_charging;
			}
			return ShellBatteryState_full;
		}
	}
	return ShellBatteryState_unknown;
}

float Shell_getBatteryLevel() {
	SYSTEM_POWER_STATUS powerStatus;
	
	if (GetSystemPowerStatus(&powerStatus) && powerStatus.BatteryLifePercent <= 100) {
		return powerStatus.BatteryLifePercent / 100.0f;
	}
	return -1.0f;
}

static DWORD WINAPI callThreadFunc(LPVOID context) {
	struct threadFuncInvocation * invocation = context;
	int (* threadFunction)(void * context);
	void * threadContext;
	
	threadFunction  = invocation->threadFunction;
	threadContext = invocation->context;
	free(invocation);
	return (DWORD) threadFunction(threadContext);
}

ShellThread Shell_createThread(int (* threadFunction)(void * context), void * context) {
	HANDLE thread;
	struct threadFuncInvocation * invocation;
	
	invocation = malloc(sizeof(struct threadFuncInvocation));
	invocation->threadFunction = threadFunction;
	invocation->context = context;
	thread = CreateThread(NULL, 0, callThreadFunc, invocation, 0, NULL);
	return thread;
}

void Shell_detachThread(ShellThread thread) {
	CloseHandle(thread);
}

int Shell_joinThread(ShellThread thread) {
	DWORD status;
	DWORD returnValue = -1;
	
	status = WaitForSingleObject(thread, INFINITE);
	if (status != WAIT_FAILED) {
		GetExitCodeThread(thread, &returnValue);
		return returnValue;
	}
	return status;
}

void Shell_exitThread(int statusCode) {
	ExitThread(statusCode);
}

ShellThread Shell_getCurrentThread() {
	HANDLE duplicate = NULL;
	
	if (DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &duplicate, 0, true, DUPLICATE_SAME_ACCESS)) {
		CloseHandle(&duplicate);
	}
	return duplicate;
}

ShellMutex Shell_createMutex() {
	return CreateMutex(NULL, false, NULL);
}

void Shell_disposeMutex(ShellMutex mutex) {
	CloseHandle(mutex);
}

void Shell_lockMutex(ShellMutex mutex) {
	WaitForSingleObject(mutex, INFINITE);
}

bool Shell_tryLockMutex(ShellMutex mutex) {
	return WaitForSingleObject(mutex, 0) != WAIT_FAILED;
}

void Shell_unlockMutex(ShellMutex mutex) {
	ReleaseMutex(mutex);
}

#define SEMAPHORE_MAX 32767

ShellSemaphore Shell_createSemaphore(unsigned int value) {
	return CreateSemaphore(NULL, value > SEMAPHORE_MAX ? SEMAPHORE_MAX : value, SEMAPHORE_MAX, NULL);
}

void Shell_disposeSemaphore(ShellSemaphore semaphore) {
	CloseHandle(semaphore);
}

void Shell_postSemaphore(ShellSemaphore semaphore) {
	ReleaseSemaphore(semaphore, 1, NULL);
}

void Shell_waitSemaphore(ShellSemaphore semaphore) {
	WaitForSingleObject(semaphore, INFINITE);
}

bool Shell_tryWaitSemaphore(ShellSemaphore semaphore) {
	return WaitForSingleObject(semaphore, 0) != WAIT_TIMEOUT;
}

void Shell_setVSync(bool sync, bool fullscreen) {
	if (fullscreen) {
		vsyncFullscreen = sync;
		if (isFullScreen) {
			setVSync(sync);
		}
		
	} else {
		vsyncWindow = sync;
		if (!isFullScreen) {
			setVSync(sync);
		}
	}
}

bool Shell_openFileDialog(const char * basePath, char * outFilePath, unsigned int maxLength) {
	OPENFILENAME openFileName;
	BOOL success;
	
	ZeroMemory(&openFileName, sizeof(openFileName));
	openFileName.lStructSize = sizeof(openFileName);
	openFileName.hwndOwner = window;
	openFileName.lpstrFile = outFilePath;
	openFileName.lpstrFile[0] = '\0';
	openFileName.nMaxFile = maxLength;
	openFileName.lpstrInitialDir = basePath;
	openFileName.Flags |= OFN_NOCHANGEDIR;
	
	success = GetOpenFileName(&openFileName);
	return success;
}

bool Shell_saveFileDialog(const char * basePath, const char * baseName, char * outFilePath, unsigned int maxLength) {
	OPENFILENAME openFileName;
	BOOL success;
	
	ZeroMemory(&openFileName, sizeof(openFileName));
	openFileName.lStructSize = sizeof(openFileName);
	openFileName.hwndOwner = window;
	if (baseName == NULL) {
		outFilePath[0] = '\0';
	} else {
		strncpy(outFilePath, baseName, maxLength);
	}
	openFileName.lpstrFile = outFilePath;
	openFileName.nMaxFile = maxLength;
	openFileName.lpstrInitialDir = basePath;
	openFileName.Flags |= OFN_NOCHANGEDIR;
	
	success = GetSaveFileName(&openFileName);
	if (baseName != NULL) {
		free(openFileName.lpstrFileTitle);
	}
	return success;
}

void Shell_systemBeep() {
	MessageBeep(MB_OK);
}

void WGLShell_redirectStdoutToFile(const char * path) {
	freopen(path, "a", stdout);
}

static unsigned int windowsVKToShellKeyCode(UINT vk) {
	switch (vk) {
		case 27:
			return KEYBOARD_ESCAPE;
		case 112:
			return KEYBOARD_F1;
		case 113:
			return KEYBOARD_F2;
		case 114:
			return KEYBOARD_F3;
		case 115:
			return KEYBOARD_F4;
		case 116:
			return KEYBOARD_F5;
		case 117:
			return KEYBOARD_F6;
		case 118:
			return KEYBOARD_F7;
		case 119:
			return KEYBOARD_F8;
		case 120:
			return KEYBOARD_F9;
		case 121:
			return KEYBOARD_F10;
		case 122:
			return KEYBOARD_F11;
		case 123:
			return KEYBOARD_F12;
		case 145:
			return KEYBOARD_SCROLL_LOCK;
		case 144:
			return KEYPAD_NUM_LOCK;
		case 192:
			return KEYBOARD_GRAVE_ACCENT_AND_TILDE;
		case 49:
			return KEYBOARD_1;
		case 50:
			return KEYBOARD_2;
		case 51:
			return KEYBOARD_3;
		case 52:
			return KEYBOARD_4;
		case 53:
			return KEYBOARD_5;
		case 54:
			return KEYBOARD_6;
		case 55:
			return KEYBOARD_7;
		case 56:
			return KEYBOARD_8;
		case 57:
			return KEYBOARD_9;
		case 48:
			return KEYBOARD_0;
		case 189:
			return KEYBOARD_HYPHEN;
		case 187:
			return KEYBOARD_EQUAL_SIGN;
		case 8:
			return KEYBOARD_DELETE_OR_BACKSPACE;
		case 9:
			return KEYBOARD_TAB;
		case 81:
			return KEYBOARD_Q;
		case 87:
			return KEYBOARD_W;
		case 69:
			return KEYBOARD_E;
		case 82:
			return KEYBOARD_R;
		case 84:
			return KEYBOARD_T;
		case 89:
			return KEYBOARD_Y;
		case 85:
			return KEYBOARD_U;
		case 73:
			return KEYBOARD_I;
		case 79:
			return KEYBOARD_O;
		case 80:
			return KEYBOARD_P;
		case 219:
			return KEYBOARD_OPEN_BRACKET;
		case 221:
			return KEYBOARD_CLOSE_BRACKET;
		case 220:
			return KEYBOARD_BACKSLASH;
		case 20:
			return KEYBOARD_CAPS_LOCK;
		case 65:
			return KEYBOARD_A;
		case 83:
			return KEYBOARD_S;
		case 68:
			return KEYBOARD_D;
		case 70:
			return KEYBOARD_F;
		case 71:
			return KEYBOARD_G;
		case 72:
			return KEYBOARD_H;
		case 74:
			return KEYBOARD_J;
		case 75:
			return KEYBOARD_K;
		case 76:
			return KEYBOARD_L;
		case 186:
			return KEYBOARD_SEMICOLON;
		case 222:
			return KEYBOARD_QUOTE;
		case 13:
			return KEYBOARD_RETURN_OR_ENTER;
		case 160:
			return KEYBOARD_LEFT_SHIFT;
		case 90:
			return KEYBOARD_Z;
		case 88:
			return KEYBOARD_X;
		case 67:
			return KEYBOARD_C;
		case 86:
			return KEYBOARD_V;
		case 66:
			return KEYBOARD_B;
		case 78:
			return KEYBOARD_N;
		case 77:
			return KEYBOARD_M;
		case 188:
			return KEYBOARD_COMMA;
		case 190:
			return KEYBOARD_PERIOD;
		case 191:
			return KEYBOARD_SLASH;
		case 161:
			return KEYBOARD_RIGHT_SHIFT;
		case 162:
			return KEYBOARD_LEFT_CONTROL;
		case 164:
			return KEYBOARD_LEFT_ALT;
		case 241:
			return KEYBOARD_LEFT_GUI;
		case 32:
			return KEYBOARD_SPACEBAR;
		case 234:
			return KEYBOARD_RIGHT_GUI;
		case 249:
			return KEYBOARD_MENU;
		case 45:
			return KEYBOARD_INSERT;
		case 36:
			return KEYBOARD_HOME;
		case 33:
			return KEYBOARD_PAGE_UP;
		case 46:
			return KEYBOARD_DELETE_FORWARD;
		case 35:
			return KEYBOARD_END;
		case 34:
			return KEYBOARD_PAGE_DOWN;
		case 37:
			return KEYBOARD_LEFT_ARROW;
		case 38:
			return KEYBOARD_UP_ARROW;
		case 39:
			return KEYBOARD_RIGHT_ARROW;
		case 40:
			return KEYBOARD_DOWN_ARROW;
		case 12:
			return KEYPAD_EQUAL_SIGN;
		case 106:
			return KEYPAD_ASTERISK;
		case 109:
			return KEYPAD_HYPHEN;
		case 107:
			return KEYPAD_PLUS;
#ifdef DEBUG
		default:
			fprintf(stderr, "Warning: Unknown key code %u\n", vk);
#endif
	}
	return 0;
}

#ifndef MAPVK_VSC_TO_VK_EX
#define MAPVK_VSC_TO_VK_EX 3
#endif

static unsigned int lParamToShellKeyCode(LPARAM lParam) {
	static HKL layout;
	
	if (layout == NULL) {
		layout = LoadKeyboardLayout("00000409", 0);
	}
	return windowsVKToShellKeyCode(MapVirtualKeyEx(lParam >> 16 & 0x7F, MAPVK_VSC_TO_VK_EX, layout));
}

static void updateModifierFlags() {
	bool capsLock, shift, control, alt;
	unsigned int oldModifierFlags = modifierFlags;
	
	capsLock = GetKeyState(VK_CAPITAL) & 0x01;
	if (capsLock && !(modifierFlags & MODIFIER_CAPS_LOCK_BIT)) {
		modifierFlags |= MODIFIER_CAPS_LOCK_BIT;
	} else if (!capsLock && (modifierFlags & MODIFIER_CAPS_LOCK_BIT)) {
		modifierFlags &= ~MODIFIER_CAPS_LOCK_BIT;
	}
	
	shift = !!(GetKeyState(VK_SHIFT) & 0x8000);
	if (shift && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		modifierFlags |= MODIFIER_SHIFT_BIT;
	} else if (!shift && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		modifierFlags &= ~MODIFIER_SHIFT_BIT;
	}
	
	control = !!(GetKeyState(VK_CONTROL) & 0x8000);
	if (control && !(modifierFlags & MODIFIER_CONTROL_BIT)) {
		modifierFlags |= MODIFIER_CONTROL_BIT;
	} else if (!control && (modifierFlags & MODIFIER_CONTROL_BIT)) {
		modifierFlags &= ~MODIFIER_CONTROL_BIT;
	}
	
	alt = !!(GetKeyState(VK_MENU) & 0x8000);
	if (alt && !(modifierFlags & MODIFIER_ALT_BIT)) {
		modifierFlags |= MODIFIER_ALT_BIT;
	} else if (!alt && (modifierFlags & MODIFIER_ALT_BIT)) {
		modifierFlags &= ~MODIFIER_ALT_BIT;
	}
	
	if (modifierFlags != oldModifierFlags) {
		if (keyModifiersChangedCallback != NULL) {
			keyModifiersChangedCallback(modifierFlags);
		}
	}
}

static LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_PAINT: {
#ifdef DEBUG
			GLenum error;
#endif
			
			ValidateRect(window, NULL);
			if (drawCallback != NULL) {
				drawCallback();
			}
			
#ifdef DEBUG
			while ((error = glGetError()) != GL_NO_ERROR) {
				fprintf(stderr, "GL error: 0x%X\n", error);
			}
#endif
			SwapBuffers(displayContext);
			return 0;
		}
		
		case WM_SIZING:
		case WM_SIZE: {
			RECT rect;
			
			GetClientRect(window, &rect);
			if ((int) (rect.right - rect.left) != lastWidth || (int) (rect.bottom - rect.top) != lastHeight) {
				lastWidth = rect.right - rect.left;
				lastHeight = rect.bottom - rect.top;
				glViewport(0, 0, lastWidth, lastHeight);
				if (resizeCallback != NULL) {
					resizeCallback(lastWidth, lastHeight);
				}
				redisplayNeeded = true;
			}
			return 0;
		}
		
		case WM_LBUTTONDOWN:
			buttonMask |= 1 << 0;
			if (mouseDownCallback != NULL) {
				mouseDownCallback(0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			return 0;
			
		case WM_LBUTTONUP:
			buttonMask &= ~(1 << 0);
			if (mouseUpCallback != NULL) {
				mouseUpCallback(0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			return 0;
			
		case WM_RBUTTONDOWN:
			buttonMask |= 1 << 1;
			if (mouseDownCallback != NULL) {
				mouseDownCallback(1, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			return 0;
			
		case WM_RBUTTONUP:
			buttonMask &= ~(1 << 1);
			if (mouseUpCallback != NULL) {
				mouseUpCallback(1, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			return 0;
			
		case WM_MBUTTONDOWN:
			buttonMask |= 1 << 2;
			if (mouseDownCallback != NULL) {
				mouseDownCallback(2, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			return 0;
			
		case WM_MBUTTONUP:
			buttonMask &= ~(1 << 2);
			if (mouseDownCallback != NULL) {
				mouseUpCallback(2, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			return 0;
			
		case WM_MOUSEMOVE: {
			int x, y, reportedX, reportedY;
			
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);
			if ((x == lastMouseX && y == lastMouseY) || (x == ignoreX && y == ignoreY)) {
				return 0;
			}
			
			if (cursorHiddenUntilMouseMoves) {
				cursorHiddenUntilMouseMoves = false;
				if (!mouseDeltaMode) {
					ShowCursor(true);
				}
			}
			if (mouseDeltaMode) {
				reportedX = x - lastMouseX;
				reportedY = y - lastMouseY;
			} else {
				reportedX = x;
				reportedY = y;
			}
			lastMouseX = x;
			lastMouseY = y;
			if (buttonMask != 0) {
				if (mouseDraggedCallback != NULL) {
					mouseDraggedCallback(buttonMask, reportedX, reportedY);
				}
			} else {
				if (mouseMovedCallback != NULL) {
					mouseMovedCallback(reportedX, reportedY);
				}
			}
			ignoreX = ignoreY = INT_MAX;
			if (mouseDeltaMode) {
				warpPointerAndIgnoreEvent(getWindowCenterX(), getWindowCenterY());
			}
			return 0;
		}
		
		case WM_MOUSEWHEEL:
			accumulatedWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
			if (accumulatedWheelDelta / WHEEL_DELTA != 0) {
				int deltaX = 0;
				int deltaY = accumulatedWheelDelta / WHEEL_DELTA;
				accumulatedWheelDelta -= deltaY * WHEEL_DELTA;
				
				if (modifierFlags & MODIFIER_SHIFT_BIT) {
					deltaX = deltaY;
					deltaY = 0;
				}
				
				if (scrollWheelCallback != NULL) {
					scrollWheelCallback(-deltaX, -deltaY);
				}
			}
			break;
			
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			MSG msg;
			unsigned int charCode = 0, keyCode;
			
			updateModifierFlags();
			
			if (PeekMessage(&msg, window, WM_KEYFIRST, WM_KEYLAST, PM_NOREMOVE) && msg.message == WM_CHAR) {
				charCode = msg.wParam;
			}
			keyCode = lParamToShellKeyCode(lParam);
			
			if (keyCode == KEYBOARD_F4 && ((modifierFlags & MODIFIER_ALT_BIT) || (modifierFlags & MODIFIER_CONTROL_BIT))) {
				if (confirmQuitCallback == NULL || confirmQuitCallback()) {
					exit(EXIT_SUCCESS);
				}
			}
			
			if (keyCode != 0 && keyDownCallback != NULL) {
				keyDownCallback(charCode, keyCode, modifierFlags, !!(HIWORD(lParam) & KF_REPEAT));
			}
			if (keyCode == KEYBOARD_LEFT_SHIFT || keyCode == KEYBOARD_RIGHT_SHIFT) {
				if (!(modifierFlags & MODIFIER_SHIFT_BIT)) {
					modifierFlags |= MODIFIER_SHIFT_BIT;
					if (keyModifiersChangedCallback != NULL) {
						keyModifiersChangedCallback(modifierFlags);
					}
				}
				
			} else if (keyCode == KEYBOARD_LEFT_CONTROL || keyCode == KEYBOARD_RIGHT_CONTROL) {
				if (!(modifierFlags & MODIFIER_CONTROL_BIT)) {
					modifierFlags |= MODIFIER_CONTROL_BIT;
					if (keyModifiersChangedCallback != NULL) {
						keyModifiersChangedCallback(modifierFlags);
					}
				}
				
			} else if (keyCode == KEYBOARD_LEFT_ALT || keyCode == KEYBOARD_RIGHT_ALT) {
				if (!(modifierFlags & MODIFIER_ALT_BIT)) {
					modifierFlags |= MODIFIER_ALT_BIT;
					if (keyModifiersChangedCallback != NULL) {
						keyModifiersChangedCallback(modifierFlags);
					}
				}
			}
			return 0;
		}
		
		case WM_SYSCHAR:
			// Must trap WM_SYSCHAR to prevent beeping from alt+key presses which are handled by WM_SYSKEYDOWN
			return 0;
			
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			unsigned int keyCode;
			
			updateModifierFlags();
			
			keyCode = lParamToShellKeyCode(lParam);
			if (keyCode != 0 && keyUpCallback != NULL) {
				keyUpCallback(keyCode, modifierFlags);
			}
			if (keyCode == KEYBOARD_LEFT_SHIFT || keyCode == KEYBOARD_RIGHT_SHIFT) {
				if (modifierFlags & MODIFIER_SHIFT_BIT) {
					modifierFlags &= ~MODIFIER_SHIFT_BIT;
					if (keyModifiersChangedCallback != NULL) {
						keyModifiersChangedCallback(modifierFlags);
					}
				}
				
			} else if (keyCode == KEYBOARD_LEFT_CONTROL || keyCode == KEYBOARD_RIGHT_CONTROL) {
				if (modifierFlags & MODIFIER_CONTROL_BIT) {
					modifierFlags &= ~MODIFIER_CONTROL_BIT;
					if (keyModifiersChangedCallback != NULL) {
						keyModifiersChangedCallback(modifierFlags);
					}
				}
				
			} else if (keyCode == KEYBOARD_LEFT_ALT || keyCode == KEYBOARD_RIGHT_ALT) {
				if (modifierFlags & MODIFIER_ALT_BIT) {
					modifierFlags &= ~MODIFIER_ALT_BIT;
					if (keyModifiersChangedCallback != NULL) {
						keyModifiersChangedCallback(modifierFlags);
					}
				}
			}
			return 0;
		}
		
		case WM_ACTIVATEAPP:
			if (wParam) {
				if (foregroundedCallback != NULL) {
					foregroundedCallback();
				}
			} else {
				if (backgroundedCallback != NULL) {
					backgroundedCallback();
				}
			}
			break;
			
		case WM_CLOSE:
			if (confirmQuitCallback != NULL && !confirmQuitCallback()) {
				return 0;
			}
			break;
			
		case WM_DESTROY:
			if (windowShown) {
				PostQuitMessage(0);
			}
			return 0;
			
		case WM_USER:
			return 0;
	}
	return DefWindowProc(window, message, wParam, lParam);
}

static HWND createShellWindow(const char * windowTitle, RECT windowRect, HINSTANCE instance) {
	HWND window = CreateWindow("wglshell",
	                           windowTitle,
	                           WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_OVERLAPPEDWINDOW,
	                           windowRect.left,
	                           windowRect.top,
	                           windowRect.right - windowRect.left,
	                           windowRect.bottom - windowRect.top,
	                           NULL,
	                           NULL,
	                           instance,
	                           NULL);
	if (window == NULL) {
		fprintf(stderr, "CreateWindow failed! GetLastError(): %ld\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	return window;
}

static void destroyGLContext() {
	wglDeleteContext(glContext);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR commandLine, int command) {
	struct WGLShellConfiguration configuration;
	WNDCLASSEX windowClass;
	ATOM result;
	PIXELFORMATDESCRIPTOR pixelFormat = {
		sizeof(PIXELFORMATDESCRIPTOR),           // nSize
		1,                                       // nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL, // dwFlags
		PFD_TYPE_RGBA,                           // iPixelType
		32,                                      // cColorBits
		0, 0, 0, 0, 0, 0,                        // c(Red|Green|Blue)(Shift|Bits)
		0,                                       // cAlphaBits
		0,                                       // cAlphaShift
		0,                                       // cAccumBits
		0, 0, 0, 0,                              // cAccum(Red|Green|Blue|Alpha)Bits
		24,                                      // cDepthBits
		8,                                       // cStencilBits
		0,                                       // cAuxBuffers
		PFD_MAIN_PLANE,                          // iLayerType
		0,                                       // bReserved
		0, 0, 0                                  // dw(Layer|Visible|Damage)Mask
	};
	int format;
	const unsigned char * glExtensions;
	int argc, argIndex;
	LPWSTR * argvW;
	const char ** argv;
	char * arg;
	size_t lengthUTF8;
	RECT windowRect;
	
	configuration.windowX = 20;
	configuration.windowY = 50;
	configuration.windowWidth = 800;
	configuration.windowHeight = 600;
	configuration.windowTitle = "Change configuration->windowTitle in WGLTarget_configure()";
	configuration.iconResource = 101;
	configuration.displayMode.doubleBuffer = true;
	configuration.displayMode.colorBits = 24;
	configuration.displayMode.alphaBits = 8;
	configuration.displayMode.depthBuffer = false;
	configuration.displayMode.depthBits = 24;
	configuration.displayMode.stencilBuffer = false;
	configuration.displayMode.stencilBits = 8;
	configuration.displayMode.accumBuffer = false;
	configuration.displayMode.accumBits = 32;
	configuration.displayMode.multisample = false;
	configuration.displayMode.sampleBuffers = 1;
	configuration.displayMode.samples = 4;
	
	argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
	argv = malloc(sizeof(const char *) * argc);
	for (argIndex = 0; argIndex < argc; argIndex++) {
		lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, argvW[argIndex], -1, NULL, 0, NULL, NULL);
		arg = malloc(lengthUTF8 + 1);
		WideCharToMultiByte(CP_UTF8, 0, argvW[argIndex], -1, arg, lengthUTF8 + 1, NULL, NULL);
		argv[argIndex] = arg;
	}
	LocalFree(argvW);
	WGLTarget_configure(instance, prevInstance, commandLine, command, argc, argv, &configuration);
	
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = windowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = instance;
	windowClass.hIcon = LoadIcon(instance, MAKEINTRESOURCE(configuration.iconResource));
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "wglshell";
	windowClass.hIconSm = NULL;
	result = RegisterClassEx(&windowClass);
	if (result == 0) {
		fprintf(stderr, "RegisterClassEx failed! GetLastError(): %ld\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	
	windowRect.left = configuration.windowX;
	windowRect.top = configuration.windowY;
	windowRect.right = windowRect.left + configuration.windowWidth;
	windowRect.bottom = windowRect.top + configuration.windowHeight;
	AdjustWindowRect(&windowRect, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_OVERLAPPEDWINDOW, false);
	
	window = createShellWindow(configuration.windowTitle, windowRect, instance);
	
	if (configuration.displayMode.doubleBuffer) {
		pixelFormat.dwFlags |= PFD_DOUBLEBUFFER;
	}
	pixelFormat.cColorBits = configuration.displayMode.colorBits;
	pixelFormat.cAlphaBits = configuration.displayMode.alphaBits;
	pixelFormat.cDepthBits = configuration.displayMode.depthBits;
	pixelFormat.cStencilBits = configuration.displayMode.stencilBits;
	
	displayContext = GetDC(window);
	format = ChoosePixelFormat(displayContext, &pixelFormat);
	if (format == 0) {
		fprintf(stderr, "ChoosePixelFormat failed! GetLastError(): %ld\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	if (!SetPixelFormat(displayContext, format, &pixelFormat)) {
		fprintf(stderr, "SetPixelFormat failed! GetLastError(): %ld\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	glContext = wglCreateContext(displayContext);
	if (glContext == NULL) {
		fprintf(stderr, "wglCreateContext failed! GetLastError(): %ld\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	wglMakeCurrent(displayContext, glContext);
	
	const char * WINAPI (* wglGetExtensionsStringEXT)(void) = (const char * WINAPI (*)(void)) wglGetProcAddress("wglGetExtensionsStringEXT");
	if (strstr(wglGetExtensionsStringEXT(), "WGL_ARB_pixel_format")) {
		int attributes[21];
		unsigned int attributeCount = 0;
		UINT formatCount;
		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
		
		attributes[attributeCount++] = WGL_DRAW_TO_WINDOW_ARB;
		attributes[attributeCount++] = GL_TRUE;
		attributes[attributeCount++] = WGL_SUPPORT_OPENGL_ARB;
		attributes[attributeCount++] = GL_TRUE;
		if (configuration.displayMode.doubleBuffer) {
			attributes[attributeCount++] = WGL_DOUBLE_BUFFER_ARB;
			attributes[attributeCount++] = GL_TRUE;
		}
		attributes[attributeCount++] = WGL_PIXEL_TYPE_ARB;
		attributes[attributeCount++] = WGL_TYPE_RGBA_ARB;
		attributes[attributeCount++] = WGL_COLOR_BITS_ARB;
		attributes[attributeCount++] = configuration.displayMode.colorBits;
		if (configuration.displayMode.depthBuffer) {
			attributes[attributeCount++] = WGL_DEPTH_BITS_ARB;
			attributes[attributeCount++] = configuration.displayMode.depthBits;
		}
		if (configuration.displayMode.stencilBuffer) {
			attributes[attributeCount++] = WGL_STENCIL_BITS_ARB;
			attributes[attributeCount++] = configuration.displayMode.stencilBits;
		}
		if (configuration.displayMode.accumBuffer) {
			attributes[attributeCount++] = WGL_ACCUM_BITS_ARB;
			attributes[attributeCount++] = configuration.displayMode.accumBits;
		}
		if (configuration.displayMode.multisample) {
			attributes[attributeCount++] = WGL_SAMPLE_BUFFERS_ARB;
			attributes[attributeCount++] = configuration.displayMode.sampleBuffers;
			attributes[attributeCount++] = WGL_SAMPLES_ARB;
			attributes[attributeCount++] = configuration.displayMode.samples;
		}
		attributes[attributeCount++] = 0;
		
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
		if (wglChoosePixelFormatARB != NULL && wglChoosePixelFormatARB(displayContext, attributes, NULL, 1, &format, &formatCount) && formatCount > 0) {
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(glContext);
			ReleaseDC(window, displayContext);
			DestroyWindow(window);
			window = createShellWindow(configuration.windowTitle, windowRect, instance);
			displayContext = GetDC(window);
			if (displayContext == NULL) {
				fprintf(stderr, "GetDC failed! GetLastError(): %ld\n", GetLastError());
				exit(EXIT_FAILURE);
			}
			
			if (!SetPixelFormat(displayContext, format, &pixelFormat)) {
				fprintf(stderr, "SetPixelFormat failed! GetLastError(): %ld\n", GetLastError());
				exit(EXIT_FAILURE);
			}
			
			glContext = wglCreateContext(displayContext);
			if (glContext == NULL) {
				fprintf(stderr, "wglCreateContext failed! GetLastError(): %ld\n", GetLastError());
				exit(EXIT_FAILURE);
			}
			
			wglMakeCurrent(displayContext, glContext);
		}
	}
	
	glExtensions = glGetString(GL_EXTENSIONS);
	if (!strstr((char *) glExtensions, "GL_ARB_shader_objects") || strstr((char *) glGetString(GL_RENDERER), "GMA")) {
		GLGraphics_init(GL_API_VERSION_DESKTOP_1);
	} else {
		GLGraphics_init(GL_API_VERSION_DESKTOP_2);
	}
	if (!strstr((char *) glExtensions, "WGL_EXT_swap_control") && !strstr(wglGetExtensionsStringEXT(), "WGL_EXT_swap_control")) {
		wglSwapIntervalEXT(1);
	}
	
	setVSync(vsyncWindow);
	ShowWindow(window, command);
	windowShown = true;
	if ((int) configuration.windowWidth != lastWidth && (int) configuration.windowHeight != lastHeight) {
		lastWidth = configuration.windowWidth;
		lastHeight = configuration.windowHeight;
		if (resizeCallback != NULL) {
			resizeCallback(configuration.windowWidth, configuration.windowHeight);
		}
	}
	
	atexit(destroyGLContext);
	
	Target_init();
	
	return EXIT_SUCCESS;
}
