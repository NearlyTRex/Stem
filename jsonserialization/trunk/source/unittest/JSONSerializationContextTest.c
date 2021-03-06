#include "jsonio/JSONEmitter.h"
#include "jsonserialization/JSONSerializationContext.h"
#include "unittest/TestSuite.h"
#include "utilities/IOUtilities.h"
#include "utilities/printfFormats.h"
#include <float.h>
#include <setjmp.h>
#include <stdint.h>
#include <unistd.h>

static void verifyInit(JSONSerializationContext * context) {
	TestCase_assert(context->jmpBuf == NULL, "Expected NULL but got %p", context->jmpBuf);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	TestCase_assert(context->dispose == JSONSerializationContext_dispose, "Expected %p but got %p", JSONSerializationContext_dispose, context->dispose);
	TestCase_assert(context->errorString == JSONSerialization_errorString, "Expected %p but got %p", JSONSerialization_errorString, context->errorString);
	TestCase_assert(context->beginStructure == JSONSerializationContext_beginStructure, "Expected %p but got %p", JSONSerializationContext_beginStructure, context->beginStructure);
	TestCase_assert(context->beginDictionary == JSONSerializationContext_beginDictionary, "Expected %p but got %p", JSONSerializationContext_beginDictionary, context->beginDictionary);
	TestCase_assert(context->beginArray == JSONSerializationContext_beginArray, "Expected %p but got %p", JSONSerializationContext_beginArray, context->beginArray);
	TestCase_assert(context->endStructure == JSONSerializationContext_endStructure, "Expected %p but got %p", JSONSerializationContext_endStructure, context->endStructure);
	TestCase_assert(context->endDictionary == JSONSerializationContext_endDictionary, "Expected %p but got %p", JSONSerializationContext_endDictionary, context->endDictionary);
	TestCase_assert(context->endArray == JSONSerializationContext_endArray, "Expected %p but got %p", JSONSerializationContext_endArray, context->endArray);
	TestCase_assert(context->writeBoolean == JSONSerializationContext_writeBoolean, "Expected %p but got %p", JSONSerializationContext_writeBoolean, context->writeBoolean);
	TestCase_assert(context->writeInt8 == JSONSerializationContext_writeInt8, "Expected %p but got %p", JSONSerializationContext_writeInt8, context->writeInt8);
	TestCase_assert(context->writeUInt8 == JSONSerializationContext_writeUInt8, "Expected %p but got %p", JSONSerializationContext_writeUInt8, context->writeUInt8);
	TestCase_assert(context->writeInt16 == JSONSerializationContext_writeInt16, "Expected %p but got %p", JSONSerializationContext_writeInt16, context->writeInt16);
	TestCase_assert(context->writeUInt16 == JSONSerializationContext_writeUInt16, "Expected %p but got %p", JSONSerializationContext_writeUInt16, context->writeUInt16);
	TestCase_assert(context->writeInt32 == JSONSerializationContext_writeInt32, "Expected %p but got %p", JSONSerializationContext_writeInt32, context->writeInt32);
	TestCase_assert(context->writeUInt32 == JSONSerializationContext_writeUInt32, "Expected %p but got %p", JSONSerializationContext_writeUInt32, context->writeUInt32);
	TestCase_assert(context->writeInt64 == JSONSerializationContext_writeInt64, "Expected %p but got %p", JSONSerializationContext_writeInt64, context->writeInt64);
	TestCase_assert(context->writeUInt64 == JSONSerializationContext_writeUInt64, "Expected %p but got %p", JSONSerializationContext_writeUInt64, context->writeUInt64);
	TestCase_assert(context->writeFloat == JSONSerializationContext_writeFloat, "Expected %p but got %p", JSONSerializationContext_writeFloat, context->writeFloat);
	TestCase_assert(context->writeDouble == JSONSerializationContext_writeDouble, "Expected %p but got %p", JSONSerializationContext_writeDouble, context->writeDouble);
	TestCase_assert(context->writeFixed16_16 == JSONSerializationContext_writeFixed16_16, "Expected %p but got %p", JSONSerializationContext_writeFixed16_16, context->writeFixed16_16);
	TestCase_assert(context->writeEnumeration == JSONSerializationContext_writeEnumeration, "Expected %p but got %p", JSONSerializationContext_writeEnumeration, context->writeEnumeration);
	TestCase_assert(context->writeBitfield8 == JSONSerializationContext_writeBitfield8, "Expected %p but got %p", JSONSerializationContext_writeBitfield8, context->writeBitfield8);
	TestCase_assert(context->writeBitfield16 == JSONSerializationContext_writeBitfield16, "Expected %p but got %p", JSONSerializationContext_writeBitfield16, context->writeBitfield16);
	TestCase_assert(context->writeBitfield32 == JSONSerializationContext_writeBitfield32, "Expected %p but got %p", JSONSerializationContext_writeBitfield32, context->writeBitfield32);
	TestCase_assert(context->writeBitfield64 == JSONSerializationContext_writeBitfield64, "Expected %p but got %p", JSONSerializationContext_writeBitfield64, context->writeBitfield64);
	TestCase_assert(context->writeString == JSONSerializationContext_writeString, "Expected %p but got %p", JSONSerializationContext_writeString, context->writeString);
	TestCase_assert(context->writeStringNullable == JSONSerializationContext_writeStringNullable, "Expected %p but got %p", JSONSerializationContext_writeStringNullable, context->writeStringNullable);
	TestCase_assert(context->writeBlob == JSONSerializationContext_writeBlob, "Expected %p but got %p", JSONSerializationContext_writeBlob, context->writeBlob);
}

static void testInit() {
	JSONSerializationContext context, * contextPtr;
	
	memset(&context, 0, sizeof(context));
	JSONSerializationContext_init(&context);
	verifyInit(&context);
	JSONSerializationContext_dispose(&context);
	
	contextPtr = JSONSerializationContext_create();
	TestCase_assert(contextPtr != NULL, "Expected non-NULL but got NULL");
	if (contextPtr == NULL) {return;} // Suppress clang warning
	verifyInit(contextPtr);
	contextPtr->dispose(contextPtr);
}

static void testTopLevelContainers() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "key");
	context->endStructure(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems == NULL, "Expected NULL but got %p", node->subitems);
	JSONNode_dispose(node);
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems == NULL, "Expected NULL but got %p", node->subitems);
	JSONNode_dispose(node);
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems == NULL, "Expected NULL but got %p", node->subitems);
	JSONNode_dispose(node);
}

static void testNumberValues() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt8(context, "item", 0);
	context->writeUInt8(context, "item", 1);
	context->writeInt16(context, "item", 2);
	context->writeUInt16(context, "item", 3);
	context->writeInt32(context, "item", 4);
	context->writeUInt32(context, "item", 5);
	context->writeInt64(context, "item", 6);
	context->writeUInt64(context, "item", 7);
	context->writeFloat(context, "item", 8);
	context->writeDouble(context, "item", 9);
	context->writeFixed16_16(context, "item", 0xA0000);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 11, "Expected 11 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.number == 0, "Expected 0 but got %g", node->subitems[0].value.number);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.number == 1, "Expected 1 but got %g", node->subitems[1].value.number);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].key);
	TestCase_assert(node->subitems[2].value.number == 2, "Expected 2 but got %g", node->subitems[2].value.number);
	TestCase_assert(node->subitems[3].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[3].type);
	TestCase_assert(node->subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[3].key);
	TestCase_assert(node->subitems[3].value.number == 3, "Expected 3 but got %g", node->subitems[3].value.number);
	TestCase_assert(node->subitems[4].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[4].type);
	TestCase_assert(node->subitems[4].key == NULL, "Expected NULL but got %p", node->subitems[4].key);
	TestCase_assert(node->subitems[4].value.number == 4, "Expected 4 but got %g", node->subitems[4].value.number);
	TestCase_assert(node->subitems[5].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[5].type);
	TestCase_assert(node->subitems[5].key == NULL, "Expected NULL but got %p", node->subitems[5].key);
	TestCase_assert(node->subitems[5].value.number == 5, "Expected 5 but got %g", node->subitems[5].value.number);
	TestCase_assert(node->subitems[6].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[6].type);
	TestCase_assert(node->subitems[6].key == NULL, "Expected NULL but got %p", node->subitems[6].key);
	TestCase_assert(node->subitems[6].value.number == 6, "Expected 6 but got %g", node->subitems[6].value.number);
	TestCase_assert(node->subitems[7].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[7].type);
	TestCase_assert(node->subitems[7].key == NULL, "Expected NULL but got %p", node->subitems[7].key);
	TestCase_assert(node->subitems[7].value.number == 7, "Expected 7 but got %g", node->subitems[7].value.number);
	TestCase_assert(node->subitems[8].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[8].type);
	TestCase_assert(node->subitems[8].key == NULL, "Expected NULL but got %p", node->subitems[8].key);
	TestCase_assert(node->subitems[8].value.number == 8, "Expected 8 but got %f", node->subitems[8].value.number);
	TestCase_assert(node->subitems[9].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[9].type);
	TestCase_assert(node->subitems[9].key == NULL, "Expected NULL but got %p", node->subitems[9].key);
	TestCase_assert(node->subitems[9].value.number == 9, "Expected 9 but got %f", node->subitems[9].value.number);
	TestCase_assert(node->subitems[10].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[10].type);
	TestCase_assert(node->subitems[10].key == NULL, "Expected NULL but got %p", node->subitems[10].key);
	TestCase_assert(node->subitems[10].stringLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[10].stringLength);
	TestCase_assert(!strcmp(node->subitems[10].value.string, "0xA0000"), "Expected \"0xA0000\" but got \"%s\"", node->subitems[10].value.string);
	JSONNode_dispose(node);
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt8(context, "item", INT8_MIN);
	context->writeUInt8(context, "item", UINT8_MAX);
	context->writeInt16(context, "item", INT16_MIN);
	context->writeUInt16(context, "item", UINT16_MAX);
	context->writeInt32(context, "item", INT32_MIN);
	context->writeUInt32(context, "item", UINT32_MAX);
	context->writeInt64(context, "item", -(1ll << DBL_MANT_DIG));
	context->writeUInt64(context, "item", 1ull << DBL_MANT_DIG);
	context->writeFloat(context, "item", FLT_MAX);
	context->writeDouble(context, "item", DBL_MAX);
	context->writeFixed16_16(context, "item", FIXED_16_16_MIN);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 11, "Expected 11 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.number == INT8_MIN, "Expected %d but got %g", INT8_MIN, node->subitems[0].value.number);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.number == UINT8_MAX, "Expected %d but got %g", UINT8_MAX, node->subitems[1].value.number);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].key);
	TestCase_assert(node->subitems[2].value.number == INT16_MIN, "Expected %d but got %g", INT16_MIN, node->subitems[2].value.number);
	TestCase_assert(node->subitems[3].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[3].type);
	TestCase_assert(node->subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[3].key);
	TestCase_assert(node->subitems[3].value.number == UINT16_MAX, "Expected %d but got %g", UINT16_MAX, node->subitems[3].value.number);
	TestCase_assert(node->subitems[4].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[4].type);
	TestCase_assert(node->subitems[4].key == NULL, "Expected NULL but got %p", node->subitems[4].key);
	TestCase_assert(node->subitems[4].value.number == INT32_MIN, "Expected %d but got %g", INT32_MIN, node->subitems[4].value.number);
	TestCase_assert(node->subitems[5].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[5].type);
	TestCase_assert(node->subitems[5].key == NULL, "Expected NULL but got %p", node->subitems[5].key);
	TestCase_assert(node->subitems[5].value.number == UINT32_MAX, "Expected %d but got %g", UINT32_MAX, node->subitems[5].value.number);
	TestCase_assert(node->subitems[6].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[6].type);
	TestCase_assert(node->subitems[6].key == NULL, "Expected NULL but got %p", node->subitems[6].key);
	TestCase_assert(node->subitems[6].value.number == -(1ll << DBL_MANT_DIG), "Expected " LONG_LONG_FORMAT " but got %g", -(1ll << DBL_MANT_DIG), node->subitems[6].value.number);
	TestCase_assert(node->subitems[7].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[7].type);
	TestCase_assert(node->subitems[7].key == NULL, "Expected NULL but got %p", node->subitems[7].key);
	TestCase_assert(node->subitems[7].value.number == 1ull << DBL_MANT_DIG, "Expected " ULONG_LONG_FORMAT " but got %g", 1ull << DBL_MANT_DIG, node->subitems[7].value.number);
	TestCase_assert(node->subitems[8].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[8].type);
	TestCase_assert(node->subitems[8].key == NULL, "Expected NULL but got %p", node->subitems[8].key);
	TestCase_assert(node->subitems[8].value.number == FLT_MAX, "Expected %f but got %f", FLT_MAX, node->subitems[8].value.number);
	TestCase_assert(node->subitems[9].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[9].type);
	TestCase_assert(node->subitems[9].key == NULL, "Expected NULL but got %p", node->subitems[9].key);
	TestCase_assert(node->subitems[9].value.number == DBL_MAX, "Expected %f but got %f", DBL_MAX, node->subitems[9].value.number);
	TestCase_assert(node->subitems[10].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[10].type);
	TestCase_assert(node->subitems[10].key == NULL, "Expected NULL but got %p", node->subitems[10].key);
	TestCase_assert(node->subitems[10].stringLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[10].stringLength);
	TestCase_assert(!strcmp(node->subitems[10].value.string, "0x80000002"), "Expected \"0x80000002\" but got \"%s\"", node->subitems[10].value.string);
	JSONNode_dispose(node);
}

