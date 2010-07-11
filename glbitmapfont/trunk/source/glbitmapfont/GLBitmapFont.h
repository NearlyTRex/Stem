#ifndef __GL_BITMAP_FONT_H__
#define __GL_BITMAP_FONT_H__

#include "glfont/GLFont.h"
#include "gltexture/GLTexture.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"

typedef struct GLBitmapFont GLBitmapFont;

struct GLBitmapFont_charEntry {
	// Number of pixels offset from current position to start drawing glyph
	float preadvance;
	// Number of total pixels advanced after drawing this glyph
	float advance;
	// Texture coordinates, in pixels
	float textureLeft;
	float textureRight;
	float textureBottom;
	float textureTop;
	// Number of items in kernChars
	size_t kernCharCount;
	struct GLBitmapFont_kernEntry {
		// If this character is preceded by previous, offset is applied to both preadvance and advance.
		// previous must be unique within the kernChars list, and is only meaningful if it is one of the 95
		// printable characters represented by GLBitmapFont (' ' through '~').
		char previous;
		float offset;
	} * kernChars;
};

#define GLBitmapFont_structContents \
	GLFont_structContents \
	\
	char * textureName; \
	GLTexture * texture; \
	bool textureNameAllocated; \
	struct GLBitmapFont_charEntry characters[95];

struct GLBitmapFont {
	GLBitmapFont_structContents
};

GLBitmapFont * GLBitmapFont_create(struct GLBitmapFont_charEntry characters[95]);
void GLBitmapFont_init(GLBitmapFont * self, struct GLBitmapFont_charEntry characters[95]);
void GLBitmapFont_dispose(void * selfPtr);

GLBitmapFont * GLBitmapFont_deserialize(DeserializationContext * context);
bool GLBitmapFont_loadSerializedData(GLBitmapFont * self, DeserializationContext * context);
void GLBitmapFont_serialize(GLBitmapFont * self, SerializationContext * context);

float GLBitmapFont_measureString(void * selfPtr, char * string, size_t length);
size_t GLBitmapFont_indexAtWidth(void * selfPtr, char * string, size_t length, float emWidth, bool * outLeadingEdge);
void GLBitmapFont_drawString(void * selfPtr, char * string, size_t length, float emHeight, float offsetX, float offsetY, float offsetZ);

#endif
