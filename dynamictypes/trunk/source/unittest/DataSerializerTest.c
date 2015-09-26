#include "dynamictypes/AssociativeArray.h"
#include "dynamictypes/DataArray.h"
#include "dynamictypes/DataSerializer.h"
#include "dynamictypes/DataValue.h"
#include "dynamictypes/HashTable.h"
#include "serialization/TestDeserializationContext.h"
#include "serialization/TestSerializationContext.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

#define ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES \
	enumKV(DATA_TYPE_BOOLEAN), \
	enumKV(DATA_TYPE_INT8), \
	enumKV(DATA_TYPE_UINT8), \
	enumKV(DATA_TYPE_INT16), \
	enumKV(DATA_TYPE_UINT16), \
	enumKV(DATA_TYPE_INT32), \
	enumKV(DATA_TYPE_UINT32), \
	enumKV(DATA_TYPE_INT64), \
	enumKV(DATA_TYPE_UINT64), \
	enumKV(DATA_TYPE_FLOAT), \
	enumKV(DATA_TYPE_DOUBLE), \
	enumKV(DATA_TYPE_STRING), \
	enumKV(DATA_TYPE_BLOB), \
	enumKV(DATA_TYPE_HASH_TABLE), \
	enumKV(DATA_TYPE_ARRAY), \
	enumKV(DATA_TYPE_ASSOCIATIVE_ARRAY), \
	enumKV(DATA_TYPE_FIXED_16_16)