static void testLargeIntegerRepresentability() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt64(context, "item", -(1ll << DBL_MANT_DIG));
	context->writeUInt64(context, "item", 1ull << DBL_MANT_DIG);
	context->writeInt64(context, "item", INT64_MIN + 1);
	context->writeUInt64(context, "item", UINT64_MAX);
	context->writeInt64(context, "item", -(1ll << (DBL_MANT_DIG + 1)));
	context->writeUInt64(context, "item", 1ull << (DBL_MANT_DIG + 1));
	context->writeInt64(context, "item", -((1ll << DBL_MANT_DIG) + 1));
	context->writeUInt64(context, "item", (1ull << DBL_MANT_DIG) + 1);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 8, "Expected 8 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.number == -(1ll << DBL_MANT_DIG), "Expected " LONG_LONG_FORMAT " but got %g", -(1ll << DBL_MANT_DIG), node->subitems[0].value.number);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.number == 1ull << DBL_MANT_DIG, "Expected " ULONG_LONG_FORMAT " but got %g", 1ull << DBL_MANT_DIG, node->subitems[1].value.number);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].key);
	TestCase_assert(node->subitems[2].stringLength == 20, "Expected 20 but got " SIZE_T_FORMAT, node->subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].value.string, "-9223372036854775807"), "Expected \"-9223372036854775807\" but got \"%s\"", node->subitems[2].value.string);
	TestCase_assert(node->subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[3].type);
	TestCase_assert(node->subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[3].key);
	TestCase_assert(node->subitems[3].stringLength == 20, "Expected 20 but got " SIZE_T_FORMAT, node->subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[3].value.string, "18446744073709551615"), "Expected \"18446744073709551615\" but got \"%s\"", node->subitems[3].value.string);
	TestCase_assert(node->subitems[4].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[4].type);
	TestCase_assert(node->subitems[4].key == NULL, "Expected NULL but got %p", node->subitems[4].key);
	TestCase_assert(node->subitems[4].value.number == -(1ll << (DBL_MANT_DIG + 1)), "Expected " LONG_LONG_FORMAT " but got %g", -(1ll << (DBL_MANT_DIG + 1)), node->subitems[4].value.number);
	TestCase_assert(node->subitems[5].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[5].type);
	TestCase_assert(node->subitems[5].key == NULL, "Expected NULL but got %p", node->subitems[5].key);
	TestCase_assert(node->subitems[5].value.number == 1ull << (DBL_MANT_DIG + 1), "Expected " ULONG_LONG_FORMAT " but got %g", 1ull << (DBL_MANT_DIG + 1), node->subitems[5].value.number);
	TestCase_assert(node->subitems[6].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[6].type);
	TestCase_assert(node->subitems[6].key == NULL, "Expected NULL but got %p", node->subitems[6].key);
	TestCase_assert(node->subitems[6].stringLength == 17, "Expected 17 but got " SIZE_T_FORMAT, node->subitems[6].stringLength);
	TestCase_assert(!strcmp(node->subitems[6].value.string, "-9007199254740993"), "Expected \"-9007199254740993\" but got \"%s\"", node->subitems[6].value.string);
	TestCase_assert(node->subitems[7].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[7].type);
	TestCase_assert(node->subitems[7].key == NULL, "Expected NULL but got %p", node->subitems[7].key);
	TestCase_assert(node->subitems[7].stringLength == 16, "Expected 16 but got " SIZE_T_FORMAT, node->subitems[7].stringLength);
	TestCase_assert(!strcmp(node->subitems[7].value.string, "9007199254740993"), "Expected \"9007199254740993\" but got \"%s\"", node->subitems[7].value.string);
	JSONNode_dispose(node);
}

static void testStringValues() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeString(context, "item", "foo");
	context->writeString(context, "item", "Hello, world!");
	context->writeStringNullable(context, "item", "null");
	context->writeStringNullable(context, "item", NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 4, "Expected 4 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 3, "Expected 3 but got " SIZE_T_FORMAT, node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "foo"), "Expected \"foo\" but got \"%s\"", node->subitems[0].value.string);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].stringLength == 13, "Expected 13 but got " SIZE_T_FORMAT, node->subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].value.string, "Hello, world!"), "Expected \"Hello, world!\" but got \"%s\"", node->subitems[1].value.string);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].key);
	TestCase_assert(node->subitems[2].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].value.string, "null"), "Expected \"null\" but got \"%s\"", node->subitems[2].value.string);
	TestCase_assert(node->subitems[3].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[3].type);
	TestCase_assert(node->subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[3].key);
	JSONNode_dispose(node);
}

static void testBlobValues() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeBlob(context, "item", "foo", 3);
	context->writeBlob(context, "item", "Hello, world!", 13);
	context->writeBlob(context, "item", NULL, 0);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 3, "Expected 3 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "Zm9v"), "Expected \"Zm9v\" but got \"%s\"", node->subitems[0].value.string);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].stringLength == 20, "Expected 20 but got " SIZE_T_FORMAT, node->subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].value.string, "SGVsbG8sIHdvcmxkIQ=="), "Expected \"SGVsbG8sIHdvcmxkIQ==\" but got \"%s\"", node->subitems[1].value.string);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].key);
	JSONNode_dispose(node);
}

static void testBooleanValues() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeBoolean(context, "item", false);
	context->writeBoolean(context, "item", true);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[1].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[1].value.boolean == true, "Expected true but got false");
	JSONNode_dispose(node);
}

#define ENUM_TEST_0 0
#define ENUM_TEST_1 1
#define ENUM_TEST_zero 0
#define ENUM_TEST_one 1
#define ENUM_TEST_two 2

static void testEnumerations() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", ENUM_TEST_0, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_1, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_zero, enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_one,  enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_two,  enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 5, "Expected 5 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 11, "Expected 11 but got " SIZE_T_FORMAT, node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "ENUM_TEST_0"), "Expected \"ENUM_TEST_0\" but got \"%s\"", node->subitems[0].value.string);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].stringLength == 11, "Expected 11 but got " SIZE_T_FORMAT, node->subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].value.string, "ENUM_TEST_1"), "Expected \"ENUM_TEST_1\" but got \"%s\"", node->subitems[1].value.string);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].key);
	TestCase_assert(node->subitems[2].stringLength == 14, "Expected 14 but got " SIZE_T_FORMAT, node->subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].value.string, "ENUM_TEST_zero"), "Expected \"ENUM_TEST_zero\" but got \"%s\"", node->subitems[2].value.string);
	TestCase_assert(node->subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[3].type);
	TestCase_assert(node->subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[3].key);
	TestCase_assert(node->subitems[3].stringLength == 13, "Expected 13 but got " SIZE_T_FORMAT, node->subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[3].value.string, "ENUM_TEST_one"), "Expected \"ENUM_TEST_one\" but got \"%s\"", node->subitems[3].value.string);
	TestCase_assert(node->subitems[4].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[4].type);
	TestCase_assert(node->subitems[4].key == NULL, "Expected NULL but got %p", node->subitems[4].key);
	TestCase_assert(node->subitems[4].stringLength == 13, "Expected 13 but got " SIZE_T_FORMAT, node->subitems[4].stringLength);
	TestCase_assert(!strcmp(node->subitems[4].value.string, "ENUM_TEST_two"), "Expected \"ENUM_TEST_two\" but got \"%s\"", node->subitems[4].value.string);
	JSONNode_dispose(node);
}

