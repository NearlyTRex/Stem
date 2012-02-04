#include "unittest/framework/TestSuite.h"
#include "binaryserialization/BinaryDeserializationContext.h"
#include "utilities/IOUtilities.h"
#include <float.h>
#include <string.h>
#include <unistd.h>

static void testInit() {
	BinaryDeserializationContext context, * contextPtr;
	const char * tempFilePath;
	int fd;
	
	memset(&context, 0, sizeof(context));
	context.status = -1;
	context.jmpBuf = (void *) -1;
	BinaryDeserializationContext_initWithBytes(&context, "Stem", 4);
	TestCase_assert(context.jmpBuf == NULL, "Expected NULL but got %p", context.jmpBuf);
	TestCase_assert(context.status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context.status);
	TestCase_assert(context.bigEndian, "Expected true but got false");
	TestCase_assert(context.dispose == BinaryDeserializationContext_dispose, "Expected %p but got %p", BinaryDeserializationContext_dispose, context.dispose);
	TestCase_assert(context.beginStructure == BinaryDeserializationContext_beginStructure, "Expected %p but got %p", BinaryDeserializationContext_beginStructure, context.beginStructure);
	TestCase_assert(context.beginDictionary == BinaryDeserializationContext_beginDictionary, "Expected %p but got %p", BinaryDeserializationContext_beginDictionary, context.beginDictionary);
	TestCase_assert(context.beginArray == BinaryDeserializationContext_beginArray, "Expected %p but got %p", BinaryDeserializationContext_beginArray, context.beginArray);
	TestCase_assert(context.endStructure == BinaryDeserializationContext_endStructure, "Expected %p but got %p", BinaryDeserializationContext_endStructure, context.endStructure);
	TestCase_assert(context.endDictionary == BinaryDeserializationContext_endDictionary, "Expected %p but got %p", BinaryDeserializationContext_endDictionary, context.endDictionary);
	TestCase_assert(context.endArray == BinaryDeserializationContext_endArray, "Expected %p but got %p", BinaryDeserializationContext_endArray, context.endArray);
	TestCase_assert(context.readInt8 == BinaryDeserializationContext_readInt8, "Expected %p but got %p", BinaryDeserializationContext_readInt8, context.readInt8);
	TestCase_assert(context.readUInt8 == BinaryDeserializationContext_readUInt8, "Expected %p but got %p", BinaryDeserializationContext_readUInt8, context.readUInt8);
	TestCase_assert(context.readInt16 == BinaryDeserializationContext_readInt16, "Expected %p but got %p", BinaryDeserializationContext_readInt16, context.readInt16);
	TestCase_assert(context.readUInt16 == BinaryDeserializationContext_readUInt16, "Expected %p but got %p", BinaryDeserializationContext_readUInt16, context.readUInt16);
	TestCase_assert(context.readInt32 == BinaryDeserializationContext_readInt32, "Expected %p but got %p", BinaryDeserializationContext_readInt32, context.readInt32);
	TestCase_assert(context.readUInt32 == BinaryDeserializationContext_readUInt32, "Expected %p but got %p", BinaryDeserializationContext_readUInt32, context.readUInt32);
	TestCase_assert(context.readInt64 == BinaryDeserializationContext_readInt64, "Expected %p but got %p", BinaryDeserializationContext_readInt64, context.readInt64);
	TestCase_assert(context.readUInt64 == BinaryDeserializationContext_readUInt64, "Expected %p but got %p", BinaryDeserializationContext_readUInt64, context.readUInt64);
	TestCase_assert(context.readFloat == BinaryDeserializationContext_readFloat, "Expected %p but got %p", BinaryDeserializationContext_readFloat, context.readFloat);
	TestCase_assert(context.readDouble == BinaryDeserializationContext_readDouble, "Expected %p but got %p", BinaryDeserializationContext_readDouble, context.readDouble);
	TestCase_assert(context.readString == BinaryDeserializationContext_readString, "Expected %p but got %p", BinaryDeserializationContext_readString, context.readString);
	TestCase_assert(context.readBoolean == BinaryDeserializationContext_readBoolean, "Expected %p but got %p", BinaryDeserializationContext_readBoolean, context.readBoolean);
	TestCase_assert(context.readEnumeration == BinaryDeserializationContext_readEnumeration, "Expected %p but got %p", BinaryDeserializationContext_readEnumeration, context.readEnumeration);
	TestCase_assert(context.readBitfield8 == BinaryDeserializationContext_readBitfield8, "Expected %p but got %p", BinaryDeserializationContext_readBitfield8, context.readBitfield8);
	TestCase_assert(context.readBitfield16 == BinaryDeserializationContext_readBitfield16, "Expected %p but got %p", BinaryDeserializationContext_readBitfield16, context.readBitfield16);
	TestCase_assert(context.readBitfield32 == BinaryDeserializationContext_readBitfield32, "Expected %p but got %p", BinaryDeserializationContext_readBitfield32, context.readBitfield32);
	TestCase_assert(context.readBitfield64 == BinaryDeserializationContext_readBitfield64, "Expected %p but got %p", BinaryDeserializationContext_readBitfield64, context.readBitfield64);
	TestCase_assert(context.readNextDictionaryKey == BinaryDeserializationContext_readNextDictionaryKey, "Expected %p but got %p", BinaryDeserializationContext_readNextDictionaryKey, context.readNextDictionaryKey);
	TestCase_assert(context.hasDictionaryKey == BinaryDeserializationContext_hasDictionaryKey, "Expected %p but got %p", BinaryDeserializationContext_hasDictionaryKey, context.hasDictionaryKey);
	context.dispose(&context);
	
	memset(&context, 0, sizeof(context));
	context.status = -1;
	context.jmpBuf = (void *) -1;
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	writeFileSimple(tempFilePath, "metS", 4);
	BinaryDeserializationContext_initWithFile(&context, tempFilePath);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(context.jmpBuf == NULL, "Expected NULL but got %p", context.jmpBuf);
	TestCase_assert(context.status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context.status);
	TestCase_assert(!context.bigEndian, "Expected false but got true");
	TestCase_assert(context.dispose == BinaryDeserializationContext_dispose, "Expected %p but got %p", BinaryDeserializationContext_dispose, context.dispose);
	TestCase_assert(context.beginStructure == BinaryDeserializationContext_beginStructure, "Expected %p but got %p", BinaryDeserializationContext_beginStructure, context.beginStructure);
	TestCase_assert(context.beginDictionary == BinaryDeserializationContext_beginDictionary, "Expected %p but got %p", BinaryDeserializationContext_beginDictionary, context.beginDictionary);
	TestCase_assert(context.beginArray == BinaryDeserializationContext_beginArray, "Expected %p but got %p", BinaryDeserializationContext_beginArray, context.beginArray);
	TestCase_assert(context.endStructure == BinaryDeserializationContext_endStructure, "Expected %p but got %p", BinaryDeserializationContext_endStructure, context.endStructure);
	TestCase_assert(context.endDictionary == BinaryDeserializationContext_endDictionary, "Expected %p but got %p", BinaryDeserializationContext_endDictionary, context.endDictionary);
	TestCase_assert(context.endArray == BinaryDeserializationContext_endArray, "Expected %p but got %p", BinaryDeserializationContext_endArray, context.endArray);
	TestCase_assert(context.readInt8 == BinaryDeserializationContext_readInt8, "Expected %p but got %p", BinaryDeserializationContext_readInt8, context.readInt8);
	TestCase_assert(context.readUInt8 == BinaryDeserializationContext_readUInt8, "Expected %p but got %p", BinaryDeserializationContext_readUInt8, context.readUInt8);
	TestCase_assert(context.readInt16 == BinaryDeserializationContext_readInt16, "Expected %p but got %p", BinaryDeserializationContext_readInt16, context.readInt16);
	TestCase_assert(context.readUInt16 == BinaryDeserializationContext_readUInt16, "Expected %p but got %p", BinaryDeserializationContext_readUInt16, context.readUInt16);
	TestCase_assert(context.readInt32 == BinaryDeserializationContext_readInt32, "Expected %p but got %p", BinaryDeserializationContext_readInt32, context.readInt32);
	TestCase_assert(context.readUInt32 == BinaryDeserializationContext_readUInt32, "Expected %p but got %p", BinaryDeserializationContext_readUInt32, context.readUInt32);
	TestCase_assert(context.readInt64 == BinaryDeserializationContext_readInt64, "Expected %p but got %p", BinaryDeserializationContext_readInt64, context.readInt64);
	TestCase_assert(context.readUInt64 == BinaryDeserializationContext_readUInt64, "Expected %p but got %p", BinaryDeserializationContext_readUInt64, context.readUInt64);
	TestCase_assert(context.readFloat == BinaryDeserializationContext_readFloat, "Expected %p but got %p", BinaryDeserializationContext_readFloat, context.readFloat);
	TestCase_assert(context.readDouble == BinaryDeserializationContext_readDouble, "Expected %p but got %p", BinaryDeserializationContext_readDouble, context.readDouble);
	TestCase_assert(context.readString == BinaryDeserializationContext_readString, "Expected %p but got %p", BinaryDeserializationContext_readString, context.readString);
	TestCase_assert(context.readBoolean == BinaryDeserializationContext_readBoolean, "Expected %p but got %p", BinaryDeserializationContext_readBoolean, context.readBoolean);
	TestCase_assert(context.readEnumeration == BinaryDeserializationContext_readEnumeration, "Expected %p but got %p", BinaryDeserializationContext_readEnumeration, context.readEnumeration);
	TestCase_assert(context.readBitfield8 == BinaryDeserializationContext_readBitfield8, "Expected %p but got %p", BinaryDeserializationContext_readBitfield8, context.readBitfield8);
	TestCase_assert(context.readBitfield16 == BinaryDeserializationContext_readBitfield16, "Expected %p but got %p", BinaryDeserializationContext_readBitfield16, context.readBitfield16);
	TestCase_assert(context.readBitfield32 == BinaryDeserializationContext_readBitfield32, "Expected %p but got %p", BinaryDeserializationContext_readBitfield32, context.readBitfield32);
	TestCase_assert(context.readBitfield64 == BinaryDeserializationContext_readBitfield64, "Expected %p but got %p", BinaryDeserializationContext_readBitfield64, context.readBitfield64);
	TestCase_assert(context.readNextDictionaryKey == BinaryDeserializationContext_readNextDictionaryKey, "Expected %p but got %p", BinaryDeserializationContext_readNextDictionaryKey, context.readNextDictionaryKey);
	TestCase_assert(context.hasDictionaryKey == BinaryDeserializationContext_hasDictionaryKey, "Expected %p but got %p", BinaryDeserializationContext_hasDictionaryKey, context.hasDictionaryKey);
	context.dispose(&context);
	
	contextPtr = BinaryDeserializationContext_createWithBytes("metS", 4);
	TestCase_assert(contextPtr != NULL, "Expected non-NULL but got NULL");
	if (contextPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(contextPtr->jmpBuf == NULL, "Expected NULL but got %p", contextPtr->jmpBuf);
	TestCase_assert(contextPtr->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, contextPtr->status);
	TestCase_assert(!contextPtr->bigEndian, "Expected false but got true");
	TestCase_assert(contextPtr->dispose == BinaryDeserializationContext_dispose, "Expected %p but got %p", BinaryDeserializationContext_dispose, contextPtr->dispose);
	TestCase_assert(contextPtr->beginStructure == BinaryDeserializationContext_beginStructure, "Expected %p but got %p", BinaryDeserializationContext_beginStructure, contextPtr->beginStructure);
	TestCase_assert(contextPtr->beginDictionary == BinaryDeserializationContext_beginDictionary, "Expected %p but got %p", BinaryDeserializationContext_beginDictionary, contextPtr->beginDictionary);
	TestCase_assert(contextPtr->beginArray == BinaryDeserializationContext_beginArray, "Expected %p but got %p", BinaryDeserializationContext_beginArray, contextPtr->beginArray);
	TestCase_assert(contextPtr->endStructure == BinaryDeserializationContext_endStructure, "Expected %p but got %p", BinaryDeserializationContext_endStructure, contextPtr->endStructure);
	TestCase_assert(contextPtr->endDictionary == BinaryDeserializationContext_endDictionary, "Expected %p but got %p", BinaryDeserializationContext_endDictionary, contextPtr->endDictionary);
	TestCase_assert(contextPtr->endArray == BinaryDeserializationContext_endArray, "Expected %p but got %p", BinaryDeserializationContext_endArray, contextPtr->endArray);
	TestCase_assert(contextPtr->readInt8 == BinaryDeserializationContext_readInt8, "Expected %p but got %p", BinaryDeserializationContext_readInt8, contextPtr->readInt8);
	TestCase_assert(contextPtr->readUInt8 == BinaryDeserializationContext_readUInt8, "Expected %p but got %p", BinaryDeserializationContext_readUInt8, contextPtr->readUInt8);
	TestCase_assert(contextPtr->readInt16 == BinaryDeserializationContext_readInt16, "Expected %p but got %p", BinaryDeserializationContext_readInt16, contextPtr->readInt16);
	TestCase_assert(contextPtr->readUInt16 == BinaryDeserializationContext_readUInt16, "Expected %p but got %p", BinaryDeserializationContext_readUInt16, contextPtr->readUInt16);
	TestCase_assert(contextPtr->readInt32 == BinaryDeserializationContext_readInt32, "Expected %p but got %p", BinaryDeserializationContext_readInt32, contextPtr->readInt32);
	TestCase_assert(contextPtr->readUInt32 == BinaryDeserializationContext_readUInt32, "Expected %p but got %p", BinaryDeserializationContext_readUInt32, contextPtr->readUInt32);
	TestCase_assert(contextPtr->readInt64 == BinaryDeserializationContext_readInt64, "Expected %p but got %p", BinaryDeserializationContext_readInt64, contextPtr->readInt64);
	TestCase_assert(contextPtr->readUInt64 == BinaryDeserializationContext_readUInt64, "Expected %p but got %p", BinaryDeserializationContext_readUInt64, contextPtr->readUInt64);
	TestCase_assert(contextPtr->readFloat == BinaryDeserializationContext_readFloat, "Expected %p but got %p", BinaryDeserializationContext_readFloat, contextPtr->readFloat);
	TestCase_assert(contextPtr->readDouble == BinaryDeserializationContext_readDouble, "Expected %p but got %p", BinaryDeserializationContext_readDouble, contextPtr->readDouble);
	TestCase_assert(contextPtr->readString == BinaryDeserializationContext_readString, "Expected %p but got %p", BinaryDeserializationContext_readString, contextPtr->readString);
	TestCase_assert(contextPtr->readBoolean == BinaryDeserializationContext_readBoolean, "Expected %p but got %p", BinaryDeserializationContext_readBoolean, contextPtr->readBoolean);
	TestCase_assert(contextPtr->readEnumeration == BinaryDeserializationContext_readEnumeration, "Expected %p but got %p", BinaryDeserializationContext_readEnumeration, contextPtr->readEnumeration);
	TestCase_assert(contextPtr->readBitfield8 == BinaryDeserializationContext_readBitfield8, "Expected %p but got %p", BinaryDeserializationContext_readBitfield8, contextPtr->readBitfield8);
	TestCase_assert(contextPtr->readBitfield16 == BinaryDeserializationContext_readBitfield16, "Expected %p but got %p", BinaryDeserializationContext_readBitfield16, contextPtr->readBitfield16);
	TestCase_assert(contextPtr->readBitfield32 == BinaryDeserializationContext_readBitfield32, "Expected %p but got %p", BinaryDeserializationContext_readBitfield32, contextPtr->readBitfield32);
	TestCase_assert(contextPtr->readBitfield64 == BinaryDeserializationContext_readBitfield64, "Expected %p but got %p", BinaryDeserializationContext_readBitfield64, contextPtr->readBitfield64);
	TestCase_assert(contextPtr->readNextDictionaryKey == BinaryDeserializationContext_readNextDictionaryKey, "Expected %p but got %p", BinaryDeserializationContext_readNextDictionaryKey, contextPtr->readNextDictionaryKey);
	TestCase_assert(contextPtr->hasDictionaryKey == BinaryDeserializationContext_hasDictionaryKey, "Expected %p but got %p", BinaryDeserializationContext_hasDictionaryKey, contextPtr->hasDictionaryKey);
	contextPtr->dispose(contextPtr);
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	writeFileSimple(tempFilePath, "Stem", 4);
	contextPtr = BinaryDeserializationContext_createWithFile(tempFilePath);
	close(fd);
	unlink(tempFilePath);
	TestCase_assert(contextPtr != NULL, "Expected non-NULL but got NULL");
	if (contextPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(contextPtr->jmpBuf == NULL, "Expected NULL but got %p", contextPtr->jmpBuf);
	TestCase_assert(contextPtr->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, contextPtr->status);
	TestCase_assert(contextPtr->bigEndian, "Expected true but got false");
	TestCase_assert(contextPtr->dispose == BinaryDeserializationContext_dispose, "Expected %p but got %p", BinaryDeserializationContext_dispose, contextPtr->dispose);
	TestCase_assert(contextPtr->beginStructure == BinaryDeserializationContext_beginStructure, "Expected %p but got %p", BinaryDeserializationContext_beginStructure, contextPtr->beginStructure);
	TestCase_assert(contextPtr->beginDictionary == BinaryDeserializationContext_beginDictionary, "Expected %p but got %p", BinaryDeserializationContext_beginDictionary, contextPtr->beginDictionary);
	TestCase_assert(contextPtr->beginArray == BinaryDeserializationContext_beginArray, "Expected %p but got %p", BinaryDeserializationContext_beginArray, contextPtr->beginArray);
	TestCase_assert(contextPtr->endStructure == BinaryDeserializationContext_endStructure, "Expected %p but got %p", BinaryDeserializationContext_endStructure, contextPtr->endStructure);
	TestCase_assert(contextPtr->endDictionary == BinaryDeserializationContext_endDictionary, "Expected %p but got %p", BinaryDeserializationContext_endDictionary, contextPtr->endDictionary);
	TestCase_assert(contextPtr->endArray == BinaryDeserializationContext_endArray, "Expected %p but got %p", BinaryDeserializationContext_endArray, contextPtr->endArray);
	TestCase_assert(contextPtr->readInt8 == BinaryDeserializationContext_readInt8, "Expected %p but got %p", BinaryDeserializationContext_readInt8, contextPtr->readInt8);
	TestCase_assert(contextPtr->readUInt8 == BinaryDeserializationContext_readUInt8, "Expected %p but got %p", BinaryDeserializationContext_readUInt8, contextPtr->readUInt8);
	TestCase_assert(contextPtr->readInt16 == BinaryDeserializationContext_readInt16, "Expected %p but got %p", BinaryDeserializationContext_readInt16, contextPtr->readInt16);
	TestCase_assert(contextPtr->readUInt16 == BinaryDeserializationContext_readUInt16, "Expected %p but got %p", BinaryDeserializationContext_readUInt16, contextPtr->readUInt16);
	TestCase_assert(contextPtr->readInt32 == BinaryDeserializationContext_readInt32, "Expected %p but got %p", BinaryDeserializationContext_readInt32, contextPtr->readInt32);
	TestCase_assert(contextPtr->readUInt32 == BinaryDeserializationContext_readUInt32, "Expected %p but got %p", BinaryDeserializationContext_readUInt32, contextPtr->readUInt32);
	TestCase_assert(contextPtr->readInt64 == BinaryDeserializationContext_readInt64, "Expected %p but got %p", BinaryDeserializationContext_readInt64, contextPtr->readInt64);
	TestCase_assert(contextPtr->readUInt64 == BinaryDeserializationContext_readUInt64, "Expected %p but got %p", BinaryDeserializationContext_readUInt64, contextPtr->readUInt64);
	TestCase_assert(contextPtr->readFloat == BinaryDeserializationContext_readFloat, "Expected %p but got %p", BinaryDeserializationContext_readFloat, contextPtr->readFloat);
	TestCase_assert(contextPtr->readDouble == BinaryDeserializationContext_readDouble, "Expected %p but got %p", BinaryDeserializationContext_readDouble, contextPtr->readDouble);
	TestCase_assert(contextPtr->readString == BinaryDeserializationContext_readString, "Expected %p but got %p", BinaryDeserializationContext_readString, contextPtr->readString);
	TestCase_assert(contextPtr->readBoolean == BinaryDeserializationContext_readBoolean, "Expected %p but got %p", BinaryDeserializationContext_readBoolean, contextPtr->readBoolean);
	TestCase_assert(contextPtr->readEnumeration == BinaryDeserializationContext_readEnumeration, "Expected %p but got %p", BinaryDeserializationContext_readEnumeration, contextPtr->readEnumeration);
	TestCase_assert(contextPtr->readBitfield8 == BinaryDeserializationContext_readBitfield8, "Expected %p but got %p", BinaryDeserializationContext_readBitfield8, contextPtr->readBitfield8);
	TestCase_assert(contextPtr->readBitfield16 == BinaryDeserializationContext_readBitfield16, "Expected %p but got %p", BinaryDeserializationContext_readBitfield16, contextPtr->readBitfield16);
	TestCase_assert(contextPtr->readBitfield32 == BinaryDeserializationContext_readBitfield32, "Expected %p but got %p", BinaryDeserializationContext_readBitfield32, contextPtr->readBitfield32);
	TestCase_assert(contextPtr->readBitfield64 == BinaryDeserializationContext_readBitfield64, "Expected %p but got %p", BinaryDeserializationContext_readBitfield64, contextPtr->readBitfield64);
	TestCase_assert(contextPtr->readNextDictionaryKey == BinaryDeserializationContext_readNextDictionaryKey, "Expected %p but got %p", BinaryDeserializationContext_readNextDictionaryKey, contextPtr->readNextDictionaryKey);
	TestCase_assert(contextPtr->hasDictionaryKey == BinaryDeserializationContext_hasDictionaryKey, "Expected %p but got %p", BinaryDeserializationContext_hasDictionaryKey, contextPtr->hasDictionaryKey);
	contextPtr->dispose(contextPtr);
}

static void testInitErrors() {
	BinaryDeserializationContext * context;
	
	context = BinaryDeserializationContext_createWithBytes("", 0);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, context->status);
	context->dispose(context);
	
	context = BinaryDeserializationContext_createWithBytes("Ste", 3);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, context->status);
	context->dispose(context);
	
	context = BinaryDeserializationContext_createWithBytes("met", 3);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, context->status);
	context->dispose(context);
	
	context = BinaryDeserializationContext_createWithBytes("nope", 4);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == BINARY_SERIALIZATION_ERROR_INVALID_SIGNATURE, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_INVALID_SIGNATURE, context->status);
	context->dispose(context);
}

