/*
  Copyright (c) 2011 Alex Diener
  
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

#ifndef __JSON_DESERIALIZATION_CONTEXT_H__
#define __JSON_DESERIALIZATION_CONTEXT_H__

#include "serialization/DeserializationContext.h"
#include "jsonio/JSONIO.h"
#include "jsonserialization/JSONSerializationShared.h"

typedef struct JSONDeserializationContext JSONDeserializationContext;

struct JSONDeserializationContext_nodeStackItem {
	struct JSONNode * node;
	size_t index;
	enum JSONSerializationContainerType containerType;
};

#define JSONDeserializationContext_structContents \
	DeserializationContext_structContents \
	\
	struct JSONNode * rootNode; \
	struct JSONNode * currentNode; \
	enum JSONSerializationContainerType currentContainerType; \
	struct JSONDeserializationContext_nodeStackItem * nodeStack; \
	size_t nodeStackAllocatedSize; \
	size_t nodeStackCurrentDepth; \
	size_t nextNodeIndex; \
	bool finished;

struct JSONDeserializationContext {
	JSONDeserializationContext_structContents
};

JSONDeserializationContext * JSONDeserializationContext_createWithFile(const char * filePath);
JSONDeserializationContext * JSONDeserializationContext_createWithString(const char * string, size_t length);
JSONDeserializationContext * JSONDeserializationContext_createWithJSONNode(struct JSONNode * node);
void JSONDeserializationContext_initWithFile(compat_type(JSONDeserializationContext *) selfPtr, const char * filePath);
void JSONDeserializationContext_initWithString(compat_type(JSONDeserializationContext *) selfPtr, const char * string, size_t length);
void JSONDeserializationContext_initWithJSONNode(compat_type(JSONDeserializationContext *) selfPtr, struct JSONNode * node);
void JSONDeserializationContext_dispose(compat_type(JSONDeserializationContext *) selfPtr);

void JSONDeserializationContext_beginStructure(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
size_t JSONDeserializationContext_beginDictionary(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
size_t JSONDeserializationContext_beginArray(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
void JSONDeserializationContext_endStructure(compat_type(JSONDeserializationContext *) selfPtr);
void JSONDeserializationContext_endDictionary(compat_type(JSONDeserializationContext *) selfPtr);
void JSONDeserializationContext_endArray(compat_type(JSONDeserializationContext *) selfPtr);
int8_t JSONDeserializationContext_readInt8(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
uint8_t JSONDeserializationContext_readUInt8(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
int16_t JSONDeserializationContext_readInt16(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
uint16_t JSONDeserializationContext_readUInt16(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
int32_t JSONDeserializationContext_readInt32(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
uint32_t JSONDeserializationContext_readUInt32(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
int64_t JSONDeserializationContext_readInt64(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
uint64_t JSONDeserializationContext_readUInt64(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
float JSONDeserializationContext_readFloat(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
double JSONDeserializationContext_readDouble(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
const char * JSONDeserializationContext_readString(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
bool JSONDeserializationContext_readBoolean(compat_type(JSONDeserializationContext *) selfPtr, const char * key);
int JSONDeserializationContext_readEnumeration(compat_type(JSONDeserializationContext *) selfPtr, const char * key, ...);
uint8_t JSONDeserializationContext_readBitfield8(compat_type(JSONDeserializationContext *) selfPtr, const char * key, ...);
uint16_t JSONDeserializationContext_readBitfield16(compat_type(JSONDeserializationContext *) selfPtr, const char * key, ...);
uint32_t JSONDeserializationContext_readBitfield32(compat_type(JSONDeserializationContext *) selfPtr, const char * key, ...);
uint64_t JSONDeserializationContext_readBitfield64(compat_type(JSONDeserializationContext *) selfPtr, const char * key, ...);
const char * JSONDeserializationContext_readNextDictionaryKey(compat_type(JSONDeserializationContext *) selfPtr);
bool JSONDeserializationContext_hasDictionaryKey(compat_type(JSONDeserializationContext *) selfPtr, const char * key);

#endif
