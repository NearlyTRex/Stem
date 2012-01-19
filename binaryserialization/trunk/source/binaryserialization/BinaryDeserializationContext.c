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

#include "binaryserialization/BinaryDeserializationContext.h"
#include "utilities/IOUtilities.h"
#include <string.h>
#include <stdarg.h>

#define SUPERCLASS DeserializationContext

#ifndef SIZE_T_MAX
#define SIZE_T_MAX ((size_t) SIZE_MAX)
#endif

#define failWithStatus(STATUS, RETURN_CODE) \
	self->status = (STATUS); \
	if (self->jmpBuf != NULL) { \
		longjmp(*self->jmpBuf, self->status); \
	} \
	RETURN_CODE;

#define checkCanReadValue(LENGTH, RETURN_CODE) \
	if (self->containerStack[self->containerCount - 1].type == BINARY_SERIALIZATION_CONTAINER_TYPE_ARRAY && self->containerStack[self->containerCount - 1].nextItemIndex > self->containerStack[self->containerCount - 1].count) { \
		failWithStatus(SERIALIZATION_ERROR_END_OF_CONTAINER, RETURN_CODE) \
	} \
	if (self->position > self->length - LENGTH) { \
		failWithStatus(BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, RETURN_CODE) \
	}

#define lookUpKey(KEY, FAIL_RETURN_CODE) \
	if (self->containerCount == 0) { \
		failWithStatus(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, FAIL_RETURN_CODE) \
	} \
	if (self->containerStack[self->containerCount - 1].type != BINARY_SERIALIZATION_CONTAINER_TYPE_ARRAY && key == NULL) { \
		failWithStatus(SERIALIZATION_ERROR_NULL_KEY, FAIL_RETURN_CODE) \
	} \
	if (self->containerStack[self->containerCount - 1].type == BINARY_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) { \
		size_t charIndex, lastCharIndex; \
		uint32_t keyIndex = 0; \
		size_t firstMatchOffset = SIZE_T_MAX; \
		\
		lastCharIndex = self->containerStack[self->containerCount - 1].startOffset + 8; \
		for (charIndex = lastCharIndex; charIndex < self->length && keyIndex < self->containerStack[self->containerCount - 1].count; charIndex++) { \
			if (self->bytes[charIndex] == 0) { \
				if (!strcmp((char *) self->bytes + lastCharIndex, KEY)) { \
					if (firstMatchOffset == SIZE_T_MAX || KEY == (char *) self->bytes + lastCharIndex) { \
						if (self->bigEndian) { \
							firstMatchOffset = self->containerStack[self->containerCount - 1].startOffset + (self->bytes[charIndex + 1] << 24 | self->bytes[charIndex + 2] << 16 | self->bytes[charIndex + 3] << 8 | self->bytes[charIndex + 4]); \
						} else { \
							firstMatchOffset = self->containerStack[self->containerCount - 1].startOffset + (self->bytes[charIndex + 4] << 24 | self->bytes[charIndex + 3] << 16 | self->bytes[charIndex + 2] << 8 | self->bytes[charIndex + 1]); \
						} \
					} \
					if (KEY == (char *) self->bytes + lastCharIndex) { \
						break; \
					} \
				} \
				charIndex += 4; \
				keyIndex++; \
				lastCharIndex = charIndex + 1; \
			} \
		} \
		if (firstMatchOffset != SIZE_T_MAX) { \
			self->position = firstMatchOffset; \
		} else { \
			failWithStatus(SERIALIZATION_ERROR_KEY_NOT_FOUND, FAIL_RETURN_CODE) \
		} \
	} else if (self->containerStack[self->containerCount - 1].type == BINARY_SERIALIZATION_CONTAINER_TYPE_ARRAY) { \
		self->containerStack[self->containerCount - 1].nextItemIndex++; \
	}

BinaryDeserializationContext * BinaryDeserializationContext_createWithFile(const char * filePath) {
	stemobject_create_implementation(BinaryDeserializationContext, initWithFile, filePath)
}

BinaryDeserializationContext * BinaryDeserializationContext_createWithBytes(const void * bytes, size_t length) {
	stemobject_create_implementation(BinaryDeserializationContext, initWithBytes, bytes, length)
}

