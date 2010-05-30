#include "unittest/framework/TestSuite.h"
#include "jsonio/JSONParser.h"
#include "utilities/IOUtilities.h"
#include <limits.h>
#include <math.h>
#include <unistd.h>

#define loadStringFromTemporaryFile(outNode, string, length, outError) { \
	const char * tempFilePath; \
	int fd; \
	\
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd); \
	writeFileSimple(tempFilePath, string, length); \
	outNode = JSONParser_loadFile(tempFilePath, outError); \
	close(fd); \
	unlink(tempFilePath); \
}

#define loadConstantStringFromTemporaryFile(outNode, string, outError) loadStringFromTemporaryFile(outNode, string, strlen(string), outError);

#define stringAndLength(str) str, strlen(str)

static void testTopLevelNode() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("[]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 0, "Expected 0 but got %zu", node->value.count);
	TestCase_assert(node->subitems == NULL, "Expected NULL but got %p", node->subitems);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("{}"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 0, "Expected 0 but got %zu", node->value.count);
	TestCase_assert(node->subitems == NULL, "Expected NULL but got %p", node->subitems);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 0, "Expected 0 but got %zu", node->value.count);
	TestCase_assert(node->subitems == NULL, "Expected NULL but got %p", node->subitems);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "{}", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 0, "Expected 0 but got %zu", node->value.count);
	TestCase_assert(node->subitems == NULL, "Expected NULL but got %p", node->subitems);
	JSONNode_dispose(node);
}

static void testNumberValues() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("[0]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.number == 0, "Expected 0 but got %f", node->subitems[0].value.number);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[-2.753]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(fabs(node->subitems[0].value.number - -2.753) < 0.00001, "Expected -2.753 but got %f", node->subitems[0].value.number);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[1e+2]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(fabs(node->subitems[0].value.number - 100) < 0.00001, "Expected 100 but got %f", node->subitems[0].value.number);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[0.2e-1]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(fabs(node->subitems[0].value.number - 0.02) < 0.00001, "Expected 0.02 but got %f", node->subitems[0].value.number);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[0]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.number == 0, "Expected 0 but got %f", node->subitems[0].value.number);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[-2.753]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(fabs(node->subitems[0].value.number - -2.753) < 0.00001, "Expected -2.753 but got %f", node->subitems[0].value.number);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[1e+2]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(fabs(node->subitems[0].value.number - 100) < 0.00001, "Expected 100 but got %f", node->subitems[0].value.number);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[0.2e-1]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(fabs(node->subitems[0].value.number - 0.02) < 0.00001, "Expected 0.02 but got %f", node->subitems[0].value.number);
	JSONNode_dispose(node);
}

static void testStringValues() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("[\"\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 0, "Expected 0 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, ""), "Expected \"\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[\"hello\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 5, "Expected 5 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "hello"), "Expected \"hello\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[\"\"]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 0, "Expected 0 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, ""), "Expected \"\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[\"hello\"]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 5, "Expected 5 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "hello"), "Expected \"hello\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
}

static void testBooleanValues() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("[false]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == false, "Expected false but got true");
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[true]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == true, "Expected true but got false");
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[false]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == false, "Expected false but got true");
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[true]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == true, "Expected true but got false");
	JSONNode_dispose(node);
}

static void testNullValue() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("[null]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[null]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	JSONNode_dispose(node);
}

static void testArrays() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("[false, true]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[1].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.boolean == true, "Expected true but got false");
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[true, 3, null]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 3, "Expected 3 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == true, "Expected true but got false");
	TestCase_assert(node->subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[2].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].key);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[false, true]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[1].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.boolean == true, "Expected true but got false");
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[true, 3, null]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 3, "Expected 3 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == true, "Expected true but got false");
	TestCase_assert(node->subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[2].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[2].key);
	JSONNode_dispose(node);
}

