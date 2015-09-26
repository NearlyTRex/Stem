#include "dynamictypes/AssociativeArray.h"
#include "dynamictypes/DataArray.h"
#include "dynamictypes/DataDeserializationContext.h"
#include "dynamictypes/HashTable.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"
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
	verifyFunctionPointer(context, readBoolean); \
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
	verifyFunctionPointer(context, readFixed16_16); \
	verifyFunctionPointer(context, readEnumeration); \
	verifyFunctionPointer(context, readBitfield8); \
	verifyFunctionPointer(context, readBitfield16); \
	verifyFunctionPointer(context, readBitfield32); \
	verifyFunctionPointer(context, readBitfield64); \
	verifyFunctionPointer(context, readString); \
	verifyFunctionPointer(context, readBlob); \
	verifyFunctionPointer(context, readNextDictionaryKey); \
	verifyFunctionPointer(context, hasDictionaryKey)

static void testInit() {
	DataDeserializationContext context, * contextPtr;
	
	memset(&context, 0x00, sizeof(context));
	DataDeserializationContext_init(&context, valueCreateBoolean(false));
	TestCase_assert(context.status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context.status);
	verifyFunctionPointers(&context);
	DataDeserializationContext_dispose(&context);
	
	memset(&context, 0xFF, sizeof(context));
	DataDeserializationContext_init(&context, valueCreateBoolean(false));
	TestCase_assert(context.status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context.status);
	verifyFunctionPointers(&context);
	DataDeserializationContext_dispose(&context);
	
	contextPtr = DataDeserializationContext_create(valueCreateBoolean(false));
	TestCase_assert(contextPtr->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, contextPtr->status);
	verifyFunctionPointers(contextPtr);
	DataDeserializationContext_dispose(contextPtr);
}

