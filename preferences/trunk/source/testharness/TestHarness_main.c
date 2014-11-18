#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "preferences/Preferences.h"

static void printUsage() {
	fprintf(stderr, "Usage: preferences_testharness [-i identifier] [-a {set | defaults}]\n");
}

#define ACTION_NONE 0
#define ACTION_SET 1
#define ACTION_DEFAULTS 2

int main(int argc, char ** argv) {
	int argIndex;
	char * identifier = "preferences_testharness";
	Preferences * preferences;
	char defaultData1[] = {0x00, 0x01, 0x02, 0x03, 0x04};
	char defaultData2[] = {0xFE, 0xED, 0xFA, 0xCE};
	void * data;
	size_t length, byteIndex;
	int action = ACTION_NONE;
	
	for (argIndex = 0; argIndex < argc; argIndex++) {
		if (!strcmp(argv[argIndex], "--help")) {
			printUsage();
			return EXIT_SUCCESS;
		}
		
		if (!strcmp(argv[argIndex], "-i")) {
			if (argc < argIndex + 2) {
				printUsage();
				return EXIT_FAILURE;
			}
			identifier = argv[++argIndex];
			
		} else if (!strcmp(argv[argIndex], "-a")) {
			if (argc < argIndex + 2) {
				printUsage();
				return EXIT_FAILURE;
			}
			
			if (!strcmp(argv[argIndex + 1], "set")) {
				action = ACTION_SET;
				
			} else if (!strcmp(argv[argIndex + 1], "defaults")) {
				action = ACTION_DEFAULTS;
				
			} else {
				printUsage();
				return EXIT_FAILURE;
			}
			argIndex++;
		}
	}
	
	preferences = Preferences_create(identifier);
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
	
	printf("integer1: %d\n", Preferences_getInteger(preferences, "integer1"));
	printf("integer2: %d\n", Preferences_getInteger(preferences, "integer2"));
	printf("float1: %f\n", Preferences_getFloat(preferences, "float1"));
	printf("float2: %f\n", Preferences_getFloat(preferences, "float2"));
	printf("boolean1: %s\n", Preferences_getBoolean(preferences, "boolean1") ? "true" : "false");
	printf("boolean2: %s\n", Preferences_getBoolean(preferences, "boolean2") ? "true" : "false");
	printf("string1: %s\n", Preferences_getString(preferences, "string1"));
	printf("string2: %s\n", Preferences_getString(preferences, "string2"));
	printf("data1:");
	data = Preferences_getData(preferences, "data1", &length);
	for (byteIndex = 0; byteIndex < length; byteIndex++) {
		printf(" %02X", ((unsigned char *) data)[byteIndex]);
	}
	putchar('\n');
	printf("data2:");
	data = Preferences_getData(preferences, "data2", &length);
	for (byteIndex = 0; byteIndex < length; byteIndex++) {
		printf(" %02X", ((unsigned char *) data)[byteIndex]);
	}
	putchar('\n');
	
	if (action == ACTION_SET) {
		char data1[] = {0xFF, 0x7F, 0x00};
		char data2[] = {0xAA, 0xBB};
		
		Preferences_setInteger(preferences, "integer1", -50);
		Preferences_setInteger(preferences, "integer2", 123);
		Preferences_setFloat(preferences, "float1", -1.0f);
		Preferences_setFloat(preferences, "float2", 2.25f);
		Preferences_setBoolean(preferences, "boolean1", false);
		Preferences_setBoolean(preferences, "boolean2", true);
		Preferences_setString(preferences, "string1", "bar");
		Preferences_setString(preferences, "string2", "baz");
		Preferences_setData(preferences, "data1", data1, sizeof(data1));
		Preferences_setData(preferences, "data2", data2, sizeof(data2));
		Preferences_save(preferences);
		
	} else if (action == ACTION_DEFAULTS) {
		Preferences_loadDefaultValues(preferences);
		Preferences_save(preferences);
	}
	
	Preferences_dispose(preferences);
	
	return EXIT_SUCCESS;
}
