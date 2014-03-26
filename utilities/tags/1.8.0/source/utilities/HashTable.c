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

#include "utilities/HashTable.h"
#include "utilities/lookup3.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_DENSITY 5
#define MIN_SIZE 8

static const size_t primes[] = {
	8 + 3,
	16 + 3,
	32 + 5,
	64 + 3,
	128 + 3,
	256 + 27,
	512 + 9,
	1024 + 9,
	2048 + 5,
	4096 + 3,
	8192 + 27,
	16384 + 43,
	32768 + 3,
	65536 + 45,
	131072 + 29,
	262144 + 3,
	524288 + 21,
	1048576 + 7,
	2097152 + 17,
	4194304 + 15,
	8388608 + 9,
	16777216 + 43,
	33554432 + 35,
	67108864 + 15,
	134217728 + 29,
	268435456 + 3,
	536870912 + 11,
	1073741824 + 85,
	0
};

static size_t nextPrimeSize(unsigned int value) {
	unsigned int primeIndex, newValue;
	
	newValue = MIN_SIZE;
	for (primeIndex = 0; primeIndex < sizeof(primes) / sizeof(primes[0]); primeIndex++) {
		if (newValue > value) {
			return primes[primeIndex];
		}
		newValue <<= 1;
	}
	fprintf(stderr, "Warning: Hash table grew too large; ran out of precomputed primes! size = %u\n", value);
	return value;
}

HashTable * hashCreate(size_t structMaxSize) {
	HashTable * hash;
	
	hash = malloc(sizeof(HashTable));
	hash->structMaxSize = structMaxSize;
	if (structMaxSize <= sizeof(union HashTableEntryValue)) {
		hash->entrySize = sizeof(struct HashTableEntry);
	} else {
		hash->entrySize = sizeof(struct HashTableEntry) - sizeof(union HashTableEntryValue) + structMaxSize;
	}
	if (structMaxSize > 0) {
		hash->notFoundStructPtr = calloc(1, structMaxSize);
	} else {
		hash->notFoundStructPtr = NULL;
	}
	hash->keyCount = 0;
	hash->bucketCount = nextPrimeSize(0);
	hash->buckets = calloc(sizeof(struct HashTableBucket), hash->bucketCount);
	return hash;
}

static struct HashTableEntry * rehashGetEntry(struct HashTableBucket * buckets, size_t entrySize, size_t bucketIndex, size_t entryIndex) {
	return buckets[bucketIndex].entries + entryIndex * entrySize;
}

static void rehashNewEntrySlot(HashTable * hash, const char * key, struct HashTableBucket * buckets, size_t bucketCount, size_t * outBucketIndex, size_t * outEntryIndex) {
	size_t bucketIndex, entryIndex;
	unsigned int hashValue;
	
	hashValue = hashlittle(key, strlen(key), 0);
	bucketIndex = hashValue % bucketCount;
	for (entryIndex = 0; entryIndex < buckets[bucketIndex].count; entryIndex++) {
		if (!strcmp(rehashGetEntry(buckets, hash->entrySize, bucketIndex, entryIndex)->key, key)) {
			break;
		}
	}
	if (entryIndex >= buckets[bucketIndex].count) {
		if (buckets[bucketIndex].allocatedCount == 0) {
			buckets[bucketIndex].allocatedCount = 1;
		} else {
			buckets[bucketIndex].allocatedCount *= 2;
		}
		buckets[bucketIndex].entries = realloc(buckets[bucketIndex].entries, hash->entrySize * buckets[bucketIndex].allocatedCount);
		buckets[bucketIndex].count++;
	}
	*outBucketIndex = bucketIndex;
	*outEntryIndex = entryIndex;
}

