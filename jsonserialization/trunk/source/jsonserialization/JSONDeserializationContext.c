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

#include "jsonserialization/JSONDeserializationContext.h"
#include "jsonio/JSONParser.h"
#include "utilities/Base64.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

#define SUPERCLASS DeserializationContext

JSONDeserializationContext * JSONDeserializationContext_createWithFile(const char * filePath) {
	stemobject_create_implementation(JSONDeserializationContext, initWithFile, filePath)
}

JSONDeserializationContext * JSONDeserializationContext_createWithString(const char * string, size_t length) {
	stemobject_create_implementation(JSONDeserializationContext, initWithString, string, length)
}

JSONDeserializationContext * JSONDeserializationContext_createWithJSONNode(struct JSONNode * node) {
	stemobject_create_implementation(JSONDeserializationContext, initWithJSONNode, node)
}

static void JSONDeserializationContext_init(JSONDeserializationContext * self) {
	call_super(init, self);
	self->dispose = JSONDeserializationContext_dispose;
	self->errorString = JSONSerialization_errorString;
	self->beginStructure = JSONDeserializationContext_beginStructure;
	self->beginDictionary = JSONDeserializationContext_beginDictionary;
	self->beginArray = JSONDeserializationContext_beginArray;
	self->endStructure = JSONDeserializationContext_endStructure;
	self->endDictionary = JSONDeserializationContext_endDictionary;
	self->endArray = JSONDeserializationContext_endArray;
	self->readBoolean = JSONDeserializationContext_readBoolean;
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
	self->readFixed16_16 = JSONDeserializationContext_readFixed16_16;
	self->readEnumeration = JSONDeserializationContext_readEnumeration;
	self->readBitfield8 = JSONDeserializationContext_readBitfield8;
	self->readBitfield16 = JSONDeserializationContext_readBitfield16;
	self->readBitfield32 = JSONDeserializationContext_readBitfield32;
	self->readBitfield64 = JSONDeserializationContext_readBitfield64;
	self->readString = JSONDeserializationContext_readString;
	self->readBlob = JSONDeserializationContext_readBlob;
	self->readNextDictionaryKey = JSONDeserializationContext_readNextDictionaryKey;
	self->hasDictionaryKey = JSONDeserializationContext_hasDictionaryKey;
	self->currentNode = NULL;
	self->nodeStack = NULL;
	self->nodeStackAllocatedSize = 0;
	self->nodeStackCurrentDepth = 0;
	self->finished = false;
	self->ownedBlobs = NULL;
	self->blobCount = 0;
}

bool JSONDeserializationContext_initWithFile(JSONDeserializationContext * self, const char * filePath) {
	JSONDeserializationContext_init(self);
	self->rootNode = JSONParser_loadFile(filePath, NULL);
	if (self->rootNode == NULL) {
		self->status = JSON_SERIALIZATION_ERROR_INVALID_JSON;
	}
	return true;
}

bool JSONDeserializationContext_initWithString(JSONDeserializationContext * self, const char * string, size_t length) {
	JSONDeserializationContext_init(self);
	self->rootNode = JSONParser_loadString(string, length, NULL);
	if (self->rootNode == NULL) {
		self->status = JSON_SERIALIZATION_ERROR_INVALID_JSON;
	}
	return true;
}

bool JSONDeserializationContext_initWithJSONNode(JSONDeserializationContext * self, struct JSONNode * node) {
	JSONDeserializationContext_init(self);
	if (node == NULL) {
		self->rootNode = NULL;
		self->status = JSON_SERIALIZATION_ERROR_INVALID_JSON;
	} else {
		self->rootNode = JSONNode_copy(node);
	}
	return true;
}

void JSONDeserializationContext_dispose(JSONDeserializationContext * self) {
	unsigned int blobIndex;
	
	if (self->rootNode != NULL) {
		JSONNode_dispose(self->rootNode);
	}
	free(self->nodeStack);
	for (blobIndex = 0; blobIndex < self->blobCount; blobIndex++) {
		free(self->ownedBlobs[blobIndex]);
	}
	free(self->ownedBlobs);
	call_super(dispose, self);
}

