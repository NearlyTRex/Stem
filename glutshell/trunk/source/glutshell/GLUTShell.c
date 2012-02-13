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
#include "glutshell/GLUTTarget.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#if defined(__APPLE__)
#include <limits.h>
#include <CoreFoundation/CoreFoundation.h>
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glut.h>
#endif

#if defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#elif defined(__linux)
#include <time.h>
#elif defined(WIN32)
#include <windows.h>
#endif

#include "glgraphics/GLGraphics.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"
#include "shell/Target.h"

struct GLUTShellTimer {
	double interval;
	double nextFireTime;
	bool repeat;
	unsigned int id;
	void (* callback)(unsigned int timerID, void * context);
	void * context;
};

static unsigned int buttonMask = 0;
static unsigned int modifierMask = 0;
static bool inFullScreenMode = false;
static struct GLUTShellConfiguration configuration;
static unsigned int nextTimerID;
static size_t timerCount;
static struct GLUTShellTimer * timers;

void Shell_mainLoop() {
	glutMainLoop();
}

void Shell_redisplay() {
	glutPostRedisplay();
}

bool Shell_isFullScreen() {
	return inFullScreenMode;
}

bool Shell_setFullScreen(bool fullScreen) {
	if (fullScreen && !inFullScreenMode) {
		inFullScreenMode = true;
		configuration.windowX = glutGet(GLUT_WINDOW_X);
		configuration.windowY = glutGet(GLUT_WINDOW_Y);
		glutFullScreen();
		
	} else if (!fullScreen && inFullScreenMode) {
		glutPositionWindow(configuration.windowX, configuration.windowY);
		glutReshapeWindow(configuration.windowWidth, configuration.windowHeight);
		inFullScreenMode = false;
	}
	
	return true;
}

double Shell_getCurrentTime() {
#if defined(__APPLE__)
	static mach_timebase_info_data_t timebaseInfo;
	
	if (timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}
	return mach_absolute_time() * (double) timebaseInfo.numer / timebaseInfo.denom * 0.000000001;
	
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
#endif
}

const char * Shell_getResourcePath() {
#ifdef __APPLE__
	static char resourcePath[PATH_MAX];
	CFBundleRef mainBundle;
	CFURLRef resourcesDirectoryURL;
	
	mainBundle = CFBundleGetMainBundle();
	resourcesDirectoryURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	CFURLGetFileSystemRepresentation(resourcesDirectoryURL, true, (UInt8 *) resourcePath, PATH_MAX);
	CFRelease(resourcesDirectoryURL);
	
	return resourcePath;
#else
	return "Resources";
#endif
}

enum ShellBatteryState Shell_getBatteryState() {
	return ShellBatteryState_unknown;
}

float Shell_getBatteryLevel() {
	return -1.0f;
}

void Shell_getMainScreenSize(unsigned int * outWidth, unsigned int * outHeight) {
	if (outWidth != NULL) {
		*outWidth = glutGet(GLUT_SCREEN_WIDTH);
	}
	if (outHeight != NULL) {
		*outHeight = glutGet(GLUT_SCREEN_HEIGHT);
	}
}

static void idleFunc() {
	unsigned int timerIndex, timerIndex2;
	double currentTime;
	
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
				if (timerCount == 0) {
					glutIdleFunc(NULL);
				}
			}
		}
	}
}

unsigned int Shell_setTimer(double interval, bool repeat, void (* callback)(unsigned int timerID, void * context), void * context) {
	timers = realloc(timers, sizeof(struct GLUTShellTimer) * (timerCount + 1));
	timers[timerCount].interval = interval;
	timers[timerCount].nextFireTime = Shell_getCurrentTime() + interval;
	timers[timerCount].repeat = repeat;
	timers[timerCount].id = nextTimerID++;
	timers[timerCount].callback = callback;
	timers[timerCount].context = context;
	glutIdleFunc(idleFunc);
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
			if (timerCount == 0) {
				glutIdleFunc(NULL);
			}
			break;
		}
	}
}

