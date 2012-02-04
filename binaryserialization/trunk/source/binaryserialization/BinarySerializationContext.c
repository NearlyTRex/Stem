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

#include "binaryserialization/BinarySerializationContext.h"
#include <string.h>

#define SUPERCLASS SerializationContext

BinarySerializationContext * BinarySerializationContext_create(bool bigEndian) {
	stemobject_create_implementation(BinarySerializationContext, init, bigEndian)
}

void BinarySerializationContext_init(BinarySerializationContext * self, bool bigEndian) {
	call_super(init, self);
	
	self->jmpBuf = NULL;
	self->status = SERIALIZATION_ERROR_OK;
	
	self->bigEndian = bigEndian;
	self->memwriteContext = memwriteContextInit(NULL, 0, 0, true);
	memwrite(&self->memwriteContext, 4, self->bigEndian ? "Stem" : "metS");
	self->containerCount = 0;
	self->containerListSize = 1;
	self->containerStack = malloc(sizeof(struct BinarySerializationContext_containerNode) * self->containerListSize);
	
	self->dispose = BinarySerializationContext_dispose;
	self->beginStructure = BinarySerializationContext_beginStructure;
	self->beginDictionary = BinarySerializationContext_beginDictionary;
	self->beginArray = BinarySerializationContext_beginArray;
	self->endStructure = BinarySerializationContext_endStructure;
	self->endDictionary = BinarySerializationContext_endDictionary;
	self->endArray = BinarySerializationContext_endArray;
	self->writeInt8 = BinarySerializationContext_writeInt8;
	self->writeUInt8 = BinarySerializationContext_writeUInt8;
	self->writeInt16 = BinarySerializationContext_writeInt16;
	self->writeUInt16 = BinarySerializationContext_writeUInt16;
	self->writeInt32 = BinarySerializationContext_writeInt32;
	self->writeUInt32 = BinarySerializationContext_writeUInt32;
	self->writeInt64 = BinarySerializationContext_writeInt64;
	self->writeUInt64 = BinarySerializationContext_writeUInt64;
	self->writeFloat = BinarySerializationContext_writeFloat;
	self->writeDouble = BinarySerializationContext_writeDouble;
	self->writeString = BinarySerializationContext_writeString;
	self->writeBoolean = BinarySerializationContext_writeBoolean;
	self->writeEnumeration = BinarySerializationContext_writeEnumeration;
	self->writeBitfield8 = BinarySerializationContext_writeBitfield8;
	self->writeBitfield16 = BinarySerializationContext_writeBitfield16;
	self->writeBitfield32 = BinarySerializationContext_writeBitfield32;
	self->writeBitfield64 = BinarySerializationContext_writeBitfield64;
	self->writeToBytes = BinarySerializationContext_writeToBytes;
	self->writeToFile = BinarySerializationContext_writeToFile;
}

void BinarySerializationContext_dispose(BinarySerializationContext * self) {
	size_t containerIndex;
	uint32_t keyIndex;
	
	free(self->memwriteContext.data);
	for (containerIndex = 0; containerIndex < self->containerCount; containerIndex++) {
		if (self->containerStack[containerIndex].type == BINARY_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) {
			for (keyIndex = 0; keyIndex < self->containerStack[containerIndex].count; keyIndex++) {
				free(self->containerStack[containerIndex].keys[keyIndex]);
			}
			free(self->containerStack[containerIndex].keys);
			free(self->containerStack[containerIndex].offsets);
		}
	}
	free(self->containerStack);
	call_super(dispose, self);
}

void * BinarySerializationContext_writeToBytes(BinarySerializationContext * self, size_t * outLength) {
	void * bytes;
	
	bytes = malloc(self->memwriteContext.length);
	memcpy(bytes, self->memwriteContext.data, self->memwriteContext.length);
	*outLength = self->memwriteContext.length;
	return bytes;
}

bool BinarySerializationContext_writeToFile(BinarySerializationContext * self, const char * filePath) {
	return writeFileSimple(filePath, self->memwriteContext.data, self->memwriteContext.length);
}

#define writePreamble(self, key) { \
	struct BinarySerializationContext_containerNode * container; \
	\
	container = &self->containerStack[self->containerCount - 1]; \
	if (container->type == BINARY_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) { \
		if (container->count >= container->keyListSize) { \
			container->keyListSize *= 2; \
			container->keys = realloc(container->keys, sizeof(char *) * container->keyListSize); \
			container->offsets = realloc(container->offsets, sizeof(uint32_t) * container->keyListSize); \
		} \
		container->keys[container->count] = malloc(strlen(key) + 1); \
		strcpy(container->keys[container->count], key); \
		container->offsets[container->count] = self->memwriteContext.position - container->startOffset; \
	} \
	container->count++; \
}

static void writeUInt16Internal(BinarySerializationContext * self, uint16_t value) {
	unsigned char bytes[2];
	
	if (self->bigEndian) {
		bytes[0] = (value >> 8) & 0xFF;
		bytes[1] = value & 0xFF;
	} else {
		bytes[1] = (value >> 8) & 0xFF;
		bytes[0] = value & 0xFF;
	}
	memwrite(&self->memwriteContext, 2, bytes);
}

