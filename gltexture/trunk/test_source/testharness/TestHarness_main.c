#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include "shell/Target.h"
#include "eaglshell/EAGLShell.h"
#include "eaglshell/EAGLTarget.h"
#include "3dmath/Matrix.h"
#include "glutshell/GLUTTarget.h"
#include "glgraphics/GLIncludes.h"
#include "gltexture/GLTexture.h"
#include "pngimageio/PNGImageIO.h"
#include "utilities/IOUtilities.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_TEXTURES 5
#define NUM_MIN_FILTERS 6
#define NUM_MAG_FILTERS 2
#define NUM_WRAP_MODES 2
#define NUM_AUTO_BLEND_MODES 4

unsigned char subImageAlpha[] = {
	0xFF, 0x00,
	0x00, 0xFF
};
unsigned char subImageLuminance[] = {
	0x00, 0x7F,
	0x7F, 0xFF
};
unsigned char subImageLuminanceAlpha[] = {
	0xFF,0xFF, 0x00,0xFF,
	0x00,0xFF, 0xFF,0x7F
};
unsigned char subImageRGB[] = {
	0xFF,0x00,0xFF, 0x00,0x00,0x00,
	0xFF,0xFF,0xFF, 0x7F,0x3F,0x00
};
unsigned char subImageRGBA[] = {
	0xFF,0x00,0xFF,0xFF, 0x00,0x00,0x00,0x7F,
	0xFF,0xFF,0xFF,0x7F, 0x7F,0x3F,0x00,0xFF
};

struct {
	char * fileName;
	GLenum dataFormat;
	float ratio;
	unsigned char * subImage2x2;
	unsigned int subImageRowBytes;
} textureImages[NUM_TEXTURES] = {
	{"reference4x2gray.png", GL_ALPHA, 2.0f, subImageAlpha, 2},
	{"basn0g01.png", GL_LUMINANCE, 1.0f, subImageLuminance, 2},
	{"basn4a16.png", GL_LUMINANCE_ALPHA, 1.0f, subImageLuminanceAlpha, 4},
	{"basn2c16.png", GL_RGB, 1.0f, subImageRGB, 6},
	{"basn6a16.png", GL_RGBA, 1.0f, subImageRGBA, 8}
};
GLenum minFilters[NUM_MIN_FILTERS] = {
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR
};
GLenum magFilters[NUM_MAG_FILTERS] = {
	GL_NEAREST,
	GL_LINEAR
};
GLenum wrapModes[NUM_WRAP_MODES] = {
	GL_CLAMP_TO_EDGE,
	GL_REPEAT
};
enum GLTextureAutoBlendMode autoBlendModes[NUM_AUTO_BLEND_MODES] = {
	AUTO_BLEND_MODE_NONE,
	AUTO_BLEND_MODE_OPAQUE,
	AUTO_BLEND_MODE_TRANSPARENT_NONPREMULTIPLIED,
	AUTO_BLEND_MODE_TRANSPARENT_PREMULTIPLIED
};

static unsigned int viewportWidth = 800, viewportHeight = 600;
static GLTexture * texture = NULL;
static int textureIndex = 0;
static int minFilterIndex = 0;
static int magFilterIndex = 0;
static int wrapSModeIndex = 0;
static int wrapTModeIndex = 0;
static int autoBlendModeIndex = 0;
static bool autoMipmap = false;
static bool anisotropicFilter = false;
static bool whiteBackground = false;
static bool zoomedOut = false;
static bool extendedTexCoords = false;
static bool iPhoneMode = false;

struct vertex_p3f_t2f {
	float position[3];
	float texCoords[2];
};

struct vertex_p2f {
	float position[2];
};

static void loadTextureImage() {
	BitmapImage * image;
	
	if (texture != NULL) {
		texture->dispose(texture);
	}
	image = PNGImageIO_loadPNGFile(resourcePath(textureImages[textureIndex].fileName), PNG_PIXEL_FORMAT_AUTOMATIC, true);
	texture = GLTexture_create(textureImages[textureIndex].dataFormat,
	                           GL_UNSIGNED_BYTE, // TODO: Test data formats other than GL_UNSIGNED_BYTE somehow
	                           minFilters[minFilterIndex],
	                           magFilters[magFilterIndex],
	                           wrapModes[wrapSModeIndex],
	                           wrapModes[wrapTModeIndex],
	                           autoBlendModes[autoBlendModeIndex],
	                           autoMipmap,
	                           anisotropicFilter);
	texture->setImage(texture, 0, image->width, image->height, image->bytesPerRow, image->pixels);
	image->dispose(image);
}

