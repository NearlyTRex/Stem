/*
  Copyright (c) 2015 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener alex@ludobloom.com
*/

#include "testharness/BouncingBallScreen.h"
#include "testharness/ResourceWiring.h"
#include "testharness/SingleFrameScreen.h"
#include "testharness/SharedEvents.h"
#include "testharness/TestHarness_globals.h"

#include "gamemath/Vector2i.h"
#include "glgraphics/GLIncludes.h"
#include "resourcemanager/ResourceManager.h"
#include "screenmanager/ScreenManager.h"
#include "shell/Shell.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"

#if defined(STEM_PLATFORM_macosx)
#include "nsopenglshell/NSOpenGLShell.h"
#include "nsopenglshell/NSOpenGLTarget.h"
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
#include "wglshell/WGLShell.h"
#include "wglshell/WGLTarget.h"
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
#include "glxshell/GLXShell.h"
#include "glxshell/GLXTarget.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// TODO:
// - Show contact area (and vectors?) in scene 1
// - Animate mode (interpolate all shapes to first collision, resolve, continue until all resolved)
// - Hotkey to resolve detected collision in scene 1 (move lastPosition to point of intersection)
// - Display numbers for all relevant properties of selected object

// Feature wishlist:
// - Demo scenes: Prebuilt setups where collidable objects can be controlled
//  - Platformer controls with gravity
//  - Overhead controls
//  - Bouncing balls
//  - Single-frame and sub-frame simulation (set up lastPosition and position for each object, and step through intersection tests and resolution in detail)
// - Place arbitrary shapes into a simulation and allow them to intercollide
// - Stress test (control number/speed of collisiding objects with performance metrics)

static ScreenManager * screenManager;
static SingleFrameScreen * singleFrameScreen;
static BouncingBallScreen * bouncingBallScreen;
static ResourceManager * resourceManager;

static bool Target_draw() {
	return EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_DRAW), NULL);
}

static void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat) {
	if (keyCode == KEYBOARD_RETURN_OR_ENTER && (modifiers & MODIFIER_ALT_BIT)) {
		if (Shell_isFullScreen()) {
			Shell_exitFullScreen();
		} else {
			Shell_enterFullScreen(Shell_getDisplayIndexFromWindow());
		}
		EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(Shell_isFullScreen() ? EVENT_FULLSCREENED : EVENT_WINDOWED), NULL);
		
	} else if (keyCode == KEYBOARD_1) {
		ScreenManager_setScreen(screenManager, singleFrameScreen);
		
	} else if (keyCode == KEYBOARD_2) {
		ScreenManager_setScreen(screenManager, bouncingBallScreen);
		
	} else {
		struct keyEvent event;
		
		event.keyCode = keyCode;
		event.charCode = charCode;
		event.modifiers = modifiers;
		event.isRepeat = isRepeat;
		EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), &event);
	}
}

static void Target_keyUp(unsigned int keyCode, unsigned int modifiers) {
	struct keyEvent event;
	
	event.keyCode = keyCode;
	event.modifiers = modifiers;
	EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_KEY_UP), &event);
}

static void Target_keyModifiersChanged(unsigned int modifiers) {
	struct keyEvent event;
	
	g_shiftKeyDown = !!(modifiers & MODIFIER_SHIFT_BIT);
	g_altKeyDown = !!(modifiers & MODIFIER_ALT_BIT);
	g_controlKeyDown = !!(modifiers & MODIFIER_CONTROL_BIT);
	
	event.modifiers = modifiers;
	EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_KEY_MODIFIERS_CHANGED), &event);
}

static void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	struct mouseEvent event;
	
	event.button = buttonNumber;
	event.position = VECTOR2f(x, y);
	EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), &event);
}

static void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
	struct mouseEvent event;
	
	event.button = buttonNumber;
	event.position = VECTOR2f(x, y);
	EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), &event);
}