static void testSerialize() {
	DataValue value;
	TestSerializationContext * context;
	jmp_buf jmpEnv;
	const char ** keys;
	size_t keyCount, keyIndex;
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	value = valueCreateArray(arrayCreateWithValues(
		valueCreateBoolean(false),
		valueCreateBoolean(true),
		valueCreateInt8(0),
		valueCreateInt8(1),
		valueCreateUInt8(0),
		valueCreateUInt8(1),
		valueCreateInt16(0),
		valueCreateInt16(1),
		valueCreateUInt16(0),
		valueCreateUInt16(1),
		valueCreateInt32(0),
		valueCreateInt32(1),
		valueCreateUInt32(0),
		valueCreateUInt32(1),
		valueCreateInt64(0),
		valueCreateInt64(1),
		valueCreateUInt64(0),
		valueCreateUInt64(1),
		valueCreateFloat(0.0f),
		valueCreateFloat(1.0f),
		valueCreateDouble(0.0),
		valueCreateDouble(1.0),
		valueCreateFixed16_16(0x00000),
		valueCreateFixed16_16(0x10000),
		valueCreateString("a", DATA_USE_STRLEN, false, false),
		valueCreateString("foo", DATA_USE_STRLEN, false, false),
		valueCreateBlob("a", 1, false, false),
		valueCreateBlob("foo", 3, false, false)
	), true, false);
	
	context->expectCall(context, context->beginArray, "data");
	context->expectCall(context, context->writeString, NULL, "array");
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, false);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, true);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt8, NULL, 0);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt8, NULL, 1);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt8, NULL, 0);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt8, NULL, 1);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt16, NULL, 0);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt16, NULL, 1);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt16, NULL, 0);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt16, NULL, 1);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt32, NULL, 0);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt32, NULL, 1);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt32, NULL, 0);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt32, NULL, 1);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt64, NULL, 0ll);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt64, NULL, 1ll);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt64, NULL, 0ull);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt64, NULL, 1ull);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeFloat, NULL, 0.0f);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeFloat, NULL, 1.0f);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeDouble, NULL, 0.0);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeDouble, NULL, 1.0);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeFixed16_16, NULL, 0x00000);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeFixed16_16, NULL, 0x10000);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeString, NULL, "a");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeString, NULL, "foo");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBlob, NULL, "a", 1);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBlob, NULL, "foo", 3);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	DataValue_serialize(&value, (SerializationContext *) context);
	
	TestSerializationContext_finish(context);
	TestSerializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	value = valueCreateHashTable(hashCreateWithKeysAndValues(
		"bool_0", valueCreateBoolean(false),
		"bool_1", valueCreateBoolean(true),
		"int8_0", valueCreateInt8(0),
		"int8_1", valueCreateInt8(1),
		"uint8_0", valueCreateUInt8(0),
		"uint8_1", valueCreateUInt8(1),
		"int16_0", valueCreateInt16(0),
		"int16_1", valueCreateInt16(1),
		"uint16_0", valueCreateUInt16(0),
		"uint16_1", valueCreateUInt16(1),
		"int32_0", valueCreateInt32(0),
		"int32_1", valueCreateInt32(1),
		"uint32_0", valueCreateUInt32(0),
		"uint32_1", valueCreateUInt32(1),
		"int64_0", valueCreateInt64(0),
		"int64_1", valueCreateInt64(1),
		"uint64_0", valueCreateUInt64(0),
		"uint64_1", valueCreateUInt64(1),
		"float_0", valueCreateFloat(0.0f),
		"float_1", valueCreateFloat(1.0f),
		"double_0", valueCreateDouble(0.0),
		"double_1", valueCreateDouble(1.0),
		"fixed_0", valueCreateFixed16_16(0x00000),
		"fixed_1", valueCreateFixed16_16(0x10000),
		"string_a", valueCreateString("a", DATA_USE_STRLEN, false, false),
		"string_foo", valueCreateString("foo", DATA_USE_STRLEN, false, false),
		"blob_a", valueCreateBlob("a", 1, false, false),
		"blob_foo", valueCreateBlob("foo", 3, false, false),
	NULL), true, false);
	
	context->expectCall(context, context->beginArray, "data");
	context->expectCall(context, context->writeString, NULL, "structure");
	context->expectCall(context, context->beginArray, NULL);
	keys = hashGetKeys(value.value.hashTable, &keyCount);
	for (keyIndex = 0; keyIndex < keyCount; keyIndex++) {
		if (!strcmp(keys[keyIndex], "bool_0")) {
			context->expectCall(context, context->writeString, NULL, "bool_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeBoolean, NULL, false);
		} else if (!strcmp(keys[keyIndex], "bool_1")) {
			context->expectCall(context, context->writeString, NULL, "bool_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeBoolean, NULL, true);
		} else if (!strcmp(keys[keyIndex], "int8_0")) {
			context->expectCall(context, context->writeString, NULL, "int8_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeInt8, NULL, 0);
		} else if (!strcmp(keys[keyIndex], "int8_1")) {
			context->expectCall(context, context->writeString, NULL, "int8_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeInt8, NULL, 1);
		} else if (!strcmp(keys[keyIndex], "uint8_0")) {
			context->expectCall(context, context->writeString, NULL, "uint8_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeUInt8, NULL, 0);
		} else if (!strcmp(keys[keyIndex], "uint8_1")) {
			context->expectCall(context, context->writeString, NULL, "uint8_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeUInt8, NULL, 1);
		} else if (!strcmp(keys[keyIndex], "int16_0")) {
			context->expectCall(context, context->writeString, NULL, "int16_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeInt16, NULL, 0);
		} else if (!strcmp(keys[keyIndex], "int16_1")) {
			context->expectCall(context, context->writeString, NULL, "int16_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeInt16, NULL, 1);
		} else if (!strcmp(keys[keyIndex], "uint16_0")) {
			context->expectCall(context, context->writeString, NULL, "uint16_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeUInt16, NULL, 0);
		} else if (!strcmp(keys[keyIndex], "uint16_1")) {
			context->expectCall(context, context->writeString, NULL, "uint16_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeUInt16, NULL, 1);
		} else if (!strcmp(keys[keyIndex], "int32_0")) {
			context->expectCall(context, context->writeString, NULL, "int32_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeInt32, NULL, 0);
		} else if (!strcmp(keys[keyIndex], "int32_1")) {
			context->expectCall(context, context->writeString, NULL, "int32_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeInt32, NULL, 1);
		} else if (!strcmp(keys[keyIndex], "uint32_0")) {
			context->expectCall(context, context->writeString, NULL, "uint32_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeUInt32, NULL, 0);
		} else if (!strcmp(keys[keyIndex], "uint32_1")) {
			context->expectCall(context, context->writeString, NULL, "uint32_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeUInt32, NULL, 1);
		} else if (!strcmp(keys[keyIndex], "int64_0")) {
			context->expectCall(context, context->writeString, NULL, "int64_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeInt64, NULL, 0ll);
		} else if (!strcmp(keys[keyIndex], "int64_1")) {
			context->expectCall(context, context->writeString, NULL, "int64_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeInt64, NULL, 1ll);
		} else if (!strcmp(keys[keyIndex], "uint64_0")) {
			context->expectCall(context, context->writeString, NULL, "uint64_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeUInt64, NULL, 0ull);
		} else if (!strcmp(keys[keyIndex], "uint64_1")) {
			context->expectCall(context, context->writeString, NULL, "uint64_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeUInt64, NULL, 1ull);
		} else if (!strcmp(keys[keyIndex], "float_0")) {
			context->expectCall(context, context->writeString, NULL, "float_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeFloat, NULL, 0.0f);
		} else if (!strcmp(keys[keyIndex], "float_1")) {
			context->expectCall(context, context->writeString, NULL, "float_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeFloat, NULL, 1.0f);
		} else if (!strcmp(keys[keyIndex], "double_0")) {
			context->expectCall(context, context->writeString, NULL, "double_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeDouble, NULL, 0.0);
		} else if (!strcmp(keys[keyIndex], "double_1")) {
			context->expectCall(context, context->writeString, NULL, "double_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeDouble, NULL, 1.0);
		} else if (!strcmp(keys[keyIndex], "fixed_0")) {
			context->expectCall(context, context->writeString, NULL, "fixed_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeFixed16_16, NULL, 0x00000);
		} else if (!strcmp(keys[keyIndex], "fixed_1")) {
			context->expectCall(context, context->writeString, NULL, "fixed_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeFixed16_16, NULL, 0x10000);
		} else if (!strcmp(keys[keyIndex], "string_a")) {
			context->expectCall(context, context->writeString, NULL, "string_a");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeString, NULL, "a");
		} else if (!strcmp(keys[keyIndex], "string_foo")) {
			context->expectCall(context, context->writeString, NULL, "string_foo");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeString, NULL, "foo");
		} else if (!strcmp(keys[keyIndex], "blob_a")) {
			context->expectCall(context, context->writeString, NULL, "blob_a");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeBlob, NULL, "a", 1);
		} else if (!strcmp(keys[keyIndex], "blob_foo")) {
			context->expectCall(context, context->writeString, NULL, "blob_foo");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeBlob, NULL, "foo", 3);
		}
	}
	free(keys);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	DataValue_serialize(&value, (SerializationContext *) context);
	
	TestSerializationContext_finish(context);
	TestSerializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	value = valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues(
		"bool_0", valueCreateBoolean(false),
		"bool_1", valueCreateBoolean(true),
		"int8_0", valueCreateInt8(0),
		"int8_1", valueCreateInt8(1),
		"uint8_0", valueCreateUInt8(0),
		"uint8_1", valueCreateUInt8(1),
		"int16_0", valueCreateInt16(0),
		"int16_1", valueCreateInt16(1),
		"uint16_0", valueCreateUInt16(0),
		"uint16_1", valueCreateUInt16(1),
		"int32_0", valueCreateInt32(0),
		"int32_1", valueCreateInt32(1),
		"uint32_0", valueCreateUInt32(0),
		"uint32_1", valueCreateUInt32(1),
		"int64_0", valueCreateInt64(0),
		"int64_1", valueCreateInt64(1),
		"uint64_0", valueCreateUInt64(0),
		"uint64_1", valueCreateUInt64(1),
		"float_0", valueCreateFloat(0.0f),
		"float_1", valueCreateFloat(1.0f),
		"double_0", valueCreateDouble(0.0),
		"double_1", valueCreateDouble(1.0),
		"fixed_0", valueCreateFixed16_16(0x00000),
		"fixed_1", valueCreateFixed16_16(0x10000),
		"string_a", valueCreateString("a", DATA_USE_STRLEN, false, false),
		"string_foo", valueCreateString("foo", DATA_USE_STRLEN, false, false),
		"blob_a", valueCreateBlob("a", 1, false, false),
		"blob_foo", valueCreateBlob("foo", 3, false, false),
	NULL), true, false);
	
	context->expectCall(context, context->beginArray, "data");
	context->expectCall(context, context->writeString, NULL, "dictionary");
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeString, NULL, "bool_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, false);
	context->expectCall(context, context->writeString, NULL, "bool_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, true);
	context->expectCall(context, context->writeString, NULL, "int8_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt8, NULL, 0);
	context->expectCall(context, context->writeString, NULL, "int8_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt8, NULL, 1);
	context->expectCall(context, context->writeString, NULL, "uint8_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt8, NULL, 0);
	context->expectCall(context, context->writeString, NULL, "uint8_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt8, NULL, 1);
	context->expectCall(context, context->writeString, NULL, "int16_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt16, NULL, 0);
	context->expectCall(context, context->writeString, NULL, "int16_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt16, NULL, 1);
	context->expectCall(context, context->writeString, NULL, "uint16_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt16, NULL, 0);
	context->expectCall(context, context->writeString, NULL, "uint16_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt16, NULL, 1);
	context->expectCall(context, context->writeString, NULL, "int32_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt32, NULL, 0);
	context->expectCall(context, context->writeString, NULL, "int32_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt32, NULL, 1);
	context->expectCall(context, context->writeString, NULL, "uint32_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt32, NULL, 0);
	context->expectCall(context, context->writeString, NULL, "uint32_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt32, NULL, 1);
	context->expectCall(context, context->writeString, NULL, "int64_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt64, NULL, 0ll);
	context->expectCall(context, context->writeString, NULL, "int64_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt64, NULL, 1ll);
	context->expectCall(context, context->writeString, NULL, "uint64_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt64, NULL, 0ull);
	context->expectCall(context, context->writeString, NULL, "uint64_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeUInt64, NULL, 1ull);
	context->expectCall(context, context->writeString, NULL, "float_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeFloat, NULL, 0.0f);
	context->expectCall(context, context->writeString, NULL, "float_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeFloat, NULL, 1.0f);
	context->expectCall(context, context->writeString, NULL, "double_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeDouble, NULL, 0.0);
	context->expectCall(context, context->writeString, NULL, "double_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeDouble, NULL, 1.0);
	context->expectCall(context, context->writeString, NULL, "fixed_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeFixed16_16, NULL, 0x00000);
	context->expectCall(context, context->writeString, NULL, "fixed_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeFixed16_16, NULL, 0x10000);
	context->expectCall(context, context->writeString, NULL, "string_a");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeString, NULL, "a");
	context->expectCall(context, context->writeString, NULL, "string_foo");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeString, NULL, "foo");
	context->expectCall(context, context->writeString, NULL, "blob_a");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBlob, NULL, "a", 1);
	context->expectCall(context, context->writeString, NULL, "blob_foo");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBlob, NULL, "foo", 3);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	DataValue_serialize(&value, (SerializationContext *) context);
	
	TestSerializationContext_finish(context);
	TestSerializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	value = valueCreateArray(arrayCreateWithValues(
		valueCreateArray(arrayCreateWithValues(
			valueCreateBoolean(false)
		), true, false),
		valueCreateHashTable(hashCreateWithKeysAndValues(
			"bool_1", valueCreateBoolean(true),
		NULL), true, false),
		valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues(
			"int8_0", valueCreateInt8(0),
		NULL), true, false)
	), true, false);
	
	context->expectCall(context, context->beginArray, "data");
	context->expectCall(context, context->writeString, NULL, "array");
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, false);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_HASH_TABLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeString, NULL, "bool_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, true);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_ASSOCIATIVE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeString, NULL, "int8_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt8, NULL, 0);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	DataValue_serialize(&value, (SerializationContext *) context);
	
	TestSerializationContext_finish(context);
	TestSerializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	value = valueCreateHashTable(hashCreateWithKeysAndValues(
		"array", valueCreateArray(arrayCreateWithValues(
			valueCreateBoolean(false)
		), true, false),
		"hash_table", valueCreateHashTable(hashCreateWithKeysAndValues(
			"bool_1", valueCreateBoolean(true),
		NULL), true, false),
		"associative_array", valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues(
			"int8_0", valueCreateInt8(0),
		NULL), true, false),
	NULL), true, false);
	
	context->expectCall(context, context->beginArray, "data");
	context->expectCall(context, context->writeString, NULL, "structure");
	context->expectCall(context, context->beginArray, NULL);
	keys = hashGetKeys(value.value.hashTable, &keyCount);
	for (keyIndex = 0; keyIndex < keyCount; keyIndex++) {
		if (!strcmp(keys[keyIndex], "array")) {
			context->expectCall(context, context->writeString, NULL, "array");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->beginArray, NULL);
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeBoolean, NULL, false);
			context->expectCall(context, context->endArray, NULL);
			
		} else if (!strcmp(keys[keyIndex], "hash_table")) {
			context->expectCall(context, context->writeString, NULL, "hash_table");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_HASH_TABLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->beginArray, NULL);
			context->expectCall(context, context->writeString, NULL, "bool_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeBoolean, NULL, true);
			context->expectCall(context, context->endArray, NULL);
			
		} else if (!strcmp(keys[keyIndex], "associative_array")) {
			context->expectCall(context, context->writeString, NULL, "associative_array");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_ASSOCIATIVE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->beginArray, NULL);
			context->expectCall(context, context->writeString, NULL, "int8_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeInt8, NULL, 0);
			context->expectCall(context, context->endArray, NULL);
		}
	}
	free(keys);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	DataValue_serialize(&value, (SerializationContext *) context);
	
	TestSerializationContext_finish(context);
	TestSerializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	value = valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues(
		"array", valueCreateArray(arrayCreateWithValues(
			valueCreateBoolean(false)
		), true, false),
		"hash_table", valueCreateHashTable(hashCreateWithKeysAndValues(
			"bool_1", valueCreateBoolean(true),
		NULL), true, false),
		"associative_array", valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues(
			"int8_0", valueCreateInt8(0),
		NULL), true, false),
	NULL), true, false);
	
	context->expectCall(context, context->beginArray, "data");
	context->expectCall(context, context->writeString, NULL, "dictionary");
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeString, NULL, "array");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, false);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->writeString, NULL, "hash_table");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_HASH_TABLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeString, NULL, "bool_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, true);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->writeString, NULL, "associative_array");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_ASSOCIATIVE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeString, NULL, "int8_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeInt8, NULL, 0);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	DataValue_serialize(&value, (SerializationContext *) context);
	
	TestSerializationContext_finish(context);
	TestSerializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	value = valueCreateArray(arrayCreateWithValues(
		valueCreateBoolean(false),
		valueCreatePointer(NULL),
		valueCreateBoolean(true)
	), true, false);
	
	context->expectCall(context, context->beginArray, "data");
	context->expectCall(context, context->writeString, NULL, "array");
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, false);
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, true);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	DataValue_serialize(&value, (SerializationContext *) context);
	
	TestSerializationContext_finish(context);
	TestSerializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	value = valueCreateHashTable(hashCreateWithKeysAndValues(
		"bool_0", valueCreateBoolean(false),
		"pointer_0", valueCreatePointer(NULL),
		"bool_1", valueCreateBoolean(true),
	NULL), true, false);
	
	context->expectCall(context, context->beginArray, "data");
	context->expectCall(context, context->writeString, NULL, "structure");
	context->expectCall(context, context->beginArray, NULL);
	keys = hashGetKeys(value.value.hashTable, &keyCount);
	for (keyIndex = 0; keyIndex < keyCount; keyIndex++) {
		if (!strcmp(keys[keyIndex], "bool_0")) {
			context->expectCall(context, context->writeString, NULL, "bool_0");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeBoolean, NULL, false);
		} else if (!strcmp(keys[keyIndex], "bool_1")) {
			context->expectCall(context, context->writeString, NULL, "bool_1");
			context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
			context->expectCall(context, context->writeBoolean, NULL, true);
		}
	}
	free(keys);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	DataValue_serialize(&value, (SerializationContext *) context);
	
	TestSerializationContext_finish(context);
	TestSerializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestSerializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	value = valueCreateAssociativeArray(associativeArrayCreateWithKeysAndValues(
		"bool_0", valueCreateBoolean(false),
		"pointer_0", valueCreatePointer(NULL),
		"bool_1", valueCreateBoolean(true),
	NULL), true, false);
	
	context->expectCall(context, context->beginArray, "data");
	context->expectCall(context, context->writeString, NULL, "dictionary");
	context->expectCall(context, context->beginArray, NULL);
	context->expectCall(context, context->writeString, NULL, "bool_0");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, false);
	context->expectCall(context, context->writeString, NULL, "bool_1");
	context->expectCall(context, context->writeEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->writeBoolean, NULL, true);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	DataValue_serialize(&value, (SerializationContext *) context);
	
	TestSerializationContext_finish(context);
	TestSerializationContext_dispose(context);
	valueDispose(&value);
}

static void testDeserialize() {
	DataValue value, * hashTableValue;
	TestDeserializationContext * context;
	jmp_buf jmpEnv;
	const char ** keys;
	size_t keyIndex;
	unsigned int failIndex;
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginArray, "data", 2);
	context->expectCall(context, context->readString, NULL, "array");
	context->expectCall(context, context->beginArray, NULL, 56);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, false);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, true);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt8, NULL, 0);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt8, NULL, 1);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt8, NULL, 0);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt8, NULL, 1);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt16, NULL, 0);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt16, NULL, 1);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt16, NULL, 0);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt16, NULL, 1);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt32, NULL, 0);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt32, NULL, 1);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt32, NULL, 0);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt32, NULL, 1);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt64, NULL, 0ll);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt64, NULL, 1ll);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt64, NULL, 0ull);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt64, NULL, 1ull);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFloat, NULL, 0.0f);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFloat, NULL, 1.0f);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readDouble, NULL, 0.0);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readDouble, NULL, 1.0);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFixed16_16, NULL, 0x00000);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFixed16_16, NULL, 0x10000);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readString, NULL, "a");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readString, NULL, "foo");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBlob, NULL, "a", 1);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBlob, NULL, "foo", 3);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	value = DataValue_deserialize((DeserializationContext *) context);
	
	TestCase_assert(value.type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, value.type);
	TestCase_assert(value.value.array->count == 28, "Expected 28 but got " SIZE_T_FORMAT, value.value.array->count);
	TestCase_assert(value.value.array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value.value.array->values[0].type);
	TestCase_assert(!value.value.array->values[0].value.boolean, "Expected false but got true");
	TestCase_assert(value.value.array->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value.value.array->values[1].type);
	TestCase_assert(value.value.array->values[1].value.boolean, "Expected true but got false");
	TestCase_assert(value.value.array->values[2].type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value.value.array->values[2].type);
	TestCase_assert(value.value.array->values[2].value.int8 == 0, "Expected 0 but got %d", value.value.array->values[2].value.int8);
	TestCase_assert(value.value.array->values[3].type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value.value.array->values[3].type);
	TestCase_assert(value.value.array->values[3].value.int8 == 1, "Expected 1 but got %d", value.value.array->values[3].value.int8);
	TestCase_assert(value.value.array->values[4].type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, value.value.array->values[4].type);
	TestCase_assert(value.value.array->values[4].value.uint8 == 0, "Expected 0 but got %d", value.value.array->values[4].value.uint8);
	TestCase_assert(value.value.array->values[5].type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, value.value.array->values[5].type);
	TestCase_assert(value.value.array->values[5].value.uint8 == 1, "Expected 1 but got %d", value.value.array->values[5].value.uint8);
	TestCase_assert(value.value.array->values[6].type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, value.value.array->values[6].type);
	TestCase_assert(value.value.array->values[6].value.int16 == 0, "Expected 0 but got %d", value.value.array->values[6].value.int16);
	TestCase_assert(value.value.array->values[7].type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, value.value.array->values[7].type);
	TestCase_assert(value.value.array->values[7].value.int16 == 1, "Expected 1 but got %d", value.value.array->values[7].value.int16);
	TestCase_assert(value.value.array->values[8].type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, value.value.array->values[8].type);
	TestCase_assert(value.value.array->values[8].value.uint16 == 0, "Expected 0 but got %d", value.value.array->values[16].value.uint16);
	TestCase_assert(value.value.array->values[9].type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, value.value.array->values[9].type);
	TestCase_assert(value.value.array->values[9].value.uint16 == 1, "Expected 1 but got %d", value.value.array->values[9].value.uint16);
	TestCase_assert(value.value.array->values[10].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, value.value.array->values[10].type);
	TestCase_assert(value.value.array->values[10].value.int32 == 0, "Expected 0 but got %d", value.value.array->values[10].value.int32);
	TestCase_assert(value.value.array->values[11].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, value.value.array->values[11].type);
	TestCase_assert(value.value.array->values[11].value.int32 == 1, "Expected 1 but got %d", value.value.array->values[11].value.int32);
	TestCase_assert(value.value.array->values[12].type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, value.value.array->values[12].type);
	TestCase_assert(value.value.array->values[12].value.uint32 == 0, "Expected 0 but got %d", value.value.array->values[12].value.uint32);
	TestCase_assert(value.value.array->values[13].type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, value.value.array->values[13].type);
	TestCase_assert(value.value.array->values[13].value.uint32 == 1, "Expected 1 but got %d", value.value.array->values[13].value.uint32);
	TestCase_assert(value.value.array->values[14].type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, value.value.array->values[14].type);
	TestCase_assert(value.value.array->values[14].value.int64 == 0, "Expected 0 but got " INT64_FORMAT, value.value.array->values[14].value.int64);
	TestCase_assert(value.value.array->values[15].type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, value.value.array->values[15].type);
	TestCase_assert(value.value.array->values[15].value.int64 == 1, "Expected 1 but got " INT64_FORMAT, value.value.array->values[15].value.int64);
	TestCase_assert(value.value.array->values[16].type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, value.value.array->values[16].type);
	TestCase_assert(value.value.array->values[16].value.uint64 == 0, "Expected 0 but got " UINT64_FORMAT, value.value.array->values[16].value.uint64);
	TestCase_assert(value.value.array->values[17].type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, value.value.array->values[17].type);
	TestCase_assert(value.value.array->values[17].value.uint64 == 1, "Expected 1 but got " UINT64_FORMAT, value.value.array->values[17].value.uint64);
	TestCase_assert(value.value.array->values[18].type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, value.value.array->values[18].type);
	TestCase_assert(value.value.array->values[18].value.float32 == 0.0f, "Expected 0.0 but got %f", value.value.array->values[18].value.float32);
	TestCase_assert(value.value.array->values[19].type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, value.value.array->values[19].type);
	TestCase_assert(value.value.array->values[19].value.float32 == 1.0f, "Expected 1.0 but got %f", value.value.array->values[19].value.float32);
	TestCase_assert(value.value.array->values[20].type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, value.value.array->values[20].type);
	TestCase_assert(value.value.array->values[20].value.float64 == 0.0, "Expected 0.0 but got %f", value.value.array->values[20].value.float64);
	TestCase_assert(value.value.array->values[21].type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, value.value.array->values[21].type);
	TestCase_assert(value.value.array->values[21].value.float64 == 1.0, "Expected 1.0 but got %f", value.value.array->values[21].value.float64);
	TestCase_assert(value.value.array->values[22].type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, value.value.array->values[22].type);
	TestCase_assert(value.value.array->values[22].value.fixed == 0x00000, "Expected 0x00000 but got 0x%05X", value.value.array->values[22].value.fixed);
	TestCase_assert(value.value.array->values[23].type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, value.value.array->values[23].type);
	TestCase_assert(value.value.array->values[23].value.fixed == 0x10000, "Expected 0x10000 but got 0x%05X", value.value.array->values[23].value.fixed);
	TestCase_assert(value.value.array->values[24].type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, value.value.array->values[24].type);
	TestCase_assert(!strcmp(value.value.array->values[24].value.string, "a"), "Expected \"a\" but got \"%s\"", value.value.array->values[24].value.string);
	TestCase_assert(value.value.array->values[25].type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, value.value.array->values[25].type);
	TestCase_assert(!strcmp(value.value.array->values[25].value.string, "foo"), "Expected \"foo\" but got \"%s\"", value.value.array->values[25].value.string);
	TestCase_assert(value.value.array->values[26].type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, value.value.array->values[26].type);
	TestCase_assert(value.value.array->values[26].value.blob.length == 1, "Expected 1 but got " SIZE_T_FORMAT, value.value.array->values[26].value.blob.length);
	TestCase_assert(!memcmp(value.value.array->values[26].value.blob.bytes, "a", 1), "Expected \"a\" but got \"%.*s\"", (int) value.value.array->values[26].value.blob.length, (char *) value.value.array->values[26].value.blob.bytes);
	TestCase_assert(value.value.array->values[27].type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, value.value.array->values[27].type);
	TestCase_assert(value.value.array->values[27].value.blob.length == 3, "Expected 3 but got " SIZE_T_FORMAT, value.value.array->values[27].value.blob.length);
	TestCase_assert(!memcmp(value.value.array->values[27].value.blob.bytes, "foo", 3), "Expected \"foo\" but got \"%.*s\"", (int) value.value.array->values[27].value.blob.length, (char *) value.value.array->values[27].value.blob.bytes);
	
	TestDeserializationContext_finish(context);
	TestDeserializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginArray, "data", 2);
	context->expectCall(context, context->readString, NULL, "structure");
	context->expectCall(context, context->beginArray, NULL, 84);
	context->expectCall(context, context->readString, NULL, "bool_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, false);
	context->expectCall(context, context->readString, NULL, "bool_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, true);
	context->expectCall(context, context->readString, NULL, "int8_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt8, NULL, 0);
	context->expectCall(context, context->readString, NULL, "int8_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt8, NULL, 1);
	context->expectCall(context, context->readString, NULL, "uint8_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt8, NULL, 0);
	context->expectCall(context, context->readString, NULL, "uint8_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt8, NULL, 1);
	context->expectCall(context, context->readString, NULL, "int16_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt16, NULL, 0);
	context->expectCall(context, context->readString, NULL, "int16_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt16, NULL, 1);
	context->expectCall(context, context->readString, NULL, "uint16_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt16, NULL, 0);
	context->expectCall(context, context->readString, NULL, "uint16_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt16, NULL, 1);
	context->expectCall(context, context->readString, NULL, "int32_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt32, NULL, 0);
	context->expectCall(context, context->readString, NULL, "int32_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt32, NULL, 1);
	context->expectCall(context, context->readString, NULL, "uint32_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt32, NULL, 0);
	context->expectCall(context, context->readString, NULL, "uint32_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt32, NULL, 1);
	context->expectCall(context, context->readString, NULL, "int64_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt64, NULL, 0ll);
	context->expectCall(context, context->readString, NULL, "int64_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt64, NULL, 1ll);
	context->expectCall(context, context->readString, NULL, "uint64_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt64, NULL, 0ull);
	context->expectCall(context, context->readString, NULL, "uint64_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt64, NULL, 1ull);
	context->expectCall(context, context->readString, NULL, "float_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFloat, NULL, 0.0f);
	context->expectCall(context, context->readString, NULL, "float_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFloat, NULL, 1.0f);
	context->expectCall(context, context->readString, NULL, "double_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readDouble, NULL, 0.0);
	context->expectCall(context, context->readString, NULL, "double_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readDouble, NULL, 1.0);
	context->expectCall(context, context->readString, NULL, "fixed_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFixed16_16, NULL, 0x00000);
	context->expectCall(context, context->readString, NULL, "fixed_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFixed16_16, NULL, 0x10000);
	context->expectCall(context, context->readString, NULL, "string_a");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readString, NULL, "a");
	context->expectCall(context, context->readString, NULL, "string_foo");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readString, NULL, "foo");
	context->expectCall(context, context->readString, NULL, "blob_a");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBlob, NULL, "a", 1);
	context->expectCall(context, context->readString, NULL, "blob_foo");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBlob, NULL, "foo", 3);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	value = DataValue_deserialize((DeserializationContext *) context);
	
	TestCase_assert(value.type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, value.type);
	TestCase_assert(value.value.hashTable->count == 28, "Expected 28 but got " SIZE_T_FORMAT, value.value.hashTable->count);
	keys = hashGetKeys(value.value.hashTable, NULL);
	for (keyIndex = 0; keyIndex < value.value.hashTable->count; keyIndex++) {
		hashTableValue = hashGet(value.value.hashTable, keys[keyIndex]);
		if (!strcmp(keys[keyIndex], "bool_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, hashTableValue->type);
			TestCase_assert(!hashTableValue->value.boolean, "Expected false but got true");
		} else if (!strcmp(keys[keyIndex], "bool_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, hashTableValue->type);
			TestCase_assert(hashTableValue->value.boolean, "Expected true but got false");
		} else if (!strcmp(keys[keyIndex], "int8_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, hashTableValue->type);
			TestCase_assert(hashTableValue->value.int8 == 0, "Expected 0 but got %d", hashTableValue->value.int8);
		} else if (!strcmp(keys[keyIndex], "int8_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, hashTableValue->type);
			TestCase_assert(hashTableValue->value.int8 == 1, "Expected 1 but got %d", hashTableValue->value.int8);
		} else if (!strcmp(keys[keyIndex], "uint8_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, hashTableValue->type);
			TestCase_assert(hashTableValue->value.uint8 == 0, "Expected 0 but got %d", hashTableValue->value.uint8);
		} else if (!strcmp(keys[keyIndex], "uint8_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, hashTableValue->type);
			TestCase_assert(hashTableValue->value.uint8 == 1, "Expected 1 but got %d", hashTableValue->value.uint8);
		} else if (!strcmp(keys[keyIndex], "int16_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, hashTableValue->type);
			TestCase_assert(hashTableValue->value.int16 == 0, "Expected 0 but got %d", hashTableValue->value.int16);
		} else if (!strcmp(keys[keyIndex], "int16_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, hashTableValue->type);
			TestCase_assert(hashTableValue->value.int16 == 1, "Expected 1 but got %d", hashTableValue->value.int16);
		} else if (!strcmp(keys[keyIndex], "uint16_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, hashTableValue->type);
			TestCase_assert(hashTableValue->value.uint16 == 0, "Expected 0 but got %d", hashTableValue->value.uint16);
		} else if (!strcmp(keys[keyIndex], "uint16_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, hashTableValue->type);
			TestCase_assert(hashTableValue->value.uint16 == 1, "Expected 1 but got %d", hashTableValue->value.uint16);
		} else if (!strcmp(keys[keyIndex], "int32_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, hashTableValue->type);
			TestCase_assert(hashTableValue->value.int32 == 0, "Expected 0 but got %d", hashTableValue->value.int32);
		} else if (!strcmp(keys[keyIndex], "int32_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, hashTableValue->type);
			TestCase_assert(hashTableValue->value.int32 == 1, "Expected 1 but got %d", hashTableValue->value.int32);
		} else if (!strcmp(keys[keyIndex], "uint32_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, hashTableValue->type);
			TestCase_assert(hashTableValue->value.uint32 == 0, "Expected 0 but got %d", hashTableValue->value.uint32);
		} else if (!strcmp(keys[keyIndex], "uint32_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, hashTableValue->type);
			TestCase_assert(hashTableValue->value.uint32 == 1, "Expected 1 but got %d", hashTableValue->value.uint32);
		} else if (!strcmp(keys[keyIndex], "int64_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, hashTableValue->type);
			TestCase_assert(hashTableValue->value.int64 == 0, "Expected 0 but got " INT64_FORMAT, hashTableValue->value.int64);
		} else if (!strcmp(keys[keyIndex], "int64_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, hashTableValue->type);
			TestCase_assert(hashTableValue->value.int64 == 1, "Expected 1 but got " INT64_FORMAT, hashTableValue->value.int64);
		} else if (!strcmp(keys[keyIndex], "uint64_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, hashTableValue->type);
			TestCase_assert(hashTableValue->value.uint64 == 0, "Expected 0 but got " UINT64_FORMAT, hashTableValue->value.uint64);
		} else if (!strcmp(keys[keyIndex], "uint64_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, hashTableValue->type);
			TestCase_assert(hashTableValue->value.uint64 == 1, "Expected 1 but got " UINT64_FORMAT, hashTableValue->value.uint64);
		} else if (!strcmp(keys[keyIndex], "float_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, hashTableValue->type);
			TestCase_assert(hashTableValue->value.float32 == 0.0f, "Expected 0.0 but got %f", hashTableValue->value.float32);
		} else if (!strcmp(keys[keyIndex], "float_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, hashTableValue->type);
			TestCase_assert(hashTableValue->value.float32 == 1.0f, "Expected 1.0 but got %f", hashTableValue->value.float32);
		} else if (!strcmp(keys[keyIndex], "double_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, hashTableValue->type);
			TestCase_assert(hashTableValue->value.float64 == 0.0, "Expected 0.0 but got %f", hashTableValue->value.float64);
		} else if (!strcmp(keys[keyIndex], "double_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, hashTableValue->type);
			TestCase_assert(hashTableValue->value.float64 == 1.0, "Expected 1.0 but got %f", hashTableValue->value.float64);
		} else if (!strcmp(keys[keyIndex], "fixed_0")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, hashTableValue->type);
			TestCase_assert(hashTableValue->value.fixed == 0x00000, "Expected 0x00000 but got 0x%05X", hashTableValue->value.fixed);
		} else if (!strcmp(keys[keyIndex], "fixed_1")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, hashTableValue->type);
			TestCase_assert(hashTableValue->value.fixed == 0x10000, "Expected 0x10000 but got 0x%05X", hashTableValue->value.fixed);
		} else if (!strcmp(keys[keyIndex], "string_a")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, hashTableValue->type);
			TestCase_assert(!strcmp(hashTableValue->value.string, "a"), "Expected \"a\" but got \"%s\"", hashTableValue->value.string);
		} else if (!strcmp(keys[keyIndex], "string_foo")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, hashTableValue->type);
			TestCase_assert(!strcmp(hashTableValue->value.string, "foo"), "Expected \"foo\" but got \"%s\"", hashTableValue->value.string);
		} else if (!strcmp(keys[keyIndex], "blob_a")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, hashTableValue->type);
			TestCase_assert(hashTableValue->value.blob.length == 1, "Expected 1 but got " SIZE_T_FORMAT, hashTableValue->value.blob.length);
			TestCase_assert(!memcmp(hashTableValue->value.blob.bytes, "a", 1), "Expected \"a\" but got \"%.*s\"", (int) hashTableValue->value.blob.length, (char *) hashTableValue->value.blob.bytes);
		} else if (!strcmp(keys[keyIndex], "blob_foo")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, hashTableValue->type);
			TestCase_assert(hashTableValue->value.blob.length == 3, "Expected 3 but got " SIZE_T_FORMAT, hashTableValue->value.blob.length);
			TestCase_assert(!memcmp(hashTableValue->value.blob.bytes, "foo", 3), "Expected \"foo\" but got \"%.*s\"", (int) hashTableValue->value.blob.length, (char *) hashTableValue->value.blob.bytes);
		} else {
			TestCase_assert(false, "Unexpected key in hash table: \"%s\"", keys[keyIndex]);
		}
	}
	free(keys);
	
	TestDeserializationContext_finish(context);
	TestDeserializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginArray, "data", 2);
	context->expectCall(context, context->readString, NULL, "dictionary");
	context->expectCall(context, context->beginArray, NULL, 84);
	context->expectCall(context, context->readString, NULL, "bool_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, false);
	context->expectCall(context, context->readString, NULL, "bool_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, true);
	context->expectCall(context, context->readString, NULL, "int8_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt8, NULL, 0);
	context->expectCall(context, context->readString, NULL, "int8_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt8, NULL, 1);
	context->expectCall(context, context->readString, NULL, "uint8_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt8, NULL, 0);
	context->expectCall(context, context->readString, NULL, "uint8_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt8, NULL, 1);
	context->expectCall(context, context->readString, NULL, "int16_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt16, NULL, 0);
	context->expectCall(context, context->readString, NULL, "int16_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt16, NULL, 1);
	context->expectCall(context, context->readString, NULL, "uint16_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt16, NULL, 0);
	context->expectCall(context, context->readString, NULL, "uint16_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt16, NULL, 1);
	context->expectCall(context, context->readString, NULL, "int32_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt32, NULL, 0);
	context->expectCall(context, context->readString, NULL, "int32_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt32, NULL, 1);
	context->expectCall(context, context->readString, NULL, "uint32_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt32, NULL, 0);
	context->expectCall(context, context->readString, NULL, "uint32_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT32, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt32, NULL, 1);
	context->expectCall(context, context->readString, NULL, "int64_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt64, NULL, 0ll);
	context->expectCall(context, context->readString, NULL, "int64_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt64, NULL, 1ll);
	context->expectCall(context, context->readString, NULL, "uint64_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt64, NULL, 0ull);
	context->expectCall(context, context->readString, NULL, "uint64_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_UINT64, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readUInt64, NULL, 1ull);
	context->expectCall(context, context->readString, NULL, "float_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFloat, NULL, 0.0f);
	context->expectCall(context, context->readString, NULL, "float_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FLOAT, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFloat, NULL, 1.0f);
	context->expectCall(context, context->readString, NULL, "double_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readDouble, NULL, 0.0);
	context->expectCall(context, context->readString, NULL, "double_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_DOUBLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readDouble, NULL, 1.0);
	context->expectCall(context, context->readString, NULL, "fixed_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFixed16_16, NULL, 0x00000);
	context->expectCall(context, context->readString, NULL, "fixed_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_FIXED_16_16, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readFixed16_16, NULL, 0x10000);
	context->expectCall(context, context->readString, NULL, "string_a");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readString, NULL, "a");
	context->expectCall(context, context->readString, NULL, "string_foo");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_STRING, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readString, NULL, "foo");
	context->expectCall(context, context->readString, NULL, "blob_a");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBlob, NULL, "a", 1);
	context->expectCall(context, context->readString, NULL, "blob_foo");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BLOB, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBlob, NULL, "foo", 3);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	value = DataValue_deserialize((DeserializationContext *) context);
	
	TestCase_assert(value.type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, value.type);
	TestCase_assert(value.value.associativeArray->count == 28, "Expected 28 but got " SIZE_T_FORMAT, value.value.associativeArray->count);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[0], "bool_0"), "Expected \"bool_0\" but got \"%s\"", value.value.associativeArray->keys[0]);
	TestCase_assert(value.value.associativeArray->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value.value.associativeArray->values[0].type);
	TestCase_assert(!value.value.associativeArray->values[0].value.boolean, "Expected false but got true");
	TestCase_assert(!strcmp(value.value.associativeArray->keys[1], "bool_1"), "Expected \"bool_1\" but got \"%s\"", value.value.associativeArray->keys[1]);
	TestCase_assert(value.value.associativeArray->values[1].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value.value.associativeArray->values[1].type);
	TestCase_assert(value.value.associativeArray->values[1].value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(value.value.associativeArray->keys[2], "int8_0"), "Expected \"int8_0\" but got \"%s\"", value.value.associativeArray->keys[2]);
	TestCase_assert(value.value.associativeArray->values[2].type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value.value.associativeArray->values[2].type);
	TestCase_assert(value.value.associativeArray->values[2].value.int8 == 0, "Expected 0 but got %d", value.value.associativeArray->values[2].value.int8);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[3], "int8_1"), "Expected \"int8_1\" but got \"%s\"", value.value.associativeArray->keys[3]);
	TestCase_assert(value.value.associativeArray->values[3].type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value.value.associativeArray->values[3].type);
	TestCase_assert(value.value.associativeArray->values[3].value.int8 == 1, "Expected 1 but got %d", value.value.associativeArray->values[3].value.int8);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[4], "uint8_0"), "Expected \"uint8_0\" but got \"%s\"", value.value.associativeArray->keys[4]);
	TestCase_assert(value.value.associativeArray->values[4].type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, value.value.associativeArray->values[4].type);
	TestCase_assert(value.value.associativeArray->values[4].value.uint8 == 0, "Expected 0 but got %d", value.value.associativeArray->values[4].value.uint8);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[5], "uint8_1"), "Expected \"uint8_1\" but got \"%s\"", value.value.associativeArray->keys[5]);
	TestCase_assert(value.value.associativeArray->values[5].type == DATA_TYPE_UINT8, "Expected %d but got %d", DATA_TYPE_UINT8, value.value.associativeArray->values[5].type);
	TestCase_assert(value.value.associativeArray->values[5].value.uint8 == 1, "Expected 1 but got %d", value.value.associativeArray->values[5].value.uint8);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[6], "int16_0"), "Expected \"int16_0\" but got \"%s\"", value.value.associativeArray->keys[6]);
	TestCase_assert(value.value.associativeArray->values[6].type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, value.value.associativeArray->values[6].type);
	TestCase_assert(value.value.associativeArray->values[6].value.int16 == 0, "Expected 0 but got %d", value.value.associativeArray->values[6].value.int16);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[7], "int16_1"), "Expected \"int16_1\" but got \"%s\"", value.value.associativeArray->keys[7]);
	TestCase_assert(value.value.associativeArray->values[7].type == DATA_TYPE_INT16, "Expected %d but got %d", DATA_TYPE_INT16, value.value.associativeArray->values[7].type);
	TestCase_assert(value.value.associativeArray->values[7].value.int16 == 1, "Expected 1 but got %d", value.value.associativeArray->values[7].value.int16);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[8], "uint16_0"), "Expected \"uint16_0\" but got \"%s\"", value.value.associativeArray->keys[8]);
	TestCase_assert(value.value.associativeArray->values[8].type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, value.value.associativeArray->values[8].type);
	TestCase_assert(value.value.associativeArray->values[8].value.uint16 == 0, "Expected 0 but got %d", value.value.associativeArray->values[16].value.uint16);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[9], "uint16_1"), "Expected \"uint16_1\" but got \"%s\"", value.value.associativeArray->keys[9]);
	TestCase_assert(value.value.associativeArray->values[9].type == DATA_TYPE_UINT16, "Expected %d but got %d", DATA_TYPE_UINT16, value.value.associativeArray->values[9].type);
	TestCase_assert(value.value.associativeArray->values[9].value.uint16 == 1, "Expected 1 but got %d", value.value.associativeArray->values[9].value.uint16);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[10], "int32_0"), "Expected \"int32_0\" but got \"%s\"", value.value.associativeArray->keys[10]);
	TestCase_assert(value.value.associativeArray->values[10].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, value.value.associativeArray->values[10].type);
	TestCase_assert(value.value.associativeArray->values[10].value.int32 == 0, "Expected 0 but got %d", value.value.associativeArray->values[10].value.int32);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[11], "int32_1"), "Expected \"int32_1\" but got \"%s\"", value.value.associativeArray->keys[11]);
	TestCase_assert(value.value.associativeArray->values[11].type == DATA_TYPE_INT32, "Expected %d but got %d", DATA_TYPE_INT32, value.value.associativeArray->values[11].type);
	TestCase_assert(value.value.associativeArray->values[11].value.int32 == 1, "Expected 1 but got %d", value.value.associativeArray->values[11].value.int32);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[12], "uint32_0"), "Expected \"uint32_0\" but got \"%s\"", value.value.associativeArray->keys[12]);
	TestCase_assert(value.value.associativeArray->values[12].type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, value.value.associativeArray->values[12].type);
	TestCase_assert(value.value.associativeArray->values[12].value.uint32 == 0, "Expected 0 but got %d", value.value.associativeArray->values[12].value.uint32);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[13], "uint32_1"), "Expected \"uint32_1\" but got \"%s\"", value.value.associativeArray->keys[13]);
	TestCase_assert(value.value.associativeArray->values[13].type == DATA_TYPE_UINT32, "Expected %d but got %d", DATA_TYPE_UINT32, value.value.associativeArray->values[13].type);
	TestCase_assert(value.value.associativeArray->values[13].value.uint32 == 1, "Expected 1 but got %d", value.value.associativeArray->values[13].value.uint32);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[14], "int64_0"), "Expected \"int64_0\" but got \"%s\"", value.value.associativeArray->keys[14]);
	TestCase_assert(value.value.associativeArray->values[14].type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, value.value.associativeArray->values[14].type);
	TestCase_assert(value.value.associativeArray->values[14].value.int64 == 0, "Expected 0 but got " INT64_FORMAT, value.value.associativeArray->values[14].value.int64);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[15], "int64_1"), "Expected \"int64_1\" but got \"%s\"", value.value.associativeArray->keys[15]);
	TestCase_assert(value.value.associativeArray->values[15].type == DATA_TYPE_INT64, "Expected %d but got %d", DATA_TYPE_INT64, value.value.associativeArray->values[15].type);
	TestCase_assert(value.value.associativeArray->values[15].value.int64 == 1, "Expected 1 but got " INT64_FORMAT, value.value.associativeArray->values[15].value.int64);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[16], "uint64_0"), "Expected \"uint64_0\" but got \"%s\"", value.value.associativeArray->keys[16]);
	TestCase_assert(value.value.associativeArray->values[16].type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, value.value.associativeArray->values[16].type);
	TestCase_assert(value.value.associativeArray->values[16].value.uint64 == 0, "Expected 0 but got " UINT64_FORMAT, value.value.associativeArray->values[16].value.uint64);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[17], "uint64_1"), "Expected \"uint64_1\" but got \"%s\"", value.value.associativeArray->keys[17]);
	TestCase_assert(value.value.associativeArray->values[17].type == DATA_TYPE_UINT64, "Expected %d but got %d", DATA_TYPE_UINT64, value.value.associativeArray->values[17].type);
	TestCase_assert(value.value.associativeArray->values[17].value.uint64 == 1, "Expected 1 but got " UINT64_FORMAT, value.value.associativeArray->values[17].value.uint64);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[18], "float_0"), "Expected \"float_0\" but got \"%s\"", value.value.associativeArray->keys[18]);
	TestCase_assert(value.value.associativeArray->values[18].type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, value.value.associativeArray->values[18].type);
	TestCase_assert(value.value.associativeArray->values[18].value.float32 == 0.0f, "Expected 0.0 but got %f", value.value.associativeArray->values[18].value.float32);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[19], "float_1"), "Expected \"float_1\" but got \"%s\"", value.value.associativeArray->keys[19]);
	TestCase_assert(value.value.associativeArray->values[19].type == DATA_TYPE_FLOAT, "Expected %d but got %d", DATA_TYPE_FLOAT, value.value.associativeArray->values[19].type);
	TestCase_assert(value.value.associativeArray->values[19].value.float32 == 1.0f, "Expected 1.0 but got %f", value.value.associativeArray->values[19].value.float32);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[20], "double_0"), "Expected \"double_0\" but got \"%s\"", value.value.associativeArray->keys[20]);
	TestCase_assert(value.value.associativeArray->values[20].type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, value.value.associativeArray->values[20].type);
	TestCase_assert(value.value.associativeArray->values[20].value.float64 == 0.0, "Expected 0.0 but got %f", value.value.associativeArray->values[20].value.float64);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[21], "double_1"), "Expected \"double_1\" but got \"%s\"", value.value.associativeArray->keys[21]);
	TestCase_assert(value.value.associativeArray->values[21].type == DATA_TYPE_DOUBLE, "Expected %d but got %d", DATA_TYPE_DOUBLE, value.value.associativeArray->values[21].type);
	TestCase_assert(value.value.associativeArray->values[21].value.float64 == 1.0, "Expected 1.0 but got %f", value.value.associativeArray->values[21].value.float64);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[22], "fixed_0"), "Expected \"fixed_0\" but got \"%s\"", value.value.associativeArray->keys[22]);
	TestCase_assert(value.value.associativeArray->values[22].type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, value.value.associativeArray->values[22].type);
	TestCase_assert(value.value.associativeArray->values[22].value.fixed == 0x00000, "Expected 0x00000 but got 0x%05X", value.value.associativeArray->values[22].value.fixed);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[23], "fixed_1"), "Expected \"fixed_1\" but got \"%s\"", value.value.associativeArray->keys[23]);
	TestCase_assert(value.value.associativeArray->values[23].type == DATA_TYPE_FIXED_16_16, "Expected %d but got %d", DATA_TYPE_FIXED_16_16, value.value.associativeArray->values[23].type);
	TestCase_assert(value.value.associativeArray->values[23].value.fixed == 0x10000, "Expected 0x10000 but got 0x%05X", value.value.associativeArray->values[23].value.fixed);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[24], "string_a"), "Expected \"string_a\" but got \"%s\"", value.value.associativeArray->keys[24]);
	TestCase_assert(value.value.associativeArray->values[24].type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, value.value.associativeArray->values[24].type);
	TestCase_assert(!strcmp(value.value.associativeArray->values[24].value.string, "a"), "Expected \"a\" but got \"%s\"", value.value.associativeArray->values[24].value.string);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[25], "string_foo"), "Expected \"string_foo\" but got \"%s\"", value.value.associativeArray->keys[25]);
	TestCase_assert(value.value.associativeArray->values[25].type == DATA_TYPE_STRING, "Expected %d but got %d", DATA_TYPE_STRING, value.value.associativeArray->values[25].type);
	TestCase_assert(!strcmp(value.value.associativeArray->values[25].value.string, "foo"), "Expected \"foo\" but got \"%s\"", value.value.associativeArray->values[25].value.string);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[26], "blob_a"), "Expected \"blob_a\" but got \"%s\"", value.value.associativeArray->keys[26]);
	TestCase_assert(value.value.associativeArray->values[26].type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, value.value.associativeArray->values[26].type);
	TestCase_assert(value.value.associativeArray->values[26].value.blob.length == 1, "Expected 1 but got " SIZE_T_FORMAT, value.value.associativeArray->values[26].value.blob.length);
	TestCase_assert(!memcmp(value.value.associativeArray->values[26].value.blob.bytes, "a", 1), "Expected \"a\" but got \"%.*s\"", (int) value.value.associativeArray->values[26].value.blob.length, (char *) value.value.associativeArray->values[26].value.blob.bytes);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[27], "blob_foo"), "Expected \"blob_foo\" but got \"%s\"", value.value.associativeArray->keys[27]);
	TestCase_assert(value.value.associativeArray->values[27].type == DATA_TYPE_BLOB, "Expected %d but got %d", DATA_TYPE_BLOB, value.value.associativeArray->values[27].type);
	TestCase_assert(value.value.associativeArray->values[27].value.blob.length == 3, "Expected 3 but got " SIZE_T_FORMAT, value.value.associativeArray->values[27].value.blob.length);
	TestCase_assert(!memcmp(value.value.associativeArray->values[27].value.blob.bytes, "foo", 3), "Expected \"foo\" but got \"%.*s\"", (int) value.value.associativeArray->values[27].value.blob.length, (char *) value.value.associativeArray->values[27].value.blob.bytes);
	
	TestDeserializationContext_finish(context);
	TestDeserializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginArray, "data", 2);
	context->expectCall(context, context->readString, NULL, "array");
	context->expectCall(context, context->beginArray, NULL, 6);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL, 2);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, false);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_HASH_TABLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL, 3);
	context->expectCall(context, context->readString, NULL, "bool_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, true);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_ASSOCIATIVE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL, 3);
	context->expectCall(context, context->readString, NULL, "int8_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt8, NULL, 0);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	value = DataValue_deserialize((DeserializationContext *) context);
	
	TestCase_assert(value.type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, value.type);
	TestCase_assert(value.value.array->count == 3, "Expected 3 but got " SIZE_T_FORMAT, value.value.array->count);
	TestCase_assert(value.value.array->values[0].type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, value.value.array->values[0].type);
	TestCase_assert(value.value.array->values[0].value.array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, value.value.array->values[0].value.array->count);
	TestCase_assert(value.value.array->values[0].value.array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value.value.array->values[0].value.array->values[0].type);
	TestCase_assert(!value.value.array->values[0].value.array->values[0].value.boolean, "Expected false but got true");
	TestCase_assert(value.value.array->values[1].type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, value.value.array->values[1].type);
	TestCase_assert(value.value.array->values[1].value.hashTable->count == 1, "Expected 1 but got " SIZE_T_FORMAT, value.value.array->values[1].value.hashTable->count);
	hashTableValue = hashGet(value.value.array->values[1].value.hashTable, "bool_1");
	TestCase_assert(hashTableValue != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashTableValue->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, hashTableValue->type);
	TestCase_assert(hashTableValue->value.boolean, "Expected true but got false");
	TestCase_assert(value.value.array->values[2].type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, value.value.array->values[2].type);
	TestCase_assert(value.value.array->values[2].value.associativeArray->count == 1, "Expected 1 but got " SIZE_T_FORMAT, value.value.array->values[2].value.associativeArray->count);
	TestCase_assert(!strcmp(value.value.array->values[2].value.associativeArray->keys[0], "int8_0"), "Expected \"int8_0\" but got \"%s\"", value.value.array->values[2].value.associativeArray->keys[0]);
	TestCase_assert(value.value.array->values[2].value.associativeArray->values[0].type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value.value.array->values[2].value.associativeArray->values[0].type);
	TestCase_assert(value.value.array->values[2].value.associativeArray->values[0].value.int8 == 0, "Expected 0 but got %d", value.value.array->values[2].value.associativeArray->values[0].value.int8);
	
	TestDeserializationContext_finish(context);
	TestDeserializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginArray, "data", 2);
	context->expectCall(context, context->readString, NULL, "structure");
	context->expectCall(context, context->beginArray, NULL, 9);
	context->expectCall(context, context->readString, NULL, "array");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL, 2);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, false);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->readString, NULL, "hash_table");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_HASH_TABLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL, 3);
	context->expectCall(context, context->readString, NULL, "bool_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, true);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->readString, NULL, "associative_array");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_ASSOCIATIVE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL, 3);
	context->expectCall(context, context->readString, NULL, "int8_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt8, NULL, 0);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	value = DataValue_deserialize((DeserializationContext *) context);
	
	TestCase_assert(value.type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, value.type);
	TestCase_assert(value.value.hashTable->count == 3, "Expected 3 but got " SIZE_T_FORMAT, value.value.hashTable->count);
	keys = hashGetKeys(value.value.hashTable, NULL);
	for (keyIndex = 0; keyIndex < value.value.hashTable->count; keyIndex++) {
		hashTableValue = hashGet(value.value.hashTable, keys[keyIndex]);
		if (!strcmp(keys[keyIndex], "array")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, hashTableValue->type);
			TestCase_assert(hashTableValue->value.array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, hashTableValue->value.array->count);
			TestCase_assert(hashTableValue->value.array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, hashTableValue->value.array->values[0].type);
			TestCase_assert(!hashTableValue->value.array->values[0].value.boolean, "Expected false but got true");
			
		} else if (!strcmp(keys[keyIndex], "hash_table")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, hashTableValue->type);
			TestCase_assert(hashTableValue->value.hashTable->count == 1, "Expected 1 but got " SIZE_T_FORMAT, hashTableValue->value.hashTable->count);
			hashTableValue = hashGet(hashTableValue->value.hashTable, "bool_1");
			TestCase_assert(hashTableValue != NULL, "Expected non-NULL but got NULL");
			TestCase_assert(hashTableValue->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, hashTableValue->type);
			TestCase_assert(hashTableValue->value.boolean, "Expected true but got false");
			
		} else if (!strcmp(keys[keyIndex], "associative_array")) {
			TestCase_assert(hashTableValue->type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, hashTableValue->type);
			TestCase_assert(hashTableValue->value.associativeArray->count == 1, "Expected 1 but got " SIZE_T_FORMAT, hashTableValue->value.associativeArray->count);
			TestCase_assert(!strcmp(hashTableValue->value.associativeArray->keys[0], "int8_0"), "Expected \"int8_0\" but got \"%s\"", hashTableValue->value.associativeArray->keys[0]);
			TestCase_assert(hashTableValue->value.associativeArray->values[0].type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, hashTableValue->value.associativeArray->values[0].type);
			TestCase_assert(hashTableValue->value.associativeArray->values[0].value.int8 == 0, "Expected 0 but got %d", hashTableValue->value.associativeArray->values[0].value.int8);
			
		} else {
			TestCase_assert(false, "Unexpected key in hash table: \"%s\"", keys[keyIndex]);
		}
	}
	free(keys);
	
	TestDeserializationContext_finish(context);
	TestDeserializationContext_dispose(context);
	valueDispose(&value);
	
	context = TestDeserializationContext_create(&jmpEnv);
	if (setjmp(jmpEnv) != 0) {
		TestCase_assert(false, "%s", context->error);
	}
	
	context->expectCall(context, context->beginArray, "data", 2);
	context->expectCall(context, context->readString, NULL, "dictionary");
	context->expectCall(context, context->beginArray, NULL, 9);
	context->expectCall(context, context->readString, NULL, "array");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL, 2);
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, false);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->readString, NULL, "hash_table");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_HASH_TABLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL, 3);
	context->expectCall(context, context->readString, NULL, "bool_1");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readBoolean, NULL, true);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->readString, NULL, "associative_array");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_ASSOCIATIVE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->beginArray, NULL, 3);
	context->expectCall(context, context->readString, NULL, "int8_0");
	context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
	context->expectCall(context, context->readInt8, NULL, 0);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	context->expectCall(context, context->endArray);
	
	value = DataValue_deserialize((DeserializationContext *) context);
	
	TestCase_assert(value.type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, value.type);
	TestCase_assert(value.value.associativeArray->count == 3, "Expected 3 but got " SIZE_T_FORMAT, value.value.associativeArray->count);
	TestCase_assert(value.value.associativeArray->values[0].type == DATA_TYPE_ARRAY, "Expected %d but got %d", DATA_TYPE_ARRAY, value.value.associativeArray->values[0].type);
	TestCase_assert(!strcmp(value.value.associativeArray->keys[0], "array"), "Expected \"array\" but got \"%s\"", value.value.associativeArray->keys[0]);
	TestCase_assert(value.value.associativeArray->values[0].value.array->count == 1, "Expected 1 but got " SIZE_T_FORMAT, value.value.associativeArray->values[0].value.array->count);
	TestCase_assert(value.value.associativeArray->values[0].value.array->values[0].type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, value.value.associativeArray->values[0].value.array->values[0].type);
	TestCase_assert(!value.value.associativeArray->values[0].value.array->values[0].value.boolean, "Expected false but got true");
	TestCase_assert(!strcmp(value.value.associativeArray->keys[1], "hash_table"), "Expected \"hash_table\" but got \"%s\"", value.value.associativeArray->keys[1]);
	TestCase_assert(value.value.associativeArray->values[1].type == DATA_TYPE_HASH_TABLE, "Expected %d but got %d", DATA_TYPE_HASH_TABLE, value.value.associativeArray->values[1].type);
	TestCase_assert(value.value.associativeArray->values[1].value.hashTable->count == 1, "Expected 1 but got " SIZE_T_FORMAT, value.value.associativeArray->values[1].value.hashTable->count);
	hashTableValue = hashGet(value.value.associativeArray->values[1].value.hashTable, "bool_1");
	TestCase_assert(hashTableValue != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(hashTableValue->type == DATA_TYPE_BOOLEAN, "Expected %d but got %d", DATA_TYPE_BOOLEAN, hashTableValue->type);
	TestCase_assert(hashTableValue->value.boolean, "Expected true but got false");
	TestCase_assert(!strcmp(value.value.associativeArray->keys[2], "associative_array"), "Expected \"associative_array\" but got \"%s\"", value.value.associativeArray->keys[2]);
	TestCase_assert(value.value.associativeArray->values[2].type == DATA_TYPE_ASSOCIATIVE_ARRAY, "Expected %d but got %d", DATA_TYPE_ASSOCIATIVE_ARRAY, value.value.associativeArray->values[2].type);
	TestCase_assert(value.value.associativeArray->values[2].value.associativeArray->count == 1, "Expected 1 but got " SIZE_T_FORMAT, value.value.associativeArray->values[2].value.associativeArray->count);
	TestCase_assert(!strcmp(value.value.associativeArray->values[2].value.associativeArray->keys[0], "int8_0"), "Expected \"int8_0\" but got \"%s\"", value.value.associativeArray->values[2].value.associativeArray->keys[0]);
	TestCase_assert(value.value.associativeArray->values[2].value.associativeArray->values[0].type == DATA_TYPE_INT8, "Expected %d but got %d", DATA_TYPE_INT8, value.value.associativeArray->values[2].value.associativeArray->values[0].type);
	TestCase_assert(value.value.associativeArray->values[2].value.associativeArray->values[0].value.int8 == 0, "Expected 0 but got %d", value.value.associativeArray->values[2].value.associativeArray->values[0].value.int8);
	
	TestDeserializationContext_finish(context);
	TestDeserializationContext_dispose(context);
	valueDispose(&value);
	
	for (failIndex = 0; failIndex < 22; failIndex++) {
		context = TestDeserializationContext_create(&jmpEnv);
		if (setjmp(jmpEnv) != 0) {
			TestCase_assert(false, "%s (failIndex %u)", context->error, failIndex);
		}
		
		context->expectCall(context, context->beginArray, "data", 2);
		context->expectCall(context, context->readString, NULL, "array");
		context->expectCall(context, context->beginArray, NULL, 6);
		context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
		context->expectCall(context, context->beginArray, NULL, 2);
		context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
		context->expectCall(context, context->readBoolean, NULL, false);
		context->expectCall(context, context->endArray);
		context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_HASH_TABLE, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
		context->expectCall(context, context->beginArray, NULL, 3);
		context->expectCall(context, context->readString, NULL, "bool_1");
		context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_BOOLEAN, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
		context->expectCall(context, context->readBoolean, NULL, true);
		context->expectCall(context, context->endArray);
		context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_ASSOCIATIVE_ARRAY, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
		context->expectCall(context, context->beginArray, NULL, 3);
		context->expectCall(context, context->readString, NULL, "int8_0");
		context->expectCall(context, context->readEnumeration, NULL, DATA_TYPE_INT8, ALL_DATA_TYPE_ENUM_KEYS_AND_VALUES, NULL);
		context->expectCall(context, context->readInt8, NULL, 0);
		context->expectCall(context, context->endArray);
		context->expectCall(context, context->endArray);
		context->expectCall(context, context->endArray);
		context->failNthCall(context, failIndex, 1);
		
		value = DataValue_deserialize((DeserializationContext *) context);
		
		context->dispose(context);
		
		TestCase_assert(value.type == DATA_TYPE_BOOLEAN && !value.value.boolean, "DataValue_deserialize didn't return false when deserialization call %d failed", failIndex);
	}
}

TEST_SUITE(DataSerializerTest,
           testSerialize,
           testDeserialize)
