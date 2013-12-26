#include "shell/Shell.h"
#include "glutshell/GLUTShell.h"
#include "glutshell/GLUTTarget.h"

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include "glgraphics/GLGraphics.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"
#include "shell/ShellThreads.h"
#include "shell/Target.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#if defined(WIN32)
#include <windows.h>
#define msleep(ms) Sleep(ms)
#else
#define msleep(ms) usleep((ms) * 1000)
#endif

static unsigned int timer1ID = UINT_MAX, timer2ID = UINT_MAX;
static bool deltaMode;

void GLUTTarget_configure(int argc, const char ** argv, struct GLUTShellConfiguration * configuration) {
	int argIndex;
	char workingDir[PATH_MAX];
	
	printf("GLUTTarget_configure(%d", argc);
	for (argIndex = 0; argIndex < argc; argIndex++) {
		printf(", \"%s\"", argv[argIndex]);
	}
	printf(", %p)\n", configuration);
	
	printf("configuration->windowX: %d\n", configuration->windowX);
	printf("configuration->windowY: %d\n", configuration->windowY);
	printf("configuration->windowWidth: %d\n", configuration->windowWidth);
	printf("configuration->windowHeight: %d\n", configuration->windowHeight);
	printf("configuration->windowTitle: %s\n", configuration->windowTitle);
	printf("configuration->displayMode.doubleBuffer: %s\n", configuration->displayMode.doubleBuffer ? "true" : "false");
	printf("configuration->displayMode.depthBuffer: %s\n", configuration->displayMode.depthBuffer ? "true" : "false");
	printf("configuration->displayMode.stencilBuffer: %s\n", configuration->displayMode.stencilBuffer ? "true" : "false");
	printf("configuration->displayMode.accumBuffer: %s\n", configuration->displayMode.accumBuffer ? "true" : "false");
	printf("configuration->displayMode.multisample: %s\n", configuration->displayMode.multisample ? "true" : "false");
	
	configuration->windowTitle = "GLUTShell Test Harness";
	printf("configuration->windowTitle = \"%s\"\n", configuration->windowTitle);
	
	printf("getcwd(): %s\n", getcwd(workingDir, PATH_MAX));
}

void Target_init() {
	printf("Target_init()\n");
	printf("GLGraphics_getOpenGLAPIVersion(): %d\n", GLGraphics_getOpenGLAPIVersion());
	Shell_mainLoop();
}

