#include "unittest/TestSuite.h"
#include "serialization/DeserializationContext.h"
#include <stdlib.h>

static void testInit() {
	DeserializationContext context;
	
	memset(&context, 0xFF, sizeof(context));
	DeserializationContext_init(&context);
	TestCase_assert(context.jmpBuf == NULL, "Expected NULL but got %p", context.jmpBuf);
	TestCase_assert(context.status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context.status);
	TestCase_assert(context.dispose == DeserializationContext_dispose, "Expected %p but got %p", DeserializationContext_dispose, context.dispose);
	TestCase_assert(context.errorString == Serialization_errorString, "Expected %p but got %p", Serialization_errorString, context.errorString);
	TestCase_assert(context.beginStructure == NULL, "Expected NULL but got %p", context.beginStructure);
	TestCase_assert(context.beginDictionary == NULL, "Expected NULL but got %p", context.beginDictionary);
	TestCase_assert(context.beginArray == NULL, "Expected NULL but got %p", context.beginArray);
	TestCase_assert(context.endStructure == NULL, "Expected NULL but got %p", context.endStructure);
	TestCase_assert(context.endDictionary == NULL, "Expected NULL but got %p", context.endDictionary);
	TestCase_assert(context.endArray == NULL, "Expected NULL but got %p", context.endArray);
	TestCase_assert(context.readBoolean == NULL, "Expected NULL but got %p", context.readBoolean);
	TestCase_assert(context.readInt8 == NULL, "Expected NULL but got %p", context.readInt8);
	TestCase_assert(context.readUInt8 == NULL, "Expected NULL but got %p", context.readUInt8);
	TestCase_assert(context.readInt16 == NULL, "Expected NULL but got %p", context.readInt16);
	TestCase_assert(context.readUInt16 == NULL, "Expected NULL but got %p", context.readUInt16);
	TestCase_assert(context.readInt32 == NULL, "Expected NULL but got %p", context.readInt32);
	TestCase_assert(context.readUInt32 == NULL, "Expected NULL but got %p", context.readUInt32);
	TestCase_assert(context.readInt64 == NULL, "Expected NULL but got %p", context.readInt64);
	TestCase_assert(context.readUInt64 == NULL, "Expected NULL but got %p", context.readUInt64);
	TestCase_assert(context.readFloat == NULL, "Expected NULL but got %p", context.readFloat);
	TestCase_assert(context.readDouble == NULL, "Expected NULL but got %p", context.readDouble);
	TestCase_assert(context.readEnumeration == NULL, "Expected NULL but got %p", context.readEnumeration);
	TestCase_assert(context.readBitfield8 == NULL, "Expected NULL but got %p", context.readBitfield8);
	TestCase_assert(context.readBitfield16 == NULL, "Expected NULL but got %p", context.readBitfield16);
	TestCase_assert(context.readBitfield32 == NULL, "Expected NULL but got %p", context.readBitfield32);
	TestCase_assert(context.readBitfield64 == NULL, "Expected NULL but got %p", context.readBitfield64);
	TestCase_assert(context.readString == NULL, "Expected NULL but got %p", context.readString);
	TestCase_assert(context.readBlob == NULL, "Expected NULL but got %p", context.readBlob);
	TestCase_assert(context.readNextDictionaryKey == NULL, "Expected NULL but got %p", context.readNextDictionaryKey);
	TestCase_assert(context.hasDictionaryKey == NULL, "Expected NULL but got %p", context.hasDictionaryKey);
}

TEST_SUITE(DeserializationContextTest, testInit)
