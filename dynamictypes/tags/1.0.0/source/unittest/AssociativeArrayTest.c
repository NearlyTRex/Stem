#include "dynamictypes/AssociativeArray.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

static void testCreate() {
	AssociativeArray * array;
	
	array = associativeArrayCreate();
	TestCase_assert(array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(array->count == 0, "Expected 0 but got " SIZE_T_FORMAT, array->count);
	associativeArrayDispose(array);
}

static void testCreateWithKeysAndValues() {
	AssociativeArray * array;
	
	array = associativeArrayCreateWithKeysAndValues("a", valueCreateBoolean(false), NULL);
	TestCase_assert(array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(!array->values[0].value.boolean, "Expected false but got true");
	TestCase_assert(!strcmp(array->keys[0], "a"), "Expected \"a\" but got \"%s\"", array->keys[0]);
	associativeArrayDispose(array);
	
	array = associativeArrayCreateWithKeysAndValues("b", valueCreateBoolean(true), "foo", valueCreateInt8(2), NULL);
	TestCase_assert(array != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(array->keys[0], "b"), "Expected \"b\" but got \"%s\"", array->keys[0]);
	TestCase_assert(array->values[1].type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, array->values[1].type);
	TestCase_assert(array->values[1].value.int8 == 2, "Expected 2 but got %d", array->values[1].value.int8);
	TestCase_assert(!strcmp(array->keys[1], "foo"), "Expected \"foo\" but got \"%s\"", array->keys[1]);
	associativeArrayDispose(array);
}

static void testAppend() {
	AssociativeArray * array;
	
	array = associativeArrayCreate();
	TestCase_assert(array->count == 0, "Expected 0 but got " SIZE_T_FORMAT, array->count);
	
	associativeArrayAppend(array, "a", valueCreateBoolean(false));
	TestCase_assert(array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(!array->values[0].value.boolean, "Expected false but got true");
	TestCase_assert(!strcmp(array->keys[0], "a"), "Expected \"a\" but got \"%s\"", array->keys[0]);
	
	associativeArrayAppend(array, "b", valueCreateBoolean(true));
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[1].type);
	TestCase_assert(array->values[1].value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(array->keys[1], "b"), "Expected \"b\" but got \"%s\"", array->keys[1]);
	
	associativeArrayAppend(array, "c", valueCreateInt32(1));
	TestCase_assert(array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[2].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[2].type);
	TestCase_assert(array->values[2].value.int32 == 1, "Expected 1 but got %d", array->values[2].value.int32);
	TestCase_assert(!strcmp(array->keys[2], "c"), "Expected \"c\" but got \"%s\"", array->keys[2]);
	
	associativeArrayDispose(array);
}

static void testGetKeyAtIndex() {
	AssociativeArray * array;
	const char * key;
	
	array = associativeArrayCreate();
	associativeArrayAppend(array, "a", valueCreateBoolean(false));
	associativeArrayAppend(array, "b", valueCreateBoolean(true));
	associativeArrayAppend(array, "c", valueCreateInt32(1));
	
	TestCase_assert(array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, array->count);
	
	key = associativeArrayGetKeyAtIndex(array, 0);
	TestCase_assert(key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(key, "a"), "Expected \"a\" but got \"%s\"", key);
	
	key = associativeArrayGetKeyAtIndex(array, 1);
	TestCase_assert(key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(key, "b"), "Expected \"b\" but got \"%s\"", key);
	
	key = associativeArrayGetKeyAtIndex(array, 2);
	TestCase_assert(key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(key, "c"), "Expected \"c\" but got \"%s\"", key);
	
	key = associativeArrayGetKeyAtIndex(array, 3);
	TestCase_assert(key == NULL, "Expected NULL but got %p", key);
	
	associativeArrayDispose(array);
}

static void testGetValueAtIndex() {
	AssociativeArray * array;
	DataValue * value;
	
	array = associativeArrayCreate();
	associativeArrayAppend(array, "a", valueCreateBoolean(false));
	associativeArrayAppend(array, "b", valueCreateBoolean(true));
	associativeArrayAppend(array, "c", valueCreateInt32(1));
	
	TestCase_assert(array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, array->count);
	
	value = associativeArrayGetValueAtIndex(array, 0);
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!value->value.boolean, "Expected false but got true");
	
	value = associativeArrayGetValueAtIndex(array, 1);
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(value->value.boolean, "Expected true but got false");
	
	value = associativeArrayGetValueAtIndex(array, 2);
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, value->type);
	TestCase_assert(value->value.int32 == 1, "Expected 1 but got %d", value->value.int32);
	
	value = associativeArrayGetValueAtIndex(array, 3);
	TestCase_assert(value == NULL, "Expected NULL but got %p", value);
	
	associativeArrayDispose(array);
}

