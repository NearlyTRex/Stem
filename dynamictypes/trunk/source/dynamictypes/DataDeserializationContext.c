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

#define SUPERCLASS DeserializationContext

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
	self->finished = false;
	return true;
}

void DataDeserializationContext_dispose(DataDeserializationContext * self) {
	call_super(dispose, self);
}

#define failWithStatus(STATUS, RETURN_CODE) \
	self->status = (STATUS); \
	if (self->jmpBuf != NULL) { \
		longjmp(*self->jmpBuf, self->status); \
	} \
	RETURN_CODE

static DataValue * readValue(DataDeserializationContext * self, const char * key) {
	DataValue * value;
	
	if (self->currentValue == NULL) {
		failWithStatus(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, return NULL);
	}
	
	switch (self->currentValue->type) {
		case DATA_TYPE_ARRAY:
			if (self->index >= self->currentValue->value.array->count) {
				failWithStatus(SERIALIZATION_ERROR_END_OF_CONTAINER, return NULL);
			}
			return arrayGet(self->currentValue->value.array, self->index++);
			
		case DATA_TYPE_ASSOCIATIVE_ARRAY:
			if (key == NULL) {
				failWithStatus(SERIALIZATION_ERROR_NULL_KEY, return NULL);
			}
			value = associativeArrayGetFirstValueForKey(self->currentValue->value.associativeArray, key);
			if (value == NULL) {
				failWithStatus(SERIALIZATION_ERROR_KEY_NOT_FOUND, );
			}
			return value;
			
		case DATA_TYPE_HASH_TABLE:
			if (key == NULL) {
				failWithStatus(SERIALIZATION_ERROR_NULL_KEY, return NULL);
			}
			value = hashGet(self->currentValue->value.hashTable, key);
			if (value == NULL) {
				failWithStatus(SERIALIZATION_ERROR_KEY_NOT_FOUND, );
			}
			return value;
			
		default:
			return NULL;
	}
}

static bool pushContainer(DataDeserializationContext * self, const char * key, enum DataValueType expectedType) {
	if (self->currentValue == NULL) {
		if (self->finished) {
			failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return false);
		}
		if (self->rootValue.type != expectedType) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return false);
		}
		self->currentValue = &self->rootValue;
		
	} else {
		DataValue * container;
		
		container = readValue(self, key);
		if (container->type != expectedType) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return false);
		}
		self->stack = realloc(self->stack, sizeof(DataValue *) * (self->stackCount + 1));
		self->stack[self->stackCount].container = self->currentValue;
		self->stack[self->stackCount].index = self->index;
		self->stackCount++;
		self->currentValue = container;
	}
	self->index = 0;
	return true;
}

void DataDeserializationContext_beginStructure(DataDeserializationContext * self, const char * key) {
	pushContainer(self, key, DATA_TYPE_HASH_TABLE);
}

size_t DataDeserializationContext_beginDictionary(DataDeserializationContext * self, const char * key) {
	if (!pushContainer(self, key, DATA_TYPE_ASSOCIATIVE_ARRAY)) {
		return 0;
	}
	return self->currentValue->value.associativeArray->count;
}

size_t DataDeserializationContext_beginArray(DataDeserializationContext * self, const char * key) {
	if (!pushContainer(self, key, DATA_TYPE_ARRAY)) {
		return 0;
	}
	return self->currentValue->value.array->count;
}

static void popContainer(DataDeserializationContext * self, enum DataValueType type) {
	if (self->currentValue == NULL) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, return);
	}
	if (self->currentValue->type != type) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return);
	}
	if (self->stackCount > 0) {
		self->stackCount--;
		self->currentValue = self->stack[self->stackCount].container;
		self->index = self->stack[self->stackCount].index;
		
	} else {
		self->currentValue = NULL;
		self->finished = true;
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

#define readValueImplementation(FIELD, DEFAULT_VALUE, EXPECTED_TYPE) \
	DataValue * value = readValue(self, key); \
	if (value == NULL) { \
		return DEFAULT_VALUE; \
	} \
	if (value->type != EXPECTED_TYPE) { \
		failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return DEFAULT_VALUE); \
	} \
	return value->value.FIELD

bool DataDeserializationContext_readBoolean(DataDeserializationContext * self, const char * key) {
	readValueImplementation(boolean, false, DATA_TYPE_BOOLEAN);
}

int8_t DataDeserializationContext_readInt8(DataDeserializationContext * self, const char * key) {
	readValueImplementation(int8, 0, DATA_TYPE_INT8);
}

uint8_t DataDeserializationContext_readUInt8(DataDeserializationContext * self, const char * key) {
	readValueImplementation(uint8, 0, DATA_TYPE_UINT8);
}

int16_t DataDeserializationContext_readInt16(DataDeserializationContext * self, const char * key) {
	readValueImplementation(int16, 0, DATA_TYPE_INT16);
}

