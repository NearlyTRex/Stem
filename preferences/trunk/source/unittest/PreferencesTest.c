#include "preferences/Preferences.h"
#include "preferences/Preferences_private.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

static unsigned int loadCalls;
static unsigned int saveCalls;

void Preferences_loadPrivate(Preferences * self) {
	char data1[] = {0xFF, 0x00, 0x7F};
	char data2[] = {0xAA};
	
	loadCalls++;
	Preferences_setIntegerPrivate(self, "integer1", -3);
	Preferences_setIntegerPrivate(self, "integer2", 0);
	Preferences_setFloatPrivate(self, "float1", -4.5f);
	Preferences_setFloatPrivate(self, "float2", 1.0f);
	Preferences_setBooleanPrivate(self, "boolean1", false);
	Preferences_setBooleanPrivate(self, "boolean2", true);
	Preferences_setStringPrivate(self, "string1", "bar");
	Preferences_setStringPrivate(self, "string2", "baz");
	Preferences_setDataPrivate(self, "data1", data1, sizeof(data1));
	Preferences_setDataPrivate(self, "data2", data2, sizeof(data2));
}

void Preferences_savePrivate(Preferences * self) {
	saveCalls++;
}

void Preferences_getFilePathPrivate(const char * fileName, char * outFilePath) {
}

