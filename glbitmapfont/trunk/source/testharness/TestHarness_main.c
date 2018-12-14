#include "shell/Shell.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"
#include "3dmodelio/TextureAtlasData.h"
#include "gamemath/Matrix4x4f.h"
#include "glbitmapfont/GLBitmapFont.h"
#include "glbitmapfont/TextFlow.h"
#include "glgraphics/GLIncludes.h"
#include "jsonserialization/JSONDeserializationContext.h"
#include "pngimageio/PNGImageIO.h"
#include "utilities/IOUtilities.h"

// TODO: This test harness is a big mess; redo to use more modern rendering techniques
#if defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
#include "eaglshell/EAGLShell.h"
#include "eaglshell/EAGLTarget.h"
#elif defined(STEM_PLATFORM_macosx)
#include "nsopenglshell/NSOpenGLShell.h"
#include "nsopenglshell/NSOpenGLTarget.h"
#else
#include "glutshell/GLUTTarget.h"
#endif

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FREEFORM_LENGTH_MAX 100

static char freeformText[FREEFORM_LENGTH_MAX + 1];
static unsigned int viewportWidth = 800, viewportHeight = 600;
static GLBitmapFont * font;
static TextFlow * textFlow;
static const char * jsonPath = NULL;
static size_t lastIndexAtWidth = 0;
static bool lastLeadingEdge = false;
static float scale = 1.0f;

static void drawString(GLBitmapFont * font, const char * string, size_t length, float emHeight, float offsetX, float offsetY) {
	static GLuint indexBufferID, vertexBufferID;
	unsigned int indexCount, vertexCount;
	struct vertex_p2f_t2f_c4f * vertices;
	GLuint * indexes;
	
	if (indexBufferID == 0) {
		glGenBuffersARB(1, &indexBufferID);
		glGenBuffersARB(1, &vertexBufferID);
	}
	vertexCount = indexCount = 0;
	GLBitmapFont_getStringVertices(font, string, length, emHeight, VECTOR2f(offsetX, offsetY), VECTOR2f(0.0f, 0.0f), false, GLBITMAPFONT_NO_CLIP, GLBITMAPFONT_NO_CLIP, COLOR4f(1.0f, 1.0f, 1.0f, 1.0f), NULL, NULL, &vertexCount, &indexCount);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(struct vertex_p2f_t2f_c4f) * vertexCount, NULL, GL_STREAM_DRAW);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, NULL, GL_STREAM_DRAW);
	vertices = glMapBufferARB(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	indexes = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertexCount = indexCount = 0;
	GLBitmapFont_getStringVertices(font, string, length, emHeight, VECTOR2f(offsetX, offsetY), VECTOR2f(0.0f, 0.0f), false, GLBITMAPFONT_NO_CLIP, GLBITMAPFONT_NO_CLIP, COLOR4f(1.0f, 1.0f, 1.0f, 1.0f), vertices, indexes, &vertexCount, &indexCount);
	glUnmapBufferARB(GL_ARRAY_BUFFER);
	glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, position));
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, texCoords));
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, font->atlas->textureID);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

static void drawTextFlow(TextFlow * textFlow, float emHeight, float offsetX, float offsetY) {
	static GLuint indexBufferID, vertexBufferID;
	unsigned int indexCount, vertexCount;
	struct vertex_p2f_t2f_c4f * vertices;
	GLuint * indexes;
	
	if (indexBufferID == 0) {
		glGenBuffersARB(1, &indexBufferID);
		glGenBuffersARB(1, &vertexBufferID);
	}
	vertexCount = indexCount = 0;
	TextFlow_getVertices(textFlow, emHeight, VECTOR2f(offsetX, offsetY), VECTOR2f(0.0f, 0.0f), false, GLBITMAPFONT_NO_CLIP, GLBITMAPFONT_NO_CLIP, COLOR4f(1.0f, 1.0f, 1.0f, 1.0f), NULL, NULL, &vertexCount, &indexCount);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(struct vertex_p2f_t2f_c4f) * vertexCount, NULL, GL_STREAM_DRAW);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, NULL, GL_STREAM_DRAW);
	vertices = glMapBufferARB(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	indexes = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertexCount = indexCount = 0;
	TextFlow_getVertices(textFlow, emHeight, VECTOR2f(offsetX, offsetY), VECTOR2f(0.0f, 0.0f), false, GLBITMAPFONT_NO_CLIP, GLBITMAPFONT_NO_CLIP, COLOR4f(1.0f, 1.0f, 1.0f, 1.0f), vertices, indexes, &vertexCount, &indexCount);
	glUnmapBufferARB(GL_ARRAY_BUFFER);
	glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, position));
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, texCoords));
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, font->atlas->textureID);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

