/*
  Copyright (c) 2012 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#include "glbitmapfont/GLBitmapFont.h"
#include "glgraphics/GLGraphics.h"
#include <string.h>
#include <stddef.h>

#define SUPERCLASS GLFont

GLBitmapFont * GLBitmapFont_create(struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS]) {
	stemobject_create_implementation(GLBitmapFont, init, characters)
}

static void sharedInit(GLBitmapFont * self) {
	call_super(init, self);
	
	self->textureName = NULL;
	self->textureNameOwned = false;
	self->texture = NULL;
	self->textureOwned = false;
	self->dispose = GLBitmapFont_dispose;
	self->measureString = GLBitmapFont_measureString;
	self->indexAtWidth = GLBitmapFont_indexAtWidth;
	self->drawString = GLBitmapFont_drawString;
	self->setTexture = GLBitmapFont_setTexture;
}

void GLBitmapFont_init(GLBitmapFont * self, struct GLBitmapFont_charEntry characters[GLBITMAPFONT_NUM_CHARS]) {
	unsigned int charIndex;
	
	sharedInit(self);
	memcpy(self->characters, characters, sizeof(struct GLBitmapFont_charEntry) * GLBITMAPFONT_NUM_CHARS);
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		if (characters[charIndex].kernChars != NULL) {
			self->characters[charIndex].kernChars = malloc(sizeof(struct GLBitmapFont_kernEntry) * characters[charIndex].kernCharCount);
			memcpy(self->characters[charIndex].kernChars, characters[charIndex].kernChars, sizeof(struct GLBitmapFont_kernEntry) * characters[charIndex].kernCharCount);
		}
	}
}

void GLBitmapFont_dispose(GLBitmapFont * self) {
	unsigned int charIndex;
	
	if (self->textureNameOwned) {
		free(self->textureName);
	}
	if (self->textureOwned) {
		self->texture->dispose(self->texture);
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
	const char * textureName;
	
	sharedInit(self);
	memset(self->characters, 0x00, sizeof(self->characters));
	
	context->beginStructure(context, "glbitmapfont");
	formatVersion = context->readUInt16(context, "format_version");
	if (formatVersion > GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION) {
		self->dispose(self);
		return false;
	}
	textureName = context->readString(context, "texture_name");
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
		context->beginArray(context, "texture_bounds");
		self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].textureLeft = context->readFloat(context, NULL);
		self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].textureRight = context->readFloat(context, NULL);
		self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].textureBottom = context->readFloat(context, NULL);
		self->characters[key[0] - GLBITMAPFONT_PRINTABLE_MIN].textureTop = context->readFloat(context, NULL);
		context->endArray(context);
		
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
	
	self->textureName = malloc(strlen(textureName) + 1);
	strcpy(self->textureName, textureName);
	self->textureNameOwned = true;
	
	return true;
}

void GLBitmapFont_serialize(GLBitmapFont * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	unsigned int charIndex, kernCharIndex;
	char charString[2] = {0, 0};
	
	context->beginStructure(context, "glbitmapfont");
	context->writeUInt16(context, "format_version", GLBITMAPFONT_SERIALIZATION_FORMAT_VERSION);
	context->writeString(context, "texture_name", self->textureName);
	context->beginDictionary(context, "characters");
	for (charIndex = 0; charIndex < GLBITMAPFONT_NUM_CHARS; charIndex++) {
		charString[0] = GLBITMAPFONT_PRINTABLE_MIN + charIndex;
		context->beginStructure(context, charString);
		context->writeFloat(context, "advance", self->characters[charIndex].advance);
		context->writeFloat(context, "glyph_offset", self->characters[charIndex].glyphOffset);
		context->writeFloat(context, "glyph_width", self->characters[charIndex].glyphWidth);
		context->beginArray(context, "texture_bounds");
		context->writeFloat(context, NULL, self->characters[charIndex].textureLeft);
		context->writeFloat(context, NULL, self->characters[charIndex].textureRight);
		context->writeFloat(context, NULL, self->characters[charIndex].textureBottom);
		context->writeFloat(context, NULL, self->characters[charIndex].textureTop);
		context->endArray(context);
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

void GLBitmapFont_setTexture(GLBitmapFont * self, compat_type(GLTexture *) texturePtr, bool takeOwnership) {
	if (self->textureOwned) {
		self->texture->dispose(self->texture);
	}
	self->texture = texturePtr;
	self->textureOwned = takeOwnership;
}

float GLBitmapFont_measureString(GLBitmapFont * self, const char * string, size_t length) {
	float width = 0.0f;
	unsigned int charIndex, kernCharIndex;
	
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

size_t GLBitmapFont_indexAtWidth(GLBitmapFont * self, const char * string, size_t length, float emWidth, bool * outLeadingEdge) {
	float totalWidth = 0.0f, charWidth, halfKernOffset = 0.0f;
	size_t charIndex, kernCharIndex;
	
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
			if (totalWidth > emWidth) {
				if (outLeadingEdge != NULL) {
					*outLeadingEdge = totalWidth - charWidth * 0.5f > emWidth;
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
	return length - 1;
}

struct vertex_p4f_t2f {
	GLfloat position[4];
	GLfloat texCoords[2];
};

void GLBitmapFont_drawString(GLBitmapFont * self, const char * string, size_t length, float emHeight, float offsetX, float offsetY, float offsetZ) {
	struct vertex_p4f_t2f * vertices;
	GLushort * indexes;
	size_t charIndex, kernCharIndex, printableCharCount = 0, vertexIndex = 0, indexIndex = 0;
	float positionX = 0.0f;
	unsigned int charEntryIndex;
	
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (string[charIndex] >= GLBITMAPFONT_PRINTABLE_MIN && string[charIndex] <= GLBITMAPFONT_PRINTABLE_MAX) {
			printableCharCount++;
		}
	}
	
	vertices = malloc(sizeof(struct vertex_p4f_t2f) * 4 * printableCharCount);
	indexes = malloc(sizeof(GLushort) * 6 * printableCharCount);
	
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (string[charIndex] >= GLBITMAPFONT_PRINTABLE_MIN && string[charIndex] <= GLBITMAPFONT_PRINTABLE_MAX) {
			charEntryIndex = string[charIndex] - GLBITMAPFONT_PRINTABLE_MIN;
			if (charIndex > 0) {
				for (kernCharIndex = 0; kernCharIndex < self->characters[charEntryIndex].kernCharCount; kernCharIndex++) {
					if (string[charIndex - 1] == self->characters[charEntryIndex].kernChars[kernCharIndex].previous) {
						positionX += self->characters[charEntryIndex].kernChars[kernCharIndex].offset;
						break;
					}
				}
			}
			vertices[vertexIndex].position[0] = offsetX + (positionX + self->characters[charEntryIndex].glyphOffset) * emHeight;
			vertices[vertexIndex].position[1] = offsetY + emHeight;
			vertices[vertexIndex].position[2] = offsetZ;
			vertices[vertexIndex].position[3] = 1.0f;
			vertices[vertexIndex].texCoords[0] = self->characters[charEntryIndex].textureLeft;
			vertices[vertexIndex].texCoords[1] = self->characters[charEntryIndex].textureTop;
			vertices[vertexIndex + 1].position[0] = offsetX + (positionX + self->characters[charEntryIndex].glyphOffset) * emHeight;
			vertices[vertexIndex + 1].position[1] = offsetY;
			vertices[vertexIndex + 1].position[2] = offsetZ;
			vertices[vertexIndex + 1].position[3] = 1.0f;
			vertices[vertexIndex + 1].texCoords[0] = self->characters[charEntryIndex].textureLeft;
			vertices[vertexIndex + 1].texCoords[1] = self->characters[charEntryIndex].textureBottom;
			vertices[vertexIndex + 2].position[0] = offsetX + (positionX + self->characters[charEntryIndex].glyphOffset + self->characters[charEntryIndex].glyphWidth) * emHeight;
			vertices[vertexIndex + 2].position[1] = offsetY;
			vertices[vertexIndex + 2].position[2] = offsetZ;
			vertices[vertexIndex + 2].position[3] = 1.0f;
			vertices[vertexIndex + 2].texCoords[0] = self->characters[charEntryIndex].textureRight;
			vertices[vertexIndex + 2].texCoords[1] = self->characters[charEntryIndex].textureBottom;
			vertices[vertexIndex + 3].position[0] = offsetX + (positionX + self->characters[charEntryIndex].glyphOffset + self->characters[charEntryIndex].glyphWidth) * emHeight;
			vertices[vertexIndex + 3].position[1] = offsetY + emHeight;
			vertices[vertexIndex + 3].position[2] = offsetZ;
			vertices[vertexIndex + 3].position[3] = 1.0f;
			vertices[vertexIndex + 3].texCoords[0] = self->characters[charEntryIndex].textureRight;
			vertices[vertexIndex + 3].texCoords[1] = self->characters[charEntryIndex].textureTop;
			
			indexes[indexIndex] = vertexIndex;
			indexes[indexIndex + 1] = vertexIndex + 1;
			indexes[indexIndex + 2] = vertexIndex + 2;
			indexes[indexIndex + 3] = vertexIndex + 2;
			indexes[indexIndex + 4] = vertexIndex + 3;
			indexes[indexIndex + 5] = vertexIndex;
			
			vertexIndex += 4;
			indexIndex += 6;
			positionX += self->characters[charEntryIndex].advance;
		}
	}
	
	if (GLGraphics_getOpenGLAPIVersion() == GL_API_VERSION_ES2) {
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p4f_t2f), vertices[0].position);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p4f_t2f), vertices[0].texCoords);
		
	} else {
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(4, GL_FLOAT, sizeof(struct vertex_p4f_t2f), vertices[0].position);
		glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_p4f_t2f), vertices[0].texCoords);
	}
	
	self->texture->activate(self->texture);
	glDrawElements(GL_TRIANGLES, indexIndex, GL_UNSIGNED_SHORT, indexes);
	self->texture->deactivate(self->texture);
	
	if (GLGraphics_getOpenGLAPIVersion() == GL_API_VERSION_ES2) {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	} else {
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	
	free(vertices);
	free(indexes);
}
