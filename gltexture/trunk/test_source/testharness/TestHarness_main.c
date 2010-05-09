#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include "shell/Target.h"
#include "eaglshell/EAGLShell.h"
#include "eaglshell/EAGLTarget.h"
#include "3dmath/Matrix.h"
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
	{"reference3x2gray.png", GL_ALPHA, 3.0f / 2.0f, subImageAlpha, 2},
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
GLTexture * texture = NULL;
int textureIndex = 0;
int minFilterIndex = 0;
int magFilterIndex = 0;
int wrapSModeIndex = 0;
int wrapTModeIndex = 0;
int autoBlendModeIndex = 0;
bool autoMipmap = false;
bool anisotropicFilter = false;
bool whiteBackground = false;
bool zoomedOut = false;
bool extendedTexCoords = false;

struct vertex {
	float position[3];
	float texCoords[2];
};

const char * Target_getName() {
	return "GLTexture Test Harness";
}

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

void Target_init(int argc, char ** argv) {
	loadTextureImage();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	Shell_mainLoop();
}

void Target_draw() {
	float ratio = textureImages[textureIndex].ratio;
	float minTexCoordX = (ratio > 1.0f ? -0.5f : -0.5f / ratio) * (extendedTexCoords ? 2.0f : 1.0f) + 0.5f;
	float maxTexCoordX = (ratio > 1.0f ?  0.5f :  0.5f / ratio) * (extendedTexCoords ? 2.0f : 1.0f) + 0.5f;
	float minTexCoordY = (ratio < 1.0f ? -0.5f : -0.5f * ratio) * (extendedTexCoords ? 2.0f : 1.0f) + 0.5f;
	float maxTexCoordY = (ratio < 1.0f ?  0.5f :  0.5f * ratio) * (extendedTexCoords ? 2.0f : 1.0f) + 0.5f;
	struct vertex vertices[] = {
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
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex), vertices[0].texCoords);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(struct vertex));
	texture->deactivate(texture);
}

void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	viewportWidth = newWidth;
	viewportHeight = newHeight;
}

void Target_keyDown(unsigned int charCode, unsigned int keyCode) {
	if (charCode == '[') {
		textureIndex--;
		if (textureIndex < 0) {
			textureIndex += NUM_TEXTURES;
		}
		loadTextureImage();
		Shell_redisplay();
		
	} else if (charCode == ']') {
		textureIndex++;
		if (textureIndex >= NUM_TEXTURES) {
			textureIndex -= NUM_TEXTURES;
		}
		loadTextureImage();
		Shell_redisplay();
		
	} else if (charCode == '{') {
		autoBlendModeIndex--;
		if (autoBlendModeIndex < 0) {
			autoBlendModeIndex += NUM_AUTO_BLEND_MODES;
		}
		texture->autoBlendMode = autoBlendModes[autoBlendModeIndex];
		Shell_redisplay();
		
	} else if (charCode == '}') {
		autoBlendModeIndex++;
		if (autoBlendModeIndex >= NUM_AUTO_BLEND_MODES) {
			autoBlendModeIndex -= NUM_AUTO_BLEND_MODES;
		}
		texture->autoBlendMode = autoBlendModes[autoBlendModeIndex];
		Shell_redisplay();
		
	} else if (charCode == ',') {
		minFilterIndex--;
		if (minFilterIndex < 0) {
			minFilterIndex += NUM_MIN_FILTERS;
		}
		texture->minFilter = minFilters[minFilterIndex];
		texture->updateTexParams(texture);
		Shell_redisplay();
		
	} else if (charCode == '.') {
		minFilterIndex++;
		if (minFilterIndex >= NUM_MIN_FILTERS) {
			minFilterIndex -= NUM_MIN_FILTERS;
		}
		texture->minFilter = minFilters[minFilterIndex];
		texture->updateTexParams(texture);
		Shell_redisplay();
		
	} else if (charCode == '<') {
		magFilterIndex--;
		if (magFilterIndex < 0) {
			magFilterIndex += NUM_MAG_FILTERS;
		}
		texture->magFilter = magFilters[magFilterIndex];
		texture->updateTexParams(texture);
		Shell_redisplay();
		
	} else if (charCode == '>') {
		magFilterIndex++;
		if (magFilterIndex >= NUM_MAG_FILTERS) {
			magFilterIndex -= NUM_MAG_FILTERS;
		}
		texture->magFilter = magFilters[magFilterIndex];
		texture->updateTexParams(texture);
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_LEFT_ARROW) {
		wrapSModeIndex--;
		if (wrapSModeIndex < 0) {
			wrapSModeIndex += NUM_WRAP_MODES;
		}
		texture->wrapS = wrapModes[wrapSModeIndex];
		texture->updateTexParams(texture);
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_RIGHT_ARROW) {
		wrapSModeIndex++;
		if (wrapSModeIndex >= NUM_WRAP_MODES) {
			wrapSModeIndex -= NUM_WRAP_MODES;
		}
		texture->wrapS = wrapModes[wrapSModeIndex];
		texture->updateTexParams(texture);
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_DOWN_ARROW) {
		wrapTModeIndex--;
		if (wrapTModeIndex < 0) {
			wrapTModeIndex += NUM_WRAP_MODES;
		}
		texture->wrapT = wrapModes[wrapTModeIndex];
		texture->updateTexParams(texture);
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_UP_ARROW) {
		wrapTModeIndex++;
		if (wrapTModeIndex >= NUM_WRAP_MODES) {
			wrapTModeIndex -= NUM_WRAP_MODES;
		}
		texture->wrapT = wrapModes[wrapTModeIndex];
		texture->updateTexParams(texture);
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_M) {
		autoMipmap = !autoMipmap;
		loadTextureImage();
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_A) {
		anisotropicFilter = !anisotropicFilter;
		texture->anisotropicFilter = anisotropicFilter;
		texture->updateTexParams(texture);
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_E) {
		extendedTexCoords = !extendedTexCoords;
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_B) {
		whiteBackground = !whiteBackground;
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_Z) {
		zoomedOut = !zoomedOut;
		Shell_redisplay();
		
	} else if (keyCode == KEYBOARD_S) {
		texture->updateImage(texture, 0, 0, 0, 2, 2, textureImages[textureIndex].subImageRowBytes, textureImages[textureIndex].subImage2x2);
		Shell_redisplay();
	}
}

void Target_keyUp(unsigned int charCode, unsigned int keyCode) {
}

void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	/*
	if (y < viewportHeight / 4) {
		whiteBackground = !whiteBackground;
		Shell_redisplay();
		
	} else if (y > viewportHeight - viewportHeight / 4) {
		zoomedOut = !zoomedOut;
		Shell_redisplay();
		
	} else if (x < viewportWidth / 4) {
		textureIndex--;
		if (textureIndex < 0) {
			textureIndex += NUM_TEXTURES;
		}
		Shell_redisplay();
		
	} else if (x > viewportWidth - viewportWidth / 4) {
		textureIndex++;
		if (textureIndex >= NUM_TEXTURES) {
			textureIndex -= NUM_TEXTURES;
		}
		Shell_redisplay();
	}
	*/
}

void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
}

void Target_mouseMoved(float x, float y) {
}

void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
}

enum EAGLShellOpenGLVersion EAGLTarget_getPreferredOpenGLAPIVersion() {
	return EAGLShellOpenGLVersion_ES1 /*| EAGLShellOpenGLVersion_ES2*/;
}

void EAGLTarget_touchesCancelled(unsigned int buttonMask) {
}

void EAGLTarget_accelerometer(double x, double y, double z) {
}
