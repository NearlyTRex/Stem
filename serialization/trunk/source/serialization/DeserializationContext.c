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

#include "serialization/DeserializationContext.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

bool DeserializationContext_init(DeserializationContext * self) {
	call_super(init, self);
	
	self->jmpBuf = NULL;
	self->status = SERIALIZATION_ERROR_OK;
	self->dispose = DeserializationContext_dispose;
	self->errorString = Serialization_errorString;
	self->beginStructure = NULL;
	self->beginDictionary = NULL;
	self->beginArray = NULL;
	self->endStructure = NULL;
	self->endDictionary = NULL;
	self->endArray = NULL;
	self->readBoolean = NULL;
	self->readInt8 = NULL;
	self->readUInt8 = NULL;
	self->readInt16 = NULL;
	self->readUInt16 = NULL;
	self->readInt32 = NULL;
	self->readUInt32 = NULL;
	self->readInt64 = NULL;
	self->readUInt64 = NULL;
	self->readFloat = NULL;
	self->readDouble = NULL;
	self->readEnumeration = NULL;
	self->readBitfield8 = NULL;
	self->readBitfield16 = NULL;
	self->readBitfield32 = NULL;
	self->readBitfield64 = NULL;
	self->readString = NULL;
	self->readBlob = NULL;
	self->readNextDictionaryKey = NULL;
	self->hasDictionaryKey = NULL;
	return true;
}

void DeserializationContext_dispose(DeserializationContext * self) {
	call_super(dispose, self);
}

#define failWithStatus(STATUS, RETURN_CODE) \
	self->status = (STATUS); \
	if (self->jmpBuf != NULL) { \
		longjmp(*self->jmpBuf, self->status); \
	} \
	RETURN_CODE;

#define checkBitfieldErrors(bit_count) \
	va_list argsCopy; \
	unsigned int bitIndex, bitIndex2; \
	const char * strings[bit_count]; \
	\
	va_copy(argsCopy, args); \
	for (bitIndex = 0; bitIndex < bit_count; bitIndex++) { \
		strings[bitIndex] = va_arg(argsCopy, char *); \
		if (strings[bitIndex] == NULL) { \
			break; \
		} \
		for (bitIndex2 = 0; bitIndex2 < bitIndex; bitIndex2++) { \
			if (!strcmp(strings[bitIndex2], strings[bitIndex])) { \
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_BIT, return false) \
			} \
		} \
	} \
	va_end(argsCopy); \
	\
	if (bitIndex < bit_count && value & ~(((uint##bit_count##_t) 1 << bitIndex) - 1)) { \
		failWithStatus(SERIALIZATION_ERROR_UNNAMED_BIT, return false) \
	} \
	return true

bool DeserializationContext_checkBitfield8Errors(DeserializationContext * self, va_list args, uint8_t value) {
	checkBitfieldErrors(8);
}

bool DeserializationContext_checkBitfield16Errors(DeserializationContext * self, va_list args, uint16_t value) {
	checkBitfieldErrors(16);
}

bool DeserializationContext_checkBitfield32Errors(DeserializationContext * self, va_list args, uint32_t value) {
	checkBitfieldErrors(32);
}

bool DeserializationContext_checkBitfield64Errors(DeserializationContext * self, va_list args, uint64_t value) {
	checkBitfieldErrors(64);
}

bool DeserializationContext_checkEnumerationErrors(DeserializationContext * self, va_list args, int value) {
	va_list argsCopy;
	const char * string;
	int namedValue;
	bool found = false;
	struct enumListItem {const char * name; int value;} * enumList;
	size_t enumListAllocatedSize, enumCount, enumIndex;
	
	va_copy(argsCopy, args);
	enumCount = 0;
	enumListAllocatedSize = 1;
	enumList = malloc(sizeof(struct enumListItem) * enumListAllocatedSize);
	
	while ((string = va_arg(argsCopy, const char *)) != NULL) {
		namedValue = va_arg(argsCopy, int);
		if (namedValue == value) {
			found = true;
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
				va_end(argsCopy);
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE, return false);
				
			} else if (!strcmp(enumList[enumIndex].name, string)) {
				free(enumList);
				va_end(argsCopy);
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME, return false);
			}
		}
		enumCount++;
	}
	va_end(argsCopy);
	free(enumList);
	
	if (!found) {
		failWithStatus(SERIALIZATION_ERROR_ENUM_NOT_NAMED, return false);
	}
	return true;
}
