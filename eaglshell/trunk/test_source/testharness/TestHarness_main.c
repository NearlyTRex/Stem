#include "shell/Shell.h"
#include "shell/Target.h"
#include "eaglshell/EAGLShell.h"
#include "eaglshell/EAGLTarget.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellKeyCodes.h"
#include "shell/ShellThreads.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

static bool postRedisplayAtEndOfTarget_draw = false;
static bool darkClearColor = true;
static unsigned int viewportWidth, viewportHeight;
static struct {
	bool active;
	unsigned int x;
	unsigned int y;
} touches[32];
static struct {
	double x;
	double y;
	double z;
} lastAccelerometerReading;
static GLuint shaderProgram;
static GLint projectionMatrixUniform;
static GLint modelviewMatrixUniform;
static GLint constantColorUniform;
static unsigned int timer1ID = UINT_MAX, timer2ID = UINT_MAX;
static bool deltaMode;

#define msleep(ms) usleep((ms) * 1000)

#define VERTEX_ATTRIB_INDEX 0

void Target_init() {
	printf("Target_init()\n");
	
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

bool Target_draw() {
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
		
	} else if (keyCode == KEYBOARD_W) {
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
		
	} else if (keyCode == KEYBOARD_V) {
		EAGLShell_setAccelerometerInterval(1.0 / 60.0);
		
	} else if (keyCode == KEYBOARD_C) {
		EAGLShell_setAccelerometerInterval(1.0 / 10.0);
		
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

void Target_keyUp(unsigned int keyCode, unsigned int modifierFlags) {
	printf("Target_keyUp(%u, 0x%X)\n", keyCode, modifierFlags);
}

void Target_keyModifiersChanged(unsigned int modifierFlags) {
	printf("Target_keyModifiersChanged(0x%X) (What? This should not have been called!)\n", modifierFlags);
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

static int preferredOpenGLAPIVersion = EAGLShellOpenGLVersion_ES1 | EAGLShellOpenGLVersion_ES2;
static int retainedBacking = 0;
static int depthAttachment = 0;
static int stencilAttachment = 0;
static int colorPrecision = 32;
static int depthPrecision = 16;
static int packedDepthAndStencil = 0;

void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
	int argIndex;
	
	EAGLShell_redirectStdoutToFile();
	
	printf("EAGLTarget_configure(%d", argc);
	for (argIndex = 0; argIndex < argc; argIndex++) {
		printf(", \"%s\"", argv[argIndex]);
	}
	printf(", %p)\n", configuration);
	
	printf("configuration->preferredOpenGLAPIVersion: 0x%X\n", configuration->preferredOpenGLAPIVersion);
	printf("configuration->displayMode.retainedBacking: %d\n", configuration->displayMode.retainedBacking);
	printf("configuration->displayMode.depthAttachment: %d\n", configuration->displayMode.depthAttachment);
	printf("configuration->displayMode.stencilAttachment: %d\n", configuration->displayMode.stencilAttachment);
	printf("configuration->displayMode.colorPrecision: %d\n", configuration->displayMode.colorPrecision);
	printf("configuration->displayMode.depthPrecision: %d\n", configuration->displayMode.depthPrecision);
	printf("configuration->displayMode.packedDepthAndStencil: %d\n", configuration->displayMode.packedDepthAndStencil);
	
	configuration->preferredOpenGLAPIVersion = preferredOpenGLAPIVersion;
	printf("configuration->preferredOpenGLAPIVersion = 0x%X\n", configuration->preferredOpenGLAPIVersion);
	configuration->displayMode.retainedBacking = retainedBacking;
	printf("configuration->displayMode.retainedBacking = %d\n", configuration->displayMode.retainedBacking);
	configuration->displayMode.depthAttachment = depthAttachment;
	printf("configuration->displayMode.depthAttachment = %d\n", configuration->displayMode.depthAttachment);
	configuration->displayMode.stencilAttachment = stencilAttachment;
	printf("configuration->displayMode.stencilAttachment = %d\n", configuration->displayMode.stencilAttachment);
	configuration->displayMode.colorPrecision = colorPrecision;
	printf("configuration->displayMode.colorPrecision = %d\n", configuration->displayMode.colorPrecision);
	configuration->displayMode.depthPrecision = depthPrecision;
	printf("configuration->displayMode.depthPrecision = %d\n", configuration->displayMode.depthPrecision);
	configuration->displayMode.packedDepthAndStencil = packedDepthAndStencil;
	printf("configuration->displayMode.packedDepthAndStencil = %d\n", configuration->displayMode.packedDepthAndStencil);
}

void EAGLTarget_openURL(const char * url) {
	printf("Got URL: %s\n", url);
	sscanf(url, "eaglshell://%d,%d,%d,%d,%d,%d,%d", &preferredOpenGLAPIVersion, &retainedBacking, &depthAttachment, &stencilAttachment, &colorPrecision, &depthPrecision, &packedDepthAndStencil);
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
