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

#include "dynamictypes/AssociativeArray.h"
#include "dynamictypes/DataArray.h"
#include "dynamictypes/DataSerializer.h"
#include "dynamictypes/HashTable.h"
#include <string.h>

#define ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES \
	enumKV(DATA_TYPE_BOOLEAN), \
	enumKV(DATA_TYPE_INT8), \
	enumKV(DATA_TYPE_UINT8), \
	enumKV(DATA_TYPE_INT16), \
	enumKV(DATA_TYPE_UINT16), \
	enumKV(DATA_TYPE_INT32), \
	enumKV(DATA_TYPE_UINT32), \
	enumKV(DATA_TYPE_INT64), \
	enumKV(DATA_TYPE_UINT64), \
	enumKV(DATA_TYPE_FLOAT), \
	enumKV(DATA_TYPE_DOUBLE), \
	enumKV(DATA_TYPE_STRING), \
	enumKV(DATA_TYPE_BLOB), \
	enumKV(DATA_TYPE_HASH_TABLE), \
	enumKV(DATA_TYPE_ARRAY), \
	enumKV(DATA_TYPE_ASSOCIATIVE_ARRAY), \
	enumKV(DATA_TYPE_FIXED_16_16)

static void deserializeContainer(DataValue * container, DeserializationContext * context) {
	size_t index, count;
	const char * key = NULL;
	enum DataValueType type;
	DataValue value;
	const void * blob;
	size_t length = 0;
	
	count = context->beginArray(context, NULL);
	if (context->status != SERIALIZATION_ERROR_OK) {
		return;
	}
	for (index = 0; index < count; index++) {
		if (container->type == DATA_TYPE_HASH_TABLE || container->type == DATA_TYPE_ASSOCIATIVE_ARRAY) {
			key = context->readString(context, NULL);
			if (context->status != SERIALIZATION_ERROR_OK) {
				return;
			}
			index++;
		}
		type = context->readEnumeration(context, NULL, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
		if (context->status != SERIALIZATION_ERROR_OK) {
			return;
		}
		index++;
		switch (type) {
			case DATA_TYPE_BOOLEAN:
				value = valueCreateBoolean(context->readBoolean(context, NULL));
				break;
				
			case DATA_TYPE_INT8:
				value = valueCreateInt8(context->readInt8(context, NULL));
				break;
				
			case DATA_TYPE_UINT8:
				value = valueCreateUInt8(context->readUInt8(context, NULL));
				break;
				
			case DATA_TYPE_INT16:
				value = valueCreateInt16(context->readInt16(context, NULL));
				break;
				
			case DATA_TYPE_UINT16:
				value = valueCreateUInt16(context->readUInt16(context, NULL));
				break;
				
			case DATA_TYPE_INT32:
				value = valueCreateInt32(context->readInt32(context, NULL));
				break;
				
			case DATA_TYPE_UINT32:
				value = valueCreateUInt32(context->readUInt32(context, NULL));
				break;
				
			case DATA_TYPE_INT64:
				value = valueCreateInt64(context->readInt64(context, NULL));
				break;
				
			case DATA_TYPE_UINT64:
				value = valueCreateUInt64(context->readUInt64(context, NULL));
				break;
				
			case DATA_TYPE_FLOAT:
				value = valueCreateFloat(context->readFloat(context, NULL));
				break;
				
			case DATA_TYPE_DOUBLE:
				value = valueCreateDouble(context->readDouble(context, NULL));
				break;
				
			case DATA_TYPE_FIXED_16_16:
				value = valueCreateFixed16_16(context->readFixed16_16(context, NULL));
				break;
				
			case DATA_TYPE_STRING:
				value = valueCreateString(context->readString(context, NULL), DATA_USE_STRLEN, true, true);
				break;
				
			case DATA_TYPE_BLOB:
				blob = context->readBlob(context, NULL, &length);
				if (context->status != SERIALIZATION_ERROR_OK) {
					return;
				}
				value = valueCreateBlob(blob, length, true, true);
				break;
				
			case DATA_TYPE_ARRAY:
				value.type = DATA_TYPE_ARRAY;
				value.value.array = arrayCreate();
				deserializeContainer(&value, context);
				break;
				
			case DATA_TYPE_HASH_TABLE:
				value.type = DATA_TYPE_HASH_TABLE;
				value.value.hashTable = hashCreate();
				deserializeContainer(&value, context);
				break;
				
			case DATA_TYPE_ASSOCIATIVE_ARRAY:
				value.type = DATA_TYPE_ASSOCIATIVE_ARRAY;
				value.value.associativeArray = associativeArrayCreate();
				deserializeContainer(&value, context);
				break;
				
			case DATA_TYPE_POINTER:
				break;
		}
		if (context->status != SERIALIZATION_ERROR_OK) {
			return;
		}
		
		switch (container->type) {
			case DATA_TYPE_ARRAY:
				arrayAppend(container->value.array, value);
				break;
				
			case DATA_TYPE_HASH_TABLE:
				hashSet(container->value.hashTable, key, value);
				break;
				
			case DATA_TYPE_ASSOCIATIVE_ARRAY:
				associativeArrayAppend(container->value.associativeArray, key, value);
				break;
				
			default:
				break;
		}
	}
	context->endArray(context);
}

DataValue DataValue_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	DataValue value = {{.boolean = false}, DATA_TYPE_BOOLEAN, false};
	const char * type;
	
	context->beginArray(context, "data");
	type = context->readString(context, NULL);
	if (!strcmp(type, "array")) {
		value.type = DATA_TYPE_ARRAY;
		value.value.array = arrayCreate();
		
	} else if (!strcmp(type, "structure")) {
		value.type = DATA_TYPE_HASH_TABLE;
		value.value.hashTable = hashCreate();
		
	} else if (!strcmp(type, "dictionary")) {
		value.type = DATA_TYPE_ASSOCIATIVE_ARRAY;
		value.value.associativeArray = associativeArrayCreate();
	}
	
	deserializeContainer(&value, context);
	if (context->status != SERIALIZATION_ERROR_OK) {
		return valueCreateBoolean(false);
	}
	
	context->endArray(context);
	if (context->status != SERIALIZATION_ERROR_OK) {
		return valueCreateBoolean(false);
	}
	
	return value;
}

