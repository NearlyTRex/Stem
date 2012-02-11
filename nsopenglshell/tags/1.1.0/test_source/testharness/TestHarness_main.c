#include "shell/Shell.h"
#include "nsopenglshell/NSOpenGLTarget.h"

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include "glgraphics/GLGraphics.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"
#include "shell/Target.h"

#include <OpenGL/gl.h>

static unsigned int timer1ID = UINT_MAX, timer2ID = UINT_MAX;

void NSOpenGLTarget_configure(int argc, const char ** argv, struct NSOpenGLShellConfiguration * configuration) {
	int argIndex;
	char workingDir[PATH_MAX];
	
	printf("NSOpenGLTarget_configure(%d", argc);
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
	printf("configuration->displayMode.depthBits: %u\n", configuration->displayMode.depthBits);
	printf("configuration->displayMode.stencilBuffer: %s\n", configuration->displayMode.stencilBuffer ? "true" : "false");
	printf("configuration->displayMode.stencilBits: %u\n", configuration->displayMode.stencilBits);
	printf("configuration->displayMode.accumBuffer: %s\n", configuration->displayMode.accumBuffer ? "true" : "false");
	printf("configuration->displayMode.accumBits: %u\n", configuration->displayMode.accumBits);
	printf("configuration->displayMode.multisample: %s\n", configuration->displayMode.multisample ? "true" : "false");
	printf("configuration->displayMode.sampleBuffers: %u\n", configuration->displayMode.sampleBuffers);
	printf("configuration->displayMode.samples: %u\n", configuration->displayMode.samples);
	
	configuration->windowTitle = "NSOpenGLShell Test Harness";
	printf("configuration->windowTitle = \"%s\"\n", configuration->windowTitle);
	
	getcwd(workingDir, PATH_MAX);
	printf("getcwd(): %s\n", workingDir);
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

void timerCallback(unsigned int timerID, void * context) {
	printf("timerCallback(timerID, \"%s\")\n", (char *) context);
	if (timerID == timer2ID) {
		timer2ID = UINT_MAX;
	}
}

void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags) {
	printf("Target_keyDown(%u, %u, 0x%X)\n", charCode, keyCode, modifierFlags);
	if (keyCode == KEYBOARD_T) {
		printf("Shell_getCurrentTime(): %f\n", Shell_getCurrentTime());
		
	} else if (keyCode == KEYBOARD_R) {
		printf("Shell_getResourcePath(): %s\n", Shell_getResourcePath());
		
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
		
	} else if (keyCode == KEYBOARD_H) {
		NSOpenGLShell_setCursorVisible(false);
		
	} else if (keyCode == KEYBOARD_S) {
		NSOpenGLShell_setCursorVisible(true);
		
	} else if (keyCode == KEYBOARD_M) {
		NSOpenGLShell_hideCursorUntilMouseMoves();
		
	} else if (keyCode == KEYBOARD_0 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_arrow);
		
	} else if (keyCode == KEYBOARD_1 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_IBeam);
		
	} else if (keyCode == KEYBOARD_2 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_crosshair);
		
	} else if (keyCode == KEYBOARD_3 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_closedHand);
		
	} else if (keyCode == KEYBOARD_4 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_openHand);
		
	} else if (keyCode == KEYBOARD_5 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_pointingHand);
		
	} else if (keyCode == KEYBOARD_6 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_resizeLeft);
		
	} else if (keyCode == KEYBOARD_7 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_resizeRight);
		
	} else if (keyCode == KEYBOARD_8 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_resizeLeftRight);
		
	} else if (keyCode == KEYBOARD_9 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_resizeUp);
		
	} else if (keyCode == KEYBOARD_1 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_resizeDown);
		
	} else if (keyCode == KEYBOARD_2 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_resizeUpDown);
		
	} else if (keyCode == KEYBOARD_3 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_disappearingItem);
		
	} else if (keyCode == KEYBOARD_4 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_contextualMenu);
		
	} else if (keyCode == KEYBOARD_5 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_dragCopy);
		
	} else if (keyCode == KEYBOARD_6 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_dragLink);
		
	} else if (keyCode == KEYBOARD_7 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		NSOpenGLShell_setCursor(NSOpenGLShellCursor_operationNotAllowed);
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
