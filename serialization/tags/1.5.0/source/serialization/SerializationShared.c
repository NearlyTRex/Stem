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

#include "serialization/SerializationShared.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

const char * Serialization_errorString(int status) {
	switch (status) {
		case SERIALIZATION_ERROR_OK:
			return "No error";
			
		case SERIALIZATION_ERROR_INVALID_OPERATION:
			return "Incorrect API usage";
			
		case SERIALIZATION_ERROR_KEY_NOT_FOUND:
			return "The requested key is not present in the current dictionary or structure";
			
		case SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH:
			return "A call to begin<Array|Structure|Dictionary> wasn't paired with the correct one of end<Array|Structure|Dictionary>";
			
		case SERIALIZATION_ERROR_CONTAINER_UNDERFLOW:
			return "More calls to end<Array|Structure|Dictionary> than to begin<Array|Structure|Dictionary>";
			
		case SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS:
			return "After a matched number of calls to <begin|end><Array|Structure|Dictionary>, begin<Array|Structure|Dictionary> is called again";
			
		case SERIALIZATION_ERROR_UNNAMED_BIT:
			return "In a call to <read|write>Bitfield<8|16|32|64>, one or more of set bits isn't named in function arguments";
			
		case SERIALIZATION_ERROR_DUPLICATE_BIT:
			return "In a call to <read|write>Bitfield<8|16|32|64>, multiple bits are given the same name";
			
		case SERIALIZATION_ERROR_ENUM_NOT_NAMED:
			return "In a call to <read|write>Enumeration, the enumeration value is not named";
			
		case SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME:
			return "In a call to <read|write>Enumeration, multiple enumeration values are given the same name";
			
		case SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE:
			return "In a call to <read|write>Enumeration, multiple enumeration names are given the same value";
			
		case SERIALIZATION_ERROR_NULL_KEY:
			return "key parameter is NULL when reading from/writing to a structure or dictionary";
			
		case SERIALIZATION_ERROR_INCORRECT_TYPE:
			return "The value being deserialized is not of the requested data type";
			
		case SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE:
			return "The number value being deserialized is too large or small to fit in the requested data type";
			
		case SERIALIZATION_ERROR_END_OF_CONTAINER:
			return "More fields requested for deserialization than exist in the container being deserialized";
			
		case SERIALIZATION_ERROR_NO_CONTAINER_STARTED:
			return "A read or write call was issued before the first begin<Array|Structure|Dictionary> call, or after the last end<Array|Structure|Dictionary> call";
			
		case SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER:
			return "An attempt was made to save data without having serialized anything";
			
		default:
			return "Unknown error";
	}
}

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
				return SERIALIZATION_ERROR_DUPLICATE_BIT; \
			} \
		} \
	} \
	va_end(argsCopy); \
	\
	if (bitIndex < bit_count && value & ~(((uint##bit_count##_t) 1 << bitIndex) - 1)) { \
		return SERIALIZATION_ERROR_UNNAMED_BIT; \
	} \
	return SERIALIZATION_ERROR_OK

int Serialization_checkBitfield8Errors(uint8_t value, va_list args) {
	checkBitfieldErrors(8);
}

int Serialization_checkBitfield16Errors(uint16_t value, va_list args) {
	checkBitfieldErrors(16);
}

int Serialization_checkBitfield32Errors(uint32_t value, va_list args) {
	checkBitfieldErrors(32);
}

int Serialization_checkBitfield64Errors(uint64_t value, va_list args) {
	checkBitfieldErrors(64);
}

int Serialization_checkEnumerationErrors(int value, va_list args) {
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
				return SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE;
				
			} else if (!strcmp(enumList[enumIndex].name, string)) {
				free(enumList);
				va_end(argsCopy);
				return SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME;
			}
		}
		enumCount++;
	}
	va_end(argsCopy);
	free(enumList);
	
	if (!found) {
		return SERIALIZATION_ERROR_ENUM_NOT_NAMED;
	}
	return SERIALIZATION_ERROR_OK;
}
