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

#ifndef __GLTextureAtlas_H__
#define __GLTextureAtlas_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct GLTextureAtlas GLTextureAtlas;

#include "gamemath/Vector2f.h"
#include "glgraphics/VertexTypes.h"
#include "gltexture/GLTexture.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include "stemobject/StemObject.h"
#include "utilities/HashTable.h"
#include <math.h>

#define GLTEXTUREATLAS_SERIALIZATION_FORMAT_VERSION 1

struct GLTextureAtlas_entry {
	float left;
	float right;
	float bottom;
	float top;
};

#define GLTextureAtlas_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	char * textureName; \
	bool private_ivar(textureNameOwned); \
	GLTexture * texture; \
	bool private_ivar(textureOwned); \
	HashTable * private_ivar(hashTable);

stemobject_struct_definition(GLTextureAtlas)

GLTextureAtlas * GLTextureAtlas_create();
bool GLTextureAtlas_init(GLTextureAtlas * self);
void GLTextureAtlas_dispose(GLTextureAtlas * self);

GLTextureAtlas * GLTextureAtlas_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool GLTextureAtlas_loadSerializedData(GLTextureAtlas * self, compat_type(DeserializationContext *) deserializationContext);
void GLTextureAtlas_serialize(GLTextureAtlas * self, compat_type(SerializationContext *) serializationContext);

void GLTextureAtlas_setTexture(GLTextureAtlas * self, GLTexture * texture, bool takeOwnership);
// Caller is responsible for freeing returned pointer (but not elements within it)
const char ** GLTextureAtlas_getKeys(GLTextureAtlas * self, size_t * outCount);
bool GLTextureAtlas_hasKey(GLTextureAtlas * self, const char * key);
void GLTextureAtlas_setEntry(GLTextureAtlas * self, const char * key, struct GLTextureAtlas_entry entry);
void GLTextureAtlas_removeEntry(GLTextureAtlas * self, const char * key);
// lookup returns {0.0f, 0.0f, 0.0f, 0.0f} for nonexistent keys
struct GLTextureAtlas_entry GLTextureAtlas_lookup(GLTextureAtlas * self, const char * key);

#define GLTEXTUREATLAS_SIZE_AUTO INFINITY
// Returns the size of the specified entry, according to the following rules:
// - If width is GLTEXTUREATLAS_SIZE_AUTO, the returned size will have the specified height and an aspect-correct width calculated by the entry size.
// - If height is GLTEXTUREATLAS_SIZE_AUTO, the returned size will have the specified width and an aspect-correct height calculated by the entry size.
// - If both width and height are GLTEXTUREATLAS_SIZE_AUTO, the returned size is in pixels.
// - If neither width nor height is GLTEXTUREATLAS_SIZE_AUTO, the specified values are returned. This is not useful behavior, so specify GLTEXTUREATLAS_SIZE_AUTO for one or both of them when you call this function.
// If texture is NULL or the texture's image has not been set, behavior is undefined.
Vector2f GLTextureAtlas_getEntryDimensions(GLTextureAtlas * self, const char * key, float width, float height);

// Returns vertex data suitable for drawing a sprite with GL_TRIANGLES. Parameters:
// - key: Name of atlas entry to be drawn.
// - offset: Drawing origin. These values are added to all returned vertex positions.
// - relativeOrigin: Sprite's origin point in normalized coordinates. A value of {0, 0} will place the lower left vertex at the position specified by offset; {0.5, 0.5} will place the center at offset; {1, 1} will place the upper right corner at offset, and so on.
// - size: Dimensions of the sprite. GLTextureAtlas_getEntryDimensions() returns values appropriate for this parameter.
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
void GLTextureAtlas_getVertices(GLTextureAtlas * self,
                                const char * key,
                                Vector2f offset,
                                Vector2f relativeOrigin,
                                Vector2f size,
                                GLenum indexType,
                                unsigned int baseIndex,
                                struct vertex_p2f_t2f * outVertices,
                                void * outIndexes,
                                unsigned int * ioVertexCount,
                                unsigned int * ioIndexCount);

// Same as above, but uses vertex_p2f_t2f_c4f instead of vertex_p2f_t2f. All vertex colors will be set to
// the values specified by the color parameter.
void GLTextureAtlas_getVerticesWithColor(GLTextureAtlas * self,
                                         const char * key,
                                         Vector2f offset,
                                         Vector2f relativeOrigin,
                                         Vector2f size,
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