static void writeUInt32Internal(BinarySerializationContext * self, uint32_t value) {
	unsigned char bytes[4];
	
	if (self->bigEndian) {
		bytes[0] = value >> 24;
		bytes[1] = (value >> 16) & 0xFF;
		bytes[2] = (value >> 8) & 0xFF;
		bytes[3] = value & 0xFF;
	} else {
		bytes[3] = value >> 24;
		bytes[2] = (value >> 16) & 0xFF;
		bytes[1] = (value >> 8) & 0xFF;
		bytes[0] = value & 0xFF;
	}
	memwrite(&self->memwriteContext, 4, bytes);
}

static void writeUInt64Internal(BinarySerializationContext * self, uint64_t value) {
	unsigned char bytes[8];
	
	if (self->bigEndian) {
		bytes[0] = value >> 56;
		bytes[1] = (value >> 48) & 0xFF;
		bytes[2] = (value >> 40) & 0xFF;
		bytes[3] = (value >> 32) & 0xFF;
		bytes[4] = (value >> 24) & 0xFF;
		bytes[5] = (value >> 16) & 0xFF;
		bytes[6] = (value >> 8) & 0xFF;
		bytes[7] = value & 0xFF;
	} else {
		bytes[7] = value >> 56;
		bytes[6] = (value >> 48) & 0xFF;
		bytes[5] = (value >> 40) & 0xFF;
		bytes[4] = (value >> 32) & 0xFF;
		bytes[3] = (value >> 24) & 0xFF;
		bytes[2] = (value >> 16) & 0xFF;
		bytes[1] = (value >> 8) & 0xFF;
		bytes[0] = value & 0xFF;
	}
	memwrite(&self->memwriteContext, 8, bytes);
}

void BinarySerializationContext_beginStructure(BinarySerializationContext * self, const char * key) {
	struct BinarySerializationContext_containerNode containerNode;
	
	if (self->containerCount > 0) {
		writePreamble(self, key);
	}
	
	containerNode.type = BINARY_SERIALIZATION_CONTAINER_TYPE_STRUCT;
	// hush clang
	containerNode.count = 0;
	containerNode.startOffset = self->memwriteContext.position;
	containerNode.keyListSize = 0;
	containerNode.keys = NULL;
	containerNode.offsets = NULL;
	if (self->containerListSize <= self->containerCount) {
		self->containerListSize *= 2;
		self->containerStack = realloc(self->containerStack, sizeof(struct BinarySerializationContext_containerNode) * self->containerListSize);
	}
	self->containerStack[self->containerCount++] = containerNode;
}

void BinarySerializationContext_beginDictionary(BinarySerializationContext * self, const char * key) {
	struct BinarySerializationContext_containerNode containerNode;
	
	if (self->containerCount > 0) {
		writePreamble(self, key);
	}
	
	containerNode.type = BINARY_SERIALIZATION_CONTAINER_TYPE_DICTIONARY;
	containerNode.count = 0;
	containerNode.startOffset = self->memwriteContext.position;
	containerNode.keyListSize = 1;
	containerNode.keys = malloc(sizeof(char *) * containerNode.keyListSize);
	containerNode.offsets = malloc(sizeof(uint32_t) * containerNode.keyListSize);
	if (self->containerListSize <= self->containerCount) {
		self->containerListSize *= 2;
		self->containerStack = realloc(self->containerStack, sizeof(struct BinarySerializationContext_containerNode) * self->containerListSize);
	}
	self->containerStack[self->containerCount++] = containerNode;
}

void BinarySerializationContext_beginArray(BinarySerializationContext * self, const char * key) {
	struct BinarySerializationContext_containerNode containerNode;
	
	if (self->containerCount > 0) {
		writePreamble(self, key);
	}
	
	containerNode.type = BINARY_SERIALIZATION_CONTAINER_TYPE_ARRAY;
	containerNode.count = 0;
	containerNode.startOffset = self->memwriteContext.position;
	// hush clang
	containerNode.keyListSize = 0;
	containerNode.keys = NULL;
	containerNode.offsets = NULL;
	if (self->containerListSize <= self->containerCount) {
		self->containerListSize *= 2;
		self->containerStack = realloc(self->containerStack, sizeof(struct BinarySerializationContext_containerNode) * self->containerListSize);
	}
	self->containerStack[self->containerCount++] = containerNode;
	memwrite(&self->memwriteContext, 4, "\x00\x00\x00\x00");
}

void BinarySerializationContext_endStructure(BinarySerializationContext * self) {
	self->containerCount--;
}

