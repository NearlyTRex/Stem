#include "shell/Shell.h"

#include <stdio.h>
#include "shell/ShellKeyCodes.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

const char * Target_getName() {
	return "GLUTShell Test Harness";
}

void Target_init(int argc, char ** argv) {
	int argIndex;
	
	printf("Target_init(%d", argc);
	for (argIndex = 0; argIndex < argc; argIndex++) {
		printf(", \"%s\"", argv[argIndex]);
	}
	printf(")\n");
	
	Shell_mainLoop();
}

void Target_draw() {
	printf("Target_draw()\n");
	glClearColor(0.0f, 0.25f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Target_keyDown(int charCode, int keyCode) {
	printf("Target_keyDown(%d, %d)\n", charCode, keyCode);
	if (keyCode == KEYBOARD_T) {
		printf("Shell_getCurrentTime(): %f\n", Shell_getCurrentTime());
	} else if (keyCode == KEYBOARD_R) {
		printf("Shell_getResourcePath(): %s\n", Shell_getResourcePath());
	} else if (keyCode == KEYBOARD_D) {
		Shell_redisplay();
	}
}

void Target_keyUp(int charCode, int keyCode) {
	printf("Target_keyUp(%d, %d)\n", charCode, keyCode);
}

void Target_mouseDown(int buttonNumber, float x, float y) {
	printf("Target_mouseDown(%d, %f, %f)\n", buttonNumber, x, y);
}

void Target_mouseUp(int buttonNumber, float x, float y) {
	printf("Target_mouseUp(%d, %f, %f)\n", buttonNumber, x, y);
}

void Target_mouseMoved(float x, float y) {
	printf("Target_mouseMoved(%f, %f)\n", x, y);
}

void Target_mouseDragged(int buttonMask, float x, float y) {
	printf("Target_mouseDragged(0x%X, %f, %f)\n", buttonMask, x, y);
}

void Target_resized(int newWidth, int newHeight) {
	printf("Target_resized(%d, %d)\n", newWidth, newHeight);
}