#define failWithStatus(STATUS, RETURN_CODE) \
	self->status = (STATUS); \
	if (self->jmpBuf != NULL) { \
		longjmp(*self->jmpBuf, self->status); \
	} \
	RETURN_CODE;

#define getNextNodeIndex(FAIL_RETURN_VALUE) \
	if (self->currentNode == NULL || self->finished) { \
		failWithStatus(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, return FAIL_RETURN_VALUE) \
	} \
	if (self->currentNode->type == JSON_TYPE_OBJECT) { \
		size_t subitemIndex; \
		bool found = false; \
		\
		if (key == NULL) { \
			failWithStatus(SERIALIZATION_ERROR_NULL_KEY, return FAIL_RETURN_VALUE) \
		} \
		for (subitemIndex = 0; subitemIndex < self->currentNode->value.count; subitemIndex++) { \
			if (self->currentNode->subitems[subitemIndex].key == key) { \
				nextNodeIndex = subitemIndex; \
				found = true; \
				break; \
			} \
		} \
		if (!found) { \
			for (subitemIndex = 0; subitemIndex < self->currentNode->value.count; subitemIndex++) { \
				if (!strcmp(self->currentNode->subitems[subitemIndex].key, key)) { \
					nextNodeIndex = subitemIndex; \
					found = true; \
					break; \
				} \
			} \
		} \
		if (!found) { \
			failWithStatus(SERIALIZATION_ERROR_KEY_NOT_FOUND, return FAIL_RETURN_VALUE) \
		} \
		\
	} else { \
		nextNodeIndex = self->nextNodeIndex++; \
		if (nextNodeIndex >= self->currentNode->value.count) { \
			failWithStatus(SERIALIZATION_ERROR_END_OF_CONTAINER, return FAIL_RETURN_VALUE) \
		} \
	}

void JSONDeserializationContext_beginStructure(JSONDeserializationContext * self, const char * key) {
	size_t subitemIndex, subitemIndex2;
	
	if (self->finished) {
		failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return)
	}
	
	if (self->currentNode == NULL) {
		if (self->rootNode->type != JSON_TYPE_OBJECT) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return)
		}
		self->currentNode = self->rootNode;
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE;
		
	} else {
		size_t nextNodeIndex = 0;
		
		getNextNodeIndex()
		
		if (self->currentNode->subitems[nextNodeIndex].type != JSON_TYPE_OBJECT) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return)
		}
		
		if (self->nodeStackAllocatedSize <= self->nodeStackCurrentDepth) {
			self->nodeStackAllocatedSize++;
			self->nodeStack = realloc(self->nodeStack, sizeof(struct JSONDeserializationContext_nodeStackItem) * self->nodeStackAllocatedSize);
		}
		self->nodeStack[self->nodeStackCurrentDepth].node = self->currentNode;
		self->nodeStack[self->nodeStackCurrentDepth].index = nextNodeIndex + 1;
		self->nodeStack[self->nodeStackCurrentDepth].containerType = self->currentContainerType;
		self->nodeStackCurrentDepth++;
		
		self->currentNode = &self->currentNode->subitems[nextNodeIndex];
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE;
	}
	self->nextNodeIndex = 0;
	
	for (subitemIndex = 0; subitemIndex < self->currentNode->value.count; subitemIndex++) {
		for (subitemIndex2 = 0; subitemIndex2 < subitemIndex; subitemIndex2++) {
			if (!strcmp(self->currentNode->subitems[subitemIndex].key, self->currentNode->subitems[subitemIndex2].key)) {
				failWithStatus(JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY, return)
			}
		}
	}
}

