#include "jsonio/JSONParser.h"
#include "jsonserialization/JSONDeserializationContext.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"
#include "utilities/IOUtilities.h"
#include <string.h>
#include <float.h>
#include <unistd.h>

#define stringAndLength(str) str, strlen(str)

static void verifyInit(JSONDeserializationContext * context) {
	TestCase_assert(context->jmpBuf == NULL, "Expected NULL but got %p", context->jmpBuf);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	TestCase_assert(context->dispose == JSONDeserializationContext_dispose, "Expected %p but got %p", JSONDeserializationContext_dispose, context->dispose);
	TestCase_assert(context->errorString == JSONSerialization_errorString, "Expected %p but got %p", JSONSerialization_errorString, context->errorString);
	TestCase_assert(context->beginStructure == JSONDeserializationContext_beginStructure, "Expected %p but got %p", JSONDeserializationContext_beginStructure, context->beginStructure);
	TestCase_assert(context->beginDictionary == JSONDeserializationContext_beginDictionary, "Expected %p but got %p", JSONDeserializationContext_beginDictionary, context->beginDictionary);
	TestCase_assert(context->beginArray == JSONDeserializationContext_beginArray, "Expected %p but got %p", JSONDeserializationContext_beginArray, context->beginArray);
	TestCase_assert(context->endStructure == JSONDeserializationContext_endStructure, "Expected %p but got %p", JSONDeserializationContext_endStructure, context->endStructure);
	TestCase_assert(context->endDictionary == JSONDeserializationContext_endDictionary, "Expected %p but got %p", JSONDeserializationContext_endDictionary, context->endDictionary);
	TestCase_assert(context->endArray == JSONDeserializationContext_endArray, "Expected %p but got %p", JSONDeserializationContext_endArray, context->endArray);
	TestCase_assert(context->readBoolean == JSONDeserializationContext_readBoolean, "Expected %p but got %p", JSONDeserializationContext_readBoolean, context->readBoolean);
	TestCase_assert(context->readInt8 == JSONDeserializationContext_readInt8, "Expected %p but got %p", JSONDeserializationContext_readInt8, context->readInt8);
	TestCase_assert(context->readUInt8 == JSONDeserializationContext_readUInt8, "Expected %p but got %p", JSONDeserializationContext_readUInt8, context->readUInt8);
	TestCase_assert(context->readInt16 == JSONDeserializationContext_readInt16, "Expected %p but got %p", JSONDeserializationContext_readInt16, context->readInt16);
	TestCase_assert(context->readUInt16 == JSONDeserializationContext_readUInt16, "Expected %p but got %p", JSONDeserializationContext_readUInt16, context->readUInt16);
	TestCase_assert(context->readInt32 == JSONDeserializationContext_readInt32, "Expected %p but got %p", JSONDeserializationContext_readInt32, context->readInt32);
	TestCase_assert(context->readUInt32 == JSONDeserializationContext_readUInt32, "Expected %p but got %p", JSONDeserializationContext_readUInt32, context->readUInt32);
	TestCase_assert(context->readInt64 == JSONDeserializationContext_readInt64, "Expected %p but got %p", JSONDeserializationContext_readInt64, context->readInt64);
	TestCase_assert(context->readUInt64 == JSONDeserializationContext_readUInt64, "Expected %p but got %p", JSONDeserializationContext_readUInt64, context->readUInt64);
	TestCase_assert(context->readFloat == JSONDeserializationContext_readFloat, "Expected %p but got %p", JSONDeserializationContext_readFloat, context->readFloat);
	TestCase_assert(context->readDouble == JSONDeserializationContext_readDouble, "Expected %p but got %p", JSONDeserializationContext_readDouble, context->readDouble);
	TestCase_assert(context->readEnumeration == JSONDeserializationContext_readEnumeration, "Expected %p but got %p", JSONDeserializationContext_readEnumeration, context->readEnumeration);
	TestCase_assert(context->readBitfield8 == JSONDeserializationContext_readBitfield8, "Expected %p but got %p", JSONDeserializationContext_readBitfield8, context->readBitfield8);
	TestCase_assert(context->readBitfield16 == JSONDeserializationContext_readBitfield16, "Expected %p but got %p", JSONDeserializationContext_readBitfield16, context->readBitfield16);
	TestCase_assert(context->readBitfield32 == JSONDeserializationContext_readBitfield32, "Expected %p but got %p", JSONDeserializationContext_readBitfield32, context->readBitfield32);
	TestCase_assert(context->readBitfield64 == JSONDeserializationContext_readBitfield64, "Expected %p but got %p", JSONDeserializationContext_readBitfield64, context->readBitfield64);
	TestCase_assert(context->readString == JSONDeserializationContext_readString, "Expected %p but got %p", JSONDeserializationContext_readString, context->readString);
	TestCase_assert(context->readBlob == JSONDeserializationContext_readBlob, "Expected %p but got %p", JSONDeserializationContext_readBlob, context->readBlob);
	TestCase_assert(context->readNextDictionaryKey == JSONDeserializationContext_readNextDictionaryKey, "Expected %p but got %p", JSONDeserializationContext_readNextDictionaryKey, context->readNextDictionaryKey);
	TestCase_assert(context->hasDictionaryKey == JSONDeserializationContext_hasDictionaryKey, "Expected %p but got %p", JSONDeserializationContext_hasDictionaryKey, context->hasDictionaryKey);
}

static void testInit() {
	JSONDeserializationContext context, * contextPtr;
	struct JSONNode node;
	const char * tempFilePath;
	int fd;
	
	node.type = JSON_TYPE_ARRAY;
	node.key = NULL;
	node.value.count = 0;
	node.subitems = NULL;
	
	memset(&context, 0, sizeof(context));
	JSONDeserializationContext_initWithString(&context, stringAndLength("[]"));
	verifyInit(&context);
	context.dispose(&context);
	
	memset(&context, 0, sizeof(context));
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	writeFileSimple(tempFilePath, stringAndLength("[]"));
	JSONDeserializationContext_initWithFile(&context, tempFilePath);
	close(fd);
	unlink(tempFilePath);
	verifyInit(&context);
	context.dispose(&context);
	
	memset(&context, 0, sizeof(context));
	JSONDeserializationContext_initWithJSONNode(&context, &node);
	verifyInit(&context);
	context.dispose(&context);
	
	contextPtr = JSONDeserializationContext_createWithString(stringAndLength("[]"));
	TestCase_assert(contextPtr != NULL, "Expected non-NULL but got NULL");
	if (contextPtr == NULL) {return;} // Suppress clang warning
	verifyInit(contextPtr);
	contextPtr->dispose(contextPtr);
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	writeFileSimple(tempFilePath, stringAndLength("[]"));
	contextPtr = JSONDeserializationContext_createWithFile(tempFilePath);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(contextPtr != NULL, "Expected non-NULL but got NULL");
	if (contextPtr == NULL) {return;} // Suppress clang warning
	verifyInit(contextPtr);
	contextPtr->dispose(contextPtr);
	
	contextPtr = JSONDeserializationContext_createWithJSONNode(&node);
	TestCase_assert(contextPtr != NULL, "Expected non-NULL but got NULL");
	if (contextPtr == NULL) {return;} // Suppress clang warning
	verifyInit(contextPtr);
	contextPtr->dispose(contextPtr);
}

