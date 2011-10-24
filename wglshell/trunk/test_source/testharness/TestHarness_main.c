#include "shell/Shell.h"
#include "wglshell/WGLTarget.h"

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"

#include <GL/gl.h>

void WGLTarget_configure(void * instance, void * prevInstance, char * commandLine, int command, struct WGLShellConfiguration * configuration) {
	char workingDir[PATH_MAX];
	
	printf("WGLTarget_configure(%p, %p, \"%s\", %d, %p)", instance, prevInstance, commandLine, command, configuration);
	
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
}

void Target_init() {
	printf("Target_init()\n");
	Shell_mainLoop();
}

void Target_draw() {
	printf("Target_draw()\n");
	glClearColor(0.0f, 0.25f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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
		
	} else if (keyCode == KEYBOARD_H) {
		WGLShell_setCursorVisible(false);
		
	} else if (keyCode == KEYBOARD_S) {
		WGLShell_setCursorVisible(true);
		
	} else if (keyCode == KEYBOARD_M) {
		WGLShell_hideCursorUntilMouseMoves();
		
	} else if (keyCode == KEYBOARD_0 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_arrow);
		
	} else if (keyCode == KEYBOARD_1 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_appStarting);
		
	} else if (keyCode == KEYBOARD_2 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_crosshair);
		
	} else if (keyCode == KEYBOARD_3 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_hand);
		
	} else if (keyCode == KEYBOARD_4 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_help);
		
	} else if (keyCode == KEYBOARD_5 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_iBeam);
		
	} else if (keyCode == KEYBOARD_6 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_no);
		
	} else if (keyCode == KEYBOARD_7 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_sizeAll);
		
	} else if (keyCode == KEYBOARD_8 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_sizeNESW);
		
	} else if (keyCode == KEYBOARD_9 && !(modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_sizeNS);
		
	} else if (keyCode == KEYBOARD_1 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_sizeNWSE);
		
	} else if (keyCode == KEYBOARD_2 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_sizeWE);
		
	} else if (keyCode == KEYBOARD_3 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_upArrow);
		
	} else if (keyCode == KEYBOARD_4 && (modifierFlags & MODIFIER_SHIFT_BIT)) {
		WGLShell_setCursor(WGLShellCursor_wait);
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
