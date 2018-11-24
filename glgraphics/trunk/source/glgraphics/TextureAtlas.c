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

#include "dynamictypes/DataArray.h"
#include "dynamictypes/HashTable.h"
#include "glgraphics/TextureAtlas.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

TextureAtlas * TextureAtlas_create() {
	stemobject_create_implementation(TextureAtlas, init)
}

bool TextureAtlas_init(TextureAtlas * self) {
	call_super(init, self);
	self->dispose = TextureAtlas_dispose;
	self->textureID = 0;
	self->textureWidth = self->textureHeight = 0;
	self->private_ivar(hashTable) = hashCreate();
	return true;
}

void TextureAtlas_dispose(TextureAtlas * self) {
	hashDispose(self->private_ivar(hashTable));
	glDeleteTextures(1, &self->textureID);
	call_super(dispose, self);
}

void TextureAtlas_setTexture(TextureAtlas * self, bool magnifyNearest, unsigned int width, unsigned int height, void * bitmapData) {
	if (self->textureID != 0) {
		glDeleteTextures(1, &self->textureID);
	}
	glGenTextures(1, &self->textureID);
	
	glBindTexture(GL_TEXTURE_2D, self->textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (magnifyNearest) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	self->textureWidth = width;
	self->textureHeight = height;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmapData);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

const char ** TextureAtlas_getKeys(TextureAtlas * self, size_t * outCount) {
	return hashGetKeys(self->private_ivar(hashTable), outCount);
}

bool TextureAtlas_hasKey(TextureAtlas * self, const char * key) {
	return hashHas(self->private_ivar(hashTable), key);
}

void TextureAtlas_setEntry(TextureAtlas * self, const char * key, struct TextureAtlas_entry entry) {
	hashSet(self->private_ivar(hashTable), key, valueCreateArray(arrayCreateWithValues(
		valueCreateFloat(entry.left),
		valueCreateFloat(entry.right),
		valueCreateFloat(entry.bottom),
		valueCreateFloat(entry.top)
	), true, false));
}

void TextureAtlas_removeEntry(TextureAtlas * self, const char * key) {
	hashDelete(self->private_ivar(hashTable), key);
}

struct TextureAtlas_entry TextureAtlas_lookup(TextureAtlas * self, const char * key) {
	struct TextureAtlas_entry entry = {0.0f, 0.0f, 0.0f, 0.0f};
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

Vector2f TextureAtlas_getEntryDimensions(TextureAtlas * self, const char * key, float width, float height) {
	Vector2f dimensions;
	struct TextureAtlas_entry entry;
	
	if (self->textureID == 0 || self->textureWidth == 0 || self->textureHeight == 0 || (width != TEXTUREATLAS_SIZE_AUTO && height != TEXTUREATLAS_SIZE_AUTO)) {
		dimensions.x = width;
		dimensions.y = height;
		
	} else {
		entry = TextureAtlas_lookup(self, key);
		
		if (width == TEXTUREATLAS_SIZE_AUTO && height == TEXTUREATLAS_SIZE_AUTO) {
			dimensions.x = (entry.right - entry.left) * self->textureWidth;
			dimensions.y = (entry.top - entry.bottom) * self->textureHeight;
			
		} else if (width == TEXTUREATLAS_SIZE_AUTO) {
			dimensions.x = (entry.right - entry.left) * height / (entry.top - entry.bottom) * self->textureWidth / self->textureHeight;
			dimensions.y = height;
			
		} else if (height == TEXTUREATLAS_SIZE_AUTO) {
			dimensions.x = width;
			dimensions.y = (entry.top - entry.bottom) * width / (entry.right - entry.left) * self->textureHeight / self->textureWidth;
			
		} else {
			dimensions.x = width;
			dimensions.y = height;
		}
	}
	
	return dimensions;
}

#define getVertices_writePosition() \
	outVertices[vertexCount + 0].position[0] = \
	outVertices[vertexCount + 3].position[0] = offset.x - size.x * relativeOrigin.x; \
	outVertices[vertexCount + 0].position[1] = \
	outVertices[vertexCount + 1].position[1] = offset.y - size.y * relativeOrigin.y; \
	outVertices[vertexCount + 1].position[0] = \
	outVertices[vertexCount + 2].position[0] = offset.x + size.x * (1.0f - relativeOrigin.x); \
	outVertices[vertexCount + 2].position[1] = \
	outVertices[vertexCount + 3].position[1] = offset.y + size.y * (1.0f - relativeOrigin.y)

#define getVertices_writeTexCoords() \
	outVertices[vertexCount + 0].texCoords[0] = entry.left; \
	outVertices[vertexCount + 0].texCoords[1] = entry.bottom; \
	outVertices[vertexCount + 1].texCoords[0] = entry.right; \
	outVertices[vertexCount + 1].texCoords[1] = entry.bottom; \
	outVertices[vertexCount + 2].texCoords[0] = entry.right; \
	outVertices[vertexCount + 2].texCoords[1] = entry.top; \
	outVertices[vertexCount + 3].texCoords[0] = entry.left; \
	outVertices[vertexCount + 3].texCoords[1] = entry.top

#define getVertices_writeColor() \
	outVertices[vertexCount + 0].color[0] = \
	outVertices[vertexCount + 1].color[0] = \
	outVertices[vertexCount + 2].color[0] = \
	outVertices[vertexCount + 3].color[0] = color.red; \
	outVertices[vertexCount + 0].color[1] = \
	outVertices[vertexCount + 1].color[1] = \
	outVertices[vertexCount + 2].color[1] = \
	outVertices[vertexCount + 3].color[1] = color.green; \
	outVertices[vertexCount + 0].color[2] = \
	outVertices[vertexCount + 1].color[2] = \
	outVertices[vertexCount + 2].color[2] = \
	outVertices[vertexCount + 3].color[2] = color.blue; \
	outVertices[vertexCount + 0].color[3] = \
	outVertices[vertexCount + 1].color[3] = \
	outVertices[vertexCount + 2].color[3] = \
	outVertices[vertexCount + 3].color[3] = color.alpha

#define getVertices_writeTypedIndexes(indexes) \
	indexes[indexCount + 0] = vertexCount; \
	indexes[indexCount + 1] = vertexCount + 1; \
	indexes[indexCount + 2] = vertexCount + 2; \
	indexes[indexCount + 3] = vertexCount + 2; \
	indexes[indexCount + 4] = vertexCount + 3; \
	indexes[indexCount + 5] = vertexCount;

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

void TextureAtlas_getVertices(TextureAtlas * self, const char * key, Vector2f offset, Vector2f relativeOrigin, Vector2f size, GLenum indexType, struct vertex_p2f_t2f * outVertices, void * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int vertexCount = 0;
	
	if (ioVertexCount != NULL) {
		vertexCount = *ioVertexCount;
	}
	if (outVertices != NULL) {
		struct TextureAtlas_entry entry;
		
		entry = TextureAtlas_lookup(self, key);
		getVertices_writePosition();
		getVertices_writeTexCoords();
	}
	vertexCount += 4;
	if (outIndexes != NULL) {
		unsigned int indexCount = 0;
		
		if (ioIndexCount != NULL) {
			indexCount = *ioIndexCount;
		}
		getVertices_writeIndexes();
	}
	if (ioVertexCount != NULL) {
		*ioVertexCount = vertexCount;
	}
	if (ioIndexCount != NULL) {
		*ioIndexCount += 6;
	}
}

void TextureAtlas_getVerticesWithColor(TextureAtlas * self, const char * key, Vector2f offset, Vector2f relativeOrigin, Vector2f size, Color4f color, GLenum indexType, struct vertex_p2f_t2f_c4f * outVertices, void * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int vertexCount = 0;
	
	if (ioVertexCount != NULL) {
		vertexCount = *ioVertexCount;
	}
	if (outVertices != NULL) {
		struct TextureAtlas_entry entry;
		
		entry = TextureAtlas_lookup(self, key);
		getVertices_writePosition();
		getVertices_writeTexCoords();
		getVertices_writeColor();
	}
	vertexCount += 4;
	if (outIndexes != NULL) {
		unsigned int indexCount = 0;
		
		if (ioIndexCount != NULL) {
			indexCount = *ioIndexCount;
		}
		getVertices_writeIndexes();
	}
	if (ioVertexCount != NULL) {
		*ioVertexCount = vertexCount;
	}
	if (ioIndexCount != NULL) {
		*ioIndexCount += 6;
	}
}