static void testBitfields() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeBitfield8(context, "item", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7", NULL);
	context->writeBitfield8(context, "item", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	context->writeBitfield16(context, "item", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", NULL);
	context->writeBitfield16(context, "item", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield32(context, "item", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", NULL);
	context->writeBitfield32(context, "item", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield64(context, "item", 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", NULL);
	context->writeBitfield64(context, "item", 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield8(context, "item", 0x00, NULL);
	context->writeBitfield16(context, "item", 0x0000, NULL);
	context->writeBitfield32(context, "item", 0x00000000, NULL);
	context->writeBitfield64(context, "item", 0x0000000000000000ull, NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 12, "Expected 12 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	
	TestCase_assert(node->subitems[0].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].subitems[0].type);
	TestCase_assert(node->subitems[0].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[0].key);
	TestCase_assert(node->subitems[0].subitems[0].stringLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[0].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].subitems[0].value.string, "bit_1"), "Expected \"bit_1\" but got \"%s\"", node->subitems[0].subitems[0].value.string);
	TestCase_assert(node->subitems[0].subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].subitems[1].type);
	TestCase_assert(node->subitems[0].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[1].key);
	TestCase_assert(node->subitems[0].subitems[1].stringLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[0].subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].subitems[1].value.string, "bit_3"), "Expected \"bit_3\" but got \"%s\"", node->subitems[0].subitems[1].value.string);
	TestCase_assert(node->subitems[0].subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].subitems[2].type);
	TestCase_assert(node->subitems[0].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[2].key);
	TestCase_assert(node->subitems[0].subitems[2].stringLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[0].subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].subitems[2].value.string, "bit_5"), "Expected \"bit_5\" but got \"%s\"", node->subitems[0].subitems[2].value.string);
	TestCase_assert(node->subitems[0].subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].subitems[3].type);
	TestCase_assert(node->subitems[0].subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[3].key);
	TestCase_assert(node->subitems[0].subitems[3].stringLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[0].subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].subitems[3].value.string, "bit_7"), "Expected \"bit_7\" but got \"%s\"", node->subitems[0].subitems[3].value.string);
	
	TestCase_assert(node->subitems[1].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[0].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[1].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[0].value.string, "bit0"), "Expected \"bit0\" but got \"%s\"", node->subitems[1].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[1].type);
	TestCase_assert(node->subitems[1].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].subitems[1].key);
	TestCase_assert(node->subitems[1].subitems[1].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].value.string, "bit1"), "Expected \"bit1\" but got \"%s\"", node->subitems[1].subitems[1].value.string);
	TestCase_assert(node->subitems[1].subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[2].type);
	TestCase_assert(node->subitems[1].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[1].subitems[2].key);
	TestCase_assert(node->subitems[1].subitems[2].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[1].subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[2].value.string, "bit2"), "Expected \"bit2\" but got \"%s\"", node->subitems[1].subitems[2].value.string);
	TestCase_assert(node->subitems[1].subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[3].type);
	TestCase_assert(node->subitems[1].subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[1].subitems[3].key);
	TestCase_assert(node->subitems[1].subitems[3].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[1].subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[3].value.string, "bit4"), "Expected \"bit4\" but got \"%s\"", node->subitems[1].subitems[3].value.string);
	TestCase_assert(node->subitems[1].subitems[4].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[4].type);
	TestCase_assert(node->subitems[1].subitems[4].key == NULL, "Expected NULL but got %p", node->subitems[1].subitems[4].key);
	TestCase_assert(node->subitems[1].subitems[4].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[1].subitems[4].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[4].value.string, "bit6"), "Expected \"bit6\" but got \"%s\"", node->subitems[1].subitems[4].value.string);
	
	TestCase_assert(node->subitems[2].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].key);
	TestCase_assert(node->subitems[2].value.count == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[2].value.count);
	TestCase_assert(node->subitems[2].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[2].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].subitems[0].type);
	TestCase_assert(node->subitems[2].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[2].subitems[0].key);
	TestCase_assert(node->subitems[2].subitems[0].stringLength == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[2].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].subitems[0].value.string, "0"), "Expected \"0\" but got \"%s\"", node->subitems[2].subitems[0].value.string);
	TestCase_assert(node->subitems[2].subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].subitems[1].type);
	TestCase_assert(node->subitems[2].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[2].subitems[1].key);
	TestCase_assert(node->subitems[2].subitems[1].stringLength == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[2].subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].subitems[1].value.string, "C"), "Expected \"C\" but got \"%s\"", node->subitems[2].subitems[1].value.string);
	TestCase_assert(node->subitems[2].subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].subitems[2].type);
	TestCase_assert(node->subitems[2].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].subitems[2].key);
	TestCase_assert(node->subitems[2].subitems[2].stringLength == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[2].subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].subitems[2].value.string, "D"), "Expected \"D\" but got \"%s\"", node->subitems[2].subitems[2].value.string);
	TestCase_assert(node->subitems[2].subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].subitems[3].type);
	TestCase_assert(node->subitems[2].subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[2].subitems[3].key);
	TestCase_assert(node->subitems[2].subitems[3].stringLength == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[2].subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].subitems[3].value.string, "E"), "Expected \"E\" but got \"%s\"", node->subitems[2].subitems[3].value.string);
	TestCase_assert(node->subitems[2].subitems[4].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].subitems[4].type);
	TestCase_assert(node->subitems[2].subitems[4].key == NULL, "Expected NULL but got %p", node->subitems[2].subitems[4].key);
	TestCase_assert(node->subitems[2].subitems[4].stringLength == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[2].subitems[4].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].subitems[4].value.string, "F"), "Expected \"F\" but got \"%s\"", node->subitems[2].subitems[4].value.string);
	
	TestCase_assert(node->subitems[3].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[3].type);
	TestCase_assert(node->subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[3].key);
	TestCase_assert(node->subitems[3].value.count == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[3].value.count);
	TestCase_assert(node->subitems[3].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[3].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[3].subitems[0].type);
	TestCase_assert(node->subitems[3].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[3].subitems[0].key);
	TestCase_assert(node->subitems[3].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[3].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[3].subitems[0].value.string, "b0"), "Expected \"b0\" but got \"%s\"", node->subitems[3].subitems[0].value.string);
	TestCase_assert(node->subitems[3].subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[3].subitems[1].type);
	TestCase_assert(node->subitems[3].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[3].subitems[1].key);
	TestCase_assert(node->subitems[3].subitems[1].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[3].subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[3].subitems[1].value.string, "b1"), "Expected \"b1\" but got \"%s\"", node->subitems[3].subitems[1].value.string);
	TestCase_assert(node->subitems[3].subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[3].subitems[2].type);
	TestCase_assert(node->subitems[3].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[3].subitems[2].key);
	TestCase_assert(node->subitems[3].subitems[2].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[3].subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[3].subitems[2].value.string, "b2"), "Expected \"b2\" but got \"%s\"", node->subitems[3].subitems[2].value.string);
	TestCase_assert(node->subitems[3].subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[3].subitems[3].type);
	TestCase_assert(node->subitems[3].subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[3].subitems[3].key);
	TestCase_assert(node->subitems[3].subitems[3].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[3].subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[3].subitems[3].value.string, "b3"), "Expected \"b3\" but got \"%s\"", node->subitems[3].subitems[3].value.string);
	
	TestCase_assert(node->subitems[4].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[4].type);
	TestCase_assert(node->subitems[4].key == NULL, "Expected NULL but got %p", node->subitems[4].key);
	TestCase_assert(node->subitems[4].value.count == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[4].value.count);
	TestCase_assert(node->subitems[4].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[4].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[4].subitems[0].type);
	TestCase_assert(node->subitems[4].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[4].subitems[0].key);
	TestCase_assert(node->subitems[4].subitems[0].stringLength == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[4].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[4].subitems[0].value.string, "0"), "Expected \"0\" but got \"%s\"", node->subitems[4].subitems[0].value.string);
	TestCase_assert(node->subitems[4].subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[4].subitems[1].type);
	TestCase_assert(node->subitems[4].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[4].subitems[1].key);
	TestCase_assert(node->subitems[4].subitems[1].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[4].subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[4].subitems[1].value.string, "1C"), "Expected \"1C\" but got \"%s\"", node->subitems[4].subitems[1].value.string);
	TestCase_assert(node->subitems[4].subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[4].subitems[2].type);
	TestCase_assert(node->subitems[4].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[4].subitems[2].key);
	TestCase_assert(node->subitems[4].subitems[2].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[4].subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[4].subitems[2].value.string, "1D"), "Expected \"1D\" but got \"%s\"", node->subitems[4].subitems[2].value.string);
	TestCase_assert(node->subitems[4].subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[4].subitems[3].type);
	TestCase_assert(node->subitems[4].subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[4].subitems[3].key);
	TestCase_assert(node->subitems[4].subitems[3].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[4].subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[4].subitems[3].value.string, "1E"), "Expected \"1E\" but got \"%s\"", node->subitems[4].subitems[3].value.string);
	TestCase_assert(node->subitems[4].subitems[4].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[4].subitems[4].type);
	TestCase_assert(node->subitems[4].subitems[4].key == NULL, "Expected NULL but got %p", node->subitems[4].subitems[4].key);
	TestCase_assert(node->subitems[4].subitems[4].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[4].subitems[4].stringLength);
	TestCase_assert(!strcmp(node->subitems[4].subitems[4].value.string, "1F"), "Expected \"1F\" but got \"%s\"", node->subitems[4].subitems[4].value.string);
	
	TestCase_assert(node->subitems[5].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[5].type);
	TestCase_assert(node->subitems[5].key == NULL, "Expected NULL but got %p", node->subitems[5].key);
	TestCase_assert(node->subitems[5].value.count == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[5].value.count);
	TestCase_assert(node->subitems[5].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[5].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[5].subitems[0].type);
	TestCase_assert(node->subitems[5].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[5].subitems[0].key);
	TestCase_assert(node->subitems[5].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[5].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[5].subitems[0].value.string, "b1"), "Expected \"b1\" but got \"%s\"", node->subitems[5].subitems[0].value.string);
	TestCase_assert(node->subitems[5].subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[5].subitems[1].type);
	TestCase_assert(node->subitems[5].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[5].subitems[1].key);
	TestCase_assert(node->subitems[5].subitems[1].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[5].subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[5].subitems[1].value.string, "b2"), "Expected \"b2\" but got \"%s\"", node->subitems[5].subitems[1].value.string);
	TestCase_assert(node->subitems[5].subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[5].subitems[2].type);
	TestCase_assert(node->subitems[5].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[5].subitems[2].key);
	TestCase_assert(node->subitems[5].subitems[2].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[5].subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[5].subitems[2].value.string, "b3"), "Expected \"b3\" but got \"%s\"", node->subitems[5].subitems[2].value.string);
	TestCase_assert(node->subitems[5].subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[5].subitems[3].type);
	TestCase_assert(node->subitems[5].subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[5].subitems[3].key);
	TestCase_assert(node->subitems[5].subitems[3].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[5].subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[5].subitems[3].value.string, "b4"), "Expected \"b4\" but got \"%s\"", node->subitems[5].subitems[3].value.string);
	
	TestCase_assert(node->subitems[6].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[6].type);
	TestCase_assert(node->subitems[6].key == NULL, "Expected NULL but got %p", node->subitems[6].key);
	TestCase_assert(node->subitems[6].value.count == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[6].value.count);
	TestCase_assert(node->subitems[6].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[6].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[6].subitems[0].type);
	TestCase_assert(node->subitems[6].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[6].subitems[0].key);
	TestCase_assert(node->subitems[6].subitems[0].stringLength == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[6].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[6].subitems[0].value.string, "0"), "Expected \"0\" but got \"%s\"", node->subitems[6].subitems[0].value.string);
	TestCase_assert(node->subitems[6].subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[6].subitems[1].type);
	TestCase_assert(node->subitems[6].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[6].subitems[1].key);
	TestCase_assert(node->subitems[6].subitems[1].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[6].subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[6].subitems[1].value.string, "3C"), "Expected \"3C\" but got \"%s\"", node->subitems[6].subitems[1].value.string);
	TestCase_assert(node->subitems[6].subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[6].subitems[2].type);
	TestCase_assert(node->subitems[6].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[6].subitems[2].key);
	TestCase_assert(node->subitems[6].subitems[2].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[6].subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[6].subitems[2].value.string, "3D"), "Expected \"3D\" but got \"%s\"", node->subitems[6].subitems[2].value.string);
	TestCase_assert(node->subitems[6].subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[6].subitems[3].type);
	TestCase_assert(node->subitems[6].subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[6].subitems[3].key);
	TestCase_assert(node->subitems[6].subitems[3].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[6].subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[6].subitems[3].value.string, "3E"), "Expected \"3E\" but got \"%s\"", node->subitems[6].subitems[3].value.string);
	TestCase_assert(node->subitems[6].subitems[4].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[6].subitems[4].type);
	TestCase_assert(node->subitems[6].subitems[4].key == NULL, "Expected NULL but got %p", node->subitems[6].subitems[4].key);
	TestCase_assert(node->subitems[6].subitems[4].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[6].subitems[4].stringLength);
	TestCase_assert(!strcmp(node->subitems[6].subitems[4].value.string, "3F"), "Expected \"3F\" but got \"%s\"", node->subitems[6].subitems[4].value.string);
	
	TestCase_assert(node->subitems[7].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[7].type);
	TestCase_assert(node->subitems[7].key == NULL, "Expected NULL but got %p", node->subitems[7].key);
	TestCase_assert(node->subitems[7].value.count == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[7].value.count);
	TestCase_assert(node->subitems[7].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[7].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[7].subitems[0].type);
	TestCase_assert(node->subitems[7].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[7].subitems[0].key);
	TestCase_assert(node->subitems[7].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[7].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[7].subitems[0].value.string, "b2"), "Expected \"b2\" but got \"%s\"", node->subitems[7].subitems[0].value.string);
	TestCase_assert(node->subitems[7].subitems[1].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[7].subitems[1].type);
	TestCase_assert(node->subitems[7].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[7].subitems[1].key);
	TestCase_assert(node->subitems[7].subitems[1].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[7].subitems[1].stringLength);
	TestCase_assert(!strcmp(node->subitems[7].subitems[1].value.string, "b3"), "Expected \"b3\" but got \"%s\"", node->subitems[7].subitems[1].value.string);
	TestCase_assert(node->subitems[7].subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[7].subitems[2].type);
	TestCase_assert(node->subitems[7].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[7].subitems[2].key);
	TestCase_assert(node->subitems[7].subitems[2].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[7].subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[7].subitems[2].value.string, "b4"), "Expected \"b4\" but got \"%s\"", node->subitems[7].subitems[2].value.string);
	TestCase_assert(node->subitems[7].subitems[3].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[7].subitems[3].type);
	TestCase_assert(node->subitems[7].subitems[3].key == NULL, "Expected NULL but got %p", node->subitems[7].subitems[3].key);
	TestCase_assert(node->subitems[7].subitems[3].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[7].subitems[3].stringLength);
	TestCase_assert(!strcmp(node->subitems[7].subitems[3].value.string, "b5"), "Expected \"b5\" but got \"%s\"", node->subitems[7].subitems[3].value.string);
	
	TestCase_assert(node->subitems[8].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[8].type);
	TestCase_assert(node->subitems[8].key == NULL, "Expected NULL but got %p", node->subitems[8].key);
	TestCase_assert(node->subitems[8].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[8].value.count);
	TestCase_assert(node->subitems[8].subitems == NULL, "Expected NULL but got %p", node->subitems[8].subitems);
	
	TestCase_assert(node->subitems[9].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[9].type);
	TestCase_assert(node->subitems[9].key == NULL, "Expected NULL but got %p", node->subitems[9].key);
	TestCase_assert(node->subitems[9].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[9].value.count);
	TestCase_assert(node->subitems[9].subitems == NULL, "Expected NULL but got %p", node->subitems[9].subitems);
	
	TestCase_assert(node->subitems[10].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[10].type);
	TestCase_assert(node->subitems[10].key == NULL, "Expected NULL but got %p", node->subitems[10].key);
	TestCase_assert(node->subitems[10].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[10].value.count);
	TestCase_assert(node->subitems[10].subitems == NULL, "Expected NULL but got %p", node->subitems[10].subitems);
	
	TestCase_assert(node->subitems[11].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[11].type);
	TestCase_assert(node->subitems[11].key == NULL, "Expected NULL but got %p", node->subitems[11].key);
	TestCase_assert(node->subitems[11].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[11].value.count);
	TestCase_assert(node->subitems[11].subitems == NULL, "Expected NULL but got %p", node->subitems[11].subitems);
	
	JSONNode_dispose(node);
}

