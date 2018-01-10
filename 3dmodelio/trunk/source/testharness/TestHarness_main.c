#include "3dmodelio/Obj3DModelIO.h"
#include "gamemath/Matrix4x4f.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/MeshRenderable.h"
#include "glgraphics/OrbitCamera.h"
#include "glgraphics/Renderer.h"
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

#include <stdio.h>
#include <unistd.h>

static Renderer * renderer;
static OrbitCamera * cameraController;
static bool shiftKeyDown, controlKeyDown;

static bool Target_draw() {
	Renderer_clear(renderer);
	Renderer_setViewMatrix(renderer, OrbitCamera_getMatrix(cameraController));
	Renderer_beginDrawing(renderer);
	Renderer_drawLayer(renderer, RENDER_LAYER_3D_OPAQUE);
	Renderer_endDrawing(renderer);
	return true;
}

static void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat) {
}

static void Target_keyUp(unsigned int keyCode, unsigned int modifiers) {
}

static void Target_keyModifiersChanged(unsigned int modifiers) {
	shiftKeyDown = !!(modifiers & MODIFIER_SHIFT_BIT);
	controlKeyDown = !!(modifiers & MODIFIER_CONTROL_BIT);
}

static void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	Shell_setMouseDeltaMode(true);
}

static void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
	Shell_setMouseDeltaMode(false);
}

static void Target_mouseMoved(float x, float y) {
}

static void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	if (shiftKeyDown) {
		if (controlKeyDown) {
			OrbitCamera_offset(cameraController, 0.0f, 0.0f, -y);
		} else {
			OrbitCamera_offset(cameraController, x, -y, 0.0f);
		}
		
	} else if (controlKeyDown) {
		OrbitCamera_zoom(cameraController, y);
		
	} else {
		OrbitCamera_rotate(cameraController, x, y);
	}
	Shell_redisplay();
}

static void Target_scrollWheel(int deltaX, int deltaY) {
}

static void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	if (renderer != NULL) {
		Renderer_setProjectionMatrix(renderer, Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, 60.0f, (float) newWidth / (float) newHeight, 0.5f, 100.0f));
	}
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
	configuration->windowTitle = "GLGraphics";
	configuration->useGLCoreProfile = true;
#elif defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
void WGLTarget_configure(void * instance, void * prevInstance, char * commandLine, int command, int argc, const char ** argv, struct WGLShellConfiguration * configuration) {
	configuration->windowTitle = "GLGraphics";
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
void GLXTarget_configure(int argc, const char ** argv, struct GLXShellConfiguration * configuration) {
	configuration->windowTitle = "GLGraphics";
#else
void GLUTTarget_configure(int argc, const char ** argv, struct GLUTShellConfiguration * configuration) {
	configuration->windowTitle = "GLGraphics";
#endif
	configuration->displayMode.depthBuffer = true;
	registerShellCallbacks();
}

void Target_init() {
	MeshData * meshData;
	VertexBuffer * vertexBuffer;
	MeshRenderable * renderable;
	
	chdir(Shell_getResourcePath());
	renderer = Renderer_create();
	Renderer_setClearColor(renderer, COLOR4f(0.0f, 0.125f, 0.25f, 0.0f));
	Renderer_setLights(renderer, VECTOR3f(0.0f, 8.0f, 8.0f), COLOR4f(1.0f, 1.0f, 0.95f, 1.0f), VECTOR3f(-1.0f, -2.0f, -8.0f), COLOR4f(0.8f, 0.8f, 0.8f, 1.0f), COLOR4f(0.1f, 0.1f, 0.105f, 1.0f));
	
	meshData = Obj3DModelIO_loadFile("suzanne.obj");
	vertexBuffer = VertexBuffer_createPTNC(meshData->vertices, meshData->vertexCount, meshData->indexes, meshData->indexCount);
	renderable = MeshRenderable_create(vertexBuffer, NULL, NULL, MATRIX4x4f_IDENTITY);
	MeshData_dispose(meshData);
	Renderer_addRenderable(renderer, RENDER_LAYER_3D_OPAQUE, (Renderable *) renderable);
	
	cameraController = OrbitCamera_create();
	
	Shell_mainLoop();
}
