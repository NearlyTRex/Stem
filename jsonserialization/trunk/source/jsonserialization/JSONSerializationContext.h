/*
  Copyright (c) 2010 Alex Diener
  
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

#ifndef __JSON_SERIALIZATION_CONTEXT_H__
#define __JSON_SERIALIZATION_CONTEXT_H__

#include "serialization/SerializationContext.h"
#include "jsonio/JSONEmitter.h"
#include "jsonio/JSONIO.h"

typedef struct JSONSerializationContext JSONSerializationContext;

#define JSON_SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER 100
#define JSON_SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH 101
#define JSON_SERIALIZATION_ERROR_CONTAINER_UNDERFLOW 102
#define JSON_SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS 103
#define JSON_SERIALIZATION_ERROR_UNNAMED_BIT 104
#define JSON_SERIALIZATION_ERROR_DUPLICATE_BIT 105
#define JSON_SERIALIZATION_ERROR_ENUM_VALUE_NOT_NAMED 106
#define JSON_SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE 107
#define JSON_SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME 108
#define JSON_SERIALIZATION_ERROR_NULL_KEY 109

enum JSONSerializationContext_containerType {
	CONTAINER_TYPE_ARRAY,
	CONTAINER_TYPE_STRUCTURE,
	CONTAINER_TYPE_DICTIONARY
};

struct JSONSerializationContext_nodeStackItem {
	struct JSONNode * node;
	enum JSONSerializationContext_containerType containerType;
};

#define JSONSerializationContext_structContents \
	SerializationContext_structContents \
	\
	struct JSONNode * rootNode; \
	enum JSONSerializationContext_containerType rootContainerType; \
	struct JSONNode * currentNode; \
	struct JSONSerializationContext_nodeStackItem * nodeStack; \
	size_t nodeStackAllocatedSize; \
	size_t nodeStackCurrentDepth; \
	\
	char * (* writeToString)(void * self, enum JSONEmitterFormat format, size_t * outLength, struct JSONEmissionError * outError); \
	bool (* writeToFile)(void * self, enum JSONEmitterFormat format, const char * filePath, struct JSONEmissionError * outError); \
	struct JSONNode * (* writeToJSONNode)(void * self);

struct JSONSerializationContext {
	JSONSerializationContext_structContents
};

JSONSerializationContext * JSONSerializationContext_create();
void JSONSerializationContext_init(JSONSerializationContext * self);
void JSONSerializationContext_dispose(void * selfPtr);

char * JSONSerializationContext_writeToString(void * selfPtr, enum JSONEmitterFormat format, size_t * outLength, struct JSONEmissionError * outError);
bool JSONSerializationContext_writeToFile(void * selfPtr, enum JSONEmitterFormat format, const char * filePath, struct JSONEmissionError * outError);
struct JSONNode * JSONSerializationContext_writeToJSONNode(void * selfPtr);

void JSONSerializationContext_beginStructure(void * selfPtr, const char * key);
void JSONSerializationContext_beginDictionary(void * selfPtr, const char * key);
void JSONSerializationContext_beginArray(void * selfPtr, const char * key);
void JSONSerializationContext_endStructure(void * selfPtr);
void JSONSerializationContext_endDictionary(void * selfPtr);
void JSONSerializationContext_endArray(void * selfPtr);
void JSONSerializationContext_writeInt8(void * selfPtr, const char * key, int8_t value);
void JSONSerializationContext_writeUInt8(void * selfPtr, const char * key, uint8_t value);
void JSONSerializationContext_writeInt16(void * selfPtr, const char * key, int16_t value);
void JSONSerializationContext_writeUInt16(void * selfPtr, const char * key, uint16_t value);
void JSONSerializationContext_writeInt32(void * selfPtr, const char * key, int32_t value);
void JSONSerializationContext_writeUInt32(void * selfPtr, const char * key, uint32_t value);
void JSONSerializationContext_writeInt64(void * selfPtr, const char * key, int64_t value);
void JSONSerializationContext_writeUInt64(void * selfPtr, const char * key, uint64_t value);
void JSONSerializationContext_writeFloat(void * selfPtr, const char * key, float value);
void JSONSerializationContext_writeDouble(void * selfPtr, const char * key, double value);
void JSONSerializationContext_writeString(void * selfPtr, const char * key, const char * value);
void JSONSerializationContext_writeBoolean(void * selfPtr, const char * key, bool value);
void JSONSerializationContext_writeEnumeration(void * selfPtr, const char * key, int value, ...);
void JSONSerializationContext_writeBitfield8(void * selfPtr, const char * key, uint8_t value, ...);
void JSONSerializationContext_writeBitfield16(void * selfPtr, const char * key, uint16_t value, ...);
void JSONSerializationContext_writeBitfield32(void * selfPtr, const char * key, uint32_t value, ...);
void JSONSerializationContext_writeBitfield64(void * selfPtr, const char * key, uint64_t value, ...);

#endif
