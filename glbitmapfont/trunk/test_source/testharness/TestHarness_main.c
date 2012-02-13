#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include "shell/Target.h"
#include "3dmath/Matrix.h"
#include "glbitmapfont/GLBitmapFont.h"
#include "glgraphics/GLGraphics.h"
#include "glgraphics/GLIncludes.h"
#include "gltexture/GLTexture.h"
#include "jsonserialization/JSONDeserializationContext.h"
#include "pngimageio/PNGImageIO.h"
#include "utilities/IOUtilities.h"

#if defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
#include "eaglshell/EAGLShell.h"
#include "eaglshell/EAGLTarget.h"
#else
#include "glutshell/GLUTTarget.h"
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FREEFORM_LENGTH_MAX 100

static char freeformText[FREEFORM_LENGTH_MAX + 1];
static unsigned int viewportWidth = 800, viewportHeight = 600;
static GLBitmapFont * font;
static const char * jsonPath = NULL;
static size_t lastIndexAtWidth = 0;
static bool lastLeadingEdge = false;
static GLint matrixUniform;
static GLint textureUniform;

void Target_init() {
	const char * fontJSONFilePath;
	char textureJSONFilePath[PATH_MAX], textureImageFilePath[PATH_MAX];
	JSONDeserializationContext * context;
	GLTexture * texture;
	BitmapImage * image;
	int result;
	
	if (jsonPath == NULL) {
		result = chdir(Shell_getResourcePath());
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
		strncpy(textureJSONFilePath, font->textureName, PATH_MAX);
		
	} else {
		size_t charIndex;
		
		strncpy(textureJSONFilePath, jsonPath, PATH_MAX);
		for (charIndex = strlen(textureJSONFilePath) - 1; charIndex > 0; charIndex--) {
			if (textureJSONFilePath[charIndex] == '/') {
				charIndex++;
				break;
			}
		}
		strncpy(textureJSONFilePath + charIndex, font->textureName, PATH_MAX - charIndex);
	}
	
	context = JSONDeserializationContext_createWithFile(textureJSONFilePath);
	if (context->status != SERIALIZATION_ERROR_OK) {
		fprintf(stderr, "Fatal error: Couldn't load %s (status %d)\n", textureJSONFilePath, context->status);
		exit(EXIT_FAILURE);
	}
	texture = GLTexture_deserialize(context);
	context->dispose(context);
	if (texture == NULL) {
		fprintf(stderr, "Fatal error: Couldn't deserialize %s (status %d)\n", textureJSONFilePath, context->status);
		exit(EXIT_FAILURE);
	}
	
	if (jsonPath == NULL) {
		strncpy(textureImageFilePath, texture->imageName, PATH_MAX);
		
	} else {
		size_t charIndex;
		
		strncpy(textureImageFilePath, jsonPath, PATH_MAX);
		for (charIndex = strlen(textureImageFilePath) - 1; charIndex > 0; charIndex--) {
			if (textureImageFilePath[charIndex] == '/') {
				charIndex++;
				break;
			}
		}
		strncpy(textureImageFilePath + charIndex, texture->imageName, PATH_MAX - charIndex);
	}
	
	image = PNGImageIO_loadPNGFile(textureImageFilePath, PNG_PIXEL_FORMAT_AUTOMATIC, true);
	if (image == NULL) {
		fprintf(stderr, "Fatal error: Couldn't load %s\n", textureImageFilePath);
		exit(EXIT_FAILURE);
	}
	texture->setImage(texture, 0, image->width, image->height, image->bytesPerRow, image->pixels);
	image->dispose(image);
	font->setTexture(font, texture, true);
	
	memset(freeformText, 0, FREEFORM_LENGTH_MAX + 1);
	
	if (GLGraphics_getOpenGLAPIVersion() == GL_API_VERSION_ES2) {
		void * fileContents;
		size_t fileLength;
		GLint shaderLength;
		GLuint vertexShader, fragmentShader;
		GLuint shaderProgram;
		GLint logLength;
		
		result = chdir(Shell_getResourcePath());
		
		shaderProgram = glCreateProgram();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		fileContents = readFileSimple("basic.vert", &fileLength);
		shaderLength = fileLength;
		glShaderSource(vertexShader, 1, (const GLchar **) &fileContents, &shaderLength);
		free(fileContents);
		
		fileContents = readFileSimple("basic.frag", &fileLength);
		shaderLength = fileLength;
		glShaderSource(fragmentShader, 1, (const GLchar **) &fileContents, &shaderLength);
		free(fileContents);
		
		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			GLchar * log = malloc(logLength);
			glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
			fprintf(stderr, "Vertex shader compile log:\n%s\n", log);
			free(log);
		}
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			GLchar * log = malloc(logLength);
			glGetShaderInfoLog(fragmentShader, logLength, &logLength, log);
			fprintf(stderr, "Fragment shader compile log:\n%s\n", log);
			free(log);
		}
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindAttribLocation(shaderProgram, 0, "vertexPositionAttrib");
		glBindAttribLocation(shaderProgram, 1, "vertexTexCoordAttrib");
		glBindAttribLocation(shaderProgram, 2, "vertexColorAttrib");
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			GLchar * log = malloc(logLength);
			glGetProgramInfoLog(shaderProgram, logLength, &logLength, log);
			fprintf(stderr, "Program link log:\n%s\n", log);
			free(log);
		}
		glValidateProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			GLchar * log = malloc(logLength);
			glGetProgramInfoLog(shaderProgram, logLength, &logLength, log);
			fprintf(stderr, "Program validation log:\n%s\n", log);
			free(log);
		}
		matrixUniform = glGetUniformLocation(shaderProgram, "matrix");
		textureUniform = glGetUniformLocation(shaderProgram, "texture");
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glUseProgram(shaderProgram);
	}
	
	Shell_mainLoop();
}

