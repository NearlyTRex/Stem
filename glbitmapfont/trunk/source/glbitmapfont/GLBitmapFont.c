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

#include "glbitmapfont/GLBitmapFont.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define SUPERCLASS StemObject

GLBitmapFont * GLBitmapFont_create(struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS]) {
	stemobject_create_implementation(GLBitmapFont, init, characters)
}

static void sharedInit(GLBitmapFont * self) {
	call_super(init, self);
	
	self->atlasName = NULL;
	self->atlas = NULL;
	self->private_ivar(atlasOwned) = false;
	self->dispose = GLBitmapFont_dispose;
}

bool GLBitmapFont_init(GLBitmapFont * self, struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS]) {
	unsigned int charIndex;
	
	sharedInit(self);
	memcpy(self->characters, characters, sizeof(struct GLBitmapFont_charEntry) * GLBITMAPFONT_NUM_CHARS);
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		if (characters[charIndex].kernChars != NULL) {
			self->characters[charIndex].kernChars = malloc(sizeof(struct GLBitmapFont_kernEntry) * characters[charIndex].kernCharCount);
			memcpy(self->characters[charIndex].kernChars, characters[charIndex].kernChars, sizeof(struct GLBitmapFont_kernEntry) * characters[charIndex].kernCharCount);
		}
	}
	return true;
}

void GLBitmapFont_dispose(GLBitmapFont * self) {
	unsigned int charIndex;
	
	if (self->private_ivar(atlasOwned)) {
		self->atlas->dispose(self->atlas);
	}
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		free(self->characters[charIndex].kernChars);
	}
	call_super(dispose, self);
}

GLBitmapFont * GLBitmapFont_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	GLBitmapFont * self;
	
	self = malloc(sizeof(GLBitmapFont));
	if (!GLBitmapFont_loadSerializedData(self, context)) {
		free(self);
		return NULL;
	}
	self->protected_ivar(allocated) = true;
	
	return self;
}

bool GLBitmapFont_loadSerializedData(GLBitmapFont * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	size_t kernCharIndex, kernCharIndex2, nchars;
	unsigned int charIndex;
	const char * key, * key2;
	const char * atlasName, * atlasKey;
	
	sharedInit(self);
	memset(self->characters, 0x00, sizeof(self->characters));
	
	context->beginStructure(context, "glbitmapfont");
	formatVersion = context->readUInt16(context, "format_version");
	if (formatVersion != GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION) {
		self->dispose(self);
		return false;
	}
	atlasName = context->readString(context, "atlas_name");
	nchars = context->beginDictionary(context, "characters");
	if (nchars != GLBITMAPFONT_NUM_CHARS) {
		self->dispose(self);
		return false;
	}
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		key = context->readNextDictionaryKey(context);
		if (key[0] < GLBITMAPFONT_PRINTABLE_MIN || key[0] > GLBITMAPFONT_PRINTABLE_MAX || strlen(key) > 1) {
			self->dispose(self);
			return false;
		}
		context->beginStructure(context, key);
		self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].advance = context->readFloat(context, "advance");
		self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].glyphOffset = context->readFloat(context, "glyph_offset");
		self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].glyphWidth = context->readFloat(context, "glyph_width");
		atlasKey = context->readString(context, "atlas_key");
		if (atlasKey != NULL) {
			self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].atlasKey = strdup(atlasKey);
		}
		
		self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].kernCharCount = context->beginDictionary(context, "kerning_table");
		if (self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].kernCharCount == 0) {
			self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].kernChars = NULL;
		} else {
			self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].kernChars = malloc(sizeof(struct GLBitmapFont_kernEntry) * self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].kernCharCount);
		}
		
		for (kernCharIndex = 0; kernCharIndex < self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].kernCharCount; kernCharIndex++) {
			key2 = context->readNextDictionaryKey(context);
			for (kernCharIndex2 = 0; kernCharIndex2 < kernCharIndex; kernCharIndex2++) {
				if (self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].kernChars[kernCharIndex2].previous == key2[0]) {
					self->dispose(self);
					return false;
				}
			}
			self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].kernChars[kernCharIndex].previous = key2[0];
			self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].kernChars[kernCharIndex].offset = context->readFloat(context, key2);
		}
		context->endDictionary(context);
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		self->dispose(self);
		return false;
	}
	
	self->atlasName = Atom_fromString(atlasName);
	
	return true;
}

