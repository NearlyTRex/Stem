#include "utilities/HashTable.h"
#include "unittest/framework/TestSuite.h"
#include "unittest/framework/printfFormats.h"
#include <float.h>

static void testCreate() {
	HashTable * hashTable;
	size_t count;
	
	hashTable = hashCreate();
	TestCase_assert(hashTable != NULL, "Expected non-NULL but got NULL");
	count = hashGetCount(hashTable);
	TestCase_assert(count == 0, "Expected 0 but got %u\n", (unsigned int) count);
	hashDispose(hashTable);
}

static void testAccessors() {
	HashTable * hashTable;
	struct HashTableEntry * entryPtr;
	size_t count;
	bool boolResult;
	int8_t int8Result;
	uint8_t uint8Result;
	int16_t int16Result;
	uint16_t uint16Result;
	int32_t int32Result;
	uint32_t uint32Result;
	int64_t int64Result;
	uint64_t uint64Result;
	float floatResult;
	double doubleResult;
	const void * pointerResult;
	const char * stringResult;
	const void * blobResult;
	HashTable * hashTableResult;
	size_t length;
	bool keyPresent;
	void * ptrZero, * ptrNegativeOne;
	const char * stringEmpty = "", * stringHello = "hello ";
	char blob0[1] = {'\x00'}, blob1234[4] = {'\x01', '\x02', '\x03', '\x04'};
	HashTable * tableA, * tableB;
	
	hashTable = hashCreate();
	TestCase_assert(hashTable != NULL, "Expected non-NULL but got NULL");
	
	ptrZero = NULL;
	ptrNegativeOne = (void *) -1;
	tableA = hashCreate();
	tableB = hashCreate();
	hashSetBoolean(tableB, "a", true);
	
	// Verify table empty
	keyPresent = hashHas(hashTable, "bool_false");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "bool_true");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "int8_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "int8_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "uint8_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "uint8_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "int16_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "int16_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "uint16_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "uint16_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "int32_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "int32_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "uint32_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "uint32_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "int64_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "int64_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "uint64_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "uint64_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "float_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "float_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "double_min");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "double_max");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "pointer_NULL");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "pointer_-1");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "string_empty");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "string_hello");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "string_hello_strlen");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "blob_0");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "blob_1234");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "table_a");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "table_b");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "foo");
	TestCase_assert(!keyPresent, "Expected false but got true");
	
	// Verify hashLookup returns NULL for unset keys
	entryPtr = hashLookup(hashTable, "bool_false");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "bool_true");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	
	entryPtr = hashLookup(hashTable, "int8_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "int8_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "uint8_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "uint8_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	
	entryPtr = hashLookup(hashTable, "int16_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "int16_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "uint16_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "uint16_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	
	entryPtr = hashLookup(hashTable, "int32_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "int32_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "uint32_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "uint32_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	
	entryPtr = hashLookup(hashTable, "int64_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "int64_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "uint64_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "uint64_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	
	entryPtr = hashLookup(hashTable, "float_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "float_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "double_min");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	entryPtr = hashLookup(hashTable, "double_max");
	TestCase_assert(entryPtr == NULL, "Expected NULL but got %p", entryPtr);
	
	// Verify accessors return 0 for unset keys
	boolResult = hashGetBoolean(hashTable, "bool_false");
	TestCase_assert(!boolResult, "Expected false but got true");
	boolResult = hashGetBoolean(hashTable, "bool_true");
	TestCase_assert(!boolResult, "Expected false but got true");
	
	int8Result = hashGetInt8(hashTable, "int8_min");
	TestCase_assert(int8Result == 0, "Expected 0 but got %d", int8Result);
	int8Result = hashGetInt8(hashTable, "int8_max");
	TestCase_assert(int8Result == 0, "Expected 0 but got %d", int8Result);
	uint8Result = hashGetUInt8(hashTable, "uint8_min");
	TestCase_assert(uint8Result == 0, "Expected 0 but got %u", uint8Result);
	uint8Result = hashGetUInt8(hashTable, "uint8_max");
	TestCase_assert(uint8Result == 0, "Expected 0 but got %u", uint8Result);
	
	int16Result = hashGetInt16(hashTable, "int16_min");
	TestCase_assert(int16Result == 0, "Expected 0 but got %d", int16Result);
	int16Result = hashGetInt16(hashTable, "int16_max");
	TestCase_assert(int16Result == 0, "Expected 0 but got %d", int16Result);
	uint16Result = hashGetUInt16(hashTable, "uint16_min");
	TestCase_assert(uint16Result == 0, "Expected 0 but got %u", uint16Result);
	uint16Result = hashGetUInt16(hashTable, "uint16_max");
	TestCase_assert(uint16Result == 0, "Expected 0 but got %u", uint16Result);
	
	int32Result = hashGetInt32(hashTable, "int32_min");
	TestCase_assert(int32Result == 0, "Expected 0 but got %d", int32Result);
	int32Result = hashGetInt32(hashTable, "int32_max");
	TestCase_assert(int32Result == 0, "Expected 0 but got %d", int32Result);
	uint32Result = hashGetUInt32(hashTable, "uint32_min");
	TestCase_assert(uint32Result == 0, "Expected 0 but got %u", uint32Result);
	uint32Result = hashGetUInt32(hashTable, "uint32_max");
	TestCase_assert(uint32Result == 0, "Expected 0 but got %u", uint32Result);
	
	int64Result = hashGetInt64(hashTable, "int64_min");
	TestCase_assert(int64Result == 0, "Expected 0 but got " INT64_FORMAT, int64Result);
	int64Result = hashGetInt64(hashTable, "int64_max");
	TestCase_assert(int64Result == 0, "Expected 0 but got " INT64_FORMAT, int64Result);
	uint64Result = hashGetUInt64(hashTable, "uint64_min");
	TestCase_assert(uint64Result == 0, "Expected 0 but got " UINT64_FORMAT, uint64Result);
	uint64Result = hashGetUInt64(hashTable, "uint64_max");
	TestCase_assert(uint64Result == 0, "Expected 0 but got " UINT64_FORMAT, uint64Result);
	
	floatResult = hashGetFloat(hashTable, "float_min");
	TestCase_assert(floatResult == 0.0f, "Expected 0.0 but got %f", floatResult);
	floatResult = hashGetFloat(hashTable, "float_max");
	TestCase_assert(floatResult == 0.0f, "Expected 0.0 but got %f", floatResult);
	doubleResult = hashGetDouble(hashTable, "double_min");
	TestCase_assert(doubleResult == 0.0, "Expected 0.0 but got %f", doubleResult);
	doubleResult = hashGetDouble(hashTable, "double_max");
	TestCase_assert(doubleResult == 0.0, "Expected 0.0 but got %f", doubleResult);
	
	pointerResult = hashGetPointer(hashTable, "pointer_NULL");
	TestCase_assert(pointerResult == NULL, "Expected NULL but got %p", pointerResult);
	pointerResult = hashGetPointer(hashTable, "pointer_-1");
	TestCase_assert(pointerResult == NULL, "Expected NULL but got %p", pointerResult);
	stringResult = hashGetString(hashTable, "string_empty", &length);
	TestCase_assert(stringResult == NULL, "Expected NULL but got %p", stringResult);
	stringResult = hashGetString(hashTable, "string_hello", &length);
	TestCase_assert(stringResult == NULL, "Expected NULL but got %p", stringResult);
	stringResult = hashGetString(hashTable, "string_hello_strlen", &length);
	TestCase_assert(stringResult == NULL, "Expected NULL but got %p", stringResult);
	blobResult = hashGetBlob(hashTable, "blob_0", &length);
	TestCase_assert(blobResult == NULL, "Expected NULL but got %p", blobResult);
	blobResult = hashGetBlob(hashTable, "blob_1234", &length);
	TestCase_assert(blobResult == NULL, "Expected NULL but got %p", blobResult);
	hashTableResult = hashGetHashTable(hashTable, "table_a");
	TestCase_assert(hashTableResult == NULL, "Expected NULL but got %p", hashTableResult);
	hashTableResult = hashGetHashTable(hashTable, "table_b");
	TestCase_assert(hashTableResult == NULL, "Expected NULL but got %p", hashTableResult);
	
	// Set values
	hashSetBoolean(hashTable, "bool_false", false);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 1, "Expected 1 but got %u", (unsigned int) count);
	hashSetBoolean(hashTable, "bool_true", true);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	
	hashSetInt8(hashTable, "int8_min", INT8_MIN);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 3, "Expected 3 but got %u", (unsigned int) count);
	hashSetInt8(hashTable, "int8_max", INT8_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 4, "Expected 4 but got %u", (unsigned int) count);
	hashSetUInt8(hashTable, "uint8_min", 0);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 5, "Expected 5 but got %u", (unsigned int) count);
	hashSetUInt8(hashTable, "uint8_max", UINT8_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 6, "Expected 6 but got %u", (unsigned int) count);
	
	hashSetInt16(hashTable, "int16_min", INT16_MIN);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 7, "Expected 7 but got %u", (unsigned int) count);
	hashSetInt16(hashTable, "int16_max", INT16_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 8, "Expected 8 but got %u", (unsigned int) count);
	hashSetUInt16(hashTable, "uint16_min", 0);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 9, "Expected 9 but got %u", (unsigned int) count);
	hashSetUInt16(hashTable, "uint16_max", UINT16_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 10, "Expected 10 but got %u", (unsigned int) count);
	
	hashSetInt32(hashTable, "int32_min", INT32_MIN);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 11, "Expected 11 but got %u", (unsigned int) count);
	hashSetInt32(hashTable, "int32_max", INT32_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 12, "Expected 12 but got %u", (unsigned int) count);
	hashSetUInt32(hashTable, "uint32_min", 0);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 13, "Expected 13 but got %u", (unsigned int) count);
	hashSetUInt32(hashTable, "uint32_max", UINT32_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 14, "Expected 14 but got %u", (unsigned int) count);
	
	hashSetInt64(hashTable, "int64_min", INT64_MIN);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 15, "Expected 15 but got %u", (unsigned int) count);
	hashSetInt64(hashTable, "int64_max", INT64_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 16, "Expected 16 but got %u", (unsigned int) count);
	hashSetUInt64(hashTable, "uint64_min", 0);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 17, "Expected 17 but got %u", (unsigned int) count);
	hashSetUInt64(hashTable, "uint64_max", UINT64_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 18, "Expected 18 but got %u", (unsigned int) count);
	
	hashSetFloat(hashTable, "float_min", -FLT_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 19, "Expected 19 but got %u", (unsigned int) count);
	hashSetFloat(hashTable, "float_max", FLT_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 20, "Expected 20 but got %u", (unsigned int) count);
	hashSetDouble(hashTable, "double_min", -DBL_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 21, "Expected 21 but got %u", (unsigned int) count);
	hashSetDouble(hashTable, "double_max", DBL_MAX);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 22, "Expected 22 but got %u", (unsigned int) count);
	
	hashSetPointer(hashTable, "pointer_NULL", ptrZero);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 23, "Expected 23 but got %u", (unsigned int) count);
	hashSetPointer(hashTable, "pointer_-1", ptrNegativeOne);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 24, "Expected 24 but got %u", (unsigned int) count);
	hashSetString(hashTable, "string_empty", stringEmpty, HASH_USE_STRLEN);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 25, "Expected 25 but got %u", (unsigned int) count);
	hashSetString(hashTable, "string_hello", stringHello, 5);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 26, "Expected 26 but got %u", (unsigned int) count);
	hashSetString(hashTable, "string_hello_strlen", stringHello, HASH_USE_STRLEN);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 27, "Expected 27 but got %u", (unsigned int) count);
	hashSetBlob(hashTable, "blob_0", blob0, 1);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 28, "Expected 28 but got %u", (unsigned int) count);
	hashSetBlob(hashTable, "blob_1234", blob1234, 4);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 29, "Expected 29 but got %u", (unsigned int) count);
	hashSetHashTable(hashTable, "table_a", tableA);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 30, "Expected 30 but got %u", (unsigned int) count);
	hashSetHashTable(hashTable, "table_b", tableB);
	count = hashGetCount(hashTable);
	TestCase_assert(count == 31, "Expected 31 but got %u", (unsigned int) count);
	
	// Verify table full
	keyPresent = hashHas(hashTable, "bool_false");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "bool_true");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "int8_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "int8_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "uint8_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "uint8_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "int16_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "int16_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "uint16_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "uint16_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "int32_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "int32_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "uint32_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "uint32_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "int64_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "int64_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "uint64_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "uint64_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "float_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "float_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "double_min");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "double_max");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "pointer_NULL");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "pointer_-1");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "string_empty");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "string_hello");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "string_hello_strlen");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "blob_0");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "blob_1234");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "table_a");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "table_b");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "foo");
	TestCase_assert(!keyPresent, "Expected false but got true");
	
	// Verify hashLookup returns correct values for set keys
	entryPtr = hashLookup(hashTable, "bool_false");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_BOOLEAN, "Expected %d but got %d", HASH_TYPE_BOOLEAN, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "bool_false"), "Expected \"bool_false\" but got \"%s\"", entryPtr->key);
	TestCase_assert(!entryPtr->value.boolean, "Expected false but got true");
	entryPtr = hashLookup(hashTable, "bool_true");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_BOOLEAN, "Expected %d but got %d", HASH_TYPE_BOOLEAN, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "bool_true"), "Expected \"bool_true\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.boolean, "Expected true but got false");
	
	entryPtr = hashLookup(hashTable, "int8_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_INT8, "Expected %d but got %d", HASH_TYPE_INT8, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "int8_min"), "Expected \"int8_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.int8 == INT8_MIN, "Expected %d but got %d", INT8_MIN, entryPtr->value.int8);
	entryPtr = hashLookup(hashTable, "int8_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_INT8, "Expected %d but got %d", HASH_TYPE_INT8, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "int8_max"), "Expected \"int8_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.int8 == INT8_MAX, "Expected %d but got %d", INT8_MAX, entryPtr->value.int8);
	entryPtr = hashLookup(hashTable, "uint8_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_UINT8, "Expected %d but got %d", HASH_TYPE_UINT8, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "uint8_min"), "Expected \"uint8_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.uint8 == 0, "Expected 0 but got %u", entryPtr->value.uint8);
	entryPtr = hashLookup(hashTable, "uint8_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_UINT8, "Expected %d but got %d", HASH_TYPE_UINT8, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "uint8_max"), "Expected \"uint8_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.uint8 == UINT8_MAX, "Expected %u but got %u", UINT8_MAX, entryPtr->value.uint8);
	
	entryPtr = hashLookup(hashTable, "int16_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_INT16, "Expected %d but got %d", HASH_TYPE_INT16, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "int16_min"), "Expected \"int16_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.int16 == INT16_MIN, "Expected %d but got %d", INT16_MIN, entryPtr->value.int16);
	entryPtr = hashLookup(hashTable, "int16_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_INT16, "Expected %d but got %d", HASH_TYPE_INT16, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "int16_max"), "Expected \"int16_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.int16 == INT16_MAX, "Expected %d but got %d", INT16_MAX, entryPtr->value.int16);
	entryPtr = hashLookup(hashTable, "uint16_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_UINT16, "Expected %d but got %d", HASH_TYPE_UINT16, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "uint16_min"), "Expected \"uint16_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.uint16 == 0, "Expected 0 but got %u", entryPtr->value.uint16);
	entryPtr = hashLookup(hashTable, "uint16_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_UINT16, "Expected %d but got %d", HASH_TYPE_UINT16, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "uint16_max"), "Expected \"uint16_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.uint16 == UINT16_MAX, "Expected %u but got %u", UINT16_MAX, entryPtr->value.uint16);
	
	entryPtr = hashLookup(hashTable, "int32_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_INT32, "Expected %d but got %d", HASH_TYPE_INT32, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "int32_min"), "Expected \"int32_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.int32 == INT32_MIN, "Expected %d but got %d", INT32_MIN, entryPtr->value.int32);
	entryPtr = hashLookup(hashTable, "int32_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_INT32, "Expected %d but got %d", HASH_TYPE_INT32, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "int32_max"), "Expected \"int32_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.int32 == INT32_MAX, "Expected %d but got %d", INT32_MAX, entryPtr->value.int32);
	entryPtr = hashLookup(hashTable, "uint32_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_UINT32, "Expected %d but got %d", HASH_TYPE_UINT32, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "uint32_min"), "Expected \"uint32_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.uint32 == 0, "Expected 0 but got %u", entryPtr->value.uint32);
	entryPtr = hashLookup(hashTable, "uint32_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_UINT32, "Expected %d but got %d", HASH_TYPE_UINT32, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "uint32_max"), "Expected \"uint32_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.uint32 == UINT32_MAX, "Expected %u but got %u", UINT32_MAX, entryPtr->value.uint32);
	
	entryPtr = hashLookup(hashTable, "int64_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_INT64, "Expected %d but got %d", HASH_TYPE_INT64, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "int64_min"), "Expected \"int64_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.int64 == INT64_MIN, "Expected " INT64_FORMAT " but got " INT64_FORMAT, INT64_MIN, entryPtr->value.int64);
	entryPtr = hashLookup(hashTable, "int64_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_INT64, "Expected %d but got %d", HASH_TYPE_INT64, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "int64_max"), "Expected \"int64_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.int64 == INT64_MAX, "Expected " INT64_FORMAT " but got " INT64_FORMAT, INT64_MAX, entryPtr->value.int64);
	entryPtr = hashLookup(hashTable, "uint64_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_UINT64, "Expected %d but got %d", HASH_TYPE_UINT64, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "uint64_min"), "Expected \"uint64_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.uint64 == 0, "Expected 0 but got " UINT64_FORMAT, entryPtr->value.uint64);
	entryPtr = hashLookup(hashTable, "uint64_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_UINT64, "Expected %d but got %d", HASH_TYPE_UINT64, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "uint64_max"), "Expected \"uint64_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.uint64 == UINT64_MAX, "Expected " UINT64_FORMAT " but got " UINT64_FORMAT, UINT64_MAX, entryPtr->value.uint64);
	
	entryPtr = hashLookup(hashTable, "float_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_FLOAT, "Expected %d but got %d", HASH_TYPE_FLOAT, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "float_min"), "Expected \"float_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.float32 == -FLT_MAX, "Expected %f but got %f", -FLT_MAX, entryPtr->value.float32);
	entryPtr = hashLookup(hashTable, "float_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_FLOAT, "Expected %d but got %d", HASH_TYPE_FLOAT, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "float_max"), "Expected \"float_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.float32 == FLT_MAX, "Expected %f but got %f", FLT_MAX, entryPtr->value.float32);
	entryPtr = hashLookup(hashTable, "double_min");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_DOUBLE, "Expected %d but got %d", HASH_TYPE_DOUBLE, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "double_min"), "Expected \"double_min\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.float64 == -DBL_MAX, "Expected %f but got %f", -DBL_MAX, entryPtr->value.float64);
	entryPtr = hashLookup(hashTable, "double_max");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_DOUBLE, "Expected %d but got %d", HASH_TYPE_DOUBLE, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "double_max"), "Expected \"double_max\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.float64 == DBL_MAX, "Expected %f but got %f", DBL_MAX, entryPtr->value.float64);
	
	entryPtr = hashLookup(hashTable, "pointer_NULL");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_POINTER, "Expected %d but got %d", HASH_TYPE_POINTER, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "pointer_NULL"), "Expected \"pointer_NULL\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.pointer == ptrZero, "Expected %p but got %p", ptrZero, entryPtr->value.pointer);
	entryPtr = hashLookup(hashTable, "pointer_-1");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_POINTER, "Expected %d but got %d", HASH_TYPE_POINTER, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "pointer_-1"), "Expected \"pointer_NULL\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.pointer == ptrNegativeOne, "Expected %p but got %p", ptrNegativeOne, entryPtr->value.pointer);
	entryPtr = hashLookup(hashTable, "string_empty");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_STRING, "Expected %d but got %d", HASH_TYPE_STRING, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "string_empty"), "Expected \"string_empty\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.string.length == 0, "Expected 0 but got %u", (unsigned int) entryPtr->value.string.length);
	entryPtr = hashLookup(hashTable, "string_hello");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_STRING, "Expected %d but got %d", HASH_TYPE_STRING, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "string_hello"), "Expected \"string_hello\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.string.length == 5, "Expected 5 but got %u", (unsigned int) entryPtr->value.string.length);
	TestCase_assert(!strcmp(entryPtr->value.string.bytes, "hello"), "Expected \"hello\" but got \"%s\"", entryPtr->value.string.bytes);
	entryPtr = hashLookup(hashTable, "string_hello_strlen");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_STRING, "Expected %d but got %d", HASH_TYPE_STRING, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "string_hello_strlen"), "Expected \"string_hello_strlen\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.string.length == 6, "Expected 6 but got %u", (unsigned int) entryPtr->value.string.length);
	TestCase_assert(!strcmp(entryPtr->value.string.bytes, "hello "), "Expected \"hello \" but got \"%s\"", entryPtr->value.string.bytes);
	entryPtr = hashLookup(hashTable, "blob_0");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_BLOB, "Expected %d but got %d", HASH_TYPE_BLOB, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "blob_0"), "Expected \"blob_0\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.blob.length == 1, "Expected 1 but got %u", (unsigned int) entryPtr->value.blob.length);
	TestCase_assert(!memcmp(entryPtr->value.blob.bytes, blob0, sizeof(blob0)), "Expected \"%.*s\" but got \"%.*s\"", 1, blob0, 1, entryPtr->value.string.bytes);
	entryPtr = hashLookup(hashTable, "blob_1234");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_BLOB, "Expected %d but got %d", HASH_TYPE_BLOB, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "blob_1234"), "Expected \"blob_1234\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.blob.length == 4, "Expected 4 but got %u", (unsigned int) entryPtr->value.blob.length);
	TestCase_assert(!memcmp(entryPtr->value.blob.bytes, blob1234, sizeof(blob1234)), "Expected \"%.*s\" but got \"%.*s\"", 4, blob1234, 4, entryPtr->value.string.bytes);
	entryPtr = hashLookup(hashTable, "table_a");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_HASH, "Expected %d but got %d", HASH_TYPE_HASH, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "table_a"), "Expected \"table_a\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.hashTable != NULL, "Expected non-NULL but got NULL");
	count = hashGetCount(entryPtr->value.hashTable);
	TestCase_assert(count == 0, "Expected 0 but got %u", (unsigned int) count);
	TestCase_assert(entryPtr->value.hashTable != tableA, "Expected pointers to differ, but they didn't");
	entryPtr = hashLookup(hashTable, "table_b");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_HASH, "Expected %d but got %d", HASH_TYPE_HASH, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "table_b"), "Expected \"table_b\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.hashTable != NULL, "Expected non-NULL but got NULL");
	count = hashGetCount(entryPtr->value.hashTable);
	TestCase_assert(count == 1, "Expected 1 but got %u", (unsigned int) count);
	TestCase_assert(entryPtr->value.hashTable != tableB, "Expected pointers to differ, but they didn't");
	entryPtr = hashLookup(entryPtr->value.hashTable, "a");
	TestCase_assert(entryPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entryPtr->type == HASH_TYPE_BOOLEAN, "Expected %d but got %d", HASH_TYPE_BOOLEAN, entryPtr->type);
	TestCase_assert(!strcmp(entryPtr->key, "a"), "Expected \"a\" but got \"%s\"", entryPtr->key);
	TestCase_assert(entryPtr->value.boolean, "Expected true but got false");
	
	// Verify accessors return correct values for set keys
	boolResult = hashGetBoolean(hashTable, "bool_false");
	TestCase_assert(!boolResult, "Expected false but got true");
	boolResult = hashGetBoolean(hashTable, "bool_true");
	TestCase_assert(boolResult, "Expected true but got false");
	
	int8Result = hashGetInt8(hashTable, "int8_min");
	TestCase_assert(int8Result == INT8_MIN, "Expected %d but got %d", INT8_MIN, int8Result);
	int8Result = hashGetInt8(hashTable, "int8_max");
	TestCase_assert(int8Result == INT8_MAX, "Expected %d but got %d", INT8_MAX, int8Result);
	uint8Result = hashGetUInt8(hashTable, "uint8_min");
	TestCase_assert(uint8Result == 0, "Expected 0 but got %u", uint8Result);
	uint8Result = hashGetUInt8(hashTable, "uint8_max");
	TestCase_assert(uint8Result == UINT8_MAX, "Expected %u but got %u", UINT8_MAX, uint8Result);
	
	int16Result = hashGetInt16(hashTable, "int16_min");
	TestCase_assert(int16Result == INT16_MIN, "Expected %d but got %d", INT16_MIN, int16Result);
	int16Result = hashGetInt16(hashTable, "int16_max");
	TestCase_assert(int16Result == INT16_MAX, "Expected %d but got %d", INT16_MAX, int16Result);
	uint16Result = hashGetUInt16(hashTable, "uint16_min");
	TestCase_assert(uint16Result == 0, "Expected 0 but got %u", uint16Result);
	uint16Result = hashGetUInt16(hashTable, "uint16_max");
	TestCase_assert(uint16Result == UINT16_MAX, "Expected %u but got %u", UINT16_MAX, uint16Result);
	
	int32Result = hashGetInt32(hashTable, "int32_min");
	TestCase_assert(int32Result == INT32_MIN, "Expected %d but got %d", INT32_MIN, int32Result);
	int32Result = hashGetInt32(hashTable, "int32_max");
	TestCase_assert(int32Result == INT32_MAX, "Expected %d but got %d", INT32_MAX, int32Result);
	uint32Result = hashGetUInt32(hashTable, "uint32_min");
	TestCase_assert(uint32Result == 0, "Expected 0 but got %u", uint32Result);
	uint32Result = hashGetUInt32(hashTable, "uint32_max");
	TestCase_assert(uint32Result == UINT32_MAX, "Expected %u but got %u", UINT32_MAX, uint32Result);
	
	int64Result = hashGetInt64(hashTable, "int64_min");
	TestCase_assert(int64Result == INT64_MIN, "Expected " INT64_FORMAT " but got " INT64_FORMAT, INT64_MIN, int64Result);
	int64Result = hashGetInt64(hashTable, "int64_max");
	TestCase_assert(int64Result == INT64_MAX, "Expected " INT64_FORMAT " but got " INT64_FORMAT, INT64_MAX, int64Result);
	uint64Result = hashGetUInt64(hashTable, "uint64_min");
	TestCase_assert(uint64Result == 0, "Expected 0 but got " UINT64_FORMAT, uint64Result);
	uint64Result = hashGetUInt64(hashTable, "uint64_max");
	TestCase_assert(uint64Result == UINT64_MAX, "Expected " UINT64_FORMAT " but got " UINT64_FORMAT, UINT64_MAX, uint64Result);
	
	floatResult = hashGetFloat(hashTable, "float_min");
	TestCase_assert(floatResult == -FLT_MAX, "Expected %f but got %f", -FLT_MAX, floatResult);
	floatResult = hashGetFloat(hashTable, "float_max");
	TestCase_assert(floatResult == FLT_MAX, "Expected %f but got %f", FLT_MAX, floatResult);
	doubleResult = hashGetDouble(hashTable, "double_min");
	TestCase_assert(doubleResult == -DBL_MAX, "Expected %f but got %f", -DBL_MAX, doubleResult);
	doubleResult = hashGetDouble(hashTable, "double_max");
	TestCase_assert(doubleResult == DBL_MAX, "Expected %f but got %f", DBL_MAX, doubleResult);
	
	pointerResult = hashGetPointer(hashTable, "pointer_NULL");
	TestCase_assert(pointerResult == ptrZero, "Expected %p but got %p", ptrZero, pointerResult);
	pointerResult = hashGetPointer(hashTable, "pointer_-1");
	TestCase_assert(pointerResult == ptrNegativeOne, "Expected %p but got %p", ptrNegativeOne, pointerResult);
	stringResult = hashGetString(hashTable, "string_empty", &length);
	TestCase_assert(stringResult != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(stringResult, stringEmpty), "Expected \"%s\" but got \"%s\"", stringEmpty, stringResult);
	TestCase_assert(length == 0, "Expected 0 but got %u", (unsigned int) length);
	stringResult = hashGetString(hashTable, "string_hello", &length);
	TestCase_assert(stringResult != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strncmp(stringResult, stringHello, 5), "Expected \"%.5s\" but got \"%.5s\"", stringHello, stringResult);
	TestCase_assert(length == 5, "Expected 5 but got %u", (unsigned int) length);
	stringResult = hashGetString(hashTable, "string_hello_strlen", &length);
	TestCase_assert(stringResult != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(stringResult, stringHello), "Expected \"%s\" but got \"%s\"", stringHello, stringResult);
	TestCase_assert(length == 6, "Expected 6 but got %u", (unsigned int) length);
	blobResult = hashGetBlob(hashTable, "blob_0", &length);
	TestCase_assert(blobResult != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 1, "Expected 1 but got %u", (unsigned int) length);
	TestCase_assert(!memcmp(blobResult, blob0, sizeof(blob0)), "Expected \"%.*s\" but got \"%.*s\"", 1, (char *) blob0, 1, (char *) blobResult);
	blobResult = hashGetBlob(hashTable, "blob_1234", &length);
	TestCase_assert(blobResult != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 4, "Expected 4 but got %u", (unsigned int) length);
	TestCase_assert(!memcmp(blobResult, blob1234, sizeof(blob1234)), "Expected \"%.*s\" but got \"%.*s\"", 4, (char *) blob1234, 4, (char *) blobResult);
	hashTableResult = hashGetHashTable(hashTable, "table_a");
	TestCase_assert(hashTableResult != NULL, "Expected non-NULL but got NULL");
	count = hashGetCount(hashTableResult);
	TestCase_assert(count == 0, "Expected 0 but got %u", (unsigned int) count);
	TestCase_assert(hashTableResult != tableA, "Expected pointers to differ, but they didn't");
	hashTableResult = hashGetHashTable(hashTable, "table_b");
	TestCase_assert(hashTableResult != NULL, "Expected non-NULL but got NULL");
	count = hashGetCount(hashTableResult);
	TestCase_assert(count == 1, "Expected 1 but got %u", (unsigned int) count);
	TestCase_assert(hashTableResult != tableB, "Expected pointers to differ, but they didn't");
	boolResult = hashGetBoolean(hashTableResult, "a");
	TestCase_assert(boolResult, "Expected true but got false");
}

static void testCopy() {
	HashTable * hashTable;
	HashTable * copy;
	size_t count, countCopy;
	const char * string, * stringCopy;
	const void * blob, * blobCopy;
	HashTable * subtable, * subtableCopy;
	size_t length, lengthCopy;
	
	hashTable = hashCreate();
	hashSetString(hashTable, "a", "hello", HASH_USE_STRLEN);
	hashSetBlob(hashTable, "b", "foo", 3);
	subtable = hashCreate();
	hashSetString(subtable, "c", "foo", HASH_USE_STRLEN);
	hashSetPointer(subtable, "d", (void *) 0x1234);
	hashSetHashTable(hashTable, "c", subtable);
	hashDispose(subtable);
	
	copy = hashCopy(hashTable);
	TestCase_assert(copy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(copy != hashTable, "Expected pointers to differ, but they didn't");
	
	count = hashGetCount(hashTable);
	countCopy = hashGetCount(copy);
	TestCase_assert(count == countCopy, "Expected %u but got %u", (unsigned int) count, (unsigned int) countCopy);
	
	string = hashGetString(hashTable, "a", &length);
	stringCopy = hashGetString(copy, "a", &lengthCopy);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(stringCopy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 5, "Expected 5 but got %u", (unsigned int) length);
	TestCase_assert(lengthCopy == 5, "Expected 5 but got %u", (unsigned int) lengthCopy);
	TestCase_assert(!strcmp(string, "hello"), "Expected \"hello\" but got \"%s\"", string);
	TestCase_assert(!strcmp(stringCopy, "hello"), "Expected \"hello\" but got \"%s\"", stringCopy);
	TestCase_assert(string != stringCopy, "Expected pointers to differ, but they didn't");
	
	blob = hashGetBlob(hashTable, "b", &length);
	blobCopy = hashGetBlob(copy, "b", &lengthCopy);
	TestCase_assert(blob != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(blobCopy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 3, "Expected 3 but got %u", (unsigned int) length);
	TestCase_assert(lengthCopy == 3, "Expected 3 but got %u", (unsigned int) lengthCopy);
	TestCase_assert(!memcmp(blob, "foo", 3), "Expected \"foo\" but got \"%.*s\"", (unsigned int) length, (char *) blob);
	TestCase_assert(!memcmp(blobCopy, "foo", 3), "Expected \"foo\" but got \"%.*s\"", (unsigned int) lengthCopy, (char *) blobCopy);
	TestCase_assert(blob != blobCopy, "Expected pointers to differ, but they didn't");
	
	subtable = hashGetHashTable(hashTable, "c");
	subtableCopy = hashGetHashTable(copy, "c");
	TestCase_assert(subtable != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(subtableCopy != NULL, "Expected non-NULL but got NULL");
	count = hashGetCount(subtable);
	countCopy = hashGetCount(subtableCopy);
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	TestCase_assert(countCopy == 2, "Expected 2 but got %u", (unsigned int) countCopy);
	TestCase_assert(subtable != subtableCopy, "Expected pointers to differ, but they didn't");
	
	hashDispose(hashTable);
	hashDispose(copy);
}

static void testReplaceValues() {
	HashTable * hash;
	size_t count;
	const char ** keys;
	int32_t int32;
	const char * string;
	
	hash = hashCreate();
	hashSetInt32(hash, "a", 1);
	hashSetInt32(hash, "b", 2);
	count = hashGetCount(hash);
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	int32 = hashGetInt32(hash, "b");
	TestCase_assert(int32 == 2, "Expected 2 but got %d", int32);
	
	hashSetInt32(hash, "b", 3);
	count = hashGetCount(hash);
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	int32 = hashGetInt32(hash, "b");
	TestCase_assert(int32 == 3, "Expected 3 but got %d", int32);
	
	hashSetString(hash, "a", "hello", HASH_USE_STRLEN);
	count = hashGetCount(hash);
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	string = hashGetString(hash, "a", NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(string, "hello"), "Expected \"hello\" but got \"%s\"", string);
	
	hashSetInt32(hash, "a", 1);
	count = hashGetCount(hash);
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	int32 = hashGetInt32(hash, "a");
	TestCase_assert(int32 == 1, "Expected 1 but got %d", int32);
	
	keys = hashGetKeys(hash, &count);
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	TestCase_assert((!strcmp(keys[0], "a") && !strcmp(keys[1], "b")) || (!strcmp(keys[0], "b") && !strcmp(keys[1], "a")), "Expected \"a\" and \"b\", but got \"%s\" and \"%s\"", keys[0], keys[1]);
	free(keys);
}

static void testDeleteValues() {
	HashTable * hash;
	size_t count;
	const char ** keys;
	bool found;
	
	hash = hashCreate();
	hashSetInt32(hash, "a", 1);
	hashSetInt32(hash, "b", 2);
	hashSetInt32(hash, "c", 3);
	count = hashGetCount(hash);
	TestCase_assert(count == 3, "Expected 3 but got %u", (unsigned int) count);
	
	found = hashDelete(hash, "d");
	TestCase_assert(!found, "Expected false but got true");
	
	found = hashDelete(hash, "b");
	TestCase_assert(found, "Expected true but got false");
	count = hashGetCount(hash);
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	TestCase_assert(!hashHas(hash, "b"), "Expected false but got true");
	keys = hashGetKeys(hash, &count);
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	TestCase_assert((!strcmp(keys[0], "a") && !strcmp(keys[1], "c")) || (!strcmp(keys[0], "c") && !strcmp(keys[1], "a")), "Expected \"a\" and \"c\", but got \"%s\" and \"%s\"", keys[0], keys[1]);
	free(keys);
	
	found = hashDelete(hash, "b");
	TestCase_assert(!found, "Expected false but got true");
	
	found = hashDelete(hash, "a");
	TestCase_assert(found, "Expected true but got false");
	count = hashGetCount(hash);
	TestCase_assert(count == 1, "Expected 1 but got %u", (unsigned int) count);
	TestCase_assert(!hashHas(hash, "a"), "Expected false but got true");
	keys = hashGetKeys(hash, &count);
	TestCase_assert(count == 1, "Expected 1 but got %u", (unsigned int) count);
	TestCase_assert(!strcmp(keys[0], "c"), "Expected \"c\" but got \"%s\"", keys[0]);
	free(keys);
	
	hashDispose(hash);
}

static void testGetKeys() {
	HashTable * hashTable;
	const char ** keys;
	size_t count;
	
	hashTable = hashCreate();
	hashSetBoolean(hashTable, "a", false);
	count = 0;
	keys = hashGetKeys(hashTable, &count);
	
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(count == 1, "Expected 1 but got %u", (unsigned int) count);
	TestCase_assert(!strcmp(keys[0], "a"), "Expected \"a\" but got \"%s\"", keys[0]);
	
	hashDispose(hashTable);
	free(keys);
	
	hashTable = hashCreate();
	hashSetBoolean(hashTable, "bar", true);
	hashSetInt32(hashTable, "foo", 1);
	count = 0;
	keys = hashGetKeys(hashTable, &count);
	
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(count == 2, "Expected 2 but got %u", (unsigned int) count);
	TestCase_assert((!strcmp(keys[0], "foo") && !strcmp(keys[1], "bar")) || (!strcmp(keys[0], "bar") && !strcmp(keys[1], "foo")), "Expected \"foo\" and \"bar\" but got \"%s\" and \"%s\"", keys[0], keys[1]);
	
	hashDispose(hashTable);
	free(keys);
}

TEST_SUITE(HashTableTest,
           testCreate,
           testAccessors,
           testCopy,
           testReplaceValues,
           testDeleteValues,
           testGetKeys)
