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
	HashTable * private_ivar(hashTable); \
	\
	const char ** (* getKeys)(self_type * self, size_t * outCount); \
	bool (* hasKey)(self_type * self, const char * key); \
	void (* setEntry)(self_type * self, const char * key, struct GLTextureAtlas_entry entry); \
	void (* removeEntry)(self_type * self, const char * key); \
	struct GLTextureAtlas_entry (* lookup)(self_type * self, const char * key); \
	unsigned int (* getVertices)(self_type * self, const char * key, float offsetX, float offsetY, float relativeOriginX, float relativeOriginY, float width, float height, struct vertex_p2f_t2f * outVertices); \
	unsigned int (* getVerticesWithColor)(self_type * self, const char * key, float offsetX, float offsetY, float relativeOriginX, float relativeOriginY, float width, float height, float red, float green, float blue, float alpha, struct vertex_p2f_t2f_c4f * outVertices); \
	unsigned int (* getIndexes)(self_type * self, void * outIndexes, GLenum indexType, unsigned int indexOffset);

stemobject_struct_definition(GLTextureAtlas)

GLTextureAtlas * GLTextureAtlas_create();
bool GLTextureAtlas_init(GLTextureAtlas * self);
void GLTextureAtlas_dispose(GLTextureAtlas * self);

GLTextureAtlas * GLTextureAtlas_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool GLTextureAtlas_loadSerializedData(GLTextureAtlas * self, compat_type(DeserializationContext *) deserializationContext);
void GLTextureAtlas_serialize(GLTextureAtlas * self, compat_type(SerializationContext *) serializationContext);

const char ** GLTextureAtlas_getKeys(GLTextureAtlas * self, size_t * outCount);
bool GLTextureAtlas_hasKey(GLTextureAtlas * self, const char * key);
void GLTextureAtlas_setEntry(GLTextureAtlas * self, const char * key, struct GLTextureAtlas_entry entry);
void GLTextureAtlas_removeEntry(GLTextureAtlas * self, const char * key);
// lookup returns {0.0f, 0.0f, 0.0f, 0.0f} for nonexistent keys
struct GLTextureAtlas_entry GLTextureAtlas_lookup(GLTextureAtlas * self, const char * key);

unsigned int GLTextureAtlas_getVertices(GLTextureAtlas * self, const char * key, float offsetX, float offsetY, float relativeOriginX, float relativeOriginY, float width, float height, struct vertex_p2f_t2f * outVertices);
unsigned int GLTextureAtlas_getVerticesWithColor(GLTextureAtlas * self, const char * key, float offsetX, float offsetY, float relativeOriginX, float relativeOriginY, float width, float height, float red, float green, float blue, float alpha, struct vertex_p2f_t2f_c4f * outVertices);
unsigned int GLTextureAtlas_getIndexes(GLTextureAtlas * self, void * outIndexes, GLenum indexType, unsigned int indexOffset);

#ifdef __cplusplus
}
#endif
#endif
