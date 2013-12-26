#include "jsonio/JSONEmitter.h"
#include "unittest/TestSuite.h"
#include "unittest/printfFormats.h"
#include "utilities/IOUtilities.h"
#include <unistd.h>

#define writeNodeToTemporaryFile(node, outString, outLength, format, outError) { \
	const char * tempFilePath; \
	int fd; \
	\
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd); \
	if (JSONEmitter_writeFile(node, format, tempFilePath, outError)) { \
		outString = readFileSimple(tempFilePath, &length); \
	} else { \
		outString = NULL; \
	} \
	close(fd); \
	unlink(tempFilePath); \
}

static void testTopLevelNode() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_ARRAY;
	node.value.count = 0;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 2, "Expected 2 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[]", length), "Expected \"[]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 2, "Expected 2 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[]", length), "Expected \"[]\" but got \"%s\"", string);
	free(string);
	
	node.type = JSON_TYPE_OBJECT;
	node.value.count = 0;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 2, "Expected 2 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{}", length), "Expected \"{}\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 2, "Expected 2 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{}", length), "Expected \"{}\" but got \"%s\"", string);
	free(string);
}

static void testNumberValues() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_ARRAY;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].type = JSON_TYPE_NUMBER;
	node.subitems[0].value.number = 1;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 3, "Expected 3 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[1]", length), "Expected \"[1]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 3, "Expected 3 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[1]", length), "Expected \"[1]\" but got \"%s\"", string);
	free(string);
	
	node.subitems[0].value.number = -8.25;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 7, "Expected 7 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[-8.25]", length), "Expected \"[-8.25]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 7, "Expected 7 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[-8.25]", length), "Expected \"[-8.25]\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems);
}

static void testStringValues() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_ARRAY;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].type = JSON_TYPE_STRING;
	node.subitems[0].value.string = "";
	node.subitems[0].stringLength = 0;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 4, "Expected 4 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\"]", length), "Expected \"[\"\"]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 4, "Expected 4 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\"]", length), "Expected \"[\"\"]\" but got \"%s\"", string);
	free(string);
	
	node.subitems[0].value.string = "hello";
	node.subitems[0].stringLength = 5;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 9, "Expected 9 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"hello\"]", length), "Expected \"[\"hello\"]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 9, "Expected 9 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"hello\"]", length), "Expected \"[\"hello\"]\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems);
}

static void testBooleanValues() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_ARRAY;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].type = JSON_TYPE_BOOLEAN;
	node.subitems[0].value.boolean = false;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 7, "Expected 7 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[false]", length), "Expected \"[false]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 7, "Expected 7 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[false]", length), "Expected \"[false]\" but got \"%s\"", string);
	free(string);
	
	node.subitems[0].value.boolean = true;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 6, "Expected 6 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[true]", length), "Expected \"[true]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 6, "Expected 6 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[true]", length), "Expected \"[true]\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems);
}

static void testNullValue() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_ARRAY;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].type = JSON_TYPE_NULL;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 6, "Expected 6 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[null]", length), "Expected \"[null]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 6, "Expected 6 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[null]", length), "Expected \"[null]\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems);
}

static void testArrays() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_ARRAY;
	node.subitems = malloc(sizeof(struct JSONNode) * 3);
	node.value.count = 2;
	node.subitems[0].type = JSON_TYPE_BOOLEAN;
	node.subitems[0].value.boolean = false;
	node.subitems[1].type = JSON_TYPE_BOOLEAN;
	node.subitems[1].value.boolean = true;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 13, "Expected 13 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[false, true]", length), "Expected \"[false, true]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 13, "Expected 13 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[false, true]", length), "Expected \"[false, true]\" but got \"%s\"", string);
	free(string);
	
	node.value.count = 3;
	node.subitems[0].value.boolean = true;
	node.subitems[1].type = JSON_TYPE_NUMBER;
	node.subitems[1].value.number = 3;
	node.subitems[2].type = JSON_TYPE_NULL;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 15, "Expected 15 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[true, 3, null]", length), "Expected \"[true, 3, null]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 15, "Expected 15 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[true, 3, null]", length), "Expected \"[true, 3, null]\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems);
}