static void displayFunc() {
#ifdef DEBUG
	GLenum error;
#endif
	
	if (Target_draw()) {
		glutSwapBuffers();
	}
	
#ifdef DEBUG
	error = glGetError();
	while (error != GL_NO_ERROR) {
		fprintf(stderr, "GL error: %s\n", gluErrorString(error));
		error = glGetError();
	}
#endif
}

static void reshapeFunc(int newWidth, int newHeight) {
	if (!inFullScreenMode) {
		configuration.windowWidth = newWidth;
		configuration.windowHeight = newHeight;
	}
	
	Target_resized(newWidth, newHeight);
	
#ifndef __APPLE__
	displayFunc();
#endif
}

// HACK: GLUTShell can't truly distinguish key code from char code
static int glutCharCodeToShellKeyCode(int key) {
	switch (key) {
		case '\x1B':
			return KEYBOARD_ESCAPE;
		case '`':
		case '~':
			return KEYBOARD_GRAVE_ACCENT_AND_TILDE;
		case '1':
		case '!':
			return KEYBOARD_1;
		case '2':
		case '@':
			return KEYBOARD_2;
		case '3':
		case '#':
			return KEYBOARD_3;
		case '4':
		case '$':
			return KEYBOARD_4;
		case '5':
		case '%':
			return KEYBOARD_5;
		case '6':
		case '^':
			return KEYBOARD_6;
		case '7':
		case '&':
			return KEYBOARD_7;
		case '8':
		case '*':
			return KEYBOARD_8;
		case '9':
		case '(':
			return KEYBOARD_9;
		case '0':
		case ')':
			return KEYBOARD_0;
		case '-':
		case '_':
			return KEYBOARD_HYPHEN;
		case '=':
		case '+':
			return KEYBOARD_EQUAL_SIGN;
		case '\x7F':
			return KEYBOARD_DELETE_OR_BACKSPACE;
		case '\t':
			return KEYBOARD_TAB;
		case 'q':
		case 'Q':
			return KEYBOARD_Q;
		case 'w':
		case 'W':
			return KEYBOARD_W;
		case 'e':
		case 'E':
			return KEYBOARD_E;
		case 'r':
		case 'R':
			return KEYBOARD_R;
		case 't':
		case 'T':
			return KEYBOARD_T;
		case 'y':
		case 'Y':
			return KEYBOARD_Y;
		case 'u':
		case 'U':
			return KEYBOARD_U;
		case 'i':
		case 'I':
			return KEYBOARD_I;
		case 'o':
		case 'O':
			return KEYBOARD_O;
		case 'p':
		case 'P':
			return KEYBOARD_P;
		case '[':
		case '{':
			return KEYBOARD_OPEN_BRACKET;
		case ']':
		case '}':
			return KEYBOARD_CLOSE_BRACKET;
		case '\\':
		case '|':
			return KEYBOARD_BACKSLASH;
		case 'a':
		case 'A':
			return KEYBOARD_A;
		case 's':
		case 'S':
			return KEYBOARD_S;
		case 'd':
		case 'D':
			return KEYBOARD_D;
		case 'f':
		case 'F':
			return KEYBOARD_F;
		case 'g':
		case 'G':
			return KEYBOARD_G;
		case 'h':
		case 'H':
			return KEYBOARD_H;
		case 'j':
		case 'J':
			return KEYBOARD_J;
		case 'k':
		case 'K':
			return KEYBOARD_K;
		case 'l':
		case 'L':
			return KEYBOARD_L;
		case ';':
		case ':':
			return KEYBOARD_SEMICOLON;
		case '\'':
		case '"':
			return KEYBOARD_QUOTE;
		case '\r':
		case '\n':
			return KEYBOARD_RETURN_OR_ENTER;
		case 'z':
		case 'Z':
			return KEYBOARD_Z;
		case 'x':
		case 'X':
			return KEYBOARD_X;
		case 'c':
		case 'C':
			return KEYBOARD_C;
		case 'v':
		case 'V':
			return KEYBOARD_V;
		case 'b':
		case 'B':
			return KEYBOARD_B;
		case 'n':
		case 'N':
			return KEYBOARD_N;
		case 'm':
		case 'M':
			return KEYBOARD_M;
		case ',':
		case '<':
			return KEYBOARD_COMMA;
		case '.':
		case '>':
			return KEYBOARD_PERIOD;
		case '/':
		case '?':
			return KEYBOARD_SLASH;
		case ' ':
			return KEYBOARD_SPACEBAR;
		default:
			return -1;
	}
}