static void testArrays() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
		context->beginArray(context, "item");
		context->endArray(context);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems == NULL, "Expected NULL but got %p", node->subitems[0].subitems);
	JSONNode_dispose(node);
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
		context->beginArray(context, "item");
			context->writeInt8(context, "item", 0);
		context->endArray(context);
		context->beginArray(context, "item");
			context->writeUInt8(context, "item", 1);
			context->beginArray(context, "item");
				context->writeInt16(context, "item", 2);
				context->writeUInt16(context, "item", 3);
				context->writeInt32(context, "item", 4);
				context->writeUInt32(context, "item", 5);
				context->writeInt64(context, "item", 6);
				context->writeUInt64(context, "item", 7);
			context->endArray(context);
			context->writeFloat(context, "item", 8);
			context->writeDouble(context, "item", 9);
			context->writeFixed16_16(context, "item", 0xA0000);
			context->writeString(context, "item", "10");
			context->writeStringNullable(context, "item", NULL);
			context->writeBoolean(context, "item", true);
			context->writeEnumeration(context, "item", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "item", 1, "13", NULL);
			context->writeBitfield16(context, "item", 1, "14", NULL);
			context->writeBitfield32(context, "item", 1, "15", NULL);
			context->writeBitfield64(context, "item", 1, "16", NULL);
		context->endArray(context);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].subitems[0].type);
	TestCase_assert(node->subitems[0].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[0].subitems[0].key);
	TestCase_assert(node->subitems[0].subitems[0].value.number == 0, "Expected 0 but got %g", node->subitems[0].subitems[0].value.number);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 13, "Expected 13 but got " SIZE_T_FORMAT, node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[0].value.number == 1, "Expected 1 but got %g", node->subitems[1].subitems[0].value.number);
	TestCase_assert(node->subitems[1].subitems[1].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[1].type);
	TestCase_assert(node->subitems[1].subitems[1].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[1].key);
	TestCase_assert(node->subitems[1].subitems[1].value.count == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[1].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[0].value.number == 2, "Expected 2 but got %g", node->subitems[1].subitems[1].subitems[0].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[1].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[1].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[1].subitems[1].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[1].value.number == 3, "Expected 3 but got %g", node->subitems[1].subitems[1].subitems[1].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[2].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[2].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[2].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[1].subitems[2].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[2].value.number == 4, "Expected 4 but got %g", node->subitems[1].subitems[1].subitems[2].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[3].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[3].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[3].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[1].subitems[3].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[3].value.number == 5, "Expected 5 but got %g", node->subitems[1].subitems[1].subitems[3].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[4].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[4].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[4].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[1].subitems[4].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[4].value.number == 6, "Expected 6 but got %g", node->subitems[1].subitems[1].subitems[4].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[5].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[5].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[5].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[1].subitems[5].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[5].value.number == 7, "Expected 7 but got %g", node->subitems[1].subitems[1].subitems[5].value.number);
	TestCase_assert(node->subitems[1].subitems[2].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[2].type);
	TestCase_assert(node->subitems[1].subitems[2].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[2].key);
	TestCase_assert(node->subitems[1].subitems[2].value.number == 8, "Expected 8 but got %g", node->subitems[1].subitems[2].value.number);
	TestCase_assert(node->subitems[1].subitems[3].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[3].type);
	TestCase_assert(node->subitems[1].subitems[3].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[3].key);
	TestCase_assert(node->subitems[1].subitems[3].value.number == 9, "Expected 9 but got %g", node->subitems[1].subitems[3].value.number);
	TestCase_assert(node->subitems[1].subitems[4].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[4].type);
	TestCase_assert(node->subitems[1].subitems[4].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[4].key);
	TestCase_assert(node->subitems[1].subitems[4].stringLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[1].subitems[4].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[4].value.string, "0xA0000"), "Expected \"0xA0000\" but got \"%s\"", node->subitems[1].subitems[4].value.string);
	TestCase_assert(node->subitems[1].subitems[5].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[5].type);
	TestCase_assert(node->subitems[1].subitems[5].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[5].key);
	TestCase_assert(node->subitems[1].subitems[5].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[5].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[5].value.string, "10"), "Expected \"10\" but got \"%s\"", node->subitems[1].subitems[5].value.string);
	TestCase_assert(node->subitems[1].subitems[6].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[1].subitems[6].type);
	TestCase_assert(node->subitems[1].subitems[6].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[6].key);
	TestCase_assert(node->subitems[1].subitems[7].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[1].subitems[7].type);
	TestCase_assert(node->subitems[1].subitems[7].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[7].key);
	TestCase_assert(node->subitems[1].subitems[7].value.boolean == true, "Expected true but got false");
	TestCase_assert(node->subitems[1].subitems[8].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[8].type);
	TestCase_assert(node->subitems[1].subitems[8].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[8].key);
	TestCase_assert(node->subitems[1].subitems[8].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[1].subitems[8].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[8].value.string, "enum"), "Expected \"enum\" but got \"%s\"", node->subitems[1].subitems[8].value.string);
	TestCase_assert(node->subitems[1].subitems[9].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[9].type);
	TestCase_assert(node->subitems[1].subitems[9].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[9].key);
	TestCase_assert(node->subitems[1].subitems[9].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[9].value.count);
	TestCase_assert(node->subitems[1].subitems[9].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[9].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[9].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[9].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[9].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[9].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[9].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[9].subitems[0].value.string, "13"), "Expected \"13\" but got \"%s\"", node->subitems[1].subitems[9].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[10].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[10].type);
	TestCase_assert(node->subitems[1].subitems[10].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[10].key);
	TestCase_assert(node->subitems[1].subitems[10].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[10].value.count);
	TestCase_assert(node->subitems[1].subitems[10].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[10].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[10].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[10].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[10].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[10].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[10].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[10].subitems[0].value.string, "14"), "Expected \"14\" but got \"%s\"", node->subitems[1].subitems[10].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[11].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[11].type);
	TestCase_assert(node->subitems[1].subitems[11].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[11].key);
	TestCase_assert(node->subitems[1].subitems[11].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[11].value.count);
	TestCase_assert(node->subitems[1].subitems[11].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[11].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[11].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[11].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[11].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[11].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[11].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[11].subitems[0].value.string, "15"), "Expected \"15\" but got \"%s\"", node->subitems[1].subitems[11].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[12].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[12].type);
	TestCase_assert(node->subitems[1].subitems[12].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[12].key);
	TestCase_assert(node->subitems[1].subitems[12].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[12].value.count);
	TestCase_assert(node->subitems[1].subitems[12].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[12].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[12].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[12].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[12].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[12].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[12].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[12].subitems[0].value.string, "16"), "Expected \"16\" but got \"%s\"", node->subitems[1].subitems[12].subitems[0].value.string);
	JSONNode_dispose(node);
}

