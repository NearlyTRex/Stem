#include "jsonio/JSONIO.h"
#include "unittest/framework/TestSuite.h"
#include "jsonio/JSONParser.h"
#include "jsonio/JSONEmitter.h"

#define stringAndLength(str) str, strlen(str)
#if defined(WIN32)
#define SIZE_T_FORMAT "%Iu"
#else
#define SIZE_T_FORMAT "%zu"
#endif

static void testEscapeJSONString() {
	char * string;
	size_t length;
	
	string = escapeJSONString(stringAndLength("\\"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 2, "Expected 2 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "\\\\"), "Expected \"\\\\\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString(stringAndLength("\""), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 2, "Expected 2 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "\\\""), "Expected \"\\\"\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString(stringAndLength("\b\f\n\r\t"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 9, "Expected 9 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "\\b\\f\\n\\r\t"), "Expected \"\\b\\f\\n\\r\t\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString(stringAndLength("\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E"), "Expected \"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString("a\0b", 3, &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "a\\u0000b"), "Expected \"a\\u0000b\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString("/", 1, &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 1, "Expected 1 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "/"), "Expected \"/\" but got \"%s\"", string);
	free(string);
}

static void testUnescapeJSONString() {
	char * string;
	size_t length;
	
	string = unescapeJSONString(stringAndLength("\\\\"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 1, "Expected 1 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "\\"), "Expected \"\\\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("\\\""), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 1, "Expected 1 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "\""), "Expected \"\"\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("\\b\\f\\n\\r\\t"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 5, "Expected 5 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "\b\f\n\r\t"), "Expected \"\b\f\n\r\t\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("\\u0020\\u26A0\\uD834\\udd1e"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 8, "Expected 8 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E"), "Expected \"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("a\\u0000b"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 3, "Expected 3 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!memcmp(string, "a\0b", 4), "Expected \"a\\0b\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("\\/"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 1, "Expected 1 but got " SIZE_T_FORMAT, length);
	TestCase_assert(!strcmp(string, "/"), "Expected \"/\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("\\u000"), &length);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	
	string = unescapeJSONString(stringAndLength("\\unvld"), &length);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
	
	string = unescapeJSONString(stringAndLength("\\uD800"), &length);
	TestCase_assert(string == NULL, "Expected NULL but got %p", string);
}

static void testJSONNode_subitemForKey() {
	struct JSONNode node;
	size_t subitemIndex;
	
	node.type = JSON_TYPE_OBJECT;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].key = "foo";
	node.subitems[0].keyLength = 3;
	node.subitems[0].type = JSON_TYPE_NULL;
	
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("foo"));
	TestCase_assert(subitemIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, subitemIndex);
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("bar"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, JSON_SUBITEM_NOT_FOUND, subitemIndex);
	free(node.subitems);
	
	node.type = JSON_TYPE_OBJECT;
	node.value.count = 3;
	node.subitems = malloc(sizeof(struct JSONNode) * 3);
	node.subitems[0].key = "bar";
	node.subitems[0].keyLength = 3;
	node.subitems[0].type = JSON_TYPE_NULL;
	node.subitems[1].key = "f\0oo";
	node.subitems[1].keyLength = 4;
	node.subitems[1].type = JSON_TYPE_NULL;
	node.subitems[2].key = "f\0oo";
	node.subitems[2].keyLength = 4;
	node.subitems[2].type = JSON_TYPE_NULL;
	
	subitemIndex = JSONNode_subitemIndexForKey(&node, "f\0oo", 4);
	TestCase_assert(subitemIndex == 1, "Expected 1 but got " SIZE_T_FORMAT, subitemIndex);
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("f"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, JSON_SUBITEM_NOT_FOUND, subitemIndex);
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("bar"));
	TestCase_assert(subitemIndex == 0, "Expected 0 but got " SIZE_T_FORMAT, subitemIndex);
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("foo"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, JSON_SUBITEM_NOT_FOUND, subitemIndex);
	free(node.subitems);
	
	node.type = JSON_TYPE_ARRAY;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].type = JSON_TYPE_NULL;
	
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("foo"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, JSON_SUBITEM_NOT_FOUND, subitemIndex);
	free(node.subitems);
	
	node.type = JSON_TYPE_NULL;
	node.subitems = NULL;
	
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("foo"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, JSON_SUBITEM_NOT_FOUND, subitemIndex);
}