static unsigned int glutModifiersToShellModifiers(unsigned int glutModifiers) {
	unsigned int result = 0;
	
	if ((glutModifiers & GLUT_ACTIVE_SHIFT)) {
		result |= MODIFIER_SHIFT_BIT;
	}
	if ((glutModifiers & GLUT_ACTIVE_CTRL)) {
		result |= MODIFIER_CONTROL_BIT;
	}
	if ((glutModifiers & GLUT_ACTIVE_ALT)) {
		result |= MODIFIER_ALT_BIT;
	}
	return result;
}

static void keyDownFunc(unsigned char charCode, int x, int y) {
	unsigned int newModifierMask;
	
	newModifierMask = glutModifiersToShellModifiers(glutGetModifiers());
	if (newModifierMask != modifierMask) {
		modifierMask = newModifierMask;
		Target_keyModifiersChanged(modifierMask);
	}
	Target_keyDown(charCode, glutCharCodeToShellKeyCode(charCode), modifierMask);
}

static void keyUpFunc(unsigned char charCode, int x, int y) {
	unsigned int newModifierMask;
	
	newModifierMask = glutModifiersToShellModifiers(glutGetModifiers());
	if (newModifierMask != modifierMask) {
		modifierMask = newModifierMask;
		Target_keyModifiersChanged(modifierMask);
	}
	Target_keyUp(glutCharCodeToShellKeyCode(charCode), modifierMask);
}

static int glutSpecialToShellKeyCode(int key) {
	switch (key) {
		case GLUT_KEY_F1:
			return KEYBOARD_F1;
		case GLUT_KEY_F2:
			return KEYBOARD_F2;
		case GLUT_KEY_F3:
			return KEYBOARD_F3;
		case GLUT_KEY_F4:
			return KEYBOARD_F4;
		case GLUT_KEY_F5:
			return KEYBOARD_F5;
		case GLUT_KEY_F6:
			return KEYBOARD_F6;
		case GLUT_KEY_F7:
			return KEYBOARD_F7;
		case GLUT_KEY_F8:
			return KEYBOARD_F8;
		case GLUT_KEY_F9:
			return KEYBOARD_F9;
		case GLUT_KEY_F10:
			return KEYBOARD_F10;
		case GLUT_KEY_F11:
			return KEYBOARD_F11;
		case GLUT_KEY_F12:
			return KEYBOARD_F12;
		case GLUT_KEY_LEFT:
			return KEYBOARD_LEFT_ARROW;
		case GLUT_KEY_UP:
			return KEYBOARD_UP_ARROW;
		case GLUT_KEY_RIGHT:
			return KEYBOARD_RIGHT_ARROW;
		case GLUT_KEY_DOWN:
			return KEYBOARD_DOWN_ARROW;
		case GLUT_KEY_PAGE_UP:
			return KEYBOARD_PAGE_UP;
		case GLUT_KEY_PAGE_DOWN:
			return KEYBOARD_PAGE_DOWN;
		case GLUT_KEY_HOME:
			return KEYBOARD_HOME;
		case GLUT_KEY_END:
			return KEYBOARD_END;
		case GLUT_KEY_INSERT:
			return KEYBOARD_INSERT;
		default:
			return -1;
	}
}

static void specialDownFunc(int key, int x, int y) {
	int keyCode;
	unsigned int newModifierMask;
	
	newModifierMask = glutModifiersToShellModifiers(glutGetModifiers());
	if (newModifierMask != modifierMask) {
		modifierMask = newModifierMask;
		Target_keyModifiersChanged(modifierMask);
	}
	
	keyCode = glutSpecialToShellKeyCode(key);
	if (keyCode != -1) {
		Target_keyDown(0, keyCode, modifierMask);
	}
}