#define beginAndVerifyContainer(CONTAINER_TYPE, KEY, EXPECTED_SIZE) { \
	size_t size; \
	size = context->begin##CONTAINER_TYPE(context, KEY); \
	TestCase_assert(size == (EXPECTED_SIZE), "Expected %zu but got %zu (status %d)", (size_t) (EXPECTED_SIZE), size, context->status); \
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
#define printfSpecifier_int64_t "%lld"
#define printfSpecifier_uint64_t "%llu"
#define printfSpecifier_float "%f"
#define printfSpecifier_double "%f"
#define printfSpecifier_bool "%d"
#define readAndVerifyNumber(TYPE, KEY, EXPECTED_VALUE) { \
	TYPE value; \
	value = context->readFunction_##TYPE(context, KEY); \
	TestCase_assert(value == (EXPECTED_VALUE), "Expected " printfSpecifier_##TYPE " but got " printfSpecifier_##TYPE " (status %d)", (TYPE) (EXPECTED_VALUE), value, context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define readAndVerifyString(KEY, EXPECTED_VALUE) { \
	const char * value; \
	value = context->readString(context, KEY); \
	TestCase_assert(value != NULL, "Expected non-NULL but got NULL"); \
	TestCase_assert(!strcmp(value, (EXPECTED_VALUE)), "Expected \"%s\" but got \"%s\" (status %d)", (EXPECTED_VALUE), value, context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define readAndVerifyEnumeration(KEY, EXPECTED_VALUE, ...) { \
	int value; \
	value = context->readEnumeration(context, KEY, __VA_ARGS__); \
	TestCase_assert(value == (EXPECTED_VALUE), "Expected %d but got %d (status %d)", (EXPECTED_VALUE), value, context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define printfSpecifier_bitfield8 "0x%02X"
#define printfSpecifier_bitfield16 "0x%04X"
#define printfSpecifier_bitfield32 "0x%08X"
#define printfSpecifier_bitfield64 "0x%016llX"
#define readAndVerifyBitfield(NBITS, KEY, EXPECTED_VALUE, ...) { \
	uint##NBITS##_t value; \
	value = context->readBitfield##NBITS(context, KEY, __VA_ARGS__); \
	TestCase_assert(value == (EXPECTED_VALUE), "Expected " printfSpecifier_bitfield##NBITS " but got " printfSpecifier_bitfield##NBITS " (status %d)", (EXPECTED_VALUE), value, context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status); \
}

#define verifyReadNextDictionaryKey(KEY) \
	key = context->readNextDictionaryKey(context); \
	TestCase_assert(key != NULL, "Expected non-NULL but got NULL (status %d)", context->status); \
	TestCase_assert(!strcmp(key, (KEY)), "Expected \"%s\" but got \"%s\"", (KEY), key); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status);

#define verifyHasDictionaryKey(KEY, HAS_KEY) \
	TestCase_assert(context->hasDictionaryKey(context, (KEY)) == (HAS_KEY), "Expected %s but got %s (status %d)", (HAS_KEY) ? "true" : "false", (HAS_KEY) ? "false" : "true", context->status); \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Got error from operation that should have succeeded: %d", context->status);

static void testNumberValues() {
	BinaryDeserializationContext * context;
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
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
		"\x40\x22\x00\x00\x00\x00\x00\x00",
		50
	);
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
	
	context = BinaryDeserializationContext_createWithBytes(
		"metS"
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
		"\x00\x00\x00\x00\x00\x00\x22\x40",
		50
	);
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
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
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
		"\x7F\xEF\xFF\xFF\xFF\xFF\xFF\xFF",
		50
	);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 10)
	readAndVerifyNumber(int8_t, "", INT8_MIN)
	readAndVerifyNumber(uint8_t, "", UINT8_MAX)
	readAndVerifyNumber(int16_t, "", INT16_MIN)
	readAndVerifyNumber(uint16_t, "", UINT16_MAX)
	readAndVerifyNumber(int32_t, "", INT32_MIN)
	readAndVerifyNumber(uint32_t, "", UINT32_MAX)
	readAndVerifyNumber(int64_t, "", INT64_MIN)
	readAndVerifyNumber(uint64_t, "", UINT64_MAX)
	readAndVerifyNumber(float, "", FLT_MAX)
	readAndVerifyNumber(double, "", DBL_MAX)
	context->endArray(context);
	context->dispose(context);
	
	context = BinaryDeserializationContext_createWithBytes(
		"metS"
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
		"\xFF\xFF\xFF\xFF\xFF\xFF\xEF\x7F",
		50
	);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 10)
	readAndVerifyNumber(int8_t, "", INT8_MIN)
	readAndVerifyNumber(uint8_t, "", UINT8_MAX)
	readAndVerifyNumber(int16_t, "", INT16_MIN)
	readAndVerifyNumber(uint16_t, "", UINT16_MAX)
	readAndVerifyNumber(int32_t, "", INT32_MIN)
	readAndVerifyNumber(uint32_t, "", UINT32_MAX)
	readAndVerifyNumber(int64_t, "", INT64_MIN)
	readAndVerifyNumber(uint64_t, "", UINT64_MAX)
	readAndVerifyNumber(float, "", FLT_MAX)
	readAndVerifyNumber(double, "", DBL_MAX)
	context->endArray(context);
	context->dispose(context);
}

