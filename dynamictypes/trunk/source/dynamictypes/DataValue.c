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
#include "dynamictypes/DataValue.h"
#include "dynamictypes/HashTable.h"
#include <string.h>

DataValue valueCreateBoolean(bool value) {
	return (DataValue) {{.boolean = value}, DATA_TYPE_BOOLEAN, false};
}

DataValue valueCreateInt8(int8_t value) {
	return (DataValue) {{.int8 = value}, DATA_TYPE_INT8, false};
}

DataValue valueCreateUInt8(uint8_t value) {
	return (DataValue) {{.uint8 = value}, DATA_TYPE_UINT8, false};
}

DataValue valueCreateInt16(int16_t value) {
	return (DataValue) {{.int16 = value}, DATA_TYPE_INT16, false};
}

DataValue valueCreateUInt16(uint16_t value) {
	return (DataValue) {{.uint16 = value}, DATA_TYPE_UINT16, false};
}

DataValue valueCreateInt32(int32_t value) {
	return (DataValue) {{.int32 = value}, DATA_TYPE_INT32, false};
}

DataValue valueCreateUInt32(uint32_t value) {
	return (DataValue) {{.uint32 = value}, DATA_TYPE_UINT32, false};
}

DataValue valueCreateInt64(int64_t value) {
	return (DataValue) {{.int64 = value}, DATA_TYPE_INT64, false};
}

DataValue valueCreateUInt64(uint64_t value) {
	return (DataValue) {{.uint64 = value}, DATA_TYPE_UINT64, false};
}

DataValue valueCreateFloat(float value) {
	return (DataValue) {{.float32 = value}, DATA_TYPE_FLOAT, false};
}

DataValue valueCreateDouble(double value) {
	return (DataValue) {{.float64 = value}, DATA_TYPE_DOUBLE, false};
}

DataValue valueCreatePointer(void * value) {
	return (DataValue) {{.pointer = value}, DATA_TYPE_POINTER, false};
}

#if defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
static char * strndup(const char * s1, size_t n) {
	size_t len = 0;
	char * s2;
	
	while (len < n && s1[len] != 0) {
		len++;
	}
	s2 = malloc(len + 1);
	memcpy(s2, s1, len);
	s2[len] = 0;
	return s2;
}
#endif

DataValue valueCreateString(const char * value, size_t length, bool takeOwnership, bool copy) {
	DataValue result;
	
	result.type = DATA_TYPE_STRING;
	if (length == DATA_USE_STRLEN) {
		length = strlen(value);
	}
	if (copy) {
		result.value.string = strndup(value, length);
	} else {
		result.value.string = value;
	}
	result.owned = takeOwnership;
	return result;
}

DataValue valueCreateBlob(const void * value, size_t length, bool takeOwnership, bool copy) {
	DataValue result;
	
	result.type = DATA_TYPE_BLOB;
	if (copy) {
		result.value.blob.bytes = malloc(length);
		memcpy((void *) result.value.blob.bytes, value, length);
	} else {
		result.value.blob.bytes = value;
	}
	result.value.blob.length = length;
	result.owned = takeOwnership;
	return result;
}

DataValue valueCreateHashTable(HashTable * value, bool takeOwnership, bool copy) {
	DataValue result;
	
	result.type = DATA_TYPE_HASH_TABLE;
	if (copy) {
		result.value.hashTable = hashCopy(value);
	} else {
		result.value.hashTable = value;
	}
	result.owned = takeOwnership;
	return result;
}

DataValue valueCreateArray(DataArray * value, bool takeOwnership, bool copy) {
	DataValue result;
	
	result.type = DATA_TYPE_ARRAY;
	if (copy) {
		result.value.array = arrayCopy(value);
	} else {
		result.value.array = value;
	}
	result.owned = takeOwnership;
	return result;
}

DataValue valueCreateAssociativeArray(struct AssociativeArray * value, bool takeOwnership, bool copy) {
	DataValue result;
	
	result.type = DATA_TYPE_ASSOCIATIVE_ARRAY;
	if (copy) {
		result.value.associativeArray = associativeArrayCopy(value);
	} else {
		result.value.associativeArray = value;
	}
	result.owned = takeOwnership;
	return result;
}

