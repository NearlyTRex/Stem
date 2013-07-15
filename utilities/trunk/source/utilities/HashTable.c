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
#include <stdio.h>

#define DEFAULT_BUCKET_COUNT 128

HashTable * hashCreate() {
	HashTable * hash;
	
	hash = malloc(sizeof(HashTable));
	hash->keyCount = 0;
	hash->bucketCount = DEFAULT_BUCKET_COUNT;
	hash->buckets = calloc(sizeof(struct HashTableBucket), hash->bucketCount);
	return hash;
}

static void hashSet(HashTable * hash, struct HashTableEntry entry) {
	size_t bucketIndex, entryIndex;
	
	bucketIndex = hashlittle(entry.key, strlen(entry.key), 0) % hash->bucketCount;
	for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
		if (!strcmp(hash->buckets[bucketIndex].entries[entryIndex].key, entry.key)) {
			break;
		}
	}
	if (entryIndex >= hash->buckets[bucketIndex].count) {
		if (hash->buckets[bucketIndex].allocatedCount == 0) {
			hash->buckets[bucketIndex].allocatedCount = 1;
		} else {
			hash->buckets[bucketIndex].allocatedCount *= 2;
		}
		hash->buckets[bucketIndex].entries = realloc(hash->buckets[bucketIndex].entries, sizeof(struct HashTableEntry) * hash->buckets[bucketIndex].allocatedCount);
		hash->buckets[bucketIndex].count++;
		hash->keyCount++;
	}
	entry.key = strdup(entry.key);
	hash->buckets[bucketIndex].entries[entryIndex] = entry;
}

HashTable * hashCopy(HashTable * hash) {
	HashTable * copy;
	size_t count = 0, keyIndex;
	struct HashTableEntry * entry;
	const char ** keys;
	
	copy = hashCreate();
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

static void disposeEntry(struct HashTableEntry entry) {
	switch (entry.type) {
		case HASH_TYPE_STRING:
			free(entry.value.string.bytes);
			break;
			
		case HASH_TYPE_BLOB:
			free(entry.value.blob.bytes);
			break;
			
		case HASH_TYPE_HASH:
			hashDispose(entry.value.hashTable);
			break;
			
		default:
			break;
	}
	free((char *) entry.key);
}

void hashDispose(HashTable * hash) {
	size_t bucketIndex, entryIndex;
	
	for (bucketIndex = 0; bucketIndex < hash->bucketCount; bucketIndex++) {
		for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
			disposeEntry(hash->buckets[bucketIndex].entries[entryIndex]);
		}
		free(hash->buckets[bucketIndex].entries);
	}
	free(hash->buckets);
	free(hash);
}

bool hashHas(HashTable * hash, const char * key) {
	return hashLookup(hash, key) != NULL;
}

bool hashDelete(HashTable * hash, const char * key) {
	size_t bucketIndex, entryIndex;
	
	bucketIndex = hashlittle(key, strlen(key), 0) % hash->bucketCount;
	for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
		if (!strcmp(key, hash->buckets[bucketIndex].entries[entryIndex].key)) {
			disposeEntry(hash->buckets[bucketIndex].entries[entryIndex]);
			hash->buckets[bucketIndex].count--;
			for (; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
				hash->buckets[bucketIndex].entries[entryIndex] = hash->buckets[bucketIndex].entries[entryIndex + 1];
			}
			hash->keyCount--;
			return true;
		}
	}
	return false;
}

struct HashTableEntry * hashLookup(HashTable * hash, const char * key) {
	size_t bucketIndex, entryIndex;
	
	bucketIndex = hashlittle(key, strlen(key), 0) % hash->bucketCount;
	for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
		if (!strcmp(key, hash->buckets[bucketIndex].entries[entryIndex].key)) {
			return &hash->buckets[bucketIndex].entries[entryIndex];
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
			keys[keyIndex++] = hash->buckets[bucketIndex].entries[entryIndex].key;
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
