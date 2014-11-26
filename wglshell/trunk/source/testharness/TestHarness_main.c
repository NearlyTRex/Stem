#include "shell/Shell.h"
#include "wglshell/WGLShell.h"
#include "wglshell/WGLTarget.h"

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include "glgraphics/GLGraphics.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"
#include "shell/ShellThreads.h"

#include <GL/gl.h>

#include <windows.h>
#define msleep(ms) Sleep(ms)

static unsigned int timer1ID = UINT_MAX, timer2ID = UINT_MAX;
static bool deltaMode;
static bool syncFullscreen = true, syncWindow = true;
static bool printMouseMoved = true;

static void registerShellCallbacks();
static void unregisterShellCallbacks();

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

static void restoreCallbacksTimer(unsigned int timerID, void * context) {
	registerShellCallbacks();
	printf("Restored event callbacks\n");
}

static void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags) {
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
		
	} else if (keyCode == KEYBOARD_U) {
		Shell_openURL("http://ludobloom.com/");
		
	} else if (keyCode == KEYBOARD_D) {
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_E) {
		unsigned int displayIndex = Shell_getDisplayIndexFromWindow();
		printf("Shell_enterFullScreen(%u): %s\n", displayIndex, Shell_enterFullScreen(displayIndex) ? "true" : "false");
		
	} else if (keyCode == KEYBOARD_W) {
		Shell_exitFullScreen();
		
	} else if (keyCode == KEYBOARD_G) {
		printf("Shell_isFullScreen(): %s\n", Shell_isFullScreen() ? "true" : "false");
		
	} else if (keyCode == KEYBOARD_B) {
		printf("Shell_getBatteryState(): %d\n", Shell_getBatteryState());
		printf("Shell_getBatteryLevel(): %f\n", Shell_getBatteryLevel());
		
	} else if (keyCode == KEYBOARD_X) {
		int x = 0, y = 0;
		unsigned int width = 0, height = 0;
		static unsigned int screenIndex;
		
		screenIndex %= Shell_getDisplayCount();
		Shell_getDisplayBounds(screenIndex, &x, &y, &width, &height);
		printf("Shell_getDisplayBounds(%u): %d, %d, %u, %u\n", screenIndex, x, y, width, height);
		screenIndex++;
		
	} else if (keyCode == KEYBOARD_C) {
		printf("Shell_getDisplayCount(): %u\n", Shell_getDisplayCount());
		
	} else if (keyCode == KEYBOARD_V) {
		bool sync, fullscreen;
		
		fullscreen = Shell_isFullScreen();
		if (fullscreen) {
			syncFullscreen = !syncFullscreen;
			sync = syncFullscreen;
		} else {
			syncWindow = !syncWindow;
			sync = syncWindow;
		}
		Shell_setVSync(sync, fullscreen);
		printf("Shell_setVSync(%s, %s)\n", sync ? "true" : "false", fullscreen ? "true" : "false");
		
	} else if (keyCode == KEYBOARD_O) {
		char filePath[PATH_MAX];
		bool success;
		
		success = Shell_openFileDialog(NULL, filePath, PATH_MAX);
		if (success) {
			printf("Shell_openFileDialog returned true with path \"%s\"\n", filePath);
		} else {
			printf("Shell_openFileDialog returned false\n");
		}
		
	} else if (keyCode == KEYBOARD_P) {
		char filePath[PATH_MAX];
		bool success;
		
		success = Shell_saveFileDialog(NULL, "test", filePath, PATH_MAX);
		if (success) {
			printf("Shell_saveFileDialog returned true with path \"%s\"\n", filePath);
		} else {
			printf("Shell_saveFileDialog returned false\n");
		}
		
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
		
	} else if (keyCode == KEYBOARD_N) {
		printMouseMoved = !printMouseMoved;
		printf("Mouse move messages %s\n", printMouseMoved ? "enabled" : "disabled");
		
	} else if (keyCode == KEYBOARD_DELETE_OR_BACKSPACE) {
		unregisterShellCallbacks();
		printf("Removed all event callbacks for 5 seconds\n");
		Shell_setTimer(5.0, false, restoreCallbacksTimer, NULL);
		
	} else if (keyCode == KEYBOARD_SPACEBAR) {
		Shell_systemBeep();
		
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
		Shell_setCursor(WGLShellCursor_appStarting);
		
	} else if (keyCode == KEYBOARD_6 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(WGLShellCursor_no);
		
	} else if (keyCode == KEYBOARD_7 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(WGLShellCursor_sizeAll);
		
	} else if (keyCode == KEYBOARD_8 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(WGLShellCursor_sizeNESW);
		
	} else if (keyCode == KEYBOARD_9 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(WGLShellCursor_sizeNS);
		
	} else if (keyCode == KEYBOARD_1 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(WGLShellCursor_sizeNWSE);
		
	} else if (keyCode == KEYBOARD_2 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(WGLShellCursor_sizeWE);
		
	} else if (keyCode == KEYBOARD_3 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(WGLShellCursor_upArrow);
		
	} else if (keyCode == KEYBOARD_4 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		Shell_setCursor(WGLShellCursor_help);
	}
}