void Target_init() {
	loadTextureImage();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	
	Shell_mainLoop();
}

static void drawControl(float x, float y, float scale, int parameter) {
	struct vertex_p2f vertices[] = {
		{{0.5f, 0.0f}},
		{{1.0f, 0.5f}},
		{{0.5f, 1.0f}},
		{{0.0f, 0.5f}}
	};
	Matrix matrix;
	
	matrix = Matrix_scaled(Matrix_translated(Matrix_identity(), x, y, 0.0f), scale, scale, 1.0f);
	glLoadMatrixf(matrix.m);
	switch (parameter) {
		case 0:
			glColor4ub(0xFF, 0x00, 0x00, 0xFF);
			break;
		case 1:
			glColor4ub(0x00, 0xFF, 0x00, 0xFF);
			break;
		case 2:
			glColor4ub(0x00, 0x00, 0xFF, 0xFF);
			break;
		case 3:
			glColor4ub(0xFF, 0xFF, 0x00, 0xFF);
			break;
		case 4:
			glColor4ub(0xFF, 0x00, 0xFF, 0xFF);
			break;
		case 5:
			glColor4ub(0x00, 0xFF, 0xff, 0xFF);
			break;
	}
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_p2f), vertices[0].position);
	glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(vertices) / sizeof(struct vertex_p2f));
}

void Target_draw() {
	float ratio = textureImages[textureIndex].ratio;
	float minTexCoordX = (ratio > 1.0f ? -0.5f : -0.5f / ratio) * (extendedTexCoords ? 2.0f : 1.0f) + 0.5f;
	float maxTexCoordX = (ratio > 1.0f ?  0.5f :  0.5f / ratio) * (extendedTexCoords ? 2.0f : 1.0f) + 0.5f;
	float minTexCoordY = (ratio < 1.0f ? -0.5f : -0.5f * ratio) * (extendedTexCoords ? 2.0f : 1.0f) + 0.5f;
	float maxTexCoordY = (ratio < 1.0f ?  0.5f :  0.5f * ratio) * (extendedTexCoords ? 2.0f : 1.0f) + 0.5f;
	struct vertex_p3f_t2f vertices[] = {
		{{-0.5f, -0.5f, 0.0f}, {minTexCoordX, minTexCoordY}},
		{{ 0.5f, -0.5f, 0.0f}, {maxTexCoordX, minTexCoordY}},
		{{ 0.5f,  0.5f, 0.0f}, {maxTexCoordX, maxTexCoordY}},
		{{ 0.5f,  0.5f, 0.0f}, {maxTexCoordX, maxTexCoordY}},
		{{-0.5f,  0.5f, 0.0f}, {minTexCoordX, maxTexCoordY}},
		{{-0.5f, -0.5f, 0.0f}, {minTexCoordX, minTexCoordY}},
		
		{{-0.5f, -0.75f, -0.5f}, {minTexCoordX, minTexCoordY}},
		{{ 0.5f, -0.75f, -0.5f}, {maxTexCoordX, minTexCoordY}},
		{{ 0.5f, -0.75f,  0.5f}, {maxTexCoordX, maxTexCoordY}},
		{{ 0.5f, -0.75f,  0.5f}, {maxTexCoordX, maxTexCoordY}},
		{{-0.5f, -0.75f,  0.5f}, {minTexCoordX, maxTexCoordY}},
		{{-0.5f, -0.75f, -0.5f}, {minTexCoordX, minTexCoordY}},
		
		{{-0.5f, 0.75f, -0.5f}, {minTexCoordX, minTexCoordY}},
		{{ 0.5f, 0.75f, -0.5f}, {maxTexCoordX, minTexCoordY}},
		{{ 0.5f, 0.75f,  0.5f}, {maxTexCoordX, maxTexCoordY}},
		{{ 0.5f, 0.75f,  0.5f}, {maxTexCoordX, maxTexCoordY}},
		{{-0.5f, 0.75f,  0.5f}, {minTexCoordX, maxTexCoordY}},
		{{-0.5f, 0.75f, -0.5f}, {minTexCoordX, minTexCoordY}}
	};
	Matrix matrix;
	
	if (whiteBackground) {
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	} else {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	matrix = Matrix_perspective(Matrix_identity(), 60.0f, (float) viewportWidth / viewportHeight, 0.25f, 100.0f);
	glMultMatrixf(matrix.m);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, zoomedOut ? -5.0f : -2.0f);
	
	texture->activate(texture);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex_p3f_t2f), vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_p3f_t2f), vertices[0].texCoords);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(struct vertex_p3f_t2f));
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	texture->deactivate(texture);
	
	if (iPhoneMode) {
		float viewRatio = (float) viewportWidth / viewportHeight;
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		matrix = Matrix_ortho(Matrix_identity(), 0.0f, viewRatio, 0.0f, 1.0f, -1.0f, 1.0f);
		glMultMatrixf(matrix.m);
		glMatrixMode(GL_MODELVIEW);
		
		drawControl(0.0f, 0.0f / 6.0f, 1.0f / 6.0f, textureIndex);
		drawControl(0.0f, 1.0f / 6.0f, 1.0f / 6.0f, autoBlendModeIndex);
		drawControl(0.0f, 2.0f / 6.0f, 1.0f / 6.0f, minFilterIndex);
		drawControl(0.0f, 3.0f / 6.0f, 1.0f / 6.0f, magFilterIndex);
		drawControl(0.0f, 4.0f / 6.0f, 1.0f / 6.0f, wrapSModeIndex);
		drawControl(0.0f, 5.0f / 6.0f, 1.0f / 6.0f, wrapTModeIndex);
		drawControl(viewRatio - 1.0f / 6.0f, 0.0f / 6.0f, 1.0f / 6.0f, autoMipmap);
		drawControl(viewRatio - 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f, anisotropicFilter);
		drawControl(viewRatio - 1.0f / 6.0f, 2.0f / 6.0f, 1.0f / 6.0f, extendedTexCoords);
		drawControl(viewRatio - 1.0f / 6.0f, 3.0f / 6.0f, 1.0f / 6.0f, whiteBackground);
		drawControl(viewRatio - 1.0f / 6.0f, 4.0f / 6.0f, 1.0f / 6.0f, zoomedOut);
		drawControl(viewRatio - 1.0f / 6.0f, 5.0f / 6.0f, 1.0f / 6.0f, 0);
	}
}

