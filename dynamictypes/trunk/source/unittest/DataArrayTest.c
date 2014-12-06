#include "dynamictypes/DataArray.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

static void testCreate() {
	DataArray * array;
	
	array = arrayCreate();
	TestCase_assert(array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(array->count == 0, "Expected 0 but got " SIZE_T_FORMAT, array->count);
	arrayDispose(array);
}

static void testCreateWithValues() {
	DataArray * array;
	
	array = arrayCreateWithValues(valueCreateBoolean(false));
	TestCase_assert(array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(!array->values[0].value.boolean, "Expected false but got true");
	arrayDispose(array);
	
	array = arrayCreateWithValues(valueCreateBoolean(true), valueCreateInt8(2));
	TestCase_assert(array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	TestCase_assert(array->values[1].type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, array->values[1].type);
	TestCase_assert(array->values[1].value.int8 == 2, "Expected 2 but got %d", array->values[1].value.int8);
	arrayDispose(array);
}

static void testAppend() {
	DataArray * array;
	
	array = arrayCreate();
	TestCase_assert(array->count == 0, "Expected 0 but got " SIZE_T_FORMAT, array->count);
	
	arrayAppend(array, valueCreateBoolean(false));
	TestCase_assert(array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(!array->values[0].value.boolean, "Expected false but got true");
	
	arrayAppend(array, valueCreateBoolean(true));
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[1].type);
	TestCase_assert(array->values[1].value.boolean, "Expected true but got false");
	
	arrayAppend(array, valueCreateInt32(1));
	TestCase_assert(array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[2].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[2].type);
	TestCase_assert(array->values[2].value.int32 == 1, "Expected 1 but got %d", array->values[2].value.int32);
	
	arrayDispose(array);
}

static void testGet() {
	DataArray * array;
	DataValue * value;
	
	array = arrayCreate();
	arrayAppend(array, valueCreateBoolean(false));
	arrayAppend(array, valueCreateBoolean(true));
	arrayAppend(array, valueCreateInt32(1));
	
	TestCase_assert(array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, array->count);
	
	value = arrayGet(array, 0);
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!value->value.boolean, "Expected false but got true");
	
	value = arrayGet(array, 1);
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(value->value.boolean, "Expected true but got false");
	
	value = arrayGet(array, 2);
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, value->type);
	TestCase_assert(value->value.int32 == 1, "Expected 1 but got %d", value->value.int32);
	
	value = arrayGet(array, 3);
	TestCase_assert(value == NULL, "Expected NULL but got %p", value);
	
	arrayDispose(array);
}

static void testInsert() {
	DataArray * array;
	
	array = arrayCreate();
	TestCase_assert(array->count == 0, "Expected 0 but got " SIZE_T_FORMAT, array->count);
	
	arrayInsert(array, 0, valueCreateBoolean(false));
	TestCase_assert(array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(!array->values[0].value.boolean, "Expected false but got true");
	
	arrayInsert(array, 0, valueCreateBoolean(true));
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	TestCase_assert(array->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[1].type);
	TestCase_assert(!array->values[1].value.boolean, "Expected false but got true");
	
	arrayInsert(array, 1, valueCreateInt32(1));
	TestCase_assert(array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	TestCase_assert(array->values[1].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[1].type);
	TestCase_assert(array->values[1].value.int32 == 1, "Expected 1 but got %d", array->values[2].value.int32);
	TestCase_assert(array->values[2].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[2].type);
	TestCase_assert(!array->values[2].value.boolean, "Expected false but got true");
	
	arrayInsert(array, 4, valueCreateBoolean(false));
	TestCase_assert(array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, array->count);
	
	arrayDispose(array);
}

static void testCopy() {
	DataArray * array, * copy;
	
	array = arrayCreate();
	copy = arrayCopy(array);
	TestCase_assert(copy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(copy->count == 0, "Expected 0 but got " SIZE_T_FORMAT, copy->count);
	TestCase_assert(copy != array, "Expected pointers to differ, but both are %p", copy);
	arrayDispose(copy);
	
	arrayAppend(array, valueCreateBoolean(false));
	arrayAppend(array, valueCreateBoolean(true));
	arrayAppend(array, valueCreateInt32(1));
	copy = arrayCopy(array);
	
	TestCase_assert(copy->count == 3, "Expected 3 but got " SIZE_T_FORMAT, copy->count);
	TestCase_assert(copy->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, copy->values[0].type);
	TestCase_assert(!copy->values[0].value.boolean, "Expected false but got true");
	TestCase_assert(copy->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, copy->values[1].type);
	TestCase_assert(copy->values[1].value.boolean, "Expected true but got false");
	TestCase_assert(copy->values[2].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, copy->values[2].type);
	TestCase_assert(copy->values[2].value.int32 == 1, "Expected 1 but got %d", copy->values[2].value.int32);
	arrayDispose(copy);
	arrayDispose(array);
	
	array = arrayCreate();
	arrayAppend(array, valueCreateString("foo", 3, true, true));
	copy = arrayCopy(array);
	TestCase_assert(copy->count == 1, "Expected 1 but got " SIZE_T_FORMAT, copy->count);
	TestCase_assert(copy->values[0].type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, copy->values[0].type);
	TestCase_assert(copy->values[0].value.string.bytes != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(copy->values[0].value.string.length == 3, "Expected 3 but got " SIZE_T_FORMAT, copy->values[0].value.string.length);
	TestCase_assert(!strcmp(copy->values[0].value.string.bytes, "foo"), "Expected \"foo\" but got \"%s\"", copy->values[0].value.string.bytes);
	TestCase_assert(copy->values[0].value.string.bytes != array->values[0].value.string.bytes, "Expected pointers to differ, but both are %p", copy->values[0].value.string.bytes);
	
	arrayDispose(copy);
	arrayDispose(array);
}

static void testReplace() {
	DataArray * array;
	
	array = arrayCreate();
	arrayAppend(array, valueCreateBoolean(false));
	arrayAppend(array, valueCreateBoolean(true));
	
	arrayReplace(array, 0, valueCreateInt32(1));
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[0].type);
	TestCase_assert(array->values[0].value.int32 == 1, "Expected 1 but got %d", array->values[0].value.int32);
	TestCase_assert(array->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[1].type);
	TestCase_assert(array->values[1].value.boolean, "Expected true but got false");
	
	arrayReplace(array, 1, valueCreateInt32(2));
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[0].type);
	TestCase_assert(array->values[0].value.int32 == 1, "Expected 1 but got %d", array->values[0].value.int32);
	TestCase_assert(array->values[1].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[1].type);
	TestCase_assert(array->values[1].value.int32 == 2, "Expected 2 but got %d", array->values[1].value.int32);
	
	arrayDispose(array);
}

static void testDelete() {
	DataArray * array;
	
	array = arrayCreate();
	arrayAppend(array, valueCreateBoolean(false));
	arrayAppend(array, valueCreateBoolean(true));
	arrayAppend(array, valueCreateInt32(1));
	
	arrayDelete(array, 0);
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	TestCase_assert(array->values[1].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[1].type);
	TestCase_assert(array->values[1].value.int32 == 1, "Expected 1 but got %d", array->values[1].value.int32);
	
	arrayDelete(array, 1);
	TestCase_assert(array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	
	arrayDispose(array);
}

TEST_SUITE(DataArrayTest,
           testCreate,
           testCreateWithValues,
           testAppend,
           testGet,
           testInsert,
           testCopy,
           testReplace,
           testDelete);