static void testObjects() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("{\"a\": false, \"b\": true}"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].keyLength == 1, "Expected 1 but got %zu", node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "a"), "Expected \"a\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[1].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].keyLength == 1, "Expected 1 but got %zu", node->subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].key, "b"), "Expected \"b\" but got \"%s\"", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.boolean == true, "Expected true but got false");
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("{\"foo\": 1, \"bar\": null, \"baz\": \"test\"}"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 3, "Expected 3 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].keyLength == 3, "Expected 3 but got %zu", node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "foo"), "Expected \"foo\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.number == 1, "Expected 1 but got %f", node->subitems[0].value.number);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].keyLength == 3, "Expected 3 but got %zu", node->subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].key, "bar"), "Expected \"bar\" but got \"%s\"", node->subitems[1].key);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[2].keyLength == 3, "Expected 3 but got %zu", node->subitems[2].keyLength);
	TestCase_assert(!strcmp(node->subitems[2].key, "baz"), "Expected \"baz\" but got \"%s\"", node->subitems[2].key);
	TestCase_assert(node->subitems[2].stringLength == 4, "Expected 4 but got %zu", node->subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].value.string, "test"), "Expected \"test\" but got \"%s\"", node->subitems[2].value.string);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "{\"a\": false, \"b\": true}", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].keyLength == 1, "Expected 1 but got %zu", node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "a"), "Expected \"a\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[1].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].keyLength == 1, "Expected 1 but got %zu", node->subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].key, "b"), "Expected \"b\" but got \"%s\"", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.boolean == true, "Expected true but got false");
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "{\"foo\": 1, \"bar\": null, \"baz\": \"test\"}", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 3, "Expected 3 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].keyLength == 3, "Expected 3 but got %zu", node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "foo"), "Expected \"foo\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.number == 1, "Expected 1 but got %f", node->subitems[0].value.number);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].keyLength == 3, "Expected 3 but got %zu", node->subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].key, "bar"), "Expected \"bar\" but got \"%s\"", node->subitems[1].key);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[2].keyLength == 3, "Expected 3 but got %zu", node->subitems[2].keyLength);
	TestCase_assert(!strcmp(node->subitems[2].key, "baz"), "Expected \"baz\" but got \"%s\"", node->subitems[2].key);
	TestCase_assert(node->subitems[2].stringLength == 4, "Expected 4 but got %zu", node->subitems[2].stringLength);
	TestCase_assert(!strcmp(node->subitems[2].value.string, "test"), "Expected \"test\" but got \"%s\"", node->subitems[2].value.string);
	JSONNode_dispose(node);
}