static void Target_keyUp(unsigned int keyCode, unsigned int modifierFlags) {
	printf("Target_keyUp(%u, 0x%X)\n", keyCode, modifierFlags);
}

static void Target_keyModifiersChanged(unsigned int modifierFlags) {
	printf("Target_keyModifiersChanged(0x%X)\n", modifierFlags);
}

static void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	printf("Target_mouseDown(%d, %f, %f)\n", buttonNumber, x, y);
}

static void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
	printf("Target_mouseUp(%d, %f, %f)\n", buttonNumber, x, y);
}

static void Target_mouseMoved(float x, float y) {
	if (printMouseMoved) {
		printf("Target_mouseMoved(%f, %f)\n", x, y);
	}
}

static void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	printf("Target_mouseDragged(0x%X, %f, %f)\n", buttonMask, x, y);
}

static void Target_scrollWheel(int deltaX, int deltaY) {
	printf("Target_scrollWheel(%d, %d)\n", deltaX, deltaY);
}

static void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	printf("Target_resized(%d, %d)\n", newWidth, newHeight);
	glViewport(0, 0, newWidth, newHeight);
}

static void Target_backgrounded() {
	printf("Target_backgrounded()\n");
}

static void Target_foregrounded() {
	printf("Target_foregrounded()\n");
}

static void registerShellCallbacks() {
	Shell_drawFunc(Target_draw);
	Shell_resizeFunc(Target_resized);
	Shell_keyDownFunc(Target_keyDown);
	Shell_keyUpFunc(Target_keyUp);
	Shell_keyModifiersChangedFunc(Target_keyModifiersChanged);
	Shell_mouseDownFunc(Target_mouseDown);
	Shell_mouseUpFunc(Target_mouseUp);
	Shell_mouseMovedFunc(Target_mouseMoved);
	Shell_mouseDraggedFunc(Target_mouseDragged);
	Shell_scrollWheelFunc(Target_scrollWheel);
	Shell_backgroundedFunc(Target_backgrounded);
	Shell_foregroundedFunc(Target_foregrounded);
}

static void unregisterShellCallbacks() {
	Shell_drawFunc(NULL);
	Shell_resizeFunc(NULL);
	Shell_keyDownFunc(NULL);
	Shell_keyUpFunc(NULL);
	Shell_keyModifiersChangedFunc(NULL);
	Shell_mouseDownFunc(NULL);
	Shell_mouseUpFunc(NULL);
	Shell_mouseMovedFunc(NULL);
	Shell_mouseDraggedFunc(NULL);
	Shell_scrollWheelFunc(NULL);
	Shell_backgroundedFunc(NULL);
	Shell_foregroundedFunc(NULL);
}

void WGLTarget_configure(void * instance, void * prevInstance, char * commandLine, int command, int argc, const char ** argv, struct WGLShellConfiguration * configuration) {
	char workingDir[PATH_MAX];
	int argIndex;
	
#ifdef STEM_ARCH_x86_64
	WGLShell_redirectStdoutToFile("stdout.txt");
#endif
	
	printf("WGLTarget_configure(%p, %p, \"%s\", %d, %d, %p, %p)\n", instance, prevInstance, commandLine, command, argc, argv, configuration);
	for (argIndex = 0; argIndex < argc; argIndex++) {
		printf("argv[%d]: \"%s\"\n", argIndex, argv[argIndex]);
	}
	
	printf("configuration->windowX: %d\n", configuration->windowX);
	printf("configuration->windowY: %d\n", configuration->windowY);
	printf("configuration->windowWidth: %u\n", configuration->windowWidth);
	printf("configuration->windowHeight: %u\n", configuration->windowHeight);
	printf("configuration->windowTitle: %s\n", configuration->windowTitle);
	printf("configuration->iconResource: %d\n", configuration->iconResource);
	printf("configuration->displayMode.doubleBuffer: %s\n", configuration->displayMode.doubleBuffer ? "true" : "false");
	printf("configuration->displayMode.colorBits: %d\n", configuration->displayMode.colorBits);
	printf("configuration->displayMode.alphaBits: %d\n", configuration->displayMode.alphaBits);
	printf("configuration->displayMode.depthBits: %d\n", configuration->displayMode.depthBits);
	printf("configuration->displayMode.stencilBits: %d\n", configuration->displayMode.stencilBits);
	
	configuration->windowTitle = "WGLShell Test Harness";
	printf("configuration->windowTitle = \"%s\"\n", configuration->windowTitle);
	
	printf("getcwd(): %s\n", getcwd(workingDir, PATH_MAX));
	
	registerShellCallbacks();
}

void Target_init() {
	printf("Target_init()\n");
	printf("GLGraphics_getOpenGLAPIVersion(): %d\n", GLGraphics_getOpenGLAPIVersion());
	Shell_mainLoop();
}
