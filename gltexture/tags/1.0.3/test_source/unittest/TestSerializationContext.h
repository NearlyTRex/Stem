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

#define TestSerializationContext_structContents(self_type) \
	SerializationContext_structContents(self_type) \
	\
	jmp_buf * sequenceBreakJmpEnv; \
	char error[SERIALIZATION_ERROR_MAX]; \
	unsigned int callIndexToFail; \
	int failStatus; \
	struct TestSerializationContext_expectedCall * expectedCalls; \
	unsigned int numExpectedCalls; \
	unsigned int nextExpectedCallIndex; \
	\
	void (* expectCall)(self_type * self, void * functionPtr, ...); \
	void (* failNthCall)(self_type * self, unsigned int callIndex, int status); \
	void (* finish)(self_type * self);

stemobject_struct_definition(TestSerializationContext)

TestSerializationContext * TestSerializationContext_create(jmp_buf * sequenceBreakJmpEnv);
void TestSerializationContext_init(TestSerializationContext * self, jmp_buf * sequenceBreakJmpEnv);
void TestSerializationContext_dispose(TestSerializationContext * self);

void TestSerializationContext_beginStructure(TestSerializationContext * self, const char * key);
void TestSerializationContext_beginDictionary(TestSerializationContext * self, const char * key);
void TestSerializationContext_beginArray(TestSerializationContext * self, const char * key);
void TestSerializationContext_endStructure(TestSerializationContext * self);
void TestSerializationContext_endDictionary(TestSerializationContext * self);
void TestSerializationContext_endArray(TestSerializationContext * self);
void TestSerializationContext_writeInt8(TestSerializationContext * self, const char * key, int8_t value);
void TestSerializationContext_writeUInt8(TestSerializationContext * self, const char * key, uint8_t value);
void TestSerializationContext_writeInt16(TestSerializationContext * self, const char * key, int16_t value);
void TestSerializationContext_writeUInt16(TestSerializationContext * self, const char * key, uint16_t value);
void TestSerializationContext_writeInt32(TestSerializationContext * self, const char * key, int32_t value);
void TestSerializationContext_writeUInt32(TestSerializationContext * self, const char * key, uint32_t value);
void TestSerializationContext_writeInt64(TestSerializationContext * self, const char * key, int64_t value);
void TestSerializationContext_writeUInt64(TestSerializationContext * self, const char * key, uint64_t value);
void TestSerializationContext_writeFloat(TestSerializationContext * self, const char * key, float value);
void TestSerializationContext_writeDouble(TestSerializationContext * self, const char * key, double value);
void TestSerializationContext_writeString(TestSerializationContext * self, const char * key, const char * value);
void TestSerializationContext_writeBoolean(TestSerializationContext * self, const char * key, bool value);
void TestSerializationContext_writeEnumeration(TestSerializationContext * self, const char * key, int value, ...) __attribute__((sentinel));
void TestSerializationContext_writeBitfield8(TestSerializationContext * self, const char * key, uint8_t value, ...) __attribute__((sentinel));
void TestSerializationContext_writeBitfield16(TestSerializationContext * self, const char * key, uint16_t value, ...) __attribute__((sentinel));
void TestSerializationContext_writeBitfield32(TestSerializationContext * self, const char * key, uint32_t value, ...) __attribute__((sentinel));
void TestSerializationContext_writeBitfield64(TestSerializationContext * self, const char * key, uint64_t value, ...) __attribute__((sentinel));

// selfPtr, functionPtr, key (if applicable), value (if applicable), additional args (if applicable)
void TestSerializationContext_expectCall(TestSerializationContext * self, void * functionPtr, ...);
void TestSerializationContext_failNthCall(TestSerializationContext * self, unsigned int callIndex, int status);
void TestSerializationContext_finish(TestSerializationContext * self);

#endif