static void rehash(HashTable * hash) {
	struct HashTableEntry * entryOld, * entryNew;
	size_t newBucketCount, bucketIndex, entryIndex, bucketIndexNew = 0, entryIndexNew = 0;
	struct HashTableBucket * newBuckets;
	
	newBucketCount = nextPrimeSize(hash->bucketCount + 1);
	newBuckets = calloc(sizeof(struct HashTableBucket), newBucketCount);
	
	for (bucketIndex = 0; bucketIndex < hash->bucketCount; bucketIndex++) {
		for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
			entryOld = rehashGetEntry(hash->buckets, hash->structMaxSize, bucketIndex, entryIndex);
			rehashNewEntrySlot(hash, entryOld->key, newBuckets, newBucketCount, &bucketIndexNew, &entryIndexNew);
			entryNew = rehashGetEntry(newBuckets, hash->entrySize, bucketIndexNew, entryIndexNew);
			memcpy(entryNew, entryOld, hash->entrySize);
		}
		free(hash->buckets[bucketIndex].entries);
	}
	free(hash->buckets);
	hash->bucketCount = newBucketCount;
	hash->buckets = newBuckets;
}

static struct HashTableEntry * getEntry(HashTable * hash, size_t bucketIndex, size_t entryIndex) {
	return hash->buckets[bucketIndex].entries + entryIndex * hash->entrySize;
}

static void newEntrySlot(HashTable * hash, const char * key, size_t * outBucketIndex, size_t * outEntryIndex) {
	size_t bucketIndex, entryIndex;
	unsigned int hashValue;
	
	hashValue = hashlittle(key, strlen(key), 0);
	bucketIndex = hashValue % hash->bucketCount;
	for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
		if (!strcmp(getEntry(hash, bucketIndex, entryIndex)->key, key)) {
			break;
		}
	}
	if (entryIndex >= hash->buckets[bucketIndex].count) {
    if (hash->keyCount / hash->bucketCount > MAX_DENSITY) {
			rehash(hash);
			bucketIndex = hashValue % hash->bucketCount;
    }
		if (hash->buckets[bucketIndex].allocatedCount == 0) {
			hash->buckets[bucketIndex].allocatedCount = 1;
		} else {
			hash->buckets[bucketIndex].allocatedCount *= 2;
		}
		hash->buckets[bucketIndex].entries = realloc(hash->buckets[bucketIndex].entries, hash->entrySize * hash->buckets[bucketIndex].allocatedCount);
		hash->buckets[bucketIndex].count++;
		hash->keyCount++;
	}
	*outBucketIndex = bucketIndex;
	*outEntryIndex = entryIndex;
}

static void hashSet(HashTable * hash, struct HashTableEntry entry) {
	size_t bucketIndex = 0, entryIndex = 0;
	
	newEntrySlot(hash, entry.key, &bucketIndex, &entryIndex);
	entry.key = strdup(entry.key);
	*getEntry(hash, bucketIndex, entryIndex) = entry;
}

HashTable * hashCopy(HashTable * hash) {
	HashTable * copy;
	size_t count = 0, keyIndex;
	struct HashTableEntry * entry;
	const char ** keys;
	
	copy = hashCreate(hash->structMaxSize);
	keys = hashGetKeys(hash, &count);
	for (keyIndex = 0; keyIndex < count; keyIndex++) {
		entry = hashLookup(hash, keys[keyIndex]);
		switch (entry->type) {
			case  HASH_TYPE_STRING:
				hashSetString(copy, entry->key, entry->value.string.bytes, entry->value.string.length);
				break;
				
			case HASH_TYPE_BLOB:
				hashSetBlob(copy, entry->key, entry->value.blob.bytes, entry->value.blob.length);
				break;
				
			case HASH_TYPE_HASH:
				hashSetHashTable(copy, entry->key, entry->value.hashTable);
				break;
				
			default:
				hashSet(copy, *entry);
				break;
		}
	}
	return copy;
}

static void disposeEntry(struct HashTableEntry * entry) {
	switch (entry->type) {
		case HASH_TYPE_STRING:
			free(entry->value.string.bytes);
			break;
			
		case HASH_TYPE_BLOB:
			free(entry->value.blob.bytes);
			break;
			
		case HASH_TYPE_HASH:
			hashDispose(entry->value.hashTable);
			break;
			
		default:
			break;
	}
	free((char *) entry->key);
}