static void testStructures() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "key");
		context->beginStructure(context, "item");
		context->endStructure(context);
	context->endStructure(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].type);
	TestCase_assert(node->subitems[0].keyLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "item"), "Expected \"item\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems == NULL, "Expected NULL but got %p", node->subitems[0].subitems);
	JSONNode_dispose(node);
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "key");
		context->beginStructure(context, "struct1");
			context->writeInt8(context, "int8", 0);
		context->endStructure(context);
		context->beginStructure(context, "struct2");
			context->writeUInt8(context, "uint8", 1);
			context->beginStructure(context, "struct3");
				context->writeInt16(context, "int16", 2);
				context->writeUInt16(context, "uint16", 3);
				context->writeInt32(context, "int32", 4);
				context->writeUInt32(context, "uint32", 5);
				context->writeInt64(context, "int64", 6);
				context->writeUInt64(context, "uint64", 7);
			context->endStructure(context);
			context->writeFloat(context, "float", 8);
			context->writeDouble(context, "double", 9);
			context->writeString(context, "string", "10");
			context->writeStringNullable(context, "string_null", NULL);
			context->writeBoolean(context, "boolean", true);
			context->writeEnumeration(context, "enumeration", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "bitfield8", 1, "13", NULL);
			context->writeBitfield16(context, "bitfield16", 1, "14", NULL);
			context->writeBitfield32(context, "bitfield32", 1, "15", NULL);
			context->writeBitfield64(context, "bitfield64", 1, "16", NULL);
			context->writeFixed16_16(context, "fixed16_16", 0xA0000);
		context->endStructure(context);
	context->endStructure(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].type);
	TestCase_assert(node->subitems[0].keyLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "struct1"), "Expected \"struct1\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].subitems[0].type);
	TestCase_assert(node->subitems[0].subitems[0].keyLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[0].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].subitems[0].key, "int8"), "Expected \"int8\" but got \"%s\"", node->subitems[0].subitems[0].key);
	TestCase_assert(node->subitems[0].subitems[0].value.number == 0, "Expected 0 but got %g", node->subitems[0].subitems[0].value.number);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].type);
	TestCase_assert(node->subitems[1].keyLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].key, "struct2"), "Expected \"struct2\" but got \"%s\"", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 13, "Expected 13 but got " SIZE_T_FORMAT, node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[0].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[0].key, "uint8"), "Expected \"uint8\" but got \"%s\"", node->subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[0].value.number == 1, "Expected 1 but got %g", node->subitems[1].subitems[0].value.number);
	TestCase_assert(node->subitems[1].subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].subitems[1].type);
	TestCase_assert(node->subitems[1].subitems[1].keyLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].key, "struct3"), "Expected \"struct3\" but got \"%s\"", node->subitems[1].subitems[1].key);
	TestCase_assert(node->subitems[1].subitems[1].value.count == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[1].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[0].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[0].key, "int16"), "Expected \"int16\" but got \"%s\"", node->subitems[1].subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[0].value.number == 2, "Expected 2 but got %g", node->subitems[1].subitems[1].subitems[0].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[1].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[1].keyLength == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[1].key, "uint16"), "Expected \"uint16\" but got \"%s\"", node->subitems[1].subitems[1].subitems[1].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[1].value.number == 3, "Expected 3 but got %g", node->subitems[1].subitems[1].subitems[1].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[2].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[2].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[2].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[2].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[2].key, "int32"), "Expected \"int32\" but got \"%s\"", node->subitems[1].subitems[1].subitems[2].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[2].value.number == 4, "Expected 4 but got %g", node->subitems[1].subitems[1].subitems[2].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[3].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[3].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[3].keyLength == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[3].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[3].key, "uint32"), "Expected \"uint32\" but got \"%s\"", node->subitems[1].subitems[1].subitems[3].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[3].value.number == 5, "Expected 5 but got %g", node->subitems[1].subitems[1].subitems[3].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[4].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[4].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[4].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[4].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[4].key, "int64"), "Expected \"int64\" but got \"%s\"", node->subitems[1].subitems[1].subitems[4].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[4].value.number == 6, "Expected 6 but got %g", node->subitems[1].subitems[1].subitems[4].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[5].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[5].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[5].keyLength == 6, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[5].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[5].key, "uint64"), "Expected \"uint64\" but got \"%s\"", node->subitems[1].subitems[1].subitems[5].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[5].value.number == 7, "Expected 7 but got %g", node->subitems[1].subitems[1].subitems[5].value.number);
	TestCase_assert(node->subitems[1].subitems[2].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[2].type);
	TestCase_assert(node->subitems[1].subitems[2].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[2].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[2].key, "float"), "Expected \"float\" but got \"%s\"", node->subitems[1].subitems[2].key);
	TestCase_assert(node->subitems[1].subitems[2].value.number == 8, "Expected 8 but got %g", node->subitems[1].subitems[2].value.number);
	TestCase_assert(node->subitems[1].subitems[3].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[3].type);
	TestCase_assert(node->subitems[1].subitems[3].keyLength == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[3].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[3].key, "double"), "Expected \"double\" but got \"%s\"", node->subitems[1].subitems[3].key);
	TestCase_assert(node->subitems[1].subitems[3].value.number == 9, "Expected 9 but got %g", node->subitems[1].subitems[3].value.number);
	TestCase_assert(node->subitems[1].subitems[4].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[4].type);
	TestCase_assert(node->subitems[1].subitems[4].keyLength == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[4].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[4].key, "string"), "Expected \"string\" but got \"%s\"", node->subitems[1].subitems[4].key);
	TestCase_assert(node->subitems[1].subitems[4].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[4].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[4].value.string, "10"), "Expected \"10\" but got \"%s\"", node->subitems[1].subitems[4].value.string);
	TestCase_assert(node->subitems[1].subitems[5].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[1].subitems[5].type);
	TestCase_assert(node->subitems[1].subitems[5].keyLength == 11, "Expected 11 but got " SIZE_T_FORMAT, node->subitems[1].subitems[5].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[5].key, "string_null"), "Expected \"string_null\" but got \"%s\"", node->subitems[1].subitems[5].key);
	TestCase_assert(node->subitems[1].subitems[6].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[1].subitems[6].type);
	TestCase_assert(node->subitems[1].subitems[6].keyLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[1].subitems[6].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[6].key, "boolean"), "Expected \"boolean\" but got \"%s\"", node->subitems[1].subitems[6].key);
	TestCase_assert(node->subitems[1].subitems[6].value.boolean == true, "Expected true but got false");
	TestCase_assert(node->subitems[1].subitems[7].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[7].type);
	TestCase_assert(node->subitems[1].subitems[7].keyLength == 11, "Expected 11 but got " SIZE_T_FORMAT, node->subitems[1].subitems[7].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[7].key, "enumeration"), "Expected \"enumeration\" but got \"%s\"", node->subitems[1].subitems[7].key);
	TestCase_assert(node->subitems[1].subitems[7].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[1].subitems[7].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[7].value.string, "enum"), "Expected \"enum\" but got \"%s\"", node->subitems[1].subitems[7].value.string);
	TestCase_assert(node->subitems[1].subitems[8].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[8].type);
	TestCase_assert(node->subitems[1].subitems[8].keyLength == 9, "Expected 9 but got " SIZE_T_FORMAT, node->subitems[1].subitems[8].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[8].key, "bitfield8"), "Expected \"bitfield8\" but got \"%s\"", node->subitems[1].subitems[8].key);
	TestCase_assert(node->subitems[1].subitems[8].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[8].value.count);
	TestCase_assert(node->subitems[1].subitems[8].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[8].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[8].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[8].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[8].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[8].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[8].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[8].subitems[0].value.string, "13"), "Expected \"13\" but got \"%s\"", node->subitems[1].subitems[8].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[9].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[9].type);
	TestCase_assert(node->subitems[1].subitems[9].keyLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[1].subitems[9].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[9].key, "bitfield16"), "Expected \"bitfield16\" but got \"%s\"", node->subitems[1].subitems[9].key);
	TestCase_assert(node->subitems[1].subitems[9].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[9].value.count);
	TestCase_assert(node->subitems[1].subitems[9].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[9].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[9].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[9].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[9].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[9].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[9].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[9].subitems[0].value.string, "14"), "Expected \"14\" but got \"%s\"", node->subitems[1].subitems[9].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[10].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[10].type);
	TestCase_assert(node->subitems[1].subitems[10].keyLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[1].subitems[10].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[10].key, "bitfield32"), "Expected \"bitfield32\" but got \"%s\"", node->subitems[1].subitems[10].key);
	TestCase_assert(node->subitems[1].subitems[10].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[10].value.count);
	TestCase_assert(node->subitems[1].subitems[10].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[10].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[10].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[10].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[10].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[10].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[10].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[10].subitems[0].value.string, "15"), "Expected \"15\" but got \"%s\"", node->subitems[1].subitems[10].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[11].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[11].type);
	TestCase_assert(node->subitems[1].subitems[11].keyLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[1].subitems[11].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[11].key, "bitfield64"), "Expected \"bitfield64\" but got \"%s\"", node->subitems[1].subitems[11].key);
	TestCase_assert(node->subitems[1].subitems[11].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[11].value.count);
	TestCase_assert(node->subitems[1].subitems[11].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[11].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[11].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[11].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[11].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[11].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[11].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[11].subitems[0].value.string, "16"), "Expected \"16\" but got \"%s\"", node->subitems[1].subitems[11].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[12].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[12].type);
	TestCase_assert(node->subitems[1].subitems[12].keyLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[1].subitems[12].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[12].key, "fixed16_16"), "Expected \"fixed16_16\" but got \"%s\"", node->subitems[1].subitems[12].key);
	TestCase_assert(node->subitems[1].subitems[12].stringLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[1].subitems[12].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[12].value.string, "0xA0000"), "Expected \"0xA0000\" but got \"%s\"", node->subitems[1].subitems[12].value.string);
	JSONNode_dispose(node);
}

