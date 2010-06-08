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

#include "jsonserialization/JSONDeserializationContext.h"

JSONDeserializationContext * JSONDeserializationContext_createWithFile(const char * filePath) {
	JSONDeserializationContext * self;
	
	self = malloc(sizeof(JSONDeserializationContext));
	JSONDeserializationContext_initWithFile(self, filePath);
	self->allocated = true;
	return self;
}

JSONDeserializationContext * JSONDeserializationContext_createWithString(const char * string, size_t length) {
	JSONDeserializationContext * self;
	
	self = malloc(sizeof(JSONDeserializationContext));
	JSONDeserializationContext_initWithString(self, string, length);
	self->allocated = true;
	return self;
}

JSONDeserializationContext * JSONDeserializationContext_createWithJSONNode(struct JSONNode * node) {
	JSONDeserializationContext * self;
	
	self = malloc(sizeof(JSONDeserializationContext));
	JSONDeserializationContext_initWithJSONNode(self, node);
	self->allocated = true;
	return self;
}

static void JSONDeserializationContext_init(JSONDeserializationContext * self) {
	DeserializationContext_init((DeserializationContext *) self);
	self->beginStructure = JSONDeserializationContext_beginStructure;
	self->beginDictionary = JSONDeserializationContext_beginDictionary;
	self->beginArray = JSONDeserializationContext_beginArray;
	self->endStructure = JSONDeserializationContext_endStructure;
	self->endDictionary = JSONDeserializationContext_endDictionary;
	self->endArray = JSONDeserializationContext_endArray;
	self->readInt8 = JSONDeserializationContext_readInt8;
	self->readUInt8 = JSONDeserializationContext_readUInt8;
	self->readInt16 = JSONDeserializationContext_readInt16;
	self->readUInt16 = JSONDeserializationContext_readUInt16;
	self->readInt32 = JSONDeserializationContext_readInt32;
	self->readUInt32 = JSONDeserializationContext_readUInt32;
	self->readInt64 = JSONDeserializationContext_readInt64;
	self->readUInt64 = JSONDeserializationContext_readUInt64;
	self->readFloat = JSONDeserializationContext_readFloat;
	self->readDouble = JSONDeserializationContext_readDouble;
	self->readString = JSONDeserializationContext_readString;
	self->readBoolean = JSONDeserializationContext_readBoolean;
	self->readEnumeration = JSONDeserializationContext_readEnumeration;
	self->readBitfield8 = JSONDeserializationContext_readBitfield8;
	self->readBitfield16 = JSONDeserializationContext_readBitfield16;
	self->readBitfield32 = JSONDeserializationContext_readBitfield32;
	self->readBitfield64 = JSONDeserializationContext_readBitfield64;
	self->readNextDictionaryKey = JSONDeserializationContext_readNextDictionaryKey;
	self->hasDictionaryKey = JSONDeserializationContext_hasDictionaryKey;
}

void JSONDeserializationContext_initWithFile(JSONDeserializationContext * self, const char * filePath) {
	JSONDeserializationContext_init(self);
}

void JSONDeserializationContext_initWithString(JSONDeserializationContext * self, const char * string, size_t length) {
	JSONDeserializationContext_init(self);
}

void JSONDeserializationContext_initWithJSONNode(JSONDeserializationContext * self, struct JSONNode * node) {
	JSONDeserializationContext_init(self);
}

void JSONDeserializationContext_dispose(void * selfPtr) {
	DeserializationContext_dispose(selfPtr);
}

unsigned int JSONDeserializationContext_beginStructure(void * selfPtr, const char * key) {
	return 0;
}

unsigned int JSONDeserializationContext_beginDictionary(void * selfPtr, const char * key) {
	return 0;
}

unsigned int JSONDeserializationContext_beginArray(void * selfPtr, const char * key) {
	return 0;
}

void JSONDeserializationContext_endStructure(void * selfPtr) {
}

void JSONDeserializationContext_endDictionary(void * selfPtr) {
}

void JSONDeserializationContext_endArray(void * selfPtr) {
}

int8_t JSONDeserializationContext_readInt8(void * selfPtr, const char * key) {
	return 0;
}

uint8_t JSONDeserializationContext_readUInt8(void * selfPtr, const char * key) {
	return 0;
}

int16_t JSONDeserializationContext_readInt16(void * selfPtr, const char * key) {
	return 0;
}

uint16_t JSONDeserializationContext_readUInt16(void * selfPtr, const char * key) {
	return 0;
}

int32_t JSONDeserializationContext_readInt32(void * selfPtr, const char * key) {
	return 0;
}

uint32_t JSONDeserializationContext_readUInt32(void * selfPtr, const char * key) {
	return 0;
}

int64_t JSONDeserializationContext_readInt64(void * selfPtr, const char * key) {
	return 0;
}

uint64_t JSONDeserializationContext_readUInt64(void * selfPtr, const char * key) {
	return 0;
}

float JSONDeserializationContext_readFloat(void * selfPtr, const char * key) {
	return 0;
}

double JSONDeserializationContext_readDouble(void * selfPtr, const char * key) {
	return 0;
}

const char * JSONDeserializationContext_readString(void * selfPtr, const char * key) {
	return NULL;
}

bool JSONDeserializationContext_readBoolean(void * selfPtr, const char * key) {
	return false;
}

int JSONDeserializationContext_readEnumeration(void * selfPtr, const char * key, ...) {
	return 0;
}

uint8_t JSONDeserializationContext_readBitfield8(void * selfPtr, const char * key, ...) {
	return 0;
}

uint16_t JSONDeserializationContext_readBitfield16(void * selfPtr, const char * key, ...) {
	return 0;
}

uint32_t JSONDeserializationContext_readBitfield32(void * selfPtr, const char * key, ...) {
	return 0;
}

uint64_t JSONDeserializationContext_readBitfield64(void * selfPtr, const char * key, ...) {
	return 0;
}

const char * JSONDeserializationContext_readNextDictionaryKey(void * selfPtr) {
	return NULL;
}

bool JSONDeserializationContext_hasDictionaryKey(void * selfPtr, const char * key) {
	return false;
}
