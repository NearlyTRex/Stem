#include "dynamictypes/AssociativeArray.h"
#include "dynamictypes/DataArray.h"
#include "dynamictypes/HashTable.h"
#include "dynamictypes/DataSerializationContext.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"
#include <string.h>

#define verifyFunctionPointer(context, name) \
	TestCase_assert((context)->name == DataSerializationContext_##name, "Expected " #name " to be %p, but got %p", DataSerializationContext_##name, (context)->name)

#define verifyFunctionPointers(context) \
	verifyFunctionPointer(context, dispose); \
	verifyFunctionPointer(context, beginStructure); \
	verifyFunctionPointer(context, beginDictionary); \
	verifyFunctionPointer(context, beginArray); \
	verifyFunctionPointer(context, endStructure); \
	verifyFunctionPointer(context, endDictionary); \
	verifyFunctionPointer(context, endArray); \
	verifyFunctionPointer(context, writeBoolean); \
	verifyFunctionPointer(context, writeInt8); \
	verifyFunctionPointer(context, writeUInt8); \
	verifyFunctionPointer(context, writeInt16); \
	verifyFunctionPointer(context, writeUInt16); \
	verifyFunctionPointer(context, writeInt32); \
	verifyFunctionPointer(context, writeUInt32); \
	verifyFunctionPointer(context, writeInt64); \
	verifyFunctionPointer(context, writeUInt64); \
	verifyFunctionPointer(context, writeFloat); \
	verifyFunctionPointer(context, writeDouble); \
	verifyFunctionPointer(context, writeEnumeration); \
	verifyFunctionPointer(context, writeBitfield8); \
	verifyFunctionPointer(context, writeBitfield16); \
	verifyFunctionPointer(context, writeBitfield32); \
	verifyFunctionPointer(context, writeBitfield64); \
	verifyFunctionPointer(context, writeString); \
	verifyFunctionPointer(context, writeBlob)

static void testInit() {
	DataSerializationContext context, * contextPtr;
	
	memset(&context, 0x00, sizeof(context));
	DataSerializationContext_init(&context);
	verifyFunctionPointers(&context);
	DataSerializationContext_dispose(&context);
	
	memset(&context, 0xFF, sizeof(context));
	DataSerializationContext_init(&context);
	verifyFunctionPointers(&context);
	DataSerializationContext_dispose(&context);
	
	contextPtr = DataSerializationContext_create();
	verifyFunctionPointers(contextPtr);
	DataSerializationContext_dispose(contextPtr);
}

#define ENUM_TEST_0 0
#define ENUM_TEST_1 1

#define assertDataValueInArray(VALUE, INDEX, TYPE, EXPECTED_VALUE, FIELD) \
	TestCase_assert(VALUE.array->values[INDEX].type == TYPE, "Expected %d but got %d", TYPE, VALUE.array->values[INDEX].type); \
	TestCase_assert(VALUE.array->values[INDEX].value.FIELD == EXPECTED_VALUE, "Expected " #EXPECTED_VALUE " but got %d", VALUE.array->values[INDEX].value.FIELD)

#define assertStringInArray(VALUE, INDEX, EXPECTED_VALUE) \
	TestCase_assert(VALUE.array->values[INDEX].type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, VALUE.array->values[INDEX].type); \
	TestCase_assert(!strcmp(VALUE.array->values[INDEX].value.string.bytes, EXPECTED_VALUE), "Expected " EXPECTED_VALUE " but got %d", VALUE.array->values[INDEX].value.string.bytes)

#define assertBlobInArray(VALUE, INDEX, EXPECTED_VALUE, LENGTH) \
	TestCase_assert(VALUE.array->values[INDEX].type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, VALUE.array->values[INDEX].type); \
	TestCase_assert(VALUE.array->values[INDEX].value.blob.length == LENGTH, "Expected " #LENGTH " but got " SIZE_T_FORMAT, VALUE.array->values[INDEX].value.blob.length); \
	TestCase_assert(!memcmp(VALUE.array->values[INDEX].value.blob.bytes, EXPECTED_VALUE, LENGTH), "Expected \"" EXPECTED_VALUE "\" but got \"%s\"", VALUE.array->values[INDEX].value.blob.bytes)

static void testValuesInArray() {
	DataSerializationContext * context;
	DataValue result;
	
	context = DataSerializationContext_create();
	context->beginArray(context, NULL);
	context->writeBoolean(context, NULL, false);
	context->writeBoolean(context, NULL, true);
	context->writeInt8(context, NULL, 0);
	context->writeInt8(context, NULL, 1);
	context->writeUInt8(context, NULL, 0);
	context->writeUInt8(context, NULL, 1);
	context->writeInt16(context, NULL, 0);
	context->writeInt16(context, NULL, 1);
	context->writeUInt16(context, NULL, 0);
	context->writeUInt16(context, NULL, 1);
	context->writeInt32(context, NULL, 0);
	context->writeInt32(context, NULL, 1);
	context->writeUInt32(context, NULL, 0);
	context->writeUInt32(context, NULL, 1);
	context->writeInt64(context, NULL, 0);
	context->writeInt64(context, NULL, 1);
	context->writeUInt64(context, NULL, 0);
	context->writeUInt64(context, NULL, 1);
	context->writeFloat(context, NULL, 0.0f);
	context->writeFloat(context, NULL, 1.0f);
	context->writeDouble(context, NULL, 0.0);
	context->writeDouble(context, NULL, 1.0);
	context->writeEnumeration(context, NULL, ENUM_TEST_0, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeEnumeration(context, NULL, ENUM_TEST_1, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeBitfield8(context, NULL, 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7", NULL);
	context->writeBitfield8(context, NULL, 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	context->writeBitfield16(context, NULL, 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", NULL);
	context->writeBitfield16(context, NULL, 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield32(context, NULL, 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", NULL);
	context->writeBitfield32(context, NULL, 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield64(context, NULL, 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", NULL);
	context->writeBitfield64(context, NULL, 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeString(context, NULL, "foo");
	context->writeString(context, NULL, "bar");
	context->writeBlob(context, NULL, "a", 1);
	context->writeBlob(context, NULL, "abcd", 4);
	context->endArray(context);
	
	result = DataSerializationContext_result(context);
	TestCase_assert(result.type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, result.type);
	TestCase_assert(result.value.array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.array->count == 36, "Expected 36 but got " SIZE_T_FORMAT, result.value.array->count);
	assertDataValueInArray(result.value, 0, DATA_TYPE_BOOLEAN, false, boolean);
	assertDataValueInArray(result.value, 1, DATA_TYPE_BOOLEAN, true, boolean);
	assertDataValueInArray(result.value, 2, DATA_TYPE_INT8, 0, int8);
	assertDataValueInArray(result.value, 3, DATA_TYPE_INT8, 1, int8);
	assertDataValueInArray(result.value, 4, DATA_TYPE_UINT8, 0, uint8);
	assertDataValueInArray(result.value, 5, DATA_TYPE_UINT8, 1, uint8);
	assertDataValueInArray(result.value, 6, DATA_TYPE_INT16, 0, int16);
	assertDataValueInArray(result.value, 7, DATA_TYPE_INT16, 1, int16);
	assertDataValueInArray(result.value, 8, DATA_TYPE_UINT16, 0, uint16);
	assertDataValueInArray(result.value, 9, DATA_TYPE_UINT16, 1, uint16);
	assertDataValueInArray(result.value, 10, DATA_TYPE_INT32, 0, int32);
	assertDataValueInArray(result.value, 11, DATA_TYPE_INT32, 1, int32);
	assertDataValueInArray(result.value, 12, DATA_TYPE_UINT32, 0, uint32);
	assertDataValueInArray(result.value, 13, DATA_TYPE_UINT32, 1, uint32);
	assertDataValueInArray(result.value, 14, DATA_TYPE_INT64, 0, int64);
	assertDataValueInArray(result.value, 15, DATA_TYPE_INT64, 1, int64);
	assertDataValueInArray(result.value, 16, DATA_TYPE_UINT64, 0, uint64);
	assertDataValueInArray(result.value, 17, DATA_TYPE_UINT64, 1, uint64);
	assertDataValueInArray(result.value, 18, DATA_TYPE_FLOAT, 0, float32);
	assertDataValueInArray(result.value, 19, DATA_TYPE_FLOAT, 1, float32);
	assertDataValueInArray(result.value, 20, DATA_TYPE_DOUBLE, 0, float64);
	assertDataValueInArray(result.value, 21, DATA_TYPE_DOUBLE, 1, float64);
	assertDataValueInArray(result.value, 22, DATA_TYPE_INT32, ENUM_TEST_0, int32);
	assertDataValueInArray(result.value, 23, DATA_TYPE_INT32, ENUM_TEST_1, int32);
	assertDataValueInArray(result.value, 24, DATA_TYPE_UINT8, 0xAA, uint8);
	assertDataValueInArray(result.value, 25, DATA_TYPE_UINT8, 0x57, uint8);
	assertDataValueInArray(result.value, 26, DATA_TYPE_UINT16, 0xF001, uint16);
	assertDataValueInArray(result.value, 27, DATA_TYPE_UINT16, 0x000F, uint16);
	assertDataValueInArray(result.value, 28, DATA_TYPE_UINT32, 0xF0000001, uint32);
	assertDataValueInArray(result.value, 29, DATA_TYPE_UINT32, 0x0000001E, uint32);
	assertDataValueInArray(result.value, 30, DATA_TYPE_UINT64, 0xF000000000000001ull, uint64);
	assertDataValueInArray(result.value, 31, DATA_TYPE_UINT64, 0x000000000000003Cull, uint64);
	assertStringInArray(result.value, 32, "foo");
	assertStringInArray(result.value, 33, "bar");
	assertBlobInArray(result.value, 34, "a", 1);
	assertBlobInArray(result.value, 35, "abcd", 4);
	
	DataSerializationContext_dispose(context);
}

#define assertDataValueInHashTable(VALUE, KEY, TYPE, EXPECTED_VALUE, FIELD) \
	value = hashGet(VALUE.hashTable, KEY); \
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL"); \
	TestCase_assert(value->type == TYPE, "Expected %d but got %d", TYPE, value->type); \
	TestCase_assert(value->value.FIELD == EXPECTED_VALUE, "Expected " #EXPECTED_VALUE " but got %d", value->value.FIELD)

#define assertStringInHashTable(VALUE, KEY, EXPECTED_VALUE) \
	value = hashGet(VALUE.hashTable, KEY); \
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL"); \
	TestCase_assert(value->type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, value->type); \
	TestCase_assert(!strcmp(value->value.string.bytes, EXPECTED_VALUE), "Expected " EXPECTED_VALUE " but got %d", value->value.string.bytes)

#define assertBlobInHashTable(VALUE, KEY, EXPECTED_VALUE, LENGTH) \
	value = hashGet(VALUE.hashTable, KEY); \
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL"); \
	TestCase_assert(value->type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, value->type); \
	TestCase_assert(value->value.blob.length == LENGTH, "Expected " #LENGTH " but got " SIZE_T_FORMAT, value->value.blob.length); \
	TestCase_assert(!memcmp(value->value.blob.bytes, EXPECTED_VALUE, LENGTH), "Expected \"" EXPECTED_VALUE "\" but got \"%s\"", value->value.blob.bytes)

static void testValuesInHashTable() {
	DataSerializationContext * context;
	DataValue result, * value;
	
	context = DataSerializationContext_create();
	context->beginStructure(context, NULL);
	context->writeBoolean(context, "bool_0", false);
	context->writeBoolean(context, "bool_1", true);
	context->writeInt8(context, "int8_0", 0);
	context->writeInt8(context, "int8_1", 1);
	context->writeUInt8(context, "uint8_0", 0);
	context->writeUInt8(context, "uint8_1", 1);
	context->writeInt16(context, "int16_0", 0);
	context->writeInt16(context, "int16_1", 1);
	context->writeUInt16(context, "uint16_0", 0);
	context->writeUInt16(context, "uint16_1", 1);
	context->writeInt32(context, "int32_0", 0);
	context->writeInt32(context, "int32_1", 1);
	context->writeUInt32(context, "uint32_0", 0);
	context->writeUInt32(context, "uint32_1", 1);
	context->writeInt64(context, "int64_0", 0);
	context->writeInt64(context, "int64_1", 1);
	context->writeUInt64(context, "uint64_0", 0);
	context->writeUInt64(context, "uint64_1", 1);
	context->writeFloat(context, "float_0", 0.0f);
	context->writeFloat(context, "float_1", 1.0f);
	context->writeDouble(context, "double_0", 0.0);
	context->writeDouble(context, "double_1", 1.0);
	context->writeEnumeration(context, "enum_0", ENUM_TEST_0, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeEnumeration(context, "enum_1", ENUM_TEST_1, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeBitfield8(context, "bitfield8_0", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7", NULL);
	context->writeBitfield8(context, "bitfield8_1", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	context->writeBitfield16(context, "bitfield16_0", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", NULL);
	context->writeBitfield16(context, "bitfield16_1", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield32(context, "bitfield32_0", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", NULL);
	context->writeBitfield32(context, "bitfield32_1", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield64(context, "bitfield64_0", 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", NULL);
	context->writeBitfield64(context, "bitfield64_1", 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeString(context, "string_0", "foo");
	context->writeString(context, "string_1", "bar");
	context->writeBlob(context, "blob_0", "a", 1);
	context->writeBlob(context, "blob_1", "abcd", 4);
	context->endStructure(context);
	
	result = DataSerializationContext_result(context);
	TestCase_assert(result.type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, result.type);
	TestCase_assert(result.value.hashTable != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.hashTable->keyCount == 36, "Expected 36 but got " SIZE_T_FORMAT, result.value.hashTable->keyCount);
	assertDataValueInHashTable(result.value, "bool_0", DATA_TYPE_BOOLEAN, false, boolean);
	assertDataValueInHashTable(result.value, "bool_1", DATA_TYPE_BOOLEAN, true, boolean);
	assertDataValueInHashTable(result.value, "int8_0", DATA_TYPE_INT8, 0, int8);
	assertDataValueInHashTable(result.value, "int8_1", DATA_TYPE_INT8, 1, int8);
	assertDataValueInHashTable(result.value, "uint8_0", DATA_TYPE_UINT8, 0, uint8);
	assertDataValueInHashTable(result.value, "uint8_1", DATA_TYPE_UINT8, 1, uint8);
	assertDataValueInHashTable(result.value, "int16_0", DATA_TYPE_INT16, 0, int16);
	assertDataValueInHashTable(result.value, "int16_1", DATA_TYPE_INT16, 1, int16);
	assertDataValueInHashTable(result.value, "uint16_0", DATA_TYPE_UINT16, 0, uint16);
	assertDataValueInHashTable(result.value, "uint16_1", DATA_TYPE_UINT16, 1, uint16);
	assertDataValueInHashTable(result.value, "int32_0", DATA_TYPE_INT32, 0, int32);
	assertDataValueInHashTable(result.value, "int32_1", DATA_TYPE_INT32, 1, int32);
	assertDataValueInHashTable(result.value, "uint32_0", DATA_TYPE_UINT32, 0, uint32);
	assertDataValueInHashTable(result.value, "uint32_1", DATA_TYPE_UINT32, 1, uint32);
	assertDataValueInHashTable(result.value, "int64_0", DATA_TYPE_INT64, 0, int64);
	assertDataValueInHashTable(result.value, "int64_1", DATA_TYPE_INT64, 1, int64);
	assertDataValueInHashTable(result.value, "uint64_0", DATA_TYPE_UINT64, 0, uint64);
	assertDataValueInHashTable(result.value, "uint64_1", DATA_TYPE_UINT64, 1, uint64);
	assertDataValueInHashTable(result.value, "float_0", DATA_TYPE_FLOAT, 0, float32);
	assertDataValueInHashTable(result.value, "float_1", DATA_TYPE_FLOAT, 1, float32);
	assertDataValueInHashTable(result.value, "double_0", DATA_TYPE_DOUBLE, 0, float64);
	assertDataValueInHashTable(result.value, "double_1", DATA_TYPE_DOUBLE, 1, float64);
	assertDataValueInHashTable(result.value, "enum_0", DATA_TYPE_INT32, ENUM_TEST_0, int32);
	assertDataValueInHashTable(result.value, "enum_1", DATA_TYPE_INT32, ENUM_TEST_1, int32);
	assertDataValueInHashTable(result.value, "bitfield8_0", DATA_TYPE_UINT8, 0xAA, uint8);
	assertDataValueInHashTable(result.value, "bitfield8_1", DATA_TYPE_UINT8, 0x57, uint8);
	assertDataValueInHashTable(result.value, "bitfield16_0", DATA_TYPE_UINT16, 0xF001, uint16);
	assertDataValueInHashTable(result.value, "bitfield16_1", DATA_TYPE_UINT16, 0x000F, uint16);
	assertDataValueInHashTable(result.value, "bitfield32_0", DATA_TYPE_UINT32, 0xF0000001, uint32);
	assertDataValueInHashTable(result.value, "bitfield32_1", DATA_TYPE_UINT32, 0x0000001E, uint32);
	assertDataValueInHashTable(result.value, "bitfield64_0", DATA_TYPE_UINT64, 0xF000000000000001ull, uint64);
	assertDataValueInHashTable(result.value, "bitfield64_1", DATA_TYPE_UINT64, 0x000000000000003Cull, uint64);
	assertStringInHashTable(result.value, "string_0", "foo");
	assertStringInHashTable(result.value, "string_1", "bar");
	assertBlobInHashTable(result.value, "blob_0", "a", 1);
	assertBlobInHashTable(result.value, "blob_1", "abcd", 4);
	
	DataSerializationContext_dispose(context);
}

#define assertDataValueInAssociativeArray(VALUE, INDEX, KEY, TYPE, EXPECTED_VALUE, FIELD) \
	TestCase_assert(VALUE.associativeArray->values[INDEX].type == TYPE, "Expected %d but got %d", TYPE, VALUE.associativeArray->values[INDEX].type); \
	TestCase_assert(VALUE.associativeArray->values[INDEX].value.FIELD == EXPECTED_VALUE, "Expected " #EXPECTED_VALUE " but got %d", VALUE.associativeArray->values[INDEX].value.FIELD); \
	TestCase_assert(!strcmp(VALUE.associativeArray->keys[INDEX], KEY), "Expected \"%s\" but got \"%s\"", KEY, VALUE.associativeArray->keys[INDEX])

#define assertStringInAssociativeArray(VALUE, INDEX, KEY, EXPECTED_VALUE) \
	TestCase_assert(VALUE.associativeArray->values[INDEX].type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, VALUE.associativeArray->values[INDEX].type); \
	TestCase_assert(!strcmp(VALUE.associativeArray->values[INDEX].value.string.bytes, EXPECTED_VALUE), "Expected " EXPECTED_VALUE " but got %d", VALUE.associativeArray->values[INDEX].value.string.bytes); \
	TestCase_assert(!strcmp(VALUE.associativeArray->keys[INDEX], KEY), "Expected \"%s\" but got \"%s\"", KEY, VALUE.associativeArray->keys[INDEX])

#define assertBlobInAssociativeArray(VALUE, INDEX, KEY, EXPECTED_VALUE, LENGTH) \
	TestCase_assert(VALUE.associativeArray->values[INDEX].type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, VALUE.associativeArray->values[INDEX].type); \
	TestCase_assert(VALUE.associativeArray->values[INDEX].value.blob.length == LENGTH, "Expected " #LENGTH " but got " SIZE_T_FORMAT, VALUE.associativeArray->values[INDEX].value.blob.length); \
	TestCase_assert(!memcmp(VALUE.associativeArray->values[INDEX].value.blob.bytes, EXPECTED_VALUE, LENGTH), "Expected \"" EXPECTED_VALUE "\" but got \"%s\"", VALUE.associativeArray->values[INDEX].value.blob.bytes); \
	TestCase_assert(!strcmp(VALUE.associativeArray->keys[INDEX], KEY), "Expected \"%s\" but got \"%s\"", KEY, VALUE.associativeArray->keys[INDEX])

static void testValuesInAssociativeArray() {
	DataSerializationContext * context;
	DataValue result;
	
	context = DataSerializationContext_create();
	context->beginDictionary(context, NULL);
	context->writeBoolean(context, "bool_0", false);
	context->writeBoolean(context, "bool_1", true);
	context->writeInt8(context, "int8_0", 0);
	context->writeInt8(context, "int8_1", 1);
	context->writeUInt8(context, "uint8_0", 0);
	context->writeUInt8(context, "uint8_1", 1);
	context->writeInt16(context, "int16_0", 0);
	context->writeInt16(context, "int16_1", 1);
	context->writeUInt16(context, "uint16_0", 0);
	context->writeUInt16(context, "uint16_1", 1);
	context->writeInt32(context, "int32_0", 0);
	context->writeInt32(context, "int32_1", 1);
	context->writeUInt32(context, "uint32_0", 0);
	context->writeUInt32(context, "uint32_1", 1);
	context->writeInt64(context, "int64_0", 0);
	context->writeInt64(context, "int64_1", 1);
	context->writeUInt64(context, "uint64_0", 0);
	context->writeUInt64(context, "uint64_1", 1);
	context->writeFloat(context, "float_0", 0.0f);
	context->writeFloat(context, "float_1", 1.0f);
	context->writeDouble(context, "double_0", 0.0);
	context->writeDouble(context, "double_1", 1.0);
	context->writeEnumeration(context, "enum_0", ENUM_TEST_0, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeEnumeration(context, "enum_1", ENUM_TEST_1, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeBitfield8(context, "bitfield8_0", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7", NULL);
	context->writeBitfield8(context, "bitfield8_1", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	context->writeBitfield16(context, "bitfield16_0", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", NULL);
	context->writeBitfield16(context, "bitfield16_1", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield32(context, "bitfield32_0", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", NULL);
	context->writeBitfield32(context, "bitfield32_1", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield64(context, "bitfield64_0", 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", NULL);
	context->writeBitfield64(context, "bitfield64_1", 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeString(context, "string_0", "foo");
	context->writeString(context, "string_1", "bar");
	context->writeBlob(context, "blob_0", "a", 1);
	context->writeBlob(context, "blob_1", "abcd", 4);
	context->endDictionary(context);
	
	result = DataSerializationContext_result(context);
	TestCase_assert(result.type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, result.type);
	TestCase_assert(result.value.associativeArray != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.associativeArray->count == 36, "Expected 36 but got " SIZE_T_FORMAT, result.value.associativeArray->count);
	assertDataValueInAssociativeArray(result.value, 0, "bool_0", DATA_TYPE_BOOLEAN, false, boolean);
	assertDataValueInAssociativeArray(result.value, 1, "bool_1", DATA_TYPE_BOOLEAN, true, boolean);
	assertDataValueInAssociativeArray(result.value, 2, "int8_0", DATA_TYPE_INT8, 0, int8);
	assertDataValueInAssociativeArray(result.value, 3, "int8_1", DATA_TYPE_INT8, 1, int8);
	assertDataValueInAssociativeArray(result.value, 4, "uint8_0", DATA_TYPE_UINT8, 0, uint8);
	assertDataValueInAssociativeArray(result.value, 5, "uint8_1", DATA_TYPE_UINT8, 1, uint8);
	assertDataValueInAssociativeArray(result.value, 6, "int16_0", DATA_TYPE_INT16, 0, int16);
	assertDataValueInAssociativeArray(result.value, 7, "int16_1", DATA_TYPE_INT16, 1, int16);
	assertDataValueInAssociativeArray(result.value, 8, "uint16_0", DATA_TYPE_UINT16, 0, uint16);
	assertDataValueInAssociativeArray(result.value, 9, "uint16_1", DATA_TYPE_UINT16, 1, uint16);
	assertDataValueInAssociativeArray(result.value, 10, "int32_0", DATA_TYPE_INT32, 0, int32);
	assertDataValueInAssociativeArray(result.value, 11, "int32_1", DATA_TYPE_INT32, 1, int32);
	assertDataValueInAssociativeArray(result.value, 12, "uint32_0", DATA_TYPE_UINT32, 0, uint32);
	assertDataValueInAssociativeArray(result.value, 13, "uint32_1", DATA_TYPE_UINT32, 1, uint32);
	assertDataValueInAssociativeArray(result.value, 14, "int64_0", DATA_TYPE_INT64, 0, int64);
	assertDataValueInAssociativeArray(result.value, 15, "int64_1", DATA_TYPE_INT64, 1, int64);
	assertDataValueInAssociativeArray(result.value, 16, "uint64_0", DATA_TYPE_UINT64, 0, uint64);
	assertDataValueInAssociativeArray(result.value, 17, "uint64_1", DATA_TYPE_UINT64, 1, uint64);
	assertDataValueInAssociativeArray(result.value, 18, "float_0", DATA_TYPE_FLOAT, 0, float32);
	assertDataValueInAssociativeArray(result.value, 19, "float_1", DATA_TYPE_FLOAT, 1, float32);
	assertDataValueInAssociativeArray(result.value, 20, "double_0", DATA_TYPE_DOUBLE, 0, float64);
	assertDataValueInAssociativeArray(result.value, 21, "double_1", DATA_TYPE_DOUBLE, 1, float64);
	assertDataValueInAssociativeArray(result.value, 22, "enum_0", DATA_TYPE_INT32, ENUM_TEST_0, int32);
	assertDataValueInAssociativeArray(result.value, 23, "enum_1", DATA_TYPE_INT32, ENUM_TEST_1, int32);
	assertDataValueInAssociativeArray(result.value, 24, "bitfield8_0", DATA_TYPE_UINT8, 0xAA, uint8);
	assertDataValueInAssociativeArray(result.value, 25, "bitfield8_1", DATA_TYPE_UINT8, 0x57, uint8);
	assertDataValueInAssociativeArray(result.value, 26, "bitfield16_0", DATA_TYPE_UINT16, 0xF001, uint16);
	assertDataValueInAssociativeArray(result.value, 27, "bitfield16_1", DATA_TYPE_UINT16, 0x000F, uint16);
	assertDataValueInAssociativeArray(result.value, 28, "bitfield32_0", DATA_TYPE_UINT32, 0xF0000001, uint32);
	assertDataValueInAssociativeArray(result.value, 29, "bitfield32_1", DATA_TYPE_UINT32, 0x0000001E, uint32);
	assertDataValueInAssociativeArray(result.value, 30, "bitfield64_0", DATA_TYPE_UINT64, 0xF000000000000001ull, uint64);
	assertDataValueInAssociativeArray(result.value, 31, "bitfield64_1", DATA_TYPE_UINT64, 0x000000000000003Cull, uint64);
	assertStringInAssociativeArray(result.value, 32, "string_0", "foo");
	assertStringInAssociativeArray(result.value, 33, "string_1", "bar");
	assertBlobInAssociativeArray(result.value, 34, "blob_0", "a", 1);
	assertBlobInAssociativeArray(result.value, 35, "blob_1", "abcd", 4);
	
	DataSerializationContext_dispose(context);
}

static void testHierarchy() {
	DataSerializationContext * context;
	DataValue result, * value;
	
	context = DataSerializationContext_create();
	context->beginDictionary(context, NULL);
	context->writeBoolean(context, "bool_0", false);
	context->beginArray(context, "array_0");
	context->writeBoolean(context, "bool_1", true);
	context->endArray(context);
	context->writeInt8(context, "int8_0", 0);
	context->beginStructure(context, "struct_0");
	context->beginArray(context, "array_1");
	context->writeInt8(context, "int8_1", 1);
	context->endArray(context);
	context->writeUInt8(context, "uint8_0", 0);
	context->endStructure(context);
	context->beginDictionary(context, "dictionary_1");
	context->writeUInt8(context, "uint8_1", 1);
	context->endDictionary(context);
	context->endDictionary(context);
	
	result = DataSerializationContext_result(context);
	TestCase_assert(result.type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, result.type);
	TestCase_assert(result.value.associativeArray != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.associativeArray->count == 5, "Expected 5 but got " SIZE_T_FORMAT, result.value.associativeArray->count);
	
	assertDataValueInAssociativeArray(result.value, 0, "bool_0", DATA_TYPE_BOOLEAN, false, boolean);
	TestCase_assert(!strcmp(result.value.associativeArray->keys[1], "array_0"), "Expected \"array_0\" but got \"%s\"", result.value.associativeArray->keys[1]);
	TestCase_assert(result.value.associativeArray->values[1].type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, result.value.associativeArray->values[1].type);
	TestCase_assert(result.value.associativeArray->values[1].value.array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.associativeArray->values[1].value.array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, result.value.associativeArray->values[1].value.array->count);
	assertDataValueInAssociativeArray(result.value, 2, "int8_0", DATA_TYPE_INT8, 0, int8);
	TestCase_assert(!strcmp(result.value.associativeArray->keys[3], "struct_0"), "Expected \"struct_0\" but got \"%s\"", result.value.associativeArray->keys[3]);
	TestCase_assert(result.value.associativeArray->values[3].type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, result.value.associativeArray->values[3].type);
	TestCase_assert(result.value.associativeArray->values[3].value.hashTable != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.associativeArray->values[3].value.hashTable->keyCount == 2, "Expected 2 but got " SIZE_T_FORMAT, result.value.associativeArray->values[3].value.hashTable->keyCount);
	TestCase_assert(!strcmp(result.value.associativeArray->keys[4], "dictionary_1"), "Expected \"dictionary_1\" but got \"%s\"", result.value.associativeArray->keys[4]);
	TestCase_assert(result.value.associativeArray->values[4].type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, result.value.associativeArray->values[4].type);
	TestCase_assert(result.value.associativeArray->values[4].value.associativeArray != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.associativeArray->values[4].value.associativeArray->count == 1, "Expected 1 but got " SIZE_T_FORMAT, result.value.associativeArray->values[4].value.associativeArray->count);
	
	assertDataValueInArray(result.value.associativeArray->values[1].value, 0, DATA_TYPE_BOOLEAN, true, boolean);
	value = hashGet(result.value.associativeArray->values[3].value.hashTable, "array_1");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, value->type);
	TestCase_assert(value->value.array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->value.array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, value->value.array->count);
	assertDataValueInArray(value->value, 0, DATA_TYPE_INT8, 1, int8);
	assertDataValueInHashTable(result.value.associativeArray->values[3].value, "uint8_0", DATA_TYPE_UINT8, 0, uint8);
	assertDataValueInAssociativeArray(result.value.associativeArray->values[4].value, 0, "uint8_1", DATA_TYPE_UINT8, 1, uint8);
	
	DataSerializationContext_dispose(context);
}

#define testFailure(ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	context = DataSerializationContext_create(); \
	PREAMBLE_CODE \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d (OK)", SERIALIZATION_ERROR_OK, context->status); \
	FAIL_CODE \
	TestCase_assert(context->status == ERROR_STATUS, "Expected %d but got %d (context->status)", ERROR_STATUS, context->status); \
	context->dispose(context); \
	\
	context = DataSerializationContext_create(); \
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

#define testFailureWithAllTypes(ERROR_STATUS, PREAMBLE_CODE, KEY) \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeBoolean(context, KEY, false);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeInt8(context, KEY, 0);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeUInt8(context, KEY, 0);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeInt16(context, KEY, 0);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeUInt16(context, KEY, 0);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeInt32(context, KEY, 0);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeUInt32(context, KEY, 0);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeInt64(context, KEY, 0);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeUInt64(context, KEY, 0);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeFloat(context, KEY, 0.0f);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeDouble(context, KEY, 0.0);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeEnumeration(context, KEY, 0, enumKV(ENUM_TEST_0), NULL);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeBitfield8(context, KEY, 0, "a", NULL);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeBitfield16(context, KEY, 0, "a", NULL);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeBitfield32(context, KEY, 0, "a", NULL);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeBitfield64(context, KEY, 0, "a", NULL);); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeString(context, KEY, "a");); \
	testFailure(ERROR_STATUS, PREAMBLE_CODE, context->writeBlob(context, KEY, "a", 1);)

static void testErrors() {
	DataSerializationContext * context;
	jmp_buf jmpBuf;
	int status;
	
	//SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginArray(context, NULL);,
	            context->endStructure(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginArray(context, NULL);
	            context->beginArray(context, NULL);,
	            context->endStructure(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginArray(context, NULL);,
	            context->endDictionary(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginArray(context, NULL);
	            context->beginArray(context, NULL);,
	            context->endDictionary(context););
	
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginDictionary(context, NULL);,
	            context->endStructure(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginDictionary(context, NULL);
	            context->beginDictionary(context, "a");,
	            context->endStructure(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginDictionary(context, NULL);,
	            context->endArray(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginDictionary(context, NULL);
	            context->beginDictionary(context, "a");,
	            context->endArray(context););
	
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginStructure(context, NULL);,
	            context->endDictionary(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginStructure(context, NULL);
	            context->beginStructure(context, "a");,
	            context->endDictionary(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginStructure(context, NULL);,
	            context->endArray(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	            context->beginStructure(context, NULL);
	            context->beginStructure(context, "a");,
	            context->endArray(context););
	
	//SERIALIZATION_ERROR_CONTAINER_UNDERFLOW
	testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            ,
	            context->endArray(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            context->beginArray(context, NULL);
	            context->endArray(context);,
	            context->endArray(context););
	
	testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            ,
	            context->endDictionary(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            context->beginDictionary(context, NULL);
	            context->endDictionary(context);,
	            context->endDictionary(context););
	
	testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            ,
	            context->endStructure(context););
	testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	            context->beginStructure(context, NULL);
	            context->endStructure(context);,
	            context->endStructure(context););
	
	//SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS
	testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginArray(context, NULL);
	            context->endArray(context);,
	            context->beginArray(context, NULL););
	testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginArray(context, NULL);
	            context->endArray(context);,
	            context->beginStructure(context, NULL););
	testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginArray(context, NULL);
	            context->endArray(context);,
	            context->beginDictionary(context, NULL););
	
	testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginDictionary(context, NULL);
	            context->endDictionary(context);,
	            context->beginDictionary(context, NULL););
	testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginDictionary(context, NULL);
	            context->endDictionary(context);,
	            context->beginArray(context, NULL););
	testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginDictionary(context, NULL);
	            context->endDictionary(context);,
	            context->beginStructure(context, NULL););
	
	testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginStructure(context, NULL);
	            context->endStructure(context);,
	            context->beginStructure(context, NULL););
	testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginStructure(context, NULL);
	            context->endStructure(context);,
	            context->beginArray(context, NULL););
	testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	            context->beginStructure(context, NULL);
	            context->endStructure(context);,
	            context->beginDictionary(context, NULL););
	
	//SERIALIZATION_ERROR_UNNAMED_BIT
	testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	            context->beginArray(context, NULL);,
	            context->writeBitfield8(context, NULL, 12, "a", NULL););
	testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	            context->beginArray(context, NULL);,
	            context->writeBitfield16(context, NULL, 12, "a", NULL););
	testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	            context->beginArray(context, NULL);,
	            context->writeBitfield32(context, NULL, 12, "a", NULL););
	testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	            context->beginArray(context, NULL);,
	            context->writeBitfield64(context, NULL, 12, "a", NULL););
	
	//SERIALIZATION_ERROR_DUPLICATE_BIT
	testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	            context->beginArray(context, NULL);,
	            context->writeBitfield8(context, NULL, 1, "a", "a", NULL););
	testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	            context->beginArray(context, NULL);,
	            context->writeBitfield16(context, NULL, 1, "a", "a", NULL););
	testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	            context->beginArray(context, NULL);,
	            context->writeBitfield32(context, NULL, 1, "a", "a", NULL););
	testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	            context->beginArray(context, NULL);,
	            context->writeBitfield64(context, NULL, 1, "a", "a", NULL););
	
	//SERIALIZATION_ERROR_ENUM_NOT_NAMED
	testFailure(SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	            context->beginArray(context, NULL);,
	            context->writeEnumeration(context, NULL, 1, NULL););
	testFailure(SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	            context->beginArray(context, NULL);,
	            context->writeEnumeration(context, NULL, 1, "a", 0, NULL););
	
	//SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME
	testFailure(SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME,
	            context->beginArray(context, NULL);,
	            context->writeEnumeration(context, NULL, 0, "a", 0, "a", 1, NULL););
	
	//SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE
	testFailure(SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE,
	            context->beginArray(context, NULL);,
	            context->writeEnumeration(context, NULL, 0, "a", 0, "b", 0, NULL););
	
	//SERIALIZATION_ERROR_NULL_KEY
	testFailureWithAllTypes(SERIALIZATION_ERROR_NULL_KEY,
	                        context->beginDictionary(context, NULL);,
	                        NULL);
	testFailureWithAllTypes(SERIALIZATION_ERROR_NULL_KEY,
	                        context->beginStructure(context, NULL);,
	                        NULL);
	
	//SERIALIZATION_ERROR_NO_CONTAINER_STARTED
	testFailureWithAllTypes(SERIALIZATION_ERROR_NO_CONTAINER_STARTED,
	                        ,
	                        NULL);
	
	//SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER
	testFailure(SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER,
	            ,
	            DataSerializationContext_result(context););
}

TEST_SUITE(DataSerializationContextTest,
           testInit,
           testValuesInArray,
           testValuesInHashTable,
           testValuesInAssociativeArray,
           testHierarchy,
           testErrors)
