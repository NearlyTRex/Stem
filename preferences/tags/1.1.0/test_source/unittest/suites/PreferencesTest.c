#include "unittest/framework/TestSuite.h"
#include "preferences/Preferences.h"
#include "preferences/Preferences_private.h"

static unsigned int loadCalls;
static unsigned int saveCalls;

void Preferences_loadPrivate(compat_type(Preferences *) selfPtr) {
	char data1[] = {0xFF, 0x00, 0x7F};
	char data2[] = {0xAA};
	
	loadCalls++;
	Preferences_setIntegerPrivate(selfPtr, "integer1", -3);
	Preferences_setIntegerPrivate(selfPtr, "integer2", 0);
	Preferences_setFloatPrivate(selfPtr, "float1", -4.5f);
	Preferences_setFloatPrivate(selfPtr, "float2", 1.0f);
	Preferences_setBooleanPrivate(selfPtr, "boolean1", false);
	Preferences_setBooleanPrivate(selfPtr, "boolean2", true);
	Preferences_setStringPrivate(selfPtr, "string1", "bar");
	Preferences_setStringPrivate(selfPtr, "string2", "baz");
	Preferences_setDataPrivate(selfPtr, "data1", data1, sizeof(data1));
	Preferences_setDataPrivate(selfPtr, "data2", data2, sizeof(data2));
}

