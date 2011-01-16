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

#include "jsonserialization/JSONSerializationContext.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define failWithStatus(STATUS, RETURN_CODE) \
	self->status = (STATUS); \
	if (self->jmpBuf != NULL) { \
		longjmp(*self->jmpBuf, self->status); \
	} \
	RETURN_CODE;

#define writeNode(TARGET, CONTAINER, KEY, TYPE, VALUE_FIELD, VALUE) \
	(TARGET).type = (TYPE); \
	if ((CONTAINER) != NULL && (CONTAINER)->type == JSON_TYPE_OBJECT) { \
		if ((KEY) == NULL) { \
			failWithStatus(SERIALIZATION_ERROR_NULL_KEY, return) \
		} \
		if (self->currentContainerType == JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE) { \
			size_t nodeIndex; \
			for (nodeIndex = 0; nodeIndex < (CONTAINER)->value.count; nodeIndex++) { \
				if (!strcmp((CONTAINER)->subitems[nodeIndex].key, (KEY))) { \
					failWithStatus(JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY, return) \
				} \
			} \
		} \
		(TARGET).keyLength = strlen(KEY); \
		(TARGET).key = malloc((TARGET).keyLength + 1); \
		strcpy((TARGET).key, (KEY)); \
	} else { \
		(TARGET).key = NULL; \
	} \
	(TARGET).value.VALUE_FIELD = (VALUE);

#define writeNodeCommonArgs(TYPE, VALUE_FIELD) writeNode(self->currentNode->subitems[self->currentNode->value.count], self->currentNode, key, TYPE, VALUE_FIELD, value)

#define failIfContainerNotStarted() \
	if (self->currentNode == NULL) { \
		failWithStatus(SERIALIAZTION_ERROR_NO_CONTAINER_STARTED, return) \
	}

JSONSerializationContext * JSONSerializationContext_create() {
	stemobject_create_implementation(JSONSerializationContext, init)
}

void JSONSerializationContext_init(compat_type(JSONSerializationContext *) selfPtr) {
	JSONSerializationContext * self = selfPtr;
	
	SerializationContext_init((SerializationContext *) self);
	self->dispose = JSONSerializationContext_dispose;
	self->beginStructure = JSONSerializationContext_beginStructure;
	self->beginDictionary = JSONSerializationContext_beginDictionary;
	self->beginArray = JSONSerializationContext_beginArray;
	self->endStructure = JSONSerializationContext_endStructure;
	self->endDictionary = JSONSerializationContext_endDictionary;
	self->endArray = JSONSerializationContext_endArray;
	self->writeInt8 = JSONSerializationContext_writeInt8;
	self->writeUInt8 = JSONSerializationContext_writeUInt8;
	self->writeInt16 = JSONSerializationContext_writeInt16;
	self->writeUInt16 = JSONSerializationContext_writeUInt16;
	self->writeInt32 = JSONSerializationContext_writeInt32;
	self->writeUInt32 = JSONSerializationContext_writeUInt32;
	self->writeInt64 = JSONSerializationContext_writeInt64;
	self->writeUInt64 = JSONSerializationContext_writeUInt64;
	self->writeFloat = JSONSerializationContext_writeFloat;
	self->writeDouble = JSONSerializationContext_writeDouble;
	self->writeString = JSONSerializationContext_writeString;
	self->writeBoolean = JSONSerializationContext_writeBoolean;
	self->writeEnumeration = JSONSerializationContext_writeEnumeration;
	self->writeBitfield8 = JSONSerializationContext_writeBitfield8;
	self->writeBitfield16 = JSONSerializationContext_writeBitfield16;
	self->writeBitfield32 = JSONSerializationContext_writeBitfield32;
	self->writeBitfield64 = JSONSerializationContext_writeBitfield64;
	self->writeToFile = JSONSerializationContext_writeToFile;
	self->writeToString = JSONSerializationContext_writeToString;
	self->writeToJSONNode = JSONSerializationContext_writeToJSONNode;
	
	self->rootNode = NULL;
	self->currentNode = NULL;
	self->nodeStack = NULL;
	self->nodeStackAllocatedSize = 0;
	self->nodeStackCurrentDepth = 0;
	self->rootNodeOwnedBySelf = true;
}