static void serializeContainer(DataValue * value, SerializationContext * context);

static void serializeValue(DataValue * value, SerializationContext * context) {
	context->writeEnumeration(context, NULL, value->type, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	switch (value->type) {
		case DATA_TYPE_BOOLEAN:
			context->writeBoolean(context, NULL, value->value.boolean);
			break;
			
		case DATA_TYPE_INT8:
			context->writeInt8(context, NULL, value->value.int8);
			break;
			
		case DATA_TYPE_UINT8:
			context->writeUInt8(context, NULL, value->value.uint8);
			break;
			
		case DATA_TYPE_INT16:
			context->writeInt16(context, NULL, value->value.int16);
			break;
			
		case DATA_TYPE_UINT16:
			context->writeUInt16(context, NULL, value->value.uint16);
			break;
			
		case DATA_TYPE_INT32:
			context->writeInt32(context, NULL, value->value.int32);
			break;
			
		case DATA_TYPE_UINT32:
			context->writeUInt32(context, NULL, value->value.uint32);
			break;
			
		case DATA_TYPE_INT64:
			context->writeInt64(context, NULL, value->value.int64);
			break;
			
		case DATA_TYPE_UINT64:
			context->writeUInt64(context, NULL, value->value.uint64);
			break;
			
		case DATA_TYPE_FLOAT:
			context->writeFloat(context, NULL, value->value.float32);
			break;
			
		case DATA_TYPE_DOUBLE:
			context->writeDouble(context, NULL, value->value.float64);
			break;
			
		case DATA_TYPE_FIXED_16_16:
			context->writeFixed16_16(context, NULL, value->value.fixed);
			break;
			
		case DATA_TYPE_POINTER:
			break;
			
		case DATA_TYPE_STRING:
			context->writeString(context, NULL, value->value.string);
			break;
			
		case DATA_TYPE_BLOB:
			context->writeBlob(context, NULL, value->value.blob.bytes, value->value.blob.length);
			break;
			
		case DATA_TYPE_HASH_TABLE:
		case DATA_TYPE_ARRAY:
		case DATA_TYPE_ASSOCIATIVE_ARRAY:
			serializeContainer(value, context);
			break;
	}
}

static void serializeContainer(DataValue * container, SerializationContext * context) {
	size_t index;
	const char ** keys;
	DataValue * value;
	
	switch (container->type) {
		case DATA_TYPE_ARRAY:
			context->beginArray(context, NULL);
			for (index = 0; index < container->value.array->count; index++) {
				value = arrayGet(container->value.array, index);
				if (value->type == DATA_TYPE_POINTER) {
					continue;
				}
				serializeValue(value, context);
			}
			context->endArray(context);
			break;
			
		case DATA_TYPE_HASH_TABLE:
			context->beginArray(context, NULL);
			keys = hashGetKeys(container->value.hashTable, NULL);
			for (index = 0; index < container->value.hashTable->count; index++) {
				value = hashGet(container->value.hashTable, keys[index]);
				if (value->type == DATA_TYPE_POINTER) {
					continue;
				}
				context->writeString(context, NULL, keys[index]);
				serializeValue(value, context);
			}
			free(keys);
			context->endArray(context);
			break;
			
		case DATA_TYPE_ASSOCIATIVE_ARRAY:
			context->beginArray(context, NULL);
			for (index = 0; index < container->value.associativeArray->count; index++) {
				value = associativeArrayGetValueAtIndex(container->value.associativeArray, index);
				if (value->type == DATA_TYPE_POINTER) {
					continue;
				}
				context->writeString(context, NULL, associativeArrayGetKeyAtIndex(container->value.associativeArray, index));
				serializeValue(value, context);
			}
			context->endArray(context);
			break;
			
		default:
			break;
	}
}

void DataValue_serialize(DataValue * value, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	
	context->beginArray(context, "data");
	switch (value->type) {
		case DATA_TYPE_ARRAY:
			context->writeString(context, NULL, "array");
			break;
			
		case DATA_TYPE_HASH_TABLE:
			context->writeString(context, NULL, "structure");
			break;
			
		case DATA_TYPE_ASSOCIATIVE_ARRAY:
			context->writeString(context, NULL, "dictionary");
			break;
			
		default:
			break;
	}
	serializeContainer(value, context);
	context->endArray(context);
}
