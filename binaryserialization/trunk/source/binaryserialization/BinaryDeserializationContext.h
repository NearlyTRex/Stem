/*
  Copyright (c) 2013 Alex Diener
  
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

#ifndef __BINARY_DESERIALIZATION_CONTEXT_H__
#define __BINARY_DESERIALIZATION_CONTEXT_H__

typedef struct BinaryDeserializationContext BinaryDeserializationContext;

#include "binaryserialization/BinarySerializationShared.h"
#include "serialization/DeserializationContext.h"

struct BinaryDeserializationContext_containerNode {
	enum BinarySerializationContainerType type;
	uint32_t count;
	uint32_t nextItemIndex;
	size_t startOffset;
	size_t endOffset;
};

#define BinaryDeserializationContext_structContents(self_type) \
	DeserializationContext_structContents(self_type) \
	\
	bool bigEndian; \
	const unsigned char * bytes; \
	size_t length; \
	size_t position; \
	size_t containerCount; \
	size_t containerListSize; \
	struct BinaryDeserializationContext_containerNode * containerStack; \
	bool finished;

stemobject_struct_definition(BinaryDeserializationContext)

BinaryDeserializationContext * BinaryDeserializationContext_createWithFile(const char * filePath);
BinaryDeserializationContext * BinaryDeserializationContext_createWithBytes(const void * bytes, size_t length);
void BinaryDeserializationContext_initWithFile(BinaryDeserializationContext * self, const char * filePath);
void BinaryDeserializationContext_initWithBytes(BinaryDeserializationContext * self, const void * bytes, size_t length);
void BinaryDeserializationContext_dispose(BinaryDeserializationContext * self);

void BinaryDeserializationContext_beginStructure(BinaryDeserializationContext * self, const char * key);
size_t BinaryDeserializationContext_beginDictionary(BinaryDeserializationContext * self, const char * key);
size_t BinaryDeserializationContext_beginArray(BinaryDeserializationContext * self, const char * key);
void BinaryDeserializationContext_endStructure(BinaryDeserializationContext * self);
void BinaryDeserializationContext_endDictionary(BinaryDeserializationContext * self);
void BinaryDeserializationContext_endArray(BinaryDeserializationContext * self);
int8_t BinaryDeserializationContext_readInt8(BinaryDeserializationContext * self, const char * key);
uint8_t BinaryDeserializationContext_readUInt8(BinaryDeserializationContext * self, const char * key);
int16_t BinaryDeserializationContext_readInt16(BinaryDeserializationContext * self, const char * key);
uint16_t BinaryDeserializationContext_readUInt16(BinaryDeserializationContext * self, const char * key);
int32_t BinaryDeserializationContext_readInt32(BinaryDeserializationContext * self, const char * key);
uint32_t BinaryDeserializationContext_readUInt32(BinaryDeserializationContext * self, const char * key);
int64_t BinaryDeserializationContext_readInt64(BinaryDeserializationContext * self, const char * key);
uint64_t BinaryDeserializationContext_readUInt64(BinaryDeserializationContext * self, const char * key);
float BinaryDeserializationContext_readFloat(BinaryDeserializationContext * self, const char * key);
double BinaryDeserializationContext_readDouble(BinaryDeserializationContext * self, const char * key);
const char * BinaryDeserializationContext_readString(BinaryDeserializationContext * self, const char * key);
bool BinaryDeserializationContext_readBoolean(BinaryDeserializationContext * self, const char * key);
int BinaryDeserializationContext_readEnumeration(BinaryDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint8_t BinaryDeserializationContext_readBitfield8(BinaryDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint16_t BinaryDeserializationContext_readBitfield16(BinaryDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint32_t BinaryDeserializationContext_readBitfield32(BinaryDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint64_t BinaryDeserializationContext_readBitfield64(BinaryDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
const char * BinaryDeserializationContext_readNextDictionaryKey(BinaryDeserializationContext * self);
bool BinaryDeserializationContext_hasDictionaryKey(BinaryDeserializationContext * self, const char * key);

#endif