void JSONSerializationContext_dispose(compat_type(JSONSerializationContext *) selfPtr) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->rootNodeOwnedBySelf && self->rootNode != NULL) {
		JSONNode_dispose(self->rootNode);
	}
	free(self->nodeStack);
	SerializationContext_dispose((SerializationContext *) self);
}

char * JSONSerializationContext_writeToString(compat_type(JSONSerializationContext *) selfPtr, enum JSONEmitterFormat format, size_t * outLength, struct JSONEmissionError * outError) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->currentNode != NULL || self->status != SERIALIZATION_ERROR_OK) {
		return NULL;
	}
	if (self->rootNode == NULL) {
		self->status = JSON_SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER;
		return NULL;
	}
	return JSONEmitter_writeString(self->rootNode, format, outLength, outError);
}

bool JSONSerializationContext_writeToFile(compat_type(JSONSerializationContext *) selfPtr, enum JSONEmitterFormat format, const char * filePath, struct JSONEmissionError * outError) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->currentNode != NULL || self->status != SERIALIZATION_ERROR_OK) {
		return NULL;
	}
	if (self->rootNode == NULL) {
		self->status = JSON_SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER;
		return NULL;
	}
	return JSONEmitter_writeFile(self->rootNode, format, filePath, outError);
}

struct JSONNode * JSONSerializationContext_writeToJSONNode(compat_type(JSONSerializationContext *) selfPtr) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->currentNode != NULL || self->status != SERIALIZATION_ERROR_OK) {
		return NULL;
	}
	if (self->rootNode == NULL) {
		self->status = JSON_SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER;
	}
	if (self->rootNodeOwnedBySelf) {
		self->rootNodeOwnedBySelf = false;
		return self->rootNode;
	}
	
	return JSONNode_copy(self->rootNode);
}

void JSONSerializationContext_beginStructure(compat_type(JSONSerializationContext *) selfPtr, const char * key) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->rootNode == NULL) {
		self->rootNode = malloc(sizeof(struct JSONNode));
		self->currentNode = self->rootNode;
		self->rootContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE;
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE;
		self->rootNode->type = JSON_TYPE_NULL;
		self->rootNode->subitems = NULL;
		writeNode(*self->rootNode, (struct JSONNode *) NULL, key, JSON_TYPE_OBJECT, count, 0);
		
	} else {
		if (self->currentNode == NULL) {
			failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return)
		}
		if (self->nodeStackAllocatedSize <= self->nodeStackCurrentDepth) {
			self->nodeStackAllocatedSize++;
			self->nodeStack = realloc(self->nodeStack, sizeof(struct JSONSerializationContext_nodeStackItem) * self->nodeStackAllocatedSize);
		}
		self->nodeStack[self->nodeStackCurrentDepth].node = self->currentNode;
		self->nodeStack[self->nodeStackCurrentDepth].containerType = JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE;
		self->nodeStackCurrentDepth++;
		
		self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
		self->currentNode->subitems[self->currentNode->value.count].type = JSON_TYPE_NULL;
		self->currentNode->subitems[self->currentNode->value.count].subitems = NULL;
		writeNode(self->currentNode->subitems[self->currentNode->value.count], self->currentNode, key, JSON_TYPE_OBJECT, count, 0);
		self->currentNode = &self->currentNode->subitems[self->currentNode->value.count++];
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE;
	}
}

void JSONSerializationContext_beginDictionary(compat_type(JSONSerializationContext *) selfPtr, const char * key) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->rootNode == NULL) {
		self->rootNode = malloc(sizeof(struct JSONNode));
		self->currentNode = self->rootNode;
		self->rootContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY;
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY;
		self->rootNode->type = JSON_TYPE_NULL;
		self->rootNode->subitems = NULL;
		writeNode(*self->rootNode, (struct JSONNode *) NULL, key, JSON_TYPE_OBJECT, count, 0);
		
	} else {
		if (self->currentNode == NULL) {
			failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return)
		}
		if (self->nodeStackAllocatedSize <= self->nodeStackCurrentDepth) {
			self->nodeStackAllocatedSize++;
			self->nodeStack = realloc(self->nodeStack, sizeof(struct JSONSerializationContext_nodeStackItem) * self->nodeStackAllocatedSize);
		}
		self->nodeStack[self->nodeStackCurrentDepth].node = self->currentNode;
		self->nodeStack[self->nodeStackCurrentDepth].containerType = JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY;
		self->nodeStackCurrentDepth++;
		
		self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
		self->currentNode->subitems[self->currentNode->value.count].type = JSON_TYPE_NULL;
		self->currentNode->subitems[self->currentNode->value.count].subitems = NULL;
		writeNode(self->currentNode->subitems[self->currentNode->value.count], self->currentNode, key, JSON_TYPE_OBJECT, count, 0);
		self->currentNode = &self->currentNode->subitems[self->currentNode->value.count++];
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY;
	}
}