static void testObjects() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_OBJECT;
	node.subitems = malloc(sizeof(struct JSONNode) * 3);
	node.value.count = 2;
	node.subitems[0].key = "a";
	node.subitems[0].keyLength = 1;
	node.subitems[0].type = JSON_TYPE_BOOLEAN;
	node.subitems[0].value.boolean = false;
	node.subitems[1].key = "b";
	node.subitems[1].keyLength = 1;
	node.subitems[1].type = JSON_TYPE_BOOLEAN;
	node.subitems[1].value.boolean = true;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 23, "Expected 23 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{\"a\": false, \"b\": true}", length), "Expected \"{\"a\": false, \"b\": true}\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 23, "Expected 23 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{\"a\": false, \"b\": true}", length), "Expected \"{\"a\": false, \"b\": true}\" but got \"%s\"", string);
	free(string);
	
	node.value.count = 3;
	node.subitems[0].key = "foo";
	node.subitems[0].keyLength = 3;
	node.subitems[0].type = JSON_TYPE_NUMBER;
	node.subitems[0].value.number = 1;
	node.subitems[1].key = "bar";
	node.subitems[1].keyLength = 3;
	node.subitems[1].type = JSON_TYPE_NULL;
	node.subitems[2].key = "baz";
	node.subitems[2].keyLength = 3;
	node.subitems[2].type = JSON_TYPE_STRING;
	node.subitems[2].value.string = "test";
	node.subitems[2].stringLength = 4;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 38, "Expected 38 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{\"foo\": 1, \"bar\": null, \"baz\": \"test\"}", length), "Expected \"{\"foo\": 1, \"bar\": null, \"baz\": \"test\"}\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 38, "Expected 38 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{\"foo\": 1, \"bar\": null, \"baz\": \"test\"}", length), "Expected \"{\"foo\": 1, \"bar\": null, \"baz\": \"test\"}\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems);
}

static void testNestedArraysAndObjects() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_ARRAY;
	node.subitems = malloc(sizeof(struct JSONNode) * 3);
	node.value.count = 2;
	node.subitems[0].type = JSON_TYPE_OBJECT;
	node.subitems[0].value.count = 0;
	node.subitems[0].subitems = NULL;
	node.subitems[1].type = JSON_TYPE_ARRAY;
	node.subitems[1].value.count = 0;
	node.subitems[1].subitems = NULL;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[{}, []]", length), "Expected \"[{}, []]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[{}, []]", length), "Expected \"[{}, []]\" but got \"%s\"", string);
	free(string);
	
	node.type = JSON_TYPE_OBJECT;
	node.value.count = 3;
	node.subitems[0].key = "1";
	node.subitems[0].keyLength = 1;
	node.subitems[0].type = JSON_TYPE_ARRAY;
	node.subitems[0].value.count = 3;
	node.subitems[0].subitems = malloc(sizeof(struct JSONNode) * 3);
	node.subitems[0].subitems[0].type = JSON_TYPE_BOOLEAN;
	node.subitems[0].subitems[0].value.boolean = false;
	node.subitems[0].subitems[1].type = JSON_TYPE_NUMBER;
	node.subitems[0].subitems[1].value.number = 2;
	node.subitems[0].subitems[2].type = JSON_TYPE_OBJECT;
	node.subitems[0].subitems[2].value.count = 0;
	node.subitems[0].subitems[2].subitems = NULL;
	node.subitems[1].key = "2";
	node.subitems[1].keyLength = 1;
	node.subitems[1].type = JSON_TYPE_OBJECT;
	node.subitems[1].value.count = 1;
	node.subitems[1].subitems = malloc(sizeof(struct JSONNode));
	node.subitems[1].subitems[0].key = "";
	node.subitems[1].subitems[0].keyLength = 0;
	node.subitems[1].subitems[0].type = JSON_TYPE_ARRAY;
	node.subitems[1].subitems[0].value.count = 0;
	node.subitems[1].subitems[0].subitems = NULL;
	node.subitems[2].key = "3";
	node.subitems[2].keyLength = 1;
	node.subitems[2].type = JSON_TYPE_NULL;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 47, "Expected 47 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{\"1\": [false, 2, {}], \"2\": {\"\": []}, \"3\": null}", length), "Expected \"{\"1\": [false, 2, {}], \"2\": {\"\": []}, \"3\": null}\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 47, "Expected 47 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{\"1\": [false, 2, {}], \"2\": {\"\": []}, \"3\": null}", length), "Expected \"{\"1\": [false, 2, {}], \"2\": {\"\": []}, \"3\": null}\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems[0].subitems);
	free(node.subitems[1].subitems);
	free(node.subitems);
}