void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	viewportWidth = newWidth;
	viewportHeight = newHeight;
}

static void cycleTextures(int direction) {
	textureIndex += direction;
	if (textureIndex < 0) {
		textureIndex += NUM_TEXTURES;
	} else if (textureIndex >= NUM_TEXTURES) {
		textureIndex -= NUM_TEXTURES;
	}
	loadTextureImage();
	Shell_redisplay();
}

static void cycleAutoBlendModes(int direction) {
	autoBlendModeIndex += direction;
	if (autoBlendModeIndex < 0) {
		autoBlendModeIndex += NUM_AUTO_BLEND_MODES;
	} else if (autoBlendModeIndex >= NUM_AUTO_BLEND_MODES) {
		autoBlendModeIndex -= NUM_AUTO_BLEND_MODES;
	}
	texture->autoBlendMode = autoBlendModes[autoBlendModeIndex];
	Shell_redisplay();
}

static void cycleMinFilters(int direction) {
	minFilterIndex += direction;
	if (minFilterIndex < 0) {
		minFilterIndex += NUM_MIN_FILTERS;
	} else if (minFilterIndex >= NUM_MIN_FILTERS) {
		minFilterIndex -= NUM_MIN_FILTERS;
	}
	texture->minFilter = minFilters[minFilterIndex];
	texture->updateTexParams(texture);
	Shell_redisplay();
}

static void cycleMagFilters(int direction) {
	magFilterIndex += direction;
	if (magFilterIndex < 0) {
		magFilterIndex += NUM_MAG_FILTERS;
	} else if (magFilterIndex >= NUM_MAG_FILTERS) {
		magFilterIndex -= NUM_MAG_FILTERS;
	}
	texture->magFilter = magFilters[magFilterIndex];
	texture->updateTexParams(texture);
	Shell_redisplay();
}

static void cycleWrapS(int direction) {
	wrapSModeIndex += direction;
	if (wrapSModeIndex < 0) {
		wrapSModeIndex += NUM_WRAP_MODES;
	} else if (wrapSModeIndex >= NUM_WRAP_MODES) {
		wrapSModeIndex -= NUM_WRAP_MODES;
	}
	texture->wrapS = wrapModes[wrapSModeIndex];
	texture->updateTexParams(texture);
	Shell_redisplay();
}

static void cycleWrapT(int direction) {
	wrapTModeIndex += direction;
	if (wrapTModeIndex < 0) {
		wrapTModeIndex += NUM_WRAP_MODES;
	} else if (wrapTModeIndex >= NUM_WRAP_MODES) {
		wrapTModeIndex -= NUM_WRAP_MODES;
	}
	texture->wrapT = wrapModes[wrapTModeIndex];
	texture->updateTexParams(texture);
	Shell_redisplay();
}

static void toggleAutoMipmap() {
	autoMipmap = !autoMipmap;
	loadTextureImage();
	Shell_redisplay();
}

