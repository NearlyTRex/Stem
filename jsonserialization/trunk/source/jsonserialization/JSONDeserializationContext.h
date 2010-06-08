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

#ifndef __JSON_DESERIALIZATION_CONTEXT_H__
#define __JSON_DESERIALIZATION_CONTEXT_H__

#include "serialization/DeserializationContext.h"
#include "jsonio/JSONIO.h"

typedef struct JSONDeserializationContext JSONDeserializationContext;

#define JSONDeserializationContext_structContents \
	DeserializationContext_structContents

struct JSONDeserializationContext {
	JSONDeserializationContext_structContents
};

JSONDeserializationContext * JSONDeserializationContext_createWithFile(const char * filePath);
JSONDeserializationContext * JSONDeserializationContext_createWithString(const char * string, size_t length);
JSONDeserializationContext * JSONDeserializationContext_createWithJSONNode(struct JSONNode * node);
void JSONDeserializationContext_initWithFile(JSONDeserializationContext * self, const char * filePath);
void JSONDeserializationContext_initWithString(JSONDeserializationContext * self, const char * string, size_t length);
void JSONDeserializationContext_initWithJSONNode(JSONDeserializationContext * self, struct JSONNode * node);
void JSONDeserializationContext_dispose(void * selfPtr);

unsigned int JSONDeserializationContext_beginStructure(void * selfPtr, const char * key);
unsigned int JSONDeserializationContext_beginDictionary(void * selfPtr, const char * key);
unsigned int JSONDeserializationContext_beginArray(void * selfPtr, const char * key);
void JSONDeserializationContext_endStructure(void * selfPtr);
void JSONDeserializationContext_endDictionary(void * selfPtr);
void JSONDeserializationContext_endArray(void * selfPtr);
int8_t JSONDeserializationContext_readInt8(void * selfPtr, const char * key);
uint8_t JSONDeserializationContext_readUInt8(void * selfPtr, const char * key);
int16_t JSONDeserializationContext_readInt16(void * selfPtr, const char * key);
uint16_t JSONDeserializationContext_readUInt16(void * selfPtr, const char * key);
int32_t JSONDeserializationContext_readInt32(void * selfPtr, const char * key);
uint32_t JSONDeserializationContext_readUInt32(void * selfPtr, const char * key);
int64_t JSONDeserializationContext_readInt64(void * selfPtr, const char * key);
uint64_t JSONDeserializationContext_readUInt64(void * selfPtr, const char * key);
float JSONDeserializationContext_readFloat(void * selfPtr, const char * key);
double JSONDeserializationContext_readDouble(void * selfPtr, const char * key);
const char * JSONDeserializationContext_readString(void * selfPtr, const char * key);
bool JSONDeserializationContext_readBoolean(void * selfPtr, const char * key);
int JSONDeserializationContext_readEnumeration(void * selfPtr, const char * key, ...);
uint8_t JSONDeserializationContext_readBitfield8(void * selfPtr, const char * key, ...);
uint16_t JSONDeserializationContext_readBitfield16(void * selfPtr, const char * key, ...);
uint32_t JSONDeserializationContext_readBitfield32(void * selfPtr, const char * key, ...);
uint64_t JSONDeserializationContext_readBitfield64(void * selfPtr, const char * key, ...);
const char * JSONDeserializationContext_readNextDictionaryKey(void * selfPtr);
bool JSONDeserializationContext_hasDictionaryKey(void * selfPtr, const char * key);

#endif
