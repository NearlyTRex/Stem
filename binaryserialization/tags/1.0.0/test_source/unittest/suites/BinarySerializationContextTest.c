#include "unittest/framework/TestSuite.h"
#include "binaryserialization/BinarySerializationContext.h"
#include "utilities/IOUtilities.h"
#include <stdio.h>
#include <float.h>
#include <unistd.h>

static void testInit() {
	BinarySerializationContext context, * contextPtr;
	
	memset(&context, 0xFF, sizeof(context));
	BinarySerializationContext_init(&context, false);
	TestCase_assert(context.jmpBuf == NULL, "Expected NULL but got %p", context.jmpBuf);
	TestCase_assert(context.status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context.status);
	TestCase_assert(context.dispose == BinarySerializationContext_dispose, "Expected %p but got %p", BinarySerializationContext_dispose, context.dispose);
	TestCase_assert(context.beginStructure == BinarySerializationContext_beginStructure, "Expected %p but got %p", BinarySerializationContext_beginStructure, context.beginStructure);
	TestCase_assert(context.beginDictionary == BinarySerializationContext_beginDictionary, "Expected %p but got %p", BinarySerializationContext_beginDictionary, context.beginDictionary);
	TestCase_assert(context.beginArray == BinarySerializationContext_beginArray, "Expected %p but got %p", BinarySerializationContext_beginArray, context.beginArray);
	TestCase_assert(context.endStructure == BinarySerializationContext_endStructure, "Expected %p but got %p", BinarySerializationContext_endStructure, context.endStructure);
	TestCase_assert(context.endDictionary == BinarySerializationContext_endDictionary, "Expected %p but got %p", BinarySerializationContext_endDictionary, context.endDictionary);
	TestCase_assert(context.endArray == BinarySerializationContext_endArray, "Expected %p but got %p", BinarySerializationContext_endArray, context.endArray);
	TestCase_assert(context.writeInt8 == BinarySerializationContext_writeInt8, "Expected %p but got %p", BinarySerializationContext_writeInt8, context.writeInt8);
	TestCase_assert(context.writeUInt8 == BinarySerializationContext_writeUInt8, "Expected %p but got %p", BinarySerializationContext_writeUInt8, context.writeUInt8);
	TestCase_assert(context.writeInt16 == BinarySerializationContext_writeInt16, "Expected %p but got %p", BinarySerializationContext_writeInt16, context.writeInt16);
	TestCase_assert(context.writeUInt16 == BinarySerializationContext_writeUInt16, "Expected %p but got %p", BinarySerializationContext_writeUInt16, context.writeUInt16);
	TestCase_assert(context.writeInt32 == BinarySerializationContext_writeInt32, "Expected %p but got %p", BinarySerializationContext_writeInt32, context.writeInt32);
	TestCase_assert(context.writeUInt32 == BinarySerializationContext_writeUInt32, "Expected %p but got %p", BinarySerializationContext_writeUInt32, context.writeUInt32);
	TestCase_assert(context.writeInt64 == BinarySerializationContext_writeInt64, "Expected %p but got %p", BinarySerializationContext_writeInt64, context.writeInt64);
	TestCase_assert(context.writeUInt64 == BinarySerializationContext_writeUInt64, "Expected %p but got %p", BinarySerializationContext_writeUInt64, context.writeUInt64);
	TestCase_assert(context.writeFloat == BinarySerializationContext_writeFloat, "Expected %p but got %p", BinarySerializationContext_writeFloat, context.writeFloat);
	TestCase_assert(context.writeDouble == BinarySerializationContext_writeDouble, "Expected %p but got %p", BinarySerializationContext_writeDouble, context.writeDouble);
	TestCase_assert(context.writeString == BinarySerializationContext_writeString, "Expected %p but got %p", BinarySerializationContext_writeString, context.writeString);
	TestCase_assert(context.writeBoolean == BinarySerializationContext_writeBoolean, "Expected %p but got %p", BinarySerializationContext_writeBoolean, context.writeBoolean);
	TestCase_assert(context.writeEnumeration == BinarySerializationContext_writeEnumeration, "Expected %p but got %p", BinarySerializationContext_writeEnumeration, context.writeEnumeration);
	TestCase_assert(context.writeBitfield8 == BinarySerializationContext_writeBitfield8, "Expected %p but got %p", BinarySerializationContext_writeBitfield8, context.writeBitfield8);
	TestCase_assert(context.writeBitfield16 == BinarySerializationContext_writeBitfield16, "Expected %p but got %p", BinarySerializationContext_writeBitfield16, context.writeBitfield16);
	TestCase_assert(context.writeBitfield32 == BinarySerializationContext_writeBitfield32, "Expected %p but got %p", BinarySerializationContext_writeBitfield32, context.writeBitfield32);
	TestCase_assert(context.writeBitfield64 == BinarySerializationContext_writeBitfield64, "Expected %p but got %p", BinarySerializationContext_writeBitfield64, context.writeBitfield64);
	TestCase_assert(context.writeToBytes == BinarySerializationContext_writeToBytes, "Expected %p but got %p", BinarySerializationContext_writeToBytes, context.writeToBytes);
	TestCase_assert(context.writeToFile == BinarySerializationContext_writeToFile, "Expected %p but got %p", BinarySerializationContext_writeToFile, context.writeToFile);
	
	contextPtr = BinarySerializationContext_create(false);
	TestCase_assert(contextPtr != NULL, "Expected non-NULL but got NULL");
	if (contextPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(contextPtr->jmpBuf == NULL, "Expected NULL but got %p", contextPtr->jmpBuf);
	TestCase_assert(contextPtr->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, contextPtr->status);
	TestCase_assert(contextPtr->dispose == BinarySerializationContext_dispose, "Expected %p but got %p", BinarySerializationContext_dispose, contextPtr->dispose);
	TestCase_assert(contextPtr->beginStructure == BinarySerializationContext_beginStructure, "Expected %p but got %p", BinarySerializationContext_beginStructure, contextPtr->beginStructure);
	TestCase_assert(contextPtr->beginDictionary == BinarySerializationContext_beginDictionary, "Expected %p but got %p", BinarySerializationContext_beginDictionary, contextPtr->beginDictionary);
	TestCase_assert(contextPtr->beginArray == BinarySerializationContext_beginArray, "Expected %p but got %p", BinarySerializationContext_beginArray, contextPtr->beginArray);
	TestCase_assert(contextPtr->endStructure == BinarySerializationContext_endStructure, "Expected %p but got %p", BinarySerializationContext_endStructure, contextPtr->endStructure);
	TestCase_assert(contextPtr->endDictionary == BinarySerializationContext_endDictionary, "Expected %p but got %p", BinarySerializationContext_endDictionary, contextPtr->endDictionary);
	TestCase_assert(contextPtr->endArray == BinarySerializationContext_endArray, "Expected %p but got %p", BinarySerializationContext_endArray, contextPtr->endArray);
	TestCase_assert(contextPtr->writeInt8 == BinarySerializationContext_writeInt8, "Expected %p but got %p", BinarySerializationContext_writeInt8, contextPtr->writeInt8);
	TestCase_assert(contextPtr->writeUInt8 == BinarySerializationContext_writeUInt8, "Expected %p but got %p", BinarySerializationContext_writeUInt8, contextPtr->writeUInt8);
	TestCase_assert(contextPtr->writeInt16 == BinarySerializationContext_writeInt16, "Expected %p but got %p", BinarySerializationContext_writeInt16, contextPtr->writeInt16);
	TestCase_assert(contextPtr->writeUInt16 == BinarySerializationContext_writeUInt16, "Expected %p but got %p", BinarySerializationContext_writeUInt16, contextPtr->writeUInt16);
	TestCase_assert(contextPtr->writeInt32 == BinarySerializationContext_writeInt32, "Expected %p but got %p", BinarySerializationContext_writeInt32, contextPtr->writeInt32);
	TestCase_assert(contextPtr->writeUInt32 == BinarySerializationContext_writeUInt32, "Expected %p but got %p", BinarySerializationContext_writeUInt32, contextPtr->writeUInt32);
	TestCase_assert(contextPtr->writeInt64 == BinarySerializationContext_writeInt64, "Expected %p but got %p", BinarySerializationContext_writeInt64, contextPtr->writeInt64);
	TestCase_assert(contextPtr->writeUInt64 == BinarySerializationContext_writeUInt64, "Expected %p but got %p", BinarySerializationContext_writeUInt64, contextPtr->writeUInt64);
	TestCase_assert(contextPtr->writeFloat == BinarySerializationContext_writeFloat, "Expected %p but got %p", BinarySerializationContext_writeFloat, contextPtr->writeFloat);
	TestCase_assert(contextPtr->writeDouble == BinarySerializationContext_writeDouble, "Expected %p but got %p", BinarySerializationContext_writeDouble, contextPtr->writeDouble);
	TestCase_assert(contextPtr->writeString == BinarySerializationContext_writeString, "Expected %p but got %p", BinarySerializationContext_writeString, contextPtr->writeString);
	TestCase_assert(contextPtr->writeBoolean == BinarySerializationContext_writeBoolean, "Expected %p but got %p", BinarySerializationContext_writeBoolean, contextPtr->writeBoolean);
	TestCase_assert(contextPtr->writeEnumeration == BinarySerializationContext_writeEnumeration, "Expected %p but got %p", BinarySerializationContext_writeEnumeration, contextPtr->writeEnumeration);
	TestCase_assert(contextPtr->writeBitfield8 == BinarySerializationContext_writeBitfield8, "Expected %p but got %p", BinarySerializationContext_writeBitfield8, contextPtr->writeBitfield8);
	TestCase_assert(contextPtr->writeBitfield16 == BinarySerializationContext_writeBitfield16, "Expected %p but got %p", BinarySerializationContext_writeBitfield16, contextPtr->writeBitfield16);
	TestCase_assert(contextPtr->writeBitfield32 == BinarySerializationContext_writeBitfield32, "Expected %p but got %p", BinarySerializationContext_writeBitfield32, contextPtr->writeBitfield32);
	TestCase_assert(contextPtr->writeBitfield64 == BinarySerializationContext_writeBitfield64, "Expected %p but got %p", BinarySerializationContext_writeBitfield64, contextPtr->writeBitfield64);
	TestCase_assert(contextPtr->writeToBytes == BinarySerializationContext_writeToBytes, "Expected %p but got %p", BinarySerializationContext_writeToBytes, contextPtr->writeToBytes);
	TestCase_assert(contextPtr->writeToFile == BinarySerializationContext_writeToFile, "Expected %p but got %p", BinarySerializationContext_writeToFile, contextPtr->writeToFile);
	contextPtr->dispose(contextPtr);
}

#define PRINTABLE_BUFFER_SIZE 256

static char * printableBytes(unsigned char * bytes, size_t length) {
	static char printableBuffer[PRINTABLE_BUFFER_SIZE * 3 + 1];
	size_t charIndex;
	
	for (charIndex = 0; charIndex < length && charIndex < PRINTABLE_BUFFER_SIZE; charIndex++) {
		snprintf(printableBuffer + charIndex * 3, 4, "%02X ", bytes[charIndex]);
	}
	if (charIndex > 0) {
		printableBuffer[charIndex * 3 - 1] = 0;
	}
	return printableBuffer;
}

static size_t firstByteDifference(unsigned char * bytes, unsigned char * expected, size_t length) {
	size_t charIndex;
	
	for (charIndex = 0; charIndex < length; charIndex++) {
		if (bytes[charIndex] != expected[charIndex]) {
			break;
		}
	}
	return charIndex;
}

#define assertBytesMatch(bytes, expected, length) TestCase_assert(!memcmp(bytes, expected, length), "Bytes didn't match; got %s (first difference at %zu)", printableBytes(bytes, length), firstByteDifference(bytes, (unsigned char *) expected, length))

static void testTopLevelContainers() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "key");
	context->endStructure(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 4, "Expected 4 but got %zu", length);
	assertBytesMatch(bytes, "Stem", length);
	free(bytes);
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 8, "Expected 8 but got %zu", length);
	assertBytesMatch(bytes, "Stem\x00\x00\x00\x00", length);
	free(bytes);
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 12, "Expected 12 but got %zu", length);
	assertBytesMatch(bytes, "Stem\x00\x00\x00\x00\x00\x00\x00\x08", length);
	free(bytes);
}

