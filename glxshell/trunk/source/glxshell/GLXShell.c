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

#include "glxshell/GLXShell.h"
#include "glxshell/GLXTarget.h"
#include "glgraphics/GLGraphics.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"
#include "shell/ShellThreads.h"
#include "shell/Target.h"
#include <GL/glx.h>
#include <GL/glxext.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

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

static unsigned int buttonMask = 0;
static unsigned int modifierMask = 0;
static bool inFullScreenMode = false;
static bool vsyncWindow = VSYNC_DEFAULT_WINDOW, vsyncFullscreen = VSYNC_DEFAULT_FULLSCREEN;
static struct GLXShellConfiguration configuration;
static unsigned int nextTimerID;
static size_t timerCount;
static struct GLXShellTimer * timers;

void Shell_mainLoop() {
}

void Shell_redisplay() {
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

bool Shell_setFullScreen(bool fullScreen) {
	return false;
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
		strncpy(supportPath, getenv("HOME"), PATH_MAX);
	} else {
		snprintf(supportPath, PATH_MAX, "%s/.%s", getenv("HOME"), subdirectory);
	}
	mkdir(supportPath, 0777);
	return supportPath;
}

enum ShellBatteryState Shell_getBatteryState() {
	return ShellBatteryState_unknown;
}

float Shell_getBatteryLevel() {
	return -1.0f;
}

void Shell_getMainScreenSize(unsigned int * outWidth, unsigned int * outHeight) {
}

unsigned int Shell_setTimer(double interval, bool repeat, void (* callback)(unsigned int timerID, void * context), void * context) {
	return 0;
}

void Shell_cancelTimer(unsigned int timerID) {
}

static bool cursorHiddenByHide;
static bool showCursorOnNextMouseMove;
static int lastUnhiddenCursor = ShellCursor_arrow;
static bool mouseDeltaMode;
static int restoreMouseX, restoreMouseY;
static int lastMouseX, lastMouseY;
static int ignoreX = INT_MAX, ignoreY = INT_MAX;

void Shell_setCursorVisible(bool visible) {
	if (visible && cursorHiddenByHide) {
		cursorHiddenByHide = false;
		if (!mouseDeltaMode) {
			//Shell_setCursor(lastUnhiddenCursor);
		}
	} else if (!visible && !cursorHiddenByHide) {
		cursorHiddenByHide = true;
		//glutSetCursor(GLUT_CURSOR_NONE);
	}
}

void Shell_hideCursorUntilMouseMoves() {
	if (!cursorHiddenByHide) {
		showCursorOnNextMouseMove = true;
		//glutSetCursor(GLUT_CURSOR_NONE);
	}
}

void Shell_setCursor(int cursor) {
	lastUnhiddenCursor = cursor;
	if (cursorHiddenByHide || mouseDeltaMode) {
		return;
	}
	switch (cursor) {
		case ShellCursor_arrow:
			//glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			break;
		case ShellCursor_iBeam:
			//glutSetCursor(GLUT_CURSOR_TEXT);
			break;
		case ShellCursor_crosshair:
			//glutSetCursor(GLUT_CURSOR_CROSSHAIR);
			break;
		case ShellCursor_hand:
			//glutSetCursor(GLUT_CURSOR_INFO);
			break;
		case ShellCursor_wait:
			//glutSetCursor(GLUT_CURSOR_WAIT);
			break;
	}
}

static void warpPointerAndIgnoreEvent(int x, int y) {
	ignoreX = x;
	ignoreY = y;
	lastMouseX = x;
	lastMouseY = y;
	//glutWarpPointer(x, y);
}

void Shell_setMouseDeltaMode(bool deltaMode) {
	if (!mouseDeltaMode && deltaMode) {
		restoreMouseX = lastMouseX;
		restoreMouseY = lastMouseY;
		warpPointerAndIgnoreEvent(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
		//glutSetCursor(GLUT_CURSOR_NONE);
		mouseDeltaMode = true;
		
	} else if (mouseDeltaMode && !deltaMode) {
		warpPointerAndIgnoreEvent(restoreMouseX, restoreMouseY);
		mouseDeltaMode = false;
		//Shell_setCursor(lastUnhiddenCursor);
	}
}

void GLXShell_setVSync(bool sync, bool fullscreen) {
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

static void displayFunc() {
#ifdef DEBUG
	GLenum error;
#endif
	
	if (Target_draw()) {
		//glutSwapBuffers();
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
	//displayFunc();
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

int main(int argc, char ** argv) {
	unsigned int displayMode;
	char workingDir[PATH_MAX];
	
	getcwd(workingDir, PATH_MAX);
	//glutInit(&argc, argv);
	chdir(workingDir);
	
	configuration.windowX = 2;
	configuration.windowY = 28;
	configuration.windowWidth = 800;
	configuration.windowHeight = 600;
	configuration.windowTitle = "GLXShell";
	configuration.displayMode.doubleBuffer = true;
	configuration.displayMode.depthBuffer = false;
	configuration.displayMode.stencilBuffer = false;
	configuration.displayMode.accumBuffer = false;
	configuration.displayMode.multisample = false;
	
	GLXTarget_configure(argc, (const char **) argv, &configuration);
	
	/*
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
	*/
	
	setVSync(vsyncWindow);
	GLGraphics_init(GL_API_VERSION_DESKTOP_1);
	
	Target_init(argc, argv);
	
	return EXIT_SUCCESS;
}