#if !TARGET_OPENGL_ES
#define glOrthof glOrtho
#endif

bool Target_draw() {
	Matrix projectionMatrix;
	float ratio;
	float stringWidth;
	char indexString[32];
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	ratio = (float) viewportWidth / viewportHeight;
	projectionMatrix = Matrix_ortho(Matrix_identity(), -ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
	
	if (GLGraphics_getOpenGLAPIVersion() == GL_API_VERSION_ES2) {
		glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, projectionMatrix.m);
		glUniform1i(textureUniform, 0);
		glVertexAttrib4f(2, 1.0f, 1.0f, 1.0f, 1.0f);
		
	} else {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(projectionMatrix.m);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	stringWidth = font->measureString(font, "Hello, world!", 13);
	font->drawString(font, "Hello, world!", 13, 0.1f, stringWidth * -0.05f, 0.0f, 0.0f);
	
	stringWidth = font->measureString(font, freeformText, strlen(freeformText));
	font->drawString(font, freeformText, strlen(freeformText), 0.0625f, stringWidth * -0.03125f, -0.0625f, 0.0f);
	
	if (GLGraphics_getOpenGLAPIVersion() == GL_API_VERSION_ES2) {
		glVertexAttrib4f(2, 0.875f, 0.875f, 0.5f, 1.0f);
	} else {
		glColor4f(0.875f, 0.875f, 0.5f, 1.0f);
	}
	snprintf(indexString, 32, "%u, %s", (unsigned int) lastIndexAtWidth, lastLeadingEdge ? "true" : "false");
	stringWidth = font->measureString(font, indexString, strlen(indexString));
	font->drawString(font, indexString, strlen(indexString), 0.05f, stringWidth * -0.025f, 0.1f, 0.0f);
	
	return true;
}

void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	viewportWidth = newWidth;
	viewportHeight = newHeight;
}

void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int keyModifiers) {
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

void Target_keyUp(unsigned int keyCode, unsigned int keyModifiers) {
}

void Target_keyModifiersChanged(unsigned int keyModifiers) {
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
	
	stringWidth = font->measureString(font, string, stringLength);
	lastIndexAtWidth = font->indexAtWidth(font, string, stringLength, (x / (float) viewportHeight * 2.0f - (float) viewportWidth / viewportHeight + stringWidth * emHeight * 0.5f) / emHeight, &lastLeadingEdge);
	Shell_redisplay();
}

void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	queryIndexAtPosition(x, y);
#if defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
	if (buttonNumber > 0) {
		EAGLShell_showKeyboard();
	}
#endif
}

void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
}

void Target_mouseMoved(float x, float y) {
}

void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	queryIndexAtPosition(x, y);
}

#if defined(STEM_PLATFORM_iphonesimulator) || defined(STEM_PLATFORM_iphoneos)
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
	configuration->preferredOpenGLAPIVersion = EAGLShellOpenGLVersion_ES1 | EAGLShellOpenGLVersion_ES2;
}

void EAGLTarget_openURL(const char * url) {
}

void EAGLTarget_touchesCancelled(unsigned int buttonMask) {
}

void EAGLTarget_accelerometer(double x, double y, double z) {
}
#else
static void printUsage() {
	fprintf(stderr, "Usage: glbitmapfont_testharness [-json /path/to/font.json]\n");
}

void GLUTTarget_configure(int argc, const char ** argv, struct GLUTShellConfiguration * configuration) {
	int argIndex;
	
	for (argIndex = 1; argIndex < argc; argIndex++) {
		if (!strcmp(argv[argIndex], "-json")) {
			argIndex++;
			if (argIndex >= argc) {
				printUsage();
				break;
			}
			jsonPath = argv[argIndex];
			
		} else {
			printUsage();
		}
	}
	configuration->windowTitle = "GLBitmapFont Test Harness";
}
#endif