static void sharedInit(BinaryDeserializationContext * self) {
	call_super(init, self);
	
	self->status = SERIALIZATION_ERROR_OK;
	self->position = 0;
	if (self->length >= 4) {
		if (!memcmp(self->bytes, "Stem", 4)) {
			self->bigEndian = true;
			self->position = 4;
		} else if (!memcmp(self->bytes, "metS", 4)) {
			self->bigEndian = false;
			self->position = 4;
		} else {
			self->status = BINARY_SERIALIZATION_ERROR_INVALID_SIGNATURE;
		}
	} else {
		self->status = BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF;
	}
	self->jmpBuf = NULL;
	self->containerCount = 0;
	self->containerListSize = 1;
	self->containerStack = malloc(sizeof(struct containerNode) * self->containerListSize);
	self->finished = false;
	
	self->dispose = BinaryDeserializationContext_dispose;
	self->beginStructure = BinaryDeserializationContext_beginStructure;
	self->beginDictionary = BinaryDeserializationContext_beginDictionary;
	self->beginArray = BinaryDeserializationContext_beginArray;
	self->endStructure = BinaryDeserializationContext_endStructure;
	self->endDictionary = BinaryDeserializationContext_endDictionary;
	self->endArray = BinaryDeserializationContext_endArray;
	self->readInt8 = BinaryDeserializationContext_readInt8;
	self->readUInt8 = BinaryDeserializationContext_readUInt8;
	self->readInt16 = BinaryDeserializationContext_readInt16;
	self->readUInt16 = BinaryDeserializationContext_readUInt16;
	self->readInt32 = BinaryDeserializationContext_readInt32;
	self->readUInt32 = BinaryDeserializationContext_readUInt32;
	self->readInt64 = BinaryDeserializationContext_readInt64;
	self->readUInt64 = BinaryDeserializationContext_readUInt64;
	self->readFloat = BinaryDeserializationContext_readFloat;
	self->readDouble = BinaryDeserializationContext_readDouble;
	self->readString = BinaryDeserializationContext_readString;
	self->readBoolean = BinaryDeserializationContext_readBoolean;
	self->readEnumeration = BinaryDeserializationContext_readEnumeration;
	self->readBitfield8 = BinaryDeserializationContext_readBitfield8;
	self->readBitfield16 = BinaryDeserializationContext_readBitfield16;
	self->readBitfield32 = BinaryDeserializationContext_readBitfield32;
	self->readBitfield64 = BinaryDeserializationContext_readBitfield64;
	self->readNextDictionaryKey = BinaryDeserializationContext_readNextDictionaryKey;
	self->hasDictionaryKey = BinaryDeserializationContext_hasDictionaryKey;
}

void BinaryDeserializationContext_initWithFile(BinaryDeserializationContext * self, const char * filePath) {
	self->bytes = readFileSimple(filePath, &self->length);
	sharedInit(self);
}

void BinaryDeserializationContext_initWithBytes(BinaryDeserializationContext * self, const void * bytes, size_t length) {
	void * bytesMutable;
	
	bytesMutable = malloc(length);
	memcpy(bytesMutable, bytes, length);
	self->bytes = bytesMutable;
	self->length = length;
	sharedInit(self);
}

void BinaryDeserializationContext_dispose(BinaryDeserializationContext * self) {
	free((void *) self->bytes);
	free(self->containerStack);
	call_super(dispose, self);
}

static uint32_t readUInt32Internal(BinaryDeserializationContext * self) {
	uint32_t value;
	
	if (self->position > self->length - 4) {
		failWithStatus(BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, return 0)
	}
	if (self->bigEndian) {
		value = self->bytes[self->position] << 24 | self->bytes[self->position + 1] << 16 | self->bytes[self->position + 2] << 8 | self->bytes[self->position + 3];
	} else {
		value = self->bytes[self->position + 3] << 24 | self->bytes[self->position + 2] << 16 | self->bytes[self->position + 1] << 8 | self->bytes[self->position];
	}
	self->position += 4;
	return value;
}