static bool Target_draw() {
	Matrix4x4f projectionMatrix;
	float ratio;
	float stringWidth;
	char indexString[32];
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	ratio = (float) viewportWidth / viewportHeight;
	projectionMatrix = Matrix4x4f_ortho(MATRIX4x4f_IDENTITY, -ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectionMatrix.m);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	drawTextFlow(textFlow, 0.0625f * scale, -6.0f * 0.0625f * scale, 0.625f);
	
	stringWidth = GLBitmapFont_measureString(font, "Hello, world!", 13);
	drawString(font, "Hello, world!", 13, 0.1f * scale, stringWidth * -0.05f * scale, 0.0f);
	
	stringWidth = GLBitmapFont_measureString(font, freeformText, strlen(freeformText));
	drawString(font, freeformText, strlen(freeformText), 0.0625f * scale, stringWidth * -0.03125f * scale, -0.0625f * scale);
	
	glColor4f(0.875f, 0.875f, 0.5f, 1.0f);
	snprintf(indexString, 32, "%u, %s", (unsigned int) lastIndexAtWidth, lastLeadingEdge ? "true" : "false");
	stringWidth = GLBitmapFont_measureString(font, indexString, strlen(indexString));
	drawString(font, indexString, strlen(indexString), 0.05f * scale, stringWidth * -0.025f * scale, 0.1f * scale);
	
	return true;
}

static void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	viewportWidth = newWidth;
	viewportHeight = newHeight;
}

static void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int keyModifiers, bool isRepeat) {
	if (keyCode == KEYBOARD_DELETE_OR_BACKSPACE && strlen(freeformText) > 0) {
		freeformText[strlen(freeformText) - 1] = '\x00';
		Shell_redisplay();
	} else if (charCode >= GLBITMAPFONT_PRINTABLE_MIN && charCode <= GLBITMAPFONT_PRINTABLE_MAX && strlen(freeformText) < FREEFORM_LENGTH_MAX) {
		freeformText[strlen(freeformText)] = charCode;
		Shell_redisplay();
#if defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
	} else if (keyCode == KEYBOARD_RETURN_OR_ENTER) {
		EAGLShell_hideKeyboard();
#endif
	}
}

static void queryIndexAtPosition(float x, float y) {
	float stringWidth;
	const char * string;
	size_t stringLength;
	float emHeight;
	
	if (y > viewportHeight / 2) {
		string = freeformText;
		stringLength = strlen(freeformText);
		emHeight = 0.0625f;
		
	} else {
		string = "Hello, world!";
		stringLength = 13;
		emHeight = 0.1f;
	}
	
	stringWidth = GLBitmapFont_measureString(font, string, stringLength);
	lastIndexAtWidth = GLBitmapFont_indexAtWidth(font, string, stringLength, (x / (float) viewportHeight * 2.0f - (float) viewportWidth / viewportHeight + stringWidth * emHeight * 0.5f) / emHeight, &lastLeadingEdge);
	Shell_redisplay();
}

static void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	queryIndexAtPosition(x, y);
#if defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
	if (buttonNumber > 0) {
		EAGLShell_showKeyboard();
	}
#endif
}

static void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	queryIndexAtPosition(x, y);
}

static void registerShellCallbacks() {
	Shell_drawFunc(Target_draw);
	Shell_resizeFunc(Target_resized);
	Shell_keyDownFunc(Target_keyDown);
	Shell_mouseDownFunc(Target_mouseDown);
	Shell_mouseDraggedFunc(Target_mouseDragged);
}

static void printUsage() {
	fprintf(stderr, "Usage: glbitmapfont_testharness [-json /path/to/font.json] [-scale %%f]\n");
}

static void parseArgs(int argc, const char ** argv) {
	int argIndex;
	bool printUsageAfterParsing = false;
	
	for (argIndex = 1; argIndex < argc; argIndex++) {
		if (!strcmp(argv[argIndex], "--help")) {
			printUsageAfterParsing = true;
			
		} else if (!strcmp(argv[argIndex], "-json")) {
			argIndex++;
			if (argIndex >= argc) {
				printf("-json specified as last argument; must be followed by a file path\n");
				printUsageAfterParsing = true;
				break;
			}
			jsonPath = argv[argIndex];
			
		} else if (!strcmp(argv[argIndex], "-scale")) {
			argIndex++;
			if (argIndex >= argc) {
				printf("-scale specified as last argument; must be followed by a number\n");
				printUsageAfterParsing = true;
				break;
			}
			if (!sscanf(argv[argIndex], "%f", &scale)) {
				printf("Couldn't parse value specified to -scale (%s) as a number\n", argv[argIndex]);
				printUsageAfterParsing = true;
				break;
			}
			
		} else {
			printf("Unrecognized argument: %s\n", argv[argIndex]);
			printUsageAfterParsing = true;
		}
	}
	if (printUsageAfterParsing) {
		printUsage();
	}
}