size_t JSONDeserializationContext_beginDictionary(JSONDeserializationContext * self, const char * key) {
	if (self->finished) {
		failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return 0)
	}
	
	if (self->currentNode == NULL) {
		if (self->rootNode->type != JSON_TYPE_OBJECT) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0)
		}
		self->currentNode = self->rootNode;
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY;
		
	} else {
		size_t nextNodeIndex = 0;
		
		getNextNodeIndex(0)
		
		if (self->currentNode->subitems[nextNodeIndex].type != JSON_TYPE_OBJECT) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0)
		}
		
		if (self->nodeStackAllocatedSize <= self->nodeStackCurrentDepth) {
			self->nodeStackAllocatedSize++;
			self->nodeStack = realloc(self->nodeStack, sizeof(struct JSONDeserializationContext_nodeStackItem) * self->nodeStackAllocatedSize);
		}
		self->nodeStack[self->nodeStackCurrentDepth].node = self->currentNode;
		self->nodeStack[self->nodeStackCurrentDepth].index = nextNodeIndex + 1;
		self->nodeStack[self->nodeStackCurrentDepth].containerType = self->currentContainerType;
		self->nodeStackCurrentDepth++;
		
		self->currentNode = &self->currentNode->subitems[nextNodeIndex];
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY;
	}
	self->nextNodeIndex = 0;
	
	return self->currentNode->value.count;
}

size_t JSONDeserializationContext_beginArray(JSONDeserializationContext * self, const char * key) {
	if (self->finished) {
		failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return 0)
	}
	
	if (self->currentNode == NULL) {
		if (self->rootNode->type != JSON_TYPE_ARRAY) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0)
		}
		self->currentNode = self->rootNode;
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY;
		
	} else {
		size_t nextNodeIndex = 0;
		
		getNextNodeIndex(0)
		
		if (self->currentNode->subitems[nextNodeIndex].type != JSON_TYPE_ARRAY) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0)
		}
		
		if (self->nodeStackAllocatedSize <= self->nodeStackCurrentDepth) {
			self->nodeStackAllocatedSize++;
			self->nodeStack = realloc(self->nodeStack, sizeof(struct JSONDeserializationContext_nodeStackItem) * self->nodeStackAllocatedSize);
		}
		self->nodeStack[self->nodeStackCurrentDepth].node = self->currentNode;
		self->nodeStack[self->nodeStackCurrentDepth].index = nextNodeIndex + 1;
		self->nodeStack[self->nodeStackCurrentDepth].containerType = self->currentContainerType;
		self->nodeStackCurrentDepth++;
		
		self->currentNode = &self->currentNode->subitems[nextNodeIndex];
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY;
	}
	self->nextNodeIndex = 0;
	
	return self->currentNode->value.count;
}

#define _failIfNotCurrentContainerType(TYPE) \
	if (self->currentContainerType != (TYPE)) { \
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return) \
	}

#define _failIfContainerNotFullyRead() \
	if (self->nextNodeIndex < self->currentNode->value.count) { \
		failWithStatus(JSON_SERIALIZATION_ERROR_CONTAINER_NOT_FULLY_READ, return) \
	}

void JSONDeserializationContext_endStructure(JSONDeserializationContext * self) {
	if (self->nodeStackCurrentDepth > 0) {
		_failIfNotCurrentContainerType(JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE)
		self->currentNode = self->nodeStack[--self->nodeStackCurrentDepth].node;
		self->nextNodeIndex = self->nodeStack[self->nodeStackCurrentDepth].index;
		self->currentContainerType = self->nodeStack[self->nodeStackCurrentDepth].containerType;
		
	} else if (self->currentNode == NULL || self->finished) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,)
		
	} else {
		_failIfNotCurrentContainerType(JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE)
		self->finished = true;
	}
}

void JSONDeserializationContext_endDictionary(JSONDeserializationContext * self) {
	if (self->nodeStackCurrentDepth > 0) {
		_failIfNotCurrentContainerType(JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY)
		self->currentNode = self->nodeStack[--self->nodeStackCurrentDepth].node;
		self->nextNodeIndex = self->nodeStack[self->nodeStackCurrentDepth].index;
		self->currentContainerType = self->nodeStack[self->nodeStackCurrentDepth].containerType;
		
	} else if (self->currentNode == NULL || self->finished) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,)
		
	} else {
		_failIfNotCurrentContainerType(JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY)
		self->finished = true;
	}
}