void GLBitmapFont_serialize(GLBitmapFont * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	unsigned int charIndex, kernCharIndex;
	char charString[2] = {0, 0};
	
	context->beginStructure(context, "glbitmapfont");
	context->writeUInt16(context, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->writeString(context, "atlas_name", self->atlasName);
	context->beginDictionary(context, "characters");
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		charString[0] = GLBITMAPFONT_PRINTABLE_MIN + charIndex;
		context->beginStructure(context, charString);
		context->writeFloat(context, "advance", self->characters[charIndex].advance);
		context->writeFloat(context, "glyph_offset", self->characters[charIndex].glyphOffset);
		context->writeFloat(context, "glyph_width", self->characters[charIndex].glyphWidth);
		context->writeString(context, "atlas_key", self->characters[charIndex].atlasKey);
		context->beginDictionary(context, "kerning_table");
		for (kernCharIndex = 0; kernCharIndex < self->characters[charIndex].kernCharCount; kernCharIndex++) {
			charString[0] = self->characters[charIndex].kernChars[kernCharIndex].previous;
			context->writeFloat(context, charString, self->characters[charIndex].kernChars[kernCharIndex].offset);
		}
		context->endDictionary(context);
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
}

void GLBitmapFont_setTextureAtlas(GLBitmapFont * self, TextureAtlas * atlas, bool takeOwnership) {
	if (self->private_ivar(atlasOwned)) {
		self->atlas->dispose(self->atlas);
	}
	self->atlas = atlas;
	self->private_ivar(atlasOwned) = takeOwnership && atlas != NULL;
}

float GLBitmapFont_measureString(GLBitmapFont * self, const char * string, size_t length) {
	float width = 0.0f;
	unsigned int charIndex, kernCharIndex;
	
	if (length == GLBITMAPFONT_USE_STRLEN) {
		length = strlen(string);
	}
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (string[charIndex] >= GLBITMAPFONT_PRINTABLE_MIN && string[charIndex] <= GLBITMAPFONT_PRINTABLE_MAX) {
			width += self->characters[string[charIndex] - GLBITMAPFONT_PRINTABLE_MIN].advance;
			if (charIndex > 0 && string[charIndex - 1] >= GLBITMAPFONT_PRINTABLE_MIN && string[charIndex - 1] <= GLBITMAPFONT_PRINTABLE_MAX) {
				for (kernCharIndex = 0; kernCharIndex < self->characters[string[charIndex] - GLBITMAPFONT_PRINTABLE_MIN].kernCharCount; kernCharIndex++) {
					if (string[charIndex - 1] == self->characters[string[charIndex] - GLBITMAPFONT_PRINTABLE_MIN].kernChars[kernCharIndex].previous) {
						width += self->characters[string[charIndex] - GLBITMAPFONT_PRINTABLE_MIN].kernChars[kernCharIndex].offset;
						break;
					}
				}
			}
		}
	}
	return width;
}

size_t GLBitmapFont_indexAtPositionX(GLBitmapFont * self, const char * string, size_t length, float emSize, float positionX, float relativeOriginX, bool * outLeadingEdge) {
	float totalWidth = 0.0f, charWidth, halfKernOffset = 0.0f;
	size_t charIndex, kernCharIndex;
	
	if (length == GLBITMAPFONT_USE_STRLEN) {
		length = strlen(string);
	}
	if (relativeOriginX != 0.0f) {
		positionX += GLBitmapFont_measureString(self, string, length) * fabs(emSize) * relativeOriginX;
	}
	positionX /= emSize;
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (string[charIndex] >= GLBITMAPFONT_PRINTABLE_MIN && string[charIndex] <= GLBITMAPFONT_PRINTABLE_MAX) {
			charWidth = self->characters[string[charIndex] - GLBITMAPFONT_PRINTABLE_MIN].advance + halfKernOffset;
			halfKernOffset = 0.0f;
			if (charIndex < length - 1 && string[charIndex + 1] >= GLBITMAPFONT_PRINTABLE_MIN && string[charIndex + 1] <= GLBITMAPFONT_PRINTABLE_MAX) {
				for (kernCharIndex = 0; kernCharIndex < self->characters[string[charIndex + 1] - GLBITMAPFONT_PRINTABLE_MIN].kernCharCount; kernCharIndex++) {
					if (self->characters[string[charIndex + 1] - GLBITMAPFONT_PRINTABLE_MIN].kernChars[kernCharIndex].previous == string[charIndex]) {
						halfKernOffset = self->characters[string[charIndex + 1] - GLBITMAPFONT_PRINTABLE_MIN].kernChars[kernCharIndex].offset * 0.5f;
						charWidth += halfKernOffset;
						break;
					}
				}
			}
			totalWidth += charWidth;
			if (totalWidth > positionX) {
				if (outLeadingEdge != NULL) {
					*outLeadingEdge = totalWidth - charWidth * 0.5f > positionX;
				}
				return charIndex;
			}
		} else {
			totalWidth += halfKernOffset;
			halfKernOffset = 0.0f;
		}
	}
	if (outLeadingEdge != NULL) {
		*outLeadingEdge = false;
	}
	if (length == 0) {
		return 0;
	}
	return length - 1;
}

