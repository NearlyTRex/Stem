#include "dynamictypes/DataArray.h"
#include "dynamictypes/HashTable.h"
#include "utilities/printfFormats.h"
#include "unittest/TestSuite.h"
#include <float.h>

static void testCreate() {
	HashTable * hashTable;
	
	hashTable = hashCreate();
	TestCase_assert(hashTable != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashTable->count == 0, "Expected 0 but got " SIZE_T_FORMAT, hashTable->count);
	hashDispose(hashTable);
}

static void testCreateWithKeysAndValues() {
	HashTable * hashTable;
	DataValue * value;
	
	hashTable = hashCreateWithKeysAndValues("a", valueCreateBoolean(false), NULL);
	TestCase_assert(hashTable != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashTable->count == 1, "Expected 1 but got " SIZE_T_FORMAT, hashTable->count);
	value = hashGet(hashTable, "a");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!value->value.boolean, "Expected false but got true");
	hashDispose(hashTable);
	
	hashTable = hashCreateWithKeysAndValues("b", valueCreateBoolean(true), "foo", valueCreateInt8(2), NULL);
	TestCase_assert(hashTable != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashTable->count == 2, "Expected 2 but got " SIZE_T_FORMAT, hashTable->count);
	value = hashGet(hashTable, "b");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(value->value.boolean, "Expected true but got false");
	value = hashGet(hashTable, "foo");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value->type);
	TestCase_assert(value->value.int8 == 2, "Expected 2 but got %d", value->value.int8);
	hashDispose(hashTable);
}