void JSONSerializationContext_beginArray(compat_type(JSONSerializationContext *) selfPtr, const char * key) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->rootNode == NULL) {
		self->rootNode = malloc(sizeof(struct JSONNode));
		self->currentNode = self->rootNode;
		self->rootContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY;
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY;
		self->rootNode->type = JSON_TYPE_NULL;
		self->rootNode->subitems = NULL;
		writeNode(*self->rootNode, (struct JSONNode *) NULL, key, JSON_TYPE_ARRAY, count, 0);
		
	} else {
		if (self->currentNode == NULL) {
			failWithStatus(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS, return)
		}
		if (self->nodeStackAllocatedSize <= self->nodeStackCurrentDepth) {
			self->nodeStackAllocatedSize++;
			self->nodeStack = realloc(self->nodeStack, sizeof(struct JSONSerializationContext_nodeStackItem) * self->nodeStackAllocatedSize);
		}
		self->nodeStack[self->nodeStackCurrentDepth].node = self->currentNode;
		self->nodeStack[self->nodeStackCurrentDepth].containerType = JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY;
		self->nodeStackCurrentDepth++;
		
		self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
		self->currentNode->subitems[self->currentNode->value.count].type = JSON_TYPE_NULL;
		self->currentNode->subitems[self->currentNode->value.count].subitems = NULL;
		writeNode(self->currentNode->subitems[self->currentNode->value.count], self->currentNode, key, JSON_TYPE_ARRAY, count, 0);
		self->currentNode = &self->currentNode->subitems[self->currentNode->value.count++];
		self->currentContainerType = JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY;
	}
}

void JSONSerializationContext_endStructure(compat_type(JSONSerializationContext *) selfPtr) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->currentNode == NULL) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, return)
	}
	
	if (self->nodeStackCurrentDepth > 0) {
		if (self->nodeStack[self->nodeStackCurrentDepth - 1].containerType != JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE) {
			failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return)
		}
		self->currentNode = self->nodeStack[--self->nodeStackCurrentDepth].node;
		self->currentContainerType = self->nodeStack[self->nodeStackCurrentDepth].containerType;
		
	} else {
		if (self->rootContainerType != JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE) {
			failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return)
		}
		self->currentNode = NULL;
	}
}

void JSONSerializationContext_endDictionary(compat_type(JSONSerializationContext *) selfPtr) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->currentNode == NULL) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, return)
	}
	
	if (self->nodeStackCurrentDepth > 0) {
		if (self->nodeStack[self->nodeStackCurrentDepth - 1].containerType != JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) {
			failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return)
		}
		self->currentNode = self->nodeStack[--self->nodeStackCurrentDepth].node;
		self->currentContainerType = self->nodeStack[self->nodeStackCurrentDepth].containerType;
		
	} else {
		if (self->rootContainerType != JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY) {
			failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return)
		}
		self->currentNode = NULL;
	}
}

void JSONSerializationContext_endArray(compat_type(JSONSerializationContext *) selfPtr) {
	JSONSerializationContext * self = selfPtr;
	
	if (self->currentNode == NULL) {
		failWithStatus(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, return)
	}
	
	if (self->nodeStackCurrentDepth > 0) {
		if (self->nodeStack[self->nodeStackCurrentDepth - 1].containerType != JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY) {
			failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return)
		}
		self->currentNode = self->nodeStack[--self->nodeStackCurrentDepth].node;
		self->currentContainerType = self->nodeStack[self->nodeStackCurrentDepth].containerType;
		
	} else {
		if (self->rootContainerType != JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY) {
			failWithStatus(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH, return)
		}
		self->currentNode = NULL;
	}
}