void GLBitmapFont_getStringVertices(GLBitmapFont * self,
                                    const char * string,
                                    size_t length,
                                    float emSize,
                                    Vector2f offset,
                                    Vector2f relativeOrigin,
                                    bool pixelSnapping,
                                    Rect4f clipBounds,
                                    Color4f color,
                                    struct vertex_p2f_t2f_c4f * outVertices,
                                    GLuint * outIndexes,
                                    unsigned int * ioVertexCount,
                                    unsigned int * ioIndexCount) {
	size_t charIndex, kernCharIndex;
	float positionX = 0.0f;
	unsigned int charEntryIndex;
	unsigned int vertexCount = *ioVertexCount, indexCount = *ioIndexCount;
	struct TextureAtlas_entry atlasEntry;
	Rect4f vertexBounds;
	
	if (clipBounds.right - clipBounds.left <= 0.0f || clipBounds.top - clipBounds.bottom <= 0.0f) {
		return;
	}
	if (length == GLBITMAPFONT_USE_STRLEN) {
		length = strlen(string);
	}
	if (relativeOrigin.x != 0.0f) {
		offset.x -= GLBitmapFont_measureString(self, string, length) * fabs(emSize) * relativeOrigin.x;
	}
	offset.y -= emSize * relativeOrigin.y;
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (string[charIndex] >= GLBITMAPFONT_PRINTABLE_MIN && string[charIndex] <= GLBITMAPFONT_PRINTABLE_MAX) {
			charEntryIndex = string[charIndex] - GLBITMAPFONT_PRINTABLE_MIN;
			
			if (charIndex > 0) {
				for (kernCharIndex = 0; kernCharIndex < self->characters[charEntryIndex].kernCharCount; kernCharIndex++) {
					if (string[charIndex - 1] == self->characters[charEntryIndex].kernChars[kernCharIndex].previous) {
						positionX += self->characters[charEntryIndex].kernChars[kernCharIndex].offset * emSize;
						break;
					}
				}
			}
			
			vertexBounds.left = offset.x + positionX + self->characters[charEntryIndex].glyphOffset * fabs(emSize);
			if (pixelSnapping) {
				vertexBounds.right = offset.x + positionX + floorf((self->characters[charEntryIndex].glyphOffset + self->characters[charEntryIndex].glyphWidth) * fabs(emSize));
			} else {
				vertexBounds.right = offset.x + positionX + (self->characters[charEntryIndex].glyphOffset + self->characters[charEntryIndex].glyphWidth) * fabs(emSize);
			}
			vertexBounds.bottom = offset.y;
			vertexBounds.top = offset.y + emSize;
			
			if (vertexBounds.left < clipBounds.right &&
			    vertexBounds.right > clipBounds.left &&
			    vertexBounds.top > clipBounds.bottom &&
			    vertexBounds.bottom < clipBounds.top) {
				if (outVertices != NULL) {
					atlasEntry = TextureAtlas_lookup(self->atlas, self->characters[charEntryIndex].atlasKey);
				
					outVertices[vertexCount + 0].position[0] = outVertices[vertexCount + 1].position[0] = vertexBounds.left;
					outVertices[vertexCount + 0].position[1] = outVertices[vertexCount + 3].position[1] = vertexBounds.top;
					outVertices[vertexCount + 2].position[0] = outVertices[vertexCount + 3].position[0] = vertexBounds.right;
					outVertices[vertexCount + 1].position[1] = outVertices[vertexCount + 2].position[1] = vertexBounds.bottom;
				
					outVertices[vertexCount + 0].texCoords[0] = outVertices[vertexCount + 1].texCoords[0] = atlasEntry.left;
					outVertices[vertexCount + 1].texCoords[1] = outVertices[vertexCount + 2].texCoords[1] = atlasEntry.bottom;
					outVertices[vertexCount + 2].texCoords[0] = outVertices[vertexCount + 3].texCoords[0] = atlasEntry.right;
					outVertices[vertexCount + 0].texCoords[1] = outVertices[vertexCount + 3].texCoords[1] = atlasEntry.top;
				
					outVertices[vertexCount + 0].color[0] = outVertices[vertexCount + 1].color[0] = outVertices[vertexCount + 2].color[0] = outVertices[vertexCount + 3].color[0] = color.red;
					outVertices[vertexCount + 0].color[1] = outVertices[vertexCount + 1].color[1] = outVertices[vertexCount + 2].color[1] = outVertices[vertexCount + 3].color[1] = color.green;
					outVertices[vertexCount + 0].color[2] = outVertices[vertexCount + 1].color[2] = outVertices[vertexCount + 2].color[2] = outVertices[vertexCount + 3].color[2] = color.blue;
					outVertices[vertexCount + 0].color[3] = outVertices[vertexCount + 1].color[3] = outVertices[vertexCount + 2].color[3] = outVertices[vertexCount + 3].color[3] = color.alpha;
					
					if (vertexBounds.left < clipBounds.left) {
						outVertices[vertexCount + 0].texCoords[0] = outVertices[vertexCount + 1].texCoords[0] = atlasEntry.right - (atlasEntry.right - atlasEntry.left) * (vertexBounds.right - clipBounds.left) / (vertexBounds.right - vertexBounds.left);
						outVertices[vertexCount + 0].position[0] = outVertices[vertexCount + 1].position[0] = clipBounds.left;
					}
					if (vertexBounds.right > clipBounds.right) {
						outVertices[vertexCount + 2].texCoords[0] = outVertices[vertexCount + 3].texCoords[0] = atlasEntry.left + (atlasEntry.right - atlasEntry.left) * (clipBounds.right - vertexBounds.left) / (vertexBounds.right - vertexBounds.left);
						outVertices[vertexCount + 2].position[0] = outVertices[vertexCount + 3].position[0] = clipBounds.right;
					}
					if (vertexBounds.bottom < clipBounds.bottom) {
						outVertices[vertexCount + 1].texCoords[1] = outVertices[vertexCount + 2].texCoords[1] = atlasEntry.top - (atlasEntry.top - atlasEntry.bottom) * (vertexBounds.top - clipBounds.bottom) / (vertexBounds.top - vertexBounds.bottom);
						outVertices[vertexCount + 1].position[1] = outVertices[vertexCount + 2].position[1] = clipBounds.bottom;
					}
					if (vertexBounds.top > clipBounds.top) {
						outVertices[vertexCount + 0].texCoords[1] = outVertices[vertexCount + 3].texCoords[1] = atlasEntry.bottom + (atlasEntry.top - atlasEntry.bottom) * (clipBounds.top - vertexBounds.bottom) / (vertexBounds.top - vertexBounds.bottom);
						outVertices[vertexCount + 0].position[1] = outVertices[vertexCount + 3].position[1] = clipBounds.top;
					}
				}
				if (outIndexes != NULL) {
					outIndexes[indexCount + 0] = vertexCount;
					outIndexes[indexCount + 1] = vertexCount + 1;
					outIndexes[indexCount + 2] = vertexCount + 2;
					outIndexes[indexCount + 3] = vertexCount + 2;
					outIndexes[indexCount + 4] = vertexCount + 3;
					outIndexes[indexCount + 5] = vertexCount;
				}
				vertexCount += 4;
				indexCount += 6;
			}
			
			positionX += self->characters[charEntryIndex].advance * emSize;
			if (pixelSnapping) {
				positionX = roundf(positionX);
			}
		}
	}
	*ioVertexCount = vertexCount;
	*ioIndexCount = indexCount;
}