static void testNestedArraysAndObjects() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("[{}, []]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 0, "Expected 0 but got %zu", node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems == NULL, "Expected NULL but got %p", node->subitems[0].subitems);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 0, "Expected 0 but got %zu", node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems == NULL, "Expected NULL but got %p", node->subitems[1].subitems);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("{\"1\": [false, 2, {}], \"2\": {\"\": []}, \"3\": null}"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 3, "Expected 3 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].keyLength == 1, "Expected 1 but got %zu", node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "1"), "Expected \"1\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 3, "Expected 3 but got %zu", node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].subitems[0].type);
	TestCase_assert(node->subitems[0].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[0].key);
	TestCase_assert(node->subitems[0].subitems[0].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[0].subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].subitems[1].type);
	TestCase_assert(node->subitems[0].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[1].key);
	TestCase_assert(node->subitems[0].subitems[1].value.number == 2, "Expected 2 but got %f", node->subitems[0].subitems[1].value.number);
	TestCase_assert(node->subitems[0].subitems[2].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].subitems[2].type);
	TestCase_assert(node->subitems[0].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[2].key);
	TestCase_assert(node->subitems[0].subitems[2].value.count == 0, "Expected 0 but got %zu", node->subitems[0].subitems[2].value.count);
	TestCase_assert(node->subitems[0].subitems[2].subitems == NULL, "Expected NULL but got %p", node->subitems[0].subitems[2].subitems);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].keyLength == 1, "Expected 1 but got %zu", node->subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].key, "2"), "Expected \"2\" but got \"%s\"", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 1, "Expected 1 but got %zu", node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].keyLength == 0, "Expected 0 but got %zu", node->subitems[1].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[0].key, ""), "Expected \"\" but got \"%s\"", node->subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[0].value.count == 0, "Expected 0 but got %zu", node->subitems[1].subitems[0].value.count);
	TestCase_assert(node->subitems[1].subitems[0].subitems == NULL, "Expected NULL but got %p", node->subitems[1].subitems[0].subitems);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[2].keyLength == 1, "Expected 1 but got %zu", node->subitems[2].keyLength);
	TestCase_assert(!strcmp(node->subitems[2].key, "3"), "Expected \"3\" but got \"%s\"", node->subitems[2].key);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[{}, []]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 0, "Expected 0 but got %zu", node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems == NULL, "Expected NULL but got %p", node->subitems[0].subitems);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 0, "Expected 0 but got %zu", node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems == NULL, "Expected NULL but got %p", node->subitems[1].subitems);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "{\"1\": [false, 2, {}], \"2\": {\"\": []}, \"3\": null}", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 3, "Expected 3 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].keyLength == 1, "Expected 1 but got %zu", node->subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[0].key, "1"), "Expected \"1\" but got \"%s\"", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.count == 3, "Expected 3 but got %zu", node->subitems[0].value.count);
	TestCase_assert(node->subitems[0].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].subitems[0].type);
	TestCase_assert(node->subitems[0].subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[0].key);
	TestCase_assert(node->subitems[0].subitems[0].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[0].subitems[1].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[0].subitems[1].type);
	TestCase_assert(node->subitems[0].subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[1].key);
	TestCase_assert(node->subitems[0].subitems[1].value.number == 2, "Expected 2 but got %f", node->subitems[0].subitems[1].value.number);
	TestCase_assert(node->subitems[0].subitems[2].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[0].subitems[2].type);
	TestCase_assert(node->subitems[0].subitems[2].key == NULL, "Expected NULL but got %p", node->subitems[0].subitems[2].key);
	TestCase_assert(node->subitems[0].subitems[2].value.count == 0, "Expected 0 but got %zu", node->subitems[0].subitems[2].value.count);
	TestCase_assert(node->subitems[0].subitems[2].subitems == NULL, "Expected NULL but got %p", node->subitems[0].subitems[2].subitems);
	TestCase_assert(node->subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].keyLength == 1, "Expected 1 but got %zu", node->subitems[1].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].key, "2"), "Expected \"2\" but got \"%s\"", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 1, "Expected 1 but got %zu", node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].keyLength == 0, "Expected 0 but got %zu", node->subitems[1].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[0].key, ""), "Expected \"\" but got \"%s\"", node->subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[0].value.count == 0, "Expected 0 but got %zu", node->subitems[1].subitems[0].value.count);
	TestCase_assert(node->subitems[1].subitems[0].subitems == NULL, "Expected NULL but got %p", node->subitems[1].subitems[0].subitems);
	TestCase_assert(node->subitems[2].type == JSON_TYPE_NULL, "Expected %d but got %d", JSON_TYPE_NULL, node->subitems[2].type);
	TestCase_assert(node->subitems[2].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[2].keyLength == 1, "Expected 1 but got %zu", node->subitems[2].keyLength);
	TestCase_assert(!strcmp(node->subitems[2].key, "3"), "Expected \"3\" but got \"%s\"", node->subitems[2].key);
	JSONNode_dispose(node);
}