static void testNumberValues() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt8(context, "item", 0);
	context->writeUInt8(context, "item", 1);
	context->writeInt16(context, "item", 2);
	context->writeUInt16(context, "item", 3);
	context->writeInt32(context, "item", 4);
	context->writeUInt32(context, "item", 5);
	context->writeInt64(context, "item", 6);
	context->writeUInt64(context, "item", 7);
	context->writeFloat(context, "item", 8);
	context->writeDouble(context, "item", 9);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 50, "Expected 50 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x0A"
	                        "\x00"
	                        "\x01"
	                        "\x00\x02"
	                        "\x00\x03"
	                        "\x00\x00\x00\x04"
	                        "\x00\x00\x00\x05"
	                        "\x00\x00\x00\x00\x00\x00\x00\x06"
	                        "\x00\x00\x00\x00\x00\x00\x00\x07"
	                        "\x41\x00\x00\x00"
	                        "\x40\x22\x00\x00\x00\x00\x00\x00", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt8(context, "item", 0);
	context->writeUInt8(context, "item", 1);
	context->writeInt16(context, "item", 2);
	context->writeUInt16(context, "item", 3);
	context->writeInt32(context, "item", 4);
	context->writeUInt32(context, "item", 5);
	context->writeInt64(context, "item", 6);
	context->writeUInt64(context, "item", 7);
	context->writeFloat(context, "item", 8);
	context->writeDouble(context, "item", 9);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 50, "Expected 50 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x0A\x00\x00\x00"
	                        "\x00"
	                        "\x01"
	                        "\x02\x00"
	                        "\x03\x00"
	                        "\x04\x00\x00\x00"
	                        "\x05\x00\x00\x00"
	                        "\x06\x00\x00\x00\x00\x00\x00\x00"
	                        "\x07\x00\x00\x00\x00\x00\x00\x00"
	                        "\x00\x00\x00\x41"
	                        "\x00\x00\x00\x00\x00\x00\x22\x40", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt8(context, "item", INT8_MIN);
	context->writeUInt8(context, "item", UINT8_MAX);
	context->writeInt16(context, "item", INT16_MIN);
	context->writeUInt16(context, "item", UINT16_MAX);
	context->writeInt32(context, "item", INT32_MIN);
	context->writeUInt32(context, "item", UINT32_MAX);
	context->writeInt64(context, "item", INT64_MIN);
	context->writeUInt64(context, "item", UINT64_MAX);
	context->writeFloat(context, "item", FLT_MAX);
	context->writeDouble(context, "item", DBL_MAX);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 50, "Expected 50 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x0A\x00\x00\x00"
	                        "\x80"
	                        "\xFF"
	                        "\x00\x80"
	                        "\xFF\xFF"
	                        "\x00\x00\x00\x80"
	                        "\xFF\xFF\xFF\xFF"
	                        "\x00\x00\x00\x00\x00\x00\x00\x80"
	                        "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	                        "\xFF\xFF\x7F\x7F"
	                        "\xFF\xFF\xFF\xFF\xFF\xFF\xEF\x7F", length);
	free(bytes);
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt8(context, "item", INT8_MIN);
	context->writeUInt8(context, "item", UINT8_MAX);
	context->writeInt16(context, "item", INT16_MIN);
	context->writeUInt16(context, "item", UINT16_MAX);
	context->writeInt32(context, "item", INT32_MIN);
	context->writeUInt32(context, "item", UINT32_MAX);
	context->writeInt64(context, "item", INT64_MIN);
	context->writeUInt64(context, "item", UINT64_MAX);
	context->writeFloat(context, "item", FLT_MAX);
	context->writeDouble(context, "item", DBL_MAX);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 50, "Expected 50 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x0A"
	                        "\x80"
	                        "\xFF"
	                        "\x80\x00"
	                        "\xFF\xFF"
	                        "\x80\x00\x00\x00"
	                        "\xFF\xFF\xFF\xFF"
	                        "\x80\x00\x00\x00\x00\x00\x00\x00"
	                        "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	                        "\x7F\x7F\xFF\xFF"
	                        "\x7F\xEF\xFF\xFF\xFF\xFF\xFF\xFF", length);
	free(bytes);
}