void Preferences_savePrivate(compat_type(Preferences *) selfPtr) {
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
	TestCase_assert(preferences.addInteger == Preferences_addInteger, "Expected %p but got %p", Preferences_addInteger, preferences.addInteger);
	TestCase_assert(preferences.addFloat == Preferences_addFloat, "Expected %p but got %p", Preferences_addFloat, preferences.addFloat);
	TestCase_assert(preferences.addBoolean == Preferences_addBoolean, "Expected %p but got %p", Preferences_addBoolean, preferences.addBoolean);
	TestCase_assert(preferences.addString == Preferences_addString, "Expected %p but got %p", Preferences_addString, preferences.addString);
	TestCase_assert(preferences.addData == Preferences_addData, "Expected %p but got %p", Preferences_addData, preferences.addData);
	TestCase_assert(preferences.getInteger == Preferences_getInteger, "Expected %p but got %p", Preferences_getInteger, preferences.getInteger);
	TestCase_assert(preferences.getFloat == Preferences_getFloat, "Expected %p but got %p", Preferences_getFloat, preferences.getFloat);
	TestCase_assert(preferences.getBoolean == Preferences_getBoolean, "Expected %p but got %p", Preferences_getBoolean, preferences.getBoolean);
	TestCase_assert(preferences.getString == Preferences_getString, "Expected %p but got %p", Preferences_getString, preferences.getString);
	TestCase_assert(preferences.getData == Preferences_getData, "Expected %p but got %p", Preferences_getData, preferences.getData);
	TestCase_assert(preferences.setInteger == Preferences_setInteger, "Expected %p but got %p", Preferences_setInteger, preferences.setInteger);
	TestCase_assert(preferences.setFloat == Preferences_setFloat, "Expected %p but got %p", Preferences_setFloat, preferences.setFloat);
	TestCase_assert(preferences.setBoolean == Preferences_setBoolean, "Expected %p but got %p", Preferences_setBoolean, preferences.setBoolean);
	TestCase_assert(preferences.setString == Preferences_setString, "Expected %p but got %p", Preferences_setString, preferences.setString);
	TestCase_assert(preferences.setData == Preferences_setData, "Expected %p but got %p", Preferences_setData, preferences.setData);
	TestCase_assert(preferences.load == Preferences_load, "Expected %p but got %p", Preferences_load, preferences.load);
	TestCase_assert(preferences.save == Preferences_save, "Expected %p but got %p", Preferences_save, preferences.save);
	TestCase_assert(preferences.loadDefaultValues == Preferences_loadDefaultValues, "Expected %p but got %p", Preferences_loadDefaultValues, preferences.loadDefaultValues);
	TestCase_assert(preferences.loadDefaultValue == Preferences_loadDefaultValue, "Expected %p but got %p", Preferences_loadDefaultValue, preferences.loadDefaultValue);
	preferences.dispose(&preferences);
	
	preferencesPtr = Preferences_create(identifier2);
	TestCase_assert(preferencesPtr != NULL, "Expected non-NULL but got NULL");
	if (preferencesPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(preferencesPtr->identifier != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(preferencesPtr->identifier != identifier2, "Pointers expected to differ but didn't");
	TestCase_assert(!strcmp(preferencesPtr->identifier, identifier2), "Expected \"identifier2\" but got \"%s\"", preferencesPtr->identifier);
	TestCase_assert(preferencesPtr->eventDispatcher != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(preferencesPtr->dispose == Preferences_dispose, "Expected %p but got %p", Preferences_dispose, preferencesPtr->dispose);
	TestCase_assert(preferencesPtr->addInteger == Preferences_addInteger, "Expected %p but got %p", Preferences_addInteger, preferencesPtr->addInteger);
	TestCase_assert(preferencesPtr->addFloat == Preferences_addFloat, "Expected %p but got %p", Preferences_addFloat, preferencesPtr->addFloat);
	TestCase_assert(preferencesPtr->addBoolean == Preferences_addBoolean, "Expected %p but got %p", Preferences_addBoolean, preferencesPtr->addBoolean);
	TestCase_assert(preferencesPtr->addString == Preferences_addString, "Expected %p but got %p", Preferences_addString, preferencesPtr->addString);
	TestCase_assert(preferencesPtr->addData == Preferences_addData, "Expected %p but got %p", Preferences_addData, preferencesPtr->addData);
	TestCase_assert(preferencesPtr->getInteger == Preferences_getInteger, "Expected %p but got %p", Preferences_getInteger, preferencesPtr->getInteger);
	TestCase_assert(preferencesPtr->getFloat == Preferences_getFloat, "Expected %p but got %p", Preferences_getFloat, preferencesPtr->getFloat);
	TestCase_assert(preferencesPtr->getBoolean == Preferences_getBoolean, "Expected %p but got %p", Preferences_getBoolean, preferencesPtr->getBoolean);
	TestCase_assert(preferencesPtr->getString == Preferences_getString, "Expected %p but got %p", Preferences_getString, preferencesPtr->getString);
	TestCase_assert(preferencesPtr->getData == Preferences_getData, "Expected %p but got %p", Preferences_getData, preferencesPtr->getData);
	TestCase_assert(preferencesPtr->setInteger == Preferences_setInteger, "Expected %p but got %p", Preferences_setInteger, preferencesPtr->setInteger);
	TestCase_assert(preferencesPtr->setFloat == Preferences_setFloat, "Expected %p but got %p", Preferences_setFloat, preferencesPtr->setFloat);
	TestCase_assert(preferencesPtr->setBoolean == Preferences_setBoolean, "Expected %p but got %p", Preferences_setBoolean, preferencesPtr->setBoolean);
	TestCase_assert(preferencesPtr->setString == Preferences_setString, "Expected %p but got %p", Preferences_setString, preferencesPtr->setString);
	TestCase_assert(preferencesPtr->setData == Preferences_setData, "Expected %p but got %p", Preferences_setData, preferencesPtr->setData);
	TestCase_assert(preferencesPtr->load == Preferences_load, "Expected %p but got %p", Preferences_load, preferencesPtr->load);
	TestCase_assert(preferencesPtr->save == Preferences_save, "Expected %p but got %p", Preferences_save, preferencesPtr->save);
	TestCase_assert(preferencesPtr->loadDefaultValues == Preferences_loadDefaultValues, "Expected %p but got %p", Preferences_loadDefaultValues, preferencesPtr->loadDefaultValues);
	TestCase_assert(preferencesPtr->loadDefaultValue == Preferences_loadDefaultValue, "Expected %p but got %p", Preferences_loadDefaultValue, preferencesPtr->loadDefaultValue);
	preferencesPtr->dispose(preferencesPtr);
}

static void testLoad() {
	Preferences * preferences;
	char defaultData1[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	char defaultData2[] = {0xFE, 0xED, 0xFA, 0xCE};
	void * data;
	size_t dataSize;
	
	preferences = Preferences_create("preferences_unittest");
	
	TestCase_assert(preferences->getInteger(preferences, "integer1") == 0, "Expected 0 but got %d", preferences->getInteger(preferences, "integer1"));
	TestCase_assert(preferences->getInteger(preferences, "integer2") == 0, "Expected 0 but got %d", preferences->getInteger(preferences, "integer2"));
	TestCase_assert(preferences->getFloat(preferences, "float1") == 0.0f, "Expected 0.0 but got %f", preferences->getFloat(preferences, "float1"));
	TestCase_assert(preferences->getFloat(preferences, "float2") == 0.0f, "Expected 0.0 but got %f", preferences->getFloat(preferences, "float2"));
	TestCase_assert(preferences->getBoolean(preferences, "boolean1") == false, "Expected false but got true");
	TestCase_assert(preferences->getBoolean(preferences, "boolean2") == false, "Expected false but got true");
	TestCase_assert(preferences->getString(preferences, "string1") == NULL, "Expected NULL but got %s", preferences->getString(preferences, "string1"));
	TestCase_assert(preferences->getString(preferences, "string2") == NULL, "Expected NULL but got %s", preferences->getString(preferences, "string2"));
	TestCase_assert(preferences->getData(preferences, "data1", NULL) == NULL, "Expected NULL but got %p", preferences->getData(preferences, "data1", NULL));
	TestCase_assert(preferences->getData(preferences, "data2", NULL) == NULL, "Expected NULL but got %p", preferences->getData(preferences, "data2", NULL));
	
	preferences->addInteger(preferences, "integer1", 1);
	preferences->addInteger(preferences, "integer2", -2);
	preferences->addFloat(preferences, "float1", 4.0f);
	preferences->addFloat(preferences, "float2", -0.125f);
	preferences->addBoolean(preferences, "boolean1", true);
	preferences->addBoolean(preferences, "boolean2", false);
	preferences->addString(preferences, "string1", "Hello, world!");
	preferences->addString(preferences, "string2", "foo");
	preferences->addData(preferences, "data1", defaultData1, sizeof(defaultData1));
	preferences->addData(preferences, "data2", defaultData2, sizeof(defaultData2));
	
	TestCase_assert(preferences->getInteger(preferences, "integer1") == 1, "Expected 1 but got %d", preferences->getInteger(preferences, "integer1"));
	TestCase_assert(preferences->getInteger(preferences, "integer2") == -2, "Expected -2 but got %d", preferences->getInteger(preferences, "integer2"));
	TestCase_assert(preferences->getFloat(preferences, "float1") == 4.0f, "Expected 4.0 but got %f", preferences->getFloat(preferences, "float1"));
	TestCase_assert(preferences->getFloat(preferences, "float2") == -0.125f, "Expected -0.125 but got %f", preferences->getFloat(preferences, "float2"));
	TestCase_assert(preferences->getBoolean(preferences, "boolean1") == true, "Expected true but got false");
	TestCase_assert(preferences->getBoolean(preferences, "boolean2") == false, "Expected false but got true");
	TestCase_assert(preferences->getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string1"), "Hello, world!"), "Expected \"Hello, world!\" but got \"%s\"", preferences->getString(preferences, "string1"));
	TestCase_assert(preferences->getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string2"), "foo"), "Expected \"foo\" but got \"%s\"", preferences->getString(preferences, "string2"));
	data = preferences->getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 5, "Expected 5 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\x00\x01\x02\x03\x04", 5), "Data didn't match expected value");
	data = preferences->getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 4, "Expected 4 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\xFE\xED\xFA\xCE", 4), "Data didn't match expected value");
	
	loadCalls = 0;
	preferences->load(preferences);
	TestCase_assert(loadCalls == 1, "Expected 1 but got %u", loadCalls);
	
	TestCase_assert(preferences->getInteger(preferences, "integer1") == -3, "Expected -3 but got %d", preferences->getInteger(preferences, "integer1"));
	TestCase_assert(preferences->getInteger(preferences, "integer2") == 0, "Expected 0 but got %d", preferences->getInteger(preferences, "integer2"));
	TestCase_assert(preferences->getFloat(preferences, "float1") == -4.5f, "Expected -4.5 but got %f", preferences->getFloat(preferences, "float1"));
	TestCase_assert(preferences->getFloat(preferences, "float2") == 1.0f, "Expected 1.0 but got %f", preferences->getFloat(preferences, "float2"));
	TestCase_assert(preferences->getBoolean(preferences, "boolean1") == false, "Expected false but got true");
	TestCase_assert(preferences->getBoolean(preferences, "boolean2") == true, "Expected true but got false");
	TestCase_assert(preferences->getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string1"), "bar"), "Expected \"bar\" but got \"%s\"", preferences->getString(preferences, "string1"));
	TestCase_assert(preferences->getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string2"), "baz"), "Expected \"baz\" but got \"%s\"", preferences->getString(preferences, "string2"));
	data = preferences->getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 3, "Expected 3 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\xFF\x00\x7F", 3), "Data didn't match expected value");
	data = preferences->getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 1, "Expected 1 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\xAA", 1), "Data didn't match expected value");
	
	preferences->dispose(preferences);
}