void JSONSerializationContext_writeInt8(compat_type(JSONSerializationContext *) selfPtr, const char * key, int8_t value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeUInt8(compat_type(JSONSerializationContext *) selfPtr, const char * key, uint8_t value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeInt16(compat_type(JSONSerializationContext *) selfPtr, const char * key, int16_t value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeUInt16(compat_type(JSONSerializationContext *) selfPtr, const char * key, uint16_t value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeInt32(compat_type(JSONSerializationContext *) selfPtr, const char * key, int32_t value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeUInt32(compat_type(JSONSerializationContext *) selfPtr, const char * key, uint32_t value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	self->currentNode->value.count++;
}

#ifndef S_SPLINT_S
#if ULLONG_MAX < 0xFFFFFFFFFFFFFFFFull
#error long long is less than 64 bits wide; cannot print as %llu in JSONSerializationContext_writeInt64 and JSONSerializationContext_writeUInt64
#endif
#endif

void JSONSerializationContext_writeInt64(compat_type(JSONSerializationContext *) selfPtr, const char * key, int64_t value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	if ((int64_t) self->currentNode->subitems[self->currentNode->value.count].value.number != value) {
		// Not representable as double; use string instead
		self->currentNode->subitems[self->currentNode->value.count].type = JSON_TYPE_STRING;
		self->currentNode->subitems[self->currentNode->value.count].value.string = malloc(24); // Generous; INT64_MIN should only be 20 chars long
		self->currentNode->subitems[self->currentNode->value.count].stringLength = snprintf(self->currentNode->subitems[self->currentNode->value.count].value.string, 24, "%lld", value);
	}
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeUInt64(compat_type(JSONSerializationContext *) selfPtr, const char * key, uint64_t value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	if ((uint64_t) self->currentNode->subitems[self->currentNode->value.count].value.number != value) {
		// Not representable as double; use string instead
		self->currentNode->subitems[self->currentNode->value.count].type = JSON_TYPE_STRING;
		self->currentNode->subitems[self->currentNode->value.count].value.string = malloc(24); // Generous; UINT64_MAX should only be 20 chars long
		self->currentNode->subitems[self->currentNode->value.count].stringLength = snprintf(self->currentNode->subitems[self->currentNode->value.count].value.string, 24, "%llu", value);
	}
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeFloat(compat_type(JSONSerializationContext *) selfPtr, const char * key, float value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeDouble(compat_type(JSONSerializationContext *) selfPtr, const char * key, double value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_NUMBER, number);
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeString(compat_type(JSONSerializationContext *) selfPtr, const char * key, const char * value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	self->currentNode->subitems[self->currentNode->value.count].stringLength = strlen(value);
	writeNode(self->currentNode->subitems[self->currentNode->value.count],
	          self->currentNode,
	          key,
	          JSON_TYPE_STRING,
	          string,
	          malloc(self->currentNode->subitems[self->currentNode->value.count].stringLength + 1));
	strcpy(self->currentNode->subitems[self->currentNode->value.count].value.string, value);
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeBoolean(compat_type(JSONSerializationContext *) selfPtr, const char * key, bool value) {
	JSONSerializationContext * self = selfPtr;
	
	failIfContainerNotStarted()
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	writeNodeCommonArgs(JSON_TYPE_BOOLEAN, boolean);
	self->currentNode->value.count++;
}

void JSONSerializationContext_writeEnumeration(compat_type(JSONSerializationContext *) selfPtr, const char * key, int value, ...) {
	JSONSerializationContext * self = selfPtr;
	va_list args;
	const char * enumName, * valueEnumName = NULL;
	int enumValue;
	struct enumListItem {const char * name; int value;} * enumList;
	size_t enumListAllocatedSize, enumCount, enumIndex;
	
	failIfContainerNotStarted()
	
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1));
	
	va_start(args, value);
	enumCount = 0;
	enumListAllocatedSize = 1;
	enumList = malloc(sizeof(struct enumListItem) * enumListAllocatedSize);
	
	for (;;) {
		enumName = va_arg(args, const char *);
		if (enumName == NULL) {
			break;
		}
		enumValue = va_arg(args, int);
		if (enumValue == value) {
			valueEnumName = enumName;
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
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE, return)
				
			} else if (!strcmp(enumList[enumIndex].name, enumName)) {
				free(enumList);
				va_end(args);
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME, return)
			}
		}
		enumCount++;
	}
	
	va_end(args);
	free(enumList);
	
	if (valueEnumName == NULL) {
		failWithStatus(SERIALIZATION_ERROR_ENUM_NOT_NAMED, return)
	}
	
	self->currentNode->subitems[self->currentNode->value.count].stringLength = strlen(valueEnumName);
	writeNode(self->currentNode->subitems[self->currentNode->value.count],
	          self->currentNode,
	          key,
	          JSON_TYPE_STRING,
	          string,
	          malloc(self->currentNode->subitems[self->currentNode->value.count].stringLength + 1));
	strcpy(self->currentNode->subitems[self->currentNode->value.count].value.string, valueEnumName);
	self->currentNode->value.count++;
}

#define writeBitfieldImplementation(NBITS) \
	JSONSerializationContext * self = selfPtr; \
	unsigned int bitCount; \
	uint##NBITS##_t uncountedBits; \
	va_list args; \
	unsigned int bitIndex, bitIndex2; \
	const char * bitName; \
	struct JSONNode * array; \
	const char * bitNames[NBITS]; \
	\
	failIfContainerNotStarted() \
	/* http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan */ \
	uncountedBits = value; \
	for (bitCount = 0; uncountedBits > 0; bitCount++) { \
		uncountedBits &= uncountedBits - 1; \
	} \
	\
	self->currentNode->subitems = realloc(self->currentNode->subitems, sizeof(struct JSONNode) * (self->currentNode->value.count + 1)); \
	array = &self->currentNode->subitems[self->currentNode->value.count]; \
	writeNode(*array, self->currentNode, key, JSON_TYPE_ARRAY, count, 0); \
	self->currentNode->value.count++; \
	if (bitCount == 0) { \
		array->subitems = NULL; \
	} else { \
		array->subitems = malloc(sizeof(struct JSONNode) * bitCount); \
	} \
	va_start(args, value); \
	for (bitIndex = 0; bitIndex < NBITS; bitIndex++) { \
		bitName = va_arg(args, const char *); \
		if (bitName == NULL) { \
			break; \
		} \
		bitNames[bitIndex] = bitName; \
		for (bitIndex2 = 0; bitIndex2 < bitIndex; bitIndex2++) { \
			if (!strcmp(bitNames[bitIndex2], bitName)) { \
				va_end(args); \
				failWithStatus(SERIALIZATION_ERROR_DUPLICATE_BIT, return) \
			} \
		} \
		if (value & ((uint##NBITS##_t) 1 << bitIndex)) { \
			array->subitems[array->value.count].type = JSON_TYPE_STRING; \
			array->subitems[array->value.count].key = NULL; \
			array->subitems[array->value.count].stringLength = strlen(bitName); \
			array->subitems[array->value.count].value.string = malloc(array->subitems[array->value.count].stringLength + 1); \
			strcpy(array->subitems[array->value.count].value.string, bitName); \
			array->value.count++; \
		} \
	} \
	va_end(args); \
	if (array->value.count < bitCount) { \
		failWithStatus(SERIALIZATION_ERROR_UNNAMED_BIT,) \
	}

void JSONSerializationContext_writeBitfield8(compat_type(JSONSerializationContext *) selfPtr, const char * key, uint8_t value, ...) {
	writeBitfieldImplementation(8)
}

void JSONSerializationContext_writeBitfield16(compat_type(JSONSerializationContext *) selfPtr, const char * key, uint16_t value, ...) {
	writeBitfieldImplementation(16)
}

void JSONSerializationContext_writeBitfield32(compat_type(JSONSerializationContext *) selfPtr, const char * key, uint32_t value, ...) {
	writeBitfieldImplementation(32)
}

void JSONSerializationContext_writeBitfield64(compat_type(JSONSerializationContext *) selfPtr, const char * key, uint64_t value, ...) {
	writeBitfieldImplementation(64)
}
