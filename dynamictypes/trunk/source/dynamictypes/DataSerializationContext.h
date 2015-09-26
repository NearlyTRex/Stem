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

#ifndef __DataSerializationContext_H__
#define __DataSerializationContext_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataSerializationContext DataSerializationContext;

#include "serialization/SerializationContext.h"
#include "dynamictypes/DataValue.h"

#define DataSerializationContext_structContents(self_type) \
	SerializationContext_structContents(self_type) \
	\
	DataValue rootValue; \
	DataValue * currentValue; \
	size_t stackCount; \
	DataValue ** stack; \
	bool finished;

stemobject_struct_definition(DataSerializationContext)

DataSerializationContext * DataSerializationContext_create();
bool DataSerializationContext_init(DataSerializationContext * self);
void DataSerializationContext_dispose(DataSerializationContext * self);

// Returned value not owned by caller. Copy if you need to keep it around.
DataValue * DataSerializationContext_result(DataSerializationContext * self);

void DataSerializationContext_beginStructure(DataSerializationContext * self, const char * key);
void DataSerializationContext_beginDictionary(DataSerializationContext * self, const char * key);
void DataSerializationContext_beginArray(DataSerializationContext * self, const char * key);
void DataSerializationContext_endStructure(DataSerializationContext * self);
void DataSerializationContext_endDictionary(DataSerializationContext * self);
void DataSerializationContext_endArray(DataSerializationContext * self);
void DataSerializationContext_writeBoolean(DataSerializationContext * self, const char * key, bool value);
void DataSerializationContext_writeInt8(DataSerializationContext * self, const char * key, int8_t value);
void DataSerializationContext_writeUInt8(DataSerializationContext * self, const char * key, uint8_t value);
void DataSerializationContext_writeInt16(DataSerializationContext * self, const char * key, int16_t value);
void DataSerializationContext_writeUInt16(DataSerializationContext * self, const char * key, uint16_t value);
void DataSerializationContext_writeInt32(DataSerializationContext * self, const char * key, int32_t value);
void DataSerializationContext_writeUInt32(DataSerializationContext * self, const char * key, uint32_t value);
void DataSerializationContext_writeInt64(DataSerializationContext * self, const char * key, int64_t value);
void DataSerializationContext_writeUInt64(DataSerializationContext * self, const char * key, uint64_t value);
void DataSerializationContext_writeFloat(DataSerializationContext * self, const char * key, float value);
void DataSerializationContext_writeDouble(DataSerializationContext * self, const char * key, double value);
void DataSerializationContext_writeFixed16_16(DataSerializationContext * self, const char * key, fixed16_16 value);
void DataSerializationContext_writeEnumeration(DataSerializationContext * self, const char * key, int value, ...) __attribute__((sentinel));
void DataSerializationContext_writeBitfield8(DataSerializationContext * self, const char * key, uint8_t value, ...) __attribute__((sentinel));
void DataSerializationContext_writeBitfield16(DataSerializationContext * self, const char * key, uint16_t value, ...) __attribute__((sentinel));
void DataSerializationContext_writeBitfield32(DataSerializationContext * self, const char * key, uint32_t value, ...) __attribute__((sentinel));
void DataSerializationContext_writeBitfield64(DataSerializationContext * self, const char * key, uint64_t value, ...) __attribute__((sentinel));
void DataSerializationContext_writeString(DataSerializationContext * self, const char * key, const char * value);
void DataSerializationContext_writeBlob(DataSerializationContext * self, const char * key, const void * value, size_t length);

#ifdef __cplusplus
}
#endif
#endif