DataValue valueCopy(DataValue * value) {
	DataValue copy = *value;
	
	if (copy.owned) {
		switch (copy.type) {
			case DATA_TYPE_STRING:
				copy.value.string = strdup(copy.value.string);
				break;
				
			case DATA_TYPE_BLOB: {
				void * bytes;
				
				bytes = malloc(copy.value.blob.length);
				memcpy(bytes, copy.value.blob.bytes, copy.value.blob.length);
				copy.value.blob.bytes = bytes;
				break;
			}
			case DATA_TYPE_HASH_TABLE:
				copy.value.hashTable = hashCopy(copy.value.hashTable);
				break;
				
			case DATA_TYPE_ARRAY:
				copy.value.array = arrayCopy(copy.value.array);
				break;
				
			case DATA_TYPE_ASSOCIATIVE_ARRAY:
				copy.value.associativeArray = associativeArrayCopy(copy.value.associativeArray);
				break;
				
			default:
				break;
		}
	}
	return copy;
}

void valueDispose(DataValue * value) {
}

#define returnNumericValue(value, defaultValue) \
	switch (value->type) { \
		case DATA_TYPE_BOOLEAN: \
			return value->value.boolean; \
		case DATA_TYPE_INT8: \
			return value->value.int8; \
		case DATA_TYPE_UINT8: \
			return value->value.uint8; \
		case DATA_TYPE_INT16: \
			return value->value.int16; \
		case DATA_TYPE_UINT16: \
			return value->value.uint16; \
		case DATA_TYPE_INT32: \
			return value->value.int32; \
		case DATA_TYPE_UINT32: \
			return value->value.uint32; \
		case DATA_TYPE_INT64: \
			return value->value.int64; \
		case DATA_TYPE_UINT64: \
			return value->value.uint64; \
		case DATA_TYPE_FLOAT: \
			return value->value.float32; \
		case DATA_TYPE_DOUBLE: \
			return value->value.float64; \
		default: \
			return defaultValue; \
	}

bool valueGetBoolean(DataValue * value) {
	returnNumericValue(value, false);
}

int8_t valueGetInt8(DataValue * value) {
	returnNumericValue(value, 0);
}

uint8_t valueGetUInt8(DataValue * value) {
	returnNumericValue(value, 0);
}

int16_t valueGetInt16(DataValue * value) {
	returnNumericValue(value, 0);
}

uint16_t valueGetUInt16(DataValue * value) {
	returnNumericValue(value, 0);
}

int32_t valueGetInt32(DataValue * value) {
	returnNumericValue(value, 0);
}

uint32_t valueGetUInt32(DataValue * value) {
	returnNumericValue(value, 0);
}

int64_t valueGetInt64(DataValue * value) {
	returnNumericValue(value, 0);
}

uint64_t valueGetUInt64(DataValue * value) {
	returnNumericValue(value, 0);
}

float valueGetFloat(DataValue * value) {
	returnNumericValue(value, 0.0f);
}

double valueGetDouble(DataValue * value) {
	returnNumericValue(value, 0.0);
}

void * valueGetPointer(DataValue * value) {
	switch (value->type) {
		case DATA_TYPE_POINTER:
			return value->value.pointer;
		case DATA_TYPE_STRING:
			return (void *) value->value.string;
		case DATA_TYPE_BLOB:
			return (void *) value->value.blob.bytes;
		case DATA_TYPE_HASH_TABLE:
			return value->value.hashTable;
		case DATA_TYPE_ARRAY:
			return value->value.array;
		case DATA_TYPE_ASSOCIATIVE_ARRAY:
			return value->value.array;
		default:
			return NULL;
	}
}

const char * valueGetString(DataValue * value) {
	if (value->type != DATA_TYPE_STRING) {
		return NULL;
	}
	return value->value.string;
}

const void * valueGetBlob(DataValue * value, size_t * outLength) {
	if (value->type == DATA_TYPE_STRING) {
		if (outLength != NULL) {
			*outLength = strlen(value->value.string);
		}
		return value->value.string;
	}
	if (value->type != DATA_TYPE_BLOB) {
		return NULL;
	}
	if (outLength != NULL) {
		*outLength = value->value.blob.length;
	}
	return value->value.blob.bytes;
}

HashTable * valueGetHashTable(DataValue * value) {
	if (value->type != DATA_TYPE_HASH_TABLE) {
		return NULL;
	}
	return value->value.hashTable;
}

DataArray * valueGetArray(DataValue * value) {
	if (value->type != DATA_TYPE_ARRAY) {
		return NULL;
	}
	return value->value.array;
}

struct AssociativeArray * valueGetAssociativeArray(DataValue * value) {
	if (value->type != DATA_TYPE_ASSOCIATIVE_ARRAY) {
		return NULL;
	}
	return value->value.associativeArray;
}
