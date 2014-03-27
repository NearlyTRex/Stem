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

#include "gltexture/GLTextureAtlas.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

static void sharedInit(GLTextureAtlas * self) {
	call_super(init, self);
	
	self->textureName = NULL;
	self->private_ivar(textureNameOwned) = false;
	self->dispose = GLTextureAtlas_dispose;
	self->getKeys = GLTextureAtlas_getKeys;
	self->hasKey = GLTextureAtlas_hasKey;
	self->setEntry = GLTextureAtlas_setEntry;
	self->removeEntry = GLTextureAtlas_removeEntry;
	self->lookup = GLTextureAtlas_lookup;
}

GLTextureAtlas * GLTextureAtlas_create() {
	stemobject_create_implementation(GLTextureAtlas, init)
}

bool GLTextureAtlas_init(GLTextureAtlas * self) {
	sharedInit(self);
	self->private_ivar(hashTable) = hashCreate(sizeof(struct GLTextureAtlas_entry));
	return true;
}

void GLTextureAtlas_dispose(GLTextureAtlas * self) {
	if (self->private_ivar(textureNameOwned)) {
		free(self->textureName);
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
	
	hashTable = hashCreate(sizeof(struct GLTextureAtlas_entry));
	for (keyIndex = 0; keyIndex < keyCount; keyIndex++) {
		key = context->readNextDictionaryKey(context);
		context->beginStructure(context, key);
		entry.left = context->readFloat(context, "left");
		entry.right = context->readFloat(context, "right");
		entry.bottom = context->readFloat(context, "bottom");
		entry.top = context->readFloat(context, "top");
		context->endStructure(context);
		hashSetStruct(hashTable, key, struct GLTextureAtlas_entry, entry);
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
	context->writeString(context, "texture_name", self->textureName);
	context->beginDictionary(context, "entries");
	keys = hashGetKeys(self->private_ivar(hashTable), &keyCount);
	for (keyIndex = 0; keyIndex < keyCount; keyIndex++) {
		context->beginStructure(context, keys[keyIndex]);
		entry = hashGetStruct(self->private_ivar(hashTable), keys[keyIndex], struct GLTextureAtlas_entry);
		context->writeFloat(context, "left", entry.left);
		context->writeFloat(context, "right", entry.right);
		context->writeFloat(context, "bottom", entry.bottom);
		context->writeFloat(context, "top", entry.top);
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
}

const char ** GLTextureAtlas_getKeys(GLTextureAtlas * self, size_t * outCount) {
	return hashGetKeys(self->private_ivar(hashTable), outCount);
}

bool GLTextureAtlas_hasKey(GLTextureAtlas * self, const char * key) {
	return hashHas(self->private_ivar(hashTable), key);
}

void GLTextureAtlas_setEntry(GLTextureAtlas * self, const char * key, struct GLTextureAtlas_entry entry) {
	hashSetStruct(self->private_ivar(hashTable), key, struct GLTextureAtlas_entry, entry);
}

void GLTextureAtlas_removeEntry(GLTextureAtlas * self, const char * key) {
	hashDelete(self->private_ivar(hashTable), key);
}

struct GLTextureAtlas_entry GLTextureAtlas_lookup(GLTextureAtlas * self, const char * key) {
	return hashGetStruct(self->private_ivar(hashTable), key, struct GLTextureAtlas_entry);
}