static void toggleAnisotropy() {
	anisotropicFilter = !anisotropicFilter;
	texture->anisotropicFilter = anisotropicFilter;
	texture->updateTexParams(texture);
	Shell_redisplay();
}

static void toggleExtendedTexCoords() {
	extendedTexCoords = !extendedTexCoords;
	Shell_redisplay();
}

static void toggleBackgroundColor() {
	whiteBackground = !whiteBackground;
	Shell_redisplay();
}

static void toggleZoomLevel() {
	zoomedOut = !zoomedOut;
	Shell_redisplay();
}

static void updateTextureImage() {
	texture->updateImage(texture, 0, 0, 0, 2, 2, textureImages[textureIndex].subImageRowBytes, textureImages[textureIndex].subImage2x2);
	Shell_redisplay();
}

void Target_keyDown(unsigned int charCode, unsigned int keyCode) {
	if (charCode == '[') {
		cycleTextures(-1);
		
	} else if (charCode == ']') {
		cycleTextures(1);
		
	} else if (charCode == '{') {
		cycleAutoBlendModes(-1);
		
	} else if (charCode == '}') {
		cycleAutoBlendModes(1);
		
	} else if (charCode == ',') {
		cycleMinFilters(-1);
		
	} else if (charCode == '.') {
		cycleMinFilters(1);
		
	} else if (charCode == '<') {
		cycleMagFilters(-1);
		
	} else if (charCode == '>') {
		cycleMagFilters(1);
		
	} else if (keyCode == KEYBOARD_LEFT_ARROW) {
		cycleWrapS(-1);
		
	} else if (keyCode == KEYBOARD_RIGHT_ARROW) {
		cycleWrapS(1);
		
	} else if (keyCode == KEYBOARD_DOWN_ARROW) {
		cycleWrapT(-1);
		
	} else if (keyCode == KEYBOARD_UP_ARROW) {
		cycleWrapT(1);
		
	} else if (keyCode == KEYBOARD_M) {
		toggleAutoMipmap();
		
	} else if (keyCode == KEYBOARD_A) {
		toggleAnisotropy();
		
	} else if (keyCode == KEYBOARD_E) {
		toggleExtendedTexCoords();
		
	} else if (keyCode == KEYBOARD_B) {
		toggleBackgroundColor();
		
	} else if (keyCode == KEYBOARD_Z) {
		toggleZoomLevel();
		
	} else if (keyCode == KEYBOARD_S) {
		updateTextureImage();
	}
}

void Target_keyUp(unsigned int charCode, unsigned int keyCode) {
}

void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	if (iPhoneMode) {
		if (x < viewportHeight / 6) {
			if (y > viewportHeight - viewportHeight / 6 * 1) {
				cycleTextures(1);
				
			} else if (y > viewportHeight - viewportHeight / 6 * 2) {
				cycleAutoBlendModes(1);
				
			} else if (y > viewportHeight - viewportHeight / 6 * 3) {
				cycleMinFilters(1);
				
			} else if (y > viewportHeight - viewportHeight / 6 * 4) {
				cycleMagFilters(1);
				
			} else if (y > viewportHeight - viewportHeight / 6 * 5) {
				cycleWrapS(1);
				
			} else {
				cycleWrapT(1);
			}
			
		} else if (x > viewportWidth - viewportHeight / 6) {
			if (y > viewportHeight - viewportHeight / 6 * 1) {
				toggleAutoMipmap();
				
			} else if (y > viewportHeight - viewportHeight / 6 * 2) {
				toggleAnisotropy();
				
			} else if (y > viewportHeight - viewportHeight / 6 * 3) {
				toggleExtendedTexCoords();
				
			} else if (y > viewportHeight - viewportHeight / 6 * 4) {
				toggleBackgroundColor();
				
			} else if (y > viewportHeight - viewportHeight / 6 * 5) {
				toggleZoomLevel();
				
			} else {
				updateTextureImage();
			}
		}
	}
}

void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
}

void Target_mouseMoved(float x, float y) {
}

void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
}

void GLUTTarget_configure(int argc, char ** argv, struct GLUTShellConfiguration * configuration) {
	configuration->windowTitle = "GLTexture Test Harness";
}

void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
	iPhoneMode = true;
	
	configuration->preferredOpenGLAPIVersion = EAGLShellOpenGLVersion_ES1 /*| EAGLShellOpenGLVersion_ES2*/;
}

void EAGLTarget_openURL(const char * url) {
}

void EAGLTarget_touchesCancelled(unsigned int buttonMask) {
}

void EAGLTarget_accelerometer(double x, double y, double z) {
}