static void testInit() {
	Preferences preferences, * preferencesPtr;
	const char * identifier1 = "identifier1", * identifier2 = "identifier2";
	
	memset(&preferences, 0, sizeof(Preferences));
	Preferences_init(&preferences, identifier1);
	TestCase_assert(preferences.eventDispatcher != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(preferences.identifier != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(preferences.identifier != identifier1, "Pointers expected to differ but didn't");
	TestCase_assert(!strcmp(preferences.identifier, identifier1), "Expected \"identifier1\" but got \"%s\"", preferences.identifier);
	TestCase_assert(preferences.dispose == Preferences_dispose, "Expected %p but got %p", Preferences_dispose, preferences.dispose);
	preferences.dispose(&preferences);
	
	preferencesPtr = Preferences_create(identifier2);
	TestCase_assert(preferencesPtr != NULL, "Expected non-NULL but got NULL");
	if (preferencesPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(preferencesPtr->identifier != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(preferencesPtr->identifier != identifier2, "Pointers expected to differ but didn't");
	TestCase_assert(!strcmp(preferencesPtr->identifier, identifier2), "Expected \"identifier2\" but got \"%s\"", preferencesPtr->identifier);
	TestCase_assert(preferencesPtr->eventDispatcher != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(preferencesPtr->dispose == Preferences_dispose, "Expected %p but got %p", Preferences_dispose, preferencesPtr->dispose);
	preferencesPtr->dispose(preferencesPtr);
}

static void testLoad() {
	Preferences * preferences;
	char defaultData1[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	char defaultData2[] = {0xFE, 0xED, 0xFA, 0xCE};
	void * data;
	size_t dataSize;
	
	preferences = Preferences_create("preferences_unittest");
	
	TestCase_assert(Preferences_getInteger(preferences, "integer1") == 0, "Expected 0 but got %d", Preferences_getInteger(preferences, "integer1"));
	TestCase_assert(Preferences_getInteger(preferences, "integer2") == 0, "Expected 0 but got %d", Preferences_getInteger(preferences, "integer2"));
	TestCase_assert(Preferences_getFloat(preferences, "float1") == 0.0f, "Expected 0.0 but got %f", Preferences_getFloat(preferences, "float1"));
	TestCase_assert(Preferences_getFloat(preferences, "float2") == 0.0f, "Expected 0.0 but got %f", Preferences_getFloat(preferences, "float2"));
	TestCase_assert(Preferences_getBoolean(preferences, "boolean1") == false, "Expected false but got true");
	TestCase_assert(Preferences_getBoolean(preferences, "boolean2") == false, "Expected false but got true");
	TestCase_assert(Preferences_getString(preferences, "string1") == NULL, "Expected NULL but got %s", Preferences_getString(preferences, "string1"));
	TestCase_assert(Preferences_getString(preferences, "string2") == NULL, "Expected NULL but got %s", Preferences_getString(preferences, "string2"));
	TestCase_assert(Preferences_getData(preferences, "data1", NULL) == NULL, "Expected NULL but got %p", Preferences_getData(preferences, "data1", NULL));
	TestCase_assert(Preferences_getData(preferences, "data2", NULL) == NULL, "Expected NULL but got %p", Preferences_getData(preferences, "data2", NULL));
	
	Preferences_addInteger(preferences, "integer1", 1);
	Preferences_addInteger(preferences, "integer2", -2);
	Preferences_addFloat(preferences, "float1", 4.0f);
	Preferences_addFloat(preferences, "float2", -0.125f);
	Preferences_addBoolean(preferences, "boolean1", true);
	Preferences_addBoolean(preferences, "boolean2", false);
	Preferences_addString(preferences, "string1", "Hello, world!");
	Preferences_addString(preferences, "string2", "foo");
	Preferences_addData(preferences, "data1", defaultData1, sizeof(defaultData1));
	Preferences_addData(preferences, "data2", defaultData2, sizeof(defaultData2));
	
	TestCase_assert(Preferences_getInteger(preferences, "integer1") == 1, "Expected 1 but got %d", Preferences_getInteger(preferences, "integer1"));
	TestCase_assert(Preferences_getInteger(preferences, "integer2") == -2, "Expected -2 but got %d", Preferences_getInteger(preferences, "integer2"));
	TestCase_assert(Preferences_getFloat(preferences, "float1") == 4.0f, "Expected 4.0 but got %f", Preferences_getFloat(preferences, "float1"));
	TestCase_assert(Preferences_getFloat(preferences, "float2") == -0.125f, "Expected -0.125 but got %f", Preferences_getFloat(preferences, "float2"));
	TestCase_assert(Preferences_getBoolean(preferences, "boolean1") == true, "Expected true but got false");
	TestCase_assert(Preferences_getBoolean(preferences, "boolean2") == false, "Expected false but got true");
	TestCase_assert(Preferences_getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string1"), "Hello, world!"), "Expected \"Hello, world!\" but got \"%s\"", Preferences_getString(preferences, "string1"));
	TestCase_assert(Preferences_getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string2"), "foo"), "Expected \"foo\" but got \"%s\"", Preferences_getString(preferences, "string2"));
	data = Preferences_getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 5, "Expected 5 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\x00\x01\x02\x03\x04", 5), "Data didn't match expected value");
	data = Preferences_getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 4, "Expected 4 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\xFE\xED\xFA\xCE", 4), "Data didn't match expected value");
	
	loadCalls = 0;
	Preferences_load(preferences);
	TestCase_assert(loadCalls == 1, "Expected 1 but got %u", loadCalls);
	
	TestCase_assert(Preferences_getInteger(preferences, "integer1") == -3, "Expected -3 but got %d", Preferences_getInteger(preferences, "integer1"));
	TestCase_assert(Preferences_getInteger(preferences, "integer2") == 0, "Expected 0 but got %d", Preferences_getInteger(preferences, "integer2"));
	TestCase_assert(Preferences_getFloat(preferences, "float1") == -4.5f, "Expected -4.5 but got %f", Preferences_getFloat(preferences, "float1"));
	TestCase_assert(Preferences_getFloat(preferences, "float2") == 1.0f, "Expected 1.0 but got %f", Preferences_getFloat(preferences, "float2"));
	TestCase_assert(Preferences_getBoolean(preferences, "boolean1") == false, "Expected false but got true");
	TestCase_assert(Preferences_getBoolean(preferences, "boolean2") == true, "Expected true but got false");
	TestCase_assert(Preferences_getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string1"), "bar"), "Expected \"bar\" but got \"%s\"", Preferences_getString(preferences, "string1"));
	TestCase_assert(Preferences_getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string2"), "baz"), "Expected \"baz\" but got \"%s\"", Preferences_getString(preferences, "string2"));
	data = Preferences_getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 3, "Expected 3 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\xFF\x00\x7F", 3), "Data didn't match expected value");
	data = Preferences_getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 1, "Expected 1 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\xAA", 1), "Data didn't match expected value");
	
	Preferences_dispose(preferences);
}