static void testStringValues() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeString(context, "item", "foo");
	context->writeString(context, "item", "Hello, world!");
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 26, "Expected 26 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x02"
	                        "foo\x00"
	                        "Hello, world!\x00", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeString(context, "item", "foo");
	context->writeString(context, "item", "Hello, world!");
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 26, "Expected 26 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x02\x00\x00\x00"
	                        "foo\x00"
	                        "Hello, world!\x00", length);
	free(bytes);
}

static void testBooleanValues() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeBoolean(context, "item", false);
	context->writeBoolean(context, "item", true);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 10, "Expected 10 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x02"
	                        "\x00"
	                        "\x01", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeBoolean(context, "item", false);
	context->writeBoolean(context, "item", true);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 10, "Expected 10 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x02\x00\x00\x00"
	                        "\x00"
	                        "\x01", length);
	free(bytes);
}

#define ENUM_TEST_0 0
#define ENUM_TEST_1 1
#define ENUM_TEST_zero 0
#define ENUM_TEST_one 1
#define ENUM_TEST_two 2

static void testEnumerations() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", ENUM_TEST_0, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_1, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_zero, enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_one,  enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_two,  enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 28, "Expected 28 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x05"
	                        "\x00\x00\x00\x00"
	                        "\x00\x00\x00\x01"
	                        "\x00\x00\x00\x00"
	                        "\x00\x00\x00\x01"
	                        "\x00\x00\x00\x02", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", ENUM_TEST_0, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_1, enumKV(ENUM_TEST_0), enumKV(ENUM_TEST_1), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_zero, enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_one,  enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL);
	context->writeEnumeration(context, "item", ENUM_TEST_two,  enumKV(ENUM_TEST_zero), enumKV(ENUM_TEST_one), enumKV(ENUM_TEST_two), NULL);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 28, "Expected 28 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x05\x00\x00\x00"
	                        "\x00\x00\x00\x00"
	                        "\x01\x00\x00\x00"
	                        "\x00\x00\x00\x00"
	                        "\x01\x00\x00\x00"
	                        "\x02\x00\x00\x00", length);
	free(bytes);
}

