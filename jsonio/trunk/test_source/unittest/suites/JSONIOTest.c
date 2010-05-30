#include "jsonio/JSONIO.h"
#include "unittest/framework/TestSuite.h"

#define stringAndLength(str) str, strlen(str)

static void testEscapeJSONString() {
	char * string;
	size_t length;
	
	string = escapeJSONString(stringAndLength("\\"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 2, "Expected 2 but got %zu", length);
	TestCase_assert(!strcmp(string, "\\\\"), "Expected \"\\\\\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString(stringAndLength("\""), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 2, "Expected 2 but got %zu", length);
	TestCase_assert(!strcmp(string, "\\\""), "Expected \"\\\"\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString(stringAndLength("\b\n\f\r\t"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 9, "Expected 9 but got %zu", length);
	TestCase_assert(!strcmp(string, "\\b\\n\\f\\r\t"), "Expected \"\\b\\n\\f\\r\t\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString(stringAndLength("\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 8, "Expected 8 but got %zu", length);
	TestCase_assert(!memcmp(string, "\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E", 9), "Expected \"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString("a\0b", 3, &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 8, "Expected 8 but got %zu", length);
	TestCase_assert(!strcmp(string, "a\\u0000b"), "Expected \"a\\u0000b\" but got \"%s\"", string);
	free(string);
	
	string = escapeJSONString("/", 1, &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 1, "Expected 1 but got %zu", length);
	TestCase_assert(!strcmp(string, "/"), "Expected \"/\" but got \"%s\"", string);
	free(string);
}

static void testUnescapeJSONString() {
	char * string;
	size_t length;
	
	string = unescapeJSONString(stringAndLength("\\\\"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 1, "Expected 1 but got %zu", length);
	TestCase_assert(!strcmp(string, "\\"), "Expected \"\\\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("\\\""), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 1, "Expected 1 but got %zu", length);
	TestCase_assert(!strcmp(string, "\""), "Expected \"\"\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("\\b\\n\\f\\r\\t"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 5, "Expected 5 but got %zu", length);
	TestCase_assert(!strcmp(string, "\b\n\f\r\t"), "Expected \"\b\n\f\r\t\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("\\u0020\\u26A0\\uD834\\udd1e"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 8, "Expected 8 but got %zu", length);
	TestCase_assert(!memcmp(string, "\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E", 9), "Expected \"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("a\\u0000b"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 3, "Expected 3 but got %zu", length);
	TestCase_assert(!memcmp(string, "a\0b", 4), "Expected \"a\\0b\" but got \"%s\"", string);
	free(string);
	
	string = unescapeJSONString(stringAndLength("\\/"), &length);
	TestCase_assert(string != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 1, "Expected 1 but got %zu", length);
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
	TestCase_assert(subitemIndex == 0, "Expected 0 but got %zu", subitemIndex);
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("bar"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected %zu but got %zu", JSON_SUBITEM_NOT_FOUND, subitemIndex);
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
	TestCase_assert(subitemIndex == 1, "Expected 1 but got %zu", subitemIndex);
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("f"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected %zu but got %zu", JSON_SUBITEM_NOT_FOUND, subitemIndex);
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("bar"));
	TestCase_assert(subitemIndex == 0, "Expected 0 but got %zu", subitemIndex);
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("foo"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected %zu but got %zu", JSON_SUBITEM_NOT_FOUND, subitemIndex);
	free(node.subitems);
	
	node.type = JSON_TYPE_ARRAY;
	node.value.count = 1;
	node.subitems = malloc(sizeof(struct JSONNode));
	node.subitems[0].type = JSON_TYPE_NULL;
	
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("foo"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected %zu but got %zu", JSON_SUBITEM_NOT_FOUND, subitemIndex);
	free(node.subitems);
	
	node.type = JSON_TYPE_NULL;
	node.subitems = NULL;
	
	subitemIndex = JSONNode_subitemIndexForKey(&node, stringAndLength("foo"));
	TestCase_assert(subitemIndex == JSON_SUBITEM_NOT_FOUND, "Expected %zu but got %zu", JSON_SUBITEM_NOT_FOUND, subitemIndex);
}

TEST_SUITE(JSONIOTest, testEscapeJSONString, testUnescapeJSONString, testJSONNode_subitemForKey)
