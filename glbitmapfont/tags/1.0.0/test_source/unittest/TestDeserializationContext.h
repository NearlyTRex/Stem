#ifndef __TEST_DESERIALIZATION_CONTEXT_H__
#define __TEST_DESERIALIZATION_CONTEXT_H__

#include "serialization/DeserializationContext.h"
#include <setjmp.h>
#include <stdlib.h>

typedef struct TestDeserializationContext TestDeserializationContext;

struct TestDeserializationContext_expectedCall {
	void * functionPtr;
	char * key;
	union {
		size_t sizeValue;
		int8_t int8Value;
		uint8_t uint8Value;
		int16_t int16Value;
		uint16_t uint16Value;
		int32_t int32Value;
		uint32_t uint32Value;
		int64_t int64Value;
		uint64_t uint64Value;
		float floatValue;
		double doubleValue;
		char * stringValue;
		bool boolValue;
		int enumValue;
	} returnValue;
	union TestDeserializationContext_additionalArg {
		char * stringValue;
		int enumValue;
	} * additionalArgs;
};

#define DESERIALIZATION_ERROR_MAX 512

#define TestDeserializationContext_structContents \
	DeserializationContext_structContents \
	\
	jmp_buf * sequenceBreakJmpEnv; \
	char error[DESERIALIZATION_ERROR_MAX]; \
	unsigned int callIndexToFail; \
	int failStatus; \
	struct TestDeserializationContext_expectedCall * expectedCalls; \
	unsigned int numExpectedCalls; \
	unsigned int nextExpectedCallIndex; \
	\
	void (* expectCall)(void * self, void * functionPtr, ...); \
	void (* failNthCall)(void * self, unsigned int callIndex, int status); \
	void (* finish)(void * self);

struct TestDeserializationContext {
	TestDeserializationContext_structContents
};

TestDeserializationContext * TestDeserializationContext_create(jmp_buf * sequenceBreakJmpEnv);
void TestDeserializationContext_init(TestDeserializationContext * self, jmp_buf * sequenceBreakJmpEnv);
void TestDeserializationContext_dispose(void * selfPtr);

size_t TestDeserializationContext_beginStructure(void * selfPtr, const char * key);
size_t TestDeserializationContext_beginDictionary(void * selfPtr, const char * key);
size_t TestDeserializationContext_beginArray(void * selfPtr, const char * key);
void TestDeserializationContext_endStructure(void * selfPtr);
void TestDeserializationContext_endDictionary(void * selfPtr);
void TestDeserializationContext_endArray(void * selfPtr);
int8_t       TestDeserializationContext_readInt8(void * selfPtr, const char * key);
uint8_t      TestDeserializationContext_readUInt8(void * selfPtr, const char * key);
int16_t      TestDeserializationContext_readInt16(void * selfPtr, const char * key);
uint16_t     TestDeserializationContext_readUInt16(void * selfPtr, const char * key);
int32_t      TestDeserializationContext_readInt32(void * selfPtr, const char * key);
uint32_t     TestDeserializationContext_readUInt32(void * selfPtr, const char * key);
int64_t      TestDeserializationContext_readInt64(void * selfPtr, const char * key);
uint64_t     TestDeserializationContext_readUInt64(void * selfPtr, const char * key);
float        TestDeserializationContext_readFloat(void * selfPtr, const char * key);
double       TestDeserializationContext_readDouble(void * selfPtr, const char * key);
const char * TestDeserializationContext_readString(void * selfPtr, const char * key);
bool         TestDeserializationContext_readBoolean(void * selfPtr, const char * key);
int          TestDeserializationContext_readEnumeration(void * selfPtr, const char * key, ...);
uint8_t      TestDeserializationContext_readBitfield8(void * selfPtr, const char * key, ...);
uint16_t     TestDeserializationContext_readBitfield16(void * selfPtr, const char * key, ...);
uint32_t     TestDeserializationContext_readBitfield32(void * selfPtr, const char * key, ...);
uint64_t     TestDeserializationContext_readBitfield64(void * selfPtr, const char * key, ...);
const char * TestDeserializationContext_readNextDictionaryKey(void * selfPtr);
bool         TestDeserializationContext_hasDictionaryKey(void * selfPtr, const char * key);

// selfPtr, functionPtr, key (if applicable), returnValue (if applicable), additional args (if applicable)
void TestDeserializationContext_expectCall(void * selfPtr, void * functionPtr, ...);
void TestDeserializationContext_failNthCall(void * selfPtr, unsigned int callIndex, int status);
void TestDeserializationContext_finish(void * selfPtr);

#endif