#if defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
	parseArgs(argc, (const char **) argv);
	configuration->preferredOpenGLAPIVersion = EAGLShellOpenGLVersion_ES1 | EAGLShellOpenGLVersion_ES2;
	registerShellCallbacks();
}
#elif defined(STEM_PLATFORM_macosx)
void NSOpenGLTarget_configure(int argc, const char ** argv, struct NSOpenGLShellConfiguration * configuration) {
	parseArgs(argc, argv);
	configuration->windowTitle = "GLBitmapFont Test Harness";
	registerShellCallbacks();
}
#else
void GLUTTarget_configure(int argc, const char ** argv, struct GLUTShellConfiguration * configuration) {
	parseArgs(argc, argv);
	configuration->windowTitle = "GLBitmapFont Test Harness";
	registerShellCallbacks();
}
#endif

void Target_init() {
	const char * fontJSONFilePath;
	char atlasJSONFilePath[PATH_MAX], textureImageFilePath[PATH_MAX];
	JSONDeserializationContext * context;
	TextureAtlasData * atlasData;
	TextureAtlas * atlas;
	BitmapImage * image;
	
	if (jsonPath == NULL) {
		chdir(Shell_getResourcePath());
		fontJSONFilePath = "test_font.json";
	} else {
		fontJSONFilePath = jsonPath;
	}
	
	context = JSONDeserializationContext_createWithFile(fontJSONFilePath);
	if (context->status != SERIALIZATION_ERROR_OK) {
		fprintf(stderr, "Fatal error: Couldn't load %s (status %d)\n", fontJSONFilePath, context->status);
		exit(EXIT_FAILURE);
	}
	font = GLBitmapFont_deserialize(context);
	context->dispose(context);
	if (font == NULL) {
		fprintf(stderr, "Fatal error: Couldn't deserialize %s (status %d)\n", fontJSONFilePath, context->status);
		exit(EXIT_FAILURE);
	}
	
	if (jsonPath == NULL) {
		strncpy(atlasJSONFilePath, font->atlasName, PATH_MAX);
		
	} else {
		size_t charIndex;
		
		strncpy(atlasJSONFilePath, jsonPath, PATH_MAX);
		for (charIndex = strlen(atlasJSONFilePath) - 1; charIndex > 0; charIndex--) {
			if (atlasJSONFilePath[charIndex] == '/') {
				charIndex++;
				break;
			}
		}
		strncpy(atlasJSONFilePath + charIndex, font->atlasName, PATH_MAX - charIndex);
	}
	
	context = JSONDeserializationContext_createWithFile(atlasJSONFilePath);
	if (context->status != SERIALIZATION_ERROR_OK) {
		fprintf(stderr, "Fatal error: Couldn't load %s (status %d)\n", atlasJSONFilePath, context->status);
		exit(EXIT_FAILURE);
	}
	atlasData = TextureAtlasData_deserialize(context);
	context->dispose(context);
	if (atlasData == NULL) {
		fprintf(stderr, "Fatal error: Couldn't deserialize %s (status %d)\n", atlasJSONFilePath, context->status);
		exit(EXIT_FAILURE);
	}
	atlas = TextureAtlasData_createTextureAtlas(atlasData);
	
	if (jsonPath == NULL) {
		strncpy(textureImageFilePath, atlasData->textureMapName, PATH_MAX);
		
	} else {
		size_t charIndex;
		
		strncpy(textureImageFilePath, jsonPath, PATH_MAX);
		for (charIndex = strlen(textureImageFilePath) - 1; charIndex > 0; charIndex--) {
			if (textureImageFilePath[charIndex] == '/') {
				charIndex++;
				break;
			}
		}
		strncpy(textureImageFilePath + charIndex, atlasData->textureMapName, PATH_MAX - charIndex);
	}
	
	image = PNGImageIO_loadPNGFile(textureImageFilePath, BITMAP_PIXEL_FORMAT_RGBA_8888, true);
	if (image == NULL) {
		fprintf(stderr, "Fatal error: Couldn't load %s\n", textureImageFilePath);
		exit(EXIT_FAILURE);
	}
	TextureAtlas_setTexture(atlas, atlasData->magnifyNearest, image->width, image->height, image->pixels);
	image->dispose(image);
	GLBitmapFont_setTextureAtlas(font, atlas, true);
	TextureAtlasData_dispose(atlasData);
	
	textFlow = TextFlow_create(font, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut eleifend commodo placerat. Aenean a viverra leo.", WORD_WRAP_NORMAL, 12.0f);
	
	memset(freeformText, 0, FREEFORM_LENGTH_MAX + 1);
	Shell_mainLoop();
}