static void testLoadDefaults() {
	Preferences * preferences;
	char defaultData1[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	char defaultData2[] = {0xFE, 0xED, 0xFA, 0xCE};
	void * data;
	size_t dataSize;
	
	preferences = Preferences_create("preferences_unittest");
	Preferences_addInteger(preferences, "integer1", 1);
	Preferences_addInteger(preferences, "integer2", -2);
	Preferences_addFloat(preferences, "float1", 4.0f);
	Preferences_addFloat(preferences, "float2", -0.125f);
	Preferences_addBoolean(preferences, "boolean1", true);
	Preferences_addBoolean(preferences, "boolean2", false);
	Preferences_addString(preferences, "string1", "Hello, world!");
	Preferences_addString(preferences, "string2", "foo");
	Preferences_addData(preferences, "data1", defaultData1, sizeof(defaultData1));
	Preferences_addData(preferences, "data2", defaultData2, sizeof(defaultData2));
	Preferences_load(preferences);
	
	Preferences_loadDefaultValue(preferences, "integer1");
	Preferences_loadDefaultValue(preferences, "float1");
	Preferences_loadDefaultValue(preferences, "boolean1");
	Preferences_loadDefaultValue(preferences, "string1");
	Preferences_loadDefaultValue(preferences, "data1");
	TestCase_assert(Preferences_getInteger(preferences, "integer1") == 1, "Expected 1 but got %d", Preferences_getInteger(preferences, "integer1"));
	TestCase_assert(Preferences_getInteger(preferences, "integer2") == 0, "Expected 0 but got %d", Preferences_getInteger(preferences, "integer2"));
	TestCase_assert(Preferences_getFloat(preferences, "float1") == 4.0f, "Expected 4.0 but got %f", Preferences_getFloat(preferences, "float1"));
	TestCase_assert(Preferences_getFloat(preferences, "float2") == 1.0f, "Expected 1.0 but got %f", Preferences_getFloat(preferences, "float2"));
	TestCase_assert(Preferences_getBoolean(preferences, "boolean1") == true, "Expected true but got false");
	TestCase_assert(Preferences_getBoolean(preferences, "boolean2") == true, "Expected true but got false");
	TestCase_assert(Preferences_getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string1"), "Hello, world!"), "Expected \"Hello, world!\" but got \"%s\"", Preferences_getString(preferences, "string1"));
	TestCase_assert(Preferences_getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string2"), "baz"), "Expected \"baz\" but got \"%s\"", Preferences_getString(preferences, "string2"));
	data = Preferences_getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 5, "Expected 5 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\x00\x01\x02\x03\x04", 5), "Data didn't match expected value");
	data = Preferences_getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 1, "Expected 1 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\xAA", 1), "Data didn't match expected value");
	
	Preferences_loadDefaultValues(preferences);
	TestCase_assert(Preferences_getInteger(preferences, "integer1") == 1, "Expected 1 but got %d", Preferences_getInteger(preferences, "integer1"));
	TestCase_assert(Preferences_getInteger(preferences, "integer2") == -2, "Expected -2 but got %d", Preferences_getInteger(preferences, "integer2"));
	TestCase_assert(Preferences_getFloat(preferences, "float1") == 4.0f, "Expected 4.0 but got %f", Preferences_getFloat(preferences, "float1"));
	TestCase_assert(Preferences_getFloat(preferences, "float2") == -0.125f, "Expected -0.125 but got %f", Preferences_getFloat(preferences, "float2"));
	TestCase_assert(Preferences_getBoolean(preferences, "boolean1") == true, "Expected true but got false");
	TestCase_assert(Preferences_getBoolean(preferences, "boolean2") == false, "Expected false but got true");
	TestCase_assert(Preferences_getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string1"), "Hello, world!"), "Expected \"Hello, world!\" but got \"%s\"", Preferences_getString(preferences, "string1"));
	TestCase_assert(Preferences_getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string2"), "foo"), "Expected \"foo\" but got \"%s\"", Preferences_getString(preferences, "string2"));
	data = Preferences_getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 5, "Expected 5 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\x00\x01\x02\x03\x04", 5), "Data didn't match expected value");
	data = Preferences_getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 4, "Expected 4 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\xFE\xED\xFA\xCE", 4), "Data didn't match expected value");
	
	Preferences_dispose(preferences);
}

