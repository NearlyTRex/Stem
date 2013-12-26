/*
  Copyright (c) 2013 Alex Diener
  
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

#include "glgraphics/GLGraphics.h"
#include "glgraphics/GLIncludes.h"
#include "shell/Shell.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"
#include "shell/ShellThreads.h"
#include "shell/Target.h"
#include "wglshell/WGLShell.h"
#include "wglshell/WGLTarget.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <windows.h>
#include <PowrProf.h>

static HWND window;
static unsigned int buttonMask;
static unsigned int modifierFlags;
static bool isFullScreen;
static bool cursorHiddenByHide;
static bool cursorHiddenUntilMouseMoves;

struct threadFuncInvocation {
	int (* threadFunction)(void * context);
	void * context;
};

void Shell_mainLoop() {
	MSG message;
	
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void Shell_redisplay() {
	InvalidateRect(window, NULL, false);
}

bool Shell_isFullScreen() {
	return isFullScreen;
}

#ifndef MONITOR_DEFAULTTONEAREST
#define MONITOR_DEFAULTTONEAREST 2
#endif
WINUSERAPI HMONITOR WINAPI MonitorFromRect(LPCRECT,DWORD);

bool Shell_setFullScreen(bool fullScreen) {
	static RECT oldWindowRect;
	static DWORD oldWindowStyle;
	
	if (isFullScreen) {
		SetWindowLong(window, GWL_STYLE, oldWindowStyle);
		SetWindowPos(window, HWND_TOP, oldWindowRect.left, oldWindowRect.top, oldWindowRect.right, oldWindowRect.bottom, SWP_NOZORDER | SWP_FRAMECHANGED);
		isFullScreen = false;
		
	} else {
		DWORD windowStyle;
		HMONITOR monitor;
		MONITORINFO monitorInfo;
		RECT screenRect;
		
		GetWindowRect(window, &oldWindowRect);
		oldWindowStyle = windowStyle = GetWindowLong(window, GWL_STYLE);
		
		windowStyle &= ~WS_OVERLAPPEDWINDOW;
		windowStyle |= WS_POPUP;
		SetWindowLong(window, GWL_STYLE, windowStyle);
		monitor = MonitorFromRect(&oldWindowRect, MONITOR_DEFAULTTONEAREST);
		monitorInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &monitorInfo);
		screenRect = monitorInfo.rcMonitor;
		SetWindowPos(window, HWND_TOP, screenRect.left, screenRect.top, screenRect.right - screenRect.left, screenRect.bottom - screenRect.top, SWP_NOZORDER | SWP_FRAMECHANGED);
		isFullScreen = true;
	}
	
	return true;
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

void Shell_setCursorVisible(bool visible) {
	if (visible) {
		if (cursorHiddenByHide) {
			cursorHiddenByHide = false;
			ShowCursor(true);
		}
	} else {
		if (!cursorHiddenByHide) {
			cursorHiddenByHide = true;
			ShowCursor(false);
		}
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

void Shell_setMouseDeltaMode(bool deltaMode) {
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

static LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_PAINT: {
#ifdef DEBUG
			GLenum error;
#endif
			
			ValidateRect(window, NULL);
			Target_draw();
			
#ifdef DEBUG
			while ((error = glGetError()) != GL_NO_ERROR) {
				fprintf(stderr, "GL error: 0x%X\n", error);
			}
#endif
			SwapBuffers(wglGetCurrentDC());
			return 0;
		}
		
		case WM_SIZING: {
			RECT rect;
			
			rect = *(RECT *) lParam;
			glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);
			Target_resized(rect.right - rect.left, rect.bottom - rect.top);
			InvalidateRect(window, NULL, false);
			return 0;
		}
		
		case WM_LBUTTONDOWN:
			buttonMask |= 1 << 0;
			Target_mouseDown(0, LOWORD(lParam), HIWORD(lParam));
			return 0;
			
		case WM_LBUTTONUP:
			buttonMask &= ~(1 << 0);
			Target_mouseUp(0, LOWORD(lParam), HIWORD(lParam));
			return 0;
			
		case WM_RBUTTONDOWN:
			buttonMask |= 1 << 1;
			Target_mouseDown(1, LOWORD(lParam), HIWORD(lParam));
			return 0;
			
		case WM_RBUTTONUP:
			buttonMask &= ~(1 << 1);
			Target_mouseUp(1, LOWORD(lParam), HIWORD(lParam));
			return 0;
			
		case WM_MBUTTONDOWN:
			buttonMask |= 1 << 2;
			Target_mouseDown(2, LOWORD(lParam), HIWORD(lParam));
			return 0;
			
		case WM_MBUTTONUP:
			buttonMask &= ~(1 << 2);
			Target_mouseUp(2, LOWORD(lParam), HIWORD(lParam));
			return 0;
			
		case WM_MOUSEMOVE:
			if (cursorHiddenUntilMouseMoves) {
				cursorHiddenUntilMouseMoves = false;
				ShowCursor(true);
			}
			if (buttonMask != 0) {
				Target_mouseDragged(buttonMask, LOWORD(lParam), HIWORD(lParam));
			} else {
				Target_mouseMoved(LOWORD(lParam), HIWORD(lParam));
			}
			return 0;
			
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			MSG msg;
			unsigned int charCode = 0, keyCode;
			bool capsLock;
			
			capsLock = GetKeyState(VK_CAPITAL) & 0x01;
			if (capsLock && !(modifierFlags & MODIFIER_CAPS_LOCK_BIT)) {
				modifierFlags |= MODIFIER_CAPS_LOCK_BIT;
				Target_keyModifiersChanged(modifierFlags);
			} else if (!capsLock && (modifierFlags & MODIFIER_CAPS_LOCK_BIT)) {
				modifierFlags &= ~MODIFIER_CAPS_LOCK_BIT;
				Target_keyModifiersChanged(modifierFlags);
			}
			
			if (PeekMessage(&msg, window, WM_KEYFIRST, WM_KEYLAST, PM_NOREMOVE) && msg.message == WM_CHAR) {
				charCode = msg.wParam;
			}
			keyCode = lParamToShellKeyCode(lParam);
			if (keyCode == KEYBOARD_LEFT_SHIFT || keyCode == KEYBOARD_RIGHT_SHIFT) {
				if (!(modifierFlags & MODIFIER_SHIFT_BIT)) {
					modifierFlags |= MODIFIER_SHIFT_BIT;
					Target_keyModifiersChanged(modifierFlags);
				}
				
			} else if (keyCode == KEYBOARD_LEFT_CONTROL || keyCode == KEYBOARD_RIGHT_CONTROL) {
				if (!(modifierFlags & MODIFIER_CONTROL_BIT)) {
					modifierFlags |= MODIFIER_CONTROL_BIT;
					Target_keyModifiersChanged(modifierFlags);
				}
				
			} else if (keyCode == KEYBOARD_LEFT_ALT || keyCode == KEYBOARD_RIGHT_ALT) {
				if (!(modifierFlags & MODIFIER_ALT_BIT)) {
					modifierFlags |= MODIFIER_ALT_BIT;
					Target_keyModifiersChanged(modifierFlags);
				}
				
			} else if (keyCode != KEYBOARD_CAPS_LOCK && keyCode != KEYBOARD_LEFT_GUI && keyCode != KEYBOARD_RIGHT_GUI) {
				Target_keyDown(charCode, keyCode, modifierFlags);
			}
			return 0;
		}
		
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			unsigned int keyCode;
			bool capsLock;
			
			capsLock = GetKeyState(VK_CAPITAL) & 0x01;
			if (capsLock && !(modifierFlags & MODIFIER_CAPS_LOCK_BIT)) {
				modifierFlags |= MODIFIER_CAPS_LOCK_BIT;
				Target_keyModifiersChanged(modifierFlags);
			} else if (!capsLock && (modifierFlags & MODIFIER_CAPS_LOCK_BIT)) {
				modifierFlags &= ~MODIFIER_CAPS_LOCK_BIT;
				Target_keyModifiersChanged(modifierFlags);
			}
			
			keyCode = lParamToShellKeyCode(lParam);
			if (keyCode == KEYBOARD_LEFT_SHIFT || keyCode == KEYBOARD_RIGHT_SHIFT) {
				if (modifierFlags & MODIFIER_SHIFT_BIT) {
					modifierFlags &= ~MODIFIER_SHIFT_BIT;
					Target_keyModifiersChanged(modifierFlags);
				}
				
			} else if (keyCode == KEYBOARD_LEFT_CONTROL || keyCode == KEYBOARD_RIGHT_CONTROL) {
				if (modifierFlags & MODIFIER_CONTROL_BIT) {
					modifierFlags &= ~MODIFIER_CONTROL_BIT;
					Target_keyModifiersChanged(modifierFlags);
				}
				
			} else if (keyCode == KEYBOARD_LEFT_ALT || keyCode == KEYBOARD_RIGHT_ALT) {
				if (modifierFlags & MODIFIER_ALT_BIT) {
					modifierFlags &= ~MODIFIER_ALT_BIT;
					Target_keyModifiersChanged(modifierFlags);
				}
				
			} else if (keyCode != KEYBOARD_CAPS_LOCK && keyCode != KEYBOARD_LEFT_GUI && keyCode != KEYBOARD_RIGHT_GUI) {
				Target_keyUp(keyCode, modifierFlags);
			}
			return 0;
		}
		
		case WM_ACTIVATEAPP:
			if (wParam) {
				Target_foregrounded();
			} else {
				Target_backgrounded();
			}
			break;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(window, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR commandLine, int command) {
	WNDCLASSEX windowClass;
	ATOM result;
	struct WGLShellConfiguration configuration;
	HDC displayContext;
	HGLRC glContext;
	PIXELFORMATDESCRIPTOR pixelFormat;
	int format;
	const unsigned char * glExtensions;
	
	configuration.windowX = 10;
	configuration.windowY = 10;
	configuration.windowWidth = 800;
	configuration.windowHeight = 600;
	configuration.windowTitle = "Change configuration->windowTitle in WGLTarget_configure()";
	configuration.iconResource = 101;
	configuration.displayMode.doubleBuffer = true;
	configuration.displayMode.colorBits = 24;
	configuration.displayMode.alphaBits = 8;
	configuration.displayMode.depthBits = 0;
	configuration.displayMode.stencilBits = 0;
	
	WGLTarget_configure(instance, prevInstance, commandLine, command, &configuration);
	
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
	
	window = CreateWindow("wglshell",
	                      configuration.windowTitle,
	                      WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_OVERLAPPEDWINDOW,
	                      configuration.windowX,
	                      configuration.windowY,
	                      configuration.windowWidth,
	                      configuration.windowHeight,
	                      NULL,
	                      NULL,
	                      instance,
	                      NULL);
	if (window == NULL) {
		fprintf(stderr, "CreateWindow failed! GetLastError(): %ld\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	
	pixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormat.nVersion = 1;
	pixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	if (configuration.displayMode.doubleBuffer) {
		pixelFormat.dwFlags |= PFD_DOUBLEBUFFER;
	}
	pixelFormat.iPixelType = PFD_TYPE_RGBA;
	pixelFormat.cAlphaBits = configuration.displayMode.colorBits;
	pixelFormat.cRedBits = 0;
	pixelFormat.cRedShift = 0;
	pixelFormat.cGreenBits = 0;
	pixelFormat.cGreenShift = 0;
	pixelFormat.cBlueBits = 0;
	pixelFormat.cBlueShift = 0;
	pixelFormat.cAlphaBits = configuration.displayMode.alphaBits;
	pixelFormat.cAlphaShift = 0;
	pixelFormat.cAccumBits = 0;
	pixelFormat.cAccumRedBits = 0;
	pixelFormat.cAccumGreenBits = 0;
	pixelFormat.cAccumBlueBits = 0;
	pixelFormat.cAccumAlphaBits = 0;
	pixelFormat.cDepthBits = configuration.displayMode.depthBits;
	pixelFormat.cStencilBits = configuration.displayMode.stencilBits;
	pixelFormat.cAuxBuffers = 0;
	pixelFormat.iLayerType = 0;
	pixelFormat.bReserved = 0;
	pixelFormat.dwLayerMask = 0;
	pixelFormat.dwVisibleMask = 0;
	pixelFormat.dwDamageMask = 0;
	
	displayContext = GetDC(window);
	if (displayContext == NULL) {
		fprintf(stderr, "GetDC failed! GetLastError(): %ld\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	
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
	
	glExtensions = glGetString(GL_EXTENSIONS);
	if (strstr((char *) glExtensions, "GL_ARB_shader_objects") || strstr((char *) glGetString(GL_RENDERER), "GMA")) {
		GLGraphics_init(GL_API_VERSION_DESKTOP_1);
	} else {
		GLGraphics_init(GL_API_VERSION_DESKTOP_2);
	}
	if (!strstr((char *) glExtensions, "WGL_EXT_swap_control") && !strstr(wglGetExtensionsStringEXT(), "WGL_EXT_swap_control")) {
		wglSwapIntervalEXT(1);
	}
	
	ShowWindow(window, command);
	Target_resized(configuration.windowWidth, configuration.windowHeight);
	Target_init();
	
	return EXIT_SUCCESS;
}
