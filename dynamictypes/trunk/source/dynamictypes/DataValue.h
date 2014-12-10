/*
  Copyright (c) 2014 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener alex@ludobloom.com
*/

#ifndef __DataValue_H__
#define __DataValue_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataValue DataValue;

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

struct AssociativeArray;
struct DataArray;
struct HashTable;

enum DataValueType {
	DATA_TYPE_BOOLEAN,
	DATA_TYPE_INT8,
	DATA_TYPE_UINT8,
	DATA_TYPE_INT16,
	DATA_TYPE_UINT16,
	DATA_TYPE_INT32,
	DATA_TYPE_UINT32,
	DATA_TYPE_INT64,
	DATA_TYPE_UINT64,
	DATA_TYPE_FLOAT,
	DATA_TYPE_DOUBLE,
	DATA_TYPE_POINTER,
	DATA_TYPE_STRING,
	DATA_TYPE_BLOB,
	DATA_TYPE_HASH_TABLE,
	DATA_TYPE_ARRAY,
	DATA_TYPE_ASSOCIATIVE_ARRAY
};

#define DATA_USE_STRLEN ((size_t) -1)

struct DataValue {
	union {
		bool boolean;
		int8_t int8;
		uint8_t uint8;
		int16_t int16;
		uint16_t uint16;
		int32_t int32;
		uint32_t uint32;
		int64_t int64;
		uint64_t uint64;
		float float32;
		double float64;
		void * pointer;
		const char * string;
		struct {
			const void * bytes;
			size_t length;
		} blob;
		struct HashTable * hashTable;
		struct DataArray * array;
		struct AssociativeArray * associativeArray;
	} value;
	enum DataValueType type;
	bool owned;
};

/** Initializes a DataValue struct with the specified primitive type and value. */
DataValue valueCreateBoolean(bool value);
DataValue valueCreateInt8(int8_t value);
DataValue valueCreateUInt8(uint8_t value);
DataValue valueCreateInt16(int16_t value);
DataValue valueCreateUInt16(uint16_t value);
DataValue valueCreateInt32(int32_t value);
DataValue valueCreateUInt32(uint32_t value);
DataValue valueCreateInt64(int64_t value);
DataValue valueCreateUInt64(uint64_t value);
DataValue valueCreateFloat(float value);
DataValue valueCreateDouble(double value);
DataValue valueCreatePointer(void * value);

/** Initializes a DataValue struct with the specified non-primitive type and value.
    You can pass DATA_USE_STRLEN for the length argument to valueCreateString to have it measured automatically.
    If takeOwnership is true, the allocated memory will be freed when valueDispose is called on this DataValue.
    If copy is true, a copy of the first argument will be created in the returned DataValue. Note that if
    takeOwnership is false and copy is true, the caller is responsible for freeing the pointer inside the
    returned DataValue. If takeOwnership is true and copy is false, the first argument must be heap-allocated,
    because free() will be called on it when the value is disposed. */
DataValue valueCreateString(const char * value, size_t length, bool takeOwnership, bool copy);
DataValue valueCreateBlob(const void * value, size_t length, bool takeOwnership, bool copy);
DataValue valueCreateHashTable(struct HashTable * value, bool takeOwnership, bool copy);
DataValue valueCreateArray(struct DataArray * value, bool takeOwnership, bool copy);
DataValue valueCreateAssociativeArray(struct AssociativeArray * value, bool takeOwnership, bool copy);

/** Initializes a copy of the specified DataValue struct. If a heap-allocated value is owned by the DataValue
    being copied, the returned DataValue will contain a new copy of it. If the DataValue being copied contains
    a non-owned non-primitive type, the copied DataValue will refer to the same memory as the original. */
DataValue valueCopy(DataValue * value);

/** Frees memory owned by this DataValue, if any. This function does nothing for DataValues with primitive
    types, or DataValues with non-owned non-primitive types. */
void valueDispose(DataValue * value);

/** Returns the raw value contained by a DataValue, or false/0/NULL if the specified DataValue is of a type
    that does not match the function being called, or is NULL. Numeric types will be automatically converted
    if possible, though results are undefined for values too large to be converted to the target type. */
bool valueGetBoolean(DataValue * value);
int8_t valueGetInt8(DataValue * value);
uint8_t valueGetUInt8(DataValue * value);
int16_t valueGetInt16(DataValue * value);
uint16_t valueGetUInt16(DataValue * value);
int32_t valueGetInt32(DataValue * value);
uint32_t valueGetUInt32(DataValue * value);
int64_t valueGetInt64(DataValue * value);
uint64_t valueGetUInt64(DataValue * value);
float valueGetFloat(DataValue * value);
double valueGetDouble(DataValue * value);
void * valueGetPointer(DataValue * value);
const char * valueGetString(DataValue * value);
const void * valueGetBlob(DataValue * value, size_t * outLength);
struct HashTable * valueGetHashTable(DataValue * value);
struct DataArray * valueGetArray(DataValue * value);
struct AssociativeArray * valueGetAssociativeArray(DataValue * value);

#ifdef __cplusplus
}
#endif
#endif
