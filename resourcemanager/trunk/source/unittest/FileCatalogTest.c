#include "resourcemanager/FileCatalog.h"
#include "serialization/TestDeserializationContext.h"
#include "serialization/TestSerializationContext.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"
#include <limits.h>
#include <string.h>

static void verifyInit(FileCatalog * fileCatalog) {
	TestCase_assert(fileCatalog != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fileCatalog->dispose == FileCatalog_dispose, "Expected %p but got %p", FileCatalog_dispose, fileCatalog->dispose);
}

static void testInit() {
	FileCatalog fileCatalog, * fileCatalogPtr;
	bool success;
	
	memset(&fileCatalog, 0x00, sizeof(fileCatalog));
	success = FileCatalog_init(&fileCatalog);
	TestCase_assert(success, "Expected true but got false");
	verifyInit(&fileCatalog);
	FileCatalog_dispose(&fileCatalog);
	
	memset(&fileCatalog, 0xFF, sizeof(fileCatalog));
	success = FileCatalog_init(&fileCatalog);
	TestCase_assert(success, "Expected true but got false");
	verifyInit(&fileCatalog);
	FileCatalog_dispose(&fileCatalog);
	
	fileCatalogPtr = FileCatalog_create();
	verifyInit(fileCatalogPtr);
	FileCatalog_dispose(fileCatalogPtr);
}

static void testGetFilePath() {
	FileCatalog * fileCatalog;
	const char * path;
	
	fileCatalog = FileCatalog_create();
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type1"), ATOM("name1"));
	TestCase_assert(path == NULL, "Expected NULL but got %p", path);
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type1"), ATOM("name2"));
	TestCase_assert(path == NULL, "Expected NULL but got %p", path);
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type2"), ATOM("name1"));
	TestCase_assert(path == NULL, "Expected NULL but got %p", path);
	
	FileCatalog_setFilePath(fileCatalog, ATOM("type1"), ATOM("name1"), "a");
	FileCatalog_setFilePath(fileCatalog, ATOM("type1"), ATOM("name2"), "b");
	FileCatalog_setFilePath(fileCatalog, ATOM("type2"), ATOM("name1"), "c");
	
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type1"), ATOM("name1"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "a"), "Expected \"a\" but got \"%s\"", path);
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type1"), ATOM("name2"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "b"), "Expected \"b\" but got \"%s\"", path);
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type2"), ATOM("name1"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "c"), "Expected \"c\" but got \"%s\"", path);
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type2"), ATOM("name2"));
	TestCase_assert(path == NULL, "Expected NULL but got %p", path);
	
	FileCatalog_dispose(fileCatalog);
}

static void testSetBasePath() {
	FileCatalog * fileCatalog;
	const char * path;
	
	fileCatalog = FileCatalog_create();
	FileCatalog_setFilePath(fileCatalog, ATOM("type"), ATOM("file1"), "local/file");
	FileCatalog_setFilePath(fileCatalog, ATOM("type"), ATOM("file2"), "/absolute/file");
	
	FileCatalog_setBasePath(fileCatalog, "directory");
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type"), ATOM("file1"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "directory/local/file"), "Expected \"directory/local/file\" but got \"%s\"", path);
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type"), ATOM("file2"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "/absolute/file"), "Expected \"/absolute/file\" but got \"%s\"", path);
	
	FileCatalog_setBasePath(fileCatalog, "/dir/");
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type"), ATOM("file1"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "/dir/local/file"), "Expected \"/dir/local/file\" but got \"%s\"", path);
	path = FileCatalog_getFilePath(fileCatalog, ATOM("type"), ATOM("file2"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "/absolute/file"), "Expected \"/absolute/file\" but got \"%s\"", path);
	
	FileCatalog_dispose(fileCatalog);
}

