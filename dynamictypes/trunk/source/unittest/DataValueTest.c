#include "dynamictypes/AssociativeArray.h"
#include "dynamictypes/DataArray.h"
#include "dynamictypes/DataValue.h"
#include "dynamictypes/HashTable.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"
#include <float.h>

static void testCreate() {
	DataValue value;
	char * string1 = "abc", * string2 = "foo";
	char blob1[] = {0x00, 0x01}, blob2[] = {0x03, 0x05, 0x07};
	HashTable * hashTable1, * hashTable2;
	DataArray * array1, * array2;
	AssociativeArray * assArray1, * assArray2;
	
	value = valueCreateBoolean(false);
	TestCase_assert(value.type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value.type);
	TestCase_assert(!value.value.boolean, "Expected false but got true");
	valueDispose(&value);
	value = valueCreateBoolean(true);
	TestCase_assert(value.type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value.type);
	TestCase_assert(value.value.boolean, "Expected true but got false");
	valueDispose(&value);
	
	value = valueCreateInt8(0);
	TestCase_assert(value.type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value.type);
	TestCase_assert(value.value.int8 == 0, "Expected 0 but got %d", value.value.int8);
	valueDispose(&value);
	value = valueCreateInt8(1);
	TestCase_assert(value.type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value.type);
	TestCase_assert(value.value.int8 == 1, "Expected 1 but got %d", value.value.int8);
	valueDispose(&value);
	
	value = valueCreateUInt8(0);
	TestCase_assert(value.type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, value.type);
	TestCase_assert(value.value.uint8 == 0, "Expected 0 but got %d", value.value.uint8);
	valueDispose(&value);
	value = valueCreateUInt8(1);
	TestCase_assert(value.type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, value.type);
	TestCase_assert(value.value.uint8 == 1, "Expected 1 but got %d", value.value.uint8);
	valueDispose(&value);
	
	value = valueCreateInt16(0);
	TestCase_assert(value.type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, value.type);
	TestCase_assert(value.value.int16 == 0, "Expected 0 but got %d", value.value.int16);
	valueDispose(&value);
	value = valueCreateInt16(1);
	TestCase_assert(value.type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, value.type);
	TestCase_assert(value.value.int16 == 1, "Expected 1 but got %d", value.value.int16);
	valueDispose(&value);
	
	value = valueCreateUInt16(0);
	TestCase_assert(value.type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, value.type);
	TestCase_assert(value.value.uint16 == 0, "Expected 0 but got %d", value.value.uint16);
	valueDispose(&value);
	value = valueCreateUInt16(1);
	TestCase_assert(value.type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, value.type);
	TestCase_assert(value.value.uint16 == 1, "Expected 1 but got %d", value.value.uint16);
	valueDispose(&value);
	
	value = valueCreateInt32(0);
	TestCase_assert(value.type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, value.type);
	TestCase_assert(value.value.int32 == 0, "Expected 0 but got %d", value.value.int32);
	valueDispose(&value);
	value = valueCreateInt32(1);
	TestCase_assert(value.type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, value.type);
	TestCase_assert(value.value.int32 == 1, "Expected 1 but got %d", value.value.int32);
	valueDispose(&value);
	
	value = valueCreateUInt32(0);
	TestCase_assert(value.type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, value.type);
	TestCase_assert(value.value.uint32 == 0, "Expected 0 but got %d", value.value.uint32);
	valueDispose(&value);
	value = valueCreateUInt32(1);
	TestCase_assert(value.type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, value.type);
	TestCase_assert(value.value.uint32 == 1, "Expected 1 but got %d", value.value.uint32);
	valueDispose(&value);
	
	value = valueCreateInt64(0);
	TestCase_assert(value.type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, value.type);
	TestCase_assert(value.value.int64 == 0, "Expected 0 but got " INT64_FORMAT, value.value.int64);
	valueDispose(&value);
	value = valueCreateInt64(1);
	TestCase_assert(value.type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, value.type);
	TestCase_assert(value.value.int64 == 1, "Expected 1 but got " INT64_FORMAT, value.value.int64);
	valueDispose(&value);
	
	value = valueCreateUInt64(0);
	TestCase_assert(value.type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, value.type);
	TestCase_assert(value.value.uint64 == 0, "Expected 0 but got " UINT64_FORMAT, value.value.uint64);
	valueDispose(&value);
	value = valueCreateUInt64(1);
	TestCase_assert(value.type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, value.type);
	TestCase_assert(value.value.uint64 == 1, "Expected 1 but got " UINT64_FORMAT, value.value.uint64);
	valueDispose(&value);
	
	value = valueCreateFloat(0.0f);
	TestCase_assert(value.type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, value.type);
	TestCase_assert(value.value.float32 == 0.0f, "Expected 0.0 but got %f", value.value.float32);
	valueDispose(&value);
	value = valueCreateFloat(1.0f);
	TestCase_assert(value.type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, value.type);
	TestCase_assert(value.value.float32 == 1.0f, "Expected 1.0 but got %f", value.value.float32);
	valueDispose(&value);
	
	value = valueCreateDouble(0.0);
	TestCase_assert(value.type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, value.type);
	TestCase_assert(value.value.float64 == 0.0, "Expected 0.0 but got %f", value.value.float64);
	valueDispose(&value);
	value = valueCreateDouble(1.0);
	TestCase_assert(value.type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, value.type);
	TestCase_assert(value.value.float64 == 1.0, "Expected 1.0 but got %f", value.value.float64);
	valueDispose(&value);
	
	value = valueCreateFixed16_16(0x00000);
	TestCase_assert(value.type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, value.type);
	TestCase_assert(value.value.fixed == 0x00000, "Expected 0x00000 but got 0x%05X", value.value.fixed);
	valueDispose(&value);
	value = valueCreateFixed16_16(0x10000);
	TestCase_assert(value.type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, value.type);
	TestCase_assert(value.value.fixed == 0x10000, "Expected 0x10000 but got 0x%05X", value.value.fixed);
	valueDispose(&value);
	
	value = valueCreatePointer((void *) 0x0);
	TestCase_assert(value.type == DATA_TYPE_POINTER, "Expected %d but got %d", DATA_TYPE_POINTER, value.type);
	TestCase_assert(value.value.pointer == (void *) 0x0, "Expected 0x0 but got %p", value.value.pointer);
	valueDispose(&value);
	value = valueCreatePointer((void *) 0x1);
	TestCase_assert(value.type == DATA_TYPE_POINTER, "Expected %d but got %d", DATA_TYPE_POINTER, value.type);
	TestCase_assert(value.value.pointer == (void *) 0x1, "Expected 0x1 but got %p", value.value.pointer);
	valueDispose(&value);
	
	value = valueCreateString(string1, 2, true, true);
	TestCase_assert(value.type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, value.type);
	TestCase_assert(!strcmp(value.value.string, "ab"), "Expected \"ab\" but got \"%s\"", value.value.string);
	TestCase_assert(value.value.string != string1, "Expected differing pointers, but both are %p", string1);
	valueDispose(&value);
	value = valueCreateString(string2, DATA_USE_STRLEN, false, false);
	TestCase_assert(value.type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, value.type);
	TestCase_assert(value.value.string == string2, "Expected %p but got %p", string2, value.value.string);
	valueDispose(&value);
	
	value = valueCreateBlob(blob1, sizeof(blob1), true, true);
	TestCase_assert(value.type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, value.type);
	TestCase_assert(value.value.blob.length == 2, "Expected 2 but got " SIZE_T_FORMAT, value.value.blob.length);
	TestCase_assert(!memcmp(value.value.blob.bytes, blob1, sizeof(blob1)), "Expected {0x%02X, 0x%02X} but got {0x%02X, 0x%02X}", blob1[0], blob1[1], ((char *) value.value.blob.bytes)[0], ((char *) value.value.blob.bytes)[1]);
	TestCase_assert(value.value.blob.bytes != blob1, "Expected differing pointers, but both are %p", blob1);
	valueDispose(&value);
	value = valueCreateBlob(blob2, sizeof(blob2), false, false);
	TestCase_assert(value.type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, value.type);
	TestCase_assert(value.value.blob.length == 3, "Expected 3 but got " SIZE_T_FORMAT, value.value.blob.length);
	TestCase_assert(value.value.blob.bytes == blob2, "Expected %p but got %p", blob2, value.value.blob.bytes);
	valueDispose(&value);
	
	hashTable1 = hashCreate();
	hashSet(hashTable1, "a", valueCreateBoolean(false));
	hashTable2 = hashCreate();
	value = valueCreateHashTable(hashTable1, true, true);
	TestCase_assert(value.type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, value.type);
	TestCase_assert(value.value.hashTable != hashTable1, "Expected differing pointers, but both are %p", hashTable1);
	TestCase_assert(value.value.hashTable->count == hashTable1->count, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, hashTable1->count, value.value.hashTable->count);
	TestCase_assert(hashGet(value.value.hashTable, "a") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashGet(value.value.hashTable, "a")->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, hashGet(value.value.hashTable, "a")->type);
	TestCase_assert(!hashGet(value.value.hashTable, "a")->value.boolean, "Expected false but got true");
	valueDispose(&value);
	value = valueCreateHashTable(hashTable2, true, false);
	TestCase_assert(value.type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, value.type);
	TestCase_assert(value.value.hashTable == hashTable2, "Expected %p but got %p", hashTable2, value.value.hashTable);
	valueDispose(&value);
	
	array1 = arrayCreate();
	arrayAppend(array1, valueCreateBoolean(false));
	array2 = arrayCreate();
	value = valueCreateArray(array1, true, true);
	TestCase_assert(value.type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, value.type);
	TestCase_assert(value.value.array != array1, "Expected differing pointers, but both are %p", array1);
	TestCase_assert(value.value.array->count == array1->count, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, array1->count, value.value.array->count);
	TestCase_assert(arrayGet(value.value.array, 0) != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(arrayGet(value.value.array, 0)->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, arrayGet(value.value.array, 0)->type);
	TestCase_assert(!arrayGet(value.value.array, 0)->value.boolean, "Expected false but got true");
	valueDispose(&value);
	value = valueCreateArray(array2, true, false);
	TestCase_assert(value.type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, value.type);
	TestCase_assert(value.value.array == array2, "Expected %p but got %p", array2, value.value.array);
	valueDispose(&value);
	
	assArray1 = associativeArrayCreate();
	associativeArrayAppend(assArray1, "a", valueCreateBoolean(false));
	assArray2 = associativeArrayCreate();
	value = valueCreateAssociativeArray(assArray1, true, true);
	TestCase_assert(value.type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, value.type);
	TestCase_assert(value.value.associativeArray != assArray1, "Expected differing pointers, but both are %p", assArray1);
	TestCase_assert(value.value.associativeArray->count == assArray1->count, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, assArray1->count, value.value.associativeArray->count);
	TestCase_assert(associativeArrayGetValueAtIndex(value.value.associativeArray, 0) != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(associativeArrayGetValueAtIndex(value.value.associativeArray, 0)->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, associativeArrayGetValueAtIndex(value.value.associativeArray, 0)->type);
	TestCase_assert(!associativeArrayGetValueAtIndex(value.value.associativeArray, 0)->value.boolean, "Expected false but got true");
	TestCase_assert(!strcmp(associativeArrayGetKeyAtIndex(value.value.associativeArray, 0), "a"), "Expected \"a\" but got \"%s\"", associativeArrayGetKeyAtIndex(value.value.associativeArray, 0));
	valueDispose(&value);
	value = valueCreateAssociativeArray(assArray2, true, false);
	TestCase_assert(value.type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, value.type);
	TestCase_assert(value.value.associativeArray == assArray2, "Expected %p but got %p", assArray2, value.value.associativeArray);
	valueDispose(&value);
}

static void testGet() {
	DataValue value;
	bool boolean;
	int8_t int8;
	uint8_t uint8;
	int16_t int16;
	uint16_t uint16;
	int32_t int32;
	uint32_t uint32;
	int64_t int64;
	uint64_t uint64;
	float float32;
	double float64;
	fixed16_16 fixed;
	void * pointer;
	const char * string;
	size_t length;
	const void * blob;
	HashTable * hashTable, * hashTableResult;
	DataArray * array, * arrayResult;
	AssociativeArray * assArray, * assArrayResult;
	
	value = valueCreateBoolean(false);
	boolean = valueGetBoolean(&value);
	TestCase_assert(!boolean, "Expected false but got true");
	valueDispose(&value);
	value = valueCreateBoolean(true);
	boolean = valueGetBoolean(&value);
	TestCase_assert(boolean, "Expected true but got false");
	valueDispose(&value);
	
	value = valueCreateInt8(0);
	int8 = valueGetInt8(&value);
	TestCase_assert(int8 == 0, "Expected 0 but got %d", int8);
	valueDispose(&value);
	value = valueCreateInt8(INT8_MIN);
	int8 = valueGetInt8(&value);
	TestCase_assert(int8 == INT8_MIN, "Expected -128 but got %d", int8);
	valueDispose(&value);
	
	value = valueCreateUInt8(0);
	uint8 = valueGetUInt8(&value);
	TestCase_assert(uint8 == 0, "Expected 0 but got %u", uint8);
	valueDispose(&value);
	value = valueCreateUInt8(UINT8_MAX);
	uint8 = valueGetUInt8(&value);
	TestCase_assert(uint8 == UINT8_MAX, "Expected 255 but got %u", uint8);
	valueDispose(&value);
	
	value = valueCreateInt16(0);
	int16 = valueGetInt16(&value);
	TestCase_assert(int16 == 0, "Expected 0 but got %d", int16);
	valueDispose(&value);
	value = valueCreateInt16(INT16_MIN);
	int16 = valueGetInt16(&value);
	TestCase_assert(int16 == INT16_MIN, "Expected -32768 but got %d", int16);
	valueDispose(&value);
	
	value = valueCreateUInt16(0);
	uint16 = valueGetUInt16(&value);
	TestCase_assert(uint16 == 0, "Expected 0 but got %u", uint16);
	valueDispose(&value);
	value = valueCreateUInt16(UINT16_MAX);
	uint16 = valueGetUInt16(&value);
	TestCase_assert(uint16 == UINT16_MAX, "Expected 65535 but got %u", uint16);
	valueDispose(&value);
	
	value = valueCreateInt32(0);
	int32 = valueGetInt32(&value);
	TestCase_assert(int32 == 0, "Expected 0 but got %d", int32);
	valueDispose(&value);
	value = valueCreateInt32(INT32_MIN);
	int32 = valueGetInt32(&value);
	TestCase_assert(int32 == INT32_MIN, "Expected -214783648 but got %d", int32);
	valueDispose(&value);
	
	value = valueCreateUInt32(0);
	uint32 = valueGetUInt32(&value);
	TestCase_assert(uint32 == 0, "Expected 0 but got %u", uint32);
	valueDispose(&value);
	value = valueCreateUInt32(UINT32_MAX);
	uint32 = valueGetUInt32(&value);
	TestCase_assert(uint32 == UINT32_MAX, "Expected 4294967295 but got %u", uint32);
	valueDispose(&value);
	
	value = valueCreateInt64(0);
	int64 = valueGetInt64(&value);
	TestCase_assert(int64 == 0, "Expected 0 but got " INT64_FORMAT, int64);
	valueDispose(&value);
	value = valueCreateInt64(INT64_MIN);
	int64 = valueGetInt64(&value);
	TestCase_assert(int64 == INT64_MIN, "Expected -9223372036854775808 but got " INT64_FORMAT, int64);
	valueDispose(&value);
	
	value = valueCreateUInt64(0);
	uint64 = valueGetUInt64(&value);
	TestCase_assert(uint64 == 0, "Expected 0 but got " UINT64_FORMAT, uint64);
	valueDispose(&value);
	value = valueCreateUInt64(UINT64_MAX);
	uint64 = valueGetUInt64(&value);
	TestCase_assert(uint64 == UINT64_MAX, "Expected 18446744073709551615 but got " UINT64_FORMAT, uint64);
	valueDispose(&value);
	
	value = valueCreateFloat(0.0f);
	float32 = valueGetFloat(&value);
	TestCase_assert(float32 == 0.0f, "Expected 0.0 but got %f", float32);
	valueDispose(&value);
	value = valueCreateFloat(FLT_MAX);
	float32 = valueGetFloat(&value);
	TestCase_assert(float32 == FLT_MAX, "Expected %f but got %f", FLT_MAX, float32);
	valueDispose(&value);
	
	value = valueCreateDouble(0.0);
	float64 = valueGetDouble(&value);
	TestCase_assert(float64 == 0.0, "Expected 0.0 but got %f", float64);
	valueDispose(&value);
	value = valueCreateDouble(DBL_MAX);
	float64 = valueGetDouble(&value);
	TestCase_assert(float64 == DBL_MAX, "Expected %f but got %f", DBL_MAX, float64);
	valueDispose(&value);
	
	value = valueCreateFixed16_16(0x00000);
	fixed = valueGetFixed16_16(&value);
	TestCase_assert(fixed == 0x00000, "Expected 0x00000 but got 0x%05X", fixed);
	valueDispose(&value);
	value = valueCreateFixed16_16(FIXED_16_16_MIN);
	fixed = valueGetFixed16_16(&value);
	TestCase_assert(fixed == FIXED_16_16_MIN, "Expected 0x%05X but got 0x%05X", FIXED_16_16_MIN, fixed);
	valueDispose(&value);
	
	value = valueCreatePointer(NULL);
	pointer = valueGetPointer(&value);
	TestCase_assert(pointer == NULL, "Expected NULL but got %p", pointer);
	valueDispose(&value);
	value = valueCreatePointer((void *) 0xFFFFFFFF);
	pointer = valueGetPointer(&value);
	TestCase_assert(pointer == (void *) 0xFFFFFFFF, "Expected 0xffffffff but got %p", pointer);
	valueDispose(&value);
	
	value = valueCreateString("", DATA_USE_STRLEN, false, false);
	string = valueGetString(&value);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(string, ""), "Expected \"\" but got \"%s\"", string);
	valueDispose(&value);
	value = valueCreateString("abc", DATA_USE_STRLEN, false, false);
	string = valueGetString(&value);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(string, "abc"), "Expected \"abc\" but got \"%s\"", string);
	valueDispose(&value);
	
	value = valueCreateBlob("", 0, false, false);
	blob = valueGetBlob(&value, &length);
	TestCase_assert(blob != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 0, "Expected 0 but got " SIZE_T_FORMAT, length);
	valueDispose(&value);
	value = valueCreateBlob("\x00\xFF", 2, false, false);
	blob = valueGetBlob(&value, &length);
	TestCase_assert(blob != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 2, "Expected 2 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!memcmp(blob, "\x00\xFF", 2), "Expected {0x00, 0xFF} but got {0x%02X, 0x%02X}", ((char *) blob)[0], ((char *) blob)[1]);
	valueDispose(&value);
	
	value = valueCreateHashTable(hashTable = hashCreate(), true, false);
	hashTableResult = valueGetHashTable(&value);
	TestCase_assert(hashTableResult == hashTable, "Expected %p but got %p", hashTable, hashTableResult);
	valueDispose(&value);
	
	value = valueCreateArray(array = arrayCreate(), true, false);
	arrayResult = valueGetArray(&value);
	TestCase_assert(arrayResult == array, "Expected %p but got %p", array, arrayResult);
	valueDispose(&value);
	
	value = valueCreateAssociativeArray(assArray = associativeArrayCreate(), true, false);
	assArrayResult = valueGetAssociativeArray(&value);
	TestCase_assert(assArrayResult == assArray, "Expected %p but got %p", assArray, assArrayResult);
	valueDispose(&value);
}

static void testCopy() {
	DataValue value, copy;
	HashTable * hashTable;
	DataArray * array;
	AssociativeArray * assArray;
	
	value = valueCreateBoolean(false);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, copy.type);
	TestCase_assert(!copy.value.boolean, "Expected false but got true");
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateBoolean(true);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, copy.type);
	TestCase_assert(copy.value.boolean, "Expected true but got false");
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateInt8(0);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, copy.type);
	TestCase_assert(copy.value.int8 == 0, "Expected 0 but got %d", copy.value.int8);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateInt8(INT8_MIN);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, copy.type);
	TestCase_assert(copy.value.int8 == INT8_MIN, "Expected -128 but got %d", copy.value.int8);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateUInt8(0);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, copy.type);
	TestCase_assert(copy.value.uint8 == 0, "Expected 0 but got %u", copy.value.uint8);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateUInt8(UINT8_MAX);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, copy.type);
	TestCase_assert(copy.value.uint8 == UINT8_MAX, "Expected 255 but got %u", copy.value.uint8);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateInt16(0);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, copy.type);
	TestCase_assert(copy.value.int16 == 0, "Expected 0 but got %d", copy.value.int16);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateInt16(INT16_MIN);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, copy.type);
	TestCase_assert(copy.value.int16 == INT16_MIN, "Expected -32768 but got %d", copy.value.int16);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateUInt16(0);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, copy.type);
	TestCase_assert(copy.value.uint16 == 0, "Expected 0 but got %u", copy.value.uint16);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateUInt16(UINT16_MAX);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, copy.type);
	TestCase_assert(copy.value.uint16 == UINT16_MAX, "Expected 65535 but got %u", copy.value.uint16);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateInt32(0);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, copy.type);
	TestCase_assert(copy.value.int32 == 0, "Expected 0 but got %d", copy.value.int32);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateInt32(INT32_MIN);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, copy.type);
	TestCase_assert(copy.value.int32 == INT32_MIN, "Expected -214783648 but got %d", copy.value.int32);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateUInt32(0);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, copy.type);
	TestCase_assert(copy.value.uint32 == 0, "Expected 0 but got %u", copy.value.uint32);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateUInt32(UINT32_MAX);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, copy.type);
	TestCase_assert(copy.value.uint32 == UINT32_MAX, "Expected 4294967295 but got %u", copy.value.uint32);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateInt64(0);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, copy.type);
	TestCase_assert(copy.value.int64 == 0, "Expected 0 but got " INT64_FORMAT, copy.value.int64);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateInt64(INT64_MIN);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, copy.type);
	TestCase_assert(copy.value.int64 == INT64_MIN, "Expected -9223372036854775808 but got " INT64_FORMAT, copy.value.int64);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateUInt64(0);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, copy.type);
	TestCase_assert(copy.value.uint64 == 0, "Expected 0 but got " UINT64_FORMAT, copy.value.uint64);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateUInt64(UINT64_MAX);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, copy.type);
	TestCase_assert(copy.value.uint64 == UINT64_MAX, "Expected 18446744073709551615 but got " UINT64_FORMAT, copy.value.uint64);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateFloat(0.0f);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, copy.type);
	TestCase_assert(copy.value.float32 == 0.0f, "Expected 0.0 but got %f", copy.value.float32);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateFloat(FLT_MAX);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, copy.type);
	TestCase_assert(copy.value.float32 == FLT_MAX, "Expected %f but got %f", FLT_MAX, copy.value.float32);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateDouble(0.0);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, copy.type);
	TestCase_assert(copy.value.float64 == 0.0, "Expected 0.0 but got %f", copy.value.float64);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateDouble(DBL_MAX);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, copy.type);
	TestCase_assert(copy.value.float64 == DBL_MAX, "Expected %f but got %f", DBL_MAX, copy.value.float64);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateFixed16_16(0x00000);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, copy.type);
	TestCase_assert(copy.value.fixed == 0x00000, "Expected 0x00000 but got 0x%05X", copy.value.fixed);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateFixed16_16(FIXED_16_16_MIN);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, copy.type);
	TestCase_assert(copy.value.fixed == FIXED_16_16_MIN, "Expected 0x%05X but got 0x%05X", FIXED_16_16_MIN, copy.value.fixed);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreatePointer(NULL);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_POINTER, "Expected %d but got %d", DATA_TYPE_POINTER, copy.type);
	TestCase_assert(copy.value.pointer == NULL, "Expected NULL but got %p", copy.value.pointer);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreatePointer((void *) 0xFFFFFFFF);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_POINTER, "Expected %d but got %d", DATA_TYPE_POINTER, copy.type);
	TestCase_assert(copy.value.pointer == (void *) 0xFFFFFFFF, "Expected 0xffffffff but got %p", copy.value.pointer);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateString("", DATA_USE_STRLEN, false, false);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, copy.type);
	TestCase_assert(copy.value.string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(copy.value.string, ""), "Expected \"\" but got \"%s\"", copy.value.string);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateString("abc", DATA_USE_STRLEN, false, false);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, copy.type);
	TestCase_assert(copy.value.string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(copy.value.string, "abc"), "Expected \"abc\" but got \"%s\"", copy.value.string);
	TestCase_assert(copy.value.string == value.value.string, "Expected %p but got %p", value.value.string, copy.value.string);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateString("abc", DATA_USE_STRLEN, true, true);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, copy.type);
	TestCase_assert(copy.value.string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(copy.value.string, "abc"), "Expected \"abc\" but got \"%s\"", copy.value.string);
	TestCase_assert(copy.value.string != value.value.string, "Expected pointers to differ, but both are %p", copy.value.string);
	valueDispose(&value);
	valueDispose(&copy);
	
	value = valueCreateBlob("", 0, false, false);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, copy.type);
	TestCase_assert(copy.value.blob.bytes != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(copy.value.blob.length == 0, "Expected 0 but got " SIZE_T_FORMAT, copy.value.blob.length);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateBlob("\x00\xFF", 2, false, false);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, copy.type);
	TestCase_assert(copy.value.blob.bytes != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(copy.value.blob.length == 2, "Expected 2 but got " SIZE_T_FORMAT, copy.value.blob.length);
	TestCase_assert(!memcmp(copy.value.blob.bytes, "\x00\xFF", 2), "Expected {0x00, 0xFF} but got {0x%02X, 0x%02X}", ((char *) copy.value.blob.bytes)[0], ((char *) copy.value.blob.bytes)[1]);
	TestCase_assert(copy.value.blob.bytes == value.value.blob.bytes, "Expected %p but got %p", value.value.blob.bytes, copy.value.blob.bytes);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateBlob("\x00\xFF", 2, true, true);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, copy.type);
	TestCase_assert(copy.value.blob.bytes != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(copy.value.blob.length == 2, "Expected 2 but got " SIZE_T_FORMAT, copy.value.blob.length);
	TestCase_assert(!memcmp(copy.value.blob.bytes, "\x00\xFF", 2), "Expected {0x00, 0xFF} but got {0x%02X, 0x%02X}", ((char *) copy.value.blob.bytes)[0], ((char *) copy.value.blob.bytes)[1]);
	TestCase_assert(copy.value.blob.bytes != value.value.blob.bytes, "Expected pointers to differ, but both are %p", copy.value.blob.bytes);
	valueDispose(&value);
	valueDispose(&copy);
	
	hashTable = hashCreate();
	hashSet(hashTable, "a", valueCreateBoolean(false));
	value = valueCreateHashTable(hashTable, false, false);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, copy.type);
	TestCase_assert(copy.value.hashTable == value.value.hashTable, "Expected %p but got %p", value.value.hashTable, copy.value.hashTable);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateHashTable(hashTable, true, true);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, copy.type);
	TestCase_assert(copy.value.hashTable->count == 1, "Expected 1 but got " SIZE_T_FORMAT, copy.value.hashTable->count);
	TestCase_assert(copy.value.hashTable != value.value.hashTable, "Expected pointers to differ, but both are %p", copy.value.hashTable);
	valueDispose(&value);
	valueDispose(&copy);
	hashDispose(hashTable);
	
	array = arrayCreate();
	arrayAppend(array, valueCreateBoolean(false));
	value = valueCreateArray(array, false, false);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, copy.type);
	TestCase_assert(copy.value.array == value.value.array, "Expected %p but got %p", value.value.array, copy.value.array);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateArray(array, true, true);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, copy.type);
	TestCase_assert(copy.value.array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, copy.value.array->count);
	TestCase_assert(copy.value.array != value.value.array, "Expected pointers to differ, but both are %p", copy.value.array);
	valueDispose(&value);
	valueDispose(&copy);
	arrayDispose(array);
	
	assArray = associativeArrayCreate();
	associativeArrayAppend(assArray, "a", valueCreateBoolean(false));
	value = valueCreateAssociativeArray(assArray, false, false);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, copy.type);
	TestCase_assert(copy.value.associativeArray == value.value.associativeArray, "Expected %p but got %p", value.value.associativeArray, copy.value.associativeArray);
	valueDispose(&value);
	valueDispose(&copy);
	value = valueCreateAssociativeArray(assArray, true, true);
	copy = valueCopy(&value);
	TestCase_assert(copy.type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, copy.type);
	TestCase_assert(copy.value.associativeArray->count == 1, "Expected 1 but got " SIZE_T_FORMAT, copy.value.associativeArray->count);
	TestCase_assert(copy.value.associativeArray != value.value.associativeArray, "Expected pointers to differ, but both are %p", copy.value.associativeArray);
	valueDispose(&value);
	valueDispose(&copy);
	associativeArrayDispose(assArray);
}