static const char * readStringInternal(BinaryDeserializationContext * self) {
	size_t charIndex;
	const char * value;
	
	for (charIndex = self->position; charIndex < self->length; charIndex++) {
		if (self->bytes[charIndex] == 0) {
			break;
		}
	}
	if (charIndex < self->length) {
		value = (const char *) self->bytes + self->position;
		self->position = charIndex + 1;
		return value;
	}
	failWithStatus(BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, return NULL)
}

void BinaryDeserializationContext_beginStructure(BinaryDeserializationContext * self, const char * key) {
	if (self->finished) {
		failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return)
	}
	
	if (self->containerCount > 0) {
		lookUpKey(key, return)
	}
	if (self->containerListSize <= self->containerCount) {
		self->containerListSize *= 2;
		self->containerStack = realloc(self->containerStack, sizeof(struct containerNode) * self->containerListSize);
	}
	self->containerStack[self->containerCount++].type = BINARY_SERIALIZATION_CONTAINER_TYPE_STRUCT;
}

size_t BinaryDeserializationContext_beginDictionary(BinaryDeserializationContext * self, const char * key) {
	uint32_t keyIndex, offset;
	struct containerNode containerNode;
	
	if (self->finished) {
		failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return 0)
	}
	
	if (self->containerCount > 0) {
		lookUpKey(key, return 0)
	}
	containerNode.type = BINARY_SERIALIZATION_CONTAINER_TYPE_DICTIONARY;
	containerNode.startOffset = self->position;
	containerNode.count = readUInt32Internal(self);
	containerNode.nextItemIndex = 0;
	containerNode.endOffset = containerNode.startOffset + readUInt32Internal(self);
	for (keyIndex = 0; keyIndex < containerNode.count; keyIndex++) {
		readStringInternal(self);
		offset = readUInt32Internal(self);
		if (offset + 4 > self->length) {
			failWithStatus(BINARY_SERIALIZATION_ERROR_INVALID_OFFSET, return 0)
		}
	}
	if (self->containerListSize <= self->containerCount) {
		self->containerListSize *= 2;
		self->containerStack = realloc(self->containerStack, sizeof(struct containerNode) * self->containerListSize);
	}
	if (containerNode.endOffset > self->length) {
		failWithStatus(BINARY_SERIALIZATION_ERROR_INVALID_OFFSET, return 0)
	}
	self->containerStack[self->containerCount++] = containerNode;
	return containerNode.count;
}

size_t BinaryDeserializationContext_beginArray(BinaryDeserializationContext * self, const char * key) {
	struct containerNode containerNode;
	
	if (self->finished) {
		failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return 0)
	}
	
	if (self->containerCount > 0) {
		lookUpKey(key, return 0)
	}
	checkCanReadValue(4, return 0)
	containerNode.type = BINARY_SERIALIZATION_CONTAINER_TYPE_ARRAY;
	containerNode.count = readUInt32Internal(self);
	containerNode.nextItemIndex = 0;
	containerNode.startOffset = containerNode.endOffset = 0; // hush clang
	if (self->containerListSize <= self->containerCount) {
		self->containerListSize *= 2;
		self->containerStack = realloc(self->containerStack, sizeof(struct containerNode) * self->containerListSize);
	}
	self->containerStack[self->containerCount++] = containerNode;
	return containerNode.count;
}

void BinaryDeserializationContext_endStructure(BinaryDeserializationContext * self) {
	if (self->containerCount == 0) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, return)
	}
	if (self->containerStack[self->containerCount - 1].type != BINARY_SERIALIZATION_CONTAINER_TYPE_STRUCT) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return)
	}
	self->containerCount--;
	if (self->containerCount == 0) {
		if (self->position < self->length) {
			failWithStatus(BINARY_SERIALIZATION_ERROR_EXTRA_DATA_AT_EOF, return)
		}
		self->finished = true;
	}
}

void BinaryDeserializationContext_endDictionary(BinaryDeserializationContext * self) {
	if (self->containerCount == 0) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, return)
	}
	if (self->containerStack[self->containerCount - 1].type != BINARY_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return)
	}
	self->position = self->containerStack[self->containerCount - 1].endOffset;
	self->containerCount--;
	if (self->containerCount == 0) {
		if (self->position < self->length) {
			failWithStatus(BINARY_SERIALIZATION_ERROR_EXTRA_DATA_AT_EOF, return)
		}
		self->finished = true;
	}
}