static void testListing() {
	FileCatalog * fileCatalog;
	const char ** items;
	size_t itemCount;
	
	fileCatalog = FileCatalog_create();
	
	itemCount = SIZE_T_MAX;
	items = FileCatalog_listTypes(fileCatalog, &itemCount);
	TestCase_assert(itemCount == 0, "Expected 0 but got " SIZE_T_FORMAT, itemCount);
	free(items);
	
	FileCatalog_setFilePath(fileCatalog, ATOM("type1"), ATOM("name1"), "a");
	FileCatalog_setFilePath(fileCatalog, ATOM("type1"), ATOM("name2"), "b");
	FileCatalog_setFilePath(fileCatalog, ATOM("type2"), ATOM("name1"), "c");
	
	itemCount = SIZE_T_MAX;
	items = FileCatalog_listTypes(fileCatalog, &itemCount);
	TestCase_assert(itemCount == 2, "Expected 2 but got " SIZE_T_FORMAT, itemCount);
	TestCase_assert(items != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(items[0] != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(items[0], "type1"), "Expected \"type1\" but got \"%s\"", items[0]);
	TestCase_assert(items[1] != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(items[1], "type2"), "Expected \"type2\" but got \"%s\"", items[1]);
	free(items);
	
	itemCount = SIZE_T_MAX;
	items = FileCatalog_listNamesForType(fileCatalog, ATOM("type1"), &itemCount);
	TestCase_assert(itemCount == 2, "Expected 2 but got " SIZE_T_FORMAT, itemCount);
	TestCase_assert(items != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(items[0] != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(items[0], "name2"), "Expected \"name2\" but got \"%s\"", items[0]);
	TestCase_assert(items[1] != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(items[1], "name1"), "Expected \"name1\" but got \"%s\"", items[1]);
	free(items);
	
	itemCount = SIZE_T_MAX;
	items = FileCatalog_listNamesForType(fileCatalog, ATOM("type2"), &itemCount);
	TestCase_assert(itemCount == 1, "Expected 1 but got " SIZE_T_FORMAT, itemCount);
	TestCase_assert(items != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(items[0] != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(items[0], "name1"), "Expected \"name1\" but got \"%s\"", items[0]);
	free(items);
	
	itemCount = SIZE_T_MAX;
	items = FileCatalog_listNamesForType(fileCatalog, ATOM("invalid"), &itemCount);
	TestCase_assert(itemCount == 0, "Expected 0 but got " SIZE_T_FORMAT, itemCount);
	free(items);
}

static void testDeserialization() {
	FileCatalog fileCatalog, * fileCatalogPtr;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	bool success;
	const char * path;
	unsigned int failIndex;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->readUInt16, "format_version", FILECATALOG_FORMAT_VERSION);
	context->expectCall(context, context->readString, "format_type", FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->beginDictionary, "types", 0);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	success = FileCatalog_loadSerializedData(&fileCatalog, context);
	
	context->finish(context);
	context->dispose(context);
	
	verifyInit(&fileCatalog);
	FileCatalog_dispose(&fileCatalog);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->readUInt16, "format_version", FILECATALOG_FORMAT_VERSION);
	context->expectCall(context, context->readString, "format_type", FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->beginDictionary, "types", 0);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	fileCatalogPtr = FileCatalog_deserialize(context);
	
	context->finish(context);
	context->dispose(context);
	
	verifyInit(fileCatalogPtr);
	FileCatalog_dispose(fileCatalogPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->readUInt16, "format_version", FILECATALOG_FORMAT_VERSION);
	context->expectCall(context, context->readString, "format_type", FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->beginDictionary, "types", 2);
	context->expectCall(context, context->readNextDictionaryKey, "type1");
	context->expectCall(context, context->beginDictionary, "type1", 2);
	context->expectCall(context, context->readNextDictionaryKey, "name1");
	context->expectCall(context, context->readString, "name1", "a");
	context->expectCall(context, context->readNextDictionaryKey, "name2");
	context->expectCall(context, context->readString, "name2", "b");
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->readNextDictionaryKey, "type2");
	context->expectCall(context, context->beginDictionary, "type2", 1);
	context->expectCall(context, context->readNextDictionaryKey, "name1");
	context->expectCall(context, context->readString, "name1", "c");
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	fileCatalogPtr = FileCatalog_deserialize(context);
	
	context->finish(context);
	context->dispose(context);
	
	verifyInit(fileCatalogPtr);
	path = FileCatalog_getFilePath(fileCatalogPtr, ATOM("type1"), ATOM("name1"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "a"), "Expected \"a\" but got \"%s\"", path);
	path = FileCatalog_getFilePath(fileCatalogPtr, ATOM("type1"), ATOM("name2"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "b"), "Expected \"b\" but got \"%s\"", path);
	path = FileCatalog_getFilePath(fileCatalogPtr, ATOM("type2"), ATOM("name1"));
	TestCase_assert(path != NULL, "Expected non-NULL but got NULL", path);
	TestCase_assert(!strcmp(path, "c"), "Expected \"c\" but got \"%s\"", path);
	FileCatalog_dispose(fileCatalogPtr);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	context->expectCall(context, context->beginStructure, FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->readUInt16, "format_version", FILECATALOG_FORMAT_VERSION);
	context->expectCall(context, context->readString, "format_type", FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->beginDictionary, "types", 1);
	context->expectCall(context, context->readNextDictionaryKey, "type1");
	context->expectCall(context, context->beginDictionary, "type1", 1);
	context->expectCall(context, context->readNextDictionaryKey, "name1");
	context->expectCall(context, context->readString, "name1", "a");
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	for (failIndex = 0; failIndex < 11; failIndex++) {
		context->rewind(context);
		context->failNthCall(context, failIndex, 1);
		fileCatalogPtr = FileCatalog_deserialize(context);
		TestCase_assert(fileCatalogPtr == NULL, "FileCatalog_deserialize didn't return NULL when deserialization call %d failed", failIndex);
	}
	context->dispose(context);
}

