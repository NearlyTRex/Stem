#include "preferences/Preferences.h"
#include "preferences/Preferences_private.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

void Preferences_getFilePathPrivate(const char * fileName, char * outFilePath, size_t maxLength) {
}

static void verifyInit(Preferences * preferences) {
	TestCase_assert(preferences->eventDispatcher != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(preferences->dispose == Preferences_dispose, "Expected %p but got %p", Preferences_dispose, preferences->dispose);
}

static void testInit() {
	Preferences preferences, * preferencesPtr;
	
	memset(&preferences, 0x00, sizeof(Preferences));
	Preferences_init(&preferences);
	verifyInit(&preferences);
	preferences.dispose(&preferences);
	
	memset(&preferences, 0xFF, sizeof(Preferences));
	Preferences_init(&preferences);
	verifyInit(&preferences);
	preferences.dispose(&preferences);
	
	preferencesPtr = Preferences_create();
	TestCase_assert(preferencesPtr != NULL, "Expected non-NULL but got NULL");
	if (preferencesPtr == NULL) {return;} // Suppress clang warning
	verifyInit(preferencesPtr);
	preferencesPtr->dispose(preferencesPtr);
}

static void testSetGet() {
	Preferences * preferences;
	DataValue * value;
	
	preferences = Preferences_create();
	
	value = Preferences_get(preferences, "bool_false");
	TestCase_assert(value == NULL, "Expected NULL but got %p", value);
	value = Preferences_get(preferences, "bool_true");
	TestCase_assert(value == NULL, "Expected NULL but got %p", value);
	value = Preferences_get(preferences, "int8_0");
	TestCase_assert(value == NULL, "Expected NULL but got %p", value);
	
	Preferences_set(preferences, "bool_false", valueCreateBoolean(false));
	Preferences_set(preferences, "bool_true", valueCreateBoolean(true));
	Preferences_set(preferences, "int8", valueCreateInt8(0));
	
	value = Preferences_get(preferences, "bool_false");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!valueGetBoolean(value), "Expected false but got true");
	
	value = Preferences_get(preferences, "bool_true");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(valueGetBoolean(value), "Expected true but got false");
	
	value = Preferences_get(preferences, "int8");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value->type);
	TestCase_assert(valueGetInt8(value) == 0, "Expected 0 but got %d", valueGetInt8(value));
	
	Preferences_set(preferences, "int8", valueCreateInt8(1));
	value = Preferences_get(preferences, "int8");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value->type);
	TestCase_assert(valueGetInt8(value) == 1, "Expected 1 but got %d", valueGetInt8(value));
	
	Preferences_dispose(preferences);
}

static void testImport() {
	Preferences * preferences;
	HashTable * hashTable;
	DataValue * value;
	
	preferences = Preferences_create();
	
	hashTable = hashCreateWithKeysAndValues("a", valueCreateBoolean(false), "b", valueCreateInt8(1), NULL);
	Preferences_import(preferences, hashTable);
	hashDispose(hashTable);
	
	value = Preferences_get(preferences, "a");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!value->value.boolean, "Expected false but got true");
	
	value = Preferences_get(preferences, "b");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value->type);
	TestCase_assert(value->value.int8 == 1, "Expected 1 but got %d", value->value.int8);
	
	hashTable = hashCreateWithKeysAndValues("b", valueCreateBoolean(true), "c", valueCreateUInt8(3), NULL);
	Preferences_import(preferences, hashTable);
	hashDispose(hashTable);
	
	value = Preferences_get(preferences, "a");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!value->value.boolean, "Expected false but got true");
	
	value = Preferences_get(preferences, "b");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(value->value.boolean, "Expected true but got false");
	
	value = Preferences_get(preferences, "c");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, value->type);
	TestCase_assert(value->value.uint8 == 3, "Expected 3 but got %u", value->value.uint8);
	
	Preferences_dispose(preferences);
}

static void testExport() {
	Preferences * preferences;
	HashTable * hashTable;
	DataValue * value;
	
	preferences = Preferences_create();
	
	Preferences_set(preferences, "a", valueCreateBoolean(false));
	Preferences_set(preferences, "b", valueCreateInt8(1));
	
	hashTable = Preferences_export(preferences);
	TestCase_assert(hashTable != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashTable->count == 2, "Expected 2 but got " SIZE_T_FORMAT, hashTable->count);
	
	value = hashGet(hashTable, "a");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!value->value.boolean, "Expected false but got true");
	
	value = hashGet(hashTable, "b");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value->type);
	TestCase_assert(value->value.int8 == 1, "Expected 1 but got %d", value->value.int8);
	hashDispose(hashTable);
	
	Preferences_set(preferences, "b", valueCreateBoolean(true));
	Preferences_set(preferences, "c", valueCreateUInt8(3));
	
	hashTable = Preferences_export(preferences);
	TestCase_assert(hashTable != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashTable->count == 3, "Expected 3 but got " SIZE_T_FORMAT, hashTable->count);
	
	value = hashGet(hashTable, "a");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!value->value.boolean, "Expected false but got true");
	
	value = hashGet(hashTable, "b");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(value->value.boolean, "Expected true but got false");
	
	value = hashGet(hashTable, "c");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, value->type);
	TestCase_assert(value->value.uint8 == 3, "Expected 3 but got %u", value->value.uint8);
	hashDispose(hashTable);
	
	Preferences_dispose(preferences);
}

