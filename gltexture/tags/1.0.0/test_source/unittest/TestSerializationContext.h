#ifndef __TEST_SERIALIZATION_CONTEXT_H__
#define __TEST_SERIALIZATION_CONTEXT_H__

#include "serialization/SerializationContext.h"
#include <setjmp.h>
#include <stdlib.h>

typedef struct TestSerializationContext TestSerializationContext;

struct TestSerializationContext_expectedCall {
	void * functionPtr;
	char * key;
	union {
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
	} value;
	union TestSerializationContext_additionalArg {
		char * stringValue;
		int enumValue;
	} * additionalArgs;
};

#define SERIALIZATION_ERROR_MAX 512

#define TestSerializationContext_structContents \
	SerializationContext_structContents \
	\
	jmp_buf * sequenceBreakJmpEnv; \
	char error[SERIALIZATION_ERROR_MAX]; \
	unsigned int callIndexToFail; \
	int failStatus; \
	struct TestSerializationContext_expectedCall * expectedCalls; \
	unsigned int numExpectedCalls; \
	unsigned int nextExpectedCallIndex; \
	\
	void (* expectCall)(void * self, void * functionPtr, ...); \
	void (* failNthCall)(void * self, unsigned int callIndex, int status); \
	void (* finish)(void * self);

struct TestSerializationContext {
	TestSerializationContext_structContents
};

TestSerializationContext * TestSerializationContext_create(jmp_buf * sequenceBreakJmpEnv);
void TestSerializationContext_init(TestSerializationContext * self, jmp_buf * sequenceBreakJmpEnv);
void TestSerializationContext_dispose(void * selfPtr);

void TestSerializationContext_beginStructure(void * selfPtr, const char * key);
void TestSerializationContext_beginDictionary(void * selfPtr, const char * key);
void TestSerializationContext_beginArray(void * selfPtr, const char * key);
void TestSerializationContext_endStructure(void * selfPtr);
void TestSerializationContext_endDictionary(void * selfPtr);
void TestSerializationContext_endArray(void * selfPtr);
void TestSerializationContext_writeInt8(void * selfPtr, const char * key, int8_t value);
void TestSerializationContext_writeUInt8(void * selfPtr, const char * key, uint8_t value);
void TestSerializationContext_writeInt16(void * selfPtr, const char * key, int16_t value);
void TestSerializationContext_writeUInt16(void * selfPtr, const char * key, uint16_t value);
void TestSerializationContext_writeInt32(void * selfPtr, const char * key, int32_t value);
void TestSerializationContext_writeUInt32(void * selfPtr, const char * key, uint32_t value);
void TestSerializationContext_writeInt64(void * selfPtr, const char * key, int64_t value);
void TestSerializationContext_writeUInt64(void * selfPtr, const char * key, uint64_t value);
void TestSerializationContext_writeFloat(void * selfPtr, const char * key, float value);
void TestSerializationContext_writeDouble(void * selfPtr, const char * key, double value);
void TestSerializationContext_writeString(void * selfPtr, const char * key, const char * value);
void TestSerializationContext_writeBoolean(void * selfPtr, const char * key, bool value);
void TestSerializationContext_writeEnumeration(void * selfPtr, const char * key, int value, ...);
void TestSerializationContext_writeBitfield8(void * selfPtr, const char * key, uint8_t value, ...);
void TestSerializationContext_writeBitfield16(void * selfPtr, const char * key, uint16_t value, ...);
void TestSerializationContext_writeBitfield32(void * selfPtr, const char * key, uint32_t value, ...);
void TestSerializationContext_writeBitfield64(void * selfPtr, const char * key, uint64_t value, ...);

// selfPtr, functionPtr, key (if applicable), value (if applicable), additional args (if applicable)
void TestSerializationContext_expectCall(void * selfPtr, void * functionPtr, ...);
void TestSerializationContext_failNthCall(void * selfPtr, unsigned int callIndex, int status);
void TestSerializationContext_finish(void * selfPtr);

#endif
