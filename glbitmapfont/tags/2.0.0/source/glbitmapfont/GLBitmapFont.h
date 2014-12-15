/*
  Copyright (c) 2014 Alex Diener
  
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

#ifndef __GLBitmapFont_H__
#define __GLBitmapFont_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gamemath/Vector2f.h"
#include "glgraphics/VertexTypes.h"
#include "gltexture/GLTextureAtlas.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include "stemobject/StemObject.h"
#include <stdbool.h>

typedef struct GLBitmapFont GLBitmapFont;

// TODO: Remove in favor of arbitrary set of characters per font
#define GLBITMAPFONT_PRINTABLE_MIN ' '
#define GLBITMAPFONT_PRINTABLE_MAX '~'
#define GLBITMAPFONT_NUM_CHARS 95

#define GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION 2

#define GLBITMAPFONT_USE_STRLEN ((size_t) -1)

struct GLBitmapFont_charEntry {
	// Number of total ems advanced after drawing this glyph
	float advance;
	// Number of ems to the right of the current position to start drawing glyph
	float glyphOffset;
	// Glyph's drawn width in ems
	float glyphWidth;
	// Glyph's texture coordinates
	const char * atlasKey;
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

#define GLBitmapFont_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	char * atlasName; \
	bool private_ivar(atlasNameOwned); \
	GLTextureAtlas * atlas; \
	bool private_ivar(atlasOwned); \
	struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS];

stemobject_struct_definition(GLBitmapFont)

GLBitmapFont * GLBitmapFont_create(struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS]);
bool GLBitmapFont_init(GLBitmapFont * self, struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS]);
void GLBitmapFont_dispose(GLBitmapFont * self);

GLBitmapFont * GLBitmapFont_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool GLBitmapFont_loadSerializedData(GLBitmapFont * self, compat_type(DeserializationContext *) deserializationContext);
void GLBitmapFont_serialize(GLBitmapFont * self, compat_type(SerializationContext *) serializationContext);

void GLBitmapFont_setTextureAtlas(GLBitmapFont * self, GLTextureAtlas * atlas, bool takeOwnership);

// Returns the width of string (of length UTF-8 bytes) in ems
float GLBitmapFont_measureString(GLBitmapFont * self, const char * string, size_t length);

// Returns the closest character index to emWidth, as measured from the beginning of string (of length UTF-8 bytes).
// If outLeadingEdge is non-NULL, it will be set to true if emWidth falls on the leading half of the
// character whose index is returned, or false if it does not.
size_t GLBitmapFont_indexAtWidth(GLBitmapFont * self, const char * string, size_t length, float emWidth, bool * outLeadingEdge);

// Returns vertex data suitable for drawing a sprite with GL_TRIANGLES. Parameters:
// - string: Text to be drawn.
// - length: Length of string. You can pass GLBITMAPFONT_USE_STRLEN if string is null-terminated.
// - emHeight: Desired text height. Text is scaled on both axes relative to this value.
// - offset: Drawing origin. These values are added to all returned vertex positions.
// - relativeOrigin: Sprite's origin point in normalized coordinates. A value of {0, 0} will place the lower left vertex at the position specified by offset; {0.5, 0.5} will place the center at offset; {1, 1} will place the upper right corner at offset, and so on.
// - indexType: Data type of outIndexes. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
// - baseIndex: Numeric offset to be applied to all returned indexes. If multiple objects are being drawn with the same vertex/index array, pass the number of vertices that have been written prior to calling this function.
// - outVertices: If not NULL, vertex data will be written to this pointer.
// - outIndexes: If not NULL, vertex index data of the type specified by indexType will be written to this pointer.
// - ioVertexCount: Returns the number of vertices written to outVertices, or if outVertices is NULL, the amount of space required to write to it. If this value is nonzero when called, the number of vertices written is added to it.
// - ioIndexCount: Returns the number of indexes written to outIndexes, or if outIndexes is NULL, the amount of space required to write to it. If this value is nonzero when called, the number of indexes written is added to it.
// 
// Typical usage: Call this function once with outVertices and outIndexes set to NULL in order to determine how much space
// is needed, by using ioVertexCount and ioIndexCount. Allocate the necessary space with glBufferData, use glMapBuffer to
// get a pointer to it, and call this function again to write the data to it.
// 
// setTextureAtlas must be called prior to calling getStringVertices or getStringVerticesWithColor.
void GLBitmapFont_getStringVertices(GLBitmapFont * self,
                                    const char * string,
                                    size_t length,
                                    float emHeight,
                                    Vector2f offset,
                                    Vector2f relativeOrigin,
                                    GLenum indexType,
                                    unsigned int baseIndex,
                                    struct vertex_p2f_t2f * outVertices,
                                    void * outIndexes,
                                    unsigned int * ioVertexCount,
                                    unsigned int * ioIndexCount);

// Same as above, but uses vertex_p2f_t2f_c4f instead of vertex_p2f_t2f. All vertex colors will be set to
// the values specified by the color parameter.
void GLBitmapFont_getStringVerticesWithColor(GLBitmapFont * self,
                                             const char * string,
                                             size_t length,
                                             float emHeight,
                                             Vector2f offset,
                                             Vector2f relativeOrigin,
                                             Color4f color,
                                             GLenum indexType,
                                             unsigned int baseIndex,
                                             struct vertex_p2f_t2f_c4f * outVertices,
                                             void * outIndexes,
                                             unsigned int * ioVertexCount,
                                             unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
