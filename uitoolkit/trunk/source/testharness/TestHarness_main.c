#include "3dmodelio/TextureAtlasData.h"
#include "gamemath/MouseCoordinateTransforms.h"
#include "glbitmapfont/GLBitmapFont.h"
#include "glgraphics/DynamicSpriteRenderable.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/Renderer.h"
#include "glgraphics/TextureAtlas.h"
#include "jsonserialization/JSONDeserializationContext.h"
#include "pngimageio/PNGImageIO.h"
#include "shell/Shell.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"
#include "uitoolkit/UIAppearance.h"
#include "uitoolkit/UIButton.h"
#include "uitoolkit/UIContainer.h"
#include "uitoolkit/UILabel.h"

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
#error Unsupported platform
#endif

#include <stdio.h>
#include <unistd.h>

static Renderer * renderer;
static unsigned int viewWidth = 1280, viewHeight = 720;
static float viewRatio = 16.0f / 9.0f;
static UIElement * rootElement;
static UIAppearance * appearance;
static TextureAtlas * atlas;
static GLBitmapFont * font;
static DynamicSpriteRenderable * renderable;

static bool Target_draw() {
	Renderer_clear(renderer);
	Renderer_setViewMatrix(renderer, MATRIX4x4f_IDENTITY);
	Renderer_setDrawMode(renderer, RENDERER_2D_TRANSLUCENT);
	Renderer_drawLayer(renderer, 0, false);
	return true;
}

static void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat) {
	if (keyCode == KEYBOARD_RETURN_OR_ENTER && (modifiers & MODIFIER_ALT_BIT)) {
		if (Shell_isFullScreen()) {
			Shell_exitFullScreen();
		} else {
			Shell_enterFullScreen(Shell_getDisplayIndexFromWindow());
		}
		
	} else {
		bool handled = rootElement->keyDown(rootElement, charCode, keyCode, modifiers, isRepeat);
		if (handled) {
			Shell_redisplay();
		}
	}
}

static void Target_keyUp(unsigned int keyCode, unsigned int modifiers) {
	if (rootElement->keyUp(rootElement, keyCode, modifiers)) {
		Shell_redisplay();
	}
}

static void Target_keyModifiersChanged(unsigned int modifiers) {
	if (rootElement->keyModifiersChanged(rootElement, modifiers)) {
		Shell_redisplay();
	}
}

static Vector2f transformMousePosition(float x, float y) {
	return VECTOR2f(x, viewHeight - y);
}

static void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	Vector2f position = transformMousePosition(x, y);
	if (rootElement->mouseDown(rootElement, buttonNumber, position.x, position.y)) {
		Shell_redisplay();
	}
}

static void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
	Vector2f position = transformMousePosition(x, y);
	if (rootElement->mouseUp(rootElement, buttonNumber, position.x, position.y)) {
		Shell_redisplay();
	}
}

static void Target_mouseMoved(float x, float y) {
	Vector2f position = transformMousePosition(x, y);
	if (rootElement->mouseMoved(rootElement, position.x, position.y)) {
		Shell_redisplay();
	}
}

static void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	Vector2f position = transformMousePosition(x, y);
	if (rootElement->mouseDragged(rootElement, buttonMask, position.x, position.y)) {
		Shell_redisplay();
	}
}

static void Target_scrollWheel(int deltaX, int deltaY) {
	if (rootElement->scrollWheel(rootElement, deltaX, deltaY)) {
		Shell_redisplay();
	}
}

static void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	viewWidth = newWidth;
	viewHeight = newHeight;
	viewRatio = (float) newWidth / newHeight;
	glViewport(0, 0, viewWidth, viewHeight);
	if (renderer != NULL) {
		Renderer_setProjectionMatrix(renderer, Matrix4x4f_ortho(MATRIX4x4f_IDENTITY, 0.0f, viewWidth, 0.0f, viewHeight, -1.0f, 1.0f));
	}
}

static void Target_backgrounded() {
}

static void Target_foregrounded() {
}

static void getUIVertices(DynamicSpriteRenderable * sprite, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount, void * context) {
	rootElement->getVertices(rootElement, VECTOR2f_ZERO, outVertices, outIndexes, ioVertexCount, ioIndexCount);
}

static void button1ActionCallback(UIButton * sender, void * context) {
	printf("Button 1 action\n");
}

static void button2ActionCallback(UIButton * sender, void * context) {
	printf("Button 2 action\n");
}

