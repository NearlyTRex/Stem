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
	void (* expectCall)(compat_type(TestDeserializationContext *) self, void * functionPtr, ...); \
	void (* failNthCall)(compat_type(TestDeserializationContext *) self, unsigned int callIndex, int status); \
	void (* finish)(compat_type(TestDeserializationContext *) self);

struct TestDeserializationContext {
	TestDeserializationContext_structContents
};

TestDeserializationContext * TestDeserializationContext_create(jmp_buf * sequenceBreakJmpEnv);
void TestDeserializationContext_init(compat_type(TestDeserializationContext *) selfPtr, jmp_buf * sequenceBreakJmpEnv);
void TestDeserializationContext_dispose(compat_type(TestDeserializationContext *) selfPtr);

size_t TestDeserializationContext_beginStructure(compat_type(TestDeserializationContext *) selfPtr, const char * key);
size_t TestDeserializationContext_beginDictionary(compat_type(TestDeserializationContext *) selfPtr, const char * key);
size_t TestDeserializationContext_beginArray(compat_type(TestDeserializationContext *) selfPtr, const char * key);
void TestDeserializationContext_endStructure(compat_type(TestDeserializationContext *) selfPtr);
void TestDeserializationContext_endDictionary(compat_type(TestDeserializationContext *) selfPtr);
void TestDeserializationContext_endArray(compat_type(TestDeserializationContext *) selfPtr);
int8_t       TestDeserializationContext_readInt8(compat_type(TestDeserializationContext *) selfPtr, const char * key);
uint8_t      TestDeserializationContext_readUInt8(compat_type(TestDeserializationContext *) selfPtr, const char * key);
int16_t      TestDeserializationContext_readInt16(compat_type(TestDeserializationContext *) selfPtr, const char * key);
uint16_t     TestDeserializationContext_readUInt16(compat_type(TestDeserializationContext *) selfPtr, const char * key);
int32_t      TestDeserializationContext_readInt32(compat_type(TestDeserializationContext *) selfPtr, const char * key);
uint32_t     TestDeserializationContext_readUInt32(compat_type(TestDeserializationContext *) selfPtr, const char * key);
int64_t      TestDeserializationContext_readInt64(compat_type(TestDeserializationContext *) selfPtr, const char * key);
uint64_t     TestDeserializationContext_readUInt64(compat_type(TestDeserializationContext *) selfPtr, const char * key);
float        TestDeserializationContext_readFloat(compat_type(TestDeserializationContext *) selfPtr, const char * key);
double       TestDeserializationContext_readDouble(compat_type(TestDeserializationContext *) selfPtr, const char * key);
const char * TestDeserializationContext_readString(compat_type(TestDeserializationContext *) selfPtr, const char * key);
bool         TestDeserializationContext_readBoolean(compat_type(TestDeserializationContext *) selfPtr, const char * key);
int          TestDeserializationContext_readEnumeration(compat_type(TestDeserializationContext *) selfPtr, const char * key, ...);
uint8_t      TestDeserializationContext_readBitfield8(compat_type(TestDeserializationContext *) selfPtr, const char * key, ...);
uint16_t     TestDeserializationContext_readBitfield16(compat_type(TestDeserializationContext *) selfPtr, const char * key, ...);
uint32_t     TestDeserializationContext_readBitfield32(compat_type(TestDeserializationContext *) selfPtr, const char * key, ...);
uint64_t     TestDeserializationContext_readBitfield64(compat_type(TestDeserializationContext *) selfPtr, const char * key, ...);
const char * TestDeserializationContext_readNextDictionaryKey(compat_type(TestDeserializationContext *) selfPtr);
bool         TestDeserializationContext_hasDictionaryKey(compat_type(TestDeserializationContext *) selfPtr, const char * key);

// selfPtr, functionPtr, key (if applicable), returnValue (if applicable), additional args (if applicable)
void TestDeserializationContext_expectCall(compat_type(TestDeserializationContext *) selfPtr, void * functionPtr, ...);
void TestDeserializationContext_failNthCall(compat_type(TestDeserializationContext *) selfPtr, unsigned int callIndex, int status);
void TestDeserializationContext_finish(compat_type(TestDeserializationContext *) selfPtr);

#endif
