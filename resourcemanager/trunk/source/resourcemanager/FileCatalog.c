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

#include "resourcemanager/FileCatalog.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

FileCatalog * FileCatalog_create(const char * basePath) {
	stemobject_create_implementation(FileCatalog, init, basePath)
}

static void sharedInit(FileCatalog * self, const char * basePath) {
	call_super(init, self);
	self->dispose = FileCatalog_dispose;
	self->basePath = strdup(basePath);
}

bool FileCatalog_init(FileCatalog * self, const char * basePath) {
	sharedInit(self, basePath);
	self->private_ivar(catalog) = hashCreate();
	return true;
}

void FileCatalog_dispose(FileCatalog * self) {
	free(self->basePath);
	hashDispose(self->private_ivar(catalog));
	call_super(dispose, self);
}

void FileCatalog_setFilePath(FileCatalog * self, Atom type, Atom name, const char * path) {
	DataValue * typeHashData;
	
	if (!hashHas(self->private_ivar(catalog), type)) {
		hashSet(self->private_ivar(catalog), type, valueCreateHashTable(hashCreate(), true, false));
	}
	typeHashData = hashGet(self->private_ivar(catalog), type);
	hashSet(typeHashData->value.hashTable, name, valueCreateString(path, DATA_USE_STRLEN, true, true));
}

const char * FileCatalog_getFilePath(FileCatalog * self, Atom type, Atom name) {
	DataValue * typeHashData, * nameData;
	
	typeHashData = hashGet(self->private_ivar(catalog), type);
	if (typeHashData == NULL) {
		return NULL;
	}
	nameData = hashGet(typeHashData->value.hashTable, name);
	if (nameData == NULL) {
		return NULL;
	}
	return nameData->value.string;
}

const char ** FileCatalog_listTypes(FileCatalog * self, size_t * outCount) {
	return hashGetKeys(self->private_ivar(catalog), outCount);
}

const char ** FileCatalog_listNamesForType(FileCatalog * self, Atom type, size_t * outCount) {
	if (!hashHas(self->private_ivar(catalog), type)) {
		*outCount = 0;
		return NULL;
	}
	return hashGetKeys(hashGet(self->private_ivar(catalog), type)->value.hashTable, outCount);
}

FileCatalog * FileCatalog_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(FileCatalog, deserializationContext, loadSerializedData)
}

bool FileCatalog_loadSerializedData(FileCatalog * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * formatType, * basePathString, * typeString, * nameString, * pathString;
	size_t typeCount, typeIndex, nameCount, nameIndex;
	HashTable * catalog, * typeHash;
	
	context->beginStructure(context, FILECATALOG_FORMAT_TYPE);
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status != SERIALIZATION_ERROR_OK || formatVersion > FILECATALOG_FORMAT_VERSION) {
		return false;
	}
	formatType = context->readString(context, "format_type");
	if (context->status != SERIALIZATION_ERROR_OK || strcmp(formatType, FILECATALOG_FORMAT_TYPE)) {
		return false;
	}
	
	basePathString = context->readString(context, "base_path");
	catalog = hashCreate();
	typeCount = context->beginDictionary(context, "types");
	for (typeIndex = 0; typeIndex < typeCount; typeIndex++) {
		typeString = context->readNextDictionaryKey(context);
		if (context->status != SERIALIZATION_ERROR_OK) {
			hashDispose(catalog);
			return false;
		}
		if (hashHas(catalog, typeString)) {
			typeHash = hashGet(catalog, typeString)->value.hashTable;
		} else {
			typeHash = hashCreate();
			hashSet(catalog, typeString, valueCreateHashTable(typeHash, true, false));
		}
		
		nameCount = context->beginDictionary(context, typeString);
		for (nameIndex = 0; nameIndex < nameCount; nameIndex++) {
			nameString = context->readNextDictionaryKey(context);
			pathString = context->readString(context, nameString);
			if (context->status != SERIALIZATION_ERROR_OK) {
				hashDispose(catalog);
				return false;
			}
			hashSet(typeHash, nameString, valueCreateString(pathString, DATA_USE_STRLEN, true, true));
		}
		context->endDictionary(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		hashDispose(catalog);
		return false;
	}
	
	sharedInit(self, basePathString);
	self->private_ivar(catalog) = catalog;
	
	return true;
}

void FileCatalog_serialize(FileCatalog * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	size_t typeCount = 0, typeIndex, nameCount = 0, nameIndex;
	const char ** types, ** names;
	HashTable * typeHash;
	
	context->beginStructure(context, FILECATALOG_FORMAT_TYPE);
	context->writeUInt16(context, "format_version", FILECATALOG_FORMAT_VERSION);
	context->writeString(context, "format_type", FILECATALOG_FORMAT_TYPE);
	context->writeString(context, "base_path", self->basePath);
	context->beginDictionary(context, "types");
	types = hashGetKeys(self->private_ivar(catalog), &typeCount);
	for (typeIndex = 0; typeIndex < typeCount; typeIndex++) {
		typeHash = hashGet(self->private_ivar(catalog), types[typeIndex])->value.hashTable;
		context->beginDictionary(context, types[typeIndex]);
		names = hashGetKeys(typeHash, &nameCount);
		for (nameIndex = 0; nameIndex < nameCount; nameIndex++) {
			context->writeString(context, names[nameIndex], hashGet(typeHash, names[nameIndex])->value.string);
		}
		context->endDictionary(context);
	}
	free(types);
	context->endDictionary(context);
	context->endStructure(context);
}