void JSONDeserializationContext_endArray(JSONDeserializationContext * self) {
	if (self->nodeStackCurrentDepth > 0) {
		_failIfNotCurrentContainerType(JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY)
		_failIfContainerNotFullyRead()
		self->currentNode = self->nodeStack[--self->nodeStackCurrentDepth].node;
		self->nextNodeIndex = self->nodeStack[self->nodeStackCurrentDepth].index;
		self->currentContainerType = self->nodeStack[self->nodeStackCurrentDepth].containerType;
		
	} else if (self->currentNode == NULL || self->finished) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,)
		
	} else {
		_failIfNotCurrentContainerType(JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY)
		_failIfContainerNotFullyRead()
		self->finished = true;
	}
}

#undef _failIfNotCurrentContainerType

#define _failIfNotOfType(TYPE, RETURN_VALUE) \
	if (self->currentNode->subitems[nextNodeIndex].type != (TYPE)) { \
		failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return (RETURN_VALUE)) \
	}

#define _failIfNumberOutOfRange(MIN, MAX) \
	if (self->currentNode->subitems[nextNodeIndex].value.number < (MIN) || self->currentNode->subitems[nextNodeIndex].value.number > (MAX)) { \
		failWithStatus(SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,) \
	}

bool JSONDeserializationContext_readBoolean(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_BOOLEAN, 0)
	return self->currentNode->subitems[nextNodeIndex].value.boolean;
}

int8_t JSONDeserializationContext_readInt8(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_NUMBER, 0)
	_failIfNumberOutOfRange(INT8_MIN, INT8_MAX)
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

uint8_t JSONDeserializationContext_readUInt8(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_NUMBER, 0)
	_failIfNumberOutOfRange(0, UINT8_MAX)
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

int16_t JSONDeserializationContext_readInt16(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_NUMBER, 0)
	_failIfNumberOutOfRange(INT16_MIN, INT16_MAX)
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

uint16_t JSONDeserializationContext_readUInt16(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_NUMBER, 0)
	_failIfNumberOutOfRange(0, UINT16_MAX)
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

int32_t JSONDeserializationContext_readInt32(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_NUMBER, 0)
	_failIfNumberOutOfRange(INT32_MIN, INT32_MAX)
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

uint32_t JSONDeserializationContext_readUInt32(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_NUMBER, 0)
	_failIfNumberOutOfRange(0, UINT32_MAX)
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

int64_t JSONDeserializationContext_readInt64(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	if (self->currentNode->subitems[nextNodeIndex].type != JSON_TYPE_NUMBER && self->currentNode->subitems[nextNodeIndex].type != JSON_TYPE_STRING) {
		failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0)
	}
	_failIfNumberOutOfRange(INT64_MIN, INT64_MAX)
	if (self->currentNode->subitems[nextNodeIndex].type == JSON_TYPE_STRING) {
		int64_t value = 0;
		double doubleValue;
		int conversions, charsConsumed;
		
		conversions = sscanf(self->currentNode->subitems[nextNodeIndex].value.string,
#ifdef __WIN32
		                     "%I64d%n", &value,
#elif defined(linux) && defined(_LP64)
		                     "%ld%n", &value,
#else
		                     "%lld%n", &value,
#endif
		                     &charsConsumed);
		if (conversions != 1 || (size_t) charsConsumed != self->currentNode->subitems[nextNodeIndex].stringLength) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE,)
		}
		doubleValue = value;
		if ((int64_t) doubleValue == value) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE,)
		}
		return value;
	}
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

uint64_t JSONDeserializationContext_readUInt64(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	if (self->currentNode->subitems[nextNodeIndex].type != JSON_TYPE_NUMBER && self->currentNode->subitems[nextNodeIndex].type != JSON_TYPE_STRING) {
		failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0)
	}
	_failIfNumberOutOfRange(0, UINT64_MAX)
	if (self->currentNode->subitems[nextNodeIndex].type == JSON_TYPE_STRING) {
		uint64_t value = 0;
		double doubleValue;
		int conversions, charsConsumed;
		
		conversions = sscanf(self->currentNode->subitems[nextNodeIndex].value.string,
#ifdef __WIN32
		                     "%I64u%n", &value,
#elif defined(linux) && defined(_LP64)
		                     "%lu%n", &value,
#else
		                     "%llu%n", &value,
#endif
		                     &charsConsumed);
		if (conversions != 1 || (size_t) charsConsumed != self->currentNode->subitems[nextNodeIndex].stringLength) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE,)
		}
		doubleValue = value;
		if ((uint64_t) doubleValue == value) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE,)
		}
		return value;
	}
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