static void testSet() {
	Preferences * preferences;
	char defaultData1[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	char defaultData2[] = {0xFE, 0xED, 0xFA, 0xCE};
	char data1[] = {0xFF, 0x7F, 0x00};
	char data2[] = {0xCC, 0xB7};
	void * data;
	size_t dataSize;
	
	preferences = Preferences_create("preferences_unittest");
	Preferences_addInteger(preferences, "integer1", 1);
	Preferences_addInteger(preferences, "integer2", -2);
	Preferences_addFloat(preferences, "float1", 4.0f);
	Preferences_addFloat(preferences, "float2", -0.125f);
	Preferences_addBoolean(preferences, "boolean1", true);
	Preferences_addBoolean(preferences, "boolean2", false);
	Preferences_addString(preferences, "string1", "Hello, world!");
	Preferences_addString(preferences, "string2", "foo");
	Preferences_addData(preferences, "data1", defaultData1, sizeof(defaultData1));
	Preferences_addData(preferences, "data2", defaultData2, sizeof(defaultData2));
	
	Preferences_setInteger(preferences, "integer1", 10);
	Preferences_setInteger(preferences, "integer2", 3);
	Preferences_setFloat(preferences, "float1", 1.25f);
	Preferences_setFloat(preferences, "float2", 8.5f);
	Preferences_setBoolean(preferences, "boolean1", false);
	Preferences_setBoolean(preferences, "boolean2", true);
	Preferences_setString(preferences, "string1", "bar");
	Preferences_setString(preferences, "string2", "baz");
	Preferences_setData(preferences, "data1", data1, sizeof(data1));
	Preferences_setData(preferences, "data2", data2, sizeof(data2));
	
	TestCase_assert(Preferences_getInteger(preferences, "integer1") == 10, "Expected 10 but got %d", Preferences_getInteger(preferences, "integer1"));
	TestCase_assert(Preferences_getInteger(preferences, "integer2") == 3, "Expected 3 but got %d", Preferences_getInteger(preferences, "integer2"));
	TestCase_assert(Preferences_getFloat(preferences, "float1") == 1.25f, "Expected 1.25 but got %f", Preferences_getFloat(preferences, "float1"));
	TestCase_assert(Preferences_getFloat(preferences, "float2") == 8.5f, "Expected 8.5 but got %f", Preferences_getFloat(preferences, "float2"));
	TestCase_assert(Preferences_getBoolean(preferences, "boolean1") == false, "Expected false but got false");
	TestCase_assert(Preferences_getBoolean(preferences, "boolean2") == true, "Expected true but got true");
	TestCase_assert(Preferences_getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string1"), "bar"), "Expected \"bar\" but got \"%s\"", Preferences_getString(preferences, "string1"));
	TestCase_assert(Preferences_getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string2"), "baz"), "Expected \"baz\" but got \"%s\"", Preferences_getString(preferences, "string2"));
	data = Preferences_getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 3, "Expected 3 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\xFF\x7F\x00", 3), "Data didn't match expected value");
	data = Preferences_getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 2, "Expected 2 but got " SIZE_T_FORMAT, dataSize);
	TestCase_assert(!memcmp(data, "\xCC\xB7", 2), "Data didn't match expected value");
	
	Preferences_dispose(preferences);
}

