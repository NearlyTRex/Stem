/*
  Copyright (c) 2018 Alex Diener
  
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

#ifndef __TextureAtlas_H__
#define __TextureAtlas_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct TextureAtlas TextureAtlas;

#include "dynamictypes/HashTable.h"
#include "gamemath/Vector2f.h"
#include "glgraphics/VertexTypes.h"
#include "stemobject/StemObject.h"
#include <math.h>

struct TextureAtlas_entry {
	float left;
	float right;
	float bottom;
	float top;
};

#define TextureAtlas_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	GLuint textureID; \
	HashTable * private_ivar(hashTable); \
	unsigned int textureWidth; \
	unsigned int textureHeight;

stemobject_struct_definition(TextureAtlas)

TextureAtlas * TextureAtlas_create();
bool TextureAtlas_init(TextureAtlas * self);
void TextureAtlas_dispose(TextureAtlas * self);

void TextureAtlas_setTexture(TextureAtlas * self, bool magnifyNearest, unsigned int width, unsigned int height, void * bitmapData);
// Caller is responsible for freeing returned pointer (but not elements within it)
const char ** TextureAtlas_getKeys(TextureAtlas * self, size_t * outCount);
bool TextureAtlas_hasKey(TextureAtlas * self, const char * key);
void TextureAtlas_setEntry(TextureAtlas * self, const char * key, struct TextureAtlas_entry entry);
void TextureAtlas_removeEntry(TextureAtlas * self, const char * key);
struct TextureAtlas_entry TextureAtlas_lookup(TextureAtlas * self, const char * key);

#define TEXTUREATLAS_SIZE_AUTO INFINITY
// Returns the size of the specified entry, according to the following rules:
// - If width is TEXTUREATLAS_SIZE_AUTO, the returned size will have the specified height and an aspect-correct width calculated by the entry size.
// - If height is TEXTUREATLAS_SIZE_AUTO, the returned size will have the specified width and an aspect-correct height calculated by the entry size.
// - If both width and height are TEXTUREATLAS_SIZE_AUTO, the returned size is in pixels.
// - If neither width nor height is TEXTUREATLAS_SIZE_AUTO, the specified values are returned. This is not useful behavior, so specify TEXTUREATLAS_SIZE_AUTO for one or both of them when you call this function.
// If setTexture hasn't been called, behavior is undefined.
Vector2f TextureAtlas_getEntryDimensions(TextureAtlas * self, const char * key, float width, float height);

// Returns vertex data suitable for drawing a sprite with GL_TRIANGLES. Parameters:
// - key: Name of atlas entry to be drawn.
// - offset: Drawing origin. These values are added to all returned vertex positions.
// - relativeOrigin: Sprite's origin point in normalized coordinates. A value of {0, 0} will place the lower left vertex at the position specified by offset; {0.5, 0.5} will place the center at offset; {1, 1} will place the upper right corner at offset, and so on.
// - size: Dimensions of the sprite. TextureAtlas_getEntryDimensions() returns values appropriate for this parameter.
// - color: All returned vertex color values will be set to the specified value.
// - outVertices: If not NULL, vertex data will be written to this pointer.
// - outIndexes: If not NULL, vertex index data will be written to this pointer.
// - ioVertexCount: Returns the number of vertices written to outVertices, or if outVertices is NULL, the amount of space required to write to it. If this value is nonzero when called, the number of vertices written is added to it. Also used as the base value for any indexes written to outIndexes.
// - ioIndexCount: Returns the number of indexes written to outIndexes, or if outIndexes is NULL, the amount of space required to write to it. If this value is nonzero when called, the number of indexes written is added to it.
// 
// Typical usage: Call this function once with outVertices and outIndexes set to NULL in order to determine how much space
// is needed, by using ioVertexCount and ioIndexCount. Allocate the necessary space with glBufferData, use glMapBuffer to
// get a pointer to it, and call this function again to write the data to it.
void TextureAtlas_getVertices(TextureAtlas * self,
                              const char * key,
                              Vector2f offset,
                              Vector2f relativeOrigin,
                              Vector2f size,
                              Color4f color,
                              struct vertex_p2f_t2f_c4f * outVertices,
                              GLuint * outIndexes,
                              unsigned int * ioVertexCount,
                              unsigned int * ioIndexCount);

#ifdef __cplusplus
}
#endif
#endif
