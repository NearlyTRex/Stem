#include "glbitmapfont/GLBitmapFont.h"
#include <string.h>

GLBitmapFont * GLBitmapFont_create(struct GLBitmapFont_charEntry characters[95]) {
	GLBitmapFont * self;
	
	self = malloc(sizeof(GLBitmapFont));
	GLBitmapFont_init(self, characters);
	self->allocated = true;
	return self;
}

static void sharedInit(GLBitmapFont * self) {
	StemObject_init((StemObject *) self);
	
	self->textureName = NULL;
	self->textureNameAllocated = false;
	self->texture = NULL;
	self->dispose = GLBitmapFont_dispose;
	self->measureString = GLBitmapFont_measureString;
	self->indexAtWidth = GLBitmapFont_indexAtWidth;
	self->drawString = GLBitmapFont_drawString;
}

void GLBitmapFont_init(GLBitmapFont * self, struct GLBitmapFont_charEntry characters[95]) {
	unsigned int charIndex;
	
	sharedInit(self);
	memcpy(self->characters, characters, sizeof(struct GLBitmapFont_charEntry) * 95);
	for (charIndex = 0; charIndex < 95; charIndex++) {
		if (characters[charIndex].kernChars != NULL) {
			self->characters[charIndex].kernChars = malloc(sizeof(struct GLBitmapFont_kernEntry) * characters[charIndex].kernCharCount);
			memcpy(self->characters[charIndex].kernChars, characters[charIndex].kernChars, sizeof(struct GLBitmapFont_kernEntry) * characters[charIndex].kernCharCount);
		}
	}
}

void GLBitmapFont_dispose(void * selfPtr) {
	GLBitmapFont * self = selfPtr;
	unsigned int charIndex;
	
	if (self->textureNameAllocated) {
		free(self->textureName);
	}
	// TODO: Dispose texture?
	for (charIndex = 0; charIndex < 95; charIndex++) {
		free(self->characters[charIndex].kernChars);
	}
	GLFont_dispose(selfPtr);
}

GLBitmapFont * GLBitmapFont_deserialize(DeserializationContext * context) {
	GLBitmapFont * self;
	bool success;
	
	self = malloc(sizeof(GLBitmapFont));
	success = GLBitmapFont_loadSerializedData(self, context);
	self->allocated = true;
	if (!success) {
		return NULL;
	}
	return self;
}

