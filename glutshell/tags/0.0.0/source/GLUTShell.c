#include "shell/Shell.h"

#include <stdio.h>
#include <stdlib.h>

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

#include "shell/ShellKeyCodes.h"
#include "shell/Target.h"

static int buttonMask = 0;

void Shell_mainLoop() {
	glutMainLoop();
}

void Shell_redisplay() {
	glutPostRedisplay();
}

double Shell_getCurrentTime() {
#if defined(__APPLE__)
	static mach_timebase_info_data_t timebaseInfo;
	
	if (timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}
	return mach_absolute_time() * timebaseInfo.numer / timebaseInfo.denom * 0.000000001;
	
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

static void displayFunc() {
	GLenum error;
	
	Target_draw();
	glutSwapBuffers();
	
	error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("GL error: %s\n", gluErrorString(error));
	}
}

static void reshapeFunc(int newWidth, int newHeight) {
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

static void keyDownFunc(unsigned char charCode, int x, int y) {
	Target_keyDown(charCode, glutCharCodeToShellKeyCode(charCode));
}

static void keyUpFunc(unsigned char charCode, int x, int y) {
	Target_keyUp(charCode, glutCharCodeToShellKeyCode(charCode));
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
	
	keyCode = glutSpecialToShellKeyCode(key);
	if (keyCode != -1) {
		Target_keyDown(-1, keyCode);
	}
}

static void specialUpFunc(int key, int x, int y) {
	int keyCode;
	
	keyCode = glutSpecialToShellKeyCode(key);
	if (keyCode != -1) {
		Target_keyUp(-1, keyCode);
	}
}

static void mouseFunc(int button, int state, int x, int y) {
	int buttonNum;
	
	buttonNum = (button == GLUT_LEFT_BUTTON ? 0 : (button == GLUT_RIGHT_BUTTON ? 1 : 2));
	buttonMask |= 1 << buttonNum;
	
	if (state == GLUT_DOWN) {
		Target_mouseDown(buttonNum, x, y);
	} else {
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
#ifdef __APPLE__
	GLint VBL = 1;
#endif
	
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(2, 28);
	glutInitWindowSize(800, 600);
	glutCreateWindow(Target_getName());
	
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
	
	Target_init(argc, argv);
	
	return EXIT_SUCCESS;
}