#define beginAndVerifyContainer(CONTAINER_TYPE, KEY, EXPECTED_SIZE) { \
	size_t size; \
	size = context->begin##CONTAINER_TYPE(context, KEY); \
	TestCase_assert(size == (EXPECTED_SIZE), "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, (size_t) (EXPECTED_SIZE), size); \
}
#define beginAndVerifyArray(KEY, EXPECTED_SIZE) beginAndVerifyContainer(Array, KEY, EXPECTED_SIZE)
#define beginAndVerifyDictionary(KEY, EXPECTED_SIZE) beginAndVerifyContainer(Dictionary, KEY, EXPECTED_SIZE)

#define readFunction_int8_t readInt8
#define readFunction_uint8_t readUInt8
#define readFunction_int16_t readInt16
#define readFunction_uint16_t readUInt16
#define readFunction_int32_t readInt32
#define readFunction_uint32_t readUInt32
#define readFunction_int64_t readInt64
#define readFunction_uint64_t readUInt64
#define readFunction_float readFloat
#define readFunction_double readDouble
#define readFunction_bool readBoolean
#define printfSpecifier_int8_t "%d"
#define printfSpecifier_uint8_t "%u"
#define printfSpecifier_int16_t "%d"
#define printfSpecifier_uint16_t "%u"
#define printfSpecifier_int32_t "%d"
#define printfSpecifier_uint32_t "%u"
#define printfSpecifier_int64_t INT64_FORMAT
#define printfSpecifier_uint64_t UINT64_FORMAT
#define printfSpecifier_float "%f"
#define printfSpecifier_double "%f"
#define printfSpecifier_bool "%d"
#define readAndVerifyNumber(TYPE, KEY, EXPECTED_VALUE) { \
	TYPE value; \
	value = context->readFunction_##TYPE(context, KEY); \
	TestCase_assert(value == (EXPECTED_VALUE), "Expected " printfSpecifier_##TYPE " but got " printfSpecifier_##TYPE, (TYPE) (EXPECTED_VALUE), value); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define readAndVerifyString(KEY, EXPECTED_VALUE) { \
	const char * value; \
	value = context->readString(context, KEY); \
	TestCase_assert(!strcmp(value, (EXPECTED_VALUE)), "Expected \"%s\" but got \"%s\"", (EXPECTED_VALUE), value); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define readAndVerifyBlob(KEY, EXPECTED_VALUE, EXPECTED_LENGTH) { \
	const void * value; \
	size_t length; \
	value = context->readBlob(context, KEY, &length); \
	TestCase_assert(length == EXPECTED_LENGTH, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT, (size_t) EXPECTED_LENGTH, length); \
	TestCase_assert(!memcmp(value, (EXPECTED_VALUE), EXPECTED_LENGTH), "Expected \"%s\" but got \"%s\"", (char *) (EXPECTED_VALUE), (char *) value); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define readAndVerifyEnumeration(KEY, EXPECTED_VALUE, ...) { \
	int value; \
	value = context->readEnumeration(context, KEY, __VA_ARGS__); \
	TestCase_assert(value == (EXPECTED_VALUE), "Expected %d but got %d", (EXPECTED_VALUE), value); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define printfSpecifier_bitfield8 "0x%02X"
#define printfSpecifier_bitfield16 "0x%04X"
#define printfSpecifier_bitfield32 "0x%08X"
#if defined(WIN32)
#define printfSpecifier_bitfield64 "0x%016I64X"
#elif defined(linux) && defined(_LP64)
#define printfSpecifier_bitfield64 "0x%016lX"
#else
#define printfSpecifier_bitfield64 "0x%016llX"
#endif
#define readAndVerifyBitfield(NBITS, KEY, EXPECTED_VALUE, ...) { \
	uint##NBITS##_t value; \
	value = context->readBitfield##NBITS(context, KEY, __VA_ARGS__); \
	TestCase_assert(value == (EXPECTED_VALUE), "Expected " printfSpecifier_bitfield##NBITS " but got " printfSpecifier_bitfield##NBITS, (EXPECTED_VALUE), value); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define verifyReadNextDictionaryKey(KEY) \
	key = context->readNextDictionaryKey(context); \
	TestCase_assert(key != NULL, "Expected non-NULL but got NULL"); \
	TestCase_assert(!strcmp(key, (KEY)), "Expected \"%s\" but got \"%s\"", (KEY), key); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status);

#define verifyHasDictionaryKey(KEY, HAS_KEY) \
	TestCase_assert(context->hasDictionaryKey(context, (KEY)) == (HAS_KEY), "Expected %s but got %s", (HAS_KEY) ? "true" : "false", (HAS_KEY) ? "false" : "true"); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status);

static void testNumberValues() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 10)
	readAndVerifyNumber(int8_t, "", 0)
	readAndVerifyNumber(uint8_t, "", 1)
	readAndVerifyNumber(int16_t, "", 2)
	readAndVerifyNumber(uint16_t, "", 3)
	readAndVerifyNumber(int32_t, "", 4)
	readAndVerifyNumber(uint32_t, "", 5)
	readAndVerifyNumber(int64_t, "", 6)
	readAndVerifyNumber(uint64_t, "", 7)
	readAndVerifyNumber(float, "", 8)
	readAndVerifyNumber(double, "", 9)
	context->endArray(context);
	context->dispose(context);
	
	context = JSONDeserializationContext_createWithString(stringAndLength("[-128, 255, -32768, 65535, -2147483648, 4294967295, -9007199254740992, 9007199254740992, 340282346638528859811704183484516925440, 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 10)
	readAndVerifyNumber(int8_t, "", INT8_MIN)
	readAndVerifyNumber(uint8_t, "", UINT8_MAX)
	readAndVerifyNumber(int16_t, "", INT16_MIN)
	readAndVerifyNumber(uint16_t, "", UINT16_MAX)
	readAndVerifyNumber(int32_t, "", INT32_MIN)
	readAndVerifyNumber(uint32_t, "", UINT32_MAX)
	readAndVerifyNumber(int64_t, "", -(1ll << DBL_MANT_DIG))
	readAndVerifyNumber(uint64_t, "", 1ull << DBL_MANT_DIG)
	readAndVerifyNumber(float, "", FLT_MAX)
	readAndVerifyNumber(double, "", DBL_MAX)
	context->endArray(context);
	context->dispose(context);
}

static void testLargeIntegerRepresentations() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("[\"-9007199254740993\", \"9007199254740993\", -9007199254740992, 9007199254740992, -9223372036854775808, \"-9223372036854775807\", \"18446744073709551615\"]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 7)
	readAndVerifyNumber(int64_t, "", -((1ll << DBL_MANT_DIG) + 1))
	readAndVerifyNumber(uint64_t, "", (1ull << DBL_MANT_DIG) + 1)
	readAndVerifyNumber(int64_t, "", -(1ll << DBL_MANT_DIG))
	readAndVerifyNumber(uint64_t, "", 1ull << DBL_MANT_DIG)
	
	// INT64_MIN is exactly representable as a double, so it must be serialized as a number
	readAndVerifyNumber(int64_t, "", INT64_MIN)
	// INT64_MIN + 1 is not representable as a double, so it must be serialized as a string
	readAndVerifyNumber(int64_t, "", INT64_MIN + 1)
	
	readAndVerifyNumber(uint64_t, "", UINT64_MAX)
	context->endArray(context);
	context->dispose(context);
}

static void testStringValues() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("[\"foo\", \"Hello, world!\"]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 2)
	readAndVerifyString("", "foo")
	readAndVerifyString("", "Hello, world!")
	context->endArray(context);
	context->dispose(context);
}

static void testBlobValues() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("[\"Zm9v\", \"SGVsbG8sIHdvcmxkIQ==\"]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 2)
	readAndVerifyBlob("", "foo", 3)
	readAndVerifyBlob("", "Hello, world!", 13)
	context->endArray(context);
	context->dispose(context);
}

static void testBooleanValues() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("[false, true]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 2)
	readAndVerifyNumber(bool, "", false)
	readAndVerifyNumber(bool, "", true)
	context->endArray(context);
	context->dispose(context);
}

#define ENUM_TEST_0 0
#define ENUM_TEST_1 1
#define ENUM_TEST_zero 0
#define ENUM_TEST_one 1
#define ENUM_TEST_two 2

static void testEnumerations() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("[\"ENUM_TEST_0\", \"ENUM_TEST_1\", \"ENUM_TEST_zero\", \"ENUM_TEST_one\", \"ENUM_TEST_two\"]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 5)
	readAndVerifyEnumeration("", ENUM_TEST_0, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL)
	readAndVerifyEnumeration("", ENUM_TEST_1, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL)
	readAndVerifyEnumeration("", ENUM_TEST_zero, enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL)
	readAndVerifyEnumeration("", ENUM_TEST_one, enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL)
	readAndVerifyEnumeration("", ENUM_TEST_two, enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL)
	context->endArray(context);
	context->dispose(context);
}

static void testBitfields() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength(
	"["
		"[\"bit_1\", \"bit_3\", \"bit_5\", \"bit_7\"],"
		"[\"bit0\", \"bit1\", \"bit2\", \"bit4\", \"bit6\"],"
		"[\"0\", \"C\", \"D\", \"E\", \"F\"],"
		"[\"b0\", \"b1\", \"b2\", \"b3\"],"
		"[\"0\", \"1C\", \"1D\", \"1E\", \"1F\"],"
		"[\"b1\", \"b2\", \"b3\", \"b4\"],"
		"[\"0\", \"3C\", \"3D\", \"3E\", \"3F\"],"
		"[\"b2\", \"b3\", \"b4\", \"b5\"],"
		"[],"
		"[],"
		"[],"
		"[]"
	"]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 12)
	readAndVerifyBitfield(8, "", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7", NULL);
	readAndVerifyBitfield(8, "", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	readAndVerifyBitfield(16, "", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", NULL);
	readAndVerifyBitfield(16, "", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(32, "", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", NULL);
	readAndVerifyBitfield(32, "", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(64, "", (uint64_t) 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", NULL);
	readAndVerifyBitfield(64, "", (uint64_t) 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(8, "", 0x00, NULL);
	readAndVerifyBitfield(16, "", 0x0000, NULL);
	readAndVerifyBitfield(32, "", 0x00000000, NULL);
	readAndVerifyBitfield(64, "", (uint64_t) 0x0000000000000000ull, NULL);
	context->endArray(context);
	context->dispose(context);
	
	context = JSONDeserializationContext_createWithString(stringAndLength(
	"["
		"[\"bit_7\", \"bit_5\", \"bit_3\", \"bit_1\"],"
		"[\"bit6\", \"bit4\", \"bit2\", \"bit1\", \"bit0\"],"
		"[\"F\", \"E\", \"D\", \"C\", \"0\"],"
		"[\"b3\", \"b2\", \"b1\", \"b0\"],"
		"[\"1F\", \"1E\", \"1D\", \"1C\", \"0\"],"
		"[\"b4\", \"b3\", \"b2\", \"b1\"],"
		"[\"3F\", \"3E\", \"3D\", \"3C\", \"0\"],"
		"[\"b5\", \"b4\", \"b3\", \"b2\"]"
	"]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 8)
	readAndVerifyBitfield(8, "", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7", NULL);
	readAndVerifyBitfield(8, "", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	readAndVerifyBitfield(16, "", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", NULL);
	readAndVerifyBitfield(16, "", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(32, "", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", NULL);
	readAndVerifyBitfield(32, "", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(64, "", (uint64_t) 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", NULL);
	readAndVerifyBitfield(64, "", (uint64_t) 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->endArray(context);
	context->dispose(context);
}

static void testArrays() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("[[-1], [0, 1, [2, 3, 4, 5, 6, 7], 8, 9, \"10\", true, \"enum\", [\"13\"], [\"14\"], [\"15\"], [\"16\"]]]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 2)
		beginAndVerifyArray("", 1)
			readAndVerifyNumber(int8_t, "", -1)
		context->endArray(context);
		beginAndVerifyArray("", 12)
			readAndVerifyNumber(int8_t, "", 0)
			readAndVerifyNumber(uint8_t, "", 1)
			beginAndVerifyArray("", 6)
				readAndVerifyNumber(int16_t, "", 2)
				readAndVerifyNumber(uint16_t, "", 3)
				readAndVerifyNumber(int32_t, "", 4)
				readAndVerifyNumber(uint32_t, "", 5)
				readAndVerifyNumber(int64_t, "", 6)
				readAndVerifyNumber(uint64_t, "", 7)
			context->endArray(context);
			readAndVerifyNumber(float, "", 8)
			readAndVerifyNumber(double, "", 9)
			readAndVerifyString("", "10")
			readAndVerifyNumber(bool, "", true)
			readAndVerifyEnumeration("", 12, "enum", 12, NULL)
			readAndVerifyBitfield(8, "", 0x01, "13", NULL)
			readAndVerifyBitfield(16, "", 0x0001, "14", NULL)
			readAndVerifyBitfield(32, "", 0x00000001, "15", NULL)
			readAndVerifyBitfield(64, "", (uint64_t) 0x0000000000000001ull, "16", NULL)
		context->endArray(context);
	context->endArray(context);
	context->dispose(context);
}

static void testStructures() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("{\"a\": {\"b\": -1}, \"c\": {\"d\": 0, \"e\": 1, \"f\": {\"g\": 2, \"h\": 3, \"i\": 4, \"j\": 5, \"k\": 6, \"l\": 7}, \"m\": 8, \"n\": 9, \"o\": \"10\", \"p\": true, \"q\": \"enum\", \"r\": [\"13\"], \"s\": [\"14\"], \"t\": [\"15\"], \"u\": [\"16\"]}}"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "");
		context->beginStructure(context, "a");
			readAndVerifyNumber(int8_t, "b", -1)
		context->endStructure(context);
		context->beginStructure(context, "c");
			readAndVerifyNumber(int8_t, "d", 0)
			readAndVerifyNumber(uint8_t, "e", 1)
			context->beginStructure(context, "f");
				readAndVerifyNumber(int16_t, "g", 2)
				readAndVerifyNumber(uint16_t, "h", 3)
				readAndVerifyNumber(int32_t, "i", 4)
				readAndVerifyNumber(uint32_t, "j", 5)
				readAndVerifyNumber(int64_t, "k", 6)
				readAndVerifyNumber(uint64_t, "l", 7)
			context->endStructure(context);
			readAndVerifyNumber(float, "m", 8)
			readAndVerifyNumber(double, "n", 9)
			readAndVerifyString("o", "10")
			readAndVerifyNumber(bool, "p", true)
			readAndVerifyEnumeration("q", 12, "enum", 12, NULL)
			readAndVerifyBitfield(8, "r", 0x01, "13", NULL)
			readAndVerifyBitfield(16, "s", 0x0001, "14", NULL)
			readAndVerifyBitfield(32, "t", 0x00000001, "15", NULL)
			readAndVerifyBitfield(64, "u", (uint64_t) 0x0000000000000001ull, "16", NULL)
		context->endStructure(context);
	context->endStructure(context);
	context->dispose(context);
	
	context = JSONDeserializationContext_createWithString(stringAndLength("{\"c\": {\"u\": [\"16\"], \"t\": [\"15\"], \"s\": [\"14\"], \"r\": [\"13\"], \"q\": \"enum\", \"p\": true, \"o\": \"10\", \"n\": 9, \"m\": 8, \"f\": {\"l\": 7, \"k\": 6, \"j\": 5, \"i\": 4, \"h\": 3, \"g\": 2}, \"e\": 1, \"d\": 0}, \"a\": {\"b2\": -10, \"b\": -1}}"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "");
		context->beginStructure(context, "a");
			readAndVerifyNumber(int8_t, "b", -1)
			readAndVerifyNumber(int8_t, "b2", -10)
		context->endStructure(context);
		context->beginStructure(context, "c");
			readAndVerifyNumber(int8_t, "d", 0)
			readAndVerifyNumber(uint8_t, "e", 1)
			context->beginStructure(context, "f");
				readAndVerifyNumber(int16_t, "g", 2)
				readAndVerifyNumber(uint16_t, "h", 3)
				readAndVerifyNumber(int32_t, "i", 4)
				readAndVerifyNumber(uint32_t, "j", 5)
				readAndVerifyNumber(int64_t, "k", 6)
				readAndVerifyNumber(uint64_t, "l", 7)
			context->endStructure(context);
			readAndVerifyNumber(float, "m", 8)
			readAndVerifyNumber(double, "n", 9)
			readAndVerifyString("o", "10")
			readAndVerifyNumber(bool, "p", true)
			readAndVerifyEnumeration("q", 12, "enum", 12, NULL)
			readAndVerifyBitfield(8, "r", 0x01, "13", NULL)
			readAndVerifyBitfield(16, "s", 0x0001, "14", NULL)
			readAndVerifyBitfield(32, "t", 0x00000001, "15", NULL)
			readAndVerifyBitfield(64, "u", (uint64_t) 0x0000000000000001ull, "16", NULL)
		context->endStructure(context);
	context->endStructure(context);
	context->dispose(context);
}

static void testDictionaries() {
	JSONDeserializationContext * context;
	const char * key;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("{\"a\": {\"b\": -1}, \"c\": {\"d\": 0, \"e\": 1, \"f\": {\"g\": 2, \"h\": 3, \"i\": 4, \"j\": 5, \"k\": 6, \"l\": 7}, \"m\": 8, \"n\": 9, \"o\": \"10\", \"p\": true, \"q\": \"enum\", \"r\": [\"13\"], \"s\": [\"14\"], \"t\": [\"15\"], \"u\": [\"16\"]}}"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyDictionary("", 2)
		beginAndVerifyDictionary("a", 1)
			readAndVerifyNumber(int8_t, "b", -1)
		context->endDictionary(context);
		beginAndVerifyDictionary("c", 12)
			readAndVerifyNumber(int8_t, "d", 0)
			readAndVerifyNumber(uint8_t, "e", 1)
			beginAndVerifyDictionary("f", 6)
				readAndVerifyNumber(int16_t, "g", 2)
				readAndVerifyNumber(uint16_t, "h", 3)
				readAndVerifyNumber(int32_t, "i", 4)
				readAndVerifyNumber(uint32_t, "j", 5)
				readAndVerifyNumber(int64_t, "k", 6)
				readAndVerifyNumber(uint64_t, "l", 7)
			context->endDictionary(context);
			readAndVerifyNumber(float, "m", 8)
			readAndVerifyNumber(double, "n", 9)
			readAndVerifyString("o", "10")
			readAndVerifyNumber(bool, "p", true)
			readAndVerifyEnumeration("q", 12, "enum", 12, NULL)
			readAndVerifyBitfield(8, "r", 0x01, "13", NULL)
			readAndVerifyBitfield(16, "s", 0x0001, "14", NULL)
			readAndVerifyBitfield(32, "t", 0x00000001, "15", NULL)
			readAndVerifyBitfield(64, "u", (uint64_t) 0x0000000000000001ull, "16", NULL)
		context->endDictionary(context);
	context->endDictionary(context);
	context->dispose(context);
	
	context = JSONDeserializationContext_createWithString(stringAndLength("{\"c\": {\"u\": [\"16\"], \"t\": [\"15\"], \"s\": [\"14\"], \"r\": [\"13\"], \"q\": \"enum\", \"p\": true, \"o\": \"10\", \"n\": 9, \"m\": 8, \"f\": {\"l\": 7, \"k\": 6, \"j\": 5, \"i\": 4, \"h\": 3, \"g\": 2}, \"e\": 1, \"d\": 0}, \"a\": {\"b2\": -10, \"b\": -1}}"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyDictionary("", 2)
		beginAndVerifyDictionary("a", 2)
			readAndVerifyNumber(int8_t, "b", -1)
			readAndVerifyNumber(int8_t, "b2", -10)
		context->endDictionary(context);
		beginAndVerifyDictionary("c", 12)
			readAndVerifyNumber(int8_t, "d", 0)
			readAndVerifyNumber(uint8_t, "e", 1)
			beginAndVerifyDictionary("f", 6)
				readAndVerifyNumber(int16_t, "g", 2)
				readAndVerifyNumber(uint16_t, "h", 3)
				readAndVerifyNumber(int32_t, "i", 4)
				readAndVerifyNumber(uint32_t, "j", 5)
				readAndVerifyNumber(int64_t, "k", 6)
				readAndVerifyNumber(uint64_t, "l", 7)
			context->endDictionary(context);
			readAndVerifyNumber(float, "m", 8)
			readAndVerifyNumber(double, "n", 9)
			readAndVerifyString("o", "10")
			readAndVerifyNumber(bool, "p", true)
			readAndVerifyEnumeration("q", 12, "enum", 12, NULL)
			readAndVerifyBitfield(8, "r", 0x01, "13", NULL)
			readAndVerifyBitfield(16, "s", 0x0001, "14", NULL)
			readAndVerifyBitfield(32, "t", 0x00000001, "15", NULL)
			readAndVerifyBitfield(64, "u", (uint64_t) 0x0000000000000001ull, "16", NULL)
		context->endDictionary(context);
	context->endDictionary(context);
	context->dispose(context);
	
	context = JSONDeserializationContext_createWithString(stringAndLength("{\"foo\": {\"bar\": 0, \"baz\": 1, \"bar\": 2}, \"foo\": 3, \"bar\": {\"foo\": 4, \"baz\": 5, \"baz\": 6}}"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyDictionary("", 3)
		verifyHasDictionaryKey("foo", true)
		verifyHasDictionaryKey("bar", true)
		verifyHasDictionaryKey("baz", false)
		verifyReadNextDictionaryKey("foo")
		beginAndVerifyDictionary(key, 3)
			verifyHasDictionaryKey("foo", false)
			verifyHasDictionaryKey("bar", true)
			verifyHasDictionaryKey("baz", true)
			verifyReadNextDictionaryKey("bar")
			readAndVerifyNumber(double, key, 0)
			verifyReadNextDictionaryKey("baz")
			readAndVerifyNumber(double, key, 1)
			verifyReadNextDictionaryKey("bar")
			readAndVerifyNumber(double, key, 2)
		context->endDictionary(context);
		verifyReadNextDictionaryKey("foo")
		readAndVerifyNumber(double, key, 3)
		verifyReadNextDictionaryKey("bar")
		beginAndVerifyDictionary(key, 3)
			verifyHasDictionaryKey("foo", true)
			verifyHasDictionaryKey("bar", false)
			verifyHasDictionaryKey("baz", true)
			verifyReadNextDictionaryKey("foo")
			readAndVerifyNumber(double, key, 4)
			verifyReadNextDictionaryKey("baz")
			readAndVerifyNumber(double, key, 5)
			verifyReadNextDictionaryKey("baz")
			readAndVerifyNumber(double, key, 6)
		context->endDictionary(context);
	context->endDictionary(context);
	context->dispose(context);
	
	context = JSONDeserializationContext_createWithString(stringAndLength("{\"\": -1, \"\": 0, \"\": 1, \"\": 2, \"\": 3, \"\": 4, \"\": 5, \"\": 6, \"\": 7, \"\": 8, \"\": 9, \"\": \"10\", \"\": true, \"\": \"enum\", \"\": [\"13\"], \"\": [\"14\"], \"\": [\"15\"], \"\": [\"16\"], \"\": [], \"\": {}, \"\": {\"\": 0, \"\": 0}}"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyDictionary("", 21)
		verifyHasDictionaryKey("", true)
		verifyHasDictionaryKey(" ", false)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(int8_t, key, -1)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(int8_t, key, 0)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(uint8_t, key, 1)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(int16_t, key, 2)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(uint16_t, key, 3)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(int32_t, key, 4)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(uint32_t, key, 5)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(int64_t, key, 6)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(uint64_t, key, 7)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(float, key, 8)
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(double, key, 9)
		verifyReadNextDictionaryKey("")
		readAndVerifyString(key, "10")
		verifyReadNextDictionaryKey("")
		readAndVerifyNumber(bool, key, true)
		verifyReadNextDictionaryKey("")
		readAndVerifyEnumeration(key, 12, "enum", 12, NULL)
		verifyReadNextDictionaryKey("")
		readAndVerifyBitfield(8, key, 0x01, "13", NULL)
		verifyReadNextDictionaryKey("")
		readAndVerifyBitfield(16, key, 0x0001, "14", NULL)
		verifyReadNextDictionaryKey("")
		readAndVerifyBitfield(32, key, 0x00000001, "15", NULL)
		verifyReadNextDictionaryKey("")
		readAndVerifyBitfield(64, key, (uint64_t) 0x0000000000000001ull, "16", NULL)
		verifyReadNextDictionaryKey("")
		beginAndVerifyArray(key, 0)
		context->endArray(context);
		verifyReadNextDictionaryKey("")
		context->beginStructure(context, key);
		context->endStructure(context);
		verifyReadNextDictionaryKey("")
		beginAndVerifyDictionary(key, 2)
		context->endDictionary(context);
	context->endDictionary(context);
	context->dispose(context);
}

static void testMixedContainers() {
	JSONDeserializationContext * context;
	
	context = JSONDeserializationContext_createWithString(stringAndLength("[{\"innerArray\": [], \"innerDict\": {}}, {\"innerArray2\": [], \"innerStruct\": {}}]"));
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("outerArray", 2)
		context->beginStructure(context, "outerStruct");
			beginAndVerifyArray("innerArray", 0)
			context->endArray(context);
			beginAndVerifyDictionary("innerDict", 0)
			context->endDictionary(context);
		context->endStructure(context);
		beginAndVerifyDictionary("outerDict", 2)
			beginAndVerifyArray("innerArray2", 0)
			context->endArray(context);
			context->beginStructure(context, "innerStruct");
			context->endStructure(context);
		context->endDictionary(context);
	context->endArray(context);
	context->dispose(context);
}

#define performActionWithTemporaryFile(STRING, ACTION) { \
	const char * tempFilePath; \
	int fd; \
	\
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd); \
	writeFileSimple(tempFilePath, (STRING), strlen(STRING)); \
	ACTION(tempFilePath) \
	close(fd); \
	unlink(tempFilePath); \
}

#define initContextWithFile(FILE_PATH) JSONDeserializationContext_initWithFile(&context2, FILE_PATH); context = &context2;
#define initContextFromTemporaryFile(STRING) performActionWithTemporaryFile(STRING, initContextWithFile)
#define createContextWithFile(FILE_PATH) context = JSONDeserializationContext_createWithFile(tempFilePath);
#define createContextFromTemporaryFile(STRING) performActionWithTemporaryFile(STRING, initContextWithFile)

static void testInitWithFile() {
	JSONDeserializationContext * context, context2;
	
	initContextFromTemporaryFile("[\"foo\"]")
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "foo")
	context->endArray(context);
	context->dispose(context);
	
	initContextFromTemporaryFile("[\"bar\"]")
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "bar")
	context->endArray(context);
	context->dispose(context);
	
	createContextFromTemporaryFile("[\"foo\"]")
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "foo")
	context->endArray(context);
	context->dispose(context);
	
	createContextFromTemporaryFile("[\"bar\"]")
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "bar")
	context->endArray(context);
	context->dispose(context);
}

static void testInitWithJSONNode() {
	JSONDeserializationContext * context, context2;
	struct JSONNode * node;
	
	node = JSONParser_loadString(stringAndLength("[\"foo\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	
	JSONDeserializationContext_initWithJSONNode(&context2, node);
	context = &context2;
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	TestCase_assert(context->rootNode != node, "Pointers expected to differ, but didn't");
	TestCase_assert(context->rootNode->subitems != node->subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(context->rootNode->subitems[0].value.string != node->subitems[0].value.string, "Pointers expected to differ, but didn't");
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "foo")
	context->endArray(context);
	context->dispose(context);
	
	context = JSONDeserializationContext_createWithJSONNode(node);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	TestCase_assert(context->rootNode != node, "Pointers expected to differ, but didn't");
	TestCase_assert(context->rootNode->subitems != node->subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(context->rootNode->subitems[0].value.string != node->subitems[0].value.string, "Pointers expected to differ, but didn't");
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "foo")
	context->endArray(context);
	context->dispose(context);
	
	JSONNode_dispose(node);
	
	node = JSONParser_loadString(stringAndLength("[\"bar\"]"), NULL);
	TestCase_assert(node != NULL, "Expected non-NULL but got NULL");
	if (node == NULL) {return;} // Suppress clang warning
	
	JSONDeserializationContext_initWithJSONNode(&context2, node);
	context = &context2;
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	TestCase_assert(context->rootNode != node, "Pointers expected to differ, but didn't");
	TestCase_assert(context->rootNode->subitems != node->subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(context->rootNode->subitems[0].value.string != node->subitems[0].value.string, "Pointers expected to differ, but didn't");
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "bar")
	context->endArray(context);
	context->dispose(context);
	
	context = JSONDeserializationContext_createWithJSONNode(node);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	TestCase_assert(context->rootNode != node, "Pointers expected to differ, but didn't");
	TestCase_assert(context->rootNode->subitems != node->subitems, "Pointers expected to differ, but didn't");
	TestCase_assert(context->rootNode->subitems[0].value.string != node->subitems[0].value.string, "Pointers expected to differ, but didn't");
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "bar")
	context->endArray(context);
	context->dispose(context);
	
	JSONNode_dispose(node);
}

static void testErrorReporting() {
	JSONDeserializationContext * context, context2;
	jmp_buf jmpBuf;
	int status;
	
	context = JSONDeserializationContext_createWithJSONNode(NULL);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == JSON_SERIALIZATION_ERROR_INVALID_JSON, "Expected %d but got %d", JSON_SERIALIZATION_ERROR_INVALID_JSON, context->status);
	context->dispose(context);
	
	JSONDeserializationContext_initWithJSONNode(&context2, NULL);
	TestCase_assert(context2.status == JSON_SERIALIZATION_ERROR_INVALID_JSON, "Expected %d but got %d", JSON_SERIALIZATION_ERROR_INVALID_JSON, context2.status);
	context2.dispose(&context2);
	
	context = JSONDeserializationContext_createWithFile("If this file exists, unit tests will fail");
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == JSON_SERIALIZATION_ERROR_INVALID_JSON, "Expected %d but got %d", JSON_SERIALIZATION_ERROR_INVALID_JSON, context->status);
	context->dispose(context);
	
	JSONDeserializationContext_initWithFile(&context2, "If this file exists, unit tests will fail");
	TestCase_assert(context2.status == JSON_SERIALIZATION_ERROR_INVALID_JSON, "Expected %d but got %d", JSON_SERIALIZATION_ERROR_INVALID_JSON, context2.status);
	context2.dispose(&context2);
	
	context = JSONDeserializationContext_createWithString(NULL, 0);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == JSON_SERIALIZATION_ERROR_INVALID_JSON, "Expected %d but got %d", JSON_SERIALIZATION_ERROR_INVALID_JSON, context->status);
	context->dispose(context);
	
	JSONDeserializationContext_initWithString(&context2, NULL, 0);
	TestCase_assert(context2.status == JSON_SERIALIZATION_ERROR_INVALID_JSON, "Expected %d but got %d", JSON_SERIALIZATION_ERROR_INVALID_JSON, context2.status);
	context2.dispose(&context2);
	
	context = JSONDeserializationContext_createWithString("", 0);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == JSON_SERIALIZATION_ERROR_INVALID_JSON, "Expected %d but got %d", JSON_SERIALIZATION_ERROR_INVALID_JSON, context->status);
	context->dispose(context);
	
	JSONDeserializationContext_initWithString(&context2, "", 0);
	TestCase_assert(context2.status == JSON_SERIALIZATION_ERROR_INVALID_JSON, "Expected %d but got %d", JSON_SERIALIZATION_ERROR_INVALID_JSON, context2.status);
	context2.dispose(&context2);
	
#define _testFailure(STRING, ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	context = JSONDeserializationContext_createWithString(STRING, strlen(STRING)); \
	PREAMBLE_CODE \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d (OK)", SERIALIZATION_ERROR_OK, context->status); \
	FAIL_CODE \
	TestCase_assert(context->status == ERROR_STATUS, "Expected %d but got %d (context->status)", ERROR_STATUS, context->status); \
	context->dispose(context); \
	\
	context = JSONDeserializationContext_createWithString(STRING, strlen(STRING)); \
	context->jmpBuf = &jmpBuf; \
	status = setjmp(jmpBuf); \
	if (!status) { \
		PREAMBLE_CODE \
		TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d (OK setjmp)", SERIALIZATION_ERROR_OK, context->status); \
		FAIL_CODE \
	} \
	TestCase_assert(status == ERROR_STATUS, "Expected %d but got %d (status setjmp)", ERROR_STATUS, status); \
	TestCase_assert(context->status == ERROR_STATUS, "Expected %d but got %d (context->status setjmp)", ERROR_STATUS, context->status); \
	context->dispose(context);
	
	_testFailure("[\"a\"]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readInt64(context, "");)
	_testFailure("[\"a\"]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readUInt64(context, "");)
	_testFailure("[\"-9007199254740992\"]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readInt64(context, "");)
	_testFailure("[\"9007199254740992\"]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readUInt64(context, "");)
	
	_testFailure("[\"-9007199254740993 foo\"]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readInt64(context, "");)
	_testFailure("[\"bar -9007199254740993\"]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readInt64(context, "");)
	_testFailure("[\"9007199254740993 bar\"]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readUInt64(context, "");)
	_testFailure("[\"foo 9007199254740993\"]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readUInt64(context, "");)
	
	_testFailure("[-129]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readInt8(context, "");)
	_testFailure("[128]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readInt8(context, "");)
	_testFailure("[-1]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readUInt8(context, "");)
	_testFailure("[256]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readUInt8(context, "");)
	_testFailure("[-32769]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readInt16(context, "");)
	_testFailure("[32768]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readInt16(context, "");)
	_testFailure("[-1]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readUInt16(context, "");)
	_testFailure("[65536]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readUInt16(context, "");)
	_testFailure("[-2147483649]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readInt32(context, "");)
	_testFailure("[2147483648]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readInt32(context, "");)
	_testFailure("[-1]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readUInt32(context, "");)
	_testFailure("[4294967296]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readUInt32(context, "");)
	_testFailure("[-18446744073709551616]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readInt64(context, "");)
	_testFailure("[18446744073709551616]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readInt64(context, "");)
	_testFailure("[-1]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readUInt64(context, "");)
	_testFailure("[36893488147419103232]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readUInt64(context, "");)
	_testFailure("[-680564733841876926926749214863536422912]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readFloat(context, "");)
	_testFailure("[680564733841876926926749214863536422912]",
	             SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE,
	             context->beginArray(context, "");,
	             context->readFloat(context, "");)
	
#define _testFiveFailures(STRING_1, STRING_2, STRING_3, STRING_4, STRING_5, ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	_testFailure(STRING_1, ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	_testFailure(STRING_2, ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	_testFailure(STRING_3, ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	_testFailure(STRING_4, ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	_testFailure(STRING_5, ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE)
	
	_testFailure("{}",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             ,
	             context->beginArray(context, "");)
	_testFiveFailures("[{}]", "[\"\"]", "[0]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->beginArray(context, "");)
	_testFailure("[]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             ,
	             context->beginStructure(context, "");)
	_testFiveFailures("[[]]", "[\"\"]", "[0]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->beginStructure(context, "");)
	_testFailure("[]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             ,
	             context->beginDictionary(context, "");)
	_testFiveFailures("[[]]", "[\"\"]", "[0]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->beginDictionary(context, "");)
	
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readInt8(context, "");)
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readUInt8(context, "");)
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readInt16(context, "");)
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readUInt16(context, "");)
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readInt32(context, "");)
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readUInt32(context, "");)
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readInt64(context, "");)
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readUInt64(context, "");)
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readFloat(context, "");)
	_testFiveFailures("[{}]", "[[]]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readDouble(context, "");)
	
	_testFiveFailures("[{}]", "[[]]", "[0]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readString(context, "");)
	
	_testFiveFailures("[{}]", "[[]]", "[0]", "[\"\"]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readBoolean(context, "");)
	
	_testFiveFailures("[{}]", "[[]]", "[0]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readEnumeration(context, "", NULL);)
	
	_testFiveFailures("[{}]", "[0]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readBitfield8(context, "", NULL);)
	_testFiveFailures("[{}]", "[0]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readBitfield16(context, "", NULL);)
	_testFiveFailures("[{}]", "[0]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readBitfield32(context, "", NULL);)
	_testFiveFailures("[{}]", "[0]", "[\"\"]", "[false]", "[null]",
	             SERIALIZATION_ERROR_INCORRECT_TYPE,
	             context->beginArray(context, "");,
	             context->readBitfield64(context, "", NULL);)
	
#undef _testFiveFailures
	
#define _testEndOfArray(READ_CODE) \
	_testFailure("[]", SERIALIZATION_ERROR_END_OF_CONTAINER, context->beginArray(context, "");, READ_CODE) \
	_testFailure("[0]", SERIALIZATION_ERROR_END_OF_CONTAINER, context->beginArray(context, ""); context->readInt8(context, "");, READ_CODE) \
	_testFailure("[[], 0]", SERIALIZATION_ERROR_END_OF_CONTAINER, context->beginArray(context, ""); context->beginArray(context, "");, READ_CODE) \
	_testFailure("[[0], 0]", SERIALIZATION_ERROR_END_OF_CONTAINER, context->beginArray(context, ""); context->beginArray(context, ""); context->readInt8(context, "");, READ_CODE)
	
	_testEndOfArray(context->beginArray(context, "");)
	_testEndOfArray(context->beginStructure(context, "");)
	_testEndOfArray(context->beginDictionary(context, "");)
	_testEndOfArray(context->readInt8(context, "");)
	_testEndOfArray(context->readUInt8(context, "");)
	_testEndOfArray(context->readInt16(context, "");)
	_testEndOfArray(context->readUInt16(context, "");)
	_testEndOfArray(context->readInt32(context, "");)
	_testEndOfArray(context->readUInt32(context, "");)
	_testEndOfArray(context->readInt64(context, "");)
	_testEndOfArray(context->readUInt64(context, "");)
	_testEndOfArray(context->readFloat(context, "");)
	_testEndOfArray(context->readDouble(context, "");)
	_testEndOfArray(context->readString(context, "");)
	_testEndOfArray(context->readBoolean(context, "");)
	_testEndOfArray(context->readEnumeration(context, "", "enum", 0, NULL);)
	_testEndOfArray(context->readBitfield8(context, "", NULL);)
	_testEndOfArray(context->readBitfield16(context, "", NULL);)
	_testEndOfArray(context->readBitfield32(context, "", NULL);)
	_testEndOfArray(context->readBitfield64(context, "", NULL);)
	
#undef _testEndOfArray
	
	_testFailure("{}",
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginDictionary(context, "");,
	             context->readNextDictionaryKey(context);)
	_testFailure("{\"\": 0}",
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginDictionary(context, "");
	             context->readNextDictionaryKey(context);,
	             context->readNextDictionaryKey(context);)
	_testFailure("{\"\": {}, \"a\": 0}",
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginDictionary(context, "");
	             context->beginDictionary(context, "");,
	             context->readNextDictionaryKey(context);)
	_testFailure("{\"\": {\"\": 0}, \"a\": 0}",
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginDictionary(context, "");
	             context->beginDictionary(context, "");
	             context->readNextDictionaryKey(context);,
	             context->readNextDictionaryKey(context);)
	
#define _testNoSuchKey(READ_CODE) \
	_testFailure("{}", SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginStructure(context, "");, READ_CODE) \
	_testFailure("{\"a\": 0}", SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginStructure(context, "");, READ_CODE) \
	_testFailure("{\"\": {}, \"a\": 0}", SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginStructure(context, ""); context->beginStructure(context, "");, READ_CODE) \
	_testFailure("{\"\": {\"a\": 0}, \"a\": 0}", SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginStructure(context, ""); context->beginStructure(context, "");, READ_CODE) \
	_testFailure("{}", SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginDictionary(context, "");, READ_CODE) \
	_testFailure("{\"a\": 0}", SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginDictionary(context, "");, READ_CODE) \
	_testFailure("{\"\": {}, \"a\": 0}", SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginDictionary(context, ""); context->beginDictionary(context, "");, READ_CODE) \
	_testFailure("{\"\": {\"a\": 0}, \"a\": 0}", SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginDictionary(context, ""); context->beginDictionary(context, "");, READ_CODE)
	
	_testNoSuchKey(context->beginArray(context, "");)
	_testNoSuchKey(context->beginStructure(context, "");)
	_testNoSuchKey(context->beginDictionary(context, "");)
	_testNoSuchKey(context->readInt8(context, "");)
	_testNoSuchKey(context->readUInt8(context, "");)
	_testNoSuchKey(context->readInt16(context, "");)
	_testNoSuchKey(context->readUInt16(context, "");)
	_testNoSuchKey(context->readInt32(context, "");)
	_testNoSuchKey(context->readUInt32(context, "");)
	_testNoSuchKey(context->readInt64(context, "");)
	_testNoSuchKey(context->readUInt64(context, "");)
	_testNoSuchKey(context->readFloat(context, "");)
	_testNoSuchKey(context->readDouble(context, "");)
	_testNoSuchKey(context->readString(context, "");)
	_testNoSuchKey(context->readBoolean(context, "");)
	_testNoSuchKey(context->readEnumeration(context, "", "enum", 0, NULL);)
	_testNoSuchKey(context->readBitfield8(context, "", NULL);)
	_testNoSuchKey(context->readBitfield16(context, "", NULL);)
	_testNoSuchKey(context->readBitfield32(context, "", NULL);)
	_testNoSuchKey(context->readBitfield64(context, "", NULL);)
	
#undef _testNoSuchKey
	
	_testFailure("{\"foo\": 0, \"foo\": 1, \"bar\": 2}",
	             JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY,
	             ,
	             context->beginStructure(context, "");)
	_testFailure("{\"\": {\"bar\": 0, \"foo\": 1, \"bar\": 2}}",
	             JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY,
	             context->beginStructure(context, "");,
	             context->beginStructure(context, "");)
	_testFailure("{\"bar\": 0, \"foo\": 1, \"foo\": 2}",
	             JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY,
	             ,
	             context->beginStructure(context, "");)
	
	_testFailure("[[\"0\", \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\", \"8\"]]",
	             JSON_SERIALIZATION_ERROR_TOO_MANY_BITS,
	             context->beginArray(context, "");,
	             context->readBitfield8(context, "", NULL);)
	_testFailure("[[\"0\", \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\", \"8\", \"9\", \"10\", \"11\", \"12\", \"13\", \"14\", \"15\", \"16\"]]",
	             JSON_SERIALIZATION_ERROR_TOO_MANY_BITS,
	             context->beginArray(context, "");,
	             context->readBitfield16(context, "", NULL);)
	_testFailure("[[\"0\", \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\", \"8\", \"9\", \"10\", \"11\", \"12\", \"13\", \"14\", \"15\", \"16\", \"17\", \"18\", \"19\", \"20\", \"21\", \"22\", \"23\", \"24\", \"25\", \"26\", \"27\", \"28\", \"29\", \"30\", \"31\", \"32\"]]",
	             JSON_SERIALIZATION_ERROR_TOO_MANY_BITS,
	             context->beginArray(context, "");,
	             context->readBitfield32(context, "", NULL);)
	_testFailure("[[\"0\", \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\", \"8\", \"9\", \"10\", \"11\", \"12\", \"13\", \"14\", \"15\", \"16\", \"17\", \"18\", \"19\", \"20\", \"21\", \"22\", \"23\", \"24\", \"25\", \"26\", \"27\", \"28\", \"29\", \"30\", \"31\", \"32\", \"33\", \"34\", \"35\", \"36\", \"37\", \"38\", \"39\", \"40\", \"41\", \"42\", \"43\", \"44\", \"45\", \"46\", \"47\", \"48\", \"49\", \"50\", \"51\", \"52\", \"53\", \"54\", \"55\", \"56\", \"57\", \"58\", \"59\", \"60\", \"61\", \"62\", \"63\", \"64\"]]",
	             JSON_SERIALIZATION_ERROR_TOO_MANY_BITS,
	             context->beginArray(context, "");,
	             context->readBitfield64(context, "", NULL);)
	
#define _testReadBitfieldFailure(STRING, ERROR, ...) \
	_testFailure(STRING, ERROR, context->beginArray(context, "");, context->readBitfield8(context, "", __VA_ARGS__);) \
	_testFailure(STRING, ERROR, context->beginArray(context, "");, context->readBitfield16(context, "", __VA_ARGS__);) \
	_testFailure(STRING, ERROR, context->beginArray(context, "");, context->readBitfield32(context, "", __VA_ARGS__);) \
	_testFailure(STRING, ERROR, context->beginArray(context, "");, context->readBitfield64(context, "", __VA_ARGS__);)
	
	_testReadBitfieldFailure("[[\"not_a_bit\"]]", SERIALIZATION_ERROR_UNNAMED_BIT, NULL)
	_testReadBitfieldFailure("[[\"not_a_bit\"]]", SERIALIZATION_ERROR_UNNAMED_BIT, "a_bit", NULL)
	
	_testReadBitfieldFailure("[[{}]]", SERIALIZATION_ERROR_INCORRECT_TYPE, NULL)
	_testReadBitfieldFailure("[[[]]]", SERIALIZATION_ERROR_INCORRECT_TYPE, NULL)
	_testReadBitfieldFailure("[[0]]", SERIALIZATION_ERROR_INCORRECT_TYPE, NULL)
	_testReadBitfieldFailure("[[false]]", SERIALIZATION_ERROR_INCORRECT_TYPE, NULL)
	_testReadBitfieldFailure("[[null]]", SERIALIZATION_ERROR_INCORRECT_TYPE, NULL)
	
	_testReadBitfieldFailure("[[\"a_bit\", \"a_bit\"]]", SERIALIZATION_ERROR_DUPLICATE_BIT, "a_bit", NULL);
	_testReadBitfieldFailure("[[\"not_a_bit\", \"not_a_bit\"]]", SERIALIZATION_ERROR_DUPLICATE_BIT, "a_bit", NULL);
	_testReadBitfieldFailure("[[\"not_a_bit\", \"not_a_bit\", \"a_bit\"]]", SERIALIZATION_ERROR_DUPLICATE_BIT, "a_bit", NULL);
	_testReadBitfieldFailure("[[\"not_a_bit\", \"a_bit\", \"not_a_bit\"]]", SERIALIZATION_ERROR_DUPLICATE_BIT, "a_bit", NULL);
	_testReadBitfieldFailure("[[\"a_bit\", \"not_a_bit\", \"not_a_bit\"]]", SERIALIZATION_ERROR_DUPLICATE_BIT, "a_bit", NULL);
	
	_testReadBitfieldFailure("[[\"a_bit\", \"a_bit\", null]]", SERIALIZATION_ERROR_DUPLICATE_BIT, "a_bit", NULL);
	_testReadBitfieldFailure("[[\"a_bit\", null, \"a_bit\"]]", SERIALIZATION_ERROR_INCORRECT_TYPE, "a_bit", NULL);
	_testReadBitfieldFailure("[[null, \"a_bit\", \"a_bit\"]]", SERIALIZATION_ERROR_INCORRECT_TYPE, "a_bit", NULL);
	
#undef _testReadBitfieldFailure
	
	_testFailure("[\"a\"]",
	             SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	             context->beginArray(context, "");,
	             context->readEnumeration(context, "", NULL);)
	_testFailure("[\"a\"]",
	             SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	             context->beginArray(context, "");,
	             context->readEnumeration(context, "", "b", 0, NULL);)
	
	_testFailure("[\"\"]",
	             SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE,
	             context->beginArray(context, "");,
	             context->readEnumeration(context, "", "enum0", 0, "enum1", 0, NULL);)
	
	_testFailure("[\"\"]",
	             SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME,
	             context->beginArray(context, "");,
	             context->readEnumeration(context, "", "enum0", 0, "enum0", 1, NULL);)
	
	_testFailure("[]",
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginArray(context, "");,
	             context->hasDictionaryKey(context, "");)
	_testFailure("[]",
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginArray(context, "");,
	             context->readNextDictionaryKey(context);)
	_testFailure("[[]]",
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginArray(context, "");
	             context->beginArray(context, "");,
	             context->hasDictionaryKey(context, "");)
	_testFailure("[[]]",
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginArray(context, "");
	             context->beginArray(context, "");,
	             context->readNextDictionaryKey(context);)
	_testFailure("{}",
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginStructure(context, "");,
	             context->hasDictionaryKey(context, "");)
	_testFailure("{}",
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginStructure(context, "");,
	             context->readNextDictionaryKey(context);)
	_testFailure("{\"\": {}}",
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginStructure(context, "");
	             context->beginStructure(context, "");,
	             context->hasDictionaryKey(context, "");)
	_testFailure("{\"\": {}}",
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginStructure(context, "");
	             context->beginStructure(context, "");,
	             context->readNextDictionaryKey(context);)
	
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readInt8(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readUInt8(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readInt16(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readUInt16(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readInt32(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readUInt32(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readInt64(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readUInt64(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readFloat(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readDouble(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readString(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBoolean(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readEnumeration(context, NULL, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBitfield8(context, NULL, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBitfield16(context, NULL, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBitfield32(context, NULL, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBitfield64(context, NULL, NULL);)
	
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readInt8(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readUInt8(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readInt16(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readUInt16(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readInt32(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readUInt32(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readInt64(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readUInt64(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readFloat(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readDouble(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readString(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBoolean(context, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readEnumeration(context, NULL, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBitfield8(context, NULL, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBitfield16(context, NULL, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBitfield32(context, NULL, NULL);)
	_testFailure("{}", SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBitfield64(context, NULL, NULL);)
	
	_testFailure("[]",
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginArray(context, "");
	             context->endArray(context);,
	             context->beginArray(context, "");)
	_testFailure("[]",
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginArray(context, "");
	             context->endArray(context);,
	             context->beginDictionary(context, "");)
	_testFailure("[]",
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginArray(context, "");
	             context->endArray(context);,
	             context->beginStructure(context, "");)
	_testFailure("{}",
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginStructure(context, "");
	             context->endStructure(context);,
	             context->beginArray(context, "");)
	_testFailure("{}",
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginStructure(context, "");
	             context->endStructure(context);,
	             context->beginDictionary(context, "");)
	_testFailure("{}",
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginStructure(context, "");
	             context->endStructure(context);,
	             context->beginStructure(context, "");)
	_testFailure("{}",
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginDictionary(context, "");
	             context->endDictionary(context);,
	             context->beginArray(context, "");)
	_testFailure("{}",
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginDictionary(context, "");
	             context->endDictionary(context);,
	             context->beginDictionary(context, "");)
	_testFailure("{}",
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginDictionary(context, "");
	             context->endDictionary(context);,
	             context->beginStructure(context, "");)
	
	_testFailure("[]",
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             ;,
	             context->endArray(context);)
	_testFailure("[[]]",
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             context->beginArray(context, "");
	             context->beginArray(context, "");
	             context->endArray(context);
	             context->endArray(context);,
	             context->endArray(context);)
	_testFailure("{}",
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             ;,
	             context->endStructure(context);)
	_testFailure("{\"\": {}}",
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             context->beginStructure(context, "");
	             context->beginStructure(context, "");
	             context->endStructure(context);
	             context->endStructure(context);,
	             context->endStructure(context);)
	_testFailure("{}",
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             ;,
	             context->endDictionary(context);)
	_testFailure("{\"\": {}}",
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             context->beginDictionary(context, "");
	             context->endDictionary(context);,
	             context->endDictionary(context);)
	
	_testFailure("[]",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "");,
	             context->endStructure(context);)
	_testFailure("[[]]",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "");
	             context->beginArray(context, "");,
	             context->endStructure(context);)
	_testFailure("[]",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "");,
	             context->endDictionary(context);)
	_testFailure("[[]]",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "");
	             context->beginArray(context, "");,
	             context->endDictionary(context);)
	_testFailure("{}",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "");,
	             context->endArray(context);)
	_testFailure("{\"\": {}}",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "");
	             context->beginStructure(context, "");,
	             context->endArray(context);)
	_testFailure("{}",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "");,
	             context->endDictionary(context);)
	_testFailure("{\"\": {}}",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "");
	             context->beginStructure(context, "");,
	             context->endDictionary(context);)
	_testFailure("{}",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "");,
	             context->endArray(context);)
	_testFailure("{\"\": {}}",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "");
	             context->beginDictionary(context, "");,
	             context->endArray(context);)
	_testFailure("{}",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "");,
	             context->endStructure(context);)
	_testFailure("{\"\": {}}",
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "");
	             context->beginDictionary(context, "");,
	             context->endStructure(context);)
	
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readInt8(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readUInt8(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readInt16(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readUInt16(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readInt32(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readUInt32(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readInt64(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readUInt64(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readFloat(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readDouble(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readString(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBoolean(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readEnumeration(context, "key", "", 0, NULL);)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBitfield8(context, "key", NULL);)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBitfield16(context, "key", NULL);)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBitfield32(context, "key", NULL);)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBitfield64(context, "key", NULL);)
	
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readInt8(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readUInt8(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readInt16(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readUInt16(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readInt32(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readUInt32(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readInt64(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readUInt64(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readFloat(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readDouble(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readString(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readBoolean(context, "key");)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readEnumeration(context, "key", "", 0, NULL);)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readBitfield8(context, "key", NULL);)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readBitfield16(context, "key", NULL);)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readBitfield32(context, "key", NULL);)
	_testFailure("[]", SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->readBitfield64(context, "key", NULL);)
	
	_testFailure("[0]", JSON_SERIALIZATION_ERROR_CONTAINER_NOT_FULLY_READ, context->beginArray(context, "");, context->endArray(context);)
	
#undef _testFailure
}

TEST_SUITE(JSONDeserializationContextTest,
           testInit,
           testNumberValues,
           testLargeIntegerRepresentations,
           testStringValues,
           testBlobValues,
           testBooleanValues,
           testEnumerations,
           testBitfields,
           testArrays,
           testStructures,
           testDictionaries,
           testMixedContainers,
           testInitWithFile,
           testInitWithJSONNode,
           testErrorReporting)