static void testStringEscapes() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_ARRAY;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].type = JSON_TYPE_STRING;
	node.subitems[0].stringLength = 1;
	node.subitems[0].value.string = "\\";
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 6, "Expected 6 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\\\\\"]", length), "Expected \"[\"\\\\\"]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 6, "Expected 6 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\\\\\"]", length), "Expected \"[\"\\\\\"]\" but got \"%s\"", string);
	free(string);
	
	node.subitems[0].value.string = "\"";
	node.subitems[0].stringLength = 1;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 6, "Expected 6 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\\\"\"]", length), "Expected \"[\"\\\"\"]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 6, "Expected 6 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\\\"\"]", length), "Expected \"[\"\\\"\"]\" but got \"%s\"", string);
	free(string);
	
	node.subitems[0].value.string = "\b\f\n\r\t";
	node.subitems[0].stringLength = 5;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 13, "Expected 13 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\\b\\f\\n\\r\t\"]", length), "Expected \"[\"\\b\\f\\n\\r\t\"]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 13, "Expected 13 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\\b\\f\\n\\r\t\"]", length), "Expected \"[\"\\b\\f\\n\\r\t\"]\" but got \"%s\"", string);
	free(string);
	
	node.subitems[0].value.string = "\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E";
	node.subitems[0].stringLength = 8;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 12, "Expected 12 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\"]", length), "Expected \"[\"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\"]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 12, "Expected 12 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\"]", length), "Expected \"[\"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\"]\" but got \"%s\"", string);
	free(string);
	
	node.subitems[0].value.string = "a\0b";
	node.subitems[0].stringLength = 3;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 12, "Expected 12 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"a\\u0000b\"]", length), "Expected \"[\"a\\u0000b\"]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 12, "Expected 12 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"a\\u0000b\"]", length), "Expected \"[\"a\\u0000b\"]\" but got \"%s\"", string);
	free(string);
	
	node.subitems[0].value.string = "/";
	node.subitems[0].stringLength = 1;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 5, "Expected 5 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"/\"]", length), "Expected \"[\"/\"]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 5, "Expected 5 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\"/\"]", length), "Expected \"[\"/\"]\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems);
}

static void testInvalidInput() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_NULL;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	
	node.type = JSON_TYPE_OBJECT;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].key = NULL;
	node.subitems[0].keyLength = 0;
	node.subitems[0].type = JSON_TYPE_NULL;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	
	node.subitems[0].key = "";
	node.subitems[0].type = JSON_TYPE_STRING;
	node.subitems[0].value.string = NULL;
	node.subitems[0].stringLength = 0;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	
	node.subitems[0].type = JSON_TYPE_ARRAY;
	node.subitems[0].value.count = 1;
	node.subitems[0].subitems = NULL;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	
	node.subitems[0].type = JSON_TYPE_OBJECT;
	node.subitems[0].value.count = 1;
	node.subitems[0].subitems = NULL;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	
	node.subitems[0].type = -1;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
}

static void testErrorReporting() {
	char * string;
	size_t length;
	struct JSONNode node;
	struct JSONEmissionError error, errorPrototype = {NULL, -1, NULL};
	
	node.type = JSON_TYPE_NULL;
	error = errorPrototype;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, &error);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	TestCase_assert(error.node == &node, "Expected %p but got %p", &node, error.node);
	TestCase_assert(error.code == JSONEmissionError_invalidNodeType, "Expected %d but got %d", JSONEmissionError_invalidNodeType, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	node.type = JSON_TYPE_OBJECT;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].key = NULL;
	node.subitems[0].keyLength = 0;
	node.subitems[0].type = JSON_TYPE_NULL;
	error = errorPrototype;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, &error);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	TestCase_assert(error.node == node.subitems, "Expected %p but got %p", node.subitems, error.node);
	TestCase_assert(error.code == JSONEmissionError_nullKeyString, "Expected %d but got %d", JSONEmissionError_nullKeyString, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	node.subitems[0].key = "";
	node.subitems[0].type = JSON_TYPE_STRING;
	node.subitems[0].value.string = NULL;
	node.subitems[0].stringLength = 0;
	error = errorPrototype;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, &error);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	TestCase_assert(error.node == node.subitems, "Expected %p but got %p", node.subitems, error.node);
	TestCase_assert(error.code == JSONEmissionError_nullValueString, "Expected %d but got %d", JSONEmissionError_nullValueString, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	node.subitems[0].type = JSON_TYPE_ARRAY;
	node.subitems[0].value.count = 1;
	node.subitems[0].subitems = NULL;
	error = errorPrototype;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, &error);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	TestCase_assert(error.node == node.subitems, "Expected %p but got %p", node.subitems, error.node);
	TestCase_assert(error.code == JSONEmissionError_nullSubitems, "Expected %d but got %d", JSONEmissionError_nullSubitems, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	node.subitems[0].type = JSON_TYPE_OBJECT;
	node.subitems[0].value.count = 1;
	node.subitems[0].subitems = NULL;
	error = errorPrototype;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, &error);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	TestCase_assert(error.node == node.subitems, "Expected %p but got %p", node.subitems, error.node);
	TestCase_assert(error.code == JSONEmissionError_nullSubitems, "Expected %d but got %d", JSONEmissionError_nullSubitems, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	node.subitems[0].type = -1;
	error = errorPrototype;
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, &error);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	TestCase_assert(error.node == node.subitems, "Expected %p but got %p", node.subitems, error.node);
	TestCase_assert(error.code == JSONEmissionError_invalidNodeType, "Expected %d but got %d", JSONEmissionError_invalidNodeType, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
}