void BinarySerializationContext_endDictionary(BinarySerializationContext * self) {
	size_t keysSize, valuesSize;
	uint32_t keyIndex;
	void * values;
	
	valuesSize = self->memwriteContext.position - self->containerStack[self->containerCount - 1].startOffset;
	values = malloc(valuesSize);
	memcpy(values, self->memwriteContext.data + self->containerStack[self->containerCount - 1].startOffset, valuesSize);
	keysSize = 0;
	for (keyIndex = 0; keyIndex < self->containerStack[self->containerCount - 1].count; keyIndex++) {
		keysSize += strlen(self->containerStack[self->containerCount - 1].keys[keyIndex]) + 5;
	}
	
	self->memwriteContext.position = self->containerStack[self->containerCount - 1].startOffset;
	writeUInt32Internal(self, self->containerStack[self->containerCount - 1].count);
	writeUInt32Internal(self, keysSize + valuesSize + 8);
	for (keyIndex = 0; keyIndex < self->containerStack[self->containerCount - 1].count; keyIndex++) {
		memwrite(&self->memwriteContext, strlen(self->containerStack[self->containerCount - 1].keys[keyIndex]) + 1, self->containerStack[self->containerCount - 1].keys[keyIndex]);
		writeUInt32Internal(self, self->containerStack[self->containerCount - 1].offsets[keyIndex] + keysSize + 8);
		free(self->containerStack[self->containerCount - 1].keys[keyIndex]);
	}
	free(self->containerStack[self->containerCount - 1].keys);
	free(self->containerStack[self->containerCount - 1].offsets);
	memwrite(&self->memwriteContext, valuesSize, values);
	free(values);
	
	self->containerCount--;
}

void BinarySerializationContext_endArray(BinarySerializationContext * self) {
	size_t position;
	
	position = self->memwriteContext.position;
	self->memwriteContext.position = self->containerStack[self->containerCount - 1].startOffset;
	writeUInt32Internal(self, self->containerStack[self->containerCount - 1].count);
	self->memwriteContext.position = position;
	self->containerCount--;
}

void BinarySerializationContext_writeInt8(BinarySerializationContext * self, const char * key, int8_t value) {
	writePreamble(self, key);
	memwrite(&self->memwriteContext, 1, &value);
}

void BinarySerializationContext_writeUInt8(BinarySerializationContext * self, const char * key, uint8_t value) {
	writePreamble(self, key);
	memwrite(&self->memwriteContext, 1, &value);
}

void BinarySerializationContext_writeInt16(BinarySerializationContext * self, const char * key, int16_t value) {
	writePreamble(self, key);
	writeUInt16Internal(self, value);
}

void BinarySerializationContext_writeUInt16(BinarySerializationContext * self, const char * key, uint16_t value) {
	writePreamble(self, key);
	writeUInt16Internal(self, value);
}

void BinarySerializationContext_writeInt32(BinarySerializationContext * self, const char * key, int32_t value) {
	writePreamble(self, key);
	writeUInt32Internal(self, value);
}

void BinarySerializationContext_writeUInt32(BinarySerializationContext * self, const char * key, uint32_t value) {
	writePreamble(self, key);
	writeUInt32Internal(self, value);
}

void BinarySerializationContext_writeInt64(BinarySerializationContext * self, const char * key, int64_t value) {
	writePreamble(self, key);
	writeUInt64Internal(self, value);
}

void BinarySerializationContext_writeUInt64(BinarySerializationContext * self, const char * key, uint64_t value) {
	writePreamble(self, key);
	writeUInt64Internal(self, value);
}

void BinarySerializationContext_writeFloat(BinarySerializationContext * self, const char * key, float value) {
	writePreamble(self, key);
	writeUInt32Internal(self, *(uint32_t *) &value);
}

void BinarySerializationContext_writeDouble(BinarySerializationContext * self, const char * key, double value) {
	writePreamble(self, key);
	writeUInt64Internal(self, *(uint64_t *) &value);
}

void BinarySerializationContext_writeString(BinarySerializationContext * self, const char * key, const char * value) {
	writePreamble(self, key);
	memwrite(&self->memwriteContext, strlen(value) + 1, value);
}

void BinarySerializationContext_writeBoolean(BinarySerializationContext * self, const char * key, bool value) {
	uint8_t value8;
	
	writePreamble(self, key);
	value8 = !!value;
	memwrite(&self->memwriteContext, 1, &value8);
}

void BinarySerializationContext_writeEnumeration(BinarySerializationContext * self, const char * key, int value, ...) {
	writePreamble(self, key);
	writeUInt32Internal(self, value);
}

void BinarySerializationContext_writeBitfield8(BinarySerializationContext * self, const char * key, uint8_t value, ...) {
	writePreamble(self, key);
	memwrite(&self->memwriteContext, 1, &value);
}

void BinarySerializationContext_writeBitfield16(BinarySerializationContext * self, const char * key, uint16_t value, ...) {
	writePreamble(self, key);
	writeUInt16Internal(self, value);
}

void BinarySerializationContext_writeBitfield32(BinarySerializationContext * self, const char * key, uint32_t value, ...) {
	writePreamble(self, key);
	writeUInt32Internal(self, value);
}

void BinarySerializationContext_writeBitfield64(BinarySerializationContext * self, const char * key, uint64_t value, ...) {
	writePreamble(self, key);
	writeUInt64Internal(self, value);
}
