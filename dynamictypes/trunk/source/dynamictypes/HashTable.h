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

#ifndef __HashTable_H__
#define __HashTable_H__

typedef struct HashTable HashTable;

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dynamictypes/DataValue.h"

struct HashTableEntry {
	const char * key;
	DataValue value;
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

/** Returns an empty hash table. */
HashTable * hashCreate();

/** Returns a hash table initialized with the specified keys and values. */
HashTable * hashCreateWithKeysAndValues(const char * key, DataValue value, ...) __attribute__((sentinel));

/** Returns a deep copy of a hash table. */
HashTable * hashCopy(HashTable * hash);

/** Disposes the hash table and all entries in it over which the hash table has ownership. */
void hashDispose(HashTable * hash);

/** Returns true if an entry with the specified key exists in the hash table. Does not search recursively. */
bool hashHas(HashTable * hash, const char * key);

/** Removes the entry with the specified key, if it exists. A return value of true indicates that the item
    was successfully removed; false indicates that it was not found in the hash table. */
bool hashDelete(HashTable * hash, const char * key);

/** Return a pointer to the DataValue corresponding to the specified key, or NULL if it doesn't exist in
    the hash table. Note that modifying the hash table may invalidate this pointer, so be sure to make a
    local copy of its contents if you need to cache it. */
DataValue * hashGet(HashTable * hash, const char * key);

/** Sets the specified value for the specified key in the hash table. If a value with the same key already
    exists, it will be overwritten. */
void hashSet(HashTable * hash, const char * key, DataValue value);

/** Returns an array containing all keys that exist in the hash table. The caller is responsible for freeing
    the returned pointer. However, the elements contained in this array must NOT be freed, only the array
    itself.
    
    Although you can pass NULL for outCount and count the items with hashGetCount, it's recommended to instead
    retrieve the number of items with outCount. */
const char ** hashGetKeys(HashTable * hash, size_t * outCount);

#endif