static void Target_mouseMoved(float x, float y) {
	struct mouseEvent event;
	
	event.position = VECTOR2f(x, y);
	EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_MOUSE_MOVED), &event);
}

static void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	struct mouseEvent event;
	
	event.button = buttonMask;
	event.position = VECTOR2f(x, y);
	EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), &event);
}

static void Target_scrollWheel(int deltaX, int deltaY) {
	struct mouseWheelEvent event;
	
	event.deltaX = deltaX;
	event.deltaY = deltaY;
	EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_MOUSE_SCROLL_WHEEL), &event);
}

static void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	g_viewWidth = newWidth;
	g_viewHeight = newHeight;
	g_viewRatio = (float) newWidth / newHeight;
	glViewport(0, 0, newWidth, newHeight);
	if (screenManager != NULL) {
		EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_RESIZED), NULL);
	}
}

static void Target_backgrounded() {
	if (screenManager != NULL) {
		EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_BACKGROUNDED), NULL);
	}
}

static void Target_foregrounded() {
	if (screenManager != NULL) {
		EventDispatcher_dispatchEvent(screenManager->eventDispatcher, ATOM(EVENT_FOREGROUNDED), NULL);
	}
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
	configuration->fullScreenMenuItem = true;
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
void WGLTarget_configure(void * instance, void * prevInstance, char * commandLine, int command, int argc, const char ** argv, struct WGLShellConfiguration * configuration) {
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
void GLXTarget_configure(int argc, const char ** argv, struct GLXShellConfiguration * configuration) {
#else
#error Unsupported platform
#endif
	int displayX, displayY;
	unsigned int displayWidth, displayHeight, windowWidth = 1280, windowHeight = 720;
	Vector2i preferredSizes[] = {{2560, 1440}, {1920, 1080}, {1280, 720}, {853, 480}};
	unsigned int preferredSizeIndex;
	
	if (argc > 1 && sscanf(argv[1], "%d", &g_randomSeed)) {
		g_fixedRandomSeed = true;
		printf("Using fixed random seed: %d\n", g_randomSeed);
	}
	
	Shell_getSafeWindowRect(0, &displayX, &displayY, &displayWidth, &displayHeight);
	for (preferredSizeIndex = 0; preferredSizeIndex < sizeof(preferredSizes) / sizeof(Vector2i); preferredSizeIndex++) {
		if ((int) displayWidth >= preferredSizes[preferredSizeIndex].x && (int) displayHeight >= preferredSizes[preferredSizeIndex].y) {
			windowWidth = preferredSizes[preferredSizeIndex].x;
			windowHeight = preferredSizes[preferredSizeIndex].y;
			break;
		}
	}
	if (preferredSizeIndex >= sizeof(preferredSizes) / sizeof(Vector2i)) {
		windowWidth = displayWidth;
		windowHeight = displayHeight;
	}
	configuration->windowX = displayX + (displayWidth - windowWidth) / 2;
	configuration->windowY = displayY + (displayHeight - windowHeight) / 2;
	configuration->windowWidth = g_viewWidth = windowWidth;
	configuration->windowHeight = g_viewHeight = windowHeight;
	configuration->windowTitle = "Collision Test Harness";
	
	registerShellCallbacks();
}

void Target_init() {
	chdir(Shell_getResourcePath());
	
	resourceManager = ResourceManager_create();
	ResourceWiring_addTypeHandlers(resourceManager);
	
	screenManager = ScreenManager_create();
	singleFrameScreen = SingleFrameScreen_create(resourceManager);
	bouncingBallScreen = BouncingBallScreen_create(resourceManager);
	ScreenManager_addScreen(screenManager, singleFrameScreen);
	ScreenManager_addScreen(screenManager, bouncingBallScreen);
	ScreenManager_setScreen(screenManager, singleFrameScreen);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	Shell_setVSync(false, false);
	Shell_setVSync(false, true);
	Shell_mainLoop();
}
