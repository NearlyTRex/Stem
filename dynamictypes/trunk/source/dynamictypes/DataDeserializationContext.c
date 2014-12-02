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

#include "dynamictypes/DataDeserializationContext.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

DataDeserializationContext * DataDeserializationContext_create(DataValue rootValue) {
	stemobject_create_implementation(DataDeserializationContext, init, rootValue)
}

bool DataDeserializationContext_init(DataDeserializationContext * self, DataValue rootValue) {
	call_super(init, self);
	self->dispose = DataDeserializationContext_dispose;
	self->beginStructure = DataDeserializationContext_beginStructure;
	self->beginDictionary = DataDeserializationContext_beginDictionary;
	self->beginArray = DataDeserializationContext_beginArray;
	self->endStructure = DataDeserializationContext_endStructure;
	self->endDictionary = DataDeserializationContext_endDictionary;
	self->endArray = DataDeserializationContext_endArray;
	self->readInt8 = DataDeserializationContext_readInt8;
	self->readUInt8 = DataDeserializationContext_readUInt8;
	self->readInt16 = DataDeserializationContext_readInt16;
	self->readUInt16 = DataDeserializationContext_readUInt16;
	self->readInt32 = DataDeserializationContext_readInt32;
	self->readUInt32 = DataDeserializationContext_readUInt32;
	self->readInt64 = DataDeserializationContext_readInt64;
	self->readUInt64 = DataDeserializationContext_readUInt64;
	self->readFloat = DataDeserializationContext_readFloat;
	self->readDouble = DataDeserializationContext_readDouble;
	self->readString = DataDeserializationContext_readString;
	self->readBoolean = DataDeserializationContext_readBoolean;
	self->readEnumeration = DataDeserializationContext_readEnumeration;
	self->readBitfield8 = DataDeserializationContext_readBitfield8;
	self->readBitfield16 = DataDeserializationContext_readBitfield16;
	self->readBitfield32 = DataDeserializationContext_readBitfield32;
	self->readBitfield64 = DataDeserializationContext_readBitfield64;
	self->readNextDictionaryKey = DataDeserializationContext_readNextDictionaryKey;
	self->hasDictionaryKey = DataDeserializationContext_hasDictionaryKey;
	return true;
}

void DataDeserializationContext_dispose(DataDeserializationContext * self) {
	call_super(dispose, self);
}

void DataDeserializationContext_beginStructure(DataDeserializationContext * self, const char * key) {
}

size_t DataDeserializationContext_beginDictionary(DataDeserializationContext * self, const char * key) {
	return 0;
}

size_t DataDeserializationContext_beginArray(DataDeserializationContext * self, const char * key) {
	return 0;
}

void DataDeserializationContext_endStructure(DataDeserializationContext * self) {
}

void DataDeserializationContext_endDictionary(DataDeserializationContext * self) {
}

void DataDeserializationContext_endArray(DataDeserializationContext * self) {
}

int8_t DataDeserializationContext_readInt8(DataDeserializationContext * self, const char * key) {
	return 0;
}

uint8_t DataDeserializationContext_readUInt8(DataDeserializationContext * self, const char * key) {
	return 0;
}

int16_t DataDeserializationContext_readInt16(DataDeserializationContext * self, const char * key) {
	return 0;
}

uint16_t DataDeserializationContext_readUInt16(DataDeserializationContext * self, const char * key) {
	return 0;
}

int32_t DataDeserializationContext_readInt32(DataDeserializationContext * self, const char * key) {
	return 0;
}

uint32_t DataDeserializationContext_readUInt32(DataDeserializationContext * self, const char * key) {
	return 0;
}

int64_t DataDeserializationContext_readInt64(DataDeserializationContext * self, const char * key) {
	return 0;
}

uint64_t DataDeserializationContext_readUInt64(DataDeserializationContext * self, const char * key) {
	return 0;
}

float DataDeserializationContext_readFloat(DataDeserializationContext * self, const char * key) {
	return 0.0f;
}

double DataDeserializationContext_readDouble(DataDeserializationContext * self, const char * key) {
	return 0.0;
}

const char * DataDeserializationContext_readString(DataDeserializationContext * self, const char * key) {
	return NULL;
}

bool DataDeserializationContext_readBoolean(DataDeserializationContext * self, const char * key) {
	return false;
}

int DataDeserializationContext_readEnumeration(DataDeserializationContext * self, const char * key, ...) {
	return 0;
}

uint8_t DataDeserializationContext_readBitfield8(DataDeserializationContext * self, const char * key, ...) {
	return 0;
}

uint16_t DataDeserializationContext_readBitfield16(DataDeserializationContext * self, const char * key, ...) {
	return 0;
}

uint32_t DataDeserializationContext_readBitfield32(DataDeserializationContext * self, const char * key, ...) {
	return 0;
}

uint64_t DataDeserializationContext_readBitfield64(DataDeserializationContext * self, const char * key, ...) {
	return 0;
}

const char * DataDeserializationContext_readNextDictionaryKey(DataDeserializationContext * self) {
	return NULL;
}

bool DataDeserializationContext_hasDictionaryKey(DataDeserializationContext * self, const char * key) {
	return false;
}
