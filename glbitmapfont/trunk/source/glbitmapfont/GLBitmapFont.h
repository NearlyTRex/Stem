#ifndef __GL_BITMAP_FONT_H__
#define __GL_BITMAP_FONT_H__

#include "glfont/GLFont.h"
#include "gltexture/GLTexture.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"

typedef struct GLBitmapFont GLBitmapFont;

// TODO: Remove in favor of arbitrary set of characters per font
#define GLBITMAPFONT_PRINTABLE_MIN ' '
#define GLBITMAPFONT_PRINTABLE_MAX '~'
#define GLBITMAPFONT_NUM_CHARS 95

struct GLBitmapFont_charEntry {
	// Number of total ems advanced after drawing this glyph
	float advance;
	// Number of ems to the right of the current position to start drawing glyph
	float glyphOffset;
	// Glyph's drawn width in ems
	float glyphWidth;
	// Glyph's texture coordinates
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
	bool textureNameOwned; \
	GLTexture * texture; \
	bool textureOwned; \
	struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS]; \
	\
	void (* setTexture)(void * self, GLTexture * texture, bool takeOwnership);

struct GLBitmapFont {
	GLBitmapFont_structContents
};

GLBitmapFont * GLBitmapFont_create(struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS]);
void GLBitmapFont_init(GLBitmapFont * self, struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS]);
void GLBitmapFont_dispose(void * selfPtr);

GLBitmapFont * GLBitmapFont_deserialize(DeserializationContext * context);
bool GLBitmapFont_loadSerializedData(GLBitmapFont * self, DeserializationContext * context);
void GLBitmapFont_serialize(GLBitmapFont * self, SerializationContext * context);

void GLBitmapFont_setTexture(void * selfPtr, GLTexture * texture, bool takeOwnership);

float GLBitmapFont_measureString(void * selfPtr, const char * string, size_t length);
size_t GLBitmapFont_indexAtWidth(void * selfPtr, const char * string, size_t length, float emWidth, bool * outLeadingEdge);
// string must not be modified while GLBitmapFont_drawString is executing
// Currently cannot correctly draw strings longer than 16383 characters
void GLBitmapFont_drawString(void * selfPtr, const char * string, size_t length, float emHeight, float offsetX, float offsetY, float offsetZ);

#endif
