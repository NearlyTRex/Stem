#include "gamemath/BezierCurve.h"
#include "gamemath/Matrix4x4f.h"
#include "gamemath/MouseCoordinateTransforms.h"
#include "shell/Shell.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"

#if defined(STEM_PLATFORM_macosx)
#include "nsopenglshell/NSOpenGLShell.h"
#include "nsopenglshell/NSOpenGLTarget.h"
#elif defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
#include "eaglshell/EAGLShell.h"
#include "eaglshell/EAGLTarget.h"
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
#include "wglshell/WGLShell.h"
#include "wglshell/WGLTarget.h"
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
#include "glxshell/GLXShell.h"
#include "glxshell/GLXTarget.h"
#else
#include "glutshell/GLUTTarget.h"
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <stdio.h>
#include <unistd.h>

#define SAMPLE_COUNT_MAX 100
// TODO: Make this adjustable
#define SAMPLE_ITERATIONS 8

#define SELECTION_RADIUS 0.025f
#define PROJECTION_RADIUS 0.0375f

static unsigned int viewWidth = 1280, viewHeight = 720;
static float viewRatio = 1.77778f;
static Vector2f points[5];
static Vector2f samples[SAMPLE_COUNT_MAX];
static Vector2f dragOrigin;
static unsigned int selectedPoint;
static bool drawPoints;
static unsigned int sampleCount = 16;

static bool Target_draw() {
	unsigned int sampleIndex;
	float projectedX, projectedY;
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor4f(0.0f, 0.0f, 0.5f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(-viewRatio, points[4].y);
	glVertex2f(viewRatio, points[4].y);
	glVertex2f(points[4].x, -1.0f);
	glVertex2f(points[4].x, 1.0f);
	glEnd();
	
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(points[0].x, points[0].y);
	glVertex2f(points[1].x, points[1].y);
	glVertex2f(points[2].x, points[2].y);
	glVertex2f(points[3].x, points[3].y);
	glEnd();
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++) {
		glVertex2f(samples[sampleIndex].x, samples[sampleIndex].y);
	}
	glEnd();
	if (drawPoints) {
		glPointSize(5.0f);
		glColor4f(0.75f, 1.0f, 0.75f, 1.0f);
		glBegin(GL_POINTS);
		for (sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++) {
			glVertex2f(samples[sampleIndex].x, samples[sampleIndex].y);
		}
		glEnd();
	}
	
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(points[selectedPoint].x - SELECTION_RADIUS, points[selectedPoint].y);
	glVertex2f(points[selectedPoint].x, points[selectedPoint].y - SELECTION_RADIUS);
	glVertex2f(points[selectedPoint].x + SELECTION_RADIUS, points[selectedPoint].y);
	glVertex2f(points[selectedPoint].x, points[selectedPoint].y + SELECTION_RADIUS);
	glEnd();
	
	projectedX = BezierCurve_sampleXAtY(points[0], points[1], points[2], points[3], points[4].y, SAMPLE_ITERATIONS);
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(projectedX - PROJECTION_RADIUS, points[4].y);
	glVertex2f(projectedX, points[4].y - PROJECTION_RADIUS);
	glVertex2f(projectedX + PROJECTION_RADIUS, points[4].y);
	glVertex2f(projectedX, points[4].y + PROJECTION_RADIUS);
	glEnd();
	
	projectedY = BezierCurve_sampleYAtX(points[0], points[1], points[2], points[3], points[4].x, SAMPLE_ITERATIONS);
	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(points[4].x - PROJECTION_RADIUS, projectedY);
	glVertex2f(points[4].x, projectedY - PROJECTION_RADIUS);
	glVertex2f(points[4].x + PROJECTION_RADIUS, projectedY);
	glVertex2f(points[4].x, projectedY + PROJECTION_RADIUS);
	glEnd();
	
	return true;
}

static void fetchSamples() {
	BezierCurve_getSamples(points[0], points[1], points[2], points[3], samples, sampleCount);
}

static void resetBezier() {
	points[0] = VECTOR2f(-0.75f, -0.75f);
	points[1] = VECTOR2f(0.0f, -0.75f);
	points[2] = VECTOR2f(0.0f, 0.75f);
	points[3] = VECTOR2f(0.75f, 0.75f);
	fetchSamples();
}

static void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat) {
	switch (keyCode) {
		case KEYBOARD_R:
			resetBezier();
			Shell_redisplay();
			break;
		case KEYBOARD_TAB:
			if (modifiers & MODIFIER_SHIFT_BIT) {
				selectedPoint = (selectedPoint + sizeof(points) / sizeof(points[0]) - 1) % (sizeof(points) / sizeof(points[0]));
			} else {
				selectedPoint = (selectedPoint + 1) % (sizeof(points) / sizeof(points[0]));
			}
			Shell_redisplay();
			break;
		case KEYBOARD_P:
			drawPoints = !drawPoints;
			Shell_redisplay();
			break;
		case KEYBOARD_EQUAL_SIGN:
			sampleCount++;
			if (sampleCount > SAMPLE_COUNT_MAX) {
				sampleCount = SAMPLE_COUNT_MAX;
			}
			fetchSamples();
			Shell_redisplay();
			break;
		case KEYBOARD_HYPHEN:
			sampleCount--;
			if (sampleCount < 2) {
				sampleCount = 2;
			}
			fetchSamples();
			Shell_redisplay();
			break;
	}
}

static void Target_keyUp(unsigned int keyCode, unsigned int modifiers) {
}

static void Target_keyModifiersChanged(unsigned int modifiers) {
}

static void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	dragOrigin = transformMousePosition_signedCenter(VECTOR2f(x, y), viewWidth, viewHeight, 1.0f);
}

static void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
}

static void Target_mouseMoved(float x, float y) {
}

static void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	Vector2f position = transformMousePosition_signedCenter(VECTOR2f(x, y), viewWidth, viewHeight, 1.0f);
	points[selectedPoint].x += position.x - dragOrigin.x;
	points[selectedPoint].y += position.y - dragOrigin.y;
	fetchSamples();
	dragOrigin = position;
	Shell_redisplay();
}

static void Target_scrollWheel(int deltaX, int deltaY) {
}

static void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	viewWidth = newWidth;
	viewHeight = newHeight;
	viewRatio = (float) viewWidth / viewHeight;
	glViewport(0, 0, viewWidth, viewHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(Matrix4x4f_ortho(MATRIX4x4f_IDENTITY, -viewRatio, viewRatio, -1.0f, 1.0f, -1.0f, 1.0f).m);
	glMatrixMode(GL_MODELVIEW);
}

static void Target_backgrounded() {
}

static void Target_foregrounded() {
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

#if defined(STEM_PLATFORM_macosx)
void NSOpenGLTarget_configure(int argc, const char ** argv, struct NSOpenGLShellConfiguration * configuration) {
	configuration->windowTitle = "GameMath";
#elif defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
void WGLTarget_configure(void * instance, void * prevInstance, char * commandLine, int command, int argc, const char ** argv, struct WGLShellConfiguration * configuration) {
	configuration->windowTitle = "GameMath";
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
void GLXTarget_configure(int argc, const char ** argv, struct GLXShellConfiguration * configuration) {
	configuration->windowTitle = "GameMath";
#else
void GLUTTarget_configure(int argc, const char ** argv, struct GLUTShellConfiguration * configuration) {
	configuration->windowTitle = "GameMath";
#endif
	registerShellCallbacks();
}

void Target_init() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	resetBezier();
	Shell_mainLoop();
}
