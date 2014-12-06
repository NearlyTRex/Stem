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

#include "dynamictypes/AssociativeArray.h"
#include "dynamictypes/DataArray.h"
#include "dynamictypes/DataDeserializationContext.h"
#include "dynamictypes/HashTable.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

DataDeserializationContext * DataDeserializationContext_create(DataValue rootValue) {
	stemobject_create_implementation(DataDeserializationContext, init, rootValue)
}

bool DataDeserializationContext_init(DataDeserializationContext * self, DataValue rootValue) {
	call_super(init, self);
	self->dispose = DataDeserializationContext_dispose;
	self->beginStructure = DataDeserializationContext_beginStructure;
	self->beginDictionary = DataDeserializationContext_beginDictionary;
	self->beginArray = DataDeserializationContext_beginArray;
	self->endStructure = DataDeserializationContext_endStructure;
	self->endDictionary = DataDeserializationContext_endDictionary;
	self->endArray = DataDeserializationContext_endArray;
	self->readBoolean = DataDeserializationContext_readBoolean;
	self->readInt8 = DataDeserializationContext_readInt8;
	self->readUInt8 = DataDeserializationContext_readUInt8;
	self->readInt16 = DataDeserializationContext_readInt16;
	self->readUInt16 = DataDeserializationContext_readUInt16;
	self->readInt32 = DataDeserializationContext_readInt32;
	self->readUInt32 = DataDeserializationContext_readUInt32;
	self->readInt64 = DataDeserializationContext_readInt64;
	self->readUInt64 = DataDeserializationContext_readUInt64;
	self->readFloat = DataDeserializationContext_readFloat;
	self->readDouble = DataDeserializationContext_readDouble;
	self->readEnumeration = DataDeserializationContext_readEnumeration;
	self->readBitfield8 = DataDeserializationContext_readBitfield8;
	self->readBitfield16 = DataDeserializationContext_readBitfield16;
	self->readBitfield32 = DataDeserializationContext_readBitfield32;
	self->readBitfield64 = DataDeserializationContext_readBitfield64;
	self->readString = DataDeserializationContext_readString;
	self->readBlob = DataDeserializationContext_readBlob;
	self->readNextDictionaryKey = DataDeserializationContext_readNextDictionaryKey;
	self->hasDictionaryKey = DataDeserializationContext_hasDictionaryKey;
	self->status = SERIALIZATION_ERROR_OK;
	self->rootValue = rootValue;
	self->currentValue = NULL;
	self->stackCount = 0;
	self->stack = NULL;
	return true;
}

void DataDeserializationContext_dispose(DataDeserializationContext * self) {
	call_super(dispose, self);
}

static DataValue * readNextValueInContainer(DataDeserializationContext * self, const char * key) {
	switch (self->currentValue->type) {
		case DATA_TYPE_ARRAY:
			return arrayGet(self->currentValue->value.array, self->index++);
			
		case DATA_TYPE_ASSOCIATIVE_ARRAY:
			return associativeArrayGetValueAtIndex(self->currentValue->value.associativeArray, self->index++);
			
		case DATA_TYPE_HASH_TABLE:
			return hashGet(self->currentValue->value.hashTable, key);
			
		default:
			return NULL;
	}
}

static void pushContainer(DataDeserializationContext * self, DataValue * container) {
	self->stack = realloc(self->stack, sizeof(DataValue *) * (self->stackCount + 1));
	self->stack[self->stackCount].container = self->currentValue;
	self->stack[self->stackCount].index = self->index;
	self->stackCount++;
	self->currentValue = container;
	self->index = 0;
}

void DataDeserializationContext_beginStructure(DataDeserializationContext * self, const char * key) {
	if (self->currentValue == NULL) {
		self->currentValue = &self->rootValue;
		self->index = 0;
	} else {
		pushContainer(self, readNextValueInContainer(self, key));
	}
}

