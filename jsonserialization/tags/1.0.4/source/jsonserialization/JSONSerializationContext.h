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

#ifndef __JSON_SERIALIZATION_CONTEXT_H__
#define __JSON_SERIALIZATION_CONTEXT_H__

#include "serialization/SerializationContext.h"
#include "jsonio/JSONEmitter.h"
#include "jsonio/JSONIO.h"
#include "jsonserialization/JSONSerializationShared.h"

typedef struct JSONSerializationContext JSONSerializationContext;

struct JSONSerializationContext_nodeStackItem {
	struct JSONNode * node;
	enum JSONSerializationContainerType containerType;
};

#define JSONSerializationContext_structContents(self_type) \
	SerializationContext_structContents(self_type) \
	\
	struct JSONNode * rootNode; \
	struct JSONNode * currentNode; \
	enum JSONSerializationContainerType currentContainerType; \
	struct JSONSerializationContext_nodeStackItem * nodeStack; \
	size_t nodeStackAllocatedSize; \
	size_t nodeStackCurrentDepth; \
	bool rootNodeOwnedBySelf; \
	\
	char * (* writeToString)(self_type * self, enum JSONEmitterFormat format, size_t * outLength, struct JSONEmissionError * outError); \
	bool (* writeToFile)(self_type * self, enum JSONEmitterFormat format, const char * filePath, struct JSONEmissionError * outError); \
	struct JSONNode * (* writeToJSONNode)(self_type * self);

stemobject_struct_definition(JSONSerializationContext)

JSONSerializationContext * JSONSerializationContext_create();
void JSONSerializationContext_init(JSONSerializationContext * self);
void JSONSerializationContext_dispose(JSONSerializationContext * self);

// Returned object owned by caller; free when done
char * JSONSerializationContext_writeToString(JSONSerializationContext * self, enum JSONEmitterFormat format, size_t * outLength, struct JSONEmissionError * outError);
bool JSONSerializationContext_writeToFile(JSONSerializationContext * self, enum JSONEmitterFormat format, const char * filePath, struct JSONEmissionError * outError);
// Returned object owned by caller; free when done with JSONNode_dispose()
struct JSONNode * JSONSerializationContext_writeToJSONNode(JSONSerializationContext * self);

void JSONSerializationContext_beginStructure(JSONSerializationContext * self, const char * key);
void JSONSerializationContext_beginDictionary(JSONSerializationContext * self, const char * key);
void JSONSerializationContext_beginArray(JSONSerializationContext * self, const char * key);
void JSONSerializationContext_endStructure(JSONSerializationContext * self);
void JSONSerializationContext_endDictionary(JSONSerializationContext * self);
void JSONSerializationContext_endArray(JSONSerializationContext * self);
void JSONSerializationContext_writeInt8(JSONSerializationContext * self, const char * key, int8_t value);
void JSONSerializationContext_writeUInt8(JSONSerializationContext * self, const char * key, uint8_t value);
void JSONSerializationContext_writeInt16(JSONSerializationContext * self, const char * key, int16_t value);
void JSONSerializationContext_writeUInt16(JSONSerializationContext * self, const char * key, uint16_t value);
void JSONSerializationContext_writeInt32(JSONSerializationContext * self, const char * key, int32_t value);
void JSONSerializationContext_writeUInt32(JSONSerializationContext * self, const char * key, uint32_t value);
void JSONSerializationContext_writeInt64(JSONSerializationContext * self, const char * key, int64_t value);
void JSONSerializationContext_writeUInt64(JSONSerializationContext * self, const char * key, uint64_t value);
void JSONSerializationContext_writeFloat(JSONSerializationContext * self, const char * key, float value);
void JSONSerializationContext_writeDouble(JSONSerializationContext * self, const char * key, double value);
void JSONSerializationContext_writeString(JSONSerializationContext * self, const char * key, const char * value);
void JSONSerializationContext_writeBoolean(JSONSerializationContext * self, const char * key, bool value);
void JSONSerializationContext_writeEnumeration(JSONSerializationContext * self, const char * key, int value, ...) __attribute__((sentinel));
void JSONSerializationContext_writeBitfield8(JSONSerializationContext * self, const char * key, uint8_t value, ...) __attribute__((sentinel));
void JSONSerializationContext_writeBitfield16(JSONSerializationContext * self, const char * key, uint16_t value, ...) __attribute__((sentinel));
void JSONSerializationContext_writeBitfield32(JSONSerializationContext * self, const char * key, uint32_t value, ...) __attribute__((sentinel));
void JSONSerializationContext_writeBitfield64(JSONSerializationContext * self, const char * key, uint64_t value, ...) __attribute__((sentinel));

#endif