static void testBitfields() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeBitfield8(context, "item", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7", NULL);
	context->writeBitfield8(context, "item", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	context->writeBitfield16(context, "item", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", NULL);
	context->writeBitfield16(context, "item", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield32(context, "item", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", NULL);
	context->writeBitfield32(context, "item", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield64(context, "item", 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", NULL);
	context->writeBitfield64(context, "item", 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield8(context, "item", 0x00, NULL);
	context->writeBitfield16(context, "item", 0x0000, NULL);
	context->writeBitfield32(context, "item", 0x00000000, NULL);
	context->writeBitfield64(context, "item", 0x0000000000000000ull, NULL);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 53, "Expected 53 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x0C"
	                        "\xAA"
	                        "\x57"
	                        "\xF0\x01"
	                        "\x00\x0F"
	                        "\xF0\x00\x00\x01"
	                        "\x00\x00\x00\x1E"
	                        "\xF0\x00\x00\x00\x00\x00\x00\x01"
	                        "\x00\x00\x00\x00\x00\x00\x00\x3C"
	                        "\x00"
	                        "\x00\x00"
	                        "\x00\x00\x00\x00"
	                        "\x00\x00\x00\x00\x00\x00\x00\x00", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeBitfield8(context, "item", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7", NULL);
	context->writeBitfield8(context, "item", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	context->writeBitfield16(context, "item", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", NULL);
	context->writeBitfield16(context, "item", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield32(context, "item", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", NULL);
	context->writeBitfield32(context, "item", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield64(context, "item", 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", NULL);
	context->writeBitfield64(context, "item", 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	context->writeBitfield8(context, "item", 0x00, NULL);
	context->writeBitfield16(context, "item", 0x0000, NULL);
	context->writeBitfield32(context, "item", 0x00000000, NULL);
	context->writeBitfield64(context, "item", 0x0000000000000000ull, NULL);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 53, "Expected 53 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x0C\x00\x00\x00"
	                        "\xAA"
	                        "\x57"
	                        "\x01\xF0"
	                        "\x0F\x00"
	                        "\x01\x00\x00\xF0"
	                        "\x1E\x00\x00\x00"
	                        "\x01\x00\x00\x00\x00\x00\x00\xF0"
	                        "\x3C\x00\x00\x00\x00\x00\x00\x00"
	                        "\x00"
	                        "\x00\x00"
	                        "\x00\x00\x00\x00"
	                        "\x00\x00\x00\x00\x00\x00\x00\x00", length);
	free(bytes);
}

static void testArrays() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
		context->beginArray(context, "item");
		context->endArray(context);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 12, "Expected 12 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x01"
	                        "\x00\x00\x00\x00", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
		context->beginArray(context, "item");
		context->endArray(context);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 12, "Expected 12 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x01\x00\x00\x00"
	                        "\x00\x00\x00\x00", length);
	free(bytes);
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
		context->beginArray(context, "item");
			context->writeInt8(context, "item", 0);
		context->endArray(context);
		context->beginArray(context, "item");
			context->writeUInt8(context, "item", 1);
			context->beginArray(context, "item");
				context->writeInt16(context, "item", 2);
				context->writeUInt16(context, "item", 3);
				context->writeInt32(context, "item", 4);
				context->writeUInt32(context, "item", 5);
				context->writeInt64(context, "item", 6);
				context->writeUInt64(context, "item", 7);
			context->endArray(context);
			context->writeFloat(context, "item", 8);
			context->writeDouble(context, "item", 9);
			context->writeString(context, "item", "10");
			context->writeBoolean(context, "item", true);
			context->writeEnumeration(context, "item", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "item", 1, "13", NULL);
			context->writeBitfield16(context, "item", 1, "14", NULL);
			context->writeBitfield32(context, "item", 1, "15", NULL);
			context->writeBitfield64(context, "item", 1, "16", NULL);
		context->endArray(context);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 85, "Expected 85 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x02"
	                        "\x00\x00\x00\x01"
	                        "\x00"
	                        "\x00\x00\x00\x0B"
	                        "\x01"
	                        "\x00\x00\x00\x06"
	                        "\x00\x02"
	                        "\x00\x03"
	                        "\x00\x00\x00\x04"
	                        "\x00\x00\x00\x05"
	                        "\x00\x00\x00\x00\x00\x00\x00\x06"
	                        "\x00\x00\x00\x00\x00\x00\x00\x07"
	                        "\x41\x00\x00\x00"
	                        "\x40\x22\x00\x00\x00\x00\x00\x00"
	                        "10\x00"
	                        "\x01"
	                        "\x00\x00\x00\x0C"
	                        "\x01"
	                        "\x00\x01"
	                        "\x00\x00\x00\x01"
	                        "\x00\x00\x00\x00\x00\x00\x00\x01", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
		context->beginArray(context, "item");
			context->writeInt8(context, "item", 0);
		context->endArray(context);
		context->beginArray(context, "item");
			context->writeUInt8(context, "item", 1);
			context->beginArray(context, "item");
				context->writeInt16(context, "item", 2);
				context->writeUInt16(context, "item", 3);
				context->writeInt32(context, "item", 4);
				context->writeUInt32(context, "item", 5);
				context->writeInt64(context, "item", 6);
				context->writeUInt64(context, "item", 7);
			context->endArray(context);
			context->writeFloat(context, "item", 8);
			context->writeDouble(context, "item", 9);
			context->writeString(context, "item", "10");
			context->writeBoolean(context, "item", true);
			context->writeEnumeration(context, "item", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "item", 1, "13", NULL);
			context->writeBitfield16(context, "item", 1, "14", NULL);
			context->writeBitfield32(context, "item", 1, "15", NULL);
			context->writeBitfield64(context, "item", 1, "16", NULL);
		context->endArray(context);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 85, "Expected 85 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x02\x00\x00\x00"
	                        "\x01\x00\x00\x00"
	                        "\x00"
	                        "\x0B\x00\x00\x00"
	                        "\x01"
	                        "\x06\x00\x00\x00"
	                        "\x02\x00"
	                        "\x03\x00"
	                        "\x04\x00\x00\x00"
	                        "\x05\x00\x00\x00"
	                        "\x06\x00\x00\x00\x00\x00\x00\x00"
	                        "\x07\x00\x00\x00\x00\x00\x00\x00"
	                        "\x00\x00\x00\x41"
	                        "\x00\x00\x00\x00\x00\x00\x22\x40"
	                        "10\x00"
	                        "\x01"
	                        "\x0C\x00\x00\x00"
	                        "\x01"
	                        "\x01\x00"
	                        "\x01\x00\x00\x00"
	                        "\x01\x00\x00\x00\x00\x00\x00\x00", length);
	free(bytes);
}

static void testStructures() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "key");
		context->beginStructure(context, "item");
		context->endStructure(context);
	context->endStructure(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 4, "Expected 4 but got %zu", length);
	assertBytesMatch(bytes, "Stem", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "key");
		context->beginStructure(context, "item");
		context->endStructure(context);
	context->endStructure(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 4, "Expected 4 but got %zu", length);
	assertBytesMatch(bytes, "metS", length);
	free(bytes);
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "key");
		context->beginStructure(context, "struct1");
			context->writeInt8(context, "int8", 0);
		context->endStructure(context);
		context->beginStructure(context, "struct2");
			context->writeUInt8(context, "uint8", 1);
			context->beginStructure(context, "struct3");
				context->writeInt16(context, "int16", 2);
				context->writeUInt16(context, "uint16", 3);
				context->writeInt32(context, "int32", 4);
				context->writeUInt32(context, "uint32", 5);
				context->writeInt64(context, "int64", 6);
				context->writeUInt64(context, "uint64", 7);
			context->endStructure(context);
			context->writeFloat(context, "float", 8);
			context->writeDouble(context, "double", 9);
			context->writeString(context, "string", "10");
			context->writeBoolean(context, "boolean", true);
			context->writeEnumeration(context, "enumeration", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "bitfield8", 1, "13", NULL);
			context->writeBitfield16(context, "bitfield16", 1, "14", NULL);
			context->writeBitfield32(context, "bitfield32", 1, "15", NULL);
			context->writeBitfield64(context, "bitfield64", 1, "16", NULL);
		context->endStructure(context);
	context->endStructure(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 69, "Expected 69 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00"
	                        "\x01"
	                        "\x00\x02"
	                        "\x00\x03"
	                        "\x00\x00\x00\x04"
	                        "\x00\x00\x00\x05"
	                        "\x00\x00\x00\x00\x00\x00\x00\x06"
	                        "\x00\x00\x00\x00\x00\x00\x00\x07"
	                        "\x41\x00\x00\x00"
	                        "\x40\x22\x00\x00\x00\x00\x00\x00"
	                        "10\x00"
	                        "\x01"
	                        "\x00\x00\x00\x0C"
	                        "\x01"
	                        "\x00\x01"
	                        "\x00\x00\x00\x01"
	                        "\x00\x00\x00\x00\x00\x00\x00\x01", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginStructure(context, "key");
		context->beginStructure(context, "struct1");
			context->writeInt8(context, "int8", 0);
		context->endStructure(context);
		context->beginStructure(context, "struct2");
			context->writeUInt8(context, "uint8", 1);
			context->beginStructure(context, "struct3");
				context->writeInt16(context, "int16", 2);
				context->writeUInt16(context, "uint16", 3);
				context->writeInt32(context, "int32", 4);
				context->writeUInt32(context, "uint32", 5);
				context->writeInt64(context, "int64", 6);
				context->writeUInt64(context, "uint64", 7);
			context->endStructure(context);
			context->writeFloat(context, "float", 8);
			context->writeDouble(context, "double", 9);
			context->writeString(context, "string", "10");
			context->writeBoolean(context, "boolean", true);
			context->writeEnumeration(context, "enumeration", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "bitfield8", 1, "13", NULL);
			context->writeBitfield16(context, "bitfield16", 1, "14", NULL);
			context->writeBitfield32(context, "bitfield32", 1, "15", NULL);
			context->writeBitfield64(context, "bitfield64", 1, "16", NULL);
		context->endStructure(context);
	context->endStructure(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 69, "Expected 69 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x00"
	                        "\x01"
	                        "\x02\x00"
	                        "\x03\x00"
	                        "\x04\x00\x00\x00"
	                        "\x05\x00\x00\x00"
	                        "\x06\x00\x00\x00\x00\x00\x00\x00"
	                        "\x07\x00\x00\x00\x00\x00\x00\x00"
	                        "\x00\x00\x00\x41"
	                        "\x00\x00\x00\x00\x00\x00\x22\x40"
	                        "10\x00"
	                        "\x01"
	                        "\x0C\x00\x00\x00"
	                        "\x01"
	                        "\x01\x00"
	                        "\x01\x00\x00\x00"
	                        "\x01\x00\x00\x00\x00\x00\x00\x00", length);
	free(bytes);
}

static void testDictionaries() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "key");
		context->beginDictionary(context, "item");
		context->endDictionary(context);
	context->endDictionary(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 29, "Expected 29 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x01\x00\x00\x00\x19"
	                        "item\x00"
	                        "\x00\x00\x00\x11"
	                        "\x00\x00\x00\x00\x00\x00\x00\x08", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "key");
		context->beginDictionary(context, "item");
		context->endDictionary(context);
	context->endDictionary(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 29, "Expected 29 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x01\x00\x00\x00\x19\x00\x00\x00"
	                        "item\x00"
	                        "\x11\x00\x00\x00"
	                        "\x00\x00\x00\x00\x08\x00\x00\x00", length);
	free(bytes);
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "key");
		context->beginDictionary(context, "struct1");
			context->writeInt8(context, "int8", 0);
		context->endDictionary(context);
		context->beginDictionary(context, "struct2");
			context->writeUInt8(context, "uint8", 1);
			context->beginDictionary(context, "struct3");
				context->writeInt16(context, "int16", 2);
				context->writeUInt16(context, "uint16", 3);
				context->writeInt32(context, "int32", 4);
				context->writeUInt32(context, "uint32", 5);
				context->writeInt64(context, "int64", 6);
				context->writeUInt64(context, "uint64", 7);
			context->endDictionary(context);
			context->writeFloat(context, "float", 8);
			context->writeDouble(context, "double", 9);
			context->writeString(context, "string", "10");
			context->writeBoolean(context, "boolean", true);
			context->writeEnumeration(context, "enumeration", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "bitfield8", 1, "13", NULL);
			context->writeBitfield16(context, "bitfield16", 1, "14", NULL);
			context->writeBitfield32(context, "bitfield32", 1, "15", NULL);
			context->writeBitfield64(context, "bitfield64", 1, "16", NULL);
		context->endDictionary(context);
	context->endDictionary(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 338, "Expected 338 but got %zu", length);
	assertBytesMatch(bytes, "Stem" // 4
	                        "\x00\x00\x00\x02\x00\x00\x01\x4E" // 12
	                        "struct1\x00" // 20
	                        "\x00\x00\x00\x20" // 24
	                        "struct2\x00" // 32
	                        "\x00\x00\x00\x32" // 36
	                        "\x00\x00\x00\x01\x00\x00\x00\x12" // 44
	                        "int8\x00" // 49
	                        "\x00\x00\x00\x11" // 53
	                        "\x00" // 54
	                        "\x00\x00\x00\x0B\x00\x00\x01\x1C" // 62
	                        "uint8\x00" // 68
	                        "\x00\x00\x00\x95" // 72
	                        "struct3\x00" // 80
	                        "\x00\x00\x00\x96" // 84
	                        "float\x00" // 90
	                        "\x00\x00\x00\xF9" // 94
	                        "double\x00" // 101
	                        "\x00\x00\x00\xFD" // 105
	                        "string\x00" // 112
	                        "\x00\x00\x01\x05" // 116
	                        "boolean\x00" // 124
	                        "\x00\x00\x01\x08" // 128
	                        "enumeration\x00" // 140
	                        "\x00\x00\x01\x09" // 144
	                        "bitfield8\x00" // 154
	                        "\x00\x00\x01\x0D" // 158
	                        "bitfield16\x00" // 169
	                        "\x00\x00\x01\x0E" // 173
	                        "bitfield32\x00" // 184
	                        "\x00\x00\x01\x10" // 188
	                        "bitfield64\x00" // 199
	                        "\x00\x00\x01\x14" // 203
	                        "\x01" // 204
	                        "\x00\x00\x00\x06\x00\x00\x00\x63" // 212
	                        "int16\x00" // 218
	                        "\x00\x00\x00\x47" // 222
	                        "uint16\x00" // 229
	                        "\x00\x00\x00\x49" // 233
	                        "int32\x00" // 239
	                        "\x00\x00\x00\x4B" // 243
	                        "uint32\x00" // 250
	                        "\x00\x00\x00\x4F" // 254
	                        "int64\x00" // 260
	                        "\x00\x00\x00\x53" // 264
	                        "uint64\x00" // 271
	                        "\x00\x00\x00\x5B" // 275
	                        "\x00\x02" // 277
	                        "\x00\x03" // 279
	                        "\x00\x00\x00\x04" // 283
	                        "\x00\x00\x00\x05" // 287
	                        "\x00\x00\x00\x00\x00\x00\x00\x06" // 295
	                        "\x00\x00\x00\x00\x00\x00\x00\x07" // 303
	                        "\x41\x00\x00\x00" // 307
	                        "\x40\x22\x00\x00\x00\x00\x00\x00" // 315
	                        "10\x00" // 318
	                        "\x01" // 319
	                        "\x00\x00\x00\x0C" // 323
	                        "\x01" // 324
	                        "\x00\x01" // 326
	                        "\x00\x00\x00\x01" // 330
	                        "\x00\x00\x00\x00\x00\x00\x00\x01", length); // 338
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginDictionary(context, "key");
		context->beginDictionary(context, "struct1");
			context->writeInt8(context, "int8", 0);
		context->endDictionary(context);
		context->beginDictionary(context, "struct2");
			context->writeUInt8(context, "uint8", 1);
			context->beginDictionary(context, "struct3");
				context->writeInt16(context, "int16", 2);
				context->writeUInt16(context, "uint16", 3);
				context->writeInt32(context, "int32", 4);
				context->writeUInt32(context, "uint32", 5);
				context->writeInt64(context, "int64", 6);
				context->writeUInt64(context, "uint64", 7);
			context->endDictionary(context);
			context->writeFloat(context, "float", 8);
			context->writeDouble(context, "double", 9);
			context->writeString(context, "string", "10");
			context->writeBoolean(context, "boolean", true);
			context->writeEnumeration(context, "enumeration", 12, "enum", 12, NULL);
			context->writeBitfield8(context, "bitfield8", 1, "13", NULL);
			context->writeBitfield16(context, "bitfield16", 1, "14", NULL);
			context->writeBitfield32(context, "bitfield32", 1, "15", NULL);
			context->writeBitfield64(context, "bitfield64", 1, "16", NULL);
		context->endDictionary(context);
	context->endDictionary(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 338, "Expected 338 but got %zu", length);
	assertBytesMatch(bytes, "metS" // 4
	                        "\x02\x00\x00\x00\x4E\x01\x00\x00" // 12
	                        "struct1\x00" // 20
	                        "\x20\x00\x00\x00" // 24
	                        "struct2\x00" // 32
	                        "\x32\x00\x00\x00" // 36
	                        "\x01\x00\x00\x00\x12\x00\x00\x00" // 44
	                        "int8\x00" // 49
	                        "\x11\x00\x00\x00" // 53
	                        "\x00" // 54
	                        "\x0B\x00\x00\x00\x1C\x01\x00\x00" // 62
	                        "uint8\x00" // 68
	                        "\x95\x00\x00\x00" // 72
	                        "struct3\x00" // 80
	                        "\x96\x00\x00\x00" // 84
	                        "float\x00" // 90
	                        "\xF9\x00\x00\x00" // 94
	                        "double\x00" // 101
	                        "\xFD\x00\x00\x00" // 105
	                        "string\x00" // 112
	                        "\x05\x01\x00\x00" // 116
	                        "boolean\x00" // 124
	                        "\x08\x01\x00\x00" // 128
	                        "enumeration\x00" // 140
	                        "\x09\x01\x00\x00" // 144
	                        "bitfield8\x00" // 154
	                        "\x0D\x01\x00\x00" // 158
	                        "bitfield16\x00" // 169
	                        "\x0E\x01\x00\x00" // 173
	                        "bitfield32\x00" // 184
	                        "\x10\x01\x00\x00" // 188
	                        "bitfield64\x00" // 199
	                        "\x14\x01\x00\x00" // 203
	                        "\x01" // 204
	                        "\x06\x00\x00\x00\x63\x00\x00\x00" // 212
	                        "int16\x00" // 218
	                        "\x47\x00\x00\x00" // 222
	                        "uint16\x00" // 229
	                        "\x49\x00\x00\x00" // 233
	                        "int32\x00" // 239
	                        "\x4B\x00\x00\x00" // 243
	                        "uint32\x00" // 250
	                        "\x4F\x00\x00\x00" // 254
	                        "int64\x00" // 260
	                        "\x53\x00\x00\x00" // 264
	                        "uint64\x00" // 271
	                        "\x5B\x00\x00\x00" // 275
	                        "\x02\x00" // 277
	                        "\x03\x00" // 279
	                        "\x04\x00\x00\x00" // 283
	                        "\x05\x00\x00\x00" // 287
	                        "\x06\x00\x00\x00\x00\x00\x00\x00" // 295
	                        "\x07\x00\x00\x00\x00\x00\x00\x00" // 303
	                        "\x00\x00\x00\x41" // 307
	                        "\x00\x00\x00\x00\x00\x00\x22\x40" // 315
	                        "10\x00" // 318
	                        "\x01" // 319
	                        "\x0C\x00\x00\x00" // 323
	                        "\x01" // 324
	                        "\x01\x00" // 326
	                        "\x01\x00\x00\x00" // 330
	                        "\x01\x00\x00\x00\x00\x00\x00\x00", length); // 338
	free(bytes);
}

static void testMixedContainers() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "outerArray");
		context->beginStructure(context, "outerStruct");
			context->beginArray(context, "innerArray");
			context->endArray(context);
			context->beginDictionary(context, "innerDict");
			context->endDictionary(context);
		context->endStructure(context);
		context->beginDictionary(context, "outerDict");
			context->beginArray(context, "innerArray2");
			context->endArray(context);
			context->beginStructure(context, "innerStruct");
			context->endStructure(context);
		context->endDictionary(context);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 64, "Expected 64 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x02"
	                        "\x00\x00\x00\x00"
	                        "\x00\x00\x00\x00\x00\x00\x00\x08"
	                        "\x00\x00\x00\x02\x00\x00\x00\x2C"
	                        "innerArray2\x00"
	                        "\x00\x00\x00\x28"
	                        "innerStruct\x00"
	                        "\x00\x00\x00\x2C"
	                        "\x00\x00\x00\x00", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "outerArray");
		context->beginStructure(context, "outerStruct");
			context->beginArray(context, "innerArray");
			context->endArray(context);
			context->beginDictionary(context, "innerDict");
			context->endDictionary(context);
		context->endStructure(context);
		context->beginDictionary(context, "outerDict");
			context->beginArray(context, "innerArray2");
			context->endArray(context);
			context->beginStructure(context, "innerStruct");
			context->endStructure(context);
		context->endDictionary(context);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	bytes = context->writeToBytes(context, &length);
	context->dispose(context);
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	if (bytes == NULL) {return;} // Suppress clang warning
	TestCase_assert(length == 64, "Expected 64 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x02\x00\x00\x00"
	                        "\x00\x00\x00\x00"
	                        "\x00\x00\x00\x00\x08\x00\x00\x00"
	                        "\x02\x00\x00\x00\x2C\x00\x00\x00"
	                        "innerArray2\x00"
	                        "\x28\x00\x00\x00"
	                        "innerStruct\x00"
	                        "\x2C\x00\x00\x00"
	                        "\x00\x00\x00\x00", length);
	free(bytes);
}

static void testWriteToFile() {
	BinarySerializationContext * context;
	const char * tempFilePath;
	int fd;
	bool success;
	void * bytes;
	size_t length;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt32(context, "item", 10);
	context->beginStructure(context, "item");
	context->writeBoolean(context, "bool", false);
	context->writeString(context, "string", "Hello, world!");
	context->endStructure(context);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	success = context->writeToFile(context, tempFilePath);
	context->dispose(context);
	bytes = readFileSimple(tempFilePath, &length);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 27, "Expected 27 but got %zu", length);
	assertBytesMatch(bytes, "Stem"
	                        "\x00\x00\x00\x02"
	                        "\x00\x00\x00\x0A"
	                        "\x00"
	                        "Hello, world!\x00", length);
	free(bytes);
	
	context = BinarySerializationContext_create(false);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	context->beginArray(context, "key");
	context->writeInt32(context, "item", 10);
	context->beginStructure(context, "item");
	context->writeBoolean(context, "bool", false);
	context->writeString(context, "string", "Hello, world!");
	context->endStructure(context);
	context->endArray(context);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Unexpected error %d", context->status);
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	success = context->writeToFile(context, tempFilePath);
	context->dispose(context);
	bytes = readFileSimple(tempFilePath, &length);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(bytes != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(length == 27, "Expected 27 but got %zu", length);
	assertBytesMatch(bytes, "metS"
	                        "\x02\x00\x00\x00"
	                        "\x0A\x00\x00\x00"
	                        "\x00"
	                        "Hello, world!\x00", length);
	free(bytes);
}

static void testInvalidOperations() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	
	// No data
	context = BinarySerializationContext_create(true);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// No top-level container
#define _testNoTopLevelContainer(FUNCTION, ...) \
	context = BinarySerializationContext_create(true); \
	context->FUNCTION(context, "key", __VA_ARGS__); \
	bytes = context->writeToBytes(context, &length); \
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes); \
	context->dispose(context);
	
	_testNoTopLevelContainer(writeInt8, 0)
	_testNoTopLevelContainer(writeUInt8, 0)
	_testNoTopLevelContainer(writeInt16, 0)
	_testNoTopLevelContainer(writeUInt16, 0)
	_testNoTopLevelContainer(writeInt32, 0)
	_testNoTopLevelContainer(writeUInt32, 0)
	_testNoTopLevelContainer(writeInt64, 0)
	_testNoTopLevelContainer(writeUInt64, 0)
	_testNoTopLevelContainer(writeFloat, 0)
	_testNoTopLevelContainer(writeDouble, 0)
	_testNoTopLevelContainer(writeString, "")
	_testNoTopLevelContainer(writeBoolean, false)
	_testNoTopLevelContainer(writeEnumeration, 0, NULL)
	_testNoTopLevelContainer(writeBitfield8, 0, NULL)
	_testNoTopLevelContainer(writeBitfield16, 0, NULL)
	_testNoTopLevelContainer(writeBitfield32, 0, NULL)
	_testNoTopLevelContainer(writeBitfield64, 0, NULL)
#undef _testNoTopLevelContainer
	
	// Unterminated array
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->beginArray(context, "key");
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// Unterminated structure
	context = BinarySerializationContext_create(true);
	context->beginStructure(context, "key");
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginStructure(context, "key");
	context->beginStructure(context, "key");
	context->endStructure(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// Unterminated dictionary
	context = BinarySerializationContext_create(true);
	context->beginDictionary(context, "key");
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginDictionary(context, "key");
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// Wrong container type terminated
#define _testWrongContainerTypeTerminatedOuter(CONTAINER_1, CONTAINER_2) \
	context = BinarySerializationContext_create(true); \
	context->begin##CONTAINER_1(context, "key"); \
	context->end##CONTAINER_2(context); \
	bytes = context->writeToBytes(context, &length); \
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes); \
	context->dispose(context);
#define _testWrongContainerTypeTerminatedInner(CONTAINER_1, CONTAINER_2) \
	context = BinarySerializationContext_create(true); \
	context->begin##CONTAINER_1(context, "key"); \
	context->begin##CONTAINER_1(context, "key"); \
	context->end##CONTAINER_2(context); \
	context->end##CONTAINER_1(context); \
	bytes = context->writeToBytes(context, &length); \
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes); \
	context->dispose(context);
	
	_testWrongContainerTypeTerminatedOuter(Array, Structure)
	_testWrongContainerTypeTerminatedOuter(Array, Dictionary)
	_testWrongContainerTypeTerminatedOuter(Structure, Array)
	_testWrongContainerTypeTerminatedOuter(Structure, Dictionary)
	_testWrongContainerTypeTerminatedOuter(Dictionary, Array)
	_testWrongContainerTypeTerminatedOuter(Dictionary, Structure)
	_testWrongContainerTypeTerminatedInner(Array, Structure)
	_testWrongContainerTypeTerminatedInner(Array, Dictionary)
	_testWrongContainerTypeTerminatedInner(Structure, Array)
	_testWrongContainerTypeTerminatedInner(Structure, Dictionary)
	_testWrongContainerTypeTerminatedInner(Dictionary, Array)
	_testWrongContainerTypeTerminatedInner(Dictionary, Structure)
#undef _testWrongContainerTypeTerminatedOuter
#undef _testWrongContainerTypeTerminatedInner
	
	// Container underflow
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->endArray(context);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginStructure(context, "key");
	context->endStructure(context);
	context->endStructure(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	context->endDictionary(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// Second top-level container started
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->endArray(context);
	context->beginArray(context, "key");
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginStructure(context, "key");
	context->endStructure(context);
	context->beginStructure(context, "key");
	context->endStructure(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	context->beginDictionary(context, "key");
	context->endDictionary(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// Bit set in bitfield but list terminated before naming bit
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeBitfield8(context, "item", 0x01, NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeBitfield16(context, "item", 0x0002, "bit0", NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeBitfield32(context, "item", 0x00000004, "bit0", "bit1", NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeBitfield64(context, "item", 0x0000000000000008ull, "bit0", "bit1", "bit2", NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// Bit names duplicated
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeBitfield8(context, "item", 0x00, "bit0", "bit0", NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeBitfield16(context, "item", 0x0000, "bit0", "bit0", NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeBitfield32(context, "item", 0x00000000, "bit0", "bit0", NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeBitfield64(context, "item", 0x0000000000000000ull, "bit0", "bit0", NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// Enum value not listed in args
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum", 1, NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// Duplicate enum values
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum0", 0, "enum1", 0, NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum0", 0, "enum1", 1, "enum2", 1, NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	// Duplicate enum names
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum", 0, "enum", 1, NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	context->beginArray(context, "key");
	context->writeEnumeration(context, "item", 0, "enum0", 0, "enum1", 1, "enum1", 2, NULL);
	context->endArray(context);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	context->dispose(context);
	
#define _testNullObjectKey(CONTAINER, FUNCTION, ...) \
	context = BinarySerializationContext_create(true); \
	context->begin##CONTAINER(context, "key"); \
	context->FUNCTION(context, NULL, __VA_ARGS__); \
	context->end##CONTAINER(context); \
	bytes = context->writeToBytes(context, &length); \
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes); \
	context->dispose(context);
	
#define _testNullObjectKeyNoArgs(CONTAINER, FUNCTION, ...) \
	context = BinarySerializationContext_create(true); \
	context->begin##CONTAINER(context, "key"); \
	context->FUNCTION(context, NULL); \
	context->end##CONTAINER(context); \
	bytes = context->writeToBytes(context, &length); \
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes); \
	context->dispose(context);
	
	// Null structure key
	_testNullObjectKeyNoArgs(Structure, beginArray)
	_testNullObjectKeyNoArgs(Structure, beginStructure)
	_testNullObjectKeyNoArgs(Structure, beginDictionary)
	_testNullObjectKey(Structure, writeInt8, 0)
	_testNullObjectKey(Structure, writeUInt8, 0)
	_testNullObjectKey(Structure, writeInt16, 0)
	_testNullObjectKey(Structure, writeUInt16, 0)
	_testNullObjectKey(Structure, writeInt32, 0)
	_testNullObjectKey(Structure, writeUInt32, 0)
	_testNullObjectKey(Structure, writeInt64, 0)
	_testNullObjectKey(Structure, writeUInt64, 0)
	_testNullObjectKey(Structure, writeFloat, 0)
	_testNullObjectKey(Structure, writeDouble, 0)
	_testNullObjectKey(Structure, writeString, "")
	_testNullObjectKey(Structure, writeBoolean, false)
	_testNullObjectKey(Structure, writeEnumeration, 0, "enum", 0, NULL)
	_testNullObjectKey(Structure, writeBitfield8, 0, NULL)
	_testNullObjectKey(Structure, writeBitfield16, 0, NULL)
	_testNullObjectKey(Structure, writeBitfield32, 0, NULL)
	_testNullObjectKey(Structure, writeBitfield64, 0, NULL)
	
	// Null dictionary key
	_testNullObjectKeyNoArgs(Dictionary, beginArray)
	_testNullObjectKeyNoArgs(Dictionary, beginStructure)
	_testNullObjectKeyNoArgs(Dictionary, beginDictionary)
	_testNullObjectKey(Dictionary, writeInt8, 0)
	_testNullObjectKey(Dictionary, writeUInt8, 0)
	_testNullObjectKey(Dictionary, writeInt16, 0)
	_testNullObjectKey(Dictionary, writeUInt16, 0)
	_testNullObjectKey(Dictionary, writeInt32, 0)
	_testNullObjectKey(Dictionary, writeUInt32, 0)
	_testNullObjectKey(Dictionary, writeInt64, 0)
	_testNullObjectKey(Dictionary, writeUInt64, 0)
	_testNullObjectKey(Dictionary, writeFloat, 0)
	_testNullObjectKey(Dictionary, writeDouble, 0)
	_testNullObjectKey(Dictionary, writeString, "")
	_testNullObjectKey(Dictionary, writeBoolean, false)
	_testNullObjectKey(Dictionary, writeEnumeration, 0, "enum", 0, NULL)
	_testNullObjectKey(Dictionary, writeBitfield8, 0, NULL)
	_testNullObjectKey(Dictionary, writeBitfield16, 0, NULL)
	_testNullObjectKey(Dictionary, writeBitfield32, 0, NULL)
	_testNullObjectKey(Dictionary, writeBitfield64, 0, NULL)
#undef _testNullObjectKey
#undef _testNullObjectKeyNoArgs
}

static void testErrorReporting() {
	BinarySerializationContext * context;
	void * bytes;
	size_t length;
	const char * tempFilePath;
	int fd;
	bool success;
	jmp_buf jmpBuf;
	int status;
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	bytes = context->writeToBytes(context, &length);
	TestCase_assert(bytes == NULL, "Expected NULL but got %p", bytes);
	TestCase_assert(context->status == BINARY_SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, context->status);
	context->dispose(context);
	
	context = BinarySerializationContext_create(true);
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	success = context->writeToFile(context, tempFilePath);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(!success, "Expected false but got true");
	TestCase_assert(context->status == BINARY_SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER, context->status);
	context->dispose(context);
	
#define _testFailure(ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	context = BinarySerializationContext_create(true); \
	PREAMBLE_CODE \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d (OK)", SERIALIZATION_ERROR_OK, context->status); \
	FAIL_CODE \
	TestCase_assert(context->status == ERROR_STATUS, "Expected %d but got %d (context->status)", ERROR_STATUS, context->status); \
	context->dispose(context); \
	\
	context = BinarySerializationContext_create(true); \
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
	
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeInt8(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeUInt8(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeInt16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeUInt16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeInt32(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeUInt32(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeInt64(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeUInt64(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeFloat(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeDouble(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeString(context, "key", "");)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBoolean(context, "key", false);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeEnumeration(context, "key", 0, "", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBitfield8(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBitfield16(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBitfield32(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->writeBitfield64(context, "key", 0, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeInt8(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeUInt8(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeInt16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeUInt16(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeInt32(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeUInt32(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeInt64(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeUInt64(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeFloat(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeDouble(context, "key", 0);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeString(context, "key", "");)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBoolean(context, "key", false);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeEnumeration(context, "key", 0, "", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBitfield8(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBitfield16(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBitfield32(context, "key", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginArray(context, ""); context->endArray(context);, context->writeBitfield64(context, "key", 0, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "key");,
	             context->endStructure(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "key");
	             context->beginArray(context, "key");,
	             context->endStructure(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "key");,
	             context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "key");
	             context->beginArray(context, "key");,
	             context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "key");,
	             context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "key");
	             context->beginStructure(context, "key");,
	             context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "key");,
	             context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "key");
	             context->beginStructure(context, "key");,
	             context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "key");,
	             context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "key");
	             context->beginDictionary(context, "key");,
	             context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "key");,
	             context->endStructure(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "key");
	             context->beginDictionary(context, "key");,
	             context->endStructure(context);)
	
	_testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, , context->endArray(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, , context->endDictionary(context);)
	_testFailure(SERIALIZATION_ERROR_CONTAINER_UNDERFLOW, , context->endStructure(context);)
	
	_testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginArray(context, "key");
	             context->endArray(context);,
	             context->beginArray(context, "key");)
	_testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginStructure(context, "key");
	             context->endStructure(context);,
	             context->beginStructure(context, "key");)
	_testFailure(SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginDictionary(context, "key");
	             context->endDictionary(context);,
	             context->beginDictionary(context, "key");)
	
	_testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield8(context, "key", 0x08, "bit0", "bit1", "bit2", NULL);)
	_testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield16(context, "key", 0x0004, "bit0", "bit1", NULL);)
	_testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield32(context, "key", 0x00000002, "bit0", NULL);)
	_testFailure(SERIALIZATION_ERROR_UNNAMED_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield64(context, "key", 0x0000000000000001ull, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield8(context, "key", 0, "bit0", "bit0", "bit1", NULL);)
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield16(context, "key", 0, "bit0", "bit1", "bit0", NULL);)
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield32(context, "key", 0, "bit0", "bit1", "bit1", NULL);)
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_BIT,
	             context->beginArray(context, "key");,
	             context->writeBitfield64(context, "key", 0, "bit0", "bit0", NULL);)
	
	_testFailure(SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	             context->beginArray(context, "key");,
	             context->writeEnumeration(context, "key", 0, "", 1, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE,
	             context->beginArray(context, "key");,
	             context->writeEnumeration(context, "key", 0, "enum0", 0, "enum1", 0, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME,
	             context->beginArray(context, "key");,
	             context->writeEnumeration(context, "key", 0, "enum0", 0, "enum0", 1, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeInt8(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeUInt8(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeInt16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeUInt16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeInt32(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeUInt32(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeInt64(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeUInt64(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeFloat(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeDouble(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeString(context, NULL, "");)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBoolean(context, NULL, false);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeEnumeration(context, NULL, 0, "", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBitfield8(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBitfield16(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBitfield32(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginDictionary(context, "key");,
	             context->writeBitfield64(context, NULL, 0, NULL);)
	
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeInt8(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeUInt8(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeInt16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeUInt16(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeInt32(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeUInt32(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeInt64(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeUInt64(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeFloat(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeDouble(context, NULL, 0);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeString(context, NULL, "");)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBoolean(context, NULL, false);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeEnumeration(context, NULL, 0, "", 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBitfield8(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBitfield16(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBitfield32(context, NULL, 0, NULL);)
	_testFailure(SERIALIZATION_ERROR_NULL_KEY,
	             context->beginStructure(context, "key");,
	             context->writeBitfield64(context, NULL, 0, NULL);)
	
#undef _testFailure
}

TEST_SUITE(BinarySerializationContextTest,
           testInit,
           testTopLevelContainers,
           testNumberValues,
           testStringValues,
           testBooleanValues,
           testEnumerations,
           testBitfields,
           testArrays,
           testStructures,
           testDictionaries,
           testMixedContainers,
           testWriteToFile,
           testInvalidOperations,
           testErrorReporting)