static unsigned int valueChangedCalls;
static const char * expectedKey;
static DataValue expectedValue;
static DataValue expectedPreviousValue;
static DataValue newValue;

static bool valueChanged(const char * eventID, void * eventData, void * context) {
	struct PreferencesEvent * event = eventData;
	
	valueChangedCalls++;
	TestCase_assert(!strcmp(eventID, PREFERENCES_EVENT_VALUE_CHANGED), "Expected " PREFERENCES_EVENT_VALUE_CHANGED " but got %s", eventID);
	TestCase_assert(!strcmp(event->key, expectedKey), "Expected \"%s\" but got \"%s\"", expectedKey, event->key);
	
	TestCase_assert(event->value.type == expectedValue.type, "Expected %d but got %d", expectedValue.type, event->value.type);
	switch (event->value.type) {
		case DATA_TYPE_BOOLEAN:
			TestCase_assert(event->value.value.boolean == expectedValue.value.boolean, "Expected %s", expectedValue.value.boolean ? "true but got false" : "false but got true");
			break;
		case DATA_TYPE_INT8:
			TestCase_assert(event->value.value.int8 == expectedValue.value.int8, "Expected %d but got %d", expectedValue.value.int8, event->value.value.int8);
			break;
		default:
			TestCase_assert(false, "Unimplemented type comparison for %d in valueChanged", event->value.type);
			break;
	}
	
	TestCase_assert(event->previousValue.type == expectedPreviousValue.type, "Expected %d but got %d", expectedPreviousValue.type, event->previousValue.type);
	switch (event->previousValue.type) {
		case DATA_TYPE_BOOLEAN:
			TestCase_assert(event->previousValue.value.boolean == expectedPreviousValue.value.boolean, "Expected %s", expectedPreviousValue.value.boolean ? "true but got false" : "false but got true");
			break;
		case DATA_TYPE_INT8:
			TestCase_assert(event->previousValue.value.int8 == expectedPreviousValue.value.int8, "Expected %d but got %d", expectedPreviousValue.value.int8, event->previousValue.value.int8);
			break;
		default:
			TestCase_assert(false, "Unimplemented type comparison for %d in valueChanged", event->previousValue.type);
			break;
	}
	
	event->value = newValue;
	return true;
}

static void testEvents() {
	Preferences * preferences;
	DataValue * value;
	
	valueChangedCalls = 0;
	preferences = Preferences_create();
	EventDispatcher_registerForEvent(preferences->eventDispatcher, Atom_fromString(PREFERENCES_EVENT_VALUE_CHANGED), valueChanged, NULL);
	
	expectedKey = "boolean";
	expectedValue = valueCreateBoolean(false);
	expectedPreviousValue = valueCreateBoolean(false);
	newValue = valueCreateBoolean(false);
	Preferences_set(preferences, expectedKey, expectedValue);
	TestCase_assert(valueChangedCalls == 1, "Expected 1 but got %u", valueChangedCalls);
	value = Preferences_get(preferences, "boolean");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(!value->value.boolean, "Expected false but got true");
	
	expectedKey = "int8";
	expectedValue = valueCreateInt8(1);
	expectedPreviousValue = valueCreateInt8(0);
	newValue = valueCreateInt8(2);
	Preferences_set(preferences, expectedKey, expectedValue);
	TestCase_assert(valueChangedCalls == 2, "Expected 2 but got %u", valueChangedCalls);
	value = Preferences_get(preferences, "int8");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value->type);
	TestCase_assert(value->value.int8 == 2, "Expected 2 but got %d", value->value.int8);
	
	expectedKey = "int8";
	expectedValue = valueCreateInt8(3);
	expectedPreviousValue = valueCreateInt8(2);
	newValue = valueCreateBoolean(true);
	Preferences_set(preferences, expectedKey, expectedValue);
	TestCase_assert(valueChangedCalls == 3, "Expected 3 but got %u", valueChangedCalls);
	value = Preferences_get(preferences, "int8");
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(value->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value->type);
	TestCase_assert(value->value.boolean, "Expected true but got false");
	
	Preferences_dispose(preferences);
}

TEST_SUITE(PreferencesTest,
           testInit,
           testSetGet,
           testImport,
           testExport,
           testEvents)