float JSONDeserializationContext_readFloat(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_NUMBER, 0)
	_failIfNumberOutOfRange(-FLT_MAX, FLT_MAX)
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

double JSONDeserializationContext_readDouble(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_NUMBER, 0)
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

fixed16_16 JSONDeserializationContext_readFixed16_16(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	if (self->currentNode->subitems[nextNodeIndex].type != JSON_TYPE_NUMBER && self->currentNode->subitems[nextNodeIndex].type != JSON_TYPE_STRING) {
		failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0)
	}
	if (self->currentNode->subitems[nextNodeIndex].type == JSON_TYPE_STRING) {
		unsigned int value = 0;
		int conversions, charsConsumed;
		
		conversions = sscanf(self->currentNode->subitems[nextNodeIndex].value.string, "%x%n", &value, &charsConsumed);
		if (conversions != 1 || (size_t) charsConsumed != self->currentNode->subitems[nextNodeIndex].stringLength) {
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE,)
		}
		return value;
	}
	return self->currentNode->subitems[nextNodeIndex].value.number;
}

int JSONDeserializationContext_readEnumeration(JSONDeserializationContext * self, const char * key, ...) {
	va_list args;
	const char * enumName;
	bool found = false;
	int enumValue, returnedEnumValue = 0;
	size_t nextNodeIndex = 0;
	struct enumListItem {const char * name; int value;} * enumList;
	size_t enumListAllocatedSize, enumCount, enumIndex;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_STRING, 0)
	
	va_start(args, key);
	enumCount = 0;
	enumListAllocatedSize = 1;
	enumList = malloc(sizeof(struct enumListItem) * enumListAllocatedSize);
	
	for (;;) {
		enumName = va_arg(args, const char *);
		if (enumName == NULL) {
			break;
		}
		enumValue = va_arg(args, int);
		if (!strcmp(enumName, self->currentNode->subitems[nextNodeIndex].value.string)) {
			found = true;
			returnedEnumValue = enumValue;
		}
		
		if (enumCount >= enumListAllocatedSize) {
			enumListAllocatedSize *= 2;
			enumList = realloc(enumList, sizeof(struct enumListItem) * enumListAllocatedSize);
		}
		enumList[enumCount].name = enumName;
		enumList[enumCount].value = enumValue;
		for (enumIndex = 0; enumIndex < enumCount; enumIndex++) {
			if (enumList[enumIndex].value == enumValue) {
				free(enumList);
				va_end(args);
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE,)
				return 0;
				
			} else if (!strcmp(enumList[enumIndex].name, enumName)) {
				free(enumList);
				va_end(args);
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME,)
				return 0;
			}
		}
		enumCount++;
	}
	
	va_end(args);
	free(enumList);
	
	if (!found) {
		failWithStatus(SERIALIZATION_ERROR_ENUM_NOT_NAMED, return 0)
	}
	return returnedEnumValue;
}