static void testDictionaries() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "key");
		context->beginDictionary(context, "item");
		context->endDictionary(context);
	context->endDictionary(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].type);
	TestCase_assert(node->subitems[0].keyLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "item"), "Expected \"item\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems == NULL, "Expected NULL but got %p", node->subitems[0].subitems);
	JSONNode_dispose(node);
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "key");
		context->beginDictionary(context, "struct1");
			context->writeInt8(context, "int8", 0);
		context->endDictionary(context);
		context->beginDictionary(context, "struct2");
			context->writeUInt8(context, "uint8", 1);
			context->beginDictionary(context, "struct3");
				context->writeInt16(context, "int16", 2);
				context->writeUInt16(context, "uint16", 3);
				context->writeInt32(context, "int32", 4);
				context->writeUInt32(context, "uint32", 5);
				context->writeInt64(context, "int64", 6);
				context->writeUInt64(context, "uint64", 7);
			context->endDictionary(context);
			context->writeFloat(context, "float", 8);
			context->writeDouble(context, "double", 9);
			context->writeString(context, "string", "10");
			context->writeStringNullable(context, "string_null", NULL);
			context->writeBoolean(context, "boolean", true);
			context->writeEnumeration(context, "enumeration", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "bitfield8", 1, "13", NULL);
			context->writeBitfield16(context, "bitfield16", 1, "14", NULL);
			context->writeBitfield32(context, "bitfield32", 1, "15", NULL);
			context->writeBitfield64(context, "bitfield64", 1, "16", NULL);
			context->writeFixed16_16(context, "fixed16_16", 0xA0000);
		context->endDictionary(context);
	context->endDictionary(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].type);
	TestCase_assert(node->subitems[0].keyLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "struct1"), "Expected \"struct1\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].subitems[0].type);
	TestCase_assert(node->subitems[0].subitems[0].keyLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[0].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].subitems[0].key, "int8"), "Expected \"int8\" but got \"%s\"", node->subitems[0].subitems[0].key);
	TestCase_assert(node->subitems[0].subitems[0].value.number == 0, "Expected 0 but got %g", node->subitems[0].subitems[0].value.number);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].type);
	TestCase_assert(node->subitems[1].keyLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].key, "struct2"), "Expected \"struct2\" but got \"%s\"", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 13, "Expected 13 but got " SIZE_T_FORMAT, node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[0].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[0].key, "uint8"), "Expected \"uint8\" but got \"%s\"", node->subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[0].value.number == 1, "Expected 1 but got %g", node->subitems[1].subitems[0].value.number);
	TestCase_assert(node->subitems[1].subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].subitems[1].type);
	TestCase_assert(node->subitems[1].subitems[1].keyLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].key, "struct3"), "Expected \"struct3\" but got \"%s\"", node->subitems[1].subitems[1].key);
	TestCase_assert(node->subitems[1].subitems[1].value.count == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[1].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[0].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[0].key, "int16"), "Expected \"int16\" but got \"%s\"", node->subitems[1].subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[0].value.number == 2, "Expected 2 but got %g", node->subitems[1].subitems[1].subitems[0].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[1].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[1].keyLength == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[1].key, "uint16"), "Expected \"uint16\" but got \"%s\"", node->subitems[1].subitems[1].subitems[1].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[1].value.number == 3, "Expected 3 but got %g", node->subitems[1].subitems[1].subitems[1].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[2].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[2].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[2].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[2].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[2].key, "int32"), "Expected \"int32\" but got \"%s\"", node->subitems[1].subitems[1].subitems[2].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[2].value.number == 4, "Expected 4 but got %g", node->subitems[1].subitems[1].subitems[2].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[3].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[3].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[3].keyLength == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[3].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[3].key, "uint32"), "Expected \"uint32\" but got \"%s\"", node->subitems[1].subitems[1].subitems[3].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[3].value.number == 5, "Expected 5 but got %g", node->subitems[1].subitems[1].subitems[3].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[4].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[4].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[4].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[4].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[4].key, "int64"), "Expected \"int64\" but got \"%s\"", node->subitems[1].subitems[1].subitems[4].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[4].value.number == 6, "Expected 6 but got %g", node->subitems[1].subitems[1].subitems[4].value.number);
	TestCase_assert(node->subitems[1].subitems[1].subitems[5].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[1].subitems[5].type);
	TestCase_assert(node->subitems[1].subitems[1].subitems[5].keyLength == 6, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].subitems[5].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].subitems[5].key, "uint64"), "Expected \"uint64\" but got \"%s\"", node->subitems[1].subitems[1].subitems[5].key);
	TestCase_assert(node->subitems[1].subitems[1].subitems[5].value.number == 7, "Expected 7 but got %g", node->subitems[1].subitems[1].subitems[5].value.number);
	TestCase_assert(node->subitems[1].subitems[2].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[2].type);
	TestCase_assert(node->subitems[1].subitems[2].keyLength == 5, "Expected 5 but got " SIZE_T_FORMAT, node->subitems[1].subitems[2].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[2].key, "float"), "Expected \"float\" but got \"%s\"", node->subitems[1].subitems[2].key);
	TestCase_assert(node->subitems[1].subitems[2].value.number == 8, "Expected 8 but got %g", node->subitems[1].subitems[2].value.number);
	TestCase_assert(node->subitems[1].subitems[3].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[3].type);
	TestCase_assert(node->subitems[1].subitems[3].keyLength == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[3].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[3].key, "double"), "Expected \"double\" but got \"%s\"", node->subitems[1].subitems[3].key);
	TestCase_assert(node->subitems[1].subitems[3].value.number == 9, "Expected 9 but got %g", node->subitems[1].subitems[3].value.number);
	TestCase_assert(node->subitems[1].subitems[4].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[4].type);
	TestCase_assert(node->subitems[1].subitems[4].keyLength == 6, "Expected 6 but got " SIZE_T_FORMAT, node->subitems[1].subitems[4].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[4].key, "string"), "Expected \"string\" but got \"%s\"", node->subitems[1].subitems[4].key);
	TestCase_assert(node->subitems[1].subitems[4].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[4].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[4].value.string, "10"), "Expected \"10\" but got \"%s\"", node->subitems[1].subitems[4].value.string);
	TestCase_assert(node->subitems[1].subitems[5].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[1].subitems[5].type);
	TestCase_assert(node->subitems[1].subitems[5].keyLength == 11, "Expected 11 but got " SIZE_T_FORMAT, node->subitems[1].subitems[5].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[5].key, "string_null"), "Expected \"string_null\" but got \"%s\"", node->subitems[1].subitems[5].key);
	TestCase_assert(node->subitems[1].subitems[6].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[1].subitems[6].type);
	TestCase_assert(node->subitems[1].subitems[6].keyLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[1].subitems[6].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[6].key, "boolean"), "Expected \"boolean\" but got \"%s\"", node->subitems[1].subitems[6].key);
	TestCase_assert(node->subitems[1].subitems[6].value.boolean == true, "Expected true but got false");
	TestCase_assert(node->subitems[1].subitems[7].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[7].type);
	TestCase_assert(node->subitems[1].subitems[7].keyLength == 11, "Expected 11 but got " SIZE_T_FORMAT, node->subitems[1].subitems[7].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[7].key, "enumeration"), "Expected \"enumeration\" but got \"%s\"", node->subitems[1].subitems[7].key);
	TestCase_assert(node->subitems[1].subitems[7].stringLength == 4, "Expected 4 but got " SIZE_T_FORMAT, node->subitems[1].subitems[7].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[7].value.string, "enum"), "Expected \"enum\" but got \"%s\"", node->subitems[1].subitems[7].value.string);
	TestCase_assert(node->subitems[1].subitems[8].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[8].type);
	TestCase_assert(node->subitems[1].subitems[8].keyLength == 9, "Expected 9 but got " SIZE_T_FORMAT, node->subitems[1].subitems[8].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[8].key, "bitfield8"), "Expected \"bitfield8\" but got \"%s\"", node->subitems[1].subitems[8].key);
	TestCase_assert(node->subitems[1].subitems[8].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[8].value.count);
	TestCase_assert(node->subitems[1].subitems[8].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[8].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[8].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[8].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[8].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[8].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[8].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[8].subitems[0].value.string, "13"), "Expected \"13\" but got \"%s\"", node->subitems[1].subitems[8].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[9].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[9].type);
	TestCase_assert(node->subitems[1].subitems[9].keyLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[1].subitems[9].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[9].key, "bitfield16"), "Expected \"bitfield16\" but got \"%s\"", node->subitems[1].subitems[9].key);
	TestCase_assert(node->subitems[1].subitems[9].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[9].value.count);
	TestCase_assert(node->subitems[1].subitems[9].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[9].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[9].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[9].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[9].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[9].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[9].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[9].subitems[0].value.string, "14"), "Expected \"14\" but got \"%s\"", node->subitems[1].subitems[9].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[10].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[10].type);
	TestCase_assert(node->subitems[1].subitems[10].keyLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[1].subitems[10].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[10].key, "bitfield32"), "Expected \"bitfield32\" but got \"%s\"", node->subitems[1].subitems[10].key);
	TestCase_assert(node->subitems[1].subitems[10].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[10].value.count);
	TestCase_assert(node->subitems[1].subitems[10].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[10].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[10].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[10].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[10].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[10].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[10].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[10].subitems[0].value.string, "15"), "Expected \"15\" but got \"%s\"", node->subitems[1].subitems[10].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[11].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[11].type);
	TestCase_assert(node->subitems[1].subitems[11].keyLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[1].subitems[11].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[11].key, "bitfield64"), "Expected \"bitfield64\" but got \"%s\"", node->subitems[1].subitems[11].key);
	TestCase_assert(node->subitems[1].subitems[11].value.count == 1, "Expected 1 but got " SIZE_T_FORMAT, node->subitems[1].subitems[11].value.count);
	TestCase_assert(node->subitems[1].subitems[11].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[11].subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[11].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[11].subitems[0].key == NULL, "Expected NULL but got \"%s\"", node->subitems[1].subitems[11].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[11].subitems[0].stringLength == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].subitems[11].subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[11].subitems[0].value.string, "16"), "Expected \"16\" but got \"%s\"", node->subitems[1].subitems[11].subitems[0].value.string);
	TestCase_assert(node->subitems[1].subitems[12].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[1].subitems[12].type);
	TestCase_assert(node->subitems[1].subitems[12].keyLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[1].subitems[12].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[12].key, "fixed16_16"), "Expected \"string\" but got \"%s\"", node->subitems[1].subitems[12].key);
	TestCase_assert(node->subitems[1].subitems[12].stringLength == 7, "Expected 7 but got " SIZE_T_FORMAT, node->subitems[1].subitems[12].stringLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[12].value.string, "0xA0000"), "Expected \"0xA0000\" but got \"%s\"", node->subitems[1].subitems[12].value.string);
	JSONNode_dispose(node);
}

static void testMixedContainers() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "outerArray");
		context->beginStructure(context, "outerStruct");
			context->beginArray(context, "innerArray");
			context->endArray(context);
			context->beginDictionary(context, "innerDict");
			context->endDictionary(context);
		context->endStructure(context);
		context->beginDictionary(context, "outerDict");
			context->beginArray(context, "innerArray2");
			context->endArray(context);
			context->beginStructure(context, "innerStruct");
			context->endStructure(context);
		context->endDictionary(context);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got " SIZE_T_FORMAT, node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].subitems[0].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[0].subitems[0].type);
	TestCase_assert(node->subitems[0].subitems[0].keyLength == 10, "Expected 10 but got " SIZE_T_FORMAT, node->subitems[0].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].subitems[0].key, "innerArray"), "Expected \"innerArray\" but got \"%s\"", node->subitems[0].subitems[0].key);
	TestCase_assert(node->subitems[0].subitems[0].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[0].subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems[0].subitems == NULL, "Expected NULL but got %p", node->subitems[0].subitems[0].subitems);
	TestCase_assert(node->subitems[0].subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].subitems[1].type);
	TestCase_assert(node->subitems[0].subitems[1].keyLength == 9, "Expected 9 but got " SIZE_T_FORMAT, node->subitems[0].subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].subitems[1].key, "innerDict"), "Expected \"innerDict\" but got \"%s\"", node->subitems[0].subitems[1].key);
	TestCase_assert(node->subitems[0].subitems[1].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[0].subitems[1].value.count);
	TestCase_assert(node->subitems[0].subitems[1].subitems == NULL, "Expected NULL but got %p", node->subitems[0].subitems[1].subitems);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 2, "Expected 2 but got " SIZE_T_FORMAT, node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[0].keyLength == 11, "Expected 11 but got " SIZE_T_FORMAT, node->subitems[1].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[0].key, "innerArray2"), "Expected \"innerArray2\" but got \"%s\"", node->subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[0].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[1].subitems[0].value.count);
	TestCase_assert(node->subitems[1].subitems[0].subitems == NULL, "Expected NULL but got %p", node->subitems[1].subitems[0].subitems);
	TestCase_assert(node->subitems[1].subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].subitems[1].type);
	TestCase_assert(node->subitems[1].subitems[1].keyLength == 11, "Expected 11 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[1].key, "innerStruct"), "Expected \"innerStruct\" but got \"%s\"", node->subitems[1].subitems[1].key);
	TestCase_assert(node->subitems[1].subitems[1].value.count == 0, "Expected 0 but got " SIZE_T_FORMAT, node->subitems[1].subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems[1].subitems == NULL, "Expected NULL but got %p", node->subitems[1].subitems[1].subitems);
	JSONNode_dispose(node);
}

static void testWriteToString() {
	JSONSerializationContext * context;
	char * string;
	size_t length;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt32(context, "item", 10);
	context->beginStructure(context, "item");
	context->writeBoolean(context, "bool", false);
	context->writeString(context, "string", "Hello, world!");
	context->endStructure(context);
	context->endArray(context);
	
	length = 0;
	string = JSONSerializationContext_writeToString(context, JSONEmitterFormat_compact, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(string, "[10,{\"bool\":false,\"string\":\"Hello, world!\"}]"), "Expected \"[10,{\"bool\":false,\"string\":\"Hello, world!\"}]\" but got \"%s\"", string);
	TestCase_assert(length == 44, "Expected 44 but got " SIZE_T_FORMAT, length);
	free(string);
	
	length = 0;
	string = JSONSerializationContext_writeToString(context, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(string, "[10, {\"bool\": false, \"string\": \"Hello, world!\"}]"), "Expected \"[10, {\"bool\": false, \"string\": \"Hello, world!\"}]\" but got \"%s\"", string);
	TestCase_assert(length == 48, "Expected 48 but got " SIZE_T_FORMAT, length);
	free(string);
	
	length = 0;
	string = JSONSerializationContext_writeToString(context, JSONEmitterFormat_multiLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(string, "[\n\t10,\n\t{\n\t\t\"bool\": false,\n\t\t\"string\": \"Hello, world!\"\n\t}\n]"), "Expected \"[\n\t10,\n\t{\n\t\t\"bool\": false,\n\t\t\"string\": \"Hello, world!\"\n\t}\n]\" but got \"%s\"", string);
	TestCase_assert(length == 59, "Expected 59 but got " SIZE_T_FORMAT, length);
	free(string);
	
	context->dispose(context);
}