void BinaryDeserializationContext_endArray(BinaryDeserializationContext * self) {
	if (self->containerCount == 0) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, return)
	}
	if (self->containerStack[self->containerCount - 1].type != BINARY_SERIALIZATION_CONTAINER_TYPE_ARRAY) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return)
	}
	if (self->containerStack[self->containerCount - 1].nextItemIndex < self->containerStack[self->containerCount - 1].count) {
		failWithStatus(BINARY_SERIALIZATION_ERROR_ARRAY_NOT_FULLY_READ, return)
	}
	self->containerCount--;
	if (self->containerCount == 0) {
		if (self->position < self->length) {
			failWithStatus(BINARY_SERIALIZATION_ERROR_EXTRA_DATA_AT_EOF, return)
		}
		self->finished = true;
	}
}

int8_t BinaryDeserializationContext_readInt8(BinaryDeserializationContext * self, const char * key) {
	int8_t value;
	
	lookUpKey(key, return 0)
	checkCanReadValue(1, return 0)
	value = self->bytes[self->position];
	self->position += 1;
	return value;
}

uint8_t BinaryDeserializationContext_readUInt8(BinaryDeserializationContext * self, const char * key) {
	uint8_t value;
	
	lookUpKey(key, return 0)
	checkCanReadValue(1, return 0)
	value = self->bytes[self->position];
	self->position += 1;
	return value;
}

int16_t BinaryDeserializationContext_readInt16(BinaryDeserializationContext * self, const char * key) {
	int16_t value;
	
	lookUpKey(key, return 0)
	checkCanReadValue(2, return 0)
	if (self->bigEndian) {
		value = self->bytes[self->position] << 8 | self->bytes[self->position + 1];
	} else {
		value = self->bytes[self->position + 1] << 8 | self->bytes[self->position];
	}
	self->position += 2;
	return value;
}

uint16_t BinaryDeserializationContext_readUInt16(BinaryDeserializationContext * self, const char * key) {
	uint16_t value;
	
	lookUpKey(key, return 0)
	checkCanReadValue(2, return 0)
	if (self->bigEndian) {
		value = self->bytes[self->position] << 8 | self->bytes[self->position + 1];
	} else {
		value = self->bytes[self->position + 1] << 8 | self->bytes[self->position];
	}
	self->position += 2;
	return value;
}

int32_t BinaryDeserializationContext_readInt32(BinaryDeserializationContext * self, const char * key) {
	int32_t value;
	
	lookUpKey(key, return 0)
	checkCanReadValue(4, return 0)
	if (self->bigEndian) {
		value = self->bytes[self->position] << 24 | self->bytes[self->position + 1] << 16 | self->bytes[self->position + 2] << 8 | self->bytes[self->position + 3];
	} else {
		value = self->bytes[self->position + 3] << 24 | self->bytes[self->position + 2] << 16 | self->bytes[self->position + 1] << 8 | self->bytes[self->position];
	}
	self->position += 4;
	return value;
}

uint32_t BinaryDeserializationContext_readUInt32(BinaryDeserializationContext * self, const char * key) {
	lookUpKey(key, return 0)
	checkCanReadValue(4, return 0)
	return readUInt32Internal(self);
}

int64_t BinaryDeserializationContext_readInt64(BinaryDeserializationContext * self, const char * key) {
	int64_t value;
	
	lookUpKey(key, return 0)
	checkCanReadValue(8, return 0)
	if (self->bigEndian) {
		value = (int64_t) self->bytes[self->position] << 56 | (int64_t) self->bytes[self->position + 1] << 48 | (int64_t) self->bytes[self->position + 2] << 40 | (int64_t) self->bytes[self->position + 3] << 32 | (int64_t) self->bytes[self->position + 4] << 24 | (int64_t) self->bytes[self->position + 5] << 16 | (int64_t) self->bytes[self->position + 6] << 8 | (int64_t) self->bytes[self->position + 7];
	} else {
		value = (int64_t) self->bytes[self->position + 7] << 56 | (int64_t) self->bytes[self->position + 6] << 48 | (int64_t) self->bytes[self->position + 5] << 40 | (int64_t) self->bytes[self->position + 4] << 32 | (int64_t) self->bytes[self->position + 3] << 24 | (int64_t) self->bytes[self->position + 2] << 16 | (int64_t) self->bytes[self->position + 1] << 8 | (int64_t) self->bytes[self->position];
	}
	self->position += 8;
	return value;
}

