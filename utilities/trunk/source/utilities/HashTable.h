/*
  Copyright (c) 2013 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __HashTable_H__
#define __HashTable_H__

typedef struct HashTable HashTable;

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

enum HashTableValueType {
	HASH_TYPE_BOOLEAN,
	HASH_TYPE_INT8,
	HASH_TYPE_UINT8,
	HASH_TYPE_INT16,
	HASH_TYPE_UINT16,
	HASH_TYPE_INT32,
	HASH_TYPE_UINT32,
	HASH_TYPE_INT64,
	HASH_TYPE_UINT64,
	HASH_TYPE_FLOAT,
	HASH_TYPE_DOUBLE,
	HASH_TYPE_POINTER,
	HASH_TYPE_STRING,
	HASH_TYPE_BLOB,
	HASH_TYPE_HASH
};

#define HASH_USE_STRLEN ((size_t) -1)

struct HashTableEntry {
	const char * key;
	enum HashTableValueType type;
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
		const void * pointer;
		struct {
			char * bytes;
			size_t length;
		} string;
		struct {
			void * bytes;
			size_t length;
		} blob;
		struct HashTable * hashTable;
	} value;
};

struct HashTableBucket {
	size_t count;
	size_t allocatedCount;
	struct HashTableEntry * entries;
};

struct HashTable {
	size_t bucketCount;
	struct HashTableBucket * buckets;
	size_t keyCount;
};

/** Creates and initializes an empty hash table. */
HashTable * hashCreate();

/** Creates and returns a deep copy of the supplied hash table. New copies are created of entries which are
    HASH_TYPE_STRING, HASH_TYPE_BLOB, and HASH_TYPE_HASH. Note, however, that HASH_TYPE_POINTER is only copied
    shallowly; the copy will point to the same memory address as the original. */
HashTable * hashCopy(HashTable * hash);

/** Disposes the hash table and all entries in it over which the hash table has ownership. */
void hashDispose(HashTable * hash);

/** Returns true if an entry with the specified key exists in the hash table. Does not search recursively. */
bool hashHas(HashTable * hash, const char * key);

/** Removes the entry with the specified key, if it exists. A return value of true indicates that the item
    was successfully removed; false indicates that it was not found in the hash table. */
bool hashDelete(HashTable * hash, const char * key);

/** Return a pointer to the HashTableEntry struct corresponding to the specified key, or NULL if it doesn't
    exist in the hash table. */
struct HashTableEntry * hashLookup(HashTable * hash, const char * key);

/** Returns the total number of entries in the hash table. */
size_t hashGetCount(HashTable * hash);

/** Returns an array containing all keys that exist in the hash table. The caller is responsible for freeing
    the returned pointer. However, the elements contained in this array must NOT be freed, only the array
    itself.
    
    Although you can pass NULL for outCount and count the items with hashGetCount, it's recommended to instead
    retrieve the number of items with outCount. */
const char ** hashGetKeys(HashTable * hash, size_t * outCount);

/** Sets the specified value for the specified key in the hash table. If a value with the same key already
    exists, it will be overwritten. */
void hashSetBoolean(  HashTable * hash, const char * key, bool value);
void hashSetInt8(     HashTable * hash, const char * key, int8_t value);
void hashSetUInt8(    HashTable * hash, const char * key, uint8_t value);
void hashSetInt16(    HashTable * hash, const char * key, int16_t value);
void hashSetUInt16(   HashTable * hash, const char * key, uint16_t value);
void hashSetInt32(    HashTable * hash, const char * key, int32_t value);
void hashSetUInt32(   HashTable * hash, const char * key, uint32_t value);
void hashSetInt64(    HashTable * hash, const char * key, int64_t value);
void hashSetUInt64(   HashTable * hash, const char * key, uint64_t value);
void hashSetFloat(    HashTable * hash, const char * key, float value);
void hashSetDouble(   HashTable * hash, const char * key, double value);
void hashSetPointer(  HashTable * hash, const char * key, const void * value);

/** These three make a copy of their inputs, so the caller retains ownership of the data. The copies will be
    freed automatically when the corresponding key is deleted. In a situation where you want to retain full
    ownership of data of any of these types without HashTable copying it, use hashSetPointer() instead.
    
    An additional note on hashSetString: If you want the string's length to be determined by strlen(), pass
    HASH_USE_STRLEN for the length parameter. */
void hashSetString(   HashTable * hash, const char * key, const char * value, size_t length);
void hashSetBlob(     HashTable * hash, const char * key, const void * value, size_t length);
void hashSetHashTable(HashTable * hash, const char * key, HashTable * value);

/** Returns the corresponding value for the specified key in the hash table, or false/0/NULL if it doesn't
    exist or is of a different type. Be aware that this does not function as a way to check if the entry is
    present, since false, 0, and NULL are all valid return values for these types. If you need to verify that
    an entry exists for the given key, use hashHas(). */
bool               hashGetBoolean(  HashTable * hash, const char * key);
int8_t             hashGetInt8(     HashTable * hash, const char * key);
uint8_t            hashGetUInt8(    HashTable * hash, const char * key);
int16_t            hashGetInt16(    HashTable * hash, const char * key);
uint16_t           hashGetUInt16(   HashTable * hash, const char * key);
int32_t            hashGetInt32(    HashTable * hash, const char * key);
uint32_t           hashGetUInt32(   HashTable * hash, const char * key);
int64_t            hashGetInt64(    HashTable * hash, const char * key);
uint64_t           hashGetUInt64(   HashTable * hash, const char * key);
float              hashGetFloat(    HashTable * hash, const char * key);
double             hashGetDouble(   HashTable * hash, const char * key);
const void *       hashGetPointer(  HashTable * hash, const char * key);

/** Returned values are owned by HashTable and must not be altered or freed by the caller. */
const char *       hashGetString(   HashTable * hash, const char * key, size_t * outLength);
const void *       hashGetBlob(     HashTable * hash, const char * key, size_t * outLength);
HashTable *        hashGetHashTable(HashTable * hash, const char * key);

#endif