#define getAllValues(value) \
	boolean = valueGetBoolean(&value); \
	int8 = valueGetInt8(&value); \
	uint8 = valueGetUInt8(&value); \
	int16 = valueGetInt16(&value); \
	uint16 = valueGetUInt16(&value); \
	int32 = valueGetInt32(&value); \
	uint32 = valueGetUInt32(&value); \
	int64 = valueGetInt64(&value); \
	uint64 = valueGetUInt64(&value); \
	float32 = valueGetFloat(&value); \
	float64 = valueGetDouble(&value); \
	fixed = valueGetFixed16_16(&value); \
	pointer = valueGetPointer(&value); \
	string = valueGetString(&value); \
	blob = valueGetBlob(&value, NULL); \
	hashTable = valueGetHashTable(&value); \
	array = valueGetArray(&value); \
	assArray = valueGetAssociativeArray(&value)

#define assertAllPrimitiveTypes(expectedBooleanValue, expectedIntegerValue, expectedFloatValue, expectedFixedValue) \
	TestCase_assert(boolean == expectedBooleanValue, "Expected %s but got %s (boolean)", expectedBooleanValue ? "true" : "false", boolean ? "true" : "false"); \
	TestCase_assert(int8 == expectedIntegerValue, "Expected %d but got %d (int8)", (int8_t) expectedIntegerValue, int8); \
	TestCase_assert(uint8 == expectedIntegerValue, "Expected %u but got %u (uint8)", (uint8_t) expectedIntegerValue, uint8); \
	TestCase_assert(int16 == expectedIntegerValue, "Expected %d but got %d (int16)", (int16_t) expectedIntegerValue, int16); \
	TestCase_assert(uint16 == expectedIntegerValue, "Expected %u but got %u (uint16)", (uint16_t) expectedIntegerValue, uint16); \
	TestCase_assert(int32 == expectedIntegerValue, "Expected %d but got %d (int32)", (int32_t) expectedIntegerValue, int32); \
	TestCase_assert(uint32 == expectedIntegerValue, "Expected %u but got %u (uint32)", (uint32_t) expectedIntegerValue, uint32); \
	TestCase_assert(int64 == expectedIntegerValue, "Expected " INT64_FORMAT " but got " INT64_FORMAT " (int64)", (int64_t) expectedIntegerValue, int64); \
	TestCase_assert(uint64 == expectedIntegerValue, "Expected " UINT64_FORMAT " but got " UINT64_FORMAT " (uint64)", (uint64_t) expectedIntegerValue, uint64); \
	TestCase_assert(float32 == expectedFloatValue, "Expected %f but got %f (float)", expectedFloatValue, float32); \
	TestCase_assert(float64 == expectedFloatValue, "Expected %f but got %f (double)", expectedFloatValue, float64); \
	TestCase_assert(fixed == expectedFixedValue, "Expected 0x%05X but got 0x%05X (fixed16_16)", expectedFixedValue, fixed)

