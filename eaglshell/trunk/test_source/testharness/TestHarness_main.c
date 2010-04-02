#include "shell/Shell.h"
#include "eaglshell/EAGLShell.h"

#include <stdio.h>
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

static bool postRedisplayAtEndOfTarget_draw = false;

const char * Target_getName() {
	return "EAGLShell Test Harness";
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
	static bool darkClearColor = false;
	
	darkClearColor = !darkClearColor;
	if (darkClearColor) {
		glClearColor(0.0f, 0.25f, 0.5f, 0.0f);
	} else {
		glClearColor(0.5f, 0.75f, 1.0f, 0.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (postRedisplayAtEndOfTarget_draw) {
		Shell_redisplay();
	}
}

void Target_keyDown(unsigned int charCode, unsigned int keyCode) {
	printf("Target_keyDown(%d, %d)\n", charCode, keyCode);
}

void Target_keyUp(unsigned int charCode, unsigned int keyCode) {
	printf("Target_keyUp(%d, %d)\n", charCode, keyCode);
	if (keyCode == KEYBOARD_W) {
		printf("Shell_setFullScreen(false): %d\n", Shell_setFullScreen(false));
		
	} else if (keyCode == KEYBOARD_R) {
		printf("Shell_getResourcePath(): %s\n", Shell_getResourcePath());
		
	} else if (keyCode == KEYBOARD_T) {
		printf("Shell_getCurrentTime(): %f\n", Shell_getCurrentTime());
		
	} else if (keyCode == KEYBOARD_A) {
		Shell_redisplay();
		postRedisplayAtEndOfTarget_draw = true;
		
	} else if (keyCode == KEYBOARD_S) {
		postRedisplayAtEndOfTarget_draw = false;
		
	} else if (keyCode == KEYBOARD_D) {
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_F) {
		printf("Shell_setFullScreen(true): %d\n", Shell_setFullScreen(true));
		
	} else if (keyCode == KEYBOARD_G) {
		printf("Shell_isFullScreen(): %d\n", Shell_isFullScreen());
		
	} else if (keyCode == KEYBOARD_B) {
		printf("Shell_getBatteryState(): %d\n", Shell_getBatteryState());
		printf("Shell_getBatteryLevel(): %f\n", Shell_getBatteryLevel());
		
	} else if (keyCode == KEYBOARD_N) {
		EAGLShell_setBatteryMonitoringEnabled(false);
		
	} else if (keyCode == KEYBOARD_M) {
		EAGLShell_setBatteryMonitoringEnabled(true);
		
	} else if (keyCode == KEYBOARD_I) {
		EAGLShell_setOrientation(EAGLShellOrientation_deviceUpright);
		
	} else if (keyCode == KEYBOARD_J) {
		EAGLShell_setOrientation(EAGLShellOrientation_deviceRotatedCounterclockwise);
		
	} else if (keyCode == KEYBOARD_K) {
		EAGLShell_setOrientation(EAGLShellOrientation_deviceUpsideDown);
		
	} else if (keyCode == KEYBOARD_L) {
		EAGLShell_setOrientation(EAGLShellOrientation_deviceRotatedClockwise);
		
	} else if (keyCode == KEYBOARD_RETURN_OR_ENTER) {
		EAGLShell_hideKeyboard();
	}
}

void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	printf("Target_mouseDown(%d, %f, %f)\n", buttonNumber, x, y);
}

void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
	printf("Target_mouseUp(%d, %f, %f)\n", buttonNumber, x, y);
	EAGLShell_showKeyboard();
}

void Target_mouseMoved(float x, float y) {
	printf("Target_mouseMoved(%f, %f)\n", x, y);
}

void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	printf("Target_mouseDragged(0x%X, %f, %f)\n", buttonMask, x, y);
}

void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	printf("Target_resized(%d, %d)\n", newWidth, newHeight);
}
