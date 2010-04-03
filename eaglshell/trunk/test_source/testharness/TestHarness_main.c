#include "shell/Shell.h"
#include "shell/Target.h"
#include "eaglshell/EAGLShell.h"
#include "eaglshell/EAGLTarget.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

static bool postRedisplayAtEndOfTarget_draw = false;
static bool darkClearColor = true;
static unsigned int viewportWidth, viewportHeight;
static struct {
	bool active;
	unsigned int x;
	unsigned int y;
} touches[32];
struct {
	double x;
	double y;
	double z;
} lastAccelerometerReading;

GLuint shaderProgram;
GLint projectionMatrixUniform;
GLint modelviewMatrixUniform;
GLint constantColorUniform;

#define VERTEX_ATTRIB_INDEX 0

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
	
	printf("EAGLShell_getOpenGLAPIVersion(): %d\n", EAGLShell_getOpenGLAPIVersion());
	
	if (EAGLShell_getOpenGLAPIVersion() == EAGLShellOpenGLVersion_ES2) {
		FILE * file;
		char filePath[PATH_MAX];
		GLchar * fileContents;
		GLint fileLength;
		GLuint vertexShader, fragmentShader;
		
		shaderProgram = glCreateProgram();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		sprintf(filePath, "%s/%s", Shell_getResourcePath(), "Vertex.vert");
		file = fopen(filePath, "rb");
		fseek(file, 0, SEEK_END);
		fileLength = ftell(file);
		fseek(file, 0, SEEK_SET);
		fileContents = malloc(fileLength);
		fread(fileContents, 1, fileLength, file);
		fclose(file);
		glShaderSource(vertexShader, 1, (const GLchar **) &fileContents, &fileLength);
		free(fileContents);
		
		sprintf(filePath, "%s/%s", Shell_getResourcePath(), "Fragment.frag");
		file = fopen(filePath, "rb");
		fseek(file, 0, SEEK_END);
		fileLength = ftell(file);
		fseek(file, 0, SEEK_SET);
		fileContents = malloc(fileLength);
		fread(fileContents, 1, fileLength, file);
		fclose(file);
		glShaderSource(fragmentShader, 1, (const GLchar **) &fileContents, &fileLength);
		free(fileContents);
		
		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindAttribLocation(shaderProgram, VERTEX_ATTRIB_INDEX, "vertexPosition");
		glLinkProgram(shaderProgram);
		projectionMatrixUniform = glGetUniformLocation(shaderProgram, "projectionMatrix");
		modelviewMatrixUniform = glGetUniformLocation(shaderProgram, "modelviewMatrix");
		constantColorUniform = glGetUniformLocation(shaderProgram, "constantColor");
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		
		glEnableVertexAttribArray(VERTEX_ATTRIB_INDEX);
		
	} else {
		glEnableClientState(GL_VERTEX_ARRAY);
	}
	
	Shell_mainLoop();
}