bool Target_draw() {
	printf("Target_draw()\n");
	glClearColor(0.0f, 0.25f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return true;
}

static void timerCallback(unsigned int timerID, void * context) {
	printf("timerCallback(timerID, \"%s\")\n", (char *) context);
	if (timerID == timer2ID) {
		timer2ID = UINT_MAX;
	}
}

static int threadFunc1(void * context) {
	printf("Secondary thread 1 %p begin\n", Shell_getCurrentThread());
	Shell_lockMutex(context);
	msleep(1000);
	Shell_unlockMutex(context);
	printf("Secondary thread 1 %p end\n", Shell_getCurrentThread());
	return 0;
}

static int threadFunc2(void * context) {
	printf("Secondary thread 2 %p begin\n", Shell_getCurrentThread());
	msleep(1000);
	Shell_postSemaphore(context);
	printf("Secondary thread 2 %p end\n", Shell_getCurrentThread());
	return 0;
}

static int threadFunc3(void * context) {
	printf("Secondary thread 3 %p begin\n", Shell_getCurrentThread());
	msleep(1000);
	printf("Secondary thread 3 %p exit\n", Shell_getCurrentThread());
	Shell_exitThread(2);
	printf("Secondary thread 3 %p end (bad!)\n", Shell_getCurrentThread());
	return 0;
}

void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags) {
	printf("Target_keyDown(%u, %u, 0x%X)\n", charCode, keyCode, modifierFlags);
	if (keyCode == KEYBOARD_Q) {
		ShellThread thread;
		ShellMutex mutex;
		ShellSemaphore semaphore;
		bool success;
		int status;
		
		printf("Current thread: %p\n\n", Shell_getCurrentThread());
		
		mutex = Shell_createMutex();
		printf("Created mutex %p\n", mutex);
		success = Shell_tryLockMutex(mutex);
		printf("Lock acquired: %s (expected true)\n", success ? "true" : "false");
		success = Shell_tryLockMutex(mutex);
		printf("Lock acquired: %s (expected true)\n\n", success ? "true" : "false");
		Shell_unlockMutex(mutex);
		Shell_unlockMutex(mutex);
		
		thread = Shell_createThread(threadFunc1, mutex);
		Shell_detachThread(thread);
		printf("Created thread %p at %f\n", thread, Shell_getCurrentTime());
		msleep(500);
		Shell_lockMutex(mutex);
		printf("Acquired lock at %f\n\n", Shell_getCurrentTime());
		Shell_unlockMutex(mutex);
		
		semaphore = Shell_createSemaphore(0);
		printf("Try wait semaphore: %s (expected false)\n", Shell_tryWaitSemaphore(semaphore) ? "true" : "false");
		thread = Shell_createThread(threadFunc2, semaphore);
		Shell_detachThread(thread);
		printf("Created thread %p at %f\n", thread, Shell_getCurrentTime());
		printf("Try wait semaphore: %s (expected false)\n", Shell_tryWaitSemaphore(semaphore) ? "true" : "false");
		Shell_waitSemaphore(semaphore);
		printf("Acquired semaphore at %f\n\n", Shell_getCurrentTime());
		
		thread = Shell_createThread(threadFunc3, mutex);
		printf("Created thread %p at %f\n", thread, Shell_getCurrentTime());
		status = Shell_joinThread(thread);
		printf("Joined thread at %f, with status %d\n\n", Shell_getCurrentTime(), status);
		
		Shell_disposeMutex(mutex);
		Shell_disposeSemaphore(semaphore);
		
	} else if (keyCode == KEYBOARD_T) {
		printf("Shell_getCurrentTime(): %f\n", Shell_getCurrentTime());
		
	} else if (keyCode == KEYBOARD_R) {
		printf("Shell_getResourcePath(): %s\n", Shell_getResourcePath());
		
	} else if (keyCode == KEYBOARD_Y) {
		printf("Shell_getSupportPath(NULL): %s\n", Shell_getSupportPath(NULL));
		printf("Shell_getSupportPath(\"glutshell\"): %s\n", Shell_getSupportPath("glutshell"));
		
	} else if (keyCode == KEYBOARD_D) {
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_F) {
		printf("Shell_setFullScreen(true): %s\n", Shell_setFullScreen(true) ? "true" : "false");
		
	} else if (keyCode == KEYBOARD_W) {
		printf("Shell_setFullScreen(false): %s\n", Shell_setFullScreen(false) ? "true" : "false");
		
	} else if (keyCode == KEYBOARD_G) {
		printf("Shell_isFullScreen(): %s\n", Shell_isFullScreen() ? "true" : "false");
		
	} else if (keyCode == KEYBOARD_B) {
		printf("Shell_getBatteryState(): %d\n", Shell_getBatteryState());
		printf("Shell_getBatteryLevel(): %f\n", Shell_getBatteryLevel());
		
	} else if (keyCode == KEYBOARD_X) {
		unsigned int width = 0, height = 0;
		
		Shell_getMainScreenSize(&width, &height);
		printf("Shell_getMainScreenSize(%u, %u)\n", width, height);
		
	} else if (keyCode == KEYBOARD_COMMA) {
		if (timer1ID == UINT_MAX) {
			timer1ID = Shell_setTimer(1.0, true, timerCallback, "Timer 1 context");
			printf("Shell_setTimer(1.0, true, %p, \"Timer 1 context\"): %u\n", timerCallback, timer1ID);
		} else {
			printf("Shell_cancelTimer(%u)\n", timer1ID);
			Shell_cancelTimer(timer1ID);
			timer1ID = UINT_MAX;
		}
		
	} else if (keyCode == KEYBOARD_PERIOD) {
		if (timer2ID == UINT_MAX) {
			timer2ID = Shell_setTimer(2.0, false, timerCallback, "Timer 2 context");
			printf("Shell_setTimer(2.0, false, %p, \"Timer 2 context\"): %u\n", timerCallback, timer2ID);
		} else {
			printf("Shell_cancelTimer(%u)\n", timer2ID);
			Shell_cancelTimer(timer2ID);
			timer2ID = UINT_MAX;
		}
		
	} else if (keyCode == KEYBOARD_SEMICOLON) {
		deltaMode = !deltaMode;
		Shell_setMouseDeltaMode(deltaMode);
		printf("Shell_setMouseDeltaMode(%s)\n", deltaMode ? "true" : "false");
		
	} else if (keyCode == KEYBOARD_H) {
		Shell_setCursorVisible(false);
		
	} else if (keyCode == KEYBOARD_S) {
		Shell_setCursorVisible(true);
		
	} else if (keyCode == KEYBOARD_M) {
		Shell_hideCursorUntilMouseMoves();
		
	} else if (keyCode == KEYBOARD_0 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(ShellCursor_arrow);
		
	} else if (keyCode == KEYBOARD_1 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(ShellCursor_iBeam);
		
	} else if (keyCode == KEYBOARD_2 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(ShellCursor_crosshair);
		
	} else if (keyCode == KEYBOARD_3 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(ShellCursor_hand);
		
	} else if (keyCode == KEYBOARD_4 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(ShellCursor_wait);
		
	} else if (keyCode == KEYBOARD_5 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_rightArrow);
		
	} else if (keyCode == KEYBOARD_6 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_destroy);
		
	} else if (keyCode == KEYBOARD_7 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_help);
		
	} else if (keyCode == KEYBOARD_8 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_cycle);
		
	} else if (keyCode == KEYBOARD_9 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_spray);
		
	} else if (keyCode == KEYBOARD_1 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_upDown);
		
	} else if (keyCode == KEYBOARD_2 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_leftRight);
		
	} else if (keyCode == KEYBOARD_3 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_topSide);
		
	} else if (keyCode == KEYBOARD_4 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_bottomSide);
		
	} else if (keyCode == KEYBOARD_5 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_leftSide);
		
	} else if (keyCode == KEYBOARD_6 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_rightSide);
		
	} else if (keyCode == KEYBOARD_7 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_topLeftCorner);
		
	} else if (keyCode == KEYBOARD_8 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_topRightCorner);
		
	} else if (keyCode == KEYBOARD_9 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_bottomRightCorner);
		
	} else if (keyCode == KEYBOARD_0 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_bottomLeftCorner);
		
	} else if (keyCode == KEYBOARD_HYPHEN && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_inherit);
		
	} else if (keyCode == KEYBOARD_EQUAL_SIGN && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(GLUTShellCursor_fullCrosshair);
	}
}

void Target_keyUp(unsigned int keyCode, unsigned int modifierFlags) {
	printf("Target_keyUp(%u, 0x%X)\n", keyCode, modifierFlags);
}

void Target_keyModifiersChanged(unsigned int modifierFlags) {
	printf("Target_keyModifiersChanged(0x%X)\n", modifierFlags);
}

void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	printf("Target_mouseDown(%d, %f, %f)\n", buttonNumber, x, y);
}

void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
	printf("Target_mouseUp(%d, %f, %f)\n", buttonNumber, x, y);
}

void Target_mouseMoved(float x, float y) {
	printf("Target_mouseMoved(%f, %f)\n", x, y);
}

void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	printf("Target_mouseDragged(0x%X, %f, %f)\n", buttonMask, x, y);
}

void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	printf("Target_resized(%d, %d)\n", newWidth, newHeight);
	glViewport(0, 0, newWidth, newHeight);
}

void Target_backgrounded() {
	printf("Target_backgrounded()\n");
}

void Target_foregrounded() {
	printf("Target_foregrounded()\n");
}