static void testAccessors() {
	HashTable * hashTable;
	DataValue * entry;
	bool keyPresent;
	
	hashTable = hashCreate();
	TestCase_assert(hashTable != NULL, "Expected non-NULL but got NULL");
	
	// Verify table empty
	keyPresent = hashHas(hashTable, "value1");
	TestCase_assert(!keyPresent, "Expected false but got true");
	keyPresent = hashHas(hashTable, "value2");
	TestCase_assert(!keyPresent, "Expected false but got true");
	
	// Verify hashGet returns NULL for unset keys
	entry = hashGet(hashTable, "value1");
	TestCase_assert(entry == NULL, "Expected NULL but got %p", entry);
	entry = hashGet(hashTable, "value2");
	TestCase_assert(entry == NULL, "Expected NULL but got %p", entry);
	
	// Set values
	hashSet(hashTable, "value1", valueCreateBoolean(false));
	TestCase_assert(hashTable->count == 1, "Expected 1 but got " SIZE_T_FORMAT, hashTable->count);
	hashSet(hashTable, "value2", valueCreateBoolean(true));
	TestCase_assert(hashTable->count == 2, "Expected 2 but got " SIZE_T_FORMAT, hashTable->count);
	
	// Verify table full
	keyPresent = hashHas(hashTable, "value1");
	TestCase_assert(keyPresent, "Expected true but got false");
	keyPresent = hashHas(hashTable, "value2");
	TestCase_assert(keyPresent, "Expected true but got false");
	
	// Verify hashGet returns correct values for set keys
	entry = hashGet(hashTable, "value1");
	TestCase_assert(entry != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entry->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, entry->type);
	TestCase_assert(!entry->value.boolean, "Expected false but got true");
	entry = hashGet(hashTable, "value2");
	TestCase_assert(entry != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(entry->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, entry->type);
	TestCase_assert(entry->value.boolean, "Expected true but got false");
}

static void testCopy() {
	HashTable * hashTable, * hashTableCopy, * subtable;
	DataValue * string, * stringCopy;
	DataValue * blob, * blobCopy;
	DataValue * subtableValue, * subtableValueCopy;
	
	hashTable = hashCreate();
	hashTableCopy = hashCopy(hashTable);
	TestCase_assert(hashTableCopy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashTableCopy != hashTable, "Expected pointers to differ, but they didn't");
	TestCase_assert(hashTable->count == hashTableCopy->count, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, hashTable->count, hashTableCopy->count);
	hashDispose(hashTable);
	hashDispose(hashTableCopy);
	
	hashTable = hashCreate();
	hashSet(hashTable, "a", valueCreateString("hello", DATA_USE_STRLEN, true, true));
	hashSet(hashTable, "b", valueCreateBlob("foo", 3, true, true));
	subtable = hashCreate();
	hashSet(subtable, "c", valueCreateString("foo", DATA_USE_STRLEN, true, true));
	hashSet(subtable, "d", valueCreatePointer((void *) 0x1234));
	hashSet(hashTable, "c", valueCreateHashTable(subtable, true, true));
	hashDispose(subtable);
	
	hashTableCopy = hashCopy(hashTable);
	TestCase_assert(hashTableCopy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashTableCopy != hashTable, "Expected pointers to differ, but they didn't");
	
	TestCase_assert(hashTable->count == hashTableCopy->count, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, hashTable->count, hashTableCopy->count);
	
	string = hashGet(hashTable, "a");
	stringCopy = hashGet(hashTableCopy, "a");
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(stringCopy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(string->type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, string->type);
	TestCase_assert(stringCopy->type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, stringCopy->type);
	TestCase_assert(!strcmp(string->value.string, "hello"), "Expected \"hello\" but got \"%s\"", string->value.string);
	TestCase_assert(!strcmp(stringCopy->value.string, "hello"), "Expected \"hello\" but got \"%s\"", stringCopy->value.string);
	TestCase_assert(string->value.string != stringCopy->value.string, "Expected pointers to differ, but they didn't");
	
	blob = hashGet(hashTable, "b");
	blobCopy = hashGet(hashTableCopy, "b");
	TestCase_assert(blob != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(blobCopy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(blob->value.blob.length == 3, "Expected 3 but got " SIZE_T_FORMAT, blob->value.blob.length);
	TestCase_assert(blobCopy->value.blob.length == 3, "Expected 3 but got " SIZE_T_FORMAT, blobCopy->value.blob.length);
	TestCase_assert(!memcmp(blob->value.blob.bytes, "foo", 3), "Expected \"foo\" but got \"%.*s\"", (unsigned int) blob->value.blob.length, (char *) blob->value.blob.bytes);
	TestCase_assert(!memcmp(blobCopy->value.blob.bytes, "foo", 3), "Expected \"foo\" but got \"%.*s\"", (unsigned int) blobCopy->value.blob.length, (char *) blobCopy->value.blob.bytes);
	TestCase_assert(blob->value.blob.bytes != blobCopy->value.blob.bytes, "Expected pointers to differ, but they didn't");
	
	subtableValue = hashGet(hashTable, "c");
	subtableValueCopy = hashGet(hashTableCopy, "c");
	TestCase_assert(subtableValue != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(subtableValueCopy != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(subtableValue->value.hashTable->count == 2, "Expected 2 but got " SIZE_T_FORMAT, subtableValue->value.hashTable->count);
	TestCase_assert(subtableValueCopy->value.hashTable->count == 2, "Expected 2 but got " SIZE_T_FORMAT, subtableValueCopy->value.hashTable->count);
	TestCase_assert(subtableValue->value.hashTable != subtableValueCopy->value.hashTable, "Expected pointers to differ, but they didn't");
	
	hashDispose(hashTable);
	hashDispose(hashTableCopy);
}

static void testReplaceValues() {
	HashTable * hash;
	const char ** keys;
	size_t count;
	DataValue * int32Value;
	DataValue * stringValue;
	
	hash = hashCreate();
	hashSet(hash, "a", valueCreateInt32(1));
	hashSet(hash, "b", valueCreateInt32(2));
	TestCase_assert(hash->count == 2, "Expected 2 but got " SIZE_T_FORMAT, hash->count);
	int32Value = hashGet(hash, "b");
	TestCase_assert(int32Value->value.int32 == 2, "Expected 2 but got %d", int32Value->value.int32);
	
	hashSet(hash, "b", valueCreateInt32(3));
	TestCase_assert(hash->count == 2, "Expected 2 but got " SIZE_T_FORMAT, hash->count);
	int32Value = hashGet(hash, "b");
	TestCase_assert(int32Value->value.int32 == 3, "Expected 3 but got %d", int32Value->value.int32);
	
	hashSet(hash, "a", valueCreateString("hello", DATA_USE_STRLEN, false, false));
	TestCase_assert(hash->count == 2, "Expected 2 but got " SIZE_T_FORMAT, hash->count);
	stringValue = hashGet(hash, "a");
	TestCase_assert(stringValue != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(stringValue->type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, stringValue->type);
	TestCase_assert(!strcmp(stringValue->value.string, "hello"), "Expected \"hello\" but got \"%s\"", stringValue->value.string);
	
	hashSet(hash, "a", valueCreateInt32(1));
	TestCase_assert(hash->count == 2, "Expected 2 but got " SIZE_T_FORMAT, hash->count);
	int32Value = hashGet(hash, "a");
	TestCase_assert(int32Value->value.int32 == 1, "Expected 1 but got %d", int32Value->value.int32);
	
	keys = hashGetKeys(hash, &count);
	TestCase_assert(count == 2, "Expected 2 but got " SIZE_T_FORMAT, count);
	TestCase_assert((!strcmp(keys[0], "a") && !strcmp(keys[1], "b")) || (!strcmp(keys[0], "b") && !strcmp(keys[1], "a")), "Expected \"a\" and \"b\", but got \"%s\" and \"%s\"", keys[0], keys[1]);
	free(keys);
}

static void testDeleteValues() {
	HashTable * hash;
	const char ** keys;
	size_t count;
	bool found;
	
	hash = hashCreate();
	hashSet(hash, "a", valueCreateInt32(1));
	hashSet(hash, "b", valueCreateInt32(2));
	hashSet(hash, "c", valueCreateInt32(3));
	TestCase_assert(hash->count == 3, "Expected 3 but got " SIZE_T_FORMAT, hash->count);
	
	found = hashDelete(hash, "d");
	TestCase_assert(!found, "Expected false but got true");
	
	found = hashDelete(hash, "b");
	TestCase_assert(found, "Expected true but got false");
	TestCase_assert(hash->count == 2, "Expected 2 but got " SIZE_T_FORMAT, hash->count);
	TestCase_assert(!hashHas(hash, "b"), "Expected false but got true");
	keys = hashGetKeys(hash, &count);
	TestCase_assert(count == 2, "Expected 2 but got " SIZE_T_FORMAT, count);
	TestCase_assert((!strcmp(keys[0], "a") && !strcmp(keys[1], "c")) || (!strcmp(keys[0], "c") && !strcmp(keys[1], "a")), "Expected \"a\" and \"c\", but got \"%s\" and \"%s\"", keys[0], keys[1]);
	free(keys);
	
	found = hashDelete(hash, "b");
	TestCase_assert(!found, "Expected false but got true");
	
	found = hashDelete(hash, "a");
	TestCase_assert(found, "Expected true but got false");
	TestCase_assert(hash->count == 1, "Expected 1 but got " SIZE_T_FORMAT, hash->count);
	TestCase_assert(!hashHas(hash, "a"), "Expected false but got true");
	keys = hashGetKeys(hash, &count);
	TestCase_assert(count == 1, "Expected 1 but got " SIZE_T_FORMAT, count);
	TestCase_assert(!strcmp(keys[0], "c"), "Expected \"c\" but got \"%s\"", keys[0]);
	free(keys);
	
	hashDispose(hash);
}

static void testGetKeys() {
	HashTable * hashTable;
	const char ** keys;
	size_t count;
	
	hashTable = hashCreate();
	hashSet(hashTable, "a", valueCreateBoolean(false));
	count = 0;
	keys = hashGetKeys(hashTable, &count);
	
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(count == 1, "Expected 1 but got " SIZE_T_FORMAT, count);
	TestCase_assert(!strcmp(keys[0], "a"), "Expected \"a\" but got \"%s\"", keys[0]);
	
	hashDispose(hashTable);
	free(keys);
	
	hashTable = hashCreate();
	hashSet(hashTable, "bar", valueCreateBoolean(true));
	hashSet(hashTable, "foo", valueCreateInt32(1));
	count = 0;
	keys = hashGetKeys(hashTable, &count);
	
	TestCase_assert(keys != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(count == 2, "Expected 2 but got " SIZE_T_FORMAT, count);
	TestCase_assert((!strcmp(keys[0], "foo") && !strcmp(keys[1], "bar")) || (!strcmp(keys[0], "bar") && !strcmp(keys[1], "foo")), "Expected \"foo\" and \"bar\" but got \"%s\" and \"%s\"", keys[0], keys[1]);
	
	hashDispose(hashTable);
	free(keys);
}

TEST_SUITE(HashTableTest,
           testCreate,
           testCreateWithKeysAndValues,
           testAccessors,
           testCopy,
           testReplaceValues,
           testDeleteValues,
           testGetKeys)
