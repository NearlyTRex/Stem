#include "glbitmapfont/GLBitmapFont.h"
#include <string.h>

GLBitmapFont * GLBitmapFont_create(float emHeight, struct GLBitmapFont_charEntry characters[95]) {
	GLBitmapFont * self;
	
	self = malloc(sizeof(GLBitmapFont));
	GLBitmapFont_init(self, emHeight, characters);
	self->allocated = true;
	return self;
}

static void sharedInit(GLBitmapFont * self) {
	StemObject_init((StemObject *) self);
	
	self->textureName = NULL;
	self->textureNameAllocated = false;
	self->dispose = GLBitmapFont_dispose;
	self->measureString = GLBitmapFont_measureString;
	self->indexAtWidth = GLBitmapFont_indexAtWidth;
	self->drawString = GLBitmapFont_drawString;
}

void GLBitmapFont_init(GLBitmapFont * self, float emHeight, struct GLBitmapFont_charEntry characters[95]) {
	unsigned int charIndex;
	
	sharedInit(self);
	self->emHeight = emHeight;
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
	return NULL;
}

bool GLBitmapFont_loadSerializedData(GLBitmapFont * self, DeserializationContext * context) {
	size_t kernCharIndex;
	unsigned int charIndex;
	const char * key, * key2;
	const char * textureName;
	
	sharedInit(self);
	
	context->beginStructure(context, "glbitmapfont");
	textureName = context->readString(context, "texture_name");
	self->emHeight = context->readFloat(context, "em_height");
	context->beginDictionary(context, "characters");
	for (charIndex = 0; charIndex < 95; charIndex++) {
		key = context->readNextDictionaryKey(context);
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
			self->characters[key[0] - ' '].kernChars[kernCharIndex].previous = key2[0];
			self->characters[key[0] - ' '].kernChars[kernCharIndex].offset = context->readFloat(context, key2);
		}
		context->endDictionary(context);
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
	
	self->textureName = malloc(strlen(textureName) + 1);
	strcpy(self->textureName, textureName);
	self->textureNameAllocated = true;
	
	return true;
}

void GLBitmapFont_serialize(GLBitmapFont * self, SerializationContext * context) {
}

float GLBitmapFont_measureString(void * selfPtr, char * string) {
	return 0.0f;
}

unsigned int GLBitmapFont_indexAtWidth(void * selfPtr, char * string, float emWidth, bool * outLeadingEdge) {
	return 0;
}

void GLBitmapFont_drawString(void * selfPtr, char * string, float emHeight, float offsetX, float offsetY, float offsetZ) {
}