static void testGetFirstIndexOfKey() {
	AssociativeArray * array;
	size_t index;
	
	array = associativeArrayCreate();
	associativeArrayAppend(array, "a", valueCreateBoolean(false));
	associativeArrayAppend(array, "a", valueCreateBoolean(true));
	associativeArrayAppend(array, "b", valueCreateInt32(1));
	
	index = associativeArrayGetFirstIndexOfKey(array, "a");
	TestCase_assert(index == 0, "Expected 0 but got " SIZE_T_FORMAT, index);
	
	index = associativeArrayGetFirstIndexOfKey(array, "b");
	TestCase_assert(index == 2, "Expected 2 but got " SIZE_T_FORMAT, index);
	
	index = associativeArrayGetFirstIndexOfKey(array, "c");
	TestCase_assert(index == ASSOCIATIVE_ARRAY_NOT_FOUND, "Expected ASSOCIATIVE_ARRAY_NOT_FOUND but got " SIZE_T_FORMAT, index);
	
	associativeArrayDispose(array);
}

static void testGetFirstValueForKey() {
	AssociativeArray * array;
	DataValue * value;
	
	array = associativeArrayCreate();
	associativeArrayAppend(array, "a", valueCreateBoolean(false));
	associativeArrayAppend(array, "a", valueCreateBoolean(true));
	associativeArrayAppend(array, "b", valueCreateInt32(1));
	
	value = associativeArrayGetFirstValueForKey(array, "a");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!value->value.boolean, "Expected false but got true");
	
	value = associativeArrayGetFirstValueForKey(array, "b");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, value->type);
	TestCase_assert(value->value.int32 == 1, "Expected 1 but got %d", value->value.int32);
	
	value = associativeArrayGetFirstValueForKey(array, "c");
	TestCase_assert(value == NULL, "Expected NULL but got %p", value);
	
	associativeArrayDispose(array);
}

static void testInsert() {
	AssociativeArray * array;
	
	array = associativeArrayCreate();
	TestCase_assert(array->count == 0, "Expected 0 but got " SIZE_T_FORMAT, array->count);
	
	associativeArrayInsert(array, 0, "a", valueCreateBoolean(false));
	TestCase_assert(array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(!array->values[0].value.boolean, "Expected false but got true");
	TestCase_assert(!strcmp(array->keys[0], "a"), "Expected \"a\" but got \"%s\"", array->keys[0]);
	
	associativeArrayInsert(array, 0, "b", valueCreateBoolean(true));
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(array->keys[0], "b"), "Expected \"b\" but got \"%s\"", array->keys[0]);
	TestCase_assert(array->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[1].type);
	TestCase_assert(!array->values[1].value.boolean, "Expected false but got true");
	TestCase_assert(!strcmp(array->keys[1], "a"), "Expected \"a\" but got \"%s\"", array->keys[1]);
	
	associativeArrayInsert(array, 1, "c", valueCreateInt32(1));
	TestCase_assert(array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(array->keys[0], "b"), "Expected \"b\" but got \"%s\"", array->keys[0]);
	TestCase_assert(array->values[1].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[1].type);
	TestCase_assert(array->values[1].value.int32 == 1, "Expected 1 but got %d", array->values[2].value.int32);
	TestCase_assert(!strcmp(array->keys[1], "c"), "Expected \"c\" but got \"%s\"", array->keys[1]);
	TestCase_assert(array->values[2].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[2].type);
	TestCase_assert(!array->values[2].value.boolean, "Expected false but got true");
	TestCase_assert(!strcmp(array->keys[2], "a"), "Expected \"a\" but got \"%s\"", array->keys[2]);
	
	associativeArrayInsert(array, 4, "d", valueCreateBoolean(false));
	TestCase_assert(array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, array->count);
	
	associativeArrayDispose(array);
}