size_t DataDeserializationContext_beginDictionary(DataDeserializationContext * self, const char * key) {
	if (self->currentValue == NULL) {
		self->currentValue = &self->rootValue;
		self->index = 0;
	} else {
		pushContainer(self, readNextValueInContainer(self, key));
	}
	return self->currentValue->value.associativeArray->count;
}

size_t DataDeserializationContext_beginArray(DataDeserializationContext * self, const char * key) {
	if (self->currentValue == NULL) {
		self->currentValue = &self->rootValue;
		self->index = 0;
	} else {
		pushContainer(self, readNextValueInContainer(self, key));
	}
	return self->currentValue->value.array->count;
}

static void popContainer(DataDeserializationContext * self, enum DataValueType type) {
	if (self->stackCount > 0) {
		self->stackCount--;
		self->currentValue = self->stack[self->stackCount].container;
		self->index = self->stack[self->stackCount].index;
	}
}

void DataDeserializationContext_endStructure(DataDeserializationContext * self) {
	popContainer(self, DATA_TYPE_HASH_TABLE);
}

void DataDeserializationContext_endDictionary(DataDeserializationContext * self) {
	popContainer(self, DATA_TYPE_ASSOCIATIVE_ARRAY);
}

void DataDeserializationContext_endArray(DataDeserializationContext * self) {
	popContainer(self, DATA_TYPE_ARRAY);
}

bool DataDeserializationContext_readBoolean(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.boolean;
}

int8_t DataDeserializationContext_readInt8(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.int8;
}

uint8_t DataDeserializationContext_readUInt8(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.uint8;
}

int16_t DataDeserializationContext_readInt16(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.int16;
}

uint16_t DataDeserializationContext_readUInt16(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.uint16;
}

int32_t DataDeserializationContext_readInt32(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.int32;
}

uint32_t DataDeserializationContext_readUInt32(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.uint32;
}

int64_t DataDeserializationContext_readInt64(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.int64;
}

uint64_t DataDeserializationContext_readUInt64(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.uint64;
}

float DataDeserializationContext_readFloat(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.float32;
}

double DataDeserializationContext_readDouble(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.float64;
}

int DataDeserializationContext_readEnumeration(DataDeserializationContext * self, const char * key, ...) {
	return readNextValueInContainer(self, key)->value.int32;
}

uint8_t DataDeserializationContext_readBitfield8(DataDeserializationContext * self, const char * key, ...) {
	return readNextValueInContainer(self, key)->value.uint8;
}

uint16_t DataDeserializationContext_readBitfield16(DataDeserializationContext * self, const char * key, ...) {
	return readNextValueInContainer(self, key)->value.uint16;
}

uint32_t DataDeserializationContext_readBitfield32(DataDeserializationContext * self, const char * key, ...) {
	return readNextValueInContainer(self, key)->value.uint32;
}

uint64_t DataDeserializationContext_readBitfield64(DataDeserializationContext * self, const char * key, ...) {
	return readNextValueInContainer(self, key)->value.uint64;
}

const char * DataDeserializationContext_readString(DataDeserializationContext * self, const char * key) {
	return readNextValueInContainer(self, key)->value.string.bytes;
}

const void * DataDeserializationContext_readBlob(DataDeserializationContext * self, const char * key, size_t * outLength) {
	DataValue * value;
	
	value = readNextValueInContainer(self, key);
	*outLength = value->value.blob.length;
	return value->value.blob.bytes;
}

const char * DataDeserializationContext_readNextDictionaryKey(DataDeserializationContext * self) {
	return associativeArrayGetKeyAtIndex(self->currentValue->value.associativeArray, self->index++);
}

bool DataDeserializationContext_hasDictionaryKey(DataDeserializationContext * self, const char * key) {
	return associativeArrayGetFirstIndexOfKey(self->currentValue->value.associativeArray, key) != ASSOCIATIVE_ARRAY_NOT_FOUND;
}