static void testSave() {
	Preferences * preferences;
	char defaultData1[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	char defaultData2[] = {0xFE, 0xED, 0xFA, 0xCE};
	char data1[] = {0xFF, 0x7F, 0x00};
	char data2[] = {0xCC, 0xB7};
	
	preferences = Preferences_create("preferences_unittest");
	Preferences_addInteger(preferences, "integer1", 1);
	Preferences_addInteger(preferences, "integer2", -2);
	Preferences_addFloat(preferences, "float1", 4.0f);
	Preferences_addFloat(preferences, "float2", -0.125f);
	Preferences_addBoolean(preferences, "boolean1", true);
	Preferences_addBoolean(preferences, "boolean2", false);
	Preferences_addString(preferences, "string1", "Hello, world!");
	Preferences_addString(preferences, "string2", "foo");
	Preferences_addData(preferences, "data1", defaultData1, sizeof(defaultData1));
	Preferences_addData(preferences, "data2", defaultData2, sizeof(defaultData2));
	
	Preferences_setInteger(preferences, "integer1", 10);
	Preferences_setInteger(preferences, "integer2", 3);
	Preferences_setFloat(preferences, "float1", 1.25f);
	Preferences_setFloat(preferences, "float2", 8.5f);
	Preferences_setBoolean(preferences, "boolean1", false);
	Preferences_setBoolean(preferences, "boolean2", true);
	Preferences_setString(preferences, "string1", "bar");
	Preferences_setString(preferences, "string2", "baz");
	Preferences_setData(preferences, "data1", data1, sizeof(data1));
	Preferences_setData(preferences, "data2", data2, sizeof(data2));
	
	saveCalls = 0;
	Preferences_save(preferences);
	TestCase_assert(saveCalls == 1, "Expected 1 but got %u", saveCalls);
	
	Preferences_dispose(preferences);
}

static unsigned int valueChangedCalls;
static const char * expectedName;
static enum PreferencesType expectedType;
static union PreferencesValue expectedValue;
static union PreferencesValue expectedPreviousValue;
static union PreferencesValue newValue;

static bool valueChanged(const char * eventID, void * eventData, void * context) {
	struct PreferencesEvent * event = eventData;
	
	valueChangedCalls++;
	TestCase_assert(!strcmp(eventID, PREFERENCES_EVENT_VALUE_CHANGED), "Expected " PREFERENCES_EVENT_VALUE_CHANGED " but got %s", eventID);
	TestCase_assert(!strcmp(event->name, expectedName), "Expected %s but got %s", expectedName, event->name);
	TestCase_assert(event->type == expectedType, "Expected %d but got %d", expectedType, event->type);
	
	switch (event->type) {
		case PREFERENCES_TYPE_INTEGER:
			TestCase_assert(event->value.integer == expectedValue.integer, "Expected %d but got %d", expectedValue.integer, event->value.integer);
			TestCase_assert(event->previousValue.integer == expectedPreviousValue.integer, "Expected %d but got %d", expectedPreviousValue.integer, event->previousValue.integer);
			event->value.integer = newValue.integer;
			break;
			
		case PREFERENCES_TYPE_FLOAT:
			TestCase_assert(event->value.number == expectedValue.number, "Expected %f but got %f", expectedValue.number, event->value.number);
			TestCase_assert(event->previousValue.number == expectedPreviousValue.number, "Expected %f but got %f", expectedPreviousValue.number, event->previousValue.number);
			event->value.number = newValue.number;
			break;
			
		case PREFERENCES_TYPE_BOOLEAN:
			TestCase_assert(event->value.boolean == expectedValue.boolean, "Expected %s but got %s", expectedValue.boolean ? "true" : "false", event->value.boolean ? "true" : "false");
			TestCase_assert(event->previousValue.boolean == expectedPreviousValue.boolean, "Expected %s but got %s", expectedPreviousValue.boolean ? "true" : "false", event->previousValue.boolean ? "true" : "false");
			event->value.boolean = newValue.boolean;
			break;
			
		case PREFERENCES_TYPE_STRING:
			TestCase_assert(!strcmp(event->value.string, expectedValue.string), "Expected %s but got %s", expectedValue.string, event->value.string);
			TestCase_assert(!strcmp(event->previousValue.string, expectedPreviousValue.string), "Expected %s but got %s", expectedPreviousValue.string, event->previousValue.string);
			event->value.string = newValue.string;
			break;
			
		case PREFERENCES_TYPE_DATA:
			TestCase_assert(event->value.data.length == expectedValue.data.length, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, expectedValue.data.length, event->value.data.length);
			TestCase_assert(!memcmp(event->value.data.bytes, expectedValue.data.bytes, expectedValue.data.length), "Expected data didn't match actual");
			TestCase_assert(event->previousValue.data.length == expectedPreviousValue.data.length, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, expectedPreviousValue.data.length, event->previousValue.data.length);
			TestCase_assert(!memcmp(event->previousValue.data.bytes, expectedPreviousValue.data.bytes, expectedPreviousValue.data.length), "Expected data didn't match actual");
			event->value.data.length = newValue.data.length;
			event->value.data.bytes = newValue.data.bytes;
			break;
	}
	
	return true;
}

static void testEvents() {
	Preferences * preferences;
	char defaultData[] = {0x01}, data1[] = {0xFF, 0x00}, data2[] = {0x7F, 0x3F, 0xBF};
	void * data;
	size_t dataLength;
	
	valueChangedCalls = 0;
	preferences = Preferences_create("preferences_unittest");
	EventDispatcher_registerForEvent(preferences->eventDispatcher, Atom_fromString(PREFERENCES_EVENT_VALUE_CHANGED), valueChanged, NULL);
	Preferences_addInteger(preferences, "integer1", 1);
	Preferences_addFloat(preferences, "float1", 1.0f);
	Preferences_addBoolean(preferences, "boolean1", true);
	Preferences_addString(preferences, "string1", "");
	Preferences_addData(preferences, "data1", defaultData, sizeof(defaultData));
	
	TestCase_assert(valueChangedCalls == 0, "Expected 0 but got %u", valueChangedCalls);
	
	expectedName = "integer1";
	expectedType = PREFERENCES_TYPE_INTEGER;
	expectedValue.integer = 2;
	expectedPreviousValue.integer = 1;
	newValue.integer = 3;
	Preferences_setInteger(preferences, "integer1", 2);
	TestCase_assert(valueChangedCalls == 1, "Expected 1 but got %u", valueChangedCalls);
	TestCase_assert(Preferences_getInteger(preferences, "integer1") == 3, "Expected 3 but got %d", Preferences_getInteger(preferences, "integer1"));
	
	expectedName = "float1";
	expectedType = PREFERENCES_TYPE_FLOAT;
	expectedValue.number = 2.0f;
	expectedPreviousValue.number = 1.0f;
	newValue.number = 3.0f;
	Preferences_setFloat(preferences, "float1", 2.0f);
	TestCase_assert(valueChangedCalls == 2, "Expected 2 but got %u", valueChangedCalls);
	TestCase_assert(Preferences_getFloat(preferences, "float1") == 3.0f, "Expected 3.0f but got %f", Preferences_getFloat(preferences, "float1"));
	
	expectedName = "boolean1";
	expectedType = PREFERENCES_TYPE_BOOLEAN;
	expectedValue.boolean = true;
	expectedPreviousValue.boolean = true;
	newValue.boolean = false;
	Preferences_setBoolean(preferences, "boolean1", true);
	TestCase_assert(valueChangedCalls == 3, "Expected 3 but got %u", valueChangedCalls);
	TestCase_assert(Preferences_getBoolean(preferences, "boolean1") == false, "Expected false but got true");
	
	expectedName = "string1";
	expectedType = PREFERENCES_TYPE_STRING;
	expectedValue.string = "foo";
	expectedPreviousValue.string = "";
	newValue.string = "bar";
	Preferences_setString(preferences, "string1", "foo");
	TestCase_assert(valueChangedCalls == 4, "Expected 4 but got %u", valueChangedCalls);
	TestCase_assert(!strcmp(Preferences_getString(preferences, "string1"), "bar"), "Expected \"bar\" but got \"%s\"", Preferences_getString(preferences, "string1"));
	
	expectedName = "data1";
	expectedType = PREFERENCES_TYPE_DATA;
	expectedValue.data.length = 2;
	expectedValue.data.bytes = data1;
	expectedPreviousValue.data.length = 1;
	expectedPreviousValue.data.bytes = defaultData;
	newValue.data.length = 3;
	newValue.data.bytes = data2;
	Preferences_setData(preferences, "data1", data1, 2);
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	data = Preferences_getData(preferences, "data1", &dataLength);
	TestCase_assert(dataLength == 3, "Expected 3 but got " SIZE_T_FORMAT, dataLength);
	TestCase_assert(!memcmp(data, data2, 3), "Expected data didn't match actual");
	
	Preferences_load(preferences);
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	
	Preferences_loadDefaultValues(preferences);
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	
	Preferences_loadDefaultValue(preferences, "integer1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	Preferences_loadDefaultValue(preferences, "float1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	Preferences_loadDefaultValue(preferences, "boolean1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	Preferences_loadDefaultValue(preferences, "string1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	Preferences_loadDefaultValue(preferences, "data1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	
	Preferences_dispose(preferences);
}

static void testDuplicateNamesChecked() {
	Preferences * preferences;
	
	preferences = Preferences_create("preferences_unittest");
	
	TestCase_assert(preferences->valueCount == 0, "Expected 0 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addInteger(preferences, "integer", 0);
	TestCase_assert(preferences->valueCount == 1, "Expected 1 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addFloat(preferences, "float", 0.0f);
	TestCase_assert(preferences->valueCount == 2, "Expected 2 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addBoolean(preferences, "boolean", false);
	TestCase_assert(preferences->valueCount == 3, "Expected 3 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addString(preferences, "string", "");
	TestCase_assert(preferences->valueCount == 4, "Expected 4 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addData(preferences, "data", "", 1);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got " SIZE_T_FORMAT, preferences->valueCount);
	
	Preferences_addInteger(preferences, "integer", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addInteger(preferences, "float", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addInteger(preferences, "boolean", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addInteger(preferences, "string", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addInteger(preferences, "data", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addInteger(preferences, "integer2", 0);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got " SIZE_T_FORMAT, preferences->valueCount);
	
	Preferences_addFloat(preferences, "integer", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addFloat(preferences, "float", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addFloat(preferences, "boolean", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addFloat(preferences, "string", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addFloat(preferences, "data", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addFloat(preferences, "float2", 0.0f);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got " SIZE_T_FORMAT, preferences->valueCount);
	
	Preferences_addBoolean(preferences, "integer", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addBoolean(preferences, "float", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addBoolean(preferences, "boolean", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addBoolean(preferences, "string", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addBoolean(preferences, "data", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addBoolean(preferences, "boolean2", false);
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got " SIZE_T_FORMAT, preferences->valueCount);
	
	Preferences_addString(preferences, "integer", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addString(preferences, "float", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addString(preferences, "boolean", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addString(preferences, "string", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addString(preferences, "data", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addString(preferences, "string2", "");
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got " SIZE_T_FORMAT, preferences->valueCount);
	
	Preferences_addData(preferences, "integer", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addData(preferences, "float", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addData(preferences, "boolean", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addData(preferences, "string", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addData(preferences, "data", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got " SIZE_T_FORMAT, preferences->valueCount);
	Preferences_addData(preferences, "data2", "", 1);
	TestCase_assert(preferences->valueCount == 10, "Expected 10 but got " SIZE_T_FORMAT, preferences->valueCount);
	
	Preferences_dispose(preferences);
}

static void testValueTypesChecked() {
	Preferences * preferences;
	char defaultData[] = {0x01};
	const char * stringPtr;
	const void * dataPtr;
	
	preferences = Preferences_create("preferences_unittest");
	Preferences_addInteger(preferences, "integer", 1);
	Preferences_addFloat(preferences, "float", 1.0f);
	Preferences_addBoolean(preferences, "boolean", true);
	Preferences_addString(preferences, "string", "\x01");
	Preferences_addData(preferences, "data", defaultData, sizeof(defaultData));
	
	stringPtr = Preferences_getString(preferences, "string");
	dataPtr = Preferences_getData(preferences, "data", NULL);
	
	TestCase_assert(Preferences_getInteger(preferences, "float") == 0, "Expected 0 but got %d", Preferences_getInteger(preferences, "float"));
	TestCase_assert(Preferences_getInteger(preferences, "boolean") == 0, "Expected 0 but got %d", Preferences_getInteger(preferences, "boolean"));
	TestCase_assert(Preferences_getInteger(preferences, "string") == 0, "Expected 0 but got %d", Preferences_getInteger(preferences, "string"));
	TestCase_assert(Preferences_getInteger(preferences, "data") == 0, "Expected 0 but got %d", Preferences_getInteger(preferences, "data"));
	
	TestCase_assert(Preferences_getFloat(preferences, "integer") == 0.0f, "Expected 0.0 but got %f", Preferences_getFloat(preferences, "integer"));
	TestCase_assert(Preferences_getFloat(preferences, "boolean") == 0.0f, "Expected 0.0 but got %f", Preferences_getFloat(preferences, "boolean"));
	TestCase_assert(Preferences_getFloat(preferences, "string") == 0.0f, "Expected 0.0 but got %f", Preferences_getFloat(preferences, "string"));
	TestCase_assert(Preferences_getFloat(preferences, "data") == 0.0f, "Expected 0.0 but got %f", Preferences_getFloat(preferences, "data"));
	
	TestCase_assert(Preferences_getBoolean(preferences, "integer") == false, "Expected false but got true");
	TestCase_assert(Preferences_getBoolean(preferences, "float") == false, "Expected false but got true");
	TestCase_assert(Preferences_getBoolean(preferences, "string") == false, "Expected false but got true");
	TestCase_assert(Preferences_getBoolean(preferences, "data") == false, "Expected false but got true");
	
	TestCase_assert(Preferences_getString(preferences, "integer") == NULL, "Expected NULL but got %p", Preferences_getString(preferences, "integer"));
	TestCase_assert(Preferences_getString(preferences, "float") == NULL, "Expected NULL but got %p", Preferences_getString(preferences, "float"));
	TestCase_assert(Preferences_getString(preferences, "boolean") == NULL, "Expected NULL but got %p", Preferences_getString(preferences, "boolean"));
	TestCase_assert(Preferences_getString(preferences, "data") == NULL, "Expected NULL but got %p", Preferences_getString(preferences, "data"));
	
	TestCase_assert(Preferences_getData(preferences, "integer", NULL) == NULL, "Expected NULL but got %p", Preferences_getData(preferences, "integer", NULL));
	TestCase_assert(Preferences_getData(preferences, "float", NULL) == NULL, "Expected NULL but got %p", Preferences_getData(preferences, "float", NULL));
	TestCase_assert(Preferences_getData(preferences, "boolean", NULL) == NULL, "Expected NULL but got %p", Preferences_getData(preferences, "boolean", NULL));
	TestCase_assert(Preferences_getData(preferences, "string", NULL) == NULL, "Expected NULL but got %p", Preferences_getData(preferences, "string", NULL));
	
	Preferences_setFloat(preferences, "integer", 0.0f);
	TestCase_assert(Preferences_getInteger(preferences, "integer") == 1, "Expected 1 but got %d", Preferences_getInteger(preferences, "integer"));
	Preferences_setBoolean(preferences, "integer", false);
	TestCase_assert(Preferences_getInteger(preferences, "integer") == 1, "Expected 1 but got %d", Preferences_getInteger(preferences, "integer"));
	Preferences_setString(preferences, "integer", "");
	TestCase_assert(Preferences_getInteger(preferences, "integer") == 1, "Expected 1 but got %d", Preferences_getInteger(preferences, "integer"));
	Preferences_setData(preferences, "integer", "", 1);
	TestCase_assert(Preferences_getInteger(preferences, "integer") == 1, "Expected 1 but got %d", Preferences_getInteger(preferences, "integer"));
	
	Preferences_setInteger(preferences, "float", 0);
	TestCase_assert(Preferences_getFloat(preferences, "float") == 1.0f, "Expected 1.0 but got %f", Preferences_getFloat(preferences, "float"));
	Preferences_setBoolean(preferences, "float", false);
	TestCase_assert(Preferences_getFloat(preferences, "float") == 1.0f, "Expected 1.0 but got %f", Preferences_getFloat(preferences, "float"));
	Preferences_setString(preferences, "float", "");
	TestCase_assert(Preferences_getFloat(preferences, "float") == 1.0f, "Expected 1.0 but got %f", Preferences_getFloat(preferences, "float"));
	Preferences_setData(preferences, "float", "", 1);
	TestCase_assert(Preferences_getFloat(preferences, "float") == 1.0f, "Expected 1.0 but got %f", Preferences_getFloat(preferences, "float"));
	
	Preferences_setInteger(preferences, "boolean", 0);
	TestCase_assert(Preferences_getBoolean(preferences, "boolean") == true, "Expected true but got false");
	Preferences_setFloat(preferences, "boolean", 0.0f);
	TestCase_assert(Preferences_getBoolean(preferences, "boolean") == true, "Expected true but got false");
	Preferences_setString(preferences, "boolean", "");
	TestCase_assert(Preferences_getBoolean(preferences, "boolean") == true, "Expected true but got false");
	Preferences_setData(preferences, "boolean", "", 1);
	TestCase_assert(Preferences_getBoolean(preferences, "boolean") == true, "Expected true but got false");
	
	Preferences_setInteger(preferences, "string", 0);
	TestCase_assert(Preferences_getString(preferences, "string") == stringPtr, "Expected %p but got %p", stringPtr, Preferences_getString(preferences, "string"));
	Preferences_setFloat(preferences, "string", 0.0f);
	TestCase_assert(Preferences_getString(preferences, "string") == stringPtr, "Expected %p but got %p", stringPtr, Preferences_getString(preferences, "string"));
	Preferences_setBoolean(preferences, "string", false);
	TestCase_assert(Preferences_getString(preferences, "string") == stringPtr, "Expected %p but got %p", stringPtr, Preferences_getString(preferences, "string"));
	Preferences_setData(preferences, "string", "", 1);
	TestCase_assert(Preferences_getString(preferences, "string") == stringPtr, "Expected %p but got %p", stringPtr, Preferences_getString(preferences, "string"));
	
	Preferences_setInteger(preferences, "data", 0);
	TestCase_assert(Preferences_getData(preferences, "data", NULL) == dataPtr, "Expected %p but got %p", dataPtr, Preferences_getData(preferences, "data", NULL));
	Preferences_setFloat(preferences, "data", 0);
	TestCase_assert(Preferences_getData(preferences, "data", NULL) == dataPtr, "Expected %p but got %p", dataPtr, Preferences_getData(preferences, "data", NULL));
	Preferences_setBoolean(preferences, "data", false);
	TestCase_assert(Preferences_getData(preferences, "data", NULL) == dataPtr, "Expected %p but got %p", dataPtr, Preferences_getData(preferences, "data", NULL));
	Preferences_setString(preferences, "data", "");
	TestCase_assert(Preferences_getData(preferences, "data", NULL) == dataPtr, "Expected %p but got %p", dataPtr, Preferences_getData(preferences, "data", NULL));
	
	Preferences_dispose(preferences);
}

TEST_SUITE(PreferencesTest,
           testInit,
           testLoad,
           testLoadDefaults,
           testSet,
           testSave,
           testEvents,
           testDuplicateNamesChecked,
           testValueTypesChecked)