static void testLoadDefaults() {
	Preferences * preferences;
	char defaultData1[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	char defaultData2[] = {0xFE, 0xED, 0xFA, 0xCE};
	void * data;
	size_t dataSize;
	
	preferences = Preferences_create("preferences_unittest");
	preferences->addInteger(preferences, "integer1", 1);
	preferences->addInteger(preferences, "integer2", -2);
	preferences->addFloat(preferences, "float1", 4.0f);
	preferences->addFloat(preferences, "float2", -0.125f);
	preferences->addBoolean(preferences, "boolean1", true);
	preferences->addBoolean(preferences, "boolean2", false);
	preferences->addString(preferences, "string1", "Hello, world!");
	preferences->addString(preferences, "string2", "foo");
	preferences->addData(preferences, "data1", defaultData1, sizeof(defaultData1));
	preferences->addData(preferences, "data2", defaultData2, sizeof(defaultData2));
	preferences->load(preferences);
	
	preferences->loadDefaultValue(preferences, "integer1");
	preferences->loadDefaultValue(preferences, "float1");
	preferences->loadDefaultValue(preferences, "boolean1");
	preferences->loadDefaultValue(preferences, "string1");
	preferences->loadDefaultValue(preferences, "data1");
	TestCase_assert(preferences->getInteger(preferences, "integer1") == 1, "Expected 1 but got %d", preferences->getInteger(preferences, "integer1"));
	TestCase_assert(preferences->getInteger(preferences, "integer2") == 0, "Expected 0 but got %d", preferences->getInteger(preferences, "integer2"));
	TestCase_assert(preferences->getFloat(preferences, "float1") == 4.0f, "Expected 4.0 but got %f", preferences->getFloat(preferences, "float1"));
	TestCase_assert(preferences->getFloat(preferences, "float2") == 1.0f, "Expected 1.0 but got %f", preferences->getFloat(preferences, "float2"));
	TestCase_assert(preferences->getBoolean(preferences, "boolean1") == true, "Expected true but got false");
	TestCase_assert(preferences->getBoolean(preferences, "boolean2") == true, "Expected true but got false");
	TestCase_assert(preferences->getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string1"), "Hello, world!"), "Expected \"Hello, world!\" but got \"%s\"", preferences->getString(preferences, "string1"));
	TestCase_assert(preferences->getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string2"), "baz"), "Expected \"baz\" but got \"%s\"", preferences->getString(preferences, "string2"));
	data = preferences->getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 5, "Expected 5 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\x00\x01\x02\x03\x04", 5), "Data didn't match expected value");
	data = preferences->getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 1, "Expected 1 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\xAA", 1), "Data didn't match expected value");
	
	preferences->loadDefaultValues(preferences);
	TestCase_assert(preferences->getInteger(preferences, "integer1") == 1, "Expected 1 but got %d", preferences->getInteger(preferences, "integer1"));
	TestCase_assert(preferences->getInteger(preferences, "integer2") == -2, "Expected -2 but got %d", preferences->getInteger(preferences, "integer2"));
	TestCase_assert(preferences->getFloat(preferences, "float1") == 4.0f, "Expected 4.0 but got %f", preferences->getFloat(preferences, "float1"));
	TestCase_assert(preferences->getFloat(preferences, "float2") == -0.125f, "Expected -0.125 but got %f", preferences->getFloat(preferences, "float2"));
	TestCase_assert(preferences->getBoolean(preferences, "boolean1") == true, "Expected true but got false");
	TestCase_assert(preferences->getBoolean(preferences, "boolean2") == false, "Expected false but got true");
	TestCase_assert(preferences->getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string1"), "Hello, world!"), "Expected \"Hello, world!\" but got \"%s\"", preferences->getString(preferences, "string1"));
	TestCase_assert(preferences->getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string2"), "foo"), "Expected \"foo\" but got \"%s\"", preferences->getString(preferences, "string2"));
	data = preferences->getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 5, "Expected 5 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\x00\x01\x02\x03\x04", 5), "Data didn't match expected value");
	data = preferences->getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 4, "Expected 4 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\xFE\xED\xFA\xCE", 4), "Data didn't match expected value");
	
	preferences->dispose(preferences);
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
	preferences->addInteger(preferences, "integer1", 1);
	preferences->addInteger(preferences, "integer2", -2);
	preferences->addFloat(preferences, "float1", 4.0f);
	preferences->addFloat(preferences, "float2", -0.125f);
	preferences->addBoolean(preferences, "boolean1", true);
	preferences->addBoolean(preferences, "boolean2", false);
	preferences->addString(preferences, "string1", "Hello, world!");
	preferences->addString(preferences, "string2", "foo");
	preferences->addData(preferences, "data1", defaultData1, sizeof(defaultData1));
	preferences->addData(preferences, "data2", defaultData2, sizeof(defaultData2));
	
	preferences->setInteger(preferences, "integer1", 10);
	preferences->setInteger(preferences, "integer2", 3);
	preferences->setFloat(preferences, "float1", 1.25f);
	preferences->setFloat(preferences, "float2", 8.5f);
	preferences->setBoolean(preferences, "boolean1", false);
	preferences->setBoolean(preferences, "boolean2", true);
	preferences->setString(preferences, "string1", "bar");
	preferences->setString(preferences, "string2", "baz");
	preferences->setData(preferences, "data1", data1, sizeof(data1));
	preferences->setData(preferences, "data2", data2, sizeof(data2));
	
	TestCase_assert(preferences->getInteger(preferences, "integer1") == 10, "Expected 10 but got %d", preferences->getInteger(preferences, "integer1"));
	TestCase_assert(preferences->getInteger(preferences, "integer2") == 3, "Expected 3 but got %d", preferences->getInteger(preferences, "integer2"));
	TestCase_assert(preferences->getFloat(preferences, "float1") == 1.25f, "Expected 1.25 but got %f", preferences->getFloat(preferences, "float1"));
	TestCase_assert(preferences->getFloat(preferences, "float2") == 8.5f, "Expected 8.5 but got %f", preferences->getFloat(preferences, "float2"));
	TestCase_assert(preferences->getBoolean(preferences, "boolean1") == false, "Expected false but got false");
	TestCase_assert(preferences->getBoolean(preferences, "boolean2") == true, "Expected true but got true");
	TestCase_assert(preferences->getString(preferences, "string1") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string1"), "bar"), "Expected \"bar\" but got \"%s\"", preferences->getString(preferences, "string1"));
	TestCase_assert(preferences->getString(preferences, "string2") != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(preferences->getString(preferences, "string2"), "baz"), "Expected \"baz\" but got \"%s\"", preferences->getString(preferences, "string2"));
	data = preferences->getData(preferences, "data1", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 3, "Expected 3 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\xFF\x7F\x00", 3), "Data didn't match expected value");
	data = preferences->getData(preferences, "data2", &dataSize);
	TestCase_assert(data != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(dataSize == 2, "Expected 2 but got %zu", dataSize);
	TestCase_assert(!memcmp(data, "\xCC\xB7", 2), "Data didn't match expected value");
	
	preferences->dispose(preferences);
}

