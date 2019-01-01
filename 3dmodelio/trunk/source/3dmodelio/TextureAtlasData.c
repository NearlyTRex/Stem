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

#include "3dmodelio/TextureAtlasData.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

TextureAtlasData * TextureAtlasData_create(Atom name, Atom textureMapName, bool magnifyNearest, unsigned int entryCount, struct TextureAtlasData_entry * entries) {
	stemobject_create_implementation(TextureAtlasData, init, name, textureMapName, magnifyNearest, entryCount, entries)
}

static void sharedInit(TextureAtlasData * self) {
	call_super(init, self);
	self->dispose = TextureAtlasData_dispose;
}

bool TextureAtlasData_init(TextureAtlasData * self, Atom name, Atom textureMapName, bool magnifyNearest, unsigned int entryCount, struct TextureAtlasData_entry * entries) {
	sharedInit(self);
	self->name = name;
	self->textureMapName = textureMapName;
	self->magnifyNearest = magnifyNearest;
	self->entryCount = entryCount;
	self->entries = malloc(sizeof(*self->entries) * entryCount);
	memcpy(self->entries, entries, sizeof(*self->entries) * entryCount);
	return true;
}

void TextureAtlasData_dispose(TextureAtlasData * self) {
	free(self->entries);
	call_super(dispose, self);
}

TextureAtlasData * TextureAtlasData_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(TextureAtlasData, deserializationContext, loadSerializedData)
}

bool TextureAtlasData_loadSerializedData(TextureAtlasData * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * formatType, * nameString, * textureMapNameString, * entryName;
	struct TextureAtlasData_entry * entries;
	unsigned int entryCount, entryIndex;
	
	context->beginStructure(context, TEXTUREATLASDATA_FORMAT_TYPE);
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status != SERIALIZATION_ERROR_OK || formatVersion > TEXTUREATLASDATA_FORMAT_VERSION) {
		return false;
	}
	formatType = context->readString(context, "format_type");
	if (context->status != SERIALIZATION_ERROR_OK || strcmp(formatType, TEXTUREATLASDATA_FORMAT_TYPE)) {
		return false;
	}
	
	nameString = context->readStringNullable(context, "name");
	textureMapNameString = context->readStringNullable(context, "texture_name");
	self->magnifyNearest = context->readBoolean(context, "magnify_nearest");
	entryCount = context->beginDictionary(context, "entries");
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	entries = malloc(sizeof(*entries) * entryCount);
	for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
		entryName = context->readNextDictionaryKey(context);
		entries[entryIndex].name = Atom_fromString(entryName);
		context->beginStructure(context, entryName);
		entries[entryIndex].left = context->readFloat(context, "left");
		entries[entryIndex].right = context->readFloat(context, "right");
		entries[entryIndex].bottom = context->readFloat(context, "bottom");
		entries[entryIndex].top = context->readFloat(context, "top");
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		free(entries);
		return false;
	}
	
	sharedInit(self);
	self->name = Atom_fromString(nameString);
	self->textureMapName = Atom_fromString(textureMapNameString);
	self->entryCount = entryCount;
	self->entries = entries;
	
	return true;
}

void TextureAtlasData_serialize(TextureAtlasData * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	unsigned int entryIndex;
	
	context->beginStructure(context, TEXTUREATLASDATA_FORMAT_TYPE);
	context->writeUInt16(context, "format_version", TEXTUREATLASDATA_FORMAT_VERSION);
	context->writeString(context, "format_type", TEXTUREATLASDATA_FORMAT_TYPE);
	context->writeStringNullable(context, "name", self->name);
	context->writeStringNullable(context, "texture_name", self->textureMapName);
	context->writeBoolean(context, "magnify_nearest", self->magnifyNearest);
	context->beginDictionary(context, "entries");
	for (entryIndex = 0; entryIndex < self->entryCount; entryIndex++) {
		context->beginStructure(context, self->entries[entryIndex].name);
		context->writeFloat(context, "left", self->entries[entryIndex].left);
		context->writeFloat(context, "right", self->entries[entryIndex].right);
		context->writeFloat(context, "bottom", self->entries[entryIndex].bottom);
		context->writeFloat(context, "top", self->entries[entryIndex].top);
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
}

TextureAtlas * TextureAtlasData_createTextureAtlas(TextureAtlasData * self) {
	TextureAtlas * textureAtlas = TextureAtlas_create();
	unsigned int entryIndex;
	Rect4f entry;
	
	for (entryIndex = 0; entryIndex < self->entryCount; entryIndex++) {
		entry.left = self->entries[entryIndex].left;
		entry.right = self->entries[entryIndex].right;
		entry.bottom = self->entries[entryIndex].bottom;
		entry.top = self->entries[entryIndex].top;
		TextureAtlas_setEntry(textureAtlas, self->entries[entryIndex].name, entry);
	}
	return textureAtlas;
}