#define assertAllPointerTypesSeparate(expectedPointerValue, expectedStringValue, expectedBlobValue, expectedHashTableValue, expectedArrayValue, expectedAssArrayValue) \
	TestCase_assert(pointer == (void *) expectedPointerValue, "Expected %p but got %p (pointer)", expectedPointerValue, pointer); \
	TestCase_assert(string == (void *) expectedStringValue, "Expected %p but got %p (string)", expectedStringValue, string); \
	TestCase_assert(blob == (void *) expectedBlobValue, "Expected %p but got %p (blob)", expectedBlobValue, blob); \
	TestCase_assert(hashTable == (void *) expectedHashTableValue, "Expected %p but got %p (hashTable)", expectedHashTableValue, hashTable); \
	TestCase_assert(array == (void *) expectedArrayValue, "Expected %p but got %p (array)", expectedArrayValue, array); \
	TestCase_assert(assArray == (void *) expectedAssArrayValue, "Expected %p but got %p (associativeArray)", expectedAssArrayValue, assArray)

#define assertAllPointerTypes(expectedValue) \
	assertAllPointerTypesSeparate(expectedValue, expectedValue, expectedValue, expectedValue, expectedValue, expectedValue)

#define assertAllTypes(expectedBooleanValue, expectedIntegerValue, expectedFloatValue, expectedFixedValue, expectedPointerValue) \
	assertAllPrimitiveTypes(expectedBooleanValue, expectedIntegerValue, expectedFloatValue, expectedFixedValue); \
	assertAllPointerTypes(expectedPointerValue)

