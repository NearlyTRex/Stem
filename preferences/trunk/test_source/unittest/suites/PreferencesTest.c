#include "unittest/framework/TestSuite.h"
#include "preferences/Preferences.h"
#include "preferences/Preferences_private.h"

static void testInit() {
	Preferences preferences, * preferencesPtr;
	
	memset(&preferences, 0, sizeof(Preferences));
	Preferences_init(&preferences, "", PREFERENCES_DOMAIN_USER);
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
	
	preferencesPtr = Preferences_create("", PREFERENCES_DOMAIN_USER);
	TestCase_assert(preferencesPtr != NULL, "Expected non-NULL but got NULL");
	if (preferencesPtr == NULL) {return;} // Suppress clang warning
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

void Preferences_loadPrivate(compat_type(Preferences *) selfPtr) {
	char data1[] = {0xFF, 0x00, 0x7F};
	char data2[] = {0xAA};
	
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
}

static void testLoad() {
	Preferences * preferences;
	char defaultData1[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	char defaultData2[] = {0xFE, 0xED, 0xFA, 0xCE};
	void * data;
	size_t dataSize;
	
	preferences = Preferences_create("unit_test", PREFERENCES_DOMAIN_USER);
	
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
	
	preferences->load(preferences);
	
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
}

static void testSave() {
}

static void testEvents() {
}

// Don't allow duplicate value names
// Ensure value types are checked (can't get non-integer with getInteger)

TEST_SUITE(PreferencesTest, testInit, testLoad, testSet, testSave, testEvents)