#define initialBits8 0x00
#define initialBits16 0x0000
#define initialBits32 0x00000000
#define initialBits64 0x0000000000000000ull
#define readBitfieldImplementation(NBITS) \
	va_list args; \
	unsigned int bitIndex, numBitsMatched = 0; \
	const char * bitName; \
	size_t subitemIndex, subitemIndex2; \
	uint##NBITS##_t bits = initialBits##NBITS; \
	size_t nextNodeIndex = 0; \
	\
	getNextNodeIndex(0) \
	_failIfNotOfType(JSON_TYPE_ARRAY, 0) \
	if (self->currentNode->subitems[nextNodeIndex].value.count > NBITS) { \
		failWithStatus(JSON_SERIALIZATION_ERROR_TOO_MANY_BITS, return 0) \
	} \
	for (subitemIndex = 0; subitemIndex < self->currentNode->subitems[nextNodeIndex].value.count; subitemIndex++) { \
		if (self->currentNode->subitems[nextNodeIndex].subitems[subitemIndex].type != JSON_TYPE_STRING) { \
			failWithStatus(SERIALIZATION_ERROR_INCORRECT_TYPE, return 0) \
		} \
		for (subitemIndex2 = 0; subitemIndex2 < subitemIndex; subitemIndex2++) { \
			if (!strcmp(self->currentNode->subitems[nextNodeIndex].subitems[subitemIndex].value.string, self->currentNode->subitems[nextNodeIndex].subitems[subitemIndex2].value.string)) { \
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_BIT, return 0) \
			} \
		} \
	} \
	va_start(args, key); \
	for (bitIndex = 0; bitIndex < NBITS; bitIndex++) { \
		bitName = va_arg(args, const char *); \
		if (bitName == NULL) { \
			break; \
		} \
		for (subitemIndex = 0; subitemIndex < self->currentNode->subitems[nextNodeIndex].value.count; subitemIndex++) { \
			if (!strcmp(bitName, self->currentNode->subitems[nextNodeIndex].subitems[subitemIndex].value.string)) { \
				bits |= (uint##NBITS##_t) 1 << bitIndex; \
				numBitsMatched++; \
				break; \
			} \
		} \
	} \
	va_end(args); \
	\
	if (numBitsMatched < self->currentNode->subitems[nextNodeIndex].value.count) { \
		failWithStatus(SERIALIZATION_ERROR_UNNAMED_BIT,) \
	} \
	\
	return bits;

uint8_t JSONDeserializationContext_readBitfield8(JSONDeserializationContext * self, const char * key, ...) {
	readBitfieldImplementation(8)
}

uint16_t JSONDeserializationContext_readBitfield16(JSONDeserializationContext * self, const char * key, ...) {
	readBitfieldImplementation(16)
}

uint32_t JSONDeserializationContext_readBitfield32(JSONDeserializationContext * self, const char * key, ...) {
	readBitfieldImplementation(32)
}

uint64_t JSONDeserializationContext_readBitfield64(JSONDeserializationContext * self, const char * key, ...) {
	readBitfieldImplementation(64)
}

const char * JSONDeserializationContext_readString(JSONDeserializationContext * self, const char * key) {
	size_t nextNodeIndex = 0;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_STRING, NULL)
	return self->currentNode->subitems[nextNodeIndex].value.string;
}

const void * JSONDeserializationContext_readBlob(JSONDeserializationContext * self, const char * key, size_t * outLength) {
	size_t nextNodeIndex = 0;
	size_t decodedLength;
	
	getNextNodeIndex(0)
	_failIfNotOfType(JSON_TYPE_STRING, NULL)
	self->ownedBlobs = realloc(self->ownedBlobs, sizeof(void *) * (self->blobCount + 1));
	decodedLength = decodeBase64(self->currentNode->subitems[nextNodeIndex].value.string, self->currentNode->subitems[nextNodeIndex].stringLength, NULL, 0);
	decodeBase64(self->currentNode->subitems[nextNodeIndex].value.string, self->currentNode->subitems[nextNodeIndex].stringLength, self->ownedBlobs[self->blobCount] = malloc(decodedLength), decodedLength);
	*outLength = decodedLength;
	return self->ownedBlobs[self->blobCount++];
}

#undef _failIfNotOfType
#undef _failIfNumberOutOfRange

const char * JSONDeserializationContext_readNextDictionaryKey(JSONDeserializationContext * self) {
	if (self->currentContainerType != JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) {
		failWithStatus(SERIALIZATION_ERROR_INVALID_OPERATION, return NULL)
	}
	
	if (self->nextNodeIndex >= self->currentNode->value.count) {
		failWithStatus(SERIALIZATION_ERROR_END_OF_CONTAINER, return NULL)
	}
	return self->currentNode->subitems[self->nextNodeIndex++].key;
}

bool JSONDeserializationContext_hasDictionaryKey(JSONDeserializationContext * self, const char * key) {
	size_t subitemIndex;
	
	if (self->currentContainerType != JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) {
		failWithStatus(SERIALIZATION_ERROR_INVALID_OPERATION, return false)
	}
	
	for (subitemIndex = 0; subitemIndex < self->currentNode->value.count; subitemIndex++) {
		if (!strcmp(self->currentNode->subitems[subitemIndex].key, key)) {
			return true;
		}
	}
	return false;
}