static void testSerialization() {
	FileCatalog * fileCatalog;
	TestSerializationContext * context;
	jmp_buf jmpEnv;
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	fileCatalog = FileCatalog_create();
	FileCatalog_setFilePath(fileCatalog, ATOM("type1"), ATOM("name1"), "a");
	
	context->expectCall(context, context->beginStructure, FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->writeUInt16, "format_version", FILECATALOG_FORMAT_VERSION);
	context->expectCall(context, context->writeString, "format_type", FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->beginDictionary, "types");
	context->expectCall(context, context->beginDictionary, "type1");
	context->expectCall(context, context->writeString, "name1", "a");
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	FileCatalog_serialize(fileCatalog, context);
	
	context->finish(context);
	context->dispose(context);
	FileCatalog_dispose(fileCatalog);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	fileCatalog = FileCatalog_create();
	FileCatalog_setFilePath(fileCatalog, ATOM("type1"), ATOM("name1"), "a");
	FileCatalog_setFilePath(fileCatalog, ATOM("type1"), ATOM("name2"), "b");
	FileCatalog_setFilePath(fileCatalog, ATOM("type2"), ATOM("name1"), "c");
	
	context->expectCall(context, context->beginStructure, FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->writeUInt16, "format_version", FILECATALOG_FORMAT_VERSION);
	context->expectCall(context, context->writeString, "format_type", FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->beginDictionary, "types");
	context->expectCall(context, context->beginDictionary, "type1");
	context->expectCall(context, context->writeString, "name2", "b");
	context->expectCall(context, context->writeString, "name1", "a");
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->beginDictionary, "type2");
	context->expectCall(context, context->writeString, "name1", "c");
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	FileCatalog_serialize(fileCatalog, context);
	
	context->finish(context);
	context->dispose(context);
	FileCatalog_dispose(fileCatalog);
}

static void testFormatVerification() {
	FileCatalog * fileCatalog;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->readUInt16, "format_version", FILECATALOG_FORMAT_VERSION);
	context->expectCall(context, context->readString, "format_type", FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->beginDictionary, "types", 0);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	fileCatalog = FileCatalog_deserialize(context);
	
	context->finish(context);
	context->dispose(context);
	
	TestCase_assert(fileCatalog != NULL, "FileCatalog_deserialize returned NULL with correct format_version and format_type");
	FileCatalog_dispose(fileCatalog);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->readUInt16, "format_version", FILECATALOG_FORMAT_VERSION + 1);
	context->expectCall(context, context->readString, "format_type", FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->beginDictionary, "types", 0);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	fileCatalog = FileCatalog_deserialize(context);
	
	// Don't context->finish(), because not all expected calls should have executed
	context->dispose(context);
	
	TestCase_assert(fileCatalog == NULL, "FileCatalog_deserialize didn't return NULL when format_version was too high");
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginStructure, FILECATALOG_FORMAT_TYPE);
	context->expectCall(context, context->readUInt16, "format_version", FILECATALOG_FORMAT_VERSION);
	context->expectCall(context, context->readString, "format_type", FILECATALOG_FORMAT_TYPE "_not");
	context->expectCall(context, context->beginDictionary, "types", 0);
	context->expectCall(context, context->endDictionary);
	context->expectCall(context, context->endStructure);
	
	fileCatalog = FileCatalog_deserialize(context);
	
	// Don't context->finish(), because not all expected calls should have executed
	context->dispose(context);
	
	TestCase_assert(fileCatalog == NULL, "FileCatalog_deserialize didn't return NULL when format_type was incorrect");
}

TEST_SUITE(FileCatalogTest,
           testInit,
           testGetFilePath,
           testSetBasePath,
           testListing,
           testDeserialization,
           testSerialization,
           testFormatVerification)