bool GLBitmapFont_loadSerializedData(GLBitmapFont * self, DeserializationContext * context) {
	size_t kernCharIndex, kernCharIndex2, nchars;
	unsigned int charIndex;
	const char * key, * key2;
	const char * textureName;
	
	sharedInit(self);
	memset(self->characters, 0x00, sizeof(self->characters));
	
	context->beginStructure(context, "glbitmapfont");
	textureName = context->readString(context, "texture_name");
	nchars = context->beginDictionary(context, "characters");
	if (nchars != 95) {
		self->dispose(self);
		return false;
	}
	for (charIndex = 0; charIndex < 95; charIndex++) {
		key = context->readNextDictionaryKey(context);
		if (key[0] < ' ' || key[0] > '~' || strlen(key) > 1) {
			self->dispose(self);
			return false;
		}
		context->beginStructure(context, key);
		self->characters[key[0] - ' '].preadvance = context->readFloat(context, "preadvance");
		self->characters[key[0] - ' '].advance = context->readFloat(context, "advance");
		context->beginArray(context, "texture_bounds");
		self->characters[key[0] - ' '].textureLeft = context->readFloat(context, NULL);
		self->characters[key[0] - ' '].textureRight = context->readFloat(context, NULL);
		self->characters[key[0] - ' '].textureBottom = context->readFloat(context, NULL);
		self->characters[key[0] - ' '].textureTop = context->readFloat(context, NULL);
		context->endArray(context);
		
		self->characters[key[0] - ' '].kernCharCount = context->beginDictionary(context, "kerning_table");
		if (self->characters[key[0] - ' '].kernCharCount == 0) {
			self->characters[key[0] - ' '].kernChars = NULL;
		} else {
			self->characters[key[0] - ' '].kernChars = malloc(sizeof(struct GLBitmapFont_kernEntry) * self->characters[key[0] - ' '].kernCharCount);
		}
		
		for (kernCharIndex = 0; kernCharIndex < self->characters[key[0] - ' '].kernCharCount; kernCharIndex++) {
			key2 = context->readNextDictionaryKey(context);
			for (kernCharIndex2 = 0; kernCharIndex2 < kernCharIndex; kernCharIndex2++) {
				if (self->characters[key[0] - ' '].kernChars[kernCharIndex2].previous == key2[0]) {
					self->dispose(self);
					return false;
				}
			}
			self->characters[key[0] - ' '].kernChars[kernCharIndex].previous = key2[0];
			self->characters[key[0] - ' '].kernChars[kernCharIndex].offset = context->readFloat(context, key2);
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
	self->textureNameAllocated = true;
	
	return true;
}

void GLBitmapFont_serialize(GLBitmapFont * self, SerializationContext * context) {
	unsigned int charIndex, kernCharIndex;
	char charString[2] = {0, 0};
	
	context->beginStructure(context, "glbitmapfont");
	context->writeString(context, "texture_name", self->textureName);
	context->beginDictionary(context, "characters");
	for (charIndex = 0; charIndex < 95; charIndex++) {
		charString[0] = ' ' + charIndex;
		context->beginStructure(context, charString);
		context->writeFloat(context, "preadvance", self->characters[charIndex].preadvance);
		context->writeFloat(context, "advance", self->characters[charIndex].advance);
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

float GLBitmapFont_measureString(void * selfPtr, char * string, size_t length) {
	GLBitmapFont * self = selfPtr;
	float width = 0.0f;
	unsigned int charIndex, kernCharIndex;
	
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (string[charIndex] >= ' ' && string[charIndex] <= '~') {
			width += self->characters[string[charIndex] - ' '].advance;
			if (charIndex > 0 && string[charIndex - 1] >= ' ' && string[charIndex - 1] <= '~') {
				for (kernCharIndex = 0; kernCharIndex < self->characters[string[charIndex] - ' '].kernCharCount; kernCharIndex++) {
					if (string[charIndex - 1] == self->characters[string[charIndex] - ' '].kernChars[kernCharIndex].previous) {
						width += self->characters[string[charIndex] - ' '].kernChars[kernCharIndex].offset;
						break;
					}
				}
			}
		}
	}
	return width;
}

size_t GLBitmapFont_indexAtWidth(void * selfPtr, char * string, size_t length, float emWidth, bool * outLeadingEdge) {
	GLBitmapFont * self = selfPtr;
	float totalWidth = 0.0f, charWidth, halfKernOffset = 0.0f;
	unsigned int charIndex, kernCharIndex;
	
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (string[charIndex] >= ' ' && string[charIndex] <= '~') {
			charWidth = self->characters[string[charIndex] - ' '].advance + halfKernOffset;
			halfKernOffset = 0.0f;
			if (charIndex < length - 1 && string[charIndex + 1] >= ' ' && string[charIndex + 1] <= '~') {
				for (kernCharIndex = 0; kernCharIndex < self->characters[string[charIndex + 1] - ' '].kernCharCount; kernCharIndex++) {
					if (self->characters[string[charIndex + 1] - ' '].kernChars[kernCharIndex].previous == string[charIndex]) {
						halfKernOffset = self->characters[string[charIndex + 1] - ' '].kernChars[kernCharIndex].offset * 0.5f;
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

void GLBitmapFont_drawString(void * selfPtr, char * string, size_t length, float emHeight, float offsetX, float offsetY, float offsetZ) {
}
