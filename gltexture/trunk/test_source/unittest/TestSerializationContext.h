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
	void (* finish)(void * self);

struct TestSerializationContext {
	TestSerializationContext_structContents
};

TestSerializationContext * TestSerializationContext_create(jmp_buf * sequenceBreakJmpEnv);
void TestSerializationContext_init(TestSerializationContext * self, jmp_buf * sequenceBreakJmpEnv);
void TestSerializationContext_dispose(void * selfPtr);

void TestSerializationContext_beginStructure(void * self, char * key);
void TestSerializationContext_beginDictionary(void * self, char * key);
void TestSerializationContext_beginArray(void * self, char * key);
void TestSerializationContext_endStructure(void * self);
void TestSerializationContext_endDictionary(void * self);
void TestSerializationContext_endArray(void * self);
void TestSerializationContext_writeInt8(void * self, char * key, int8_t value);
void TestSerializationContext_writeUInt8(void * self, char * key, uint8_t value);
void TestSerializationContext_writeInt16(void * self, char * key, int16_t value);
void TestSerializationContext_writeUInt16(void * self, char * key, uint16_t value);
void TestSerializationContext_writeInt32(void * self, char * key, int32_t value);
void TestSerializationContext_writeUInt32(void * self, char * key, uint32_t value);
void TestSerializationContext_writeInt64(void * self, char * key, int64_t value);
void TestSerializationContext_writeUInt64(void * self, char * key, uint64_t value);
void TestSerializationContext_writeFloat(void * self, char * key, float value);
void TestSerializationContext_writeDouble(void * self, char * key, double value);
void TestSerializationContext_writeString(void * self, char * key, char * value);
void TestSerializationContext_writeBoolean(void * self, char * key, bool value);
void TestSerializationContext_writeEnumeration(void * self, char * key, int value, ...);
void TestSerializationContext_writeBitfield8(void * self, char * key, uint8_t value, ...);
void TestSerializationContext_writeBitfield16(void * self, char * key, uint16_t value, ...);
void TestSerializationContext_writeBitfield32(void * self, char * key, uint32_t value, ...);
void TestSerializationContext_writeBitfield64(void * self, char * key, uint64_t value, ...);

void TestSerializationContext_expectCall(void * selfPtr, void * functionPtr, ...);
void TestSerializationContext_failNthCall(void * selfPtr, unsigned int callIndex, int status);
void TestSerializationContext_finish(void * selfPtr);

#endif