uint64_t BinaryDeserializationContext_readUInt64(BinaryDeserializationContext * self, const char * key) {
	uint64_t value;
	
	lookUpKey(key, return 0)
	checkCanReadValue(8, return 0)
	if (self->bigEndian) {
		value = (uint64_t) self->bytes[self->position] << 56 | (uint64_t) self->bytes[self->position + 1] << 48 | (uint64_t) self->bytes[self->position + 2] << 40 | (uint64_t) self->bytes[self->position + 3] << 32 | (uint64_t) self->bytes[self->position + 4] << 24 | (uint64_t) self->bytes[self->position + 5] << 16 | (uint64_t) self->bytes[self->position + 6] << 8 | (uint64_t) self->bytes[self->position + 7];
	} else {
		value = (uint64_t) self->bytes[self->position + 7] << 56 | (uint64_t) self->bytes[self->position + 6] << 48 | (uint64_t) self->bytes[self->position + 5] << 40 | (uint64_t) self->bytes[self->position + 4] << 32 | (uint64_t) self->bytes[self->position + 3] << 24 | (uint64_t) self->bytes[self->position + 2] << 16 | (uint64_t) self->bytes[self->position + 1] << 8 | (uint64_t) self->bytes[self->position];
	}
	self->position += 8;
	return value;
}

float BinaryDeserializationContext_readFloat(BinaryDeserializationContext * self, const char * key) {
	uint32_t value;
	
	value = self->readUInt32(self, key);
	return *(float *) &value;
}

double BinaryDeserializationContext_readDouble(BinaryDeserializationContext * self, const char * key) {
	uint64_t value;
	
	value = self->readUInt64(self, key);
	return *(double *) &value;
}

const char * BinaryDeserializationContext_readString(BinaryDeserializationContext * self, const char * key) {
	lookUpKey(key, return NULL)
	checkCanReadValue(0, return NULL)
	return readStringInternal(self);
}

bool BinaryDeserializationContext_readBoolean(BinaryDeserializationContext * self, const char * key) {
	return self->readUInt8(self, key);
}

int BinaryDeserializationContext_readEnumeration(BinaryDeserializationContext * self, const char * key, ...) {
	int32_t value;
	va_list args;
	char * string;
	int namedValue, returnValue = 0;
	bool found = false;
	struct enumListItem {const char * name; int value;} * enumList;
	size_t enumListAllocatedSize, enumCount, enumIndex;
	
	value = self->readInt32(self, key);
	if (self->status != SERIALIZATION_ERROR_OK) {
		return 0;
	}
	
	va_start(args, key);
	enumCount = 0;
	enumListAllocatedSize = 1;
	enumList = malloc(sizeof(struct enumListItem) * enumListAllocatedSize);
	
	while ((string = va_arg(args, char *)) != NULL) {
		namedValue = va_arg(args, int);
		if (namedValue == value) {
			found = true;
			returnValue = value;
		}
		
		if (enumCount >= enumListAllocatedSize) {
			enumListAllocatedSize *= 2;
			enumList = realloc(enumList, sizeof(struct enumListItem) * enumListAllocatedSize);
		}
		enumList[enumCount].name = string;
		enumList[enumCount].value = namedValue;
		for (enumIndex = 0; enumIndex < enumCount; enumIndex++) {
			if (enumList[enumIndex].value == namedValue) {
				free(enumList);
				va_end(args);
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE, return 0)
				
			} else if (!strcmp(enumList[enumIndex].name, string)) {
				free(enumList);
				va_end(args);
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME, return 0)
			}
		}
		enumCount++;
	}
	va_end(args);
	free(enumList);
	
	if (!found) {
		failWithStatus(SERIALIZATION_ERROR_ENUM_NOT_NAMED, return 0);
	}
	return returnValue;
}

