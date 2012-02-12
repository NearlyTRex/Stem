/*
  Copyright (c) 2012 Alex Diener
  
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

#define JSONDeserializationContext_structContents(self_type) \
	DeserializationContext_structContents(self_type) \
	\
	struct JSONNode * rootNode; \
	struct JSONNode * currentNode; \
	enum JSONSerializationContainerType currentContainerType; \
	struct JSONDeserializationContext_nodeStackItem * nodeStack; \
	size_t nodeStackAllocatedSize; \
	size_t nodeStackCurrentDepth; \
	size_t nextNodeIndex; \
	bool finished;

stemobject_struct_definition(JSONDeserializationContext)

JSONDeserializationContext * JSONDeserializationContext_createWithFile(const char * filePath);
JSONDeserializationContext * JSONDeserializationContext_createWithString(const char * string, size_t length);
JSONDeserializationContext * JSONDeserializationContext_createWithJSONNode(struct JSONNode * node);
void JSONDeserializationContext_initWithFile(JSONDeserializationContext * self, const char * filePath);
void JSONDeserializationContext_initWithString(JSONDeserializationContext * self, const char * string, size_t length);
void JSONDeserializationContext_initWithJSONNode(JSONDeserializationContext * self, struct JSONNode * node);
void JSONDeserializationContext_dispose(JSONDeserializationContext * self);

void JSONDeserializationContext_beginStructure(JSONDeserializationContext * self, const char * key);
size_t JSONDeserializationContext_beginDictionary(JSONDeserializationContext * self, const char * key);
size_t JSONDeserializationContext_beginArray(JSONDeserializationContext * self, const char * key);
void JSONDeserializationContext_endStructure(JSONDeserializationContext * self);
void JSONDeserializationContext_endDictionary(JSONDeserializationContext * self);
void JSONDeserializationContext_endArray(JSONDeserializationContext * self);
int8_t JSONDeserializationContext_readInt8(JSONDeserializationContext * self, const char * key);
uint8_t JSONDeserializationContext_readUInt8(JSONDeserializationContext * self, const char * key);
int16_t JSONDeserializationContext_readInt16(JSONDeserializationContext * self, const char * key);
uint16_t JSONDeserializationContext_readUInt16(JSONDeserializationContext * self, const char * key);
int32_t JSONDeserializationContext_readInt32(JSONDeserializationContext * self, const char * key);
uint32_t JSONDeserializationContext_readUInt32(JSONDeserializationContext * self, const char * key);
int64_t JSONDeserializationContext_readInt64(JSONDeserializationContext * self, const char * key);
uint64_t JSONDeserializationContext_readUInt64(JSONDeserializationContext * self, const char * key);
float JSONDeserializationContext_readFloat(JSONDeserializationContext * self, const char * key);
double JSONDeserializationContext_readDouble(JSONDeserializationContext * self, const char * key);
const char * JSONDeserializationContext_readString(JSONDeserializationContext * self, const char * key);
bool JSONDeserializationContext_readBoolean(JSONDeserializationContext * self, const char * key);
int JSONDeserializationContext_readEnumeration(JSONDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint8_t JSONDeserializationContext_readBitfield8(JSONDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint16_t JSONDeserializationContext_readBitfield16(JSONDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint32_t JSONDeserializationContext_readBitfield32(JSONDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint64_t JSONDeserializationContext_readBitfield64(JSONDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
const char * JSONDeserializationContext_readNextDictionaryKey(JSONDeserializationContext * self);
bool JSONDeserializationContext_hasDictionaryKey(JSONDeserializationContext * self, const char * key);

#endif