static void initTestUI() {
	struct UIAppearance_metrics metrics = {
		.fontHeight = 30.0f,
		.buttonLabelPadding = 4.0f,
		.buttonLabelColor = COLOR4f(0.0f, 0.0f, 0.0f, 1.0f),
		.buttonSlices = {2, 1, 2, 2, 1, 2}
	};
	JSONDeserializationContext * context;
	TextureAtlasData * atlasData;
	BitmapImage * image;
	UIButton * button1, * button2;
	UIElement * containerElements[3];
	UIContainer * container;
	UILabel * label;
	
	context = JSONDeserializationContext_createWithFile("testappearance.atlas");
	if (context->status != SERIALIZATION_ERROR_OK) {
		fprintf(stderr, "Fatal error: Couldn't load testappearance.atlas (status %d)\n", context->status);
		exit(EXIT_FAILURE);
	}
	atlasData = TextureAtlasData_deserialize(context);
	context->dispose(context);
	if (atlasData == NULL) {
		fprintf(stderr, "Fatal error: Couldn't deserialize testappearance.atlas (status %d)\n", context->status);
		exit(EXIT_FAILURE);
	}
	atlas = TextureAtlasData_createTextureAtlas(atlasData);
	
	image = PNGImageIO_loadPNGFile("testappearance_atlas.png", BITMAP_PIXEL_FORMAT_RGBA_8888, true);
	if (image == NULL) {
		fprintf(stderr, "Fatal error: Couldn't load testappearance_atlas.png\n");
		exit(EXIT_FAILURE);
	}
	TextureAtlas_setTexture(atlas, atlasData->magnifyNearest, image->width, image->height, image->pixels);
	BitmapImage_dispose(image);
	TextureAtlasData_dispose(atlasData);
	
	context = JSONDeserializationContext_createWithFile("testappearance.bitmapfont");
	if (context->status != SERIALIZATION_ERROR_OK) {
		fprintf(stderr, "Fatal error: Couldn't load testappearance.bitmapfont (status %d)\n", context->status);
		exit(EXIT_FAILURE);
	}
	font = GLBitmapFont_deserialize(context);
	context->dispose(context);
	if (font == NULL) {
		fprintf(stderr, "Fatal error: Couldn't deserialize testappearance.bitmapfont (status %d)\n", context->status);
		exit(EXIT_FAILURE);
	}
	GLBitmapFont_setTextureAtlas(font, atlas, false);
	
	appearance = UIAppearance_create(atlas, font, metrics);
	button1 = UIButton_create(appearance, VECTOR2f(20.0f, 20.0f), VECTOR2f(0.0f, 0.0f), "I'm a button", UIBUTTON_SIZE_TO_FIT_LABEL, button1ActionCallback, NULL);
	button2 = UIButton_create(appearance, VECTOR2f(20.0f, 60.0f), VECTOR2f(0.0f, 0.0f), "Hello", UIBUTTON_SIZE_TO_FIT_LABEL, button2ActionCallback, NULL);
	label = UILabel_create(appearance, VECTOR2f(30.0f, 120.0f), VECTOR2f_ZERO, VECTOR2f(0.0f, 0.0f), "I'm a label", COLOR4f(0.0f, 0.5f, 0.5f, 1.0f), WORD_WRAP_NONE, UILABEL_OVERFLOW_RESIZE, UILABEL_OVERFLOW_RESIZE);
	containerElements[0] = (UIElement *) button1;
	containerElements[1] = (UIElement *) button2;
	containerElements[2] = (UIElement *) label;
	container = UIContainer_create(appearance, VECTOR2f(100.0f, 100.0f), containerElements, sizeof(containerElements) / sizeof(containerElements[0]));
	rootElement = (UIElement *) container;
	
	renderable = DynamicSpriteRenderable_create(atlas, getUIVertices, NULL);
	Renderer_addRenderable(renderer, 0, (Renderable *) renderable);
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
	configuration->windowTitle = "UIToolkit Test Harness";
	configuration->windowWidth = viewWidth;
	configuration->windowHeight = viewHeight;
	configuration->useGLCoreProfile = true;
#elif defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
void WGLTarget_configure(void * instance, void * prevInstance, char * commandLine, int command, int argc, const char ** argv, struct WGLShellConfiguration * configuration) {
	configuration->windowTitle = "UIToolkit Test Harness";
	configuration->windowWidth = viewWidth;
	configuration->windowHeight = viewHeight;
	configuration->useGLCoreProfile = true;
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
void GLXTarget_configure(int argc, const char ** argv, struct GLXShellConfiguration * configuration) {
	configuration->windowTitle = "UIToolkit Test Harness";
#else
void GLUTTarget_configure(int argc, const char ** argv, struct GLUTShellConfiguration * configuration) {
	configuration->windowTitle = "UIToolkit Test Harness";
#endif
	configuration->displayMode.depthBuffer = true;
	registerShellCallbacks();
}

void Target_init() {
	chdir(Shell_getResourcePath());
	
	renderer = Renderer_create(1);
	Renderer_setClearColor(renderer, COLOR4f(1.0f, 1.0f, 1.0f, 0.0f));
	Renderer_setProjectionMatrix(renderer, Matrix4x4f_ortho(MATRIX4x4f_IDENTITY, 0.0f, viewWidth, 0.0f, viewHeight, -1.0f, 1.0f));
	
	initTestUI();
	
	Shell_mainLoop();
}