static void testSave() {
	Preferences * preferences;
	char defaultData1[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	char defaultData2[] = {0xFE, 0xED, 0xFA, 0xCE};
	char data1[] = {0xFF, 0x7F, 0x00};
	char data2[] = {0xCC, 0xB7};
	
	preferences = Preferences_create("preferences_unittest");
	preferences->addInteger(preferences, "integer1", 1);
	preferences->addInteger(preferences, "integer2", -2);
	preferences->addFloat(preferences, "float1", 4.0f);
	preferences->addFloat(preferences, "float2", -0.125f);
	preferences->addBoolean(preferences, "boolean1", true);
	preferences->addBoolean(preferences, "boolean2", false);
	preferences->addString(preferences, "string1", "Hello, world!");
	preferences->addString(preferences, "string2", "foo");
	preferences->addData(preferences, "data1", defaultData1, sizeof(defaultData1));
	preferences->addData(preferences, "data2", defaultData2, sizeof(defaultData2));
	
	preferences->setInteger(preferences, "integer1", 10);
	preferences->setInteger(preferences, "integer2", 3);
	preferences->setFloat(preferences, "float1", 1.25f);
	preferences->setFloat(preferences, "float2", 8.5f);
	preferences->setBoolean(preferences, "boolean1", false);
	preferences->setBoolean(preferences, "boolean2", true);
	preferences->setString(preferences, "string1", "bar");
	preferences->setString(preferences, "string2", "baz");
	preferences->setData(preferences, "data1", data1, sizeof(data1));
	preferences->setData(preferences, "data2", data2, sizeof(data2));
	
	saveCalls = 0;
	preferences->save(preferences);
	TestCase_assert(saveCalls == 1, "Expected 1 but got %u", saveCalls);
	
	preferences->dispose(preferences);
}

static unsigned int valueChangedCalls;
static void * expectedSender;
static const char * expectedName;
static enum PreferencesType expectedType;
static union PreferencesValue expectedValue;
static union PreferencesValue expectedPreviousValue;
static union PreferencesValue newValue;

static bool valueChanged(void * sender, const char * eventID, void * eventData, void * context) {
	struct PreferencesEvent * event = eventData;
	
	valueChangedCalls++;
	TestCase_assert(!strcmp(eventID, PREFERENCES_EVENT_VALUE_CHANGED), "Expected " PREFERENCES_EVENT_VALUE_CHANGED " but got %s", eventID);
	TestCase_assert(sender == expectedSender, "Expected %p but got %p", expectedSender, sender);
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
			TestCase_assert(event->value.data.length == expectedValue.data.length, "Expected %zu but got %zu", expectedValue.data.length, event->value.data.length);
			TestCase_assert(!memcmp(event->value.data.bytes, expectedValue.data.bytes, expectedValue.data.length), "Expected data didn't match actual");
			TestCase_assert(event->previousValue.data.length == expectedPreviousValue.data.length, "Expected %zu but got %zu", expectedPreviousValue.data.length, event->previousValue.data.length);
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
	preferences->eventDispatcher->registerForEvent(preferences->eventDispatcher, PREFERENCES_EVENT_VALUE_CHANGED, valueChanged, NULL);
	preferences->addInteger(preferences, "integer1", 1);
	preferences->addFloat(preferences, "float1", 1.0f);
	preferences->addBoolean(preferences, "boolean1", true);
	preferences->addString(preferences, "string1", "");
	preferences->addData(preferences, "data1", defaultData, sizeof(defaultData));
	
	expectedSender = preferences;
	TestCase_assert(valueChangedCalls == 0, "Expected 0 but got %u", valueChangedCalls);
	
	expectedName = "integer1";
	expectedType = PREFERENCES_TYPE_INTEGER;
	expectedValue.integer = 2;
	expectedPreviousValue.integer = 1;
	newValue.integer = 3;
	preferences->setInteger(preferences, "integer1", 2);
	TestCase_assert(valueChangedCalls == 1, "Expected 1 but got %u", valueChangedCalls);
	TestCase_assert(preferences->getInteger(preferences, "integer1") == 3, "Expected 3 but got %d", preferences->getInteger(preferences, "integer1"));
	
	expectedName = "float1";
	expectedType = PREFERENCES_TYPE_FLOAT;
	expectedValue.number = 2.0f;
	expectedPreviousValue.number = 1.0f;
	newValue.number = 3.0f;
	preferences->setFloat(preferences, "float1", 2.0f);
	TestCase_assert(valueChangedCalls == 2, "Expected 2 but got %u", valueChangedCalls);
	TestCase_assert(preferences->getFloat(preferences, "float1") == 3.0f, "Expected 3.0f but got %f", preferences->getFloat(preferences, "float1"));
	
	expectedName = "boolean1";
	expectedType = PREFERENCES_TYPE_BOOLEAN;
	expectedValue.boolean = true;
	expectedPreviousValue.boolean = true;
	newValue.boolean = false;
	preferences->setBoolean(preferences, "boolean1", true);
	TestCase_assert(valueChangedCalls == 3, "Expected 3 but got %u", valueChangedCalls);
	TestCase_assert(preferences->getBoolean(preferences, "boolean1") == false, "Expected false but got true");
	
	expectedName = "string1";
	expectedType = PREFERENCES_TYPE_STRING;
	expectedValue.string = "foo";
	expectedPreviousValue.string = "";
	newValue.string = "bar";
	preferences->setString(preferences, "string1", "foo");
	TestCase_assert(valueChangedCalls == 4, "Expected 4 but got %u", valueChangedCalls);
	TestCase_assert(!strcmp(preferences->getString(preferences, "string1"), "bar"), "Expected \"bar\" but got \"%s\"", preferences->getString(preferences, "string1"));
	
	expectedName = "data1";
	expectedType = PREFERENCES_TYPE_DATA;
	expectedValue.data.length = 2;
	expectedValue.data.bytes = data1;
	expectedPreviousValue.data.length = 1;
	expectedPreviousValue.data.bytes = defaultData;
	newValue.data.length = 3;
	newValue.data.bytes = data2;
	preferences->setData(preferences, "data1", data1, 2);
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	data = preferences->getData(preferences, "data1", &dataLength);
	TestCase_assert(dataLength == 3, "Expected 3 but got %zu", dataLength);
	TestCase_assert(!memcmp(data, data2, 3), "Expected data didn't match actual");
	
	preferences->load(preferences);
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	
	preferences->loadDefaultValues(preferences);
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	
	preferences->loadDefaultValue(preferences, "integer1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	preferences->loadDefaultValue(preferences, "float1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	preferences->loadDefaultValue(preferences, "boolean1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	preferences->loadDefaultValue(preferences, "string1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	preferences->loadDefaultValue(preferences, "data1");
	TestCase_assert(valueChangedCalls == 5, "Expected 5 but got %u", valueChangedCalls);
	
	preferences->dispose(preferences);
}

static void testDuplicateNamesChecked() {
	Preferences * preferences;
	
	preferences = Preferences_create("preferences_unittest");
	
	TestCase_assert(preferences->valueCount == 0, "Expected 0 but got %zu", preferences->valueCount);
	preferences->addInteger(preferences, "integer", 0);
	TestCase_assert(preferences->valueCount == 1, "Expected 1 but got %zu", preferences->valueCount);
	preferences->addFloat(preferences, "float", 0.0f);
	TestCase_assert(preferences->valueCount == 2, "Expected 2 but got %zu", preferences->valueCount);
	preferences->addBoolean(preferences, "boolean", false);
	TestCase_assert(preferences->valueCount == 3, "Expected 3 but got %zu", preferences->valueCount);
	preferences->addString(preferences, "string", "");
	TestCase_assert(preferences->valueCount == 4, "Expected 4 but got %zu", preferences->valueCount);
	preferences->addData(preferences, "data", "", 1);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got %zu", preferences->valueCount);
	
	preferences->addInteger(preferences, "integer", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got %zu", preferences->valueCount);
	preferences->addInteger(preferences, "float", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got %zu", preferences->valueCount);
	preferences->addInteger(preferences, "boolean", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got %zu", preferences->valueCount);
	preferences->addInteger(preferences, "string", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got %zu", preferences->valueCount);
	preferences->addInteger(preferences, "data", 0);
	TestCase_assert(preferences->valueCount == 5, "Expected 5 but got %zu", preferences->valueCount);
	preferences->addInteger(preferences, "integer2", 0);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got %zu", preferences->valueCount);
	
	preferences->addFloat(preferences, "integer", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got %zu", preferences->valueCount);
	preferences->addFloat(preferences, "float", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got %zu", preferences->valueCount);
	preferences->addFloat(preferences, "boolean", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got %zu", preferences->valueCount);
	preferences->addFloat(preferences, "string", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got %zu", preferences->valueCount);
	preferences->addFloat(preferences, "data", 0.0f);
	TestCase_assert(preferences->valueCount == 6, "Expected 6 but got %zu", preferences->valueCount);
	preferences->addFloat(preferences, "float2", 0.0f);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got %zu", preferences->valueCount);
	
	preferences->addBoolean(preferences, "integer", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got %zu", preferences->valueCount);
	preferences->addBoolean(preferences, "float", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got %zu", preferences->valueCount);
	preferences->addBoolean(preferences, "boolean", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got %zu", preferences->valueCount);
	preferences->addBoolean(preferences, "string", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got %zu", preferences->valueCount);
	preferences->addBoolean(preferences, "data", false);
	TestCase_assert(preferences->valueCount == 7, "Expected 7 but got %zu", preferences->valueCount);
	preferences->addBoolean(preferences, "boolean2", false);
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got %zu", preferences->valueCount);
	
	preferences->addString(preferences, "integer", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got %zu", preferences->valueCount);
	preferences->addString(preferences, "float", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got %zu", preferences->valueCount);
	preferences->addString(preferences, "boolean", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got %zu", preferences->valueCount);
	preferences->addString(preferences, "string", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got %zu", preferences->valueCount);
	preferences->addString(preferences, "data", "");
	TestCase_assert(preferences->valueCount == 8, "Expected 8 but got %zu", preferences->valueCount);
	preferences->addString(preferences, "string2", "");
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got %zu", preferences->valueCount);
	
	preferences->addData(preferences, "integer", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got %zu", preferences->valueCount);
	preferences->addData(preferences, "float", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got %zu", preferences->valueCount);
	preferences->addData(preferences, "boolean", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got %zu", preferences->valueCount);
	preferences->addData(preferences, "string", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got %zu", preferences->valueCount);
	preferences->addData(preferences, "data", "", 1);
	TestCase_assert(preferences->valueCount == 9, "Expected 9 but got %zu", preferences->valueCount);
	preferences->addData(preferences, "data2", "", 1);
	TestCase_assert(preferences->valueCount == 10, "Expected 10 but got %zu", preferences->valueCount);
	
	preferences->dispose(preferences);
}

static void testValueTypesChecked() {
	Preferences * preferences;
	char defaultData[] = {0x01};
	const char * stringPtr;
	const void * dataPtr;
	
	preferences = Preferences_create("preferences_unittest");
	preferences->addInteger(preferences, "integer", 1);
	preferences->addFloat(preferences, "float", 1.0f);
	preferences->addBoolean(preferences, "boolean", true);
	preferences->addString(preferences, "string", "\x01");
	preferences->addData(preferences, "data", defaultData, sizeof(defaultData));
	
	stringPtr = preferences->getString(preferences, "string");
	dataPtr = preferences->getData(preferences, "data", NULL);
	
	TestCase_assert(preferences->getInteger(preferences, "float") == 0, "Expected 0 but got %d", preferences->getInteger(preferences, "float"));
	TestCase_assert(preferences->getInteger(preferences, "boolean") == 0, "Expected 0 but got %d", preferences->getInteger(preferences, "boolean"));
	TestCase_assert(preferences->getInteger(preferences, "string") == 0, "Expected 0 but got %d", preferences->getInteger(preferences, "string"));
	TestCase_assert(preferences->getInteger(preferences, "data") == 0, "Expected 0 but got %d", preferences->getInteger(preferences, "data"));
	
	TestCase_assert(preferences->getFloat(preferences, "integer") == 0.0f, "Expected 0.0 but got %f", preferences->getFloat(preferences, "integer"));
	TestCase_assert(preferences->getFloat(preferences, "boolean") == 0.0f, "Expected 0.0 but got %f", preferences->getFloat(preferences, "boolean"));
	TestCase_assert(preferences->getFloat(preferences, "string") == 0.0f, "Expected 0.0 but got %f", preferences->getFloat(preferences, "string"));
	TestCase_assert(preferences->getFloat(preferences, "data") == 0.0f, "Expected 0.0 but got %f", preferences->getFloat(preferences, "data"));
	
	TestCase_assert(preferences->getBoolean(preferences, "integer") == false, "Expected false but got true");
	TestCase_assert(preferences->getBoolean(preferences, "float") == false, "Expected false but got true");
	TestCase_assert(preferences->getBoolean(preferences, "string") == false, "Expected false but got true");
	TestCase_assert(preferences->getBoolean(preferences, "data") == false, "Expected false but got true");
	
	TestCase_assert(preferences->getString(preferences, "integer") == NULL, "Expected NULL but got %p", preferences->getString(preferences, "integer"));
	TestCase_assert(preferences->getString(preferences, "float") == NULL, "Expected NULL but got %p", preferences->getString(preferences, "float"));
	TestCase_assert(preferences->getString(preferences, "boolean") == NULL, "Expected NULL but got %p", preferences->getString(preferences, "boolean"));
	TestCase_assert(preferences->getString(preferences, "data") == NULL, "Expected NULL but got %p", preferences->getString(preferences, "data"));
	
	TestCase_assert(preferences->getData(preferences, "integer", NULL) == NULL, "Expected NULL but got %p", preferences->getData(preferences, "integer", NULL));
	TestCase_assert(preferences->getData(preferences, "float", NULL) == NULL, "Expected NULL but got %p", preferences->getData(preferences, "float", NULL));
	TestCase_assert(preferences->getData(preferences, "boolean", NULL) == NULL, "Expected NULL but got %p", preferences->getData(preferences, "boolean", NULL));
	TestCase_assert(preferences->getData(preferences, "string", NULL) == NULL, "Expected NULL but got %p", preferences->getData(preferences, "string", NULL));
	
	preferences->setFloat(preferences, "integer", 0.0f);
	TestCase_assert(preferences->getInteger(preferences, "integer") == 1, "Expected 1 but got %d", preferences->getInteger(preferences, "integer"));
	preferences->setBoolean(preferences, "integer", false);
	TestCase_assert(preferences->getInteger(preferences, "integer") == 1, "Expected 1 but got %d", preferences->getInteger(preferences, "integer"));
	preferences->setString(preferences, "integer", "");
	TestCase_assert(preferences->getInteger(preferences, "integer") == 1, "Expected 1 but got %d", preferences->getInteger(preferences, "integer"));
	preferences->setData(preferences, "integer", "", 1);
	TestCase_assert(preferences->getInteger(preferences, "integer") == 1, "Expected 1 but got %d", preferences->getInteger(preferences, "integer"));
	
	preferences->setInteger(preferences, "float", 0);
	TestCase_assert(preferences->getFloat(preferences, "float") == 1.0f, "Expected 1.0 but got %f", preferences->getFloat(preferences, "float"));
	preferences->setBoolean(preferences, "float", false);
	TestCase_assert(preferences->getFloat(preferences, "float") == 1.0f, "Expected 1.0 but got %f", preferences->getFloat(preferences, "float"));
	preferences->setString(preferences, "float", "");
	TestCase_assert(preferences->getFloat(preferences, "float") == 1.0f, "Expected 1.0 but got %f", preferences->getFloat(preferences, "float"));
	preferences->setData(preferences, "float", "", 1);
	TestCase_assert(preferences->getFloat(preferences, "float") == 1.0f, "Expected 1.0 but got %f", preferences->getFloat(preferences, "float"));
	
	preferences->setInteger(preferences, "boolean", 0);
	TestCase_assert(preferences->getBoolean(preferences, "boolean") == true, "Expected true but got false");
	preferences->setFloat(preferences, "boolean", 0.0f);
	TestCase_assert(preferences->getBoolean(preferences, "boolean") == true, "Expected true but got false");
	preferences->setString(preferences, "boolean", "");
	TestCase_assert(preferences->getBoolean(preferences, "boolean") == true, "Expected true but got false");
	preferences->setData(preferences, "boolean", "", 1);
	TestCase_assert(preferences->getBoolean(preferences, "boolean") == true, "Expected true but got false");
	
	preferences->setInteger(preferences, "string", 0);
	TestCase_assert(preferences->getString(preferences, "string") == stringPtr, "Expected %p but got %p", stringPtr, preferences->getString(preferences, "string"));
	preferences->setFloat(preferences, "string", 0.0f);
	TestCase_assert(preferences->getString(preferences, "string") == stringPtr, "Expected %p but got %p", stringPtr, preferences->getString(preferences, "string"));
	preferences->setBoolean(preferences, "string", false);
	TestCase_assert(preferences->getString(preferences, "string") == stringPtr, "Expected %p but got %p", stringPtr, preferences->getString(preferences, "string"));
	preferences->setData(preferences, "string", "", 1);
	TestCase_assert(preferences->getString(preferences, "string") == stringPtr, "Expected %p but got %p", stringPtr, preferences->getString(preferences, "string"));
	
	preferences->setInteger(preferences, "data", 0);
	TestCase_assert(preferences->getData(preferences, "data", NULL) == dataPtr, "Expected %p but got %p", dataPtr, preferences->getData(preferences, "data", NULL));
	preferences->setFloat(preferences, "data", 0);
	TestCase_assert(preferences->getData(preferences, "data", NULL) == dataPtr, "Expected %p but got %p", dataPtr, preferences->getData(preferences, "data", NULL));
	preferences->setBoolean(preferences, "data", false);
	TestCase_assert(preferences->getData(preferences, "data", NULL) == dataPtr, "Expected %p but got %p", dataPtr, preferences->getData(preferences, "data", NULL));
	preferences->setString(preferences, "data", "");
	TestCase_assert(preferences->getData(preferences, "data", NULL) == dataPtr, "Expected %p but got %p", dataPtr, preferences->getData(preferences, "data", NULL));
	
	preferences->dispose(preferences);
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
