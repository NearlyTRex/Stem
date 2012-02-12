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

#define TestDeserializationContext_structContents(self_type) \
	DeserializationContext_structContents(self_type) \
	\
	jmp_buf * sequenceBreakJmpEnv; \
	char error[DESERIALIZATION_ERROR_MAX]; \
	unsigned int callIndexToFail; \
	int failStatus; \
	struct TestDeserializationContext_expectedCall * expectedCalls; \
	unsigned int numExpectedCalls; \
	unsigned int nextExpectedCallIndex; \
	\
	void (* expectCall)(self_type * self, void * functionPtr, ...); \
	void (* failNthCall)(self_type * self, unsigned int callIndex, int status); \
	void (* finish)(self_type * self);

stemobject_struct_definition(TestDeserializationContext)

TestDeserializationContext * TestDeserializationContext_create(jmp_buf * sequenceBreakJmpEnv);
void TestDeserializationContext_init(TestDeserializationContext * self, jmp_buf * sequenceBreakJmpEnv);
void TestDeserializationContext_dispose(TestDeserializationContext * self);

void TestDeserializationContext_beginStructure(TestDeserializationContext * self, const char * key);
size_t TestDeserializationContext_beginDictionary(TestDeserializationContext * self, const char * key);
size_t TestDeserializationContext_beginArray(TestDeserializationContext * self, const char * key);
void TestDeserializationContext_endStructure(TestDeserializationContext * self);
void TestDeserializationContext_endDictionary(TestDeserializationContext * self);
void TestDeserializationContext_endArray(TestDeserializationContext * self);
int8_t       TestDeserializationContext_readInt8(TestDeserializationContext * self, const char * key);
uint8_t      TestDeserializationContext_readUInt8(TestDeserializationContext * self, const char * key);
int16_t      TestDeserializationContext_readInt16(TestDeserializationContext * self, const char * key);
uint16_t     TestDeserializationContext_readUInt16(TestDeserializationContext * self, const char * key);
int32_t      TestDeserializationContext_readInt32(TestDeserializationContext * self, const char * key);
uint32_t     TestDeserializationContext_readUInt32(TestDeserializationContext * self, const char * key);
int64_t      TestDeserializationContext_readInt64(TestDeserializationContext * self, const char * key);
uint64_t     TestDeserializationContext_readUInt64(TestDeserializationContext * self, const char * key);
float        TestDeserializationContext_readFloat(TestDeserializationContext * self, const char * key);
double       TestDeserializationContext_readDouble(TestDeserializationContext * self, const char * key);
const char * TestDeserializationContext_readString(TestDeserializationContext * self, const char * key);
bool         TestDeserializationContext_readBoolean(TestDeserializationContext * self, const char * key);
int          TestDeserializationContext_readEnumeration(TestDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint8_t      TestDeserializationContext_readBitfield8(TestDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint16_t     TestDeserializationContext_readBitfield16(TestDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint32_t     TestDeserializationContext_readBitfield32(TestDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
uint64_t     TestDeserializationContext_readBitfield64(TestDeserializationContext * self, const char * key, ...) __attribute__((sentinel));
const char * TestDeserializationContext_readNextDictionaryKey(TestDeserializationContext * self);
bool         TestDeserializationContext_hasDictionaryKey(TestDeserializationContext * self, const char * key);

// selfPtr, functionPtr, key (if applicable), returnValue (if applicable), additional args (if applicable)
void TestDeserializationContext_expectCall(TestDeserializationContext * self, void * functionPtr, ...);
void TestDeserializationContext_failNthCall(TestDeserializationContext * self, unsigned int callIndex, int status);
void TestDeserializationContext_finish(TestDeserializationContext * self);

#endif