#define checkBitfieldErrors(bit_count) { \
	va_list args; \
	unsigned int bitIndex, bitIndex2; \
	char * strings[bit_count]; \
	\
	va_start(args, key); \
	for (bitIndex = 0; bitIndex < bit_count; bitIndex++) { \
		strings[bitIndex] = va_arg(args, char *); \
		if (strings[bitIndex] == NULL) { \
			break; \
		} \
		for (bitIndex2 = 0; bitIndex2 < bitIndex; bitIndex2++) { \
			if (!strcmp(strings[bitIndex2], strings[bitIndex])) { \
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_BIT, return 0) \
			} \
		} \
	} \
	va_end(args); \
	\
	if (bitIndex < bit_count && value & ~(((uint##bit_count##_t) 1 << bitIndex) - 1)) { \
		failWithStatus(SERIALIZATION_ERROR_UNNAMED_BIT, return 0) \
	} \
}

uint8_t BinaryDeserializationContext_readBitfield8(BinaryDeserializationContext * self, const char * key, ...) {
	uint8_t value;
	
	value = self->readUInt8(self, key);
	checkBitfieldErrors(8)
	return value;
}

uint16_t BinaryDeserializationContext_readBitfield16(BinaryDeserializationContext * self, const char * key, ...) {
	uint16_t value;
	
	value = self->readUInt16(self, key);
	checkBitfieldErrors(16)
	return value;
}

uint32_t BinaryDeserializationContext_readBitfield32(BinaryDeserializationContext * self, const char * key, ...) {
	uint32_t value;
	
	value = self->readUInt32(self, key);
	checkBitfieldErrors(32)
	return value;
}

uint64_t BinaryDeserializationContext_readBitfield64(BinaryDeserializationContext * self, const char * key, ...) {
	uint64_t value;
	
	value = self->readUInt64(self, key);
	checkBitfieldErrors(64)
	return value;
}

const char * BinaryDeserializationContext_readNextDictionaryKey(BinaryDeserializationContext * self) {
	size_t charIndex, lastCharIndex;
	uint32_t keyCount = 0;
	
	if (self->containerStack[self->containerCount - 1].type != BINARY_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) {
		failWithStatus(SERIALIZATION_ERROR_INVALID_OPERATION, return false)
	}
	if (self->containerStack[self->containerCount - 1].nextItemIndex >= self->containerStack[self->containerCount - 1].count) {
		failWithStatus(SERIALIZATION_ERROR_END_OF_CONTAINER, return NULL)
	}
	lastCharIndex = self->containerStack[self->containerCount - 1].startOffset + 8;
	for (charIndex = lastCharIndex; charIndex < self->length && keyCount < self->containerStack[self->containerCount - 1].count; charIndex++) {
		if (self->bytes[charIndex] == 0) {
			if (keyCount == self->containerStack[self->containerCount - 1].nextItemIndex) {
				self->containerStack[self->containerCount - 1].nextItemIndex++;
				return (char *) self->bytes + lastCharIndex;
			}
			charIndex += 4;
			keyCount++;
			lastCharIndex = charIndex + 1;
		}
	}
	return NULL;
}

bool BinaryDeserializationContext_hasDictionaryKey(BinaryDeserializationContext * self, const char * key) {
	size_t charIndex, lastCharIndex;
	uint32_t keyCount = 0;
	
	if (self->containerStack[self->containerCount - 1].type != BINARY_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) {
		failWithStatus(SERIALIZATION_ERROR_INVALID_OPERATION, return false)
	}
	
	lastCharIndex = self->containerStack[self->containerCount - 1].startOffset + 8;
	for (charIndex = lastCharIndex; charIndex < self->length && keyCount < self->containerStack[self->containerCount - 1].count; charIndex++) {
		if (self->bytes[charIndex] == 0) {
			if (!strcmp((char *) self->bytes + lastCharIndex, key)) {
				return true;
			}
			charIndex += 4;
			keyCount++;
			lastCharIndex = charIndex + 1;
		}
	}
	return false;
}
