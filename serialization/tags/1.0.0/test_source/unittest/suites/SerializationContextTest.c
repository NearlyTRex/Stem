#include "unittest/framework/TestSuite.h"
#include "serialization/SerializationContext.h"
#include <stdlib.h>

static void testInit() {
	SerializationContext context;
	
	memset(&context, 0, sizeof(context));
	SerializationContext_init(&context);
	TestCase_assert(context.jmpBuf == NULL, "Expected NULL but got %p", context.jmpBuf);
	TestCase_assert(context.status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context.status);
	TestCase_assert(context.dispose != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(context.beginStructure == NULL, "Expected NULL but got %p", context.beginStructure);
	TestCase_assert(context.beginDictionary == NULL, "Expected NULL but got %p", context.beginDictionary);
	TestCase_assert(context.beginArray == NULL, "Expected NULL but got %p", context.beginArray);
	TestCase_assert(context.endStructure == NULL, "Expected NULL but got %p", context.endStructure);
	TestCase_assert(context.endDictionary == NULL, "Expected NULL but got %p", context.endDictionary);
	TestCase_assert(context.endArray == NULL, "Expected NULL but got %p", context.endArray);
	TestCase_assert(context.writeInt8 == NULL, "Expected NULL but got %p", context.writeInt8);
	TestCase_assert(context.writeUInt8 == NULL, "Expected NULL but got %p", context.writeUInt8);
	TestCase_assert(context.writeInt16 == NULL, "Expected NULL but got %p", context.writeInt16);
	TestCase_assert(context.writeUInt16 == NULL, "Expected NULL but got %p", context.writeUInt16);
	TestCase_assert(context.writeInt32 == NULL, "Expected NULL but got %p", context.writeInt32);
	TestCase_assert(context.writeUInt32 == NULL, "Expected NULL but got %p", context.writeUInt32);
	TestCase_assert(context.writeInt64 == NULL, "Expected NULL but got %p", context.writeInt64);
	TestCase_assert(context.writeUInt64 == NULL, "Expected NULL but got %p", context.writeUInt64);
	TestCase_assert(context.writeFloat == NULL, "Expected NULL but got %p", context.writeFloat);
	TestCase_assert(context.writeDouble == NULL, "Expected NULL but got %p", context.writeDouble);
	TestCase_assert(context.writeString == NULL, "Expected NULL but got %p", context.writeString);
	TestCase_assert(context.writeBoolean == NULL, "Expected NULL but got %p", context.writeBoolean);
	TestCase_assert(context.writeEnumeration == NULL, "Expected NULL but got %p", context.writeEnumeration);
	TestCase_assert(context.writeBitfield8 == NULL, "Expected NULL but got %p", context.writeBitfield8);
	TestCase_assert(context.writeBitfield16 == NULL, "Expected NULL but got %p", context.writeBitfield16);
	TestCase_assert(context.writeBitfield32 == NULL, "Expected NULL but got %p", context.writeBitfield32);
	TestCase_assert(context.writeBitfield64 == NULL, "Expected NULL but got %p", context.writeBitfield64);
}

TEST_SUITE(SerializationContextTest, testInit)