static void testWriteToFile() {
	JSONSerializationContext * context;
	const char * tempFilePath;
	int fd;
	bool success;
	char * string;
	size_t length;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt32(context, "item", 10);
	context->beginStructure(context, "item");
	context->writeBoolean(context, "bool", false);
	context->writeString(context, "string", "Hello, world!");
	context->endStructure(context);
	context->endArray(context);
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	success = JSONSerializationContext_writeToFile(context, JSONEmitterFormat_compact, tempFilePath, NULL);
	string = readFileSimple(tempFilePath, &length);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(length == 44, "Expected 44 but got " SIZE_T_FORMAT, length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strncmp(string, "[10,{\"bool\":false,\"string\":\"Hello, world!\"}]", length), "Expected \"[10,{\"bool\":false,\"string\":\"Hello, world!\"}]\" but got \"%s\"", string);
	free(string);
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	success = JSONSerializationContext_writeToFile(context, JSONEmitterFormat_singleLine, tempFilePath, NULL);
	string = readFileSimple(tempFilePath, &length);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(length == 48, "Expected 48 but got " SIZE_T_FORMAT, length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strncmp(string, "[10, {\"bool\": false, \"string\": \"Hello, world!\"}]", length), "Expected \"[10, {\"bool\": false, \"string\": \"Hello, world!\"}]\" but got \"%s\"", string);
	free(string);
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	success = JSONSerializationContext_writeToFile(context, JSONEmitterFormat_multiLine, tempFilePath, NULL);
	string = readFileSimple(tempFilePath, &length);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(length == 59, "Expected 59 but got " SIZE_T_FORMAT, length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strncmp(string, "[\n\t10,\n\t{\n\t\t\"bool\": false,\n\t\t\"string\": \"Hello, world!\"\n\t}\n]", length), "Expected \"[\n\t10,\n\t{\n\t\t\"bool\": false,\n\t\t\"string\": \"Hello, world!\"\n\t}\n]\" but got \"%s\"", string);
	free(string);
	
	context->dispose(context);
}

static void testInvalidOperations() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	
	// No data
	context = JSONSerializationContext_create();
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// No top-level container
#define _testNoTopLevelContainer(FUNCTION, ...) \
	context = JSONSerializationContext_create(); \
	context->FUNCTION(context, "key", __VA_ARGS__); \
	node = JSONSerializationContext_writeToJSONNode(context); \
	TestCase_assert(node == NULL, "Expected NULL but got %p", node); \
	context->dispose(context);
	
	_testNoTopLevelContainer(writeInt8, 0)
	_testNoTopLevelContainer(writeUInt8, 0)
	_testNoTopLevelContainer(writeInt16, 0)
	_testNoTopLevelContainer(writeUInt16, 0)
	_testNoTopLevelContainer(writeInt32, 0)
	_testNoTopLevelContainer(writeUInt32, 0)
	_testNoTopLevelContainer(writeInt64, 0)
	_testNoTopLevelContainer(writeUInt64, 0)
	_testNoTopLevelContainer(writeFloat, 0)
	_testNoTopLevelContainer(writeDouble, 0)
	_testNoTopLevelContainer(writeFixed16_16, 0)
	_testNoTopLevelContainer(writeString, "")
	_testNoTopLevelContainer(writeStringNullable, NULL)
	_testNoTopLevelContainer(writeBoolean, false)
	_testNoTopLevelContainer(writeEnumeration, 0, NULL)
	_testNoTopLevelContainer(writeBitfield8, 0, NULL)
	_testNoTopLevelContainer(writeBitfield16, 0, NULL)
	_testNoTopLevelContainer(writeBitfield32, 0, NULL)
	_testNoTopLevelContainer(writeBitfield64, 0, NULL)
