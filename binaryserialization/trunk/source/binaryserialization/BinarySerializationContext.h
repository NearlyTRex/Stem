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

#ifndef __BINARY_SERIALIZATION_CONTEXT_H__
#define __BINARY_SERIALIZATION_CONTEXT_H__

#include "serialization/SerializationContext.h"
#include "binaryserialization/BinarySerializationShared.h"
#include "utilities/IOUtilities.h"

typedef struct BinarySerializationContext BinarySerializationContext;

struct BinarySerializationContext_containerNode {
	enum BinarySerializationContainerType type;
	uint32_t count;
	size_t startOffset;
	size_t keyListSize;
	char ** keys;
	uint32_t * offsets;
};

#define BinarySerializationContext_structContents(self_type) \
	SerializationContext_structContents(self_type) \
	\
	bool bigEndian; \
	struct memwriteContext memwriteContext; \
	size_t containerCount; \
	size_t containerListSize; \
	struct BinarySerializationContext_containerNode * containerStack; \
	bool finished; \
	\
	void * (* writeToBytes)(self_type * self, size_t * outLength); \
	bool (* writeToFile)(self_type * self, const char * filePath);

stemobject_struct_definition(BinarySerializationContext)

BinarySerializationContext * BinarySerializationContext_create(bool bigEndian);
void BinarySerializationContext_init(BinarySerializationContext * self, bool bigEndian);
void BinarySerializationContext_dispose(BinarySerializationContext * self);

// Returned object owned by caller; free when done
// outLength is mandatory; not checked for NULL before written
void * BinarySerializationContext_writeToBytes(BinarySerializationContext * self, size_t * outLength);
bool BinarySerializationContext_writeToFile(BinarySerializationContext * self, const char * filePath);

void BinarySerializationContext_beginStructure(BinarySerializationContext * self, const char * key);
void BinarySerializationContext_beginDictionary(BinarySerializationContext * self, const char * key);
void BinarySerializationContext_beginArray(BinarySerializationContext * self, const char * key);
void BinarySerializationContext_endStructure(BinarySerializationContext * self);
void BinarySerializationContext_endDictionary(BinarySerializationContext * self);
void BinarySerializationContext_endArray(BinarySerializationContext * self);
void BinarySerializationContext_writeInt8(BinarySerializationContext * self, const char * key, int8_t value);
void BinarySerializationContext_writeUInt8(BinarySerializationContext * self, const char * key, uint8_t value);
void BinarySerializationContext_writeInt16(BinarySerializationContext * self, const char * key, int16_t value);
void BinarySerializationContext_writeUInt16(BinarySerializationContext * self, const char * key, uint16_t value);
void BinarySerializationContext_writeInt32(BinarySerializationContext * self, const char * key, int32_t value);
void BinarySerializationContext_writeUInt32(BinarySerializationContext * self, const char * key, uint32_t value);
void BinarySerializationContext_writeInt64(BinarySerializationContext * self, const char * key, int64_t value);
void BinarySerializationContext_writeUInt64(BinarySerializationContext * self, const char * key, uint64_t value);
void BinarySerializationContext_writeFloat(BinarySerializationContext * self, const char * key, float value);
void BinarySerializationContext_writeDouble(BinarySerializationContext * self, const char * key, double value);
void BinarySerializationContext_writeString(BinarySerializationContext * self, const char * key, const char * value);
void BinarySerializationContext_writeBoolean(BinarySerializationContext * self, const char * key, bool value);
void BinarySerializationContext_writeEnumeration(BinarySerializationContext * self, const char * key, int value, ...) __attribute__((sentinel));
void BinarySerializationContext_writeBitfield8(BinarySerializationContext * self, const char * key, uint8_t value, ...) __attribute__((sentinel));
void BinarySerializationContext_writeBitfield16(BinarySerializationContext * self, const char * key, uint16_t value, ...) __attribute__((sentinel));
void BinarySerializationContext_writeBitfield32(BinarySerializationContext * self, const char * key, uint32_t value, ...) __attribute__((sentinel));
void BinarySerializationContext_writeBitfield64(BinarySerializationContext * self, const char * key, uint64_t value, ...) __attribute__((sentinel));

#endif