void Target_draw() {
	GLshort vertices[12];
	unsigned int touchIndex;
	
	if (darkClearColor) {
		glClearColor(0.0f, 0.25f, 0.5f, 0.0f);
	} else {
		glClearColor(0.25f, 0.5f, 0.75f, 0.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (EAGLShell_getOpenGLAPIVersion() == EAGLShellOpenGLVersion_ES1) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthof(0, viewportWidth, viewportHeight, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		
		if (lastAccelerometerReading.x != 0.0) {
			vertices[0] = viewportWidth / 3 - 5; vertices[1] = viewportHeight / 2;
			vertices[2] = viewportWidth / 3 + 5; vertices[3] = viewportHeight / 2;
			vertices[4] = viewportWidth / 3 + 5; vertices[5] = viewportHeight / 2 + lastAccelerometerReading.x * viewportHeight / 6;
			vertices[6] = viewportWidth / 3 - 5; vertices[7] = viewportHeight / 2 + lastAccelerometerReading.x * viewportHeight / 6;
			glVertexPointer(2, GL_SHORT, 0, vertices);
			glColor4ub(0xFF, 0x00, 0x00, 0xFF);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		if (lastAccelerometerReading.y != 0.0) {
			vertices[0] = viewportWidth / 2 - 5; vertices[1] = viewportHeight / 2;
			vertices[2] = viewportWidth / 2 + 5; vertices[3] = viewportHeight / 2;
			vertices[4] = viewportWidth / 2 + 5; vertices[5] = viewportHeight / 2 + lastAccelerometerReading.y * viewportHeight / 6;
			vertices[6] = viewportWidth / 2 - 5; vertices[7] = viewportHeight / 2 + lastAccelerometerReading.y * viewportHeight / 6;
			glVertexPointer(2, GL_SHORT, 0, vertices);
			glColor4ub(0x00, 0xFF, 0x00, 0xFF);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		if (lastAccelerometerReading.z != 0.0) {
			vertices[0] = viewportWidth - viewportWidth / 3 - 5; vertices[1] = viewportHeight / 2;
			vertices[2] = viewportWidth - viewportWidth / 3 + 5; vertices[3] = viewportHeight / 2;
			vertices[4] = viewportWidth - viewportWidth / 3 + 5; vertices[5] = viewportHeight / 2 + lastAccelerometerReading.z * viewportHeight / 6;
			vertices[6] = viewportWidth - viewportWidth / 3 - 5; vertices[7] = viewportHeight / 2 + lastAccelerometerReading.z * viewportHeight / 6;
			glVertexPointer(2, GL_SHORT, 0, vertices);
			glColor4ub(0x00, 0x00, 0xFF, 0xFF);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		
		vertices[0] = 0; vertices[1] = 0;
		vertices[2] = 16; vertices[3] = 0;
		vertices[4] = 0; vertices[5] = 16;
		
		vertices[6] = viewportWidth; vertices[7] = viewportHeight;
		vertices[8] = viewportWidth - 16; vertices[9] = viewportHeight;
		vertices[10] = viewportWidth; vertices[11] = viewportHeight - 16;
		
		glVertexPointer(2, GL_SHORT, 0, vertices);
		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		for (touchIndex = 0; touchIndex < 32; touchIndex++) {
			if (touches[touchIndex].active) {
				vertices[0] = touches[touchIndex].x - 24; vertices[1] = touches[touchIndex].y - 24;
				vertices[2] = touches[touchIndex].x - 24; vertices[3] = touches[touchIndex].y + 24;
				vertices[4] = touches[touchIndex].x + 24; vertices[5] = touches[touchIndex].y + 24;
				vertices[6] = touches[touchIndex].x + 24; vertices[7] = touches[touchIndex].y - 24;
				glVertexPointer(2, GL_SHORT, 0, vertices);
				glColor4ub(0x7F, 0xFF, 0x7F, 0xFF);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
		}
		
	} else {
		GLfloat projectionMatrix[16] = {
			 2.0f / viewportWidth,  0.0f,                   0.0f, 0.0f,
			 0.0f,                 -2.0f / viewportHeight,  0.0f, 0.0f,
			 0.0f,                  0.0f,                  -1.0f, 0.0f,
			-1.0f,                  1.0f,                   0.0f, 1.0f
		};
		GLfloat modelviewMatrix[16] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, projectionMatrix);
		glUniformMatrix4fv(modelviewMatrixUniform, 1, GL_FALSE, modelviewMatrix);
		
		if (lastAccelerometerReading.x != 0.0) {
			vertices[0] = viewportWidth / 3 - 5; vertices[1] = viewportHeight / 2;
			vertices[2] = viewportWidth / 3 + 5; vertices[3] = viewportHeight / 2;
			vertices[4] = viewportWidth / 3 + 5; vertices[5] = viewportHeight / 2 + lastAccelerometerReading.x * viewportHeight / 6;
			vertices[6] = viewportWidth / 3 - 5; vertices[7] = viewportHeight / 2 + lastAccelerometerReading.x * viewportHeight / 6;
			glVertexAttribPointer(VERTEX_ATTRIB_INDEX, 2, GL_SHORT, GL_FALSE, 0, vertices);
			glUniform4f(constantColorUniform, 1.0f, 0.0f, 0.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		if (lastAccelerometerReading.y != 0.0) {
			vertices[0] = viewportWidth / 2 - 5; vertices[1] = viewportHeight / 2;
			vertices[2] = viewportWidth / 2 + 5; vertices[3] = viewportHeight / 2;
			vertices[4] = viewportWidth / 2 + 5; vertices[5] = viewportHeight / 2 + lastAccelerometerReading.y * viewportHeight / 6;
			vertices[6] = viewportWidth / 2 - 5; vertices[7] = viewportHeight / 2 + lastAccelerometerReading.y * viewportHeight / 6;
			glVertexAttribPointer(VERTEX_ATTRIB_INDEX, 2, GL_SHORT, GL_FALSE, 0, vertices);
			glUniform4f(constantColorUniform, 0.0f, 1.0f, 0.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		if (lastAccelerometerReading.z != 0.0) {
			vertices[0] = viewportWidth - viewportWidth / 3 - 5; vertices[1] = viewportHeight / 2;
			vertices[2] = viewportWidth - viewportWidth / 3 + 5; vertices[3] = viewportHeight / 2;
			vertices[4] = viewportWidth - viewportWidth / 3 + 5; vertices[5] = viewportHeight / 2 + lastAccelerometerReading.z * viewportHeight / 6;
			vertices[6] = viewportWidth - viewportWidth / 3 - 5; vertices[7] = viewportHeight / 2 + lastAccelerometerReading.z * viewportHeight / 6;
			glVertexAttribPointer(VERTEX_ATTRIB_INDEX, 2, GL_SHORT, GL_FALSE, 0, vertices);
			glUniform4f(constantColorUniform, 0.0f, 0.0f, 1.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		}
		
		vertices[0] = 0; vertices[1] = 0;
		vertices[2] = 16; vertices[3] = 0;
		vertices[4] = 0; vertices[5] = 16;
		
		vertices[6] = viewportWidth; vertices[7] = viewportHeight;
		vertices[8] = viewportWidth - 16; vertices[9] = viewportHeight;
		vertices[10] = viewportWidth; vertices[11] = viewportHeight - 16;
		
		glVertexAttribPointer(VERTEX_ATTRIB_INDEX, 2, GL_SHORT, GL_FALSE, 0, vertices);
		glUniform4f(constantColorUniform, 1.0f, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		for (touchIndex = 0; touchIndex < 32; touchIndex++) {
			if (touches[touchIndex].active) {
				vertices[0] = touches[touchIndex].x - 24; vertices[1] = touches[touchIndex].y - 24;
				vertices[2] = touches[touchIndex].x - 24; vertices[3] = touches[touchIndex].y + 24;
				vertices[4] = touches[touchIndex].x + 24; vertices[5] = touches[touchIndex].y + 24;
				vertices[6] = touches[touchIndex].x + 24; vertices[7] = touches[touchIndex].y - 24;
				glVertexAttribPointer(VERTEX_ATTRIB_INDEX, 2, GL_SHORT, GL_FALSE, 0, vertices);
				glUniform4f(constantColorUniform, 0.5f, 1.0f, 0.5f, 1.0f);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
		}
	}
	
	if (postRedisplayAtEndOfTarget_draw) {
		darkClearColor = !darkClearColor;
		Shell_redisplay();
	}
}

void Target_keyDown(unsigned int charCode, unsigned int keyCode) {
	printf("Target_keyDown(%d, %d) (What? This should not have been called!)\n", charCode, keyCode);
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
		darkClearColor = !darkClearColor;
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_F) {
		printf("Shell_setFullScreen(true): %d\n", Shell_setFullScreen(true));
		
	} else if (keyCode == KEYBOARD_G) {
		printf("Shell_isFullScreen(): %d\n", Shell_isFullScreen());
		
	} else if (keyCode == KEYBOARD_Z) {
		EAGLShell_setAccelerometerInterval(0);
		lastAccelerometerReading.x = lastAccelerometerReading.y = lastAccelerometerReading.z = 0.0;
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_X) {
		EAGLShell_setAccelerometerInterval(1.0 / 60.0);
		
	} else if (keyCode == KEYBOARD_C) {
		EAGLShell_setAccelerometerInterval(1.0 / 10.0);
		
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
	touches[buttonNumber].active = true;
	touches[buttonNumber].x = x;
	touches[buttonNumber].y = y;
	Shell_redisplay();
}

void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
	printf("Target_mouseUp(%d, %f, %f)\n", buttonNumber, x, y);
	EAGLShell_showKeyboard();
	touches[buttonNumber].active = false;
	touches[buttonNumber].x = x;
	touches[buttonNumber].y = y;
	Shell_redisplay();
}

void Target_mouseMoved(float x, float y) {
	printf("Target_mouseMoved(%f, %f) (What? This should not have been called!)\n", x, y);
}

static unsigned int lowestBitIndex(unsigned int value) {
	// See http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightModLookup
	
	static const int mod37BitPosition[] = {
	  32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4,
	  7, 17, 0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5,
	  20, 8, 19, 18
	};
	return mod37BitPosition[(-value & value) % 37];
}

void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	printf("Target_mouseDragged(0x%X, %f, %f)\n", buttonMask, x, y);
	touches[lowestBitIndex(buttonMask)].x = x;
	touches[lowestBitIndex(buttonMask)].y = y;
	Shell_redisplay();
}

void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	printf("Target_resized(%d, %d)\n", newWidth, newHeight);
	viewportWidth = newWidth;
	viewportHeight = newHeight;
}

enum EAGLShellOpenGLVersion EAGLTarget_getPreferredOpenGLAPIVersion() {
	printf("EAGLTarget_getPreferredOpenGLAPIVersion()\n");
	return EAGLShellOpenGLVersion_ES1 | EAGLShellOpenGLVersion_ES2;
}

void EAGLTarget_touchesCancelled(unsigned int buttonMask) {
	unsigned int buttonNumber;
	
	printf("EAGLTarget_touchesCancelled(0x%X)\n", buttonMask);
	for (buttonNumber = 0; buttonNumber < 32; buttonNumber++) {
		if (buttonMask & (1 << buttonNumber)) {
			touches[buttonNumber].active = false;
		}
	}
}

void EAGLTarget_accelerometer(double x, double y, double z) {
	lastAccelerometerReading.x = x;
	lastAccelerometerReading.y = y;
	lastAccelerometerReading.z = z;
	Shell_redisplay();
}