static void testFormatVariations() {
	char * string;
	size_t length;
	struct JSONNode node;
	
	node.type = JSON_TYPE_ARRAY;
	node.subitems = malloc(sizeof(struct JSONNode) * 6);
	node.value.count = 6;
	node.subitems[0].type = JSON_TYPE_NULL;
	node.subitems[1].type = JSON_TYPE_NUMBER;
	node.subitems[1].value.number = 1;
	node.subitems[2].type = JSON_TYPE_BOOLEAN;
	node.subitems[2].value.boolean = false;
	node.subitems[3].type = JSON_TYPE_OBJECT;
	node.subitems[3].value.count = 2;
	node.subitems[3].subitems = malloc(sizeof(struct JSONNode) * 2);
	node.subitems[3].subitems[0].key = "string";
	node.subitems[3].subitems[0].keyLength = 6;
	node.subitems[3].subitems[0].type = JSON_TYPE_BOOLEAN;
	node.subitems[3].subitems[0].value.boolean = true;
	node.subitems[3].subitems[1].key = "array";
	node.subitems[3].subitems[1].keyLength = 5;
	node.subitems[3].subitems[1].type = JSON_TYPE_ARRAY;
	node.subitems[3].subitems[1].value.count = 1;
	node.subitems[3].subitems[1].subitems = malloc(sizeof(struct JSONNode));
	node.subitems[3].subitems[1].subitems[0].type = JSON_TYPE_NULL;
	node.subitems[4].type = JSON_TYPE_STRING;
	node.subitems[4].value.string = "string";
	node.subitems[4].stringLength = 6;
	node.subitems[5].type = JSON_TYPE_OBJECT;
	node.subitems[5].value.count = 0;
	node.subitems[5].subitems = NULL;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 65, "Expected 65 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "[null, 1, false, {\"string\": true, \"array\": [null]}, \"string\", {}]"), "Expected \"[null, 1, false, {\"string\": true, \"array\": [null]}, \"string\", {}]\" but got \"%s\"", string);
	free(string);
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_compact, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 57, "Expected 57 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "[null,1,false,{\"string\":true,\"array\":[null]},\"string\",{}]"), "Expected \"[null,1,false,{\"string\":true,\"array\":[null]},\"string\",{}]\" but got \"%s\"", string);
	free(string);
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_multiLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 89, "Expected 89 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "[\n\tnull,\n\t1,\n\tfalse,\n\t{\n\t\t\"string\": true,\n\t\t\"array\": [\n\t\t\tnull\n\t\t]\n\t},\n\t\"string\",\n\t{\n\t}\n]"), "Expected \"[\n\tnull,\n\t1,\n\tfalse,\n\t{\n\t\t\"string\": true,\n\t\t\"array\": [\n\t\t\tnull\n\t\t]\n\t},\n\t\"string\",\n\t{\n\t}\n]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 65, "Expected 65 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[null, 1, false, {\"string\": true, \"array\": [null]}, \"string\", {}]", length), "Expected \"[null, 1, false, {\"string\": true, \"array\": [null]}, \"string\", {}]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_compact, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 57, "Expected 57 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[null,1,false,{\"string\":true,\"array\":[null]},\"string\",{}]", length), "Expected \"[null,1,false,{\"string\":true,\"array\":[null]},\"string\",{}]\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_multiLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 89, "Expected 89 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "[\n\tnull,\n\t1,\n\tfalse,\n\t{\n\t\t\"string\": true,\n\t\t\"array\": [\n\t\t\tnull\n\t\t]\n\t},\n\t\"string\",\n\t{\n\t}\n]", length), "Expected \"[\n\tnull,\n\t1,\n\tfalse,\n\t{\n\t\t\"string\": true,\n\t\t\"array\": [\n\t\t\tnull\n\t\t]\n\t},\n\t\"string\",\n\t{\n\t}\n]\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems[3].subitems[1].subitems);
	free(node.subitems[3].subitems);
	free(node.subitems);
	
	node.type = JSON_TYPE_OBJECT;
	node.subitems = malloc(sizeof(struct JSONNode) * 3);
	node.value.count = 3;
	node.subitems[0].key = "string";
	node.subitems[0].keyLength = 6;
	node.subitems[0].type = JSON_TYPE_BOOLEAN;
	node.subitems[0].value.boolean = true;
	node.subitems[1].key = "array";
	node.subitems[1].keyLength = 5;
	node.subitems[1].type = JSON_TYPE_ARRAY;
	node.subitems[1].value.count = 4;
	node.subitems[1].subitems = malloc(sizeof(struct JSONNode) * 4);
	node.subitems[1].subitems[0].type = JSON_TYPE_NULL;
	node.subitems[1].subitems[1].type = JSON_TYPE_NUMBER;
	node.subitems[1].subitems[1].value.number = 1;
	node.subitems[1].subitems[2].type = JSON_TYPE_BOOLEAN;
	node.subitems[1].subitems[2].value.boolean = false;
	node.subitems[1].subitems[3].type = JSON_TYPE_OBJECT;
	node.subitems[1].subitems[3].value.count = 1;
	node.subitems[1].subitems[3].subitems = malloc(sizeof(struct JSONNode));
	node.subitems[1].subitems[3].subitems[0].key = "";
	node.subitems[1].subitems[3].subitems[0].keyLength = 0;
	node.subitems[1].subitems[3].subitems[0].type = JSON_TYPE_NULL;
	node.subitems[2].key = "";
	node.subitems[2].keyLength = 0;
	node.subitems[2].type = JSON_TYPE_ARRAY;
	node.subitems[2].value.count = 0;
	node.subitems[2].subitems = NULL;
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_singleLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 63, "Expected 63 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "{\"string\": true, \"array\": [null, 1, false, {\"\": null}], \"\": []}"), "Expected \"{\"string\": true, \"array\": [null, 1, false, {\"\": null}], \"\": []}\" but got \"%s\"", string);
	free(string);
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_compact, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 54, "Expected 54 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "{\"string\":true,\"array\":[null,1,false,{\"\":null}],\"\":[]}"), "Expected \"{\"string\":true,\"array\":[null,1,false,{\"\":null}],\"\":[]}\" but got \"%s\"", string);
	free(string);
	
	string = JSONEmitter_writeString(&node, JSONEmitterFormat_multiLine, &length, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 88, "Expected 88 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "{\n\t\"string\": true,\n\t\"array\": [\n\t\tnull,\n\t\t1,\n\t\tfalse,\n\t\t{\n\t\t\t\"\": null\n\t\t}\n\t],\n\t\"\": [\n\t]\n}"), "Expected \"{\n\t\"string\": true,\n\t\"array\": [\n\t\tnull,\n\t\t1,\n\t\tfalse,\n\t\t{\n\t\t\t\"\": null\n\t\t}\n\t],\n\t\"\": [\n\t]\n}\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_singleLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 63, "Expected 63 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{\"string\": true, \"array\": [null, 1, false, {\"\": null}], \"\": []}", length), "Expected \"{\"string\": true, \"array\": [null, 1, false, {\"\": null}], \"\": []}\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_compact, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 54, "Expected 54 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{\"string\":true,\"array\":[null,1,false,{\"\":null}],\"\":[]}", length), "Expected \"{\"string\":true,\"array\":[null,1,false,{\"\":null}],\"\":[]}\" but got \"%s\"", string);
	free(string);
	
	writeNodeToTemporaryFile(&node, string, &length, JSONEmitterFormat_multiLine, NULL);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 88, "Expected 88 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strncmp(string, "{\n\t\"string\": true,\n\t\"array\": [\n\t\tnull,\n\t\t1,\n\t\tfalse,\n\t\t{\n\t\t\t\"\": null\n\t\t}\n\t],\n\t\"\": [\n\t]\n}", length), "Expected \"{\n\t\"string\": true,\n\t\"array\": [\n\t\tnull,\n\t\t1,\n\t\tfalse,\n\t\t{\n\t\t\t\"\": null\n\t\t}\n\t],\n\t\"\": [\n\t]\n}\" but got \"%s\"", string);
	free(string);
	
	free(node.subitems[1].subitems[3].subitems);
	free(node.subitems[1].subitems);
	free(node.subitems);
}

TEST_SUITE(JSONEmitterTest,
           testTopLevelNode,
           testNumberValues,
           testStringValues,
           testBooleanValues,
           testNullValue,
           testArrays,
           testObjects,
           testNestedArraysAndObjects,
           testStringEscapes,
           testInvalidInput,
           testErrorReporting,
           testFormatVariations)