static void specialUpFunc(int key, int x, int y) {
	int keyCode;
	unsigned int newModifierMask;
	
	newModifierMask = glutModifiersToShellModifiers(glutGetModifiers());
	if (newModifierMask != modifierMask) {
		modifierMask = newModifierMask;
		Target_keyModifiersChanged(modifierMask);
	}
	
	keyCode = glutSpecialToShellKeyCode(key);
	if (keyCode != -1) {
		Target_keyUp(keyCode, modifierMask);
	}
}

static void mouseFunc(int button, int state, int x, int y) {
	int buttonNum;
	unsigned int newModifierMask;
	
	newModifierMask = glutModifiersToShellModifiers(glutGetModifiers());
	if (newModifierMask != modifierMask) {
		modifierMask = newModifierMask;
		Target_keyModifiersChanged(modifierMask);
	}
	
	buttonNum = (button == GLUT_LEFT_BUTTON ? 0 : (button == GLUT_RIGHT_BUTTON ? 1 : 2));
	
	if (state == GLUT_DOWN) {
		buttonMask |= 1 << buttonNum;
		Target_mouseDown(buttonNum, x, y);
	} else {
		buttonMask &= ~(1 << buttonNum);
		Target_mouseUp(buttonNum, x, y);
	}
}

static void motionFunc(int x, int y) {
	if (buttonMask != 0) {
		Target_mouseDragged(buttonMask, x, y);
	} else {
		Target_mouseMoved(x, y);
	}
}

int main(int argc, char ** argv) {
	unsigned int displayMode;
#ifdef __APPLE__
	GLint VBL = 1;
#endif
	char workingDir[PATH_MAX];
	int chdirReturn;
	char * getcwdReturn;
	
	getcwdReturn = getcwd(workingDir, PATH_MAX);
	glutInit(&argc, argv);
	chdirReturn = chdir(workingDir);
	
	configuration.windowX = 2;
	configuration.windowY = 28;
	configuration.windowWidth = 800;
	configuration.windowHeight = 600;
	configuration.windowTitle = "GLUTShell";
	configuration.displayMode.doubleBuffer = true;
	configuration.displayMode.depthBuffer = false;
	configuration.displayMode.stencilBuffer = false;
	configuration.displayMode.accumBuffer = false;
	configuration.displayMode.multisample = false;
	
	GLUTTarget_configure(argc, (const char **) argv, &configuration);
	
	displayMode = GLUT_RGBA;
	if (configuration.displayMode.doubleBuffer) {
		displayMode |= GLUT_DOUBLE;
	}
	if (configuration.displayMode.depthBuffer) {
		displayMode |= GLUT_DEPTH;
	}
	if (configuration.displayMode.stencilBuffer) {
		displayMode |= GLUT_STENCIL;
	}
	if (configuration.displayMode.accumBuffer) {
		displayMode |= GLUT_ACCUM;
	}
	if (configuration.displayMode.multisample) {
		displayMode |= GLUT_MULTISAMPLE;
	}
	
	glutInitDisplayMode(displayMode);
	glutInitWindowPosition(configuration.windowX, configuration.windowY);
	glutInitWindowSize(configuration.windowWidth, configuration.windowHeight);
	glutCreateWindow(configuration.windowTitle);
	
	glutReshapeFunc(reshapeFunc);
	glutDisplayFunc(displayFunc);
	glutKeyboardFunc(keyDownFunc);
	glutKeyboardUpFunc(keyUpFunc);
	glutSpecialFunc(specialDownFunc);
	glutSpecialUpFunc(specialUpFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutPassiveMotionFunc(motionFunc);
	
#ifdef __APPLE__
	CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &VBL);
#endif
	
	GLGraphics_init(GL_API_VERSION_DESKTOP_1);
	
	Target_init(argc, argv);
	
	return EXIT_SUCCESS;
}