uint16_t DataDeserializationContext_readUInt16(DataDeserializationContext * self, const char * key) {
	readValueImplementation(uint16, 0, DATA_TYPE_UINT16);
}

int32_t DataDeserializationContext_readInt32(DataDeserializationContext * self, const char * key) {
	readValueImplementation(int32, 0, DATA_TYPE_INT32);
}

uint32_t DataDeserializationContext_readUInt32(DataDeserializationContext * self, const char * key) {
	readValueImplementation(uint32, 0, DATA_TYPE_UINT32);
}

int64_t DataDeserializationContext_readInt64(DataDeserializationContext * self, const char * key) {
	readValueImplementation(int64, 0, DATA_TYPE_INT64);
}

uint64_t DataDeserializationContext_readUInt64(DataDeserializationContext * self, const char * key) {
	readValueImplementation(uint64, 0, DATA_TYPE_UINT64);
}

float DataDeserializationContext_readFloat(DataDeserializationContext * self, const char * key) {
	readValueImplementation(float32, 0.0f, DATA_TYPE_FLOAT);
}

double DataDeserializationContext_readDouble(DataDeserializationContext * self, const char * key) {
	readValueImplementation(float64, 0.0, DATA_TYPE_DOUBLE);
}

int DataDeserializationContext_readEnumeration(DataDeserializationContext * self, const char * key, ...) {
	va_list args;
	DataValue * value;
	int status;
	
	value = readValue(self, key);
	if (value == NULL) {
		return 0;
	}
	if (value->type != DATA_TYPE_INT32) {
		failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0);
	}
	
	va_start(args, key);
	status = Serialization_checkEnumerationErrors(value->value.int32, args);
	va_end(args);
	
	if (status != SERIALIZATION_ERROR_OK) {
		failWithStatus(status, return 0);
	}
	
	return value->value.int32;
}

#define readBitfieldImplementation(BIT_COUNT) \
	va_list args; \
	DataValue * value; \
	int status; \
	\
	value = readValue(self, key); \
	if (value == NULL) { \
		return 0; \
	} \
	if (value->type != DATA_TYPE_UINT##BIT_COUNT) { \
		failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0); \
	} \
	\
	va_start(args, key); \
	status = Serialization_checkBitfield##BIT_COUNT##Errors(value->value.uint##BIT_COUNT, args); \
	va_end(args); \
	\
	if (status != SERIALIZATION_ERROR_OK) { \
		failWithStatus(status, return 0); \
	} \
	\
	return value->value.uint##BIT_COUNT

uint8_t DataDeserializationContext_readBitfield8(DataDeserializationContext * self, const char * key, ...) {
	readBitfieldImplementation(8);
}

uint16_t DataDeserializationContext_readBitfield16(DataDeserializationContext * self, const char * key, ...) {
	readBitfieldImplementation(16);
}

uint32_t DataDeserializationContext_readBitfield32(DataDeserializationContext * self, const char * key, ...) {
	readBitfieldImplementation(32);
}

uint64_t DataDeserializationContext_readBitfield64(DataDeserializationContext * self, const char * key, ...) {
	readBitfieldImplementation(64);
}

const char * DataDeserializationContext_readString(DataDeserializationContext * self, const char * key) {
	readValueImplementation(string, NULL, DATA_TYPE_STRING);
}

const void * DataDeserializationContext_readBlob(DataDeserializationContext * self, const char * key, size_t * outLength) {
	DataValue * value;
	
	value = readValue(self, key);
	if (value == NULL) {
		return NULL;
	}
	if (value->type != DATA_TYPE_BLOB) {
		failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return NULL);
	}
	*outLength = value->value.blob.length;
	return value->value.blob.bytes;
}

const char * DataDeserializationContext_readNextDictionaryKey(DataDeserializationContext * self) {
	if (self->currentValue->type != DATA_TYPE_ASSOCIATIVE_ARRAY) {
		failWithStatus(SERIALIZATION_ERROR_INVALID_OPERATION, return NULL);
	}
	if (self->index >= self->currentValue->value.associativeArray->count) {
		failWithStatus(SERIALIZATION_ERROR_END_OF_CONTAINER, return NULL);
	}
	return associativeArrayGetKeyAtIndex(self->currentValue->value.associativeArray, self->index++);
}

bool DataDeserializationContext_hasDictionaryKey(DataDeserializationContext * self, const char * key) {
	if (self->currentValue->type != DATA_TYPE_ASSOCIATIVE_ARRAY) {
		failWithStatus(SERIALIZATION_ERROR_INVALID_OPERATION, return false);
	}
	return associativeArrayGetFirstIndexOfKey(self->currentValue->value.associativeArray, key) != ASSOCIATIVE_ARRAY_NOT_FOUND;
}