#define beginAndVerifyContainer(CONTAINER_TYPE, KEY, EXPECTED_SIZE) { \
	size_t size; \
	size = context->begin##CONTAINER_TYPE(context, KEY); \
	TestCase_assert(size == (EXPECTED_SIZE), "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT " (status %d)", (size_t) (EXPECTED_SIZE), size, context->status); \
}
#define beginAndVerifyArray(KEY, EXPECTED_SIZE) beginAndVerifyContainer(Array, KEY, EXPECTED_SIZE)
#define beginAndVerifyDictionary(KEY, EXPECTED_SIZE) beginAndVerifyContainer(Dictionary, KEY, EXPECTED_SIZE)

#define readFunction_int8_t readInt8
#define readFunction_uint8_t readUInt8
#define readFunction_int16_t readInt16
#define readFunction_uint16_t readUInt16
#define readFunction_int32_t readInt32
#define readFunction_uint32_t readUInt32
#define readFunction_int64_t readInt64
#define readFunction_uint64_t readUInt64
#define readFunction_float readFloat
#define readFunction_double readDouble
#define readFunction_fixed16_16 readFixed16_16
#define readFunction_bool readBoolean
#define printfSpecifier_int8_t "%d"
#define printfSpecifier_uint8_t "%u"
#define printfSpecifier_int16_t "%d"
#define printfSpecifier_uint16_t "%u"
#define printfSpecifier_int32_t "%d"
#define printfSpecifier_uint32_t "%u"
#define printfSpecifier_int64_t INT64_FORMAT
#define printfSpecifier_uint64_t UINT64_FORMAT
#define printfSpecifier_float "%f"
#define printfSpecifier_double "%f"
#define printfSpecifier_fixed16_16 "0x%05X"
#define printfSpecifier_bool "%d"
#define readAndVerifyNumber(TYPE, KEY, EXPECTED_VALUE) { \
	TYPE value; \
	value = context->readFunction_##TYPE(context, KEY); \
	TestCase_assert(value == (EXPECTED_VALUE), "Expected " printfSpecifier_##TYPE " but got " printfSpecifier_##TYPE " (type " #TYPE "; status %d)", (TYPE) (EXPECTED_VALUE), value, context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define readAndVerifyEnumeration(KEY, EXPECTED_VALUE, ...) { \
	int value; \
	value = context->readEnumeration(context, KEY, __VA_ARGS__); \
	TestCase_assert(value == (EXPECTED_VALUE), "Expected %d but got %d (type enum; status %d)", (EXPECTED_VALUE), value, context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define printfSpecifier_bitfield8 "0x%02X"
#define printfSpecifier_bitfield16 "0x%04X"
#define printfSpecifier_bitfield32 "0x%08X"
#if defined(WIN32)
#define printfSpecifier_bitfield64 "0x%016I64X"
#elif defined(linux) && defined(_LP64)
#define printfSpecifier_bitfield64 "0x%016lX"
#else
#define printfSpecifier_bitfield64 "0x%016llX"
#endif
#define readAndVerifyBitfield(NBITS, KEY, EXPECTED_VALUE, ...) { \
	uint##NBITS##_t value; \
	value = context->readBitfield##NBITS(context, KEY, __VA_ARGS__); \
	TestCase_assert(value == (EXPECTED_VALUE), "Expected " printfSpecifier_bitfield##NBITS " but got " printfSpecifier_bitfield##NBITS " (type bitfield" #NBITS "; status %d)", (uint##NBITS##_t)(EXPECTED_VALUE), value, context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define readAndVerifyString(KEY, EXPECTED_VALUE) { \
	const char * value; \
	value = context->readString(context, KEY); \
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL"); \
	TestCase_assert(!strcmp(value, (EXPECTED_VALUE)), "Expected \"%s\" but got \"%s\" (type string; status %d)", (EXPECTED_VALUE), value, context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define readAndVerifyBlob(KEY, EXPECTED_VALUE, LENGTH) { \
	const void * value; \
	size_t length; \
	value = context->readBlob(context, KEY, &length); \
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL"); \
	TestCase_assert(length == LENGTH, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, (size_t) LENGTH, length); \
	TestCase_assert(!memcmp(value, (EXPECTED_VALUE), (LENGTH)), "Expected \"%s\" but got \"%s\" (type blob; status %d)", (char *) (EXPECTED_VALUE), (char *) value, context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define verifyReadNextDictionaryKey(KEY) \
	key = context->readNextDictionaryKey(context); \
	TestCase_assert(key != NULL, "Expected non-NULL but got NULL (status %d)", context->status); \
	TestCase_assert(!strcmp(key, (KEY)), "Expected \"%s\" but got \"%s\"", (KEY), key); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status);

#define verifyHasDictionaryKey(KEY, HAS_KEY) \
	TestCase_assert(context->hasDictionaryKey(context, (KEY)) == (HAS_KEY), "Expected %s but got %s (status %d)", (HAS_KEY) ? "true" : "false", (HAS_KEY) ? "false" : "true", context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status);

#define ENUM_TEST_0 0
#define ENUM_TEST_1 1

#define verifyTestValues() \
	readAndVerifyNumber(bool, "bool_0", false); \
	readAndVerifyNumber(bool, "bool_1", true); \
	readAndVerifyNumber(int8_t, "int8_0", 0); \
	readAndVerifyNumber(int8_t, "int8_1", 1); \
	readAndVerifyNumber(uint8_t, "uint8_0", 0); \
	readAndVerifyNumber(uint8_t, "uint8_1", 1); \
	readAndVerifyNumber(int16_t, "int16_0", 0); \
	readAndVerifyNumber(int16_t, "int16_1", 1); \
	readAndVerifyNumber(uint16_t, "uint16_0", 0); \
	readAndVerifyNumber(uint16_t, "uint16_1", 1); \
	readAndVerifyNumber(int32_t, "int32_0", 0); \
	readAndVerifyNumber(int32_t, "int32_1", 1); \
	readAndVerifyNumber(uint32_t, "uint32_0", 0); \
	readAndVerifyNumber(uint32_t, "uint32_1", 1); \
	readAndVerifyNumber(int64_t, "int64_0", 0); \
	readAndVerifyNumber(int64_t, "int64_1", 1); \
	readAndVerifyNumber(uint64_t, "uint64_0", 0); \
	readAndVerifyNumber(uint64_t, "uint64_1", 1); \
	readAndVerifyNumber(float, "float_0", 0.0f); \
	readAndVerifyNumber(float, "float_1", 1.0f); \
	readAndVerifyNumber(double, "double_0", 0.0); \
	readAndVerifyNumber(double, "double_1", 1.0); \
	readAndVerifyNumber(fixed16_16, "fixed_0", 0x00000); \
	readAndVerifyNumber(fixed16_16, "fixed_1", 0x10000); \
	readAndVerifyEnumeration("enum_0", ENUM_TEST_0, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL); \
	readAndVerifyEnumeration("enum_1", ENUM_TEST_1, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL); \
	readAndVerifyBitfield(8, "bitfield8_0", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7", NULL); \
	readAndVerifyBitfield(8, "bitfield8_1", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL); \
	readAndVerifyBitfield(16, "bitfield16_0", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", NULL); \
	readAndVerifyBitfield(16, "bitfield16_1", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL); \
	readAndVerifyBitfield(32, "bitfield32_0", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", NULL); \
	readAndVerifyBitfield(32, "bitfield32_1", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL); \
	readAndVerifyBitfield(64, "bitfield64_0", 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", NULL); \
	readAndVerifyBitfield(64, "bitfield64_1", 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL); \
	readAndVerifyString("string_0", "foo"); \
	readAndVerifyString("string_1", "bar"); \
	readAndVerifyBlob("blob_0", "a", 1); \
	readAndVerifyBlob("blob_1", "abcd", 4)

static void testValuesInArray() {
	DataDeserializationContext * context;
	DataArray * array;
	
	array = arrayCreate();
	arrayAppend(array, valueCreateBoolean(false));
	arrayAppend(array, valueCreateBoolean(true));
	arrayAppend(array, valueCreateInt8(0));
	arrayAppend(array, valueCreateInt8(1));
	arrayAppend(array, valueCreateUInt8(0));
	arrayAppend(array, valueCreateUInt8(1));
	arrayAppend(array, valueCreateInt16(0));
	arrayAppend(array, valueCreateInt16(1));
	arrayAppend(array, valueCreateUInt16(0));
	arrayAppend(array, valueCreateUInt16(1));
	arrayAppend(array, valueCreateInt32(0));
	arrayAppend(array, valueCreateInt32(1));
	arrayAppend(array, valueCreateUInt32(0));
	arrayAppend(array, valueCreateUInt32(1));
	arrayAppend(array, valueCreateInt64(0));
	arrayAppend(array, valueCreateInt64(1));
	arrayAppend(array, valueCreateUInt64(0));
	arrayAppend(array, valueCreateUInt64(1));
	arrayAppend(array, valueCreateFloat(0.0f));
	arrayAppend(array, valueCreateFloat(1.0f));
	arrayAppend(array, valueCreateDouble(0.0));
	arrayAppend(array, valueCreateDouble(1.0));
	arrayAppend(array, valueCreateFixed16_16(0x00000));
	arrayAppend(array, valueCreateFixed16_16(0x10000));
	arrayAppend(array, valueCreateInt32(ENUM_TEST_0));
	arrayAppend(array, valueCreateInt32(ENUM_TEST_1));
	arrayAppend(array, valueCreateUInt8(0xAA));
	arrayAppend(array, valueCreateUInt8(0x57));
	arrayAppend(array, valueCreateUInt16(0xF001));
	arrayAppend(array, valueCreateUInt16(0x000F));
	arrayAppend(array, valueCreateUInt32(0xF0000001));
	arrayAppend(array, valueCreateUInt32(0x0000001E));
	arrayAppend(array, valueCreateUInt64(0xF000000000000001ull));
	arrayAppend(array, valueCreateUInt64(0x000000000000003Cull));
	arrayAppend(array, valueCreateString("foo", DATA_USE_STRLEN, false, false));
	arrayAppend(array, valueCreateString("bar", DATA_USE_STRLEN, false, false));
	arrayAppend(array, valueCreateBlob("a", 1, false, false));
	arrayAppend(array, valueCreateBlob("abcd", 4, false, false));
	
	context = DataDeserializationContext_create(valueCreateArray(array, false, false));
	beginAndVerifyArray(NULL, 38);
	verifyTestValues();
	context->endArray(context);
	
	arrayDispose(array);
	DataDeserializationContext_dispose(context);
}

static void testValuesInHashTable() {
	DataDeserializationContext * context;
	HashTable * hashTable;
	
	hashTable = hashCreate();
	hashSet(hashTable, "bool_0", valueCreateBoolean(false));
	hashSet(hashTable, "bool_1", valueCreateBoolean(true));
	hashSet(hashTable, "int8_0", valueCreateInt8(0));
	hashSet(hashTable, "int8_1", valueCreateInt8(1));
	hashSet(hashTable, "uint8_0", valueCreateUInt8(0));
	hashSet(hashTable, "uint8_1", valueCreateUInt8(1));
	hashSet(hashTable, "int16_0", valueCreateInt16(0));
	hashSet(hashTable, "int16_1", valueCreateInt16(1));
	hashSet(hashTable, "uint16_0", valueCreateUInt16(0));
	hashSet(hashTable, "uint16_1", valueCreateUInt16(1));
	hashSet(hashTable, "int32_0", valueCreateInt32(0));
	hashSet(hashTable, "int32_1", valueCreateInt32(1));
	hashSet(hashTable, "uint32_0", valueCreateUInt32(0));
	hashSet(hashTable, "uint32_1", valueCreateUInt32(1));
	hashSet(hashTable, "int64_0", valueCreateInt64(0));
	hashSet(hashTable, "int64_1", valueCreateInt64(1));
	hashSet(hashTable, "uint64_0", valueCreateUInt64(0));
	hashSet(hashTable, "uint64_1", valueCreateUInt64(1));
	hashSet(hashTable, "float_0", valueCreateFloat(0.0f));
	hashSet(hashTable, "float_1", valueCreateFloat(1.0f));
	hashSet(hashTable, "double_0", valueCreateDouble(0.0));
	hashSet(hashTable, "double_1", valueCreateDouble(1.0));
	hashSet(hashTable, "fixed_0", valueCreateFixed16_16(0x00000));
	hashSet(hashTable, "fixed_1", valueCreateFixed16_16(0x10000));
	hashSet(hashTable, "enum_0", valueCreateInt32(ENUM_TEST_0));
	hashSet(hashTable, "enum_1", valueCreateInt32(ENUM_TEST_1));
	hashSet(hashTable, "bitfield8_0", valueCreateUInt8(0xAA));
	hashSet(hashTable, "bitfield8_1", valueCreateUInt8(0x57));
	hashSet(hashTable, "bitfield16_0", valueCreateUInt16(0xF001));
	hashSet(hashTable, "bitfield16_1", valueCreateUInt16(0x000F));
	hashSet(hashTable, "bitfield32_0", valueCreateUInt32(0xF0000001));
	hashSet(hashTable, "bitfield32_1", valueCreateUInt32(0x0000001E));
	hashSet(hashTable, "bitfield64_0", valueCreateUInt64(0xF000000000000001ull));
	hashSet(hashTable, "bitfield64_1", valueCreateUInt64(0x000000000000003Cull));
	hashSet(hashTable, "string_0", valueCreateString("foo", DATA_USE_STRLEN, false, false));
	hashSet(hashTable, "string_1", valueCreateString("bar", DATA_USE_STRLEN, false, false));
	hashSet(hashTable, "blob_0", valueCreateBlob("a", 1, false, false));
	hashSet(hashTable, "blob_1", valueCreateBlob("abcd", 4, false, false));
	
	context = DataDeserializationContext_create(valueCreateHashTable(hashTable, false, false));
	context->beginStructure(context, NULL);
	verifyTestValues();
	context->endArray(context);
	
	hashDispose(hashTable);
	DataDeserializationContext_dispose(context);
}

static void testValuesInAssociativeArray() {
	DataDeserializationContext * context;
	AssociativeArray * assArray;
	
	assArray = associativeArrayCreate();
	associativeArrayAppend(assArray, "bool_0", valueCreateBoolean(false));
	associativeArrayAppend(assArray, "bool_1", valueCreateBoolean(true));
	associativeArrayAppend(assArray, "int8_0", valueCreateInt8(0));
	associativeArrayAppend(assArray, "int8_1", valueCreateInt8(1));
	associativeArrayAppend(assArray, "uint8_0", valueCreateUInt8(0));
	associativeArrayAppend(assArray, "uint8_1", valueCreateUInt8(1));
	associativeArrayAppend(assArray, "int16_0", valueCreateInt16(0));
	associativeArrayAppend(assArray, "int16_1", valueCreateInt16(1));
	associativeArrayAppend(assArray, "uint16_0", valueCreateUInt16(0));
	associativeArrayAppend(assArray, "uint16_1", valueCreateUInt16(1));
	associativeArrayAppend(assArray, "int32_0", valueCreateInt32(0));
	associativeArrayAppend(assArray, "int32_1", valueCreateInt32(1));
	associativeArrayAppend(assArray, "uint32_0", valueCreateUInt32(0));
	associativeArrayAppend(assArray, "uint32_1", valueCreateUInt32(1));
	associativeArrayAppend(assArray, "int64_0", valueCreateInt64(0));
	associativeArrayAppend(assArray, "int64_1", valueCreateInt64(1));
	associativeArrayAppend(assArray, "uint64_0", valueCreateUInt64(0));
	associativeArrayAppend(assArray, "uint64_1", valueCreateUInt64(1));
	associativeArrayAppend(assArray, "float_0", valueCreateFloat(0.0f));
	associativeArrayAppend(assArray, "float_1", valueCreateFloat(1.0f));
	associativeArrayAppend(assArray, "double_0", valueCreateDouble(0.0));
	associativeArrayAppend(assArray, "double_1", valueCreateDouble(1.0));
	associativeArrayAppend(assArray, "fixed_0", valueCreateFixed16_16(0x00000));
	associativeArrayAppend(assArray, "fixed_1", valueCreateFixed16_16(0x10000));
	associativeArrayAppend(assArray, "enum_0", valueCreateInt32(ENUM_TEST_0));
	associativeArrayAppend(assArray, "enum_1", valueCreateInt32(ENUM_TEST_1));
	associativeArrayAppend(assArray, "bitfield8_0", valueCreateUInt8(0xAA));
	associativeArrayAppend(assArray, "bitfield8_1", valueCreateUInt8(0x57));
	associativeArrayAppend(assArray, "bitfield16_0", valueCreateUInt16(0xF001));
	associativeArrayAppend(assArray, "bitfield16_1", valueCreateUInt16(0x000F));
	associativeArrayAppend(assArray, "bitfield32_0", valueCreateUInt32(0xF0000001));
	associativeArrayAppend(assArray, "bitfield32_1", valueCreateUInt32(0x0000001E));
	associativeArrayAppend(assArray, "bitfield64_0", valueCreateUInt64(0xF000000000000001ull));
	associativeArrayAppend(assArray, "bitfield64_1", valueCreateUInt64(0x000000000000003Cull));
	associativeArrayAppend(assArray, "string_0", valueCreateString("foo", DATA_USE_STRLEN, false, false));
	associativeArrayAppend(assArray, "string_1", valueCreateString("bar", DATA_USE_STRLEN, false, false));
	associativeArrayAppend(assArray, "blob_0", valueCreateBlob("a", 1, false, false));
	associativeArrayAppend(assArray, "blob_1", valueCreateBlob("abcd", 4, false, false));
	
	context = DataDeserializationContext_create(valueCreateAssociativeArray(assArray, false, false));
	beginAndVerifyDictionary(NULL, 38);
	verifyTestValues();
	context->endDictionary(context);
	
	associativeArrayDispose(assArray);
	DataDeserializationContext_dispose(context);
	
	assArray = associativeArrayCreate();
	associativeArrayAppend(assArray, "uint8_1", valueCreateUInt8(1));
	associativeArrayAppend(assArray, "uint8_0", valueCreateUInt8(0));
	associativeArrayAppend(assArray, "uint64_1", valueCreateUInt64(1));
	associativeArrayAppend(assArray, "uint64_0", valueCreateUInt64(0));
	associativeArrayAppend(assArray, "uint32_1", valueCreateUInt32(1));
	associativeArrayAppend(assArray, "uint32_0", valueCreateUInt32(0));
	associativeArrayAppend(assArray, "uint16_1", valueCreateUInt16(1));
	associativeArrayAppend(assArray, "uint16_0", valueCreateUInt16(0));
	associativeArrayAppend(assArray, "string_1", valueCreateString("bar", DATA_USE_STRLEN, false, false));
	associativeArrayAppend(assArray, "string_0", valueCreateString("foo", DATA_USE_STRLEN, false, false));
	associativeArrayAppend(assArray, "int8_1", valueCreateInt8(1));
	associativeArrayAppend(assArray, "int8_0", valueCreateInt8(0));
	associativeArrayAppend(assArray, "int64_1", valueCreateInt64(1));
	associativeArrayAppend(assArray, "int64_0", valueCreateInt64(0));
	associativeArrayAppend(assArray, "int32_1", valueCreateInt32(1));
	associativeArrayAppend(assArray, "int32_0", valueCreateInt32(0));
	associativeArrayAppend(assArray, "int16_1", valueCreateInt16(1));
	associativeArrayAppend(assArray, "int16_0", valueCreateInt16(0));
	associativeArrayAppend(assArray, "float_1", valueCreateFloat(1.0f));
	associativeArrayAppend(assArray, "float_0", valueCreateFloat(0.0f));
	associativeArrayAppend(assArray, "enum_1", valueCreateInt32(ENUM_TEST_1));
	associativeArrayAppend(assArray, "enum_0", valueCreateInt32(ENUM_TEST_0));
	associativeArrayAppend(assArray, "double_1", valueCreateDouble(1.0));
	associativeArrayAppend(assArray, "double_0", valueCreateDouble(0.0));
	associativeArrayAppend(assArray, "fixed_1", valueCreateFixed16_16(0x10000));
	associativeArrayAppend(assArray, "fixed_0", valueCreateFixed16_16(0x00000));
	associativeArrayAppend(assArray, "bool_1", valueCreateBoolean(true));
	associativeArrayAppend(assArray, "bool_0", valueCreateBoolean(false));
	associativeArrayAppend(assArray, "blob_1", valueCreateBlob("abcd", 4, false, false));
	associativeArrayAppend(assArray, "blob_0", valueCreateBlob("a", 1, false, false));
	associativeArrayAppend(assArray, "bitfield8_1", valueCreateUInt8(0x57));
	associativeArrayAppend(assArray, "bitfield8_0", valueCreateUInt8(0xAA));
	associativeArrayAppend(assArray, "bitfield64_1", valueCreateUInt64(0x000000000000003Cull));
	associativeArrayAppend(assArray, "bitfield64_0", valueCreateUInt64(0xF000000000000001ull));
	associativeArrayAppend(assArray, "bitfield32_1", valueCreateUInt32(0x0000001E));
	associativeArrayAppend(assArray, "bitfield32_0", valueCreateUInt32(0xF0000001));
	associativeArrayAppend(assArray, "bitfield16_1", valueCreateUInt16(0x000F));
	associativeArrayAppend(assArray, "bitfield16_0", valueCreateUInt16(0xF001));
	
	context = DataDeserializationContext_create(valueCreateAssociativeArray(assArray, false, false));
	beginAndVerifyDictionary(NULL, 38);
	verifyTestValues();
	context->endDictionary(context);
	
	associativeArrayDispose(assArray);
	DataDeserializationContext_dispose(context);
}

static void testHierarchy() {
	DataDeserializationContext * context;
	DataValue value;
	AssociativeArray * assArray;
	DataArray * array;
	HashTable * hashTable;
	
	value = valueCreateAssociativeArray(associativeArrayCreate(), true, false);
	associativeArrayAppend(value.value.associativeArray, "bool_0", valueCreateBoolean(false));
	associativeArrayAppend(value.value.associativeArray, "array_0", valueCreateArray(array = arrayCreate(), true, false));
	arrayAppend(array, valueCreateBoolean(true));
	associativeArrayAppend(value.value.associativeArray, "int8_0", valueCreateInt8(0));
	associativeArrayAppend(value.value.associativeArray, "struct_0", valueCreateHashTable(hashTable = hashCreate(), true, false));
	hashSet(hashTable, "array_1", valueCreateArray(array = arrayCreate(), true, false));
	arrayAppend(array, valueCreateInt8(1));
	hashSet(hashTable, "uint8_0", valueCreateUInt8(0));
	associativeArrayAppend(value.value.associativeArray, "dictionary_1", valueCreateAssociativeArray(assArray = associativeArrayCreate(), true, false));
	associativeArrayAppend(assArray, "uint8_1", valueCreateUInt8(1));
	
	context = DataDeserializationContext_create(value);
	beginAndVerifyDictionary(NULL, 5);
	readAndVerifyNumber(bool, "bool_0", false);
	beginAndVerifyArray("array_0", 1);
	readAndVerifyNumber(bool, "bool_1", true);
	context->endArray(context);
	readAndVerifyNumber(int8_t, "int8_0", 0);
	context->beginStructure(context, "struct_0");
	beginAndVerifyArray("array_1", 1);
	readAndVerifyNumber(int8_t, "int8_1", 1);
	context->endArray(context);
	readAndVerifyNumber(uint8_t, "uint8_0", 0);
	context->endStructure(context);
	beginAndVerifyDictionary("dictionary_1", 1);
	readAndVerifyNumber(uint8_t, "uint8_1", 1);
	context->endDictionary(context);
	context->endDictionary(context);
	
	valueDispose(&value);
	DataDeserializationContext_dispose(context);
}

#define testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	context = DataDeserializationContext_create(VALUE); \
	PREAMBLE_CODE \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d (OK)", SERIALIZATION_ERROR_OK, context->status); \
	FAIL_CODE \
	TestCase_assert(context->status == ERROR_STATUS, "Expected %d but got %d (context->status)", ERROR_STATUS, context->status); \
	context->dispose(context); \
	\
	context = DataDeserializationContext_create(VALUE); \
	context->jmpBuf = &jmpBuf; \
	status = setjmp(jmpBuf); \
	if (!status) { \
		PREAMBLE_CODE \
		TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d (OK setjmp)", SERIALIZATION_ERROR_OK, context->status); \
		FAIL_CODE \
	} \
	TestCase_assert(status == ERROR_STATUS, "Expected %d but got %d (status setjmp)", ERROR_STATUS, status); \
	TestCase_assert(context->status == ERROR_STATUS, "Expected %d but got %d (context->status setjmp)", ERROR_STATUS, context->status); \
	context->dispose(context)

#define testFailureWithAllTypes(VALUE, ERROR_STATUS, PREAMBLE_CODE, KEY) \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBoolean(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readInt8(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readUInt8(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readInt16(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readUInt16(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readInt32(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readUInt32(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readInt64(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readUInt64(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readFloat(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readDouble(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readFixed16_16(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readEnumeration(context, KEY, enumKV(ENUM_TEST_0), NULL);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBitfield8(context, KEY, "a", NULL);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBitfield16(context, KEY, "a", NULL);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBitfield32(context, KEY, "a", NULL);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBitfield64(context, KEY, "a", NULL);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readString(context, KEY);); \
	testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBlob(context, KEY, &length);)

#define testFailureWithAllTypesExcept(VALUE, ERROR_STATUS, PREAMBLE_CODE, KEY, EXCEPTION) \
	if (strcmp(#EXCEPTION, "boolean")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBoolean(context, KEY);); \
	} \
	if (strcmp(#EXCEPTION, "int8")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readInt8(context, KEY);); \
	} \
	if (strcmp(#EXCEPTION, "uint8")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readUInt8(context, KEY);); \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBitfield8(context, KEY, "a", NULL);); \
	} \
	if (strcmp(#EXCEPTION, "int16")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readInt16(context, KEY);); \
	} \
	if (strcmp(#EXCEPTION, "uint16")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readUInt16(context, KEY);); \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBitfield16(context, KEY, "a", NULL);); \
	} \
	if (strcmp(#EXCEPTION, "int32")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readInt32(context, KEY);); \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readEnumeration(context, KEY, enumKV(ENUM_TEST_0), NULL);); \
	} \
	if (strcmp(#EXCEPTION, "uint32")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readUInt32(context, KEY);); \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBitfield32(context, KEY, "a", NULL);); \
	} \
	if (strcmp(#EXCEPTION, "int64")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readInt64(context, KEY);); \
	} \
	if (strcmp(#EXCEPTION, "uint64")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readUInt64(context, KEY);); \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBitfield64(context, KEY, "a", NULL);); \
	} \
	if (strcmp(#EXCEPTION, "float")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readFloat(context, KEY);); \
	} \
	if (strcmp(#EXCEPTION, "double")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readDouble(context, KEY);); \
	} \
	if (strcmp(#EXCEPTION, "fixed16_16")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readFixed16_16(context, KEY);); \
	} \
	if (strcmp(#EXCEPTION, "string")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readString(context, KEY);); \
	} \
	if (strcmp(#EXCEPTION, "blob")) { \
		testFailure(VALUE, ERROR_STATUS, PREAMBLE_CODE, context->readBlob(context, KEY, &length);); \
	}

#define testArrayTypeFailure(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, VALUE) \
	testFailure(VALUE, \
	            SERIALIZATION_ERROR_INCORRECT_TYPE, \
	            , \
	            BEGIN_CONTAINER_CODE); \
	testFailure(VALUE_CREATE_FUNC(CONTAINER_INIT_FUNC(VALUE), true, false), \
	            SERIALIZATION_ERROR_INCORRECT_TYPE, \
	            BEGIN_CONTAINER_CODE, \
	            BEGIN_CONTAINER_CODE)

#define testKeyedContainerTypeFailure(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, VALUE) \
	testFailure(VALUE, \
	            SERIALIZATION_ERROR_INCORRECT_TYPE, \
	            , \
	            BEGIN_CONTAINER_CODE); \
	testFailure(VALUE_CREATE_FUNC(CONTAINER_INIT_FUNC("a", VALUE, NULL), true, false), \
	            SERIALIZATION_ERROR_INCORRECT_TYPE, \
	            BEGIN_CONTAINER_CODE, \
	            BEGIN_CONTAINER_CODE)

#define testContainerTypeFailures(TEST_MACRO, BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC) \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateBoolean(false)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateInt8(0)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateUInt8(0)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateInt16(0)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateUInt16(0)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateInt32(0)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateUInt32(0)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateInt64(0)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateUInt64(0)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateFloat(0.0f)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateDouble(0.0)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateFixed16_16(0x00000)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateString("a", 1, false, false)); \
	TEST_MACRO(BEGIN_CONTAINER_CODE, VALUE_CREATE_FUNC, CONTAINER_INIT_FUNC, valueCreateBlob("a", 1, false, false))

static void testErrors() {
	DataDeserializationContext * context;
	jmp_buf jmpBuf;
	int status;
	size_t length;
	
	//SERIALIZATION_ERROR_INVALID_OPERATION
	testFailure(valueCreateArray(arrayCreate(), true, false),
	            SERIALIZATION_ERROR_INVALID_OPERATION,
	            context->beginArray(context, NULL);,
	            context->readNextDictionaryKey(context););
	testFailure(valueCreateArray(arrayCreate(), true, false),
	            SERIALIZATION_ERROR_INVALID_OPERATION,
	            context->beginArray(context, NULL);,
	            context->hasDictionaryKey(context, "a"););
	testFailure(valueCreateHashTable(hashCreate(), true, false),
	            SERIALIZATION_ERROR_INVALID_OPERATION,
	            context->beginStructure(context, NULL);,
	            context->readNextDictionaryKey(context););
	testFailure(valueCreateHashTable(hashCreate(), true, false),
	            SERIALIZATION_ERROR_INVALID_OPERATION,
	            context->beginStructure(context, NULL);,
	            context->hasDictionaryKey(context, "a"););
	
	//SERIALIZATION_ERROR_KEY_NOT_FOUND
	testFailureWithAllTypes(valueCreateAssociativeArray(associativeArrayCreate(), false, false),
	                        SERIALIZATION_ERROR_KEY_NOT_FOUND,
	                        context->beginDictionary(context, NULL);,
	                        "a");
	testFailureWithAllTypes(valueCreateHashTable(hashCreate(), false, false),
	                        SERIALIZATION_ERROR_KEY_NOT_FOUND,
	                        context->beginStructure(context, NULL);,
	                        "a");
	
	//SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateArray(arrayCreate(), true, false)), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginArray(context, NULL);,
	            context->endStructure(context););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateArray(arrayCreate(), true, false)), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginArray(context, NULL);
	            context->beginArray(context, NULL);,
	            context->endStructure(context););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateArray(arrayCreate(), true, false)), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginArray(context, NULL);,
	            context->endDictionary(context););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateArray(arrayCreate(), true, false)), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginArray(context, NULL);
	            context->beginArray(context, NULL);,
	            context->endDictionary(context););
	
	testFailure(valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues("a", valueCreateAssociativeArray(associativeArrayCreate(), true, false), NULL), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginDictionary(context, NULL);,
	            context->endStructure(context););
	testFailure(valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues("a", valueCreateAssociativeArray(associativeArrayCreate(), true, false), NULL), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginDictionary(context, NULL);
	            context->beginDictionary(context, "a");,
	            context->endStructure(context););
	testFailure(valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues("a", valueCreateAssociativeArray(associativeArrayCreate(), true, false), NULL), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginDictionary(context, NULL);,
	            context->endArray(context););
	testFailure(valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues("a", valueCreateAssociativeArray(associativeArrayCreate(), true, false), NULL), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginDictionary(context, NULL);
	            context->beginDictionary(context, "a");,
	            context->endArray(context););
	
	testFailure(valueCreateHashTable(hashCreateWithKeysAndValues("a", valueCreateHashTable(hashCreate(), true, false), NULL), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginStructure(context, NULL);,
	            context->endDictionary(context););
	testFailure(valueCreateHashTable(hashCreateWithKeysAndValues("a", valueCreateHashTable(hashCreate(), true, false), NULL), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginStructure(context, NULL);
	            context->beginStructure(context, "a");,
	            context->endDictionary(context););
	testFailure(valueCreateHashTable(hashCreateWithKeysAndValues("a", valueCreateHashTable(hashCreate(), true, false), NULL), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginStructure(context, NULL);,
	            context->endArray(context););
	testFailure(valueCreateHashTable(hashCreateWithKeysAndValues("a", valueCreateHashTable(hashCreate(), true, false), NULL), true, false),
	            SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginStructure(context, NULL);
	            context->beginStructure(context, "a");,
	            context->endArray(context););
	
	//SERIALIZATION_ERROR_CONTAINER_UNDERFLOW
	testFailure(valueCreateArray(arrayCreate(), true, false),
	            SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            ,
	            context->endArray(context););
	testFailure(valueCreateArray(arrayCreate(), true, false),
	            SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            context->beginArray(context, NULL);
	            context->endArray(context);,
	            context->endArray(context););
	
	testFailure(valueCreateAssociativeArray(associativeArrayCreate(), true, false),
	            SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            ,
	            context->endDictionary(context););
	testFailure(valueCreateAssociativeArray(associativeArrayCreate(), true, false),
	            SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            context->beginDictionary(context, NULL);
	            context->endDictionary(context);,
	            context->endDictionary(context););
	
	testFailure(valueCreateHashTable(hashCreate(), true, false),
	            SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            ,
	            context->endStructure(context););
	testFailure(valueCreateHashTable(hashCreate(), true, false),
	            SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            context->beginStructure(context, NULL);
	            context->endStructure(context);,
	            context->endStructure(context););
	
	//SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS
	testFailure(valueCreateArray(arrayCreate(), true, false),
	            SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginArray(context, NULL);
	            context->endArray(context);,
	            context->beginArray(context, NULL););
	testFailure(valueCreateArray(arrayCreate(), true, false),
	            SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginArray(context, NULL);
	            context->endArray(context);,
	            context->beginStructure(context, NULL););
	testFailure(valueCreateArray(arrayCreate(), true, false),
	            SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginArray(context, NULL);
	            context->endArray(context);,
	            context->beginDictionary(context, NULL););
	
	testFailure(valueCreateAssociativeArray(associativeArrayCreate(), true, false),
	            SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginDictionary(context, NULL);
	            context->endDictionary(context);,
	            context->beginDictionary(context, NULL););
	testFailure(valueCreateAssociativeArray(associativeArrayCreate(), true, false),
	            SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginDictionary(context, NULL);
	            context->endDictionary(context);,
	            context->beginArray(context, NULL););
	testFailure(valueCreateAssociativeArray(associativeArrayCreate(), true, false),
	            SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginDictionary(context, NULL);
	            context->endDictionary(context);,
	            context->beginStructure(context, NULL););
	
	testFailure(valueCreateHashTable(hashCreate(), true, false),
	            SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginStructure(context, NULL);
	            context->endStructure(context);,
	            context->beginStructure(context, NULL););
	testFailure(valueCreateHashTable(hashCreate(), true, false),
	            SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginStructure(context, NULL);
	            context->endStructure(context);,
	            context->beginArray(context, NULL););
	testFailure(valueCreateHashTable(hashCreate(), true, false),
	            SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginStructure(context, NULL);
	            context->endStructure(context);,
	            context->beginDictionary(context, NULL););
	
	//SERIALIZATION_ERROR_UNNAMED_BIT
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateUInt8(12)), true, false),
	            SERIALIZATION_ERROR_UNNAMED_BIT,
	            context->beginArray(context, NULL);,
	            context->readBitfield8(context, NULL, "a", NULL););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateUInt16(12)), true, false),
	            SERIALIZATION_ERROR_UNNAMED_BIT,
	            context->beginArray(context, NULL);,
	            context->readBitfield16(context, NULL, "a", NULL););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateUInt32(12)), true, false),
	            SERIALIZATION_ERROR_UNNAMED_BIT,
	            context->beginArray(context, NULL);,
	            context->readBitfield32(context, NULL, "a", NULL););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateUInt64(12)), true, false),
	            SERIALIZATION_ERROR_UNNAMED_BIT,
	            context->beginArray(context, NULL);,
	            context->readBitfield64(context, NULL, "a", NULL););
	
	//SERIALIZATION_ERROR_DUPLICATE_BIT
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateUInt8(1)), true, false),
	            SERIALIZATION_ERROR_DUPLICATE_BIT,
	            context->beginArray(context, NULL);,
	            context->readBitfield8(context, NULL, "a", "a", NULL););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateUInt16(1)), true, false),
	            SERIALIZATION_ERROR_DUPLICATE_BIT,
	            context->beginArray(context, NULL);,
	            context->readBitfield16(context, NULL, "a", "a", NULL););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateUInt32(1)), true, false),
	            SERIALIZATION_ERROR_DUPLICATE_BIT,
	            context->beginArray(context, NULL);,
	            context->readBitfield32(context, NULL, "a", "a", NULL););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateUInt64(1)), true, false),
	            SERIALIZATION_ERROR_DUPLICATE_BIT,
	            context->beginArray(context, NULL);,
	            context->readBitfield64(context, NULL, "a", "a", NULL););
	
	//SERIALIZATION_ERROR_ENUM_NOT_NAMED
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateInt32(1)), true, false),
	            SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	            context->beginArray(context, NULL);,
	            context->readEnumeration(context, NULL, NULL););
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateInt32(1)), true, false),
	            SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	            context->beginArray(context, NULL);,
	            context->readEnumeration(context, NULL, "a", 0, NULL););
	
	//SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateInt32(0)), true, false),
	            SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME,
	            context->beginArray(context, NULL);,
	            context->readEnumeration(context, NULL, "a", 0, "a", 1, NULL););
	
	//SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE
	testFailure(valueCreateArray(arrayCreateWithValues(valueCreateInt32(0)), true, false),
	            SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE,
	            context->beginArray(context, NULL);,
	            context->readEnumeration(context, NULL, "a", 0, "b", 0, NULL););
	
	//SERIALIZATION_ERROR_NULL_KEY
	testFailureWithAllTypes(valueCreateAssociativeArray(associativeArrayCreate(), true, false),
	                        SERIALIZATION_ERROR_NULL_KEY,
	                        context->beginDictionary(context, NULL);,
	                        NULL);
	testFailureWithAllTypes(valueCreateHashTable(hashCreate(), true, false),
	                        SERIALIZATION_ERROR_NULL_KEY,
	                        context->beginStructure(context, NULL);,
	                        NULL);
	
	//SERIALIZATION_ERROR_INCORRECT_TYPE
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateBoolean(false)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              boolean);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateInt8(0)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              int8);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateUInt8(0)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              uint8);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateInt16(0)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              int16);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateUInt16(0)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              uint16);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateInt32(0)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              int32);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateUInt32(0)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              uint32);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateInt64(0)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              int64);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateUInt64(0)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              uint64);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateFloat(0.0f)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              float);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateDouble(0.0)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              double);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateFixed16_16(0x00000)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              fixed16_16);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateString("a", DATA_USE_STRLEN, false, false)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              string);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateBlob("a", 1, false, false)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              blob);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateArray(arrayCreate(), true, false)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              none);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateAssociativeArray(associativeArrayCreate(), true, false)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              none);
	testFailureWithAllTypesExcept(valueCreateArray(arrayCreateWithValues(valueCreateHashTable(hashCreate(), true, false)), true, false),
	                              SERIALIZATION_ERROR_INCORRECT_TYPE,
	                              context->beginArray(context, NULL);,
	                              NULL,
	                              none);
	
	testContainerTypeFailures(testArrayTypeFailure, context->beginArray(context, NULL);, valueCreateArray, arrayCreateWithValues);
	testContainerTypeFailures(testKeyedContainerTypeFailure, context->beginStructure(context, "a");, valueCreateHashTable, hashCreateWithKeysAndValues);
	testContainerTypeFailures(testKeyedContainerTypeFailure, context->beginDictionary(context, "a");, valueCreateAssociativeArray, associativeArrayCreateWithKeysAndValues);
	
	//SERIALIZATION_ERROR_END_OF_CONTAINER
	testFailureWithAllTypes(valueCreateArray(arrayCreate(), true, false),
	                        SERIALIZATION_ERROR_END_OF_CONTAINER,
	                        context->beginArray(context, NULL);,
	                        NULL);
	testFailureWithAllTypes(valueCreateArray(arrayCreateWithValues(valueCreateBoolean(false)), true, false),
	                        SERIALIZATION_ERROR_END_OF_CONTAINER,
	                        context->beginArray(context, NULL);
	                        context->readBoolean(context, NULL);,
	                        NULL);
	
	testFailure(valueCreateAssociativeArray(associativeArrayCreate(), true, false),
	            SERIALIZATION_ERROR_END_OF_CONTAINER,
	            context->beginDictionary(context, NULL);,
	            context->readNextDictionaryKey(context););
	testFailure(valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues("a", valueCreateBoolean(false), NULL), true, false),
	            SERIALIZATION_ERROR_END_OF_CONTAINER,
	            context->beginDictionary(context, NULL);
	            context->readNextDictionaryKey(context);,
	            context->readNextDictionaryKey(context););
	
	//SERIALIZATION_ERROR_NO_CONTAINER_STARTED
	testFailureWithAllTypes(valueCreateArray(arrayCreate(), true, false),
	                        SERIALIZATION_ERROR_NO_CONTAINER_STARTED,
	                        ,
	                        NULL);
}

static void testDictionaryKeys() {
	DataDeserializationContext * context;
	AssociativeArray * assArray;
	const char * key;
	
	assArray = associativeArrayCreate();
	associativeArrayAppend(assArray, "bool_0", valueCreateBoolean(false));
	associativeArrayAppend(assArray, "bool_1", valueCreateBoolean(true));
	
	context = DataDeserializationContext_create(valueCreateAssociativeArray(assArray, false, false));
	beginAndVerifyDictionary(NULL, 2);
	verifyHasDictionaryKey("bool_0", true);
	verifyHasDictionaryKey("bool_1", true);
	verifyHasDictionaryKey("int8_0", false);
	verifyReadNextDictionaryKey("bool_0");
	verifyReadNextDictionaryKey("bool_1");
	context->endDictionary(context);
	
	associativeArrayDispose(assArray);
	DataDeserializationContext_dispose(context);
}

TEST_SUITE(DataDeserializationContextTest,
           testInit,
           testValuesInArray,
           testValuesInHashTable,
           testValuesInAssociativeArray,
           testHierarchy,
           testErrors,
           testDictionaryKeys)