static void testStringEscapes() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("[\"\\\\\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 1, "Expected 1 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "\\"), "Expected \"\\\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[\"\\\"\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 1, "Expected 1 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "\""), "Expected \"\"\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[\"\\b\\n\\f\\r\\t\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 5, "Expected 5 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "\b\n\f\r\t"), "Expected \"\b\n\f\r\t\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[\"\\u0020\\u26A0\\uD834\\udd1e\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 8, "Expected 8 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!memcmp(node->subitems[0].value.string, "\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E", 9), "Expected \"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[\"a\\u0000b\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 3, "Expected 3 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!memcmp(node->subitems[0].value.string, "a\0b", 4), "Expected \"a\\0b\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[\"\\/\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 1, "Expected 1 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "/"), "Expected \"/\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[\"\\\\\"]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 1, "Expected 1 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "\\"), "Expected \"\\\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[\"\\\"\"]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 1, "Expected 1 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "\""), "Expected \"\"\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[\"\\b\\n\\f\\r\\t\"]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 5, "Expected 5 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "\b\n\f\r\t"), "Expected \"\b\n\f\r\t\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[\"\\u0020\\u26A0\\uD834\\udd1e\"]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 8, "Expected 8 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!memcmp(node->subitems[0].value.string, "\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E", 9), "Expected \"\x20\xE2\x9A\xA0\xF0\x9D\x84\x9E\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[\"a\\u0000b\"]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 3, "Expected 3 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!memcmp(node->subitems[0].value.string, "a\0b", 4), "Expected \"a\\0b\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, "[\"\\/\"]", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 1, "Expected 1 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_STRING, "Expected %d but got %d", JSON_TYPE_STRING, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].stringLength == 1, "Expected 1 but got %zu", node->subitems[0].stringLength);
	TestCase_assert(!strcmp(node->subitems[0].value.string, "/"), "Expected \"/\" but got \"%s\"", node->subitems[0].value.string);
	JSONNode_dispose(node);
}