static void testJSONNode_copy() {
	struct JSONNode * node1, * node2;
	char * string1, * string2;
	
	node1 = JSONParser_loadString(stringAndLength("[{\"item\": 0}, [1, {\"item\": 2, \"item\": 3, \"item\": 4, \"item\": 5, \"item\": 6, \"item\": 7}, 8, 9, \"10\", true, \"enum\", [\"13\"], [\"14\"], [\"15\"], [\"16\"]]]"), NULL);
	TestCase_assert(node1 != NULL, "Expected non-NULL but got NULL");
	if (node1 == NULL) {return;} // Suppress clang warning
	node2 = JSONNode_copy(node1);
	TestCase_assert(node2 != NULL, "Expected non-NULL but got NULL");
	if (node2 == NULL) {return;} // Suppress clang warning
	TestCase_assert(node1 != node2, "Pointers expected to differ, but didn't");
	string1 = JSONEmitter_writeString(node1, JSONEmitterFormat_compact, NULL, NULL);
	TestCase_assert(string1 != NULL, "Expected non-NULL but got NULL");
	string2 = JSONEmitter_writeString(node2, JSONEmitterFormat_compact, NULL, NULL);
	TestCase_assert(string2 != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(string1, string2), "Strings didn't match:\n  %s\n  %s", string1, string2);
	free(string1);
	free(string2);
	TestCase_assert(node1->subitems != node2->subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[0].subitems != node2->subitems[0].subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[0].subitems[0].key != node2->subitems[0].subitems[0].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems != node2->subitems[1].subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems != node2->subitems[1].subitems[1].subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[0].key != node2->subitems[1].subitems[1].subitems[0].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[1].key != node2->subitems[1].subitems[1].subitems[1].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[2].key != node2->subitems[1].subitems[1].subitems[2].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[3].key != node2->subitems[1].subitems[1].subitems[3].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[4].key != node2->subitems[1].subitems[1].subitems[4].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[5].key != node2->subitems[1].subitems[1].subitems[5].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[4].value.string != node2->subitems[1].subitems[4].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[6].value.string != node2->subitems[1].subitems[6].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[7].subitems[0].value.string != node2->subitems[1].subitems[7].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[8].subitems[0].value.string != node2->subitems[1].subitems[8].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[9].subitems[0].value.string != node2->subitems[1].subitems[9].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[10].subitems[0].value.string != node2->subitems[1].subitems[10].subitems[0].value.string, "Pointers expected to differ, but didn't");
	JSONNode_dispose(node2);
	
	node2 = malloc(sizeof(struct JSONNode));
	JSONNode_copyContents(node1, node2);
	TestCase_assert(node2 != NULL, "Expected non-NULL but got NULL");
	if (node2 == NULL) {return;} // Suppress clang warning
	string1 = JSONEmitter_writeString(node1, JSONEmitterFormat_compact, NULL, NULL);
	TestCase_assert(string1 != NULL, "Expected non-NULL but got NULL");
	string2 = JSONEmitter_writeString(node2, JSONEmitterFormat_compact, NULL, NULL);
	TestCase_assert(string2 != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(!strcmp(string1, string2), "Strings didn't match:\n  %s\n  %s", string1, string2);
	free(string1);
	free(string2);
	TestCase_assert(node1->subitems != node2->subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[0].subitems != node2->subitems[0].subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[0].subitems[0].key != node2->subitems[0].subitems[0].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems != node2->subitems[1].subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems != node2->subitems[1].subitems[1].subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[0].key != node2->subitems[1].subitems[1].subitems[0].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[1].key != node2->subitems[1].subitems[1].subitems[1].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[2].key != node2->subitems[1].subitems[1].subitems[2].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[3].key != node2->subitems[1].subitems[1].subitems[3].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[4].key != node2->subitems[1].subitems[1].subitems[4].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[1].subitems[5].key != node2->subitems[1].subitems[1].subitems[5].key, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[4].value.string != node2->subitems[1].subitems[4].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[6].value.string != node2->subitems[1].subitems[6].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[7].subitems[0].value.string != node2->subitems[1].subitems[7].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[8].subitems[0].value.string != node2->subitems[1].subitems[8].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[9].subitems[0].value.string != node2->subitems[1].subitems[9].subitems[0].value.string, "Pointers expected to differ, but didn't");
	TestCase_assert(node1->subitems[1].subitems[10].subitems[0].value.string != node2->subitems[1].subitems[10].subitems[0].value.string, "Pointers expected to differ, but didn't");
	JSONNode_dispose(node2);
	JSONNode_dispose(node1);
}

TEST_SUITE(JSONIOTest,
           testEscapeJSONString,
           testUnescapeJSONString,
           testJSONNode_subitemForKey,
           testJSONNode_copy)