static void testCopy() {
	AssociativeArray * array, * copy;
	
	array = associativeArrayCreate();
	copy = associativeArrayCopy(array);
	TestCase_assert(copy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(copy->count == 0, "Expected 0 but got " SIZE_T_FORMAT, copy->count);
	TestCase_assert(copy != array, "Expected pointers to differ, but both are %p", copy);
	associativeArrayDispose(copy);
	
	associativeArrayAppend(array, "a", valueCreateBoolean(false));
	associativeArrayAppend(array, "b", valueCreateBoolean(true));
	associativeArrayAppend(array, "c", valueCreateInt32(1));
	copy = associativeArrayCopy(array);
	
	TestCase_assert(copy->count == 3, "Expected 3 but got " SIZE_T_FORMAT, copy->count);
	TestCase_assert(copy->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, copy->values[0].type);
	TestCase_assert(!copy->values[0].value.boolean, "Expected false but got true");
	TestCase_assert(!strcmp(copy->keys[0], "a"), "Expected \"a\" but got \"%s\"", copy->keys[0]);
	TestCase_assert(copy->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, copy->values[1].type);
	TestCase_assert(copy->values[1].value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(copy->keys[1], "b"), "Expected \"b\" but got \"%s\"", copy->keys[1]);
	TestCase_assert(copy->values[2].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, copy->values[2].type);
	TestCase_assert(copy->values[2].value.int32 == 1, "Expected 1 but got %d", copy->values[2].value.int32);
	TestCase_assert(!strcmp(copy->keys[2], "c"), "Expected \"c\" but got \"%s\"", copy->keys[2]);
	associativeArrayDispose(copy);
	associativeArrayDispose(array);
	
	array = associativeArrayCreate();
	associativeArrayAppend(array, "foo", valueCreateString("foo", 3, true, true));
	copy = associativeArrayCopy(array);
	TestCase_assert(copy->count == 1, "Expected 1 but got " SIZE_T_FORMAT, copy->count);
	TestCase_assert(copy->values[0].type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, copy->values[0].type);
	TestCase_assert(copy->values[0].value.string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(copy->values[0].value.string, "foo"), "Expected \"foo\" but got \"%s\"", copy->values[0].value.string);
	TestCase_assert(copy->values[0].value.string != array->values[0].value.string, "Expected pointers to differ, but both are %p", copy->values[0].value.string);
	TestCase_assert(!strcmp(copy->keys[0], "foo"), "Expected \"foo\" but got \"%s\"", copy->keys[0]);
	
	associativeArrayDispose(copy);
	associativeArrayDispose(array);
}

static void testReplace() {
	AssociativeArray * array;
	
	array = associativeArrayCreate();
	associativeArrayAppend(array, "a", valueCreateBoolean(false));
	associativeArrayAppend(array, "b", valueCreateBoolean(true));
	
	associativeArrayReplace(array, 0, "c", valueCreateInt32(1));
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[0].type);
	TestCase_assert(array->values[0].value.int32 == 1, "Expected 1 but got %d", array->values[0].value.int32);
	TestCase_assert(!strcmp(array->keys[0], "c"), "Expected \"c\" but got \"%s\"", array->keys[0]);
	TestCase_assert(array->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[1].type);
	TestCase_assert(array->values[1].value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(array->keys[1], "b"), "Expected \"b\" but got \"%s\"", array->keys[1]);
	
	associativeArrayReplace(array, 1, "d", valueCreateInt32(2));
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[0].type);
	TestCase_assert(array->values[0].value.int32 == 1, "Expected 1 but got %d", array->values[0].value.int32);
	TestCase_assert(!strcmp(array->keys[0], "c"), "Expected \"c\" but got \"%s\"", array->keys[0]);
	TestCase_assert(array->values[1].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[1].type);
	TestCase_assert(array->values[1].value.int32 == 2, "Expected 2 but got %d", array->values[1].value.int32);
	TestCase_assert(!strcmp(array->keys[1], "d"), "Expected \"d\" but got \"%s\"", array->keys[1]);
	
	associativeArrayDispose(array);
}

static void testDelete() {
	AssociativeArray * array;
	
	array = associativeArrayCreate();
	associativeArrayAppend(array, "a", valueCreateBoolean(false));
	associativeArrayAppend(array, "b", valueCreateBoolean(true));
	associativeArrayAppend(array, "c", valueCreateInt32(1));
	
	associativeArrayDelete(array, 0);
	TestCase_assert(array->count == 2, "Expected 2 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(array->keys[0], "b"), "Expected \"b\" but got \"%s\"", array->keys[0]);
	TestCase_assert(array->values[1].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, array->values[1].type);
	TestCase_assert(array->values[1].value.int32 == 1, "Expected 1 but got %d", array->values[1].value.int32);
	TestCase_assert(!strcmp(array->keys[1], "c"), "Expected \"c\" but got \"%s\"", array->keys[1]);
	
	associativeArrayDelete(array, 1);
	TestCase_assert(array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, array->count);
	TestCase_assert(array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, array->values[0].type);
	TestCase_assert(array->values[0].value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(array->keys[0], "b"), "Expected \"b\" but got \"%s\"", array->keys[0]);
	
	associativeArrayDispose(array);
}

TEST_SUITE(AssociativeArrayTest,
           testCreate,
           testCreateWithKeysAndValues,
           testAppend,
           testGetKeyAtIndex,
           testGetValueAtIndex,
           testGetFirstIndexOfKey,
           testGetFirstValueForKey,
           testInsert,
           testCopy,
           testReplace,
           testDelete);