static void testInvalidSyntax() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength(""), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("null"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("["), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("{"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[1,]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("{1: 2}"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("{\"a\"}"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("{\"a\": }"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("{\"a\": null, }"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[\"a\": null]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[\"]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("{\": 0}"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("{\"\": \"}"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[a]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("{1, 2}"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[\"\\u012\"]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[\"\n\"]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[0xFF]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[1.]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[.1]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[01]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[-.1]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[-01]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[1.1.1]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("[]a"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString(stringAndLength("{}[]"), NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
}

static void testThatLengthIsRespected() {
	struct JSONNode * node;
	
	node = JSONParser_loadString("[]", 1, NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	
	node = JSONParser_loadString("[]", 2, NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	JSONNode_dispose(node);
}

static void testLoadNonexistentFile() {
	struct JSONNode * node;
	
	node = JSONParser_loadFile("If this file exists, unit tests will fail", NULL);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
}

static void testWhitespaceTolerance() {
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength(" \r\n\t[ \r\n\tfalse \r\n\t, \r\n\t{ \r\n\t\"\" \r\n\t: \r\n\t0 \r\n\t} \r\n\t] \r\n\t"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 1, "Expected 1 but got %zu", node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].keyLength == 0, "Expected 0 but got %zu", node->subitems[1].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[0].key, ""), "Expected \"\" but got \"%s\"", node->subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[0].value.number == 0, "Expected 0 but got %f", node->subitems[1].subitems[0].value.number);
	JSONNode_dispose(node);
	
	loadConstantStringFromTemporaryFile(node, " \r\n\t[ \r\n\tfalse \r\n\t, \r\n\t{ \r\n\t\"\" \r\n\t: \r\n\t0 \r\n\t} \r\n\t] \r\n\t", NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->type == JSON_TYPE_ARRAY, "Expected %d but got %d", JSON_TYPE_ARRAY, node->type);
	TestCase_assert(node->key == NULL, "Expected NULL but got \"%s\"", node->key);
	TestCase_assert(node->value.count == 2, "Expected 2 but got %zu", node->value.count);
	TestCase_assert(node->subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[0].type == JSON_TYPE_BOOLEAN, "Expected %d but got %d", JSON_TYPE_BOOLEAN, node->subitems[0].type);
	TestCase_assert(node->subitems[0].key == NULL, "Expected NULL but got %p", node->subitems[0].key);
	TestCase_assert(node->subitems[0].value.boolean == false, "Expected false but got true");
	TestCase_assert(node->subitems[1].type == JSON_TYPE_OBJECT, "Expected %d but got %d", JSON_TYPE_OBJECT, node->subitems[1].type);
	TestCase_assert(node->subitems[1].key == NULL, "Expected NULL but got %p", node->subitems[1].key);
	TestCase_assert(node->subitems[1].value.count == 1, "Expected 1 but got %zu", node->subitems[1].value.count);
	TestCase_assert(node->subitems[1].subitems != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].type == JSON_TYPE_NUMBER, "Expected %d but got %d", JSON_TYPE_NUMBER, node->subitems[1].subitems[0].type);
	TestCase_assert(node->subitems[1].subitems[0].key != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(node->subitems[1].subitems[0].keyLength == 0, "Expected 0 but got %zu", node->subitems[1].subitems[0].keyLength);
	TestCase_assert(!strcmp(node->subitems[1].subitems[0].key, ""), "Expected \"\" but got \"%s\"", node->subitems[1].subitems[0].key);
	TestCase_assert(node->subitems[1].subitems[0].value.number == 0, "Expected 0 but got %f", node->subitems[1].subitems[0].value.number);
	JSONNode_dispose(node);
}

static void testErrorReporting() {
	struct JSONNode * node;
	struct JSONParseError error, errorPrototype = {SIZE_T_MAX, -1, NULL};
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength(""), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 0, "Expected 0 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_rootNodeNotFound, "Expected %d but got %d", JSONParseError_rootNodeNotFound, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("  null"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 2, "Expected 2 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_rootNodeNotFound, "Expected %d but got %d", JSONParseError_rootNodeNotFound, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("{1}"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 1, "Expected 1 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_objectKeyNotFound, "Expected %d but got %d", JSONParseError_objectKeyNotFound, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("{\"\": 0, 1}"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 8, "Expected 8 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_objectKeyNotFound, "Expected %d but got %d", JSONParseError_objectKeyNotFound, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("[\"\\u\"]"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 1, "Expected 1 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_malformedString, "Expected %d but got %d", JSONParseError_malformedString, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("{\"foo\": 0, \"hello \\uD800\": 1}"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 11, "Expected 11 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_malformedString, "Expected %d but got %d", JSONParseError_malformedString, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("{\"bar\"}"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 6, "Expected 6 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_keyNotFollowedByColon, "Expected %d but got %d", JSONParseError_keyNotFollowedByColon, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("{\"\": 0, \"\"}"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 10, "Expected 10 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_keyNotFollowedByColon, "Expected %d but got %d", JSONParseError_keyNotFollowedByColon, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("{"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 1, "Expected 1 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_unexpectedEOF, "Expected %d but got %d", JSONParseError_unexpectedEOF, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("["), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 1, "Expected 1 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_unexpectedEOF, "Expected %d but got %d", JSONParseError_unexpectedEOF, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("[1, 2"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 5, "Expected 5 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_unexpectedEOF, "Expected %d but got %d", JSONParseError_unexpectedEOF, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("[a]"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 1, "Expected 1 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_unexpectedToken, "Expected %d but got %d", JSONParseError_unexpectedToken, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
	
	error = errorPrototype;
	node = JSONParser_loadString(stringAndLength("[]a"), &error);
	TestCase_assert(node == NULL, "Expected NULL but got %p", node);
	TestCase_assert(error.charIndex == 2, "Expected 2 but got %zu", error.charIndex);
	TestCase_assert(error.code == JSONParseError_unexpectedToken, "Expected %d but got %d", JSONParseError_unexpectedToken, error.code);
	TestCase_assert(error.description != NULL, "Expected non-NULL but got NULL");
}

TEST_SUITE(JSONParserTest,
           testTopLevelNode,
           testNumberValues,
           testStringValues,
           testBooleanValues,
           testNullValue,
           testArrays,
           testObjects,
           testNestedArraysAndObjects,
           testStringEscapes,
           testInvalidSyntax,
           testThatLengthIsRespected,
           testLoadNonexistentFile,
           testWhitespaceTolerance,
           testErrorReporting)
