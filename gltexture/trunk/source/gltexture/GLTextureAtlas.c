/*
  Copyright (c) 2015 Alex Diener
  
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

#include "dynamictypes/DataArray.h"
#include "dynamictypes/HashTable.h"
#include "gltexture/GLTextureAtlas.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

static void sharedInit(GLTextureAtlas * self) {
	call_super(init, self);
	
	self->textureName = NULL;
	self->private_ivar(textureNameOwned) = false;
	self->texture = NULL;
	self->private_ivar(textureOwned) = false;
	self->dispose = GLTextureAtlas_dispose;
}

GLTextureAtlas * GLTextureAtlas_create() {
	stemobject_create_implementation(GLTextureAtlas, init)
}

bool GLTextureAtlas_init(GLTextureAtlas * self) {
	sharedInit(self);
	self->private_ivar(hashTable) = hashCreate();
	return true;
}

void GLTextureAtlas_dispose(GLTextureAtlas * self) {
	if (self->private_ivar(textureNameOwned)) {
		free(self->textureName);
	}
	if (self->private_ivar(textureOwned)) {
		self->texture->dispose(self->texture);
	}
	hashDispose(self->private_ivar(hashTable));
	call_super(dispose, self);
}

GLTextureAtlas * GLTextureAtlas_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(GLTextureAtlas, deserializationContext, loadSerializedData)
}

bool GLTextureAtlas_loadSerializedData(GLTextureAtlas * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * textureName;
	size_t keyCount, keyIndex;
	const char * key;
	HashTable * hashTable;
	struct GLTextureAtlas_entry entry;
	
	context->beginStructure(context, "gltextureatlas");
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status == SERIALIZATION_ERROR_OK && formatVersion > GLTEXTURE_SERIALIZATION_FORMAT_VERSION) {
		return false;
	}
	
	textureName = context->readString(context, "texture_name");
	keyCount = context->beginDictionary(context, "entries");
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	
	hashTable = hashCreate();
	for (keyIndex = 0; keyIndex < keyCount; keyIndex++) {
		key = context->readNextDictionaryKey(context);
		context->beginStructure(context, key);
		entry.left = context->readFloat(context, "left");
		entry.right = context->readFloat(context, "right");
		entry.bottom = context->readFloat(context, "bottom");
		entry.top = context->readFloat(context, "top");
		context->endStructure(context);
		hashSet(hashTable, key, valueCreateArray(arrayCreateWithValues(
			valueCreateFloat(entry.left),
			valueCreateFloat(entry.right),
			valueCreateFloat(entry.bottom),
			valueCreateFloat(entry.top)
		), true, false));
	}
	context->endDictionary(context);
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		hashDispose(hashTable);
		return false;
	}
	
	sharedInit(self);
	self->textureName = strdup(textureName);
	self->private_ivar(textureNameOwned) = true;
	self->private_ivar(hashTable) = hashTable;
	
	return true;
}

void GLTextureAtlas_serialize(GLTextureAtlas * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	size_t keyCount, keyIndex;
	const char ** keys;
	struct GLTextureAtlas_entry entry;
	
	context->beginStructure(context, "gltextureatlas");
	context->writeUInt16(context, "format_version", GLTEXTUREATLAS_SERIALIZATION_FORMAT_VERSION);
	context->writeString(context, "texture_name", self->textureName == NULL ? "" : self->textureName);
	context->beginDictionary(context, "entries");
	keys = hashGetKeys(self->private_ivar(hashTable), &keyCount);
	for (keyIndex = 0; keyIndex < keyCount; keyIndex++) {
		context->beginStructure(context, keys[keyIndex]);
		entry = GLTextureAtlas_lookup(self, keys[keyIndex]);
		context->writeFloat(context, "left", entry.left);
		context->writeFloat(context, "right", entry.right);
		context->writeFloat(context, "bottom", entry.bottom);
		context->writeFloat(context, "top", entry.top);
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
}

void GLTextureAtlas_setTexture(GLTextureAtlas * self, GLTexture * texture, bool takeOwnership) {
	if (self->private_ivar(textureOwned)) {
		self->texture->dispose(self->texture);
	}
	self->texture = texture;
	self->private_ivar(textureOwned) = takeOwnership && texture != NULL;
}

const char ** GLTextureAtlas_getKeys(GLTextureAtlas * self, size_t * outCount) {
	return hashGetKeys(self->private_ivar(hashTable), outCount);
}

bool GLTextureAtlas_hasKey(GLTextureAtlas * self, const char * key) {
	return hashHas(self->private_ivar(hashTable), key);
}

void GLTextureAtlas_setEntry(GLTextureAtlas * self, const char * key, struct GLTextureAtlas_entry entry) {
	hashSet(self->private_ivar(hashTable), key, valueCreateArray(arrayCreateWithValues(
		valueCreateFloat(entry.left),
		valueCreateFloat(entry.right),
		valueCreateFloat(entry.bottom),
		valueCreateFloat(entry.top)
	), true, false));
}

void GLTextureAtlas_removeEntry(GLTextureAtlas * self, const char * key) {
	hashDelete(self->private_ivar(hashTable), key);
}

struct GLTextureAtlas_entry GLTextureAtlas_lookup(GLTextureAtlas * self, const char * key) {
	struct GLTextureAtlas_entry entry = {0.0f, 0.0f, 0.0f, 0.0f};
	DataValue * value;
	
	value = hashGet(self->private_ivar(hashTable), key);
	if (value != NULL) {
		entry.left = value->value.array->values[0].value.float32;
		entry.right = value->value.array->values[1].value.float32;
		entry.bottom = value->value.array->values[2].value.float32;
		entry.top = value->value.array->values[3].value.float32;
	}
	return entry;
}

Vector2f GLTextureAtlas_getEntryDimensions(GLTextureAtlas * self, const char * key, float width, float height) {
	Vector2f dimensions;
	struct GLTextureAtlas_entry entry;
	
	if (self->texture == NULL || self->texture->pixelWidth == 0 || self->texture->pixelHeight == 0 || (width != GLTEXTUREATLAS_SIZE_AUTO && height != GLTEXTUREATLAS_SIZE_AUTO)) {
		dimensions.x = width;
		dimensions.y = height;
		
	} else {
		entry = GLTextureAtlas_lookup(self, key);
		
		if (width == GLTEXTUREATLAS_SIZE_AUTO && height == GLTEXTUREATLAS_SIZE_AUTO) {
			dimensions.x = (entry.right - entry.left) * self->texture->pixelWidth;
			dimensions.y = (entry.top - entry.bottom) * self->texture->pixelHeight;
			
		} else if (width == GLTEXTUREATLAS_SIZE_AUTO) {
			dimensions.x = (entry.right - entry.left) * height / (entry.top - entry.bottom) * self->texture->pixelWidth / self->texture->pixelHeight;
			dimensions.y = height;
			
		} else if (height == GLTEXTUREATLAS_SIZE_AUTO) {
			dimensions.x = width;
			dimensions.y = (entry.top - entry.bottom) * width / (entry.right - entry.left) * self->texture->pixelHeight / self->texture->pixelWidth;
			
		} else {
			dimensions.x = width;
			dimensions.y = height;
		}
	}
	
	return dimensions;
}

#define getVertices_writePosition() \
	outVertices[*ioVertexCount + 0].position[0] = \
	outVertices[*ioVertexCount + 3].position[0] = offset.x - size.x * relativeOrigin.x; \
	outVertices[*ioVertexCount + 0].position[1] = \
	outVertices[*ioVertexCount + 1].position[1] = offset.y - size.y * relativeOrigin.y; \
	outVertices[*ioVertexCount + 1].position[0] = \
	outVertices[*ioVertexCount + 2].position[0] = offset.x + size.x * (1.0f - relativeOrigin.x); \
	outVertices[*ioVertexCount + 2].position[1] = \
	outVertices[*ioVertexCount + 3].position[1] = offset.y + size.y * (1.0f - relativeOrigin.y)

#define getVertices_writeTexCoords() \
	outVertices[*ioVertexCount + 0].texCoords[0] = entry.left; \
	outVertices[*ioVertexCount + 0].texCoords[1] = entry.bottom; \
	outVertices[*ioVertexCount + 1].texCoords[0] = entry.right; \
	outVertices[*ioVertexCount + 1].texCoords[1] = entry.bottom; \
	outVertices[*ioVertexCount + 2].texCoords[0] = entry.right; \
	outVertices[*ioVertexCount + 2].texCoords[1] = entry.top; \
	outVertices[*ioVertexCount + 3].texCoords[0] = entry.left; \
	outVertices[*ioVertexCount + 3].texCoords[1] = entry.top

#define getVertices_writeColor() \
	outVertices[*ioVertexCount + 0].color[0] = \
	outVertices[*ioVertexCount + 1].color[0] = \
	outVertices[*ioVertexCount + 2].color[0] = \
	outVertices[*ioVertexCount + 3].color[0] = color.red; \
	outVertices[*ioVertexCount + 0].color[1] = \
	outVertices[*ioVertexCount + 1].color[1] = \
	outVertices[*ioVertexCount + 2].color[1] = \
	outVertices[*ioVertexCount + 3].color[1] = color.green; \
	outVertices[*ioVertexCount + 0].color[2] = \
	outVertices[*ioVertexCount + 1].color[2] = \
	outVertices[*ioVertexCount + 2].color[2] = \
	outVertices[*ioVertexCount + 3].color[2] = color.blue; \
	outVertices[*ioVertexCount + 0].color[3] = \
	outVertices[*ioVertexCount + 1].color[3] = \
	outVertices[*ioVertexCount + 2].color[3] = \
	outVertices[*ioVertexCount + 3].color[3] = color.alpha

#define getVertices_writeTypedIndexes(indexes) \
	indexes[*ioIndexCount + 0] = *ioVertexCount; \
	indexes[*ioIndexCount + 1] = *ioVertexCount + 1; \
	indexes[*ioIndexCount + 2] = *ioVertexCount + 2; \
	indexes[*ioIndexCount + 3] = *ioVertexCount + 2; \
	indexes[*ioIndexCount + 4] = *ioVertexCount + 3; \
	indexes[*ioIndexCount + 5] = *ioVertexCount;

#define getVertices_writeIndexes() \
	switch (indexType) { \
		case GL_UNSIGNED_BYTE: { \
			GLubyte * indexesByte = outIndexes; \
			getVertices_writeTypedIndexes(indexesByte); \
			break; \
		} \
		case GL_UNSIGNED_SHORT: { \
			GLushort * indexesShort = outIndexes; \
			getVertices_writeTypedIndexes(indexesShort); \
			break; \
		} \
		case GL_UNSIGNED_INT: { \
			GLuint * indexesInt = outIndexes; \
			getVertices_writeTypedIndexes(indexesInt); \
			break; \
		} \
	}

void GLTextureAtlas_getVertices(GLTextureAtlas * self, const char * key, Vector2f offset, Vector2f relativeOrigin, Vector2f size, GLenum indexType, struct vertex_p2f_t2f * outVertices, void * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	if (outVertices != NULL) {
		struct GLTextureAtlas_entry entry;
		
		entry = GLTextureAtlas_lookup(self, key);
		getVertices_writePosition();
		getVertices_writeTexCoords();
	}
	if (outIndexes != NULL) {
		getVertices_writeIndexes();
	}
	if (ioVertexCount != NULL) {
		*ioVertexCount += 4;
	}
	if (ioIndexCount != NULL) {
		*ioIndexCount += 6;
	}
}

void GLTextureAtlas_getVerticesWithColor(GLTextureAtlas * self, const char * key, Vector2f offset, Vector2f relativeOrigin, Vector2f size, Color4f color, GLenum indexType, struct vertex_p2f_t2f_c4f * outVertices, void * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	if (outVertices != NULL) {
		struct GLTextureAtlas_entry entry;
		
		entry = GLTextureAtlas_lookup(self, key);
		getVertices_writePosition();
		getVertices_writeTexCoords();
		getVertices_writeColor();
	}
	if (outIndexes != NULL) {
		getVertices_writeIndexes();
	}
	if (ioVertexCount != NULL) {
		*ioVertexCount += 4;
	}
	if (ioIndexCount != NULL) {
		*ioIndexCount += 6;
	}
}