void hashDispose(HashTable * hash) {
	size_t bucketIndex, entryIndex;
	
	for (bucketIndex = 0; bucketIndex < hash->bucketCount; bucketIndex++) {
		for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
			disposeEntry(getEntry(hash, bucketIndex, entryIndex));
		}
		free(hash->buckets[bucketIndex].entries);
	}
	free(hash->buckets);
	free(hash->notFoundStructPtr);
	free(hash);
}

bool hashHas(HashTable * hash, const char * key) {
	return hashLookup(hash, key) != NULL;
}

bool hashDelete(HashTable * hash, const char * key) {
	size_t bucketIndex, entryIndex;
	
	bucketIndex = hashlittle(key, strlen(key), 0) % hash->bucketCount;
	for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
		if (!strcmp(key, getEntry(hash, bucketIndex, entryIndex)->key)) {
			disposeEntry(getEntry(hash, bucketIndex, entryIndex));
			hash->buckets[bucketIndex].count--;
			for (; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
				*getEntry(hash, bucketIndex, entryIndex) = *getEntry(hash, bucketIndex, entryIndex + 1);
			}
			hash->keyCount--;
			return true;
		}
	}
	return false;
}

struct HashTableEntry * hashLookup(HashTable * hash, const char * key) {
	size_t bucketIndex, entryIndex;
	struct HashTableEntry * entry;
	
	bucketIndex = hashlittle(key, strlen(key), 0) % hash->bucketCount;
	for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
		entry = getEntry(hash, bucketIndex, entryIndex);
		if (!strcmp(key, entry->key)) {
			return entry;
		}
	}
	return NULL;
}

size_t hashGetCount(HashTable * hash) {
	return hash->keyCount;
}

const char ** hashGetKeys(HashTable * hash, size_t * outCount) {
	const char ** keys;
	size_t keyIndex = 0, bucketIndex, entryIndex;
	
	keys = malloc(sizeof(const char *) * hash->keyCount);
	for (bucketIndex = 0; bucketIndex < hash->bucketCount; bucketIndex++) {
		for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
			if (keyIndex >= hash->keyCount) {
				fprintf(stderr, "Internal error: Hash table's key count is less than its actual number of keys!\n");
				return NULL;
			}
			keys[keyIndex++] = getEntry(hash, bucketIndex, entryIndex)->key;
		}
	}
	if (keyIndex < hash->keyCount) {
		fprintf(stderr, "Internal error: Hash table's key count is greater than its actual number of keys!\n");
		return NULL;
	}
	if (outCount != NULL) {
		*outCount = hash->keyCount;
	}
	return keys;
}

void hashSetBoolean(HashTable * hash, const char * key, bool value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_BOOLEAN, {.boolean = value}});
}

void hashSetInt8(HashTable * hash, const char * key, int8_t value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_INT8, {.int8 = value}});
}

void hashSetUInt8(HashTable * hash, const char * key, uint8_t value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_UINT8, {.uint8 = value}});
}

void hashSetInt16(HashTable * hash, const char * key, int16_t value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_INT16, {.int16 = value}});
}

void hashSetUInt16(HashTable * hash, const char * key, uint16_t value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_UINT16, {.uint16 = value}});
}

void hashSetInt32(HashTable * hash, const char * key, int32_t value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_INT32, {.int32 = value}});
}

void hashSetUInt32(HashTable * hash, const char * key, uint32_t value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_UINT32, {.uint32 = value}});
}

void hashSetInt64(HashTable * hash, const char * key, int64_t value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_INT64, {.int64 = value}});
}

void hashSetUInt64(HashTable * hash, const char * key, uint64_t value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_UINT64, {.uint64 = value}});
}

void hashSetFloat(HashTable * hash, const char * key, float value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_FLOAT, {.float32 = value}});
}

void hashSetDouble(HashTable * hash, const char * key, double value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_DOUBLE, {.float64 = value}});
}

void hashSetPointer(HashTable * hash, const char * key, const void * value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_POINTER, {.pointer = value}});
}

