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

// Returns the number of indexes to be drawn for the specified string, writing the appropriate indexes to outIndexes
// if a non-NULL pointer is provided. You may wish to call this function once with outIndexes set to NULL to
// determine how much space needs to be allocated, then again with a newly allocated buffer of the appropriate size.
// Use GL_TRIANGLES to draw the data returned from this function.
unsigned int GLBitmapFont_getStringIndexes(GLBitmapFont * self,
                                           const char * string,
                                           size_t length, // Passing GLBITMAPFONT_USE_STRLEN will measure with strlen()
                                           void * outIndexes,
                                           GLenum indexType, // GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT
                                           unsigned int indexOffset); // Value to be added to each index written to outIndexes, if specified. Useful if your vertex buffer contains multiple sets of data to be drawn.

// Returns the number of vertices to be drawn for the specified string, writing the appropriate data to outVertices
// if a non-NULL pointer is provided. You may wish to call this function once with outVertices set to NULL to
// determine how much space needs to be allocated, then again with a newly allocated buffer of the appropriate size.
// Use GL_TRIANGLES to draw the data returned from this function.
// setTextureAtlas must be called prior to calling getStringVertices.
unsigned int GLBitmapFont_getStringVertices(GLBitmapFont * self,
                                            const char * string,
                                            size_t length, // Passing GLBITMAPFONT_USE_STRLEN will measure with strlen()
                                            float emHeight, // Returned data is scaled on both axes relative to this value
                                            float offsetX, // Constant added to the x axis coordinate of all vertices
                                            float offsetY, // Constant added to the y axis coordinate of all vertices
                                            float relativeOriginX, // Specify 0 for left-aligned text, 0.5 for center-aligned text, or 1 for right-aligned text. Other values are accepted and will be interpolated or extrapolated as appropriate.
                                            float relativeOriginY, // Specify 0 for bottom-aligned text, 0.5 for middle-aligned text, or 1 for top-aligned text. Other values are accepted and will be interpolated or extrapolated as appropriate.
                                            struct vertex_p2f_t2f * outVertices);

// Returns the number of vertices to be drawn for the specified string, writing the appropriate data to outVertices
// if a non-NULL pointer is provided. You may wish to call this function once with outVertices set to NULL to
// determine how much space needs to be allocated, then again with a newly allocated buffer of the appropriate size.
// Use GL_TRIANGLES to draw the data returned from this function.
// setTextureAtlas must be called prior to calling getStringVerticesWithColor.
unsigned int GLBitmapFont_getStringVerticesWithColor(GLBitmapFont * self,
                                                     const char * string,
                                                     size_t length, // Passing GLBITMAPFONT_USE_STRLEN will measure with strlen()
                                                     float emHeight, // Returned data is scaled on both axes relative to this value
                                                     float offsetX, // Constant added to the x axis coordinate of all vertices
                                                     float offsetY, // Constant added to the y axis coordinate of all vertices
                                                     float relativeOriginX, // Specify 0 for left-aligned text, 0.5 for center-aligned text, or 1 for right-aligned text. Other values are accepted and will be interpolated or extrapolated as appropriate.
                                                     float relativeOriginY, // Specify 0 for bottom-aligned text, 0.5 for middle-aligned text, or 1 for top-aligned text. Other values are accepted and will be interpolated or extrapolated as appropriate.
                                                     float red,
                                                     float green,
                                                     float blue,
                                                     float alpha,
                                                     struct vertex_p2f_t2f_c4f * outVertices);

#ifdef __cplusplus
}
#endif
#endif
