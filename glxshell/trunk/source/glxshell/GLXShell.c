/*
  Copyright (c) 2017 Alex Diener
  
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

#define GLEW_STATIC 1
#include "GL/glew.h"
// glxew.h includes X.h, which defines Atom and Screen, causing conflicts with the Atom in utilities and the Screen in screenmanager
#define Atom Atom_not
#define Screen Screen_not
#include "GL/glxew.h"
#undef Atom
#undef Screen
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "glxshell/GLXShell.h"
#include "glxshell/GLXShellCallbacks.h"
#include "glxshell/GLXTarget.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"
#include "shell/ShellThreads.h"
#include "shell/Shell.h"
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/glu.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <gtk/gtk.h>
#include <gdk/gdkscreen.h>

struct GLXShellTimer {
	double interval;
	double nextFireTime;
	bool repeat;
	unsigned int id;
	void (* callback)(unsigned int timerID, void * context);
	void * context;
};

#if defined(__linux)
#define VSYNC_DEFAULT_WINDOW false
#define VSYNC_DEFAULT_FULLSCREEN true
#else
#define VSYNC_DEFAULT_WINDOW true
#define VSYNC_DEFAULT_FULLSCREEN true
#endif

static Display * display;
static int screen;
static GLXContext context;
static Window window;
static Window rootWindow;
static Atom deleteWindowAtom;

static bool redisplayNeeded;
static unsigned int buttonMask = 0;
static unsigned int modifierMask = 0;
static int lastMouseX, lastMouseY;
static bool inFullScreenMode = false;
static bool vsyncWindow = VSYNC_DEFAULT_WINDOW, vsyncFullscreen = VSYNC_DEFAULT_FULLSCREEN;
static struct GLXShellConfiguration configuration;
static unsigned int nextTimerID;
static size_t timerCount;
static struct GLXShellTimer * timers;
static int lastWidth, lastHeight;
static bool backgrounded;

static bool cursorHiddenByHide;
static bool showCursorOnNextMouseMove;
static int lastUnhiddenCursor = ShellCursor_arrow;
static bool mouseDeltaMode;
static int restoreMouseX, restoreMouseY;
static int lastMouseX, lastMouseY;
static int ignoreX = INT_MAX, ignoreY = INT_MAX;
static bool nextKeyDownIsRepeat;

static void setEmptyCursor() {
	static Cursor emptyCursor = None;
	
	if (emptyCursor == None) {
		char emptyBits[32];
		XColor color;
		Pixmap pixmap;
		
		memset(emptyBits, 0, sizeof(emptyBits));
		memset(&color, 0, sizeof(color));
		pixmap = XCreateBitmapFromData(display, window, emptyBits, 16, 16);
		if (pixmap != None) {
			emptyCursor = XCreatePixmapCursor(display, pixmap, pixmap, &color, &color, 0, 0);
			XFreePixmap(display, pixmap);
		}
	}
	XDefineCursor(display, window, emptyCursor);
}

static void setShellCursor(enum ShellCursor cursor) {
	static struct {unsigned int name; Cursor cursor;} cursors[] = {
		{XC_top_left_arrow, None}, // ShellCursor_arrow
		{XC_xterm,          None}, // ShellCursor_iBeam
		{XC_crosshair,      None}, // ShellCursor_crosshair
		{XC_hand1,          None}, // ShellCursor_hand
		{XC_watch,          None}  // ShellCursor_wait
	};
	
	if ((unsigned int) cursor > ShellCursor_wait) {
		return;
	}
	
	// XC_top_left_arrow doesn't match the default arrow cursor, so use None instead (which is equivalent to calling XUndefineCursor)
	if (cursors[cursor].cursor == None && cursor != ShellCursor_arrow) {
		cursors[cursor].cursor = XCreateFontCursor(display, cursors[cursor].name);
	}
	XDefineCursor(display, window, cursors[cursor].cursor);
}

static void warpPointerAndIgnoreEvent(int x, int y) {
	ignoreX = x;
	ignoreY = y;
	lastMouseX = x;
	lastMouseY = y;
	XWarpPointer(display, None, window, 0, 0, 0, 0, x, y);
}

static unsigned int xkeyCodeToShellKeyCode(unsigned int keyCode) {
	switch (keyCode) {
		case 9:
			return KEYBOARD_ESCAPE;
		case 67:
			return KEYBOARD_F1;
		case 68:
			return KEYBOARD_F2;
		case 69:
			return KEYBOARD_F3;
		case 70:
			return KEYBOARD_F4;
		case 71:
			return KEYBOARD_F5;
		case 72:
			return KEYBOARD_F6;
		case 73:
			return KEYBOARD_F7;
		case 74:
			return KEYBOARD_F8;
		case 75:
			return KEYBOARD_F9;
		case 76:
			return KEYBOARD_F10;
		case 95:
			return KEYBOARD_F11;
		case 96:
			return KEYBOARD_F12;
		case 107:
			return KEYBOARD_PRINT_SCREEN;
		case 78:
			return KEYBOARD_SCROLL_LOCK;
		case 127:
			return KEYBOARD_PAUSE;
		case 49:
			return KEYBOARD_GRAVE_ACCENT_AND_TILDE;
		case 10:
			return KEYBOARD_1;
		case 11:
			return KEYBOARD_2;
		case 12:
			return KEYBOARD_3;
		case 13:
			return KEYBOARD_4;
		case 14:
			return KEYBOARD_5;
		case 15:
			return KEYBOARD_6;
		case 16:
			return KEYBOARD_7;
		case 17:
			return KEYBOARD_8;
		case 18:
			return KEYBOARD_9;
		case 19:
			return KEYBOARD_0;
		case 20:
			return KEYBOARD_HYPHEN;
		case 21:
			return KEYBOARD_EQUAL_SIGN;
		case 22:
			return KEYBOARD_DELETE_OR_BACKSPACE;
		case 23:
			return KEYBOARD_TAB;
		case 24:
			return KEYBOARD_Q;
		case 25:
			return KEYBOARD_W;
		case 26:
			return KEYBOARD_E;
		case 27:
			return KEYBOARD_R;
		case 28:
			return KEYBOARD_T;
		case 29:
			return KEYBOARD_Y;
		case 30:
			return KEYBOARD_U;
		case 31:
			return KEYBOARD_I;
		case 32:
			return KEYBOARD_O;
		case 33:
			return KEYBOARD_P;
		case 34:
			return KEYBOARD_OPEN_BRACKET;
		case 35:
			return KEYBOARD_CLOSE_BRACKET;
		case 51:
			return KEYBOARD_BACKSLASH;
		case 66:
			return KEYBOARD_CAPS_LOCK;
		case 38:
			return KEYBOARD_A;
		case 39:
			return KEYBOARD_S;
		case 40:
			return KEYBOARD_D;
		case 41:
			return KEYBOARD_F;
		case 42:
			return KEYBOARD_G;
		case 43:
			return KEYBOARD_H;
		case 44:
			return KEYBOARD_J;
		case 45:
			return KEYBOARD_K;
		case 46:
			return KEYBOARD_L;
		case 47:
			return KEYBOARD_SEMICOLON;
		case 48:
			return KEYBOARD_QUOTE;
		case 36:
			return KEYBOARD_RETURN_OR_ENTER;
		case 50:
			return KEYBOARD_LEFT_SHIFT;
		case 52:
			return KEYBOARD_Z;
		case 53:
			return KEYBOARD_X;
		case 54:
			return KEYBOARD_C;
		case 55:
			return KEYBOARD_V;
		case 56:
			return KEYBOARD_B;
		case 57:
			return KEYBOARD_N;
		case 58:
			return KEYBOARD_M;
		case 59:
			return KEYBOARD_COMMA;
		case 60:
			return KEYBOARD_PERIOD;
		case 61:
			return KEYBOARD_SLASH;
		case 62:
			return KEYBOARD_RIGHT_SHIFT;
		case 37:
			return KEYBOARD_LEFT_CONTROL;
		case 133:
			return KEYBOARD_LEFT_GUI;
		case 64:
			return KEYBOARD_LEFT_ALT;
		case 65:
			return KEYBOARD_SPACEBAR;
		case 108:
			return KEYBOARD_RIGHT_ALT;
		case 134:
			return KEYBOARD_RIGHT_GUI;
		case 135:
			return KEYBOARD_MENU;
		case 105:
			return KEYBOARD_RIGHT_CONTROL;
		case 113:
			return KEYBOARD_LEFT_ARROW;
		case 114:
			return KEYBOARD_RIGHT_ARROW;
		case 111:
			return KEYBOARD_UP_ARROW;
		case 116:
			return KEYBOARD_DOWN_ARROW;
		case 118:
			return KEYBOARD_INSERT;
		case 110:
			return KEYBOARD_HOME;
		case 112:
			return KEYBOARD_PAGE_UP;
		case 119:
			return KEYBOARD_DELETE_FORWARD;
		case 115:
			return KEYBOARD_END;
		case 117:
			return KEYBOARD_PAGE_DOWN;
		case 77:
			return KEYPAD_NUM_LOCK;
		case 125:
			return KEYPAD_EQUAL_SIGN;
		case 106:
			return KEYPAD_SLASH;
		case 63:
			return KEYPAD_ASTERISK;
		case 82:
			return KEYPAD_HYPHEN;
		case 79:
			return KEYPAD_7;
		case 80:
			return KEYPAD_8;
		case 81:
			return KEYPAD_9;
		case 86:
			return KEYPAD_PLUS;
		case 83:
			return KEYPAD_4;
		case 84:
			return KEYPAD_5;
		case 85:
			return KEYPAD_6;
		case 90:
			return KEYPAD_0;
		case 91:
			return KEYPAD_PERIOD;
		case 104:
			return KEYPAD_ENTER;
#ifdef DEBUG
		default:
			fprintf(stderr, "Warning: Unknown key code %u\n", keyCode);
#endif
	}
	return 0;
}

static unsigned int xkeyStateToShellModifiers(unsigned int xkeyState) {
	unsigned int state = 0;
	if (xkeyState & ShiftMask) {
		state |= MODIFIER_SHIFT_BIT;
	}
	if (xkeyState & ControlMask) {
		state |= MODIFIER_CONTROL_BIT;
	}
	if (xkeyState & LockMask) {
		state |= MODIFIER_CAPS_LOCK_BIT;
	}
	if (xkeyState & Mod1Mask) {
		state |= MODIFIER_ALT_BIT;
	}
	return state;
}

#define MOUSE_BUTTON_INDEX_SCROLL_WHEEL_UP 3
#define MOUSE_BUTTON_INDEX_SCROLL_WHEEL_DOWN 4

void Shell_mainLoop() {
	XEvent event;
	XWindowAttributes windowAttributes;
	char charCode;
	unsigned int keyCode;
	KeySym keySym;
	unsigned int modifiers;
	char keys[32];
	unsigned int timerIndex, timerIndex2;
	double currentTime;
	int reportedX, reportedY;
	
	for (;;) {
		while (XPending(display)) {
			XNextEvent(display, &event);
			
			switch (event.type) {
				case Expose:
					redisplayNeeded = true;
				case ConfigureNotify:
					XGetWindowAttributes(display, window, &windowAttributes);
					if (windowAttributes.width != lastWidth || windowAttributes.height != lastHeight) {
						lastWidth = windowAttributes.width;
						lastHeight = windowAttributes.height;
						if (resizeCallback != NULL) {
							resizeCallback(windowAttributes.width, windowAttributes.height);
						}
						redisplayNeeded = true;
					}
					break;
					
				case KeyPress:
					if (!XLookupString(&event.xkey, &charCode, 1, &keySym, NULL)) {
						charCode = 0;
					}
					keyCode = xkeyCodeToShellKeyCode(event.xkey.keycode);
					modifiers = xkeyStateToShellModifiers(event.xkey.state);
					switch (keyCode) {
						case KEYBOARD_CAPS_LOCK:
							modifiers |= MODIFIER_CAPS_LOCK_BIT;
							break;
							
						case KEYBOARD_LEFT_SHIFT:
						case KEYBOARD_RIGHT_SHIFT:
							modifiers |= MODIFIER_SHIFT_BIT;
							break;
							
						case KEYBOARD_LEFT_CONTROL:
						case KEYBOARD_RIGHT_CONTROL:
							modifiers |= MODIFIER_CONTROL_BIT;
							break;
							
						case KEYBOARD_LEFT_ALT:
						case KEYBOARD_RIGHT_ALT:
							modifiers |= MODIFIER_ALT_BIT;
							break;
					}
					if (keyCode != 0) {
						if (keyDownCallback != NULL) {
							keyDownCallback(charCode, keyCode, modifiers, nextKeyDownIsRepeat);
						}
						nextKeyDownIsRepeat = false;
					}
					if (modifierMask != modifiers) {
						modifierMask = modifiers;
						if (keyModifiersChangedCallback != NULL) {
							keyModifiersChangedCallback(modifierMask);
						}
					}
					break;
					
				case KeyRelease:
					if (event.xkey.keycode > 255) {
						break;
					}
					XQueryKeymap(display, keys);
					if (keys[event.xkey.keycode >> 3] & 1 << (event.xkey.keycode % 8)) {
						// Xlib sends key ups for key repeats; this one is a repeat, so skip it and mark the next keyDown as a repeat
						nextKeyDownIsRepeat = true;
						break;
					}
					
					keyCode = xkeyCodeToShellKeyCode(event.xkey.keycode);
					modifiers = xkeyStateToShellModifiers(event.xkey.state);
					switch (keyCode) {
						case KEYBOARD_CAPS_LOCK:
							modifiers &= ~MODIFIER_CAPS_LOCK_BIT;
							break;
							
						case KEYBOARD_LEFT_SHIFT:
						case KEYBOARD_RIGHT_SHIFT:
							modifiers &= ~MODIFIER_SHIFT_BIT;
							break;
							
						case KEYBOARD_LEFT_CONTROL:
						case KEYBOARD_RIGHT_CONTROL:
							modifiers &= ~MODIFIER_CONTROL_BIT;
							break;
							
						case KEYBOARD_LEFT_ALT:
						case KEYBOARD_RIGHT_ALT:
							modifiers &= ~MODIFIER_ALT_BIT;
							break;
					}
					if (keyCode != 0) {
						if (keyUpCallback != NULL) {
							keyUpCallback(keyCode, modifiers);
						}
					}
					if (modifierMask != modifiers) {
						modifierMask = modifiers;
						if (keyModifiersChangedCallback != NULL) {
							keyModifiersChangedCallback(modifierMask);
						}
					}
					break;
					
				case ButtonPress:
					if (event.xbutton.button - 1 == MOUSE_BUTTON_INDEX_SCROLL_WHEEL_UP) {
						if (scrollWheelCallback != NULL) {
							scrollWheelCallback(modifierMask & MODIFIER_SHIFT_BIT ? -1 : 0, modifierMask & MODIFIER_SHIFT_BIT ? 0 : -1);
						}
						
					} else if (event.xbutton.button - 1 == MOUSE_BUTTON_INDEX_SCROLL_WHEEL_DOWN) {
						if (scrollWheelCallback != NULL) {
							scrollWheelCallback(modifierMask & MODIFIER_SHIFT_BIT ? 1 : 0, modifierMask & MODIFIER_SHIFT_BIT ? 0 : 1);
						}
							
					} else {
						buttonMask |= 1 << (event.xbutton.button - 1);
						if (mouseDownCallback != NULL) {
							mouseDownCallback(event.xbutton.button - 1, event.xbutton.x, event.xbutton.y);
						}
					}
					break;
					
				case ButtonRelease:
					if (event.xbutton.button - 1 != MOUSE_BUTTON_INDEX_SCROLL_WHEEL_UP &&
					    event.xbutton.button - 1 != MOUSE_BUTTON_INDEX_SCROLL_WHEEL_DOWN) {
						buttonMask &= ~(1 << (event.xbutton.button - 1));
						if (mouseUpCallback != NULL) {
							mouseUpCallback(event.xbutton.button - 1, event.xbutton.x, event.xbutton.y);
						}
					}
					break;
					
				case MotionNotify:
					if ((event.xbutton.x == lastMouseX && event.xbutton.y == lastMouseY) ||
					    (event.xbutton.x == ignoreX    && event.xbutton.y == ignoreY)) {
						break;
					}
					if (showCursorOnNextMouseMove) {
						setShellCursor(lastUnhiddenCursor);
					}
					if (mouseDeltaMode) {
						reportedX = event.xbutton.x - lastMouseX;
						reportedY = event.xbutton.y - lastMouseY;
					} else {
						reportedX = event.xbutton.x;
						reportedY = event.xbutton.y;
					}
					lastMouseX = event.xbutton.x;
					lastMouseY = event.xbutton.y;
					ignoreX = ignoreY = INT_MAX;
					if (buttonMask != 0) {
						if (mouseDraggedCallback != NULL) {
							mouseDraggedCallback(buttonMask, reportedX, reportedY);
						}
					} else {
						if (mouseMovedCallback != NULL) {
							mouseMovedCallback(reportedX, reportedY);
						}
					}
					if (mouseDeltaMode) {
						XGetWindowAttributes(display, window, &windowAttributes);
						warpPointerAndIgnoreEvent(windowAttributes.width / 2, windowAttributes.height / 2);
					}
					break;
					
				case FocusIn:
					if (backgrounded && (event.xfocus.mode != NotifyGrab || event.xfocus.detail != NotifyPointer)) {
						backgrounded = false;
						if (foregroundedCallback != NULL) {
							foregroundedCallback();
						}
					}
					break;
					
				case FocusOut:
					if (event.xfocus.mode != NotifyGrab && event.xfocus.mode != NotifyUngrab) {
						backgrounded = true;
						if (backgroundedCallback != NULL) {
							backgroundedCallback();
						}
					}
					break;
					
				case ClientMessage:
					if ((Atom) event.xclient.data.l[0] == deleteWindowAtom) {
						if (confirmQuitCallback != NULL && !confirmQuitCallback()) {
							break;
						}
						glXMakeCurrent(display, None, NULL);
						glXDestroyContext(display, context);
						XDestroyWindow(display, window);
						XCloseDisplay(display);
						exit(EXIT_SUCCESS);
					}
					break;
			}
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
#ifdef DEBUG
			GLenum error;
#endif
			
			redisplayNeeded = false;
			if (drawCallback != NULL && drawCallback()) {
				glXSwapBuffers(display, window);
			}
			
#ifdef DEBUG
			error = glGetError();
			while (error != GL_NO_ERROR) {
				fprintf(stderr, "GL error: %s\n", gluErrorString(error));
				error = glGetError();
			}
#endif
		}
		
		if (timerCount == 0 && !redisplayNeeded && !XPending(display)) {
			fd_set fdset;
			int socket;
			struct timeval wait;
			
			socket = ConnectionNumber(display);
			FD_ZERO(&fdset);
			FD_SET(socket, &fdset);
			wait.tv_sec = INT_MAX;
			wait.tv_usec = 0;
			select(socket + 1, &fdset, NULL, NULL, &wait);
		}
	}
}

void Shell_redisplay() {
	redisplayNeeded = true;
}

unsigned int Shell_getDisplayCount() {
	return ScreenCount(display);
}

unsigned int Shell_getDisplayIndexFromWindow() {
	return 0;
}

void Shell_getDisplayBounds(unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight) {
	if (outOffsetX != NULL) {
		*outOffsetX = 0;
	}
	if (outOffsetY != NULL) {
		*outOffsetY = 0;
	}
	if (outWidth != NULL) {
		*outWidth = DisplayWidth(display, screen);
	}
	if (outHeight != NULL) {
		*outHeight = DisplayHeight(display, screen);
	}
}

void Shell_getSafeWindowRect(unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight) {
	GdkRectangle rect;
	gdk_screen_get_monitor_workarea(gdk_screen_get_default(), displayIndex, &rect);
	if (outOffsetX != NULL) {
		*outOffsetX = rect.x;
	}
	if (outOffsetY != NULL) {
		*outOffsetY = rect.y;
	}
	if (outWidth != NULL) {
		*outWidth = rect.width;
	}
	if (outHeight != NULL) {
		*outHeight = rect.height;
	}
}

bool Shell_isFullScreen() {
	return inFullScreenMode;
}

static void setVSync(bool sync) {
	const char * extensions = glXQueryExtensionsString(glXGetCurrentDisplay(), 0);
	if (strstr(extensions, "GLX_EXT_swap_control")) {
		PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress((const GLubyte *) "glXSwapIntervalEXT");
		glXSwapIntervalEXT(glXGetCurrentDisplay(), glXGetCurrentDrawable(), sync);
	} else if (strstr(extensions, "GLX_SGI_swap_control")) {
		PFNGLXSWAPINTERVALSGIPROC glxSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC) glXGetProcAddress((const GLubyte *) "glXSwapIntervalSGI");
		glxSwapIntervalSGI(sync);
	}
}

#define _NET_WM_STATE_TOGGLE 2
bool toggleFullScreenEWMH() {
	XEvent event;
	
	event.type = ClientMessage;
	event.xclient.window = window;
	event.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False); // TODO: Cache atom?
	event.xclient.format = 32;
	event.xclient.data.l[0] = _NET_WM_STATE_TOGGLE;
	event.xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False); // TODO: Cache atom?
	event.xclient.data.l[2] = 0;
	event.xclient.data.l[3] = 1;
	event.xclient.data.l[4] = 0;
	
	return !!XSendEvent(display, rootWindow, 0, SubstructureRedirectMask | SubstructureNotifyMask, &event);
}

bool Shell_enterFullScreen(unsigned int displayIndex) {
	if (!inFullScreenMode) {
		if (!toggleFullScreenEWMH()) {
			return false;
		}
		setVSync(vsyncFullscreen);
		inFullScreenMode = true;
	}
	
	return true;
}

void Shell_exitFullScreen() {
	if (inFullScreenMode) {
		if (!toggleFullScreenEWMH()) {
			return;
		}
		inFullScreenMode = false;
		setVSync(vsyncWindow);
	}
}

double Shell_getCurrentTime() {
	struct timespec currentTime;
	
	clock_gettime(CLOCK_MONOTONIC, &currentTime);
	return currentTime.tv_sec + currentTime.tv_nsec * 0.000000001;
}

const char * Shell_getResourcePath() {
	return "Resources";
}

const char * Shell_getSupportPath(const char * subdirectory) {
	static char supportPath[PATH_MAX];
	
	if (subdirectory == NULL) {
		snprintf(supportPath, PATH_MAX, "%s/.config", getenv("HOME"));
	} else {
		snprintf(supportPath, PATH_MAX, "%s/.config/%s", getenv("HOME"), subdirectory);
	}
	mkdir(supportPath, 0777);
	return supportPath;
}

void Shell_openURL(const char * url) {
	if (!fork()) {
		execl("/usr/bin/x-www-browser", "/usr/bin/x-www-browser", url, NULL);
		exit(EXIT_SUCCESS);
	}
}

struct batteryInfo {
	enum ShellBatteryState state;
	float level;
};

static bool readNextKeyValuePair(size_t * ioPosition, const char * contents, size_t length, char * outKey, size_t keySize, char * outValue, size_t valueSize) {
	size_t charIndex, lastCharIndex, tokenLength;
	
	lastCharIndex = *ioPosition;
	while (lastCharIndex < length && isspace(contents[lastCharIndex])) {
		lastCharIndex++;
	}
	if (lastCharIndex >= length) {
		return false;
	}
	charIndex = lastCharIndex;
	while (charIndex < length && contents[charIndex] != ':') {
		charIndex++;
	}
	if (charIndex >= length) {
		return false;
	}
	tokenLength = charIndex - lastCharIndex;
	if (tokenLength > keySize) {
		tokenLength = keySize - 1;
	}
	memcpy(outKey, contents + lastCharIndex, tokenLength);
	outKey[tokenLength] = '\x00';
	
	lastCharIndex = charIndex + 1;
	while (lastCharIndex < length && isspace(contents[lastCharIndex])) {
		lastCharIndex++;
	}
	if (lastCharIndex >= length) {
		return false;
	}
	charIndex = lastCharIndex;
	while (charIndex < length && contents[charIndex] != '\n') {
		charIndex++;
	}
	if (charIndex >= length) {
		return false;
	}
	tokenLength = charIndex - lastCharIndex;
	if (tokenLength > valueSize) {
		tokenLength = valueSize - 1;
	}
	memcpy(outValue, contents + lastCharIndex, tokenLength);
	outValue[tokenLength] = '\x00';
	
	*ioPosition = charIndex + 1;
	return true;
}

static char * readVirtualFile(const char * path, size_t * outLength) {
	FILE * file;
	int byte;
	size_t allocatedSize = 16;
	char * data;
	size_t length = 0;
	
	file = fopen(path, "rb");
	if (file == NULL) {
		return NULL;
	}
	data = malloc(allocatedSize);
	for (;;) {
		byte = fgetc(file);
		if (feof(file)) {
			break;
		}
		if (length >= allocatedSize) {
			allocatedSize *= 2;
			data = realloc(data, allocatedSize);
		}
		data[length++] = byte;
	}
	fclose(file);
	
	*outLength = length;
	return data;
}

#define KEY_SIZE 24
#define VALUE_SIZE 16

static struct batteryInfo getBatteryInfo() {
	struct batteryInfo info = {ShellBatteryState_unknown, -1.0f};
	DIR * dir;
	struct dirent * dirent;
	char path[PATH_MAX];
	unsigned int batteryNumber;
	size_t length, position;
	char * contents;
	char key[KEY_SIZE], value[VALUE_SIZE];
	bool present = true;
	unsigned int lastFullCapacity_mWh = 0;
	unsigned int remainingCapacity_mWh = 0;
	
	dir = opendir("/proc/acpi");
	if (dir == NULL) {
		return info;
	}
	closedir(dir);
	
	dir = opendir("/proc/acpi/battery");
	if (dir == NULL) {
		info.state = ShellBatteryState_notBatteryPowered;
		return info;
	}
	
	while ((dirent = readdir(dir)) != NULL) {
		if (sscanf(dirent->d_name, "BAT%u", &batteryNumber)) {
			snprintf(path, PATH_MAX, "/proc/acpi/battery/BAT%u/info", batteryNumber);
			contents = readVirtualFile(path, &length);
			if (contents == NULL) {
				continue;
			}
			
			position = 0;
			for (;;) {
				if (!readNextKeyValuePair(&position, contents, length, key, KEY_SIZE, value, VALUE_SIZE)) {
					break;
				}
				if (!strcmp(key, "present")) {
					if (!strcmp(value, "no")) {
						present = false;
						break;
					}
					
				} else if (!strcmp(key, "last full capacity")) {
					sscanf(value, "%u mWh", &lastFullCapacity_mWh);
				}
			}
			free(contents);
			
			if (!present) {
				info.state = ShellBatteryState_batteryMissing;
				break;
			}
			
			snprintf(path, PATH_MAX, "/proc/acpi/battery/BAT%u/state", batteryNumber);
			contents = readVirtualFile(path, &length);
			if (contents == NULL) {
				continue;
			}
			
			position = 0;
			for (;;) {
				if (!readNextKeyValuePair(&position, contents, length, key, KEY_SIZE, value, VALUE_SIZE)) {
					break;
				}
				if (!strcmp(key, "present")) {
					if (!strcmp(value, "no")) {
						present = false;
						break;
					}
					
				} else if (!strcmp(key, "remaining capacity")) {
					sscanf(value, "%u mWh", &remainingCapacity_mWh);
					
				} else if (!strcmp(key, "charging state")) {
					if (!strcmp(value, "discharging")) {
						info.state = ShellBatteryState_unplugged;
						
					} else if (!strcmp(value, "charging")) {
						info.state = ShellBatteryState_charging;
						
					} else if (!strcmp(value, "charged")) {
						info.state = ShellBatteryState_full;
						
					} else {
						info.state = ShellBatteryState_unknown;
					}
				}
			}
			free(contents);
			
			if (!present) {
				info.state = ShellBatteryState_batteryMissing;
				break;
			}
			
			if (lastFullCapacity_mWh > 0) {
				if (remainingCapacity_mWh >= lastFullCapacity_mWh) {
					info.level = 1.0f;
				} else {
					info.level = remainingCapacity_mWh / (float) lastFullCapacity_mWh;
				}
			}
			
			// Only support for one battery
			break;
		}
	}
	closedir(dir);
	return info;
}

enum ShellBatteryState Shell_getBatteryState() {
	return getBatteryInfo().state;
}

float Shell_getBatteryLevel() {
	return getBatteryInfo().level;
}

unsigned int Shell_setTimer(double interval, bool repeat, void (* callback)(unsigned int timerID, void * context), void * context) {
	timers = realloc(timers, sizeof(struct GLXShellTimer) * (timerCount + 1));
	timers[timerCount].interval = interval;
	timers[timerCount].nextFireTime = Shell_getCurrentTime() + interval;
	timers[timerCount].repeat = repeat;
	timers[timerCount].id = nextTimerID++;
	timers[timerCount].callback = callback;
	timers[timerCount].context = context;
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
			setShellCursor(lastUnhiddenCursor);
		}
	} else if (!visible && !cursorHiddenByHide) {
		cursorHiddenByHide = true;
		setEmptyCursor();
	}
}

void Shell_hideCursorUntilMouseMoves() {
	if (!cursorHiddenByHide) {
		showCursorOnNextMouseMove = true;
		setEmptyCursor();
	}
}

void Shell_setCursor(int cursor) {
	lastUnhiddenCursor = cursor;
	if (cursorHiddenByHide || mouseDeltaMode) {
		return;
	}
	setShellCursor(cursor);
}

void Shell_setMouseDeltaMode(bool deltaMode) {
	if (!mouseDeltaMode && deltaMode) {
		XWindowAttributes windowAttributes;
		
		restoreMouseX = lastMouseX;
		restoreMouseY = lastMouseY;
		XGetWindowAttributes(display, window, &windowAttributes);
		warpPointerAndIgnoreEvent(windowAttributes.width / 2, windowAttributes.height / 2);
		setEmptyCursor();
		mouseDeltaMode = true;
		
	} else if (mouseDeltaMode && !deltaMode) {
		warpPointerAndIgnoreEvent(restoreMouseX, restoreMouseY);
		mouseDeltaMode = false;
		setShellCursor(lastUnhiddenCursor);
	}
}

void Shell_setVSync(bool sync, bool fullscreen) {
	if (fullscreen) {
		vsyncFullscreen = sync;
		if (inFullScreenMode) {
			setVSync(sync);
		}
		
	} else {
		vsyncWindow = sync;
		if (!inFullScreenMode) {
			setVSync(sync);
		}
	}
}

bool Shell_openFileDialog(const char * basePath, char * outFilePath, unsigned int maxLength) {
	GtkWidget * dialog;
	GtkFileChooser * chooser;
	gint res;
	bool success = false;
	
	dialog = gtk_file_chooser_dialog_new(NULL,
	                                     NULL,
	                                     GTK_FILE_CHOOSER_ACTION_OPEN,
	                                     "Cancel",
	                                     GTK_RESPONSE_CANCEL,
	                                     "Open",
	                                     GTK_RESPONSE_ACCEPT,
	                                     NULL);
	
	chooser = GTK_FILE_CHOOSER(dialog);
	if (basePath != NULL) {
		gtk_file_chooser_set_current_folder(chooser, basePath);
	}
	
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	printf("%d\n", res);
	if (res == GTK_RESPONSE_ACCEPT) {
		char * filename;
		GtkFileChooser * chooser = GTK_FILE_CHOOSER(dialog);
		filename = gtk_file_chooser_get_filename(chooser);
		strncpy(outFilePath, filename, maxLength);
		g_free(filename);
		success = true;
	}
	
	gtk_widget_destroy(dialog);
	while (gtk_events_pending()) {
		gtk_main_iteration();
	}
	return success;
}

bool Shell_saveFileDialog(const char * basePath, const char * baseName, char * outFilePath, unsigned int maxLength) {
	GtkWidget * dialog;
	GtkFileChooser * chooser;
	gint res;
	bool success = false;
	
	dialog = gtk_file_chooser_dialog_new(NULL,
	                                     NULL,
	                                     GTK_FILE_CHOOSER_ACTION_SAVE,
	                                     "Cancel",
	                                     GTK_RESPONSE_CANCEL,
	                                     "Open",
	                                     GTK_RESPONSE_ACCEPT,
	                                     NULL);
	
	chooser = GTK_FILE_CHOOSER(dialog);
	gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
	if (basePath != NULL) {
		gtk_file_chooser_set_current_folder(chooser, basePath);
	}
	if (baseName != NULL) {
		gtk_file_chooser_set_current_name(chooser, baseName);
	}
	
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	printf("%d\n", res);
	if (res == GTK_RESPONSE_ACCEPT) {
		char * filename;
		GtkFileChooser * chooser = GTK_FILE_CHOOSER(dialog);
		filename = gtk_file_chooser_get_filename(chooser);
		strncpy(outFilePath, filename, maxLength);
		g_free(filename);
		success = true;
	}
	
	gtk_widget_destroy(dialog);
	while (gtk_events_pending()) {
		gtk_main_iteration();
	}
	return success;
}

void Shell_systemBeep() {
	gdk_beep();
}
	
#ifndef SEM_VALUE_MAX
#define SEM_VALUE_MAX 32767
#endif

struct threadFuncInvocation {
	int (* threadFunction)(void * context);
	void * context;
};

static void * callThreadFunc(void * context) {
	struct threadFuncInvocation * invocation = context;
	int (* threadFunction)(void * context);
	void * threadContext;
	
	threadFunction  = invocation->threadFunction;
	threadContext = invocation->context;
	free(invocation);
	return (void *) threadFunction(threadContext);
}

ShellThread Shell_createThread(int (* threadFunction)(void * context), void * context) {
	pthread_t thread;
	struct threadFuncInvocation * invocation;
	
	invocation = malloc(sizeof(struct threadFuncInvocation));
	invocation->threadFunction = threadFunction;
	invocation->context = context;
	pthread_create(&thread, NULL, callThreadFunc, invocation);
	return (ShellThread) thread;
}

void Shell_detachThread(ShellThread thread) {
	pthread_detach((pthread_t) thread);
}

int Shell_joinThread(ShellThread thread) {
	int status;
	void * returnValue;
	
	status = pthread_join((pthread_t) thread, &returnValue);
	if (status == 0) {
		return (int) returnValue;
	}
	return status;
}

void Shell_exitThread(int statusCode) {
	pthread_exit((void *) statusCode);
}

ShellThread Shell_getCurrentThread() {
	return (ShellThread) pthread_self();
}

ShellMutex Shell_createMutex() {
	pthread_mutex_t * mutex;
	pthread_mutexattr_t recursiveLock;

	pthread_mutexattr_init(&recursiveLock);
	pthread_mutexattr_settype(&recursiveLock, PTHREAD_MUTEX_RECURSIVE);
	mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, &recursiveLock);
	pthread_mutexattr_destroy(&recursiveLock);
	return (ShellMutex) mutex;
}

void Shell_disposeMutex(ShellMutex mutex) {
	pthread_mutex_destroy((pthread_mutex_t *) mutex);
	free(mutex);
}

void Shell_lockMutex(ShellMutex mutex) {
	pthread_mutex_lock((pthread_mutex_t *) mutex);
}

bool Shell_tryLockMutex(ShellMutex mutex) {
	return !pthread_mutex_trylock((pthread_mutex_t *) mutex);
}

void Shell_unlockMutex(ShellMutex mutex) {
	pthread_mutex_unlock((pthread_mutex_t *) mutex);
}

ShellSemaphore Shell_createSemaphore(unsigned int value) {
	sem_t * semaphore;
	
	semaphore = malloc(sizeof(sem_t));
	sem_init(semaphore, 0, value > SEM_VALUE_MAX ? SEM_VALUE_MAX : value);
	return (ShellSemaphore) semaphore;
}

void Shell_disposeSemaphore(ShellSemaphore semaphore) {
	sem_destroy((sem_t *) semaphore);
	free(semaphore);
}

void Shell_postSemaphore(ShellSemaphore semaphore) {
	sem_post((sem_t *) semaphore);
}

void Shell_waitSemaphore(ShellSemaphore semaphore) {
	sem_wait((sem_t *) semaphore);
}

bool Shell_tryWaitSemaphore(ShellSemaphore semaphore) {
	return !sem_trywait((sem_t *) semaphore);
}

unsigned int getGLXAttributes(struct GLXShellConfiguration configuration, int * outAttributes) {
	unsigned int attributeCount = 0;
	
	outAttributes[attributeCount++] = GLX_RGBA;
	if (configuration.displayMode.doubleBuffer) {
		outAttributes[attributeCount++] = GLX_DOUBLEBUFFER;
	}
	if (configuration.displayMode.depthBuffer) {
		outAttributes[attributeCount++] = GLX_DEPTH_SIZE;
		outAttributes[attributeCount++] = configuration.displayMode.depthSize;
	}
	if (configuration.displayMode.multisample) {
		outAttributes[attributeCount++] = GLX_SAMPLE_BUFFERS;
		outAttributes[attributeCount++] = configuration.displayMode.sampleBuffers;
		outAttributes[attributeCount++] = GLX_SAMPLES;
		outAttributes[attributeCount++] = configuration.displayMode.samples;
	}
	outAttributes[attributeCount++] = None;
	return attributeCount;
}

int main(int argc, char ** argv) {
	XVisualInfo * visualInfo = NULL;
	XSetWindowAttributes windowAttributes;
	int attributes[7];
	Colormap colormap;
	
	gtk_init(&argc, &argv);
	
	display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "XOpenDisplay failed\n");
		return EXIT_FAILURE;
	}
	
	screen = DefaultScreen(display);
	rootWindow = RootWindow(display, screen);
	
	configuration.windowX = 50;
	configuration.windowY = 80;
	configuration.windowWidth = 800;
	configuration.windowHeight = 600;
	configuration.windowTitle = "GLXShell";
	configuration.displayMode.doubleBuffer = true;
	configuration.displayMode.depthBuffer = false;
	configuration.displayMode.depthSize = 24;
	configuration.displayMode.stencilBuffer = false;
	configuration.displayMode.stencilSize = 0;
	configuration.displayMode.multisample = false;
	configuration.displayMode.sampleBuffers = 1;
	configuration.displayMode.samples = 4;
	
	GLXTarget_configure(argc, (const char **) argv, &configuration);
	
	getGLXAttributes(configuration, attributes);
	visualInfo = glXChooseVisual(display, 0, attributes);
	if (visualInfo == NULL && configuration.displayMode.multisample) {
		fprintf(stderr, "Requested pixel format unavailable; trying fallback non-multisampled format\n");
		configuration.displayMode.multisample = false;
		getGLXAttributes(configuration, attributes);
		visualInfo = glXChooseVisual(display, 0, attributes);
	}
	if (visualInfo == NULL) {
		fprintf(stderr, "Requested pixel format unavailable\n");
		return EXIT_FAILURE;
	}
	colormap = XCreateColormap(display, rootWindow, visualInfo->visual, AllocNone);
	windowAttributes.colormap = colormap;
	windowAttributes.event_mask = ExposureMask |
	                              KeyPressMask |
	                              KeyReleaseMask |
	                              ButtonPressMask |
	                              ButtonReleaseMask |
	                              PointerMotionMask |
	                              ButtonMotionMask |
	                              FocusChangeMask |
	                              StructureNotifyMask;
	windowAttributes.background_pixmap = None;
	windowAttributes.background_pixel = 0;
	windowAttributes.border_pixel = 0;
	
	window = XCreateWindow(display,
	                       rootWindow,
	                       configuration.windowX,
	                       configuration.windowY,
	                       configuration.windowWidth,
	                       configuration.windowHeight,
	                       0,
	                       visualInfo->depth,
	                       InputOutput,
	                       visualInfo->visual,
	                       CWColormap | CWEventMask | CWBackPixmap | CWBorderPixel,
	                       &windowAttributes);
	
	deleteWindowAtom = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &deleteWindowAtom, 1);
	XStoreName(display, window, configuration.windowTitle);
	
	XMapWindow(display, window);
	XMoveWindow(display, window, configuration.windowX, configuration.windowY);
	
	context = glXCreateContext(display, visualInfo, NULL, GL_TRUE);
	glXMakeCurrent(display, window, context);
	
	setVSync(vsyncWindow);
	
	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		fprintf(stderr, "Warning: glewInit() failed: %s\n", glewGetErrorString(glewStatus));
	}
	
	Target_init(argc, argv);
	
	return EXIT_SUCCESS;
}