void hashSetStructPtr(HashTable * hash, const char * key, size_t size, void * value) {
	size_t bucketIndex = 0, entryIndex = 0;
	struct HashTableEntry * entry;
	
	newEntrySlot(hash, key, &bucketIndex, &entryIndex);
	entry = (struct HashTableEntry *) (hash->buckets[bucketIndex].entries + entryIndex * hash->entrySize);
	entry->key = strdup(key);
	entry->type = HASH_TYPE_STRUCT;
	memcpy(((void *) entry) + offsetof(struct HashTableEntry, value), value, size);
}

void hashSetString(HashTable * hash, const char * key, const char * value, size_t length) {
	struct HashTableEntry entry;
	
	entry.key = key;
	entry.type = HASH_TYPE_STRING;
	if (length == HASH_USE_STRLEN) {
		length = strlen(value);
	}
	entry.value.string.bytes = malloc(length + 1);
	strncpy(entry.value.string.bytes, value, length);
	entry.value.string.bytes[length] = 0;
	entry.value.string.length = length;
	hashSet(hash, entry);
}

void hashSetBlob(HashTable * hash, const char * key, const void * value, size_t length) {
	struct HashTableEntry entry;
	
	entry.key = key;
	entry.type = HASH_TYPE_BLOB;
	entry.value.blob.bytes = malloc(length);
	memcpy(entry.value.blob.bytes, value, length);
	entry.value.string.length = length;
	hashSet(hash, entry);
}

void hashSetHashTable(HashTable * hash, const char * key, HashTable * value) {
	hashSet(hash, (struct HashTableEntry) {key, HASH_TYPE_HASH, {.hashTable = hashCopy(value)}});
}

#define hashGetPrimitive(hash, key, expected_type, field, default_value) { \
	struct HashTableEntry * entry; \
	\
	entry = hashLookup(hash, key); \
	if (entry == NULL || entry->type != expected_type) { \
		return default_value; \
	} \
	return entry->value.field; \
}

bool hashGetBoolean(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_BOOLEAN, boolean, false)
}

int8_t hashGetInt8(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_INT8, int8, 0)
}

uint8_t hashGetUInt8(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_UINT8, uint8, 0)
}

int16_t hashGetInt16(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_INT16, int16, 0)
}

uint16_t hashGetUInt16(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_UINT16, uint16, 0)
}

int32_t hashGetInt32(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_INT32, int32, 0)
}

uint32_t hashGetUInt32(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_UINT32, uint32, 0)
}

int64_t hashGetInt64(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_INT64, int64, 0)
}

uint64_t hashGetUInt64(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_UINT64, uint64, 0)
}

float hashGetFloat(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_FLOAT, float32, 0.0f)
}

double hashGetDouble(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_DOUBLE, float64, 0.0)
}

const void * hashGetPointer(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_POINTER, pointer, NULL)
}

void * hashGetStructPtr(HashTable * hash, const char * key) {
	struct HashTableEntry * entry;
	
	entry = hashLookup(hash, key);
	if (entry == NULL || entry->type != HASH_TYPE_STRUCT) {
		return hash->notFoundStructPtr;
	}
	return ((void *) entry) + offsetof(struct HashTableEntry, value);
}

const char * hashGetString(HashTable * hash, const char * key, size_t * outLength) {
	struct HashTableEntry * entry;
	
	entry = hashLookup(hash, key);
	if (entry == NULL || entry->type != HASH_TYPE_STRING) {
		return NULL;
	}
	if (outLength != NULL) {
		*outLength = entry->value.string.length;
	}
	return entry->value.string.bytes;
}

const void * hashGetBlob(HashTable * hash, const char * key, size_t * outLength) {
	struct HashTableEntry * entry;
	
	entry = hashLookup(hash, key);
	if (entry == NULL || entry->type != HASH_TYPE_BLOB) {
		return NULL;
	}
	if (outLength != NULL) {
		*outLength = entry->value.blob.length;
	}
	return entry->value.blob.bytes;
}

HashTable * hashGetHashTable(HashTable * hash, const char * key) {
	hashGetPrimitive(hash, key, HASH_TYPE_HASH, hashTable, NULL)
}