static void testStringValues() {
	BinaryDeserializationContext * context;
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\x00\x00\x00\x02"
		"foo\x00"
		"Hello, world!\x00",
		26
	);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 2)
	readAndVerifyString("", "foo")
	readAndVerifyString("", "Hello, world!")
	context->endArray(context);
	context->dispose(context);
}

static void testBooleanValues() {
	BinaryDeserializationContext * context;
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\x00\x00\x00\x02"
		"\x00"
		"\x01",
		10
	);
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
	BinaryDeserializationContext * context;
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\x00\x00\x00\x05"
		"\x00\x00\x00\x00"
		"\x00\x00\x00\x01"
		"\x00\x00\x00\x00"
		"\x00\x00\x00\x01"
		"\x00\x00\x00\x02",
		28
	);
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
	
	context = BinaryDeserializationContext_createWithBytes(
		"metS"
		"\x05\x00\x00\x00"
		"\x00\x00\x00\x00"
		"\x01\x00\x00\x00"
		"\x00\x00\x00\x00"
		"\x01\x00\x00\x00"
		"\x02\x00\x00\x00",
		28
	);
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
	BinaryDeserializationContext * context;
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
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
		"\x00\x00\x00\x00\x00\x00\x00\x00",
		53
	);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 12)
	readAndVerifyBitfield(8, "", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7");
	readAndVerifyBitfield(8, "", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	readAndVerifyBitfield(16, "", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F");
	readAndVerifyBitfield(16, "", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(32, "", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F");
	readAndVerifyBitfield(32, "", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(64, "", 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F");
	readAndVerifyBitfield(64, "", 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(8, "", 0x00, NULL);
	readAndVerifyBitfield(16, "", 0x0000, NULL);
	readAndVerifyBitfield(32, "", 0x00000000, NULL);
	readAndVerifyBitfield(64, "", 0x0000000000000000ull, NULL);
	context->endArray(context);
	context->dispose(context);
	
	context = BinaryDeserializationContext_createWithBytes(
		"metS"
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
		"\x00\x00\x00\x00\x00\x00\x00\x00",
		53
	);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	beginAndVerifyArray("", 12)
	readAndVerifyBitfield(8, "", 0xAA, "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7");
	readAndVerifyBitfield(8, "", 0x57, "bit0", "bit1", "bit2", "bit3", "bit4", "bit5", "bit6", NULL);
	readAndVerifyBitfield(16, "", 0xF001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F");
	readAndVerifyBitfield(16, "", 0x000F, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(32, "", 0xF0000001, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F");
	readAndVerifyBitfield(32, "", 0x0000001E, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(64, "", 0xF000000000000001ull, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F");
	readAndVerifyBitfield(64, "", 0x000000000000003Cull, "b0", "b1", "b2", "b3", "b4", "b5", NULL);
	readAndVerifyBitfield(8, "", 0x00, NULL);
	readAndVerifyBitfield(16, "", 0x0000, NULL);
	readAndVerifyBitfield(32, "", 0x00000000, NULL);
	readAndVerifyBitfield(64, "", 0x0000000000000000ull, NULL);
	context->endArray(context);
	context->dispose(context);
}

static void testArrays() {
	BinaryDeserializationContext * context;
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\x00\x00\x00\x02"
		"\x00\x00\x00\x01"
		"\xFF"
		"\x00\x00\x00\x0C"
		"\x00"
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
		"\x00\x00\x00\x00\x00\x00\x00\x01",
		86
	);
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
			readAndVerifyBitfield(64, "", 0x0000000000000001ull, "16", NULL)
		context->endArray(context);
	context->endArray(context);
	context->dispose(context);
}

static void testStructures() {
	BinaryDeserializationContext * context;
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\xFF"
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
		"\x00\x00\x00\x00\x00\x00\x00\x01",
		72
	);
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
			readAndVerifyBitfield(64, "u", 0x0000000000000001ull, "16", NULL)
		context->endStructure(context);
	context->endStructure(context);
	context->dispose(context);
}

static void testDictionaries() {
	BinaryDeserializationContext * context;
	const char * key;
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\x00\x00\x00\x02" // top start
		"\x00\x00\x00\xE0" // top end offset
		"a\x00"
		"\x00\x00\x00\x14" // a offset from top start
		"c\x00"
		"\x00\x00\x00\x23" // c offset from top start
		"\x00\x00\x00\x01" // a start
		"\x00\x00\x00\x0F" // a end offset
		"b\x00"
		"\x00\x00\x00\x0E" // b offset from a start
		"\xFF" // b
		// a end
		"\x00\x00\x00\x0C" // c start
		"\x00\x00\x00\xBD" // c end offset
		"d\x00"
		"\x00\x00\x00\x50" // d offset from c start
		"e\x00"
		"\x00\x00\x00\x51" // e offset from c start
		"f\x00"
		"\x00\x00\x00\x52" // f offset from c start
		"m\x00"
		"\x00\x00\x00\x9A" // m offset from c start
		"n\x00"
		"\x00\x00\x00\x9E" // n offset from c start
		"o\x00"
		"\x00\x00\x00\xA6" // o offset from c start
		"p\x00"
		"\x00\x00\x00\xA9" // p offset from c start
		"q\x00"
		"\x00\x00\x00\xAA" // q offset from c start
		"r\x00"
		"\x00\x00\x00\xAE" // r offset from c start
		"s\x00"
		"\x00\x00\x00\xAF" // s offset from c start
		"t\x00"
		"\x00\x00\x00\xB1" // t offset from c start
		"u\x00"
		"\x00\x00\x00\xB5" // u offset from c start
		"\x00" // d
		"\x01" // e
		"\x00\x00\x00\x06" // f start
		"\x00\x00\x00\x48" // f end offset
		"g\x00"
		"\x00\x00\x00\x2C" // g offset from f start
		"h\x00"
		"\x00\x00\x00\x2E" // h offset from f start
		"i\x00"
		"\x00\x00\x00\x30" // i offset from f start
		"j\x00"
		"\x00\x00\x00\x34" // j offset from f start
		"k\x00"
		"\x00\x00\x00\x38" // k offset from f start
		"l\x00"
		"\x00\x00\x00\x40" // l offset from f start
		"\x00\x02" // g
		"\x00\x03" // h
		"\x00\x00\x00\x04" // i
		"\x00\x00\x00\x05" // j
		"\x00\x00\x00\x00\x00\x00\x00\x06" // k
		"\x00\x00\x00\x00\x00\x00\x00\x07" // l
		// f end
		"\x41\x00\x00\x00" // m
		"\x40\x22\x00\x00\x00\x00\x00\x00" // n
		"10\x00" // o
		"\x01" // p
		"\x00\x00\x00\x0C" // q
		"\x01" // r
		"\x00\x01" // s
		"\x00\x00\x00\x01" // t
		"\x00\x00\x00\x00\x00\x00\x00\x01", // u
		// c end
		// top end
		228
	);
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
			readAndVerifyBitfield(64, "u", 0x0000000000000001ull, "16", NULL)
		context->endDictionary(context);
	context->endDictionary(context);
	context->dispose(context);
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\x00\x00\x00\x02" // top start
		"\x00\x00\x00\xE8" //! top end offset
		"c\x00"
		"\x00\x00\x00\x14" // c offset from top start
		"a\x00"
		"\x00\x00\x00\xD1" // a offset from top start
		"\x00\x00\x00\x0C" // c start
		"\x00\x00\x00\xBD" // c end offset
		"u\x00"
		"\x00\x00\x00\x50" // u offset from c start
		"t\x00"
		"\x00\x00\x00\x58" // t offset from c start
		"s\x00"
		"\x00\x00\x00\x5C" // s offset from c start
		"r\x00"
		"\x00\x00\x00\x5E" // r offset from c start
		"q\x00"
		"\x00\x00\x00\x5F" // q offset from c start
		"p\x00"
		"\x00\x00\x00\x63" // p offset from c start
		"o\x00"
		"\x00\x00\x00\x64" // o offset from c start
		"n\x00"
		"\x00\x00\x00\x67" // n offset from c start
		"m\x00"
		"\x00\x00\x00\x6F" // m offset from c start
		"f\x00"
		"\x00\x00\x00\x73" // f offset from c start
		"e\x00"
		"\x00\x00\x00\xBB" // e offset from c start
		"d\x00"
		"\x00\x00\x00\xBC" // d offset from c start
		"\x00\x00\x00\x00\x00\x00\x00\x01" // u
		"\x00\x00\x00\x01" // t
		"\x00\x01" // s
		"\x01" // r
		"\x00\x00\x00\x0C" // q
		"\x01" // p
		"10\x00" // o
		"\x40\x22\x00\x00\x00\x00\x00\x00" // n
		"\x41\x00\x00\x00" // m
		"\x00\x00\x00\x06" // f start
		"\x00\x00\x00\x48" // f end offset
		"l\x00"
		"\x00\x00\x00\x2C" //! l offset from f start
		"k\x00"
		"\x00\x00\x00\x34" //! k offset from f start
		"j\x00"
		"\x00\x00\x00\x3C" //! j offset from f start
		"i\x00"
		"\x00\x00\x00\x40" //! i offset from f start
		"h\x00"
		"\x00\x00\x00\x44" //! h offset from f start
		"g\x00"
		"\x00\x00\x00\x46" //! g offset from f start
		"\x00\x00\x00\x00\x00\x00\x00\x07" // l
		"\x00\x00\x00\x00\x00\x00\x00\x06" // k
		"\x00\x00\x00\x05" // j
		"\x00\x00\x00\x04" // i
		"\x00\x03" // h
		"\x00\x02" // g
		// f end
		"\x01" // e
		"\x00" // d
		// c end
		"\x00\x00\x00\x02" // a start
		"\x00\x00\x00\x17" // a end offset
		"b2\x00"
		"\x00\x00\x00\x15" //! b2 offset from a start
		"b\x00"
		"\x00\x00\x00\x16" //! b offset from a start
		"\xF6" // b2
		"\xFF", // b
		// a end
		// top end
		236
	);
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
			readAndVerifyBitfield(64, "u", 0x0000000000000001ull, "16", NULL)
		context->endDictionary(context);
	context->endDictionary(context);
	context->dispose(context);
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\x00\x00\x00\x03" // top start
		"\x00\x00\x00\x67" // top end offset
		"foo\x00"
		"\x00\x00\x00\x20" // foo 1 offset from top start
		"foo\x00"
		"\x00\x00\x00\x43" // foo 2 offset from top start
		"bar\x00"
		"\x00\x00\x00\x44" // bar 1 offset from top start
		"\x00\x00\x00\x03" // foo 1 start
		"\x00\x00\x00\x23" // foo 1 end offset
		"bar\x00"
		"\x00\x00\x00\x20" // bar 2 offset from foo 1 start
		"baz\x00"
		"\x00\x00\x00\x21" // baz 1 offset from foo 1 start
		"bar\x00"
		"\x00\x00\x00\x22" // bar 3 offset from foo 1 start
		"\x00" // bar 2
		"\x01" // baz 1
		"\x02" // bar 3
		// foo 1 end
		"\x03" // foo 2
		"\x00\x00\x00\x03" // bar 1 start
		"\x00\x00\x00\x23" // bar 1 end offset
		"foo\x00"
		"\x00\x00\x00\x20" // foo 3 offset from bar 1 start
		"baz\x00"
		"\x00\x00\x00\x21" // baz 2 offset from bar 1 start
		"baz\x00"
		"\x00\x00\x00\x22" // baz 3 offset from bar 1 start
		"\x04"
		"\x05"
		"\x06",
		// bar 1 end
		// top end
		111
	);
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
			readAndVerifyNumber(int8_t, key, 0)
			verifyReadNextDictionaryKey("baz")
			readAndVerifyNumber(int8_t, key, 1)
			verifyReadNextDictionaryKey("bar")
			readAndVerifyNumber(int8_t, key, 2)
		context->endDictionary(context);
		verifyReadNextDictionaryKey("foo")
		readAndVerifyNumber(int8_t, key, 3)
		verifyReadNextDictionaryKey("bar")
		beginAndVerifyDictionary(key, 3)
			verifyHasDictionaryKey("foo", true)
			verifyHasDictionaryKey("bar", false)
			verifyHasDictionaryKey("baz", true)
			verifyReadNextDictionaryKey("foo")
			readAndVerifyNumber(int8_t, key, 4)
			verifyReadNextDictionaryKey("baz")
			readAndVerifyNumber(int8_t, key, 5)
			verifyReadNextDictionaryKey("baz")
			readAndVerifyNumber(int8_t, key, 6)
		context->endDictionary(context);
	context->endDictionary(context);
	context->dispose(context);
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\x00\x00\x00\x15" // top start
		"\x00\x00\x00\xCB" // top end offset
		"\x00"
		"\x00\x00\x00\x71" // key 0 offset from top start
		"\x00"
		"\x00\x00\x00\x72" // key 1 offset from top start
		"\x00"
		"\x00\x00\x00\x73" // key 2 offset from top start
		"\x00"
		"\x00\x00\x00\x74" // key 3 offset from top start
		"\x00"
		"\x00\x00\x00\x76" // key 4 offset from top start
		"\x00"
		"\x00\x00\x00\x78" // key 5 offset from top start
		"\x00"
		"\x00\x00\x00\x7C" // key 6 offset from top start
		"\x00"
		"\x00\x00\x00\x80" // key 7 offset from top start
		"\x00"
		"\x00\x00\x00\x88" // key 8 offset from top start
		"\x00"
		"\x00\x00\x00\x90" // key 9 offset from top start
		"\x00"
		"\x00\x00\x00\x94" // key 10 offset from top start
		"\x00"
		"\x00\x00\x00\x9C" // key 11 offset from top start
		"\x00"
		"\x00\x00\x00\x9F" // key 12 offset from top start
		"\x00"
		"\x00\x00\x00\xA0" // key 13 offset from top start
		"\x00"
		"\x00\x00\x00\xA4" // key 14 offset from top start
		"\x00"
		"\x00\x00\x00\xA5" // key 15 offset from top start
		"\x00"
		"\x00\x00\x00\xA7" // key 16 offset from top start
		"\x00"
		"\x00\x00\x00\xAB" // key 17 offset from top start
		"\x00"
		"\x00\x00\x00\xB3" // key 18 offset from top start
		"\x00"
		"\x00\x00\x00\xB7" // key 19 offset from top start
		"\x00"
		"\x00\x00\x00\xB7" // key 20 offset from top start
		"\xFF" // value 0
		"\x00" // value 1
		"\x01" // value 2
		"\x00\x02" // value 3
		"\x00\x03" // value 4
		"\x00\x00\x00\x04" // value 5
		"\x00\x00\x00\x05" // value 6
		"\x00\x00\x00\x00\x00\x00\x00\x06" // value 7
		"\x00\x00\x00\x00\x00\x00\x00\x07" // value 8
		"\x41\x00\x00\x00" // value 9
		"\x40\x22\x00\x00\x00\x00\x00\x00" // value 10
		"10\x00" // value 11
		"\x01" // value 12
		"\x00\x00\x00\x0C" // value 13
		"\x01" // value 14
		"\x00\x01" // value 15
		"\x00\x00\x00\x01" // value 16
		"\x00\x00\x00\x00\x00\x00\x00\x01" // value 17
		"\x00\x00\x00\x00" // value 18
		// value 19
		"\x00\x00\x00\x02" // value 20 start
		"\x00\x00\x00\x14" // value 20 end offset
		"\x00"
		"\x00\x00\x00\x12" // value 20 key 0 offset from value 20 start
		"\x00"
		"\x00\x00\x00\x13" // value 20 key 1 offset from value 20 start
		"\x00" // value 20 value 0
		"\x00", // value 20 value 1
		// value 20 end
		// top end
		207
	);
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
		readAndVerifyBitfield(64, key, 0x0000000000000001ull, "16", NULL)
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
	BinaryDeserializationContext * context;
	
	context = BinaryDeserializationContext_createWithBytes(
		"Stem"
		"\x00\x00\x00\x02"
		"\x00\x00\x00\x00"
		"\x00\x00\x00\x00"
		"\x00\x00\x00\x08"
		"\x00\x00\x00\x02"
		"\x00\x00\x00\x2C"
		"innerArray2\x00"
		"\x00\x00\x00\x28"
		"innerStruct\x00"
		"\x00\x00\x00\x2C"
		"\x00\x00\x00\x00",
		64
	);
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

#define performActionWithTemporaryFile(STRING, LENGTH, ACTION) { \
	const char * tempFilePath; \
	int fd; \
	\
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd); \
	writeFileSimple(tempFilePath, STRING, LENGTH); \
	ACTION(tempFilePath) \
	close(fd); \
	unlink(tempFilePath); \
}

#define initContextWithFile(FILE_PATH) BinaryDeserializationContext_initWithFile(&context2, FILE_PATH); context = &context2;
#define initContextFromTemporaryFile(STRING, LENGTH) performActionWithTemporaryFile(STRING, LENGTH, initContextWithFile)
#define createContextWithFile(FILE_PATH) context = BinaryDeserializationContext_createWithFile(tempFilePath);
#define createContextFromTemporaryFile(STRING, LENGTH) performActionWithTemporaryFile(STRING, LENGTH, initContextWithFile)

static void testInitWithFile() {
	BinaryDeserializationContext * context, context2;
	
	initContextFromTemporaryFile("Stem\x00\x00\x00\x01" "foo\x00", 12)
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "foo")
	context->endArray(context);
	context->dispose(context);
	
	initContextFromTemporaryFile("Stem\x00\x00\x00\x01" "bar\x00", 12)
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "bar")
	context->endArray(context);
	context->dispose(context);
	
	createContextFromTemporaryFile("Stem\x00\x00\x00\x01" "foo\x00", 12)
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "foo")
	context->endArray(context);
	context->dispose(context);
	
	createContextFromTemporaryFile("Stem\x00\x00\x00\x01" "bar\x00", 12)
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d", SERIALIZATION_ERROR_OK, context->status);
	beginAndVerifyArray("", 1)
		readAndVerifyString("", "bar")
	context->endArray(context);
	context->dispose(context);
}

static void testErrorReporting() {
	BinaryDeserializationContext * context, context2;
	jmp_buf jmpBuf;
	int status;
	
	context = BinaryDeserializationContext_createWithFile("If this file exists, unit tests will fail");
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, context->status);
	context->dispose(context);
	
	BinaryDeserializationContext_initWithFile(&context2, "If this file exists, unit tests will fail");
	TestCase_assert(context2.status == BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, context2.status);
	context2.dispose(&context2);
	
	context = BinaryDeserializationContext_createWithBytes(NULL, 0);
	TestCase_assert(context != NULL, "Expected non-NULL but got NULL");
	if (context == NULL) {return;} // Suppress clang warning
	TestCase_assert(context->status == BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, context->status);
	context->dispose(context);
	
	BinaryDeserializationContext_initWithBytes(&context2, NULL, 0);
	TestCase_assert(context2.status == BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, "Expected %d but got %d", BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF, context2.status);
	context2.dispose(&context2);
	
#define _testFailure(BYTES, LENGTH, ERROR_STATUS, PREAMBLE_CODE, FAIL_CODE) \
	context = BinaryDeserializationContext_createWithBytes(BYTES, LENGTH); \
	PREAMBLE_CODE \
	TestCase_assert(context->status == SERIALIZATION_ERROR_OK, "Expected %d but got %d (OK)", SERIALIZATION_ERROR_OK, context->status); \
	FAIL_CODE \
	TestCase_assert(context->status == ERROR_STATUS, "Expected %d but got %d (context->status)", ERROR_STATUS, context->status); \
	context->dispose(context); \
	\
	context = BinaryDeserializationContext_createWithBytes(BYTES, LENGTH); \
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
	
	_testFailure("Stem",
	             4,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             ,
	             context->beginArray(context, "");)
	_testFailure("Stem\x00",
	             5,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             ,
	             context->beginArray(context, "");)
	_testFailure("Stem\x00\x00",
	             6,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             ,
	             context->beginArray(context, "");)
	_testFailure("Stem\x00\x00\x00",
	             7,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             ,
	             context->beginArray(context, "");)
	
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00",
	             11,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             ,
	             context->beginDictionary(context, "");)
	
	_testFailure("Stem",
	             4,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readInt8(context, "");)
	_testFailure("Stem",
	             4,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readUInt8(context, "");)
	_testFailure("Stem\x00",
	             5,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readInt16(context, "");)
	_testFailure("Stem\x00",
	             5,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readUInt16(context, "");)
	_testFailure("Stem\x00\x00\x00",
	             7,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readInt32(context, "");)
	_testFailure("Stem\x00\x00\x00",
	             7,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readUInt32(context, "");)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00",
	             11,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readInt64(context, "");)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00",
	             11,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readUInt64(context, "");)
	_testFailure("Stem\x00\x00\x00",
	             7,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readFloat(context, "");)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00",
	             11,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readDouble(context, "");)
	_testFailure("Stem",
	             4,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readString(context, "");)
	_testFailure("Stem\xFF",
	             5,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readString(context, "");)
	_testFailure("Stem",
	             4,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readBoolean(context, "");)
	_testFailure("Stem\x00\x00\x00",
	             7,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readEnumeration(context, "", "enum", 0, NULL);)
	_testFailure("Stem",
	             4,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readBitfield8(context, "", NULL);)
	_testFailure("Stem\x00",
	             5,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readBitfield16(context, "", NULL);)
	_testFailure("Stem\x00\x00\x00",
	             7,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readBitfield32(context, "", NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00",
	             11,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             context->beginStructure(context, "");,
	             context->readBitfield64(context, "", NULL);)
	
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x09",
	             12,
	             BINARY_SERIALIZATION_ERROR_INVALID_OFFSET,
	             ,
	             context->beginDictionary(context, "");)
	
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x0D\x00\x00\x00\x00\x0E",
	             17,
	             BINARY_SERIALIZATION_ERROR_INVALID_OFFSET,
	             ,
	             context->beginDictionary(context, "");)
	
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x08",
	             12,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             ,
	             context->beginDictionary(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x0C\x00\x00\x00\x00",
	             11,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             ,
	             context->beginDictionary(context, "");)
	_testFailure("Stem\x00\x00\x00\x02\x00\x00\x00\x0D\x00\x00\x00\x00\x0D",
	             11,
	             BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF,
	             ,
	             context->beginDictionary(context, "");)
	
	_testFailure("Stem\x00",
	             5,
	             BINARY_SERIALIZATION_ERROR_EXTRA_DATA_AT_EOF,
	             context->beginStructure(context, ""); context->beginStructure(context, ""); context->endStructure(context);,
	             context->endStructure(context);)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x00\x00",
	             13,
	             BINARY_SERIALIZATION_ERROR_EXTRA_DATA_AT_EOF,
	             context->beginArray(context, ""); context->beginArray(context, ""); context->endArray(context);,
	             context->endArray(context);)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x18\x00\x00\x00\x00\x0D\x00\x00\x00\x00\x00\x00\x00\x08\x00",
	             29,
	             BINARY_SERIALIZATION_ERROR_EXTRA_DATA_AT_EOF,
	             context->beginDictionary(context, ""); context->beginDictionary(context, ""); context->endDictionary(context);,
	             context->endDictionary(context);)
	
	_testFailure("Stem\x00\x00\x00\x01\xFF",
	             9,
	             BINARY_SERIALIZATION_ERROR_ARRAY_NOT_FULLY_READ,
	             context->beginArray(context, "");,
	             context->endArray(context);)
	
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readInt8(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readUInt8(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readInt16(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readUInt16(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readInt32(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readUInt32(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readInt64(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readUInt64(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readFloat(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readDouble(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readString(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readBoolean(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readEnumeration(context, "", "enum", 0, NULL);)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readBitfield8(context, "", "bit", NULL);)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readBitfield16(context, "", "bit", NULL);)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readBitfield32(context, "", "bit", NULL);)
	_testFailure("Stem\x00\x00\x00\x01\x00",
	             9,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginArray(context, ""); context->readInt8(context, "");,
	             context->readBitfield64(context, "", "bit", NULL);)
	
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08",
	             12,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginDictionary(context, "");,
	             context->readNextDictionaryKey(context);)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x0E\x00\x00\x00\x00\x0D\x00",
	             18,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginDictionary(context, "");
	             context->readNextDictionaryKey(context);,
	             context->readNextDictionaryKey(context);)
	_testFailure("Stem\x00\x00\x00\x02\x00\x00\x00\x1C\x00\x00\x00\x00\x13" "a\x00\x00\x00\x00\x1B\x00\x00\x00\x00\x00\x00\x00\x08\x00",
	             32,
	             SERIALIZATION_ERROR_END_OF_CONTAINER,
	             context->beginDictionary(context, "");
	             context->beginDictionary(context, "");,
	             context->readNextDictionaryKey(context);)
	
#define _testNoSuchKey(READ_CODE) \
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginDictionary(context, "");, READ_CODE) \
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x0F" "a\x00\x00\x00\x00\x0E\x00", 19, SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginDictionary(context, "");, READ_CODE) \
	_testFailure("metS\x00\x00\x00\x00\x08\x00\x00\x00", 12, SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginDictionary(context, "");, READ_CODE) \
	_testFailure("metS\x01\x00\x00\x00\x0F\x00\x00\x00" "a\x00\x0E\x00\x00\x00\x00", 19, SERIALIZATION_ERROR_KEY_NOT_FOUND, context->beginDictionary(context, "");, READ_CODE)
	
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
	
	
	
	
#define _testReadBitfieldFailure(BYTES, LENGTH, ERROR, ...) \
	_testFailure(BYTES, LENGTH, ERROR, context->beginStructure(context, "");, context->readBitfield8(context, "", __VA_ARGS__);) \
	_testFailure(BYTES, LENGTH, ERROR, context->beginStructure(context, "");, context->readBitfield16(context, "", __VA_ARGS__);) \
	_testFailure(BYTES, LENGTH, ERROR, context->beginStructure(context, "");, context->readBitfield32(context, "", __VA_ARGS__);) \
	_testFailure(BYTES, LENGTH, ERROR, context->beginStructure(context, "");, context->readBitfield64(context, "", __VA_ARGS__);)
	
	_testReadBitfieldFailure("Stem\x80\x00\x00\x00\x00\x00\x00\x00", 12, SERIALIZATION_ERROR_UNNAMED_BIT, "foo", NULL)
	_testReadBitfieldFailure("Stem\x02\x00\x00\x00\x00\x00\x00\x00", 12, SERIALIZATION_ERROR_UNNAMED_BIT, "foo", NULL)
	
	_testReadBitfieldFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x00", 12, SERIALIZATION_ERROR_DUPLICATE_BIT, "foo", "foo", NULL);
	
#undef _testReadBitfieldFailure
	
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	             context->beginStructure(context, "");,
	             context->readEnumeration(context, "", NULL);)
	_testFailure("Stem\x00\x00\x00\x01",
	             8,
	             SERIALIZATION_ERROR_ENUM_NOT_NAMED,
	             context->beginStructure(context, "");,
	             context->readEnumeration(context, "", "a", 0, NULL);)
	
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE,
	             context->beginStructure(context, "");,
	             context->readEnumeration(context, "", "enum0", 0, "enum1", 0, NULL);)
	
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME,
	             context->beginStructure(context, "");,
	             context->readEnumeration(context, "", "enum0", 0, "enum0", 1, NULL);)
	
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginArray(context, "");,
	             context->hasDictionaryKey(context, "");)
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginArray(context, "");,
	             context->readNextDictionaryKey(context);)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x00",
	             12,
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginArray(context, "");
	             context->beginArray(context, "");,
	             context->hasDictionaryKey(context, "");)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x00",
	             12,
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginArray(context, "");
	             context->beginArray(context, "");,
	             context->readNextDictionaryKey(context);)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginStructure(context, "");,
	             context->hasDictionaryKey(context, "");)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginStructure(context, "");,
	             context->readNextDictionaryKey(context);)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginStructure(context, "");
	             context->beginStructure(context, "");,
	             context->hasDictionaryKey(context, "");)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_INVALID_OPERATION,
	             context->beginStructure(context, "");
	             context->beginStructure(context, "");,
	             context->readNextDictionaryKey(context);)
	
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readInt8(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readUInt8(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readInt16(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readUInt16(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readInt32(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readUInt32(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readInt64(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readUInt64(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readFloat(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readDouble(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readString(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBoolean(context, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readEnumeration(context, NULL, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBitfield8(context, NULL, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBitfield16(context, NULL, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBitfield32(context, NULL, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NULL_KEY, context->beginStructure(context, "");, context->readBitfield64(context, NULL, NULL);)
	
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readInt8(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readUInt8(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readInt16(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readUInt16(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readInt32(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readUInt32(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readInt64(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readUInt64(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readFloat(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readDouble(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readString(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBoolean(context, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readEnumeration(context, NULL, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBitfield8(context, NULL, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBitfield16(context, NULL, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBitfield32(context, NULL, NULL);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08", 12, SERIALIZATION_ERROR_NULL_KEY, context->beginDictionary(context, "");, context->readBitfield64(context, NULL, NULL);)
	
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginArray(context, "");
	             context->endArray(context);,
	             context->beginArray(context, "");)
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginArray(context, "");
	             context->endArray(context);,
	             context->beginDictionary(context, "");)
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginArray(context, "");
	             context->endArray(context);,
	             context->beginStructure(context, "");)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginStructure(context, "");
	             context->endStructure(context);,
	             context->beginArray(context, "");)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginStructure(context, "");
	             context->endStructure(context);,
	             context->beginDictionary(context, "");)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginStructure(context, "");
	             context->endStructure(context);,
	             context->beginStructure(context, "");)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08",
	             12,
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginDictionary(context, "");
	             context->endDictionary(context);,
	             context->beginArray(context, "");)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08",
	             12,
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginDictionary(context, "");
	             context->endDictionary(context);,
	             context->beginDictionary(context, "");)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08",
	             12,
	             SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS,
	             context->beginDictionary(context, "");
	             context->endDictionary(context);,
	             context->beginStructure(context, "");)
	
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             ;,
	             context->endArray(context);)
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             context->beginArray(context, "");
	             context->endArray(context);,
	             context->endArray(context);)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             ;,
	             context->endStructure(context);)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             context->beginStructure(context, "");
	             context->endStructure(context);,
	             context->endStructure(context);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08",
	             12,
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             ;,
	             context->endDictionary(context);)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x15\x00\x00\x00\x00\x0D\x00\x00\x00\x00\x00\x00\x00\x08",
	             25,
	             SERIALIZATION_ERROR_CONTAINER_UNDERFLOW,
	             context->beginDictionary(context, "");
	             context->endDictionary(context);,
	             context->endDictionary(context);)
	
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "");,
	             context->endStructure(context);)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x00",
	             12,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "");
	             context->beginArray(context, "");,
	             context->endStructure(context);)
	_testFailure("Stem\x00\x00\x00\x00",
	             8,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "");,
	             context->endDictionary(context);)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x00",
	             12,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginArray(context, "");
	             context->beginArray(context, "");,
	             context->endDictionary(context);)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "");,
	             context->endArray(context);)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "");
	             context->beginStructure(context, "");,
	             context->endArray(context);)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "");,
	             context->endDictionary(context);)
	_testFailure("Stem",
	             4,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginStructure(context, "");
	             context->beginStructure(context, "");,
	             context->endDictionary(context);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08",
	             12,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "");,
	             context->endArray(context);)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x15\x00\x00\x00\x00\x0D\x00\x00\x00\x00\x00\x00\x00\x08",
	             25,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "");
	             context->beginDictionary(context, "");,
	             context->endArray(context);)
	_testFailure("Stem\x00\x00\x00\x00\x00\x00\x00\x08",
	             12,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "");,
	             context->endStructure(context);)
	_testFailure("Stem\x00\x00\x00\x01\x00\x00\x00\x15\x00\x00\x00\x00\x0D\x00\x00\x00\x00\x00\x00\x00\x08",
	             25,
	             SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH,
	             context->beginDictionary(context, "");
	             context->beginDictionary(context, "");,
	             context->endStructure(context);)
	
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readInt8(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readUInt8(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readInt16(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readUInt16(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readInt32(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readUInt32(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readInt64(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readUInt64(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readFloat(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readDouble(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readString(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBoolean(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readEnumeration(context, "key", "", 0, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBitfield8(context, "key", NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBitfield16(context, "key", NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBitfield32(context, "key", NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, , context->readBitfield64(context, "key", NULL);)
	
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readInt8(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readUInt8(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readInt16(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readUInt16(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readInt32(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readUInt32(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readInt64(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readUInt64(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readFloat(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readDouble(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readString(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readBoolean(context, "key");)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readEnumeration(context, "key", "", 0, NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readBitfield8(context, "key", NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readBitfield16(context, "key", NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readBitfield32(context, "key", NULL);)
	_testFailure("Stem", 4, SERIALIZATION_ERROR_NO_CONTAINER_STARTED, context->beginStructure(context, ""); context->endStructure(context);, context->readBitfield64(context, "key", NULL);)
	
#undef _testFailure
}

TEST_SUITE(BinaryDeserializationContextTest,
           testInit,
           testInitErrors,
           testNumberValues,
           testStringValues,
           testBooleanValues,
           testEnumerations,
           testBitfields,
           testArrays,
           testStructures,
           testDictionaries,
           testMixedContainers,
           testInitWithFile,
           testErrorReporting)
