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

#ifndef __DataDeserializationContext_H__
#define __DataDeserializationContext_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "serialization/DeserializationContext.h"
#include "dynamictypes/DataValue.h"

typedef struct DataDeserializationContext DataDeserializationContext;

#define DataDeserializationContext_structContents(self_type) \
	DeserializationContext_structContents(self_type) \
	\
	DataValue rootValue;

stemobject_struct_definition(DataDeserializationContext)

DataDeserializationContext * DataDeserializationContext_create(DataValue rootValue);
bool DataDeserializationContext_init(DataDeserializationContext * self, DataValue rootValue);
void DataDeserializationContext_dispose(DataDeserializationContext * self);

void DataDeserializationContext_beginStructure(DataDeserializationContext * self, const char * key);
size_t DataDeserializationContext_beginDictionary(DataDeserializationContext * self, const char * key);
size_t DataDeserializationContext_beginArray(DataDeserializationContext * self, const char * key);
void DataDeserializationContext_endStructure(DataDeserializationContext * self);
void DataDeserializationContext_endDictionary(DataDeserializationContext * self);
void DataDeserializationContext_endArray(DataDeserializationContext * self);
bool DataDeserializationContext_readBoolean(DataDeserializationContext * self, const char * key);
int8_t DataDeserializationContext_readInt8(DataDeserializationContext * self, const char * key);
uint8_t DataDeserializationContext_readUInt8(DataDeserializationContext * self, const char * key);
int16_t DataDeserializationContext_readInt16(DataDeserializationContext * self, const char * key);
uint16_t DataDeserializationContext_readUInt16(DataDeserializationContext * self, const char * key);
int32_t DataDeserializationContext_readInt32(DataDeserializationContext * self, const char * key);
uint32_t DataDeserializationContext_readUInt32(DataDeserializationContext * self, const char * key);
int64_t DataDeserializationContext_readInt64(DataDeserializationContext * self, const char * key);
uint64_t DataDeserializationContext_readUInt64(DataDeserializationContext * self, const char * key);
float DataDeserializationContext_readFloat(DataDeserializationContext * self, const char * key);
double DataDeserializationContext_readDouble(DataDeserializationContext * self, const char * key);
int DataDeserializationContext_readEnumeration(DataDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint8_t DataDeserializationContext_readBitfield8(DataDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint16_t DataDeserializationContext_readBitfield16(DataDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint32_t DataDeserializationContext_readBitfield32(DataDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint64_t DataDeserializationContext_readBitfield64(DataDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
const char * DataDeserializationContext_readString(DataDeserializationContext * self, const char * key);
const void * DataDeserializationContext_readBlob(DataDeserializationContext * self, const char * key, size_t * outLength);
const char * DataDeserializationContext_readNextDictionaryKey(DataDeserializationContext * self);
bool DataDeserializationContext_hasDictionaryKey(DataDeserializationContext * self, const char * key);

#ifdef __cplusplus
}
#endif
#endif