#undef _testNoTopLevelContainer
	
	// Unterminated array
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->beginArray(context, "key");
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// Unterminated structure
	context = JSONSerializationContext_create();
	context->beginStructure(context, "key");
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginStructure(context, "key");
	context->beginStructure(context, "key");
	context->endStructure(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// Unterminated dictionary
	context = JSONSerializationContext_create();
	context->beginDictionary(context, "key");
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginDictionary(context, "key");
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// Wrong container type terminated
#define _testWrongContainerTypeTerminatedOuter(CONTAINER_1, CONTAINER_2) \
	context = JSONSerializationContext_create(); \
	context->begin##CONTAINER_1(context, "key"); \
	context->end##CONTAINER_2(context); \
	node = JSONSerializationContext_writeToJSONNode(context); \
	TestCase_assert(node == NULL, "Expected NULL but got %p", node); \
	context->dispose(context);
#define _testWrongContainerTypeTerminatedInner(CONTAINER_1, CONTAINER_2) \
	context = JSONSerializationContext_create(); \
	context->begin##CONTAINER_1(context, "key"); \
	context->begin##CONTAINER_1(context, "key"); \
	context->end##CONTAINER_2(context); \
	context->end##CONTAINER_1(context); \
	node = JSONSerializationContext_writeToJSONNode(context); \
	TestCase_assert(node == NULL, "Expected NULL but got %p", node); \
	context->dispose(context);
	
	_testWrongContainerTypeTerminatedOuter(Array, Structure)
	_testWrongContainerTypeTerminatedOuter(Array, Dictionary)
	_testWrongContainerTypeTerminatedOuter(Structure, Array)
	_testWrongContainerTypeTerminatedOuter(Structure, Dictionary)
	_testWrongContainerTypeTerminatedOuter(Dictionary, Array)
	_testWrongContainerTypeTerminatedOuter(Dictionary, Structure)
	_testWrongContainerTypeTerminatedInner(Array, Structure)
	_testWrongContainerTypeTerminatedInner(Array, Dictionary)
	_testWrongContainerTypeTerminatedInner(Structure, Array)
	_testWrongContainerTypeTerminatedInner(Structure, Dictionary)
	_testWrongContainerTypeTerminatedInner(Dictionary, Array)
	_testWrongContainerTypeTerminatedInner(Dictionary, Structure)
#undef _testWrongContainerTypeTerminatedOuter
#undef _testWrongContainerTypeTerminatedInner
	
	// Container underflow
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->endArray(context);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginStructure(context, "key");
	context->endStructure(context);
	context->endStructure(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	context->endDictionary(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// Second top-level container started
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->endArray(context);
	context->beginArray(context, "key");
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginStructure(context, "key");
	context->endStructure(context);
	context->beginStructure(context, "key");
	context->endStructure(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// Bit set in bitfield but list terminated before naming bit
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeBitfield8(context, "item", 0x01, NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeBitfield16(context, "item", 0x0002, "bit0", NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeBitfield32(context, "item", 0x00000004, "bit0", "bit1", NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeBitfield64(context, "item", 0x0000000000000008ull, "bit0", "bit1", "bit2", NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// Bit names duplicated
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeBitfield8(context, "item", 0x00, "bit0", "bit0", NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeBitfield16(context, "item", 0x0000, "bit0", "bit0", NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeBitfield32(context, "item", 0x00000000, "bit0", "bit0", NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeBitfield64(context, "item", 0x0000000000000000ull, "bit0", "bit0", NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// Enum value not listed in args
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum", 1, NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// Duplicate enum values
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum0", 0, "enum1", 0, NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum0", 0, "enum1", 1, "enum2", 1, NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	// Duplicate enum names
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum", 0, "enum", 1, NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum0", 0, "enum1", 1, "enum1", 2, NULL);
	context->endArray(context);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	context->dispose(context);
	
#define _testNullObjectKey(CONTAINER, FUNCTION, ...) \
	context = JSONSerializationContext_create(); \
	context->begin##CONTAINER(context, "key"); \
	context->FUNCTION(context, NULL, __VA_ARGS__); \
	context->end##CONTAINER(context); \
	node = JSONSerializationContext_writeToJSONNode(context); \
	TestCase_assert(node == NULL, "Expected NULL but got %p", node); \
	context->dispose(context);
	
#define _testNullObjectKeyNoArgs(CONTAINER, FUNCTION, ...) \
	context = JSONSerializationContext_create(); \
	context->begin##CONTAINER(context, "key"); \
	context->FUNCTION(context, NULL); \
	context->end##CONTAINER(context); \
	node = JSONSerializationContext_writeToJSONNode(context); \
	TestCase_assert(node == NULL, "Expected NULL but got %p", node); \
	context->dispose(context);
	
	// Null structure key
	_testNullObjectKeyNoArgs(Structure, beginArray)
	_testNullObjectKeyNoArgs(Structure, beginStructure)
	_testNullObjectKeyNoArgs(Structure, beginDictionary)
	_testNullObjectKey(Structure, writeInt8, 0)
	_testNullObjectKey(Structure, writeUInt8, 0)
	_testNullObjectKey(Structure, writeInt16, 0)
	_testNullObjectKey(Structure, writeUInt16, 0)
	_testNullObjectKey(Structure, writeInt32, 0)
	_testNullObjectKey(Structure, writeUInt32, 0)
	_testNullObjectKey(Structure, writeInt64, 0)
	_testNullObjectKey(Structure, writeUInt64, 0)
	_testNullObjectKey(Structure, writeFloat, 0)
	_testNullObjectKey(Structure, writeDouble, 0)
	_testNullObjectKey(Structure, writeFixed16_16, 0)
	_testNullObjectKey(Structure, writeString, "")
	_testNullObjectKey(Structure, writeStringNullable, NULL)
	_testNullObjectKey(Structure, writeBoolean, false)
	_testNullObjectKey(Structure, writeEnumeration, 0, "enum", 0, NULL)
	_testNullObjectKey(Structure, writeBitfield8, 0, NULL)
	_testNullObjectKey(Structure, writeBitfield16, 0, NULL)
	_testNullObjectKey(Structure, writeBitfield32, 0, NULL)
	_testNullObjectKey(Structure, writeBitfield64, 0, NULL)
	
	// Null dictionary key
	_testNullObjectKeyNoArgs(Dictionary, beginArray)
	_testNullObjectKeyNoArgs(Dictionary, beginStructure)
	_testNullObjectKeyNoArgs(Dictionary, beginDictionary)
	_testNullObjectKey(Dictionary, writeInt8, 0)
	_testNullObjectKey(Dictionary, writeUInt8, 0)
	_testNullObjectKey(Dictionary, writeInt16, 0)
	_testNullObjectKey(Dictionary, writeUInt16, 0)
	_testNullObjectKey(Dictionary, writeInt32, 0)
	_testNullObjectKey(Dictionary, writeUInt32, 0)
	_testNullObjectKey(Dictionary, writeInt64, 0)
	_testNullObjectKey(Dictionary, writeUInt64, 0)
	_testNullObjectKey(Dictionary, writeFloat, 0)
	_testNullObjectKey(Dictionary, writeDouble, 0)
	_testNullObjectKey(Dictionary, writeFixed16_16, 0)
	_testNullObjectKey(Dictionary, writeString, "")
	_testNullObjectKey(Dictionary, writeStringNullable, NULL)
	_testNullObjectKey(Dictionary, writeBoolean, false)
	_testNullObjectKey(Dictionary, writeEnumeration, 0, "enum", 0, NULL)
	_testNullObjectKey(Dictionary, writeBitfield8, 0, NULL)
	_testNullObjectKey(Dictionary, writeBitfield16, 0, NULL)
	_testNullObjectKey(Dictionary, writeBitfield32, 0, NULL)
	_testNullObjectKey(Dictionary, writeBitfield64, 0, NULL)
#undef _testNullObjectKey
#undef _testNullObjectKeyNoArgs
}

static void testErrorReporting() {
	JSONSerializationContext * context;
	struct JSONNode * node;
	const char * tempFilePath;
	int fd;
	bool success;
	char * string;
	jmp_buf jmpBuf;
	int status;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	node = JSONSerializationContext_writeToJSONNode(context);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(context->status == SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, "Expected %d but got %d", SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, context->status);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	string = JSONSerializationContext_writeToString(context, JSONEmitterFormat_compact, NULL, NULL);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	TestCase_assert(context->status == SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, "Expected %d but got %d", SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, context->status);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	success = JSONSerializationContext_writeToFile(context, JSONEmitterFormat_compact, tempFilePath, NULL);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(!success, "Expected false but got true");
	TestCase_assert(context->status == SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, "Expected %d but got %d", SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, context->status);
	context->dispose(context);
	
#define _testFailure(ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	context = JSONSerializationContext_create(); \
	PREAMBLE_CODE \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d (OK)", SERIALIZATION_ERROR_OK, context->status); \
	FAIL_CODE \
	TestCase_assert(context->status == ERROR_STATUS, "Expected %d but got %d (context->status)", ERROR_STATUS, context->status); \
	context->dispose(context); \
	\
	context = JSONSerializationContext_create(); \
	context->jmpBuf = &jmpBuf; \
	status = setjmp(jmpBuf); \
	if (!status) { \
		PREAMBLE_CODE \
		TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d (OK setjmp)", SERIALIZATION_ERROR_OK, context->status); \
		FAIL_CODE \
	} \
	TestCase_assert(status == ERROR_STATUS, "Expected %d but got %d (status setjmp)", ERROR_STATUS, status); \
	TestCase_assert(context->status == ERROR_STATUS, "Expected %d but got %d (context->status setjmp)", ERROR_STATUS, context->status); \
	context->dispose(context);
	
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeInt8(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeUInt8(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeInt16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeUInt16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeInt32(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeUInt32(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeInt64(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeUInt64(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeFloat(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeDouble(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeFixed16_16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeString(context, "key", "");)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeStringNullable(context, "key", NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBoolean(context, "key", false);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeEnumeration(context, "key", 0, "", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBitfield8(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBitfield16(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBitfield32(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBitfield64(context, "key", 0, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeInt8(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeUInt8(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeInt16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeUInt16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeInt32(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeUInt32(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeInt64(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeUInt64(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeFloat(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeDouble(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeFixed16_16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeString(context, "key", "");)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeStringNullable(context, "key", NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBoolean(context, "key", false);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeEnumeration(context, "key", 0, "", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBitfield8(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBitfield16(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBitfield32(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBitfield64(context, "key", 0, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "key");,
	             context->endStructure(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "key");
	             context->beginArray(context, "key");,
	             context->endStructure(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "key");,
	             context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "key");
	             context->beginArray(context, "key");,
	             context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "key");,
	             context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "key");
	             context->beginStructure(context, "key");,
	             context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "key");,
	             context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "key");
	             context->beginStructure(context, "key");,
	             context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "key");,
	             context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "key");
	             context->beginDictionary(context, "key");,
	             context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "key");,
	             context->endStructure(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "key");
	             context->beginDictionary(context, "key");,
	             context->endStructure(context);)
	
	_testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, , context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, , context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, , context->endStructure(context);)
	
	_testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginArray(context, "key");
	             context->endArray(context);,
	             context->beginArray(context, "key");)
	_testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginStructure(context, "key");
	             context->endStructure(context);,
	             context->beginStructure(context, "key");)
	_testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginDictionary(context, "key");
	             context->endDictionary(context);,
	             context->beginDictionary(context, "key");)
	
	_testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield8(context, "key", 0x08, "bit0", "bit1", "bit2", NULL);)
	_testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield16(context, "key", 0x0004, "bit0", "bit1", NULL);)
	_testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield32(context, "key", 0x00000002, "bit0", NULL);)
	_testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield64(context, "key", 0x0000000000000001ull, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield8(context, "key", 0, "bit0", "bit0", "bit1", NULL);)
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield16(context, "key", 0, "bit0", "bit1", "bit0", NULL);)
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield32(context, "key", 0, "bit0", "bit1", "bit1", NULL);)
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield64(context, "key", 0, "bit0", "bit0", NULL);)
	
	_testFailure(SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	             context->beginArray(context, "key");,
	             context->writeEnumeration(context, "key", 0, "", 1, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE,
	             context->beginArray(context, "key");,
	             context->writeEnumeration(context, "key", 0, "enum0", 0, "enum1", 0, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME,
	             context->beginArray(context, "key");,
	             context->writeEnumeration(context, "key", 0, "enum0", 0, "enum0", 1, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeInt8(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeUInt8(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeInt16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeUInt16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeInt32(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeUInt32(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeInt64(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeUInt64(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeFloat(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeDouble(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeFixed16_16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeString(context, NULL, "");)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeStringNullable(context, NULL, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBoolean(context, NULL, false);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeEnumeration(context, NULL, 0, "", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBitfield8(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBitfield16(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBitfield32(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBitfield64(context, NULL, 0, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeInt8(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeUInt8(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeInt16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeUInt16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeInt32(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeUInt32(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeInt64(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeUInt64(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeFloat(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeDouble(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeFixed16_16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeString(context, NULL, "");)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeStringNullable(context, NULL, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBoolean(context, NULL, false);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeEnumeration(context, NULL, 0, "", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBitfield8(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBitfield16(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBitfield32(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBitfield64(context, NULL, 0, NULL);)
	
#define _testFailureDuplicateStructureKey(FAIL_WRITE_CALL) \
	_testFailure(JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY, \
	             context->beginStructure(context, "key"); \
	             context->writeBoolean(context, "key", false);, \
	             context->FAIL_WRITE_CALL;) \
	_testFailure(JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY, \
	             context->beginStructure(context, "key"); \
	             context->writeBoolean(context, "key", false); \
	             context->writeBoolean(context, "key2", true);, \
	             context->FAIL_WRITE_CALL;) \
	_testFailure(JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY, \
	             context->beginStructure(context, "key"); \
	             context->writeBoolean(context, "key2", false); \
	             context->writeBoolean(context, "key", true);, \
	             context->FAIL_WRITE_CALL;)
	
	_testFailureDuplicateStructureKey(writeInt8(context, "key", 0))
	_testFailureDuplicateStructureKey(writeUInt8(context, "key", 0))
	_testFailureDuplicateStructureKey(writeInt16(context, "key", 0))
	_testFailureDuplicateStructureKey(writeUInt16(context, "key", 0))
	_testFailureDuplicateStructureKey(writeInt32(context, "key", 0))
	_testFailureDuplicateStructureKey(writeUInt32(context, "key", 0))
	_testFailureDuplicateStructureKey(writeInt64(context, "key", 0))
	_testFailureDuplicateStructureKey(writeUInt64(context, "key", 0))
	_testFailureDuplicateStructureKey(writeFloat(context, "key", 0))
	_testFailureDuplicateStructureKey(writeDouble(context, "key", 0))
	_testFailureDuplicateStructureKey(writeFixed16_16(context, "key", 0))
	_testFailureDuplicateStructureKey(writeString(context, "key", ""))
	_testFailureDuplicateStructureKey(writeStringNullable(context, "key", NULL))
	_testFailureDuplicateStructureKey(writeBoolean(context, "key", false))
	_testFailureDuplicateStructureKey(writeEnumeration(context, "key", 0, "", 0, NULL))
	_testFailureDuplicateStructureKey(writeBitfield8(context, "key", 0, NULL))
	_testFailureDuplicateStructureKey(writeBitfield16(context, "key", 0, NULL))
	_testFailureDuplicateStructureKey(writeBitfield32(context, "key", 0, NULL))
	_testFailureDuplicateStructureKey(writeBitfield64(context, "key", 0, NULL))
#undef _testFailureDuplicateStructureKey
	
#undef _testFailure
}

static void testThatNodeIsCopiedWhenRequestedMultipleTimes() {
	JSONSerializationContext * context;
	struct JSONNode * node1, * node2;
	char * string1, * string2;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
		context->beginStructure(context, "item");
			context->writeInt8(context, "item", 0);
		context->endStructure(context);
		context->beginArray(context, "item");
			context->writeUInt8(context, "item", 1);
			context->beginDictionary(context, "item");
				context->writeInt16(context, "item", 2);
				context->writeUInt16(context, "item", 3);
				context->writeInt32(context, "item", 4);
				context->writeUInt32(context, "item", 5);
				context->writeInt64(context, "item", 6);
				context->writeUInt64(context, "item", 7);
			context->endDictionary(context);
			context->writeFloat(context, "item", 8);
			context->writeDouble(context, "item", 9);
			context->writeString(context, "item", "10");
			context->writeBoolean(context, "item", true);
			context->writeEnumeration(context, "item", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "item", 1, "13", NULL);
			context->writeBitfield16(context, "item", 1, "14", NULL);
			context->writeBitfield32(context, "item", 1, "15", NULL);
			context->writeBitfield64(context, "item", 1, "16", NULL);
			context->writeFixed16_16(context, "item", 0xA0000);
		context->endArray(context);
	context->endArray(context);
	node1 = JSONSerializationContext_writeToJSONNode(context);
	node2 = JSONSerializationContext_writeToJSONNode(context);
	context->dispose(context);
	TestCase_assert(node1 != NULL, "Expected non-NULL but got NULL");
	if (node1 == NULL) {return;} // Suppress clang warning
	TestCase_assert(node2 != NULL, "Expected non-NULL but got NULL");
	if (node2 == NULL) {return;} // Suppress clang warning
	TestCase_assert(node1 != node2, "Pointers expected to differ, but didn't");
	string1 = JSONEmitter_writeString(node1, JSONEmitterFormat_compact, NULL, NULL);
	TestCase_assert(string1 != NULL, "Expected non-NULL but got NULL");
	string2 = JSONEmitter_writeString(node2, JSONEmitterFormat_compact, NULL, NULL);
	TestCase_assert(string2 != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(string1, string2), "Strings didn't match:\n  %s\n  %s", string1, string2);
	free(string1);
	free(string2);
	TestCase_assert(node1->subitems != node2->subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[0].subitems != node2->subitems[0].subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[0].subitems[0].key != node2->subitems[0].subitems[0].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems != node2->subitems[1].subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems != node2->subitems[1].subitems[1].subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[0].key != node2->subitems[1].subitems[1].subitems[0].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[1].key != node2->subitems[1].subitems[1].subitems[1].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[2].key != node2->subitems[1].subitems[1].subitems[2].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[3].key != node2->subitems[1].subitems[1].subitems[3].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[4].key != node2->subitems[1].subitems[1].subitems[4].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[5].key != node2->subitems[1].subitems[1].subitems[5].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[4].value.string != node2->subitems[1].subitems[4].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[6].value.string != node2->subitems[1].subitems[6].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[7].subitems[0].value.string != node2->subitems[1].subitems[7].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[8].subitems[0].value.string != node2->subitems[1].subitems[8].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[9].subitems[0].value.string != node2->subitems[1].subitems[9].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[10].subitems[0].value.string != node2->subitems[1].subitems[10].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[11].value.string != node2->subitems[1].subitems[11].value.string, "Pointers expected to differ, but didn't");
	JSONNode_dispose(node1);
	JSONNode_dispose(node2);
}

static void testDuplicateKeysAllowedInDictionariesAfterWritingStructures() {
	JSONSerializationContext * context;
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "");
		context->beginStructure(context, "a");
		context->endStructure(context);
		context->beginStructure(context, "a");
		context->endStructure(context);
	context->endDictionary(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	context->dispose(context);
	
	context = JSONSerializationContext_create();
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "");
		context->beginDictionary(context, "");
			context->beginStructure(context, "a");
			context->endStructure(context);
			context->beginStructure(context, "a");
			context->endStructure(context);
		context->endDictionary(context);
	context->endDictionary(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	context->dispose(context);
}

TEST_SUITE(JSONSerializationContextTest,
           testInit,
           testTopLevelContainers,
           testNumberValues,
           testLargeIntegerRepresentability,
           testStringValues,
           testBlobValues,
           testBooleanValues,
           testEnumerations,
           testBitfields,
           testArrays,
           testStructures,
           testDictionaries,
           testMixedContainers,
           testWriteToString,
           testWriteToFile,
           testInvalidOperations,
           testErrorReporting,
           testThatNodeIsCopiedWhenRequestedMultipleTimes,
           testDuplicateKeysAllowedInDictionariesAfterWritingStructures)