static void testConversions() {
	DataValue value;
	bool boolean;
	int8_t int8;
	uint8_t uint8;
	int16_t int16;
	uint16_t uint16;
	int32_t int32;
	uint32_t uint32;
	int64_t int64;
	uint64_t uint64;
	float float32;
	double float64;
	fixed16_16 fixed;
	void * pointer;
	const char * string;
	const void * blob;
	HashTable * hashTable;
	DataArray * array;
	AssociativeArray * assArray;
	
	value = valueCreateBoolean(false);
	getAllValues(value);
	assertAllTypes(0, 0, 0.0, 0x00000, NULL);
	value = valueCreateBoolean(true);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	
	value = valueCreateInt8(0);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateInt8(1);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	
	value = valueCreateUInt8(0);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateUInt8(1);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	
	value = valueCreateInt16(0);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateInt16(1);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	
	value = valueCreateUInt16(0);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateUInt16(1);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	
	value = valueCreateInt32(0);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateInt32(1);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	
	value = valueCreateUInt32(0);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateUInt32(1);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	
	value = valueCreateInt64(0);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateInt64(1);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	
	value = valueCreateUInt64(0);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateUInt64(1);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	
	value = valueCreateFloat(0.0f);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateFloat(1.0f);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	value = valueCreateFloat(1.25f);
	getAllValues(value);
	assertAllTypes(true, 1, 1.25, 0x14000, NULL);
	
	value = valueCreateDouble(0.0);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateDouble(1.0);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	value = valueCreateDouble(1.25);
	getAllValues(value);
	assertAllTypes(true, 1, 1.25, 0x14000, NULL);
	
	value = valueCreateFixed16_16(0x00000);
	getAllValues(value);
	assertAllTypes(false, 0, 0.0, 0x00000, NULL);
	value = valueCreateFixed16_16(0x10000);
	getAllValues(value);
	assertAllTypes(true, 1, 1.0, 0x10000, NULL);
	value = valueCreateFixed16_16(0x14000);
	getAllValues(value);
	assertAllTypes(true, 1, 1.25, 0x14000, NULL);
	
	value = valueCreatePointer(NULL);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypes(NULL);
	value = valueCreatePointer((void *) 0x1);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypesSeparate((void *) 0x1, NULL, NULL, NULL, NULL, NULL);
	
	value = valueCreateString(NULL, 0, false, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypes(NULL);
	value = valueCreateString("abcd", 4, false, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypesSeparate(value.value.string, value.value.string, value.value.string, NULL, NULL, NULL);
	
	value = valueCreateBlob(NULL, 0, false, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypes(NULL);
	value = valueCreateBlob("abcd", 4, false, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypesSeparate(value.value.blob.bytes, NULL, value.value.blob.bytes, NULL, NULL, NULL);
	
	value = valueCreateHashTable(NULL, false, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypes(NULL);
	value = valueCreateHashTable(hashCreate(), true, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypesSeparate(value.value.hashTable, NULL, NULL, value.value.hashTable, NULL, NULL);
	valueDispose(&value);
	
	value = valueCreateArray(NULL, false, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypes(NULL);
	value = valueCreateArray(arrayCreate(), true, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypesSeparate(value.value.array, NULL, NULL, NULL, value.value.array, NULL);
	valueDispose(&value);
	
	value = valueCreateAssociativeArray(NULL, false, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypes(NULL);
	value = valueCreateAssociativeArray(associativeArrayCreate(), true, false);
	getAllValues(value);
	assertAllPrimitiveTypes(false, 0, 0.0, 0x00000);
	assertAllPointerTypesSeparate(value.value.associativeArray, NULL, NULL, NULL, NULL, value.value.associativeArray);
	valueDispose(&value);
}

static void testNullTolerance() {
	bool boolean;
	int8_t int8;
	uint8_t uint8;
	int16_t int16;
	uint16_t uint16;
	int32_t int32;
	uint32_t uint32;
	int64_t int64;
	uint64_t uint64;
	float float32;
	double float64;
	fixed16_16 fixed;
	void * pointer;
	const char * string;
	const void * blob;
	HashTable * hashTable;
	DataArray * array;
	AssociativeArray * assArray;
	
	boolean = valueGetBoolean(NULL);
	TestCase_assert(!boolean, "Expected false but got true");
	int8 = valueGetInt8(NULL);
	TestCase_assert(int8 == 0, "Expected 0 but got %d", int8);
	uint8 = valueGetUInt8(NULL);
	TestCase_assert(uint8 == 0, "Expected 0 but got %u", uint8);
	int16 = valueGetInt16(NULL);
	TestCase_assert(int16 == 0, "Expected 0 but got %d", int16);
	uint16 = valueGetUInt16(NULL);
	TestCase_assert(uint16 == 0, "Expected 0 but got %u", uint16);
	int32 = valueGetInt32(NULL);
	TestCase_assert(int32 == 0, "Expected 0 but got %d", int32);
	uint32 = valueGetUInt32(NULL);
	TestCase_assert(uint32 == 0, "Expected 0 but got %u", uint32);
	int64 = valueGetInt64(NULL);
	TestCase_assert(int64 == 0, "Expected 0 but got " INT64_FORMAT, int64);
	uint64 = valueGetUInt64(NULL);
	TestCase_assert(uint64 == 0, "Expected 0 but got " UINT64_FORMAT, uint64);
	float32 = valueGetFloat(NULL);
	TestCase_assert(float32 == 0.0f, "Expected 0.0 but got %f", float32);
	float64 = valueGetDouble(NULL);
	TestCase_assert(float64 == 0.0, "Expected 0.0 but got %f", float64);
	fixed = valueGetFixed16_16(NULL);
	TestCase_assert(fixed == 0x00000, "Expected 0x00000 but got 0x%05X", fixed);
	pointer = valueGetPointer(NULL);
	TestCase_assert(pointer == NULL, "Expected NULL but got %p", pointer);
	string = valueGetString(NULL);
	TestCase_assert(string == NULL, "Expected NULL but got \"%s\"", string);
	blob = valueGetBlob(NULL, NULL);
	TestCase_assert(blob == NULL, "Expected NULL but got %p", blob);
	hashTable = valueGetHashTable(NULL);
	TestCase_assert(hashTable == NULL, "Expected NULL but got %p", hashTable);
	array = valueGetArray(NULL);
	TestCase_assert(array == NULL, "Expected NULL but got %p", array);
	assArray = valueGetAssociativeArray(NULL);
	TestCase_assert(assArray == NULL, "Expected NULL but got %p", assArray);
}

TEST_SUITE(DataValueTest,
           testCreate,
           testGet,
           testCopy,
           testConversions,
           testNullTolerance)
