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
	verifyFunctionPointer(context, writeString); \
	verifyFunctionPointer(context, writeBoolean); \
	verifyFunctionPointer(context, writeEnumeration); \
	verifyFunctionPointer(context, writeBitfield8); \
	verifyFunctionPointer(context, writeBitfield16); \
	verifyFunctionPointer(context, writeBitfield32); \
	verifyFunctionPointer(context, writeBitfield64)

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

#define assertDataValueInArray(INDEX, TYPE, EXPECTED_VALUE, FIELD) \
	TestCase_assert(result.value.array->values[INDEX].type == TYPE, "Expected %d but got %d", TYPE, result.value.array->values[INDEX].type); \
	TestCase_assert(result.value.array->values[INDEX].value.FIELD == EXPECTED_VALUE, "Expected " #EXPECTED_VALUE " but got %d", result.value.array->values[INDEX].value.FIELD)

static void testNumericValuesInArray() {
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
	context->endArray(context);
	
	result = DataSerializationContext_result(context);
	TestCase_assert(result.type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, result.type);
	TestCase_assert(result.value.array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.array->count == 22, "Expected 22 but got " SIZE_T_FORMAT, result.value.array->count);
	assertDataValueInArray(0, DATA_TYPE_BOOLEAN, false, boolean);
	assertDataValueInArray(1, DATA_TYPE_BOOLEAN, true, boolean);
	assertDataValueInArray(2, DATA_TYPE_INT8, 0, int8);
	assertDataValueInArray(3, DATA_TYPE_INT8, 1, int8);
	assertDataValueInArray(4, DATA_TYPE_UINT8, 0, uint8);
	assertDataValueInArray(5, DATA_TYPE_UINT8, 1, uint8);
	assertDataValueInArray(6, DATA_TYPE_INT16, 0, int16);
	assertDataValueInArray(7, DATA_TYPE_INT16, 1, int16);
	assertDataValueInArray(8, DATA_TYPE_UINT16, 0, uint16);
	assertDataValueInArray(9, DATA_TYPE_UINT16, 1, uint16);
	assertDataValueInArray(10, DATA_TYPE_INT32, 0, int32);
	assertDataValueInArray(11, DATA_TYPE_INT32, 1, int32);
	assertDataValueInArray(12, DATA_TYPE_UINT32, 0, uint32);
	assertDataValueInArray(13, DATA_TYPE_UINT32, 1, uint32);
	assertDataValueInArray(14, DATA_TYPE_INT64, 0, int64);
	assertDataValueInArray(15, DATA_TYPE_INT64, 1, int64);
	assertDataValueInArray(16, DATA_TYPE_UINT64, 0, uint64);
	assertDataValueInArray(17, DATA_TYPE_UINT64, 1, uint64);
	assertDataValueInArray(18, DATA_TYPE_FLOAT, 0, float32);
	assertDataValueInArray(19, DATA_TYPE_FLOAT, 1, float32);
	assertDataValueInArray(20, DATA_TYPE_DOUBLE, 0, float64);
	assertDataValueInArray(21, DATA_TYPE_DOUBLE, 1, float64);
}

#define assertDataValueInHashTable(KEY, TYPE, EXPECTED_VALUE, FIELD) \
	value = hashGet(result.value.hashTable, KEY); \
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL"); \
	TestCase_assert(value->type == TYPE, "Expected %d but got %d", TYPE, value->type); \
	TestCase_assert(value->value.FIELD == EXPECTED_VALUE, "Expected " #EXPECTED_VALUE " but got %d", value->value.FIELD)

static void testNumericValuesInHashTable() {
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
	context->endStructure(context);
	
	result = DataSerializationContext_result(context);
	TestCase_assert(result.type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, result.type);
	TestCase_assert(result.value.hashTable != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.hashTable->keyCount == 22, "Expected 22 but got " SIZE_T_FORMAT, result.value.hashTable->keyCount);
	assertDataValueInHashTable("bool_0", DATA_TYPE_BOOLEAN, false, boolean);
	assertDataValueInHashTable("bool_1", DATA_TYPE_BOOLEAN, true, boolean);
	assertDataValueInHashTable("int8_0", DATA_TYPE_INT8, 0, int8);
	assertDataValueInHashTable("int8_1", DATA_TYPE_INT8, 1, int8);
	assertDataValueInHashTable("uint8_0", DATA_TYPE_UINT8, 0, uint8);
	assertDataValueInHashTable("uint8_1", DATA_TYPE_UINT8, 1, uint8);
	assertDataValueInHashTable("int16_0", DATA_TYPE_INT16, 0, int16);
	assertDataValueInHashTable("int16_1", DATA_TYPE_INT16, 1, int16);
	assertDataValueInHashTable("uint16_0", DATA_TYPE_UINT16, 0, uint16);
	assertDataValueInHashTable("uint16_1", DATA_TYPE_UINT16, 1, uint16);
	assertDataValueInHashTable("int32_0", DATA_TYPE_INT32, 0, int32);
	assertDataValueInHashTable("int32_1", DATA_TYPE_INT32, 1, int32);
	assertDataValueInHashTable("uint32_0", DATA_TYPE_UINT32, 0, uint32);
	assertDataValueInHashTable("uint32_1", DATA_TYPE_UINT32, 1, uint32);
	assertDataValueInHashTable("int64_0", DATA_TYPE_INT64, 0, int64);
	assertDataValueInHashTable("int64_1", DATA_TYPE_INT64, 1, int64);
	assertDataValueInHashTable("uint64_0", DATA_TYPE_UINT64, 0, uint64);
	assertDataValueInHashTable("uint64_1", DATA_TYPE_UINT64, 1, uint64);
	assertDataValueInHashTable("float_0", DATA_TYPE_FLOAT, 0, float32);
	assertDataValueInHashTable("float_1", DATA_TYPE_FLOAT, 1, float32);
	assertDataValueInHashTable("double_0", DATA_TYPE_DOUBLE, 0, float64);
	assertDataValueInHashTable("double_1", DATA_TYPE_DOUBLE, 1, float64);
}

#define assertDataValueInAssociativeArray(INDEX, KEY, TYPE, EXPECTED_VALUE, FIELD) \
	TestCase_assert(result.value.associativeArray->values[INDEX].type == TYPE, "Expected %d but got %d", TYPE, result.value.associativeArray->values[INDEX].type); \
	TestCase_assert(result.value.associativeArray->values[INDEX].value.FIELD == EXPECTED_VALUE, "Expected " #EXPECTED_VALUE " but got %d", result.value.associativeArray->values[INDEX].value.FIELD); \
	TestCase_assert(!strcmp(result.value.associativeArray->keys[INDEX], KEY), "Expected \"%s\" but got \"%s\"", KEY, result.value.associativeArray->keys[INDEX])

static void testNumericValuesInAssociativeArray() {
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
	context->endDictionary(context);
	
	result = DataSerializationContext_result(context);
	TestCase_assert(result.type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, result.type);
	TestCase_assert(result.value.associativeArray != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(result.value.associativeArray->count == 22, "Expected 22 but got " SIZE_T_FORMAT, result.value.associativeArray->count);
	assertDataValueInAssociativeArray(0, "bool_0", DATA_TYPE_BOOLEAN, false, boolean);
	assertDataValueInAssociativeArray(1, "bool_1", DATA_TYPE_BOOLEAN, true, boolean);
	assertDataValueInAssociativeArray(2, "int8_0", DATA_TYPE_INT8, 0, int8);
	assertDataValueInAssociativeArray(3, "int8_1", DATA_TYPE_INT8, 1, int8);
	assertDataValueInAssociativeArray(4, "uint8_0", DATA_TYPE_UINT8, 0, uint8);
	assertDataValueInAssociativeArray(5, "uint8_1", DATA_TYPE_UINT8, 1, uint8);
	assertDataValueInAssociativeArray(6, "int16_0", DATA_TYPE_INT16, 0, int16);
	assertDataValueInAssociativeArray(7, "int16_1", DATA_TYPE_INT16, 1, int16);
	assertDataValueInAssociativeArray(8, "uint16_0", DATA_TYPE_UINT16, 0, uint16);
	assertDataValueInAssociativeArray(9, "uint16_1", DATA_TYPE_UINT16, 1, uint16);
	assertDataValueInAssociativeArray(10, "int32_0", DATA_TYPE_INT32, 0, int32);
	assertDataValueInAssociativeArray(11, "int32_1", DATA_TYPE_INT32, 1, int32);
	assertDataValueInAssociativeArray(12, "uint32_0", DATA_TYPE_UINT32, 0, uint32);
	assertDataValueInAssociativeArray(13, "uint32_1", DATA_TYPE_UINT32, 1, uint32);
	assertDataValueInAssociativeArray(14, "int64_0", DATA_TYPE_INT64, 0, int64);
	assertDataValueInAssociativeArray(15, "int64_1", DATA_TYPE_INT64, 1, int64);
	assertDataValueInAssociativeArray(16, "uint64_0", DATA_TYPE_UINT64, 0, uint64);
	assertDataValueInAssociativeArray(17, "uint64_1", DATA_TYPE_UINT64, 1, uint64);
	assertDataValueInAssociativeArray(18, "float_0", DATA_TYPE_FLOAT, 0, float32);
	assertDataValueInAssociativeArray(19, "float_1", DATA_TYPE_FLOAT, 1, float32);
	assertDataValueInAssociativeArray(20, "double_0", DATA_TYPE_DOUBLE, 0, float64);
	assertDataValueInAssociativeArray(21, "double_1", DATA_TYPE_DOUBLE, 1, float64);
}

TEST_SUITE(DataSerializationContextTest,
           testInit,
           testNumericValuesInArray,
           testNumericValuesInHashTable,
           testNumericValuesInAssociativeArray)
