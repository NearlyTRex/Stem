#include "shell/Shell.h"
#include "glutshell/GLUTTarget.h"

#include <stdio.h>
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void GLUTTarget_configure(int argc, char ** argv, struct GLUTShellConfiguration * configuration) {
	int argIndex;
	
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
	printf("configuration->displayMode.doubleBuffer: %d\n", configuration->displayMode.doubleBuffer);
	printf("configuration->displayMode.depthBuffer: %d\n", configuration->displayMode.depthBuffer);
	printf("configuration->displayMode.stencilBuffer: %d\n", configuration->displayMode.stencilBuffer);
	printf("configuration->displayMode.accumBuffer: %d\n", configuration->displayMode.accumBuffer);
	printf("configuration->displayMode.multisample: %d\n", configuration->displayMode.multisample);
	
	configuration->windowTitle = "GLUTShell Test Harness";
	printf("configuration->windowTitle = \"%s\"\n", configuration->windowTitle);
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

void Target_keyDown(unsigned int charCode, unsigned int keyCode) {
	printf("Target_keyDown(%d, %d)\n", charCode, keyCode);
	if (keyCode == KEYBOARD_T) {
		printf("Shell_getCurrentTime(): %f\n", Shell_getCurrentTime());
		
	} else if (keyCode == KEYBOARD_R) {
		printf("Shell_getResourcePath(): %s\n", Shell_getResourcePath());
		
	} else if (keyCode == KEYBOARD_D) {
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_F) {
		printf("Shell_setFullScreen(true): %d\n", Shell_setFullScreen(true));
		
	} else if (keyCode == KEYBOARD_W) {
		printf("Shell_setFullScreen(false): %d\n", Shell_setFullScreen(false));
		
	} else if (keyCode == KEYBOARD_G) {
		printf("Shell_isFullScreen(): %d\n", Shell_isFullScreen());
		
	} else if (keyCode == KEYBOARD_B) {
		printf("Shell_getBatteryState(): %d\n", Shell_getBatteryState());
		printf("Shell_getBatteryLevel(): %f\n", Shell_getBatteryLevel());
	}
}

void Target_keyUp(unsigned int charCode, unsigned int keyCode) {
	printf("Target_keyUp(%d, %d)\n", charCode, keyCode);
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
