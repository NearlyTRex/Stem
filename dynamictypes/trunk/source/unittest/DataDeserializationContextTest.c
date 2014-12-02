#include "dynamictypes/DataDeserializationContext.h"
#include "unittest/TestSuite.h"
#include <string.h>

#define verifyFunctionPointer(context, name) \
	TestCase_assert((context)->name == DataDeserializationContext_##name, "Expected " #name " to be %p, but got %p", DataDeserializationContext_##name, (context)->name)

#define verifyFunctionPointers(context) \
	verifyFunctionPointer(context, dispose); \
	verifyFunctionPointer(context, beginStructure); \
	verifyFunctionPointer(context, beginDictionary); \
	verifyFunctionPointer(context, beginArray); \
	verifyFunctionPointer(context, endStructure); \
	verifyFunctionPointer(context, endDictionary); \
	verifyFunctionPointer(context, endArray); \
	verifyFunctionPointer(context, readInt8); \
	verifyFunctionPointer(context, readUInt8); \
	verifyFunctionPointer(context, readInt16); \
	verifyFunctionPointer(context, readUInt16); \
	verifyFunctionPointer(context, readInt32); \
	verifyFunctionPointer(context, readUInt32); \
	verifyFunctionPointer(context, readInt64); \
	verifyFunctionPointer(context, readUInt64); \
	verifyFunctionPointer(context, readFloat); \
	verifyFunctionPointer(context, readDouble); \
	verifyFunctionPointer(context, readString); \
	verifyFunctionPointer(context, readBoolean); \
	verifyFunctionPointer(context, readEnumeration); \
	verifyFunctionPointer(context, readBitfield8); \
	verifyFunctionPointer(context, readBitfield16); \
	verifyFunctionPointer(context, readBitfield32); \
	verifyFunctionPointer(context, readBitfield64); \
	verifyFunctionPointer(context, readNextDictionaryKey); \
	verifyFunctionPointer(context, hasDictionaryKey)

static void testInit() {
	DataDeserializationContext context, * contextPtr;
	
	memset(&context, 0x00, sizeof(context));
	DataDeserializationContext_init(&context, valueCreateBoolean(false));
	verifyFunctionPointers(&context);
	DataDeserializationContext_dispose(&context);
	
	memset(&context, 0xFF, sizeof(context));
	DataDeserializationContext_init(&context, valueCreateBoolean(false));
	verifyFunctionPointers(&context);
	DataDeserializationContext_dispose(&context);
	
	contextPtr = DataDeserializationContext_create(valueCreateBoolean(false));
	verifyFunctionPointers(contextPtr);
	DataDeserializationContext_dispose(contextPtr);
}

TEST_SUITE(DataDeserializationContextTest,
           testInit)
