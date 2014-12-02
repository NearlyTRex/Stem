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

#include "dynamictypes/AssociativeArray.h"
#include "dynamictypes/DataArray.h"
#include "dynamictypes/HashTable.h"
#include "dynamictypes/DataSerializationContext.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

DataSerializationContext * DataSerializationContext_create() {
	stemobject_create_implementation(DataSerializationContext, init)
}

bool DataSerializationContext_init(DataSerializationContext * self) {
	call_super(init, self);
	self->dispose = DataSerializationContext_dispose;
	self->beginStructure = DataSerializationContext_beginStructure;
	self->beginDictionary = DataSerializationContext_beginDictionary;
	self->beginArray = DataSerializationContext_beginArray;
	self->endStructure = DataSerializationContext_endStructure;
	self->endDictionary = DataSerializationContext_endDictionary;
	self->endArray = DataSerializationContext_endArray;
	self->writeInt8 = DataSerializationContext_writeInt8;
	self->writeUInt8 = DataSerializationContext_writeUInt8;
	self->writeInt16 = DataSerializationContext_writeInt16;
	self->writeUInt16 = DataSerializationContext_writeUInt16;
	self->writeInt32 = DataSerializationContext_writeInt32;
	self->writeUInt32 = DataSerializationContext_writeUInt32;
	self->writeInt64 = DataSerializationContext_writeInt64;
	self->writeUInt64 = DataSerializationContext_writeUInt64;
	self->writeFloat = DataSerializationContext_writeFloat;
	self->writeDouble = DataSerializationContext_writeDouble;
	self->writeString = DataSerializationContext_writeString;
	self->writeBoolean = DataSerializationContext_writeBoolean;
	self->writeEnumeration = DataSerializationContext_writeEnumeration;
	self->writeBitfield8 = DataSerializationContext_writeBitfield8;
	self->writeBitfield16 = DataSerializationContext_writeBitfield16;
	self->writeBitfield32 = DataSerializationContext_writeBitfield32;
	self->writeBitfield64 = DataSerializationContext_writeBitfield64;
	self->rootValue = valueCreateBoolean(false);
	self->currentValue = NULL;
	return true;
}

void DataSerializationContext_dispose(DataSerializationContext * self) {
	call_super(dispose, self);
}

DataValue DataSerializationContext_result(DataSerializationContext * self) {
	return self->rootValue;
}

void DataSerializationContext_beginStructure(DataSerializationContext * self, const char * key) {
	self->rootValue = valueCreateHashTable(hashCreate(), true, false);
	self->currentValue = &self->rootValue;
}

void DataSerializationContext_beginDictionary(DataSerializationContext * self, const char * key) {
	self->rootValue = valueCreateAssociativeArray(associativeArrayCreate(), true, false);
	self->currentValue = &self->rootValue;
}

void DataSerializationContext_beginArray(DataSerializationContext * self, const char * key) {
	self->rootValue = valueCreateArray(arrayCreate(), true, false);
	self->currentValue = &self->rootValue;
}

void DataSerializationContext_endStructure(DataSerializationContext * self) {
}

void DataSerializationContext_endDictionary(DataSerializationContext * self) {
}

void DataSerializationContext_endArray(DataSerializationContext * self) {
}

static void writeValue(DataSerializationContext * self, const char * key, DataValue value) {
	switch (self->currentValue->type) {
		case DATA_TYPE_ARRAY:
			arrayAppend(self->currentValue->value.array, value);
			break;
			
		case DATA_TYPE_HASH_TABLE:
			hashSet(self->currentValue->value.hashTable, key, value);
			break;
			
		case DATA_TYPE_ASSOCIATIVE_ARRAY:
			associativeArrayAppend(self->currentValue->value.associativeArray, key, value);
			break;
			
		default:
			break;
	}
}

void DataSerializationContext_writeInt8(DataSerializationContext * self, const char * key, int8_t value) {
	writeValue(self, key, valueCreateInt8(value));
}

void DataSerializationContext_writeUInt8(DataSerializationContext * self, const char * key, uint8_t value) {
	writeValue(self, key, valueCreateUInt8(value));
}

void DataSerializationContext_writeInt16(DataSerializationContext * self, const char * key, int16_t value) {
	writeValue(self, key, valueCreateInt16(value));
}

void DataSerializationContext_writeUInt16(DataSerializationContext * self, const char * key, uint16_t value) {
	writeValue(self, key, valueCreateUInt16(value));
}

void DataSerializationContext_writeInt32(DataSerializationContext * self, const char * key, int32_t value) {
	writeValue(self, key, valueCreateInt32(value));
}

void DataSerializationContext_writeUInt32(DataSerializationContext * self, const char * key, uint32_t value) {
	writeValue(self, key, valueCreateUInt32(value));
}

void DataSerializationContext_writeInt64(DataSerializationContext * self, const char * key, int64_t value) {
	writeValue(self, key, valueCreateInt64(value));
}

void DataSerializationContext_writeUInt64(DataSerializationContext * self, const char * key, uint64_t value) {
	writeValue(self, key, valueCreateUInt64(value));
}

void DataSerializationContext_writeFloat(DataSerializationContext * self, const char * key, float value) {
	writeValue(self, key, valueCreateFloat(value));
}

void DataSerializationContext_writeDouble(DataSerializationContext * self, const char * key, double value) {
	writeValue(self, key, valueCreateDouble(value));
}

void DataSerializationContext_writeString(DataSerializationContext * self, const char * key, const char * value) {
}

void DataSerializationContext_writeBoolean(DataSerializationContext * self, const char * key, bool value) {
	writeValue(self, key, valueCreateBoolean(value));
}

void DataSerializationContext_writeEnumeration(DataSerializationContext * self, const char * key, int value, ...) {
}

void DataSerializationContext_writeBitfield8(DataSerializationContext * self, const char * key, uint8_t value, ...) {
}

void DataSerializationContext_writeBitfield16(DataSerializationContext * self, const char * key, uint16_t value, ...) {
}

void DataSerializationContext_writeBitfield32(DataSerializationContext * self, const char * key, uint32_t value, ...) {
}

void DataSerializationContext_writeBitfield64(DataSerializationContext * self, const char * key, uint64_t value, ...) {
}
