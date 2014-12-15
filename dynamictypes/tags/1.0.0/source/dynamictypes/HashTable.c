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

#include "dynamictypes/HashTable.h"
#include "dynamictypes/lookup3.h"
#include "utilities/printfFormats.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

HashTable * hashCreate() {
	HashTable * hash;
	
	hash = malloc(sizeof(HashTable));
	hash->count = 0;
	hash->bucketCount = nextPrimeSize(0);
	hash->buckets = calloc(sizeof(struct HashTableBucket), hash->bucketCount);
	return hash;
}

HashTable * hashCreateWithKeysAndValues(const char * key, DataValue value, ...) {
	HashTable * hash;
	va_list args;
	
	hash = hashCreate();
	hashSet(hash, key, value);
	va_start(args, value);
	while ((key = va_arg(args, const char *)) != NULL) {
		value = va_arg(args, DataValue);
		hashSet(hash, key, value);
	}
	return hash;
}

static void rehashNewEntrySlot(HashTable * hash, const char * key, struct HashTableBucket * buckets, size_t bucketCount, size_t * outBucketIndex, size_t * outEntryIndex) {
	size_t bucketIndex, entryIndex;
	unsigned int hashValue;
	
	hashValue = hashlittle(key, strlen(key), 0);
	bucketIndex = hashValue % bucketCount;
	for (entryIndex = 0; entryIndex < buckets[bucketIndex].count; entryIndex++) {
		if (!strcmp(buckets[bucketIndex].entries[entryIndex].key, key)) {
			break;
		}
	}
	if (entryIndex >= buckets[bucketIndex].count) {
		if (buckets[bucketIndex].allocatedCount == 0) {
			buckets[bucketIndex].allocatedCount = 1;
		} else {
			buckets[bucketIndex].allocatedCount *= 2;
		}
		buckets[bucketIndex].entries = realloc(buckets[bucketIndex].entries, sizeof(struct HashTableEntry) * buckets[bucketIndex].allocatedCount);
		buckets[bucketIndex].count++;
	}
	*outBucketIndex = bucketIndex;
	*outEntryIndex = entryIndex;
}

static void rehash(HashTable * hash) {
	struct HashTableEntry entry;
	size_t newBucketCount, bucketIndex, entryIndex, bucketIndexNew = 0, entryIndexNew = 0;
	struct HashTableBucket * newBuckets;
	
	newBucketCount = nextPrimeSize(hash->bucketCount + 1);
	newBuckets = calloc(sizeof(struct HashTableBucket), newBucketCount);
	
	for (bucketIndex = 0; bucketIndex < hash->bucketCount; bucketIndex++) {
		for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
			entry = hash->buckets[bucketIndex].entries[entryIndex];
			rehashNewEntrySlot(hash, entry.key, newBuckets, newBucketCount, &bucketIndexNew, &entryIndexNew);
			newBuckets[bucketIndexNew].entries[entryIndexNew] = entry;
		}
		free(hash->buckets[bucketIndex].entries);
	}
	free(hash->buckets);
	hash->bucketCount = newBucketCount;
	hash->buckets = newBuckets;
}

static void newEntrySlot(HashTable * hash, const char * key, size_t * outBucketIndex, size_t * outEntryIndex) {
	size_t bucketIndex, entryIndex;
	unsigned int hashValue;
	
	hashValue = hashlittle(key, strlen(key), 0);
	bucketIndex = hashValue % hash->bucketCount;
	for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
		if (!strcmp(hash->buckets[bucketIndex].entries[entryIndex].key, key)) {
			break;
		}
	}
	if (entryIndex >= hash->buckets[bucketIndex].count) {
    if (hash->count / hash->bucketCount > MAX_DENSITY) {
			rehash(hash);
			bucketIndex = hashValue % hash->bucketCount;
			for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
				if (!strcmp(hash->buckets[bucketIndex].entries[entryIndex].key, key)) {
					break;
				}
			}
    }
		if (hash->buckets[bucketIndex].allocatedCount == 0) {
			hash->buckets[bucketIndex].allocatedCount = 1;
		} else {
			hash->buckets[bucketIndex].allocatedCount *= 2;
		}
		hash->buckets[bucketIndex].entries = realloc(hash->buckets[bucketIndex].entries, sizeof(struct HashTableEntry) * hash->buckets[bucketIndex].allocatedCount);
		hash->buckets[bucketIndex].count++;
		hash->count++;
	}
	*outBucketIndex = bucketIndex;
	*outEntryIndex = entryIndex;
}

HashTable * hashCopy(HashTable * hash) {
	HashTable * copy;
	size_t count = 0, keyIndex;
	const char ** keys;
	
	copy = hashCreate();
	keys = hashGetKeys(hash, &count);
	for (keyIndex = 0; keyIndex < count; keyIndex++) {
		hashSet(copy, keys[keyIndex], valueCopy(hashGet(hash, keys[keyIndex])));
	}
	return copy;
}

static void disposeEntry(struct HashTableEntry * entry) {
	free((char *) entry->key);
	valueDispose(&entry->value);
}

void hashDispose(HashTable * hash) {
	size_t bucketIndex, entryIndex;
	
	for (bucketIndex = 0; bucketIndex < hash->bucketCount; bucketIndex++) {
		for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
			disposeEntry(&hash->buckets[bucketIndex].entries[entryIndex]);
		}
		free(hash->buckets[bucketIndex].entries);
	}
	free(hash->buckets);
	free(hash);
}

bool hashHas(HashTable * hash, const char * key) {
	return hashGet(hash, key) != NULL;
}

bool hashDelete(HashTable * hash, const char * key) {
	size_t bucketIndex, entryIndex;
	
	bucketIndex = hashlittle(key, strlen(key), 0) % hash->bucketCount;
	for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
		if (!strcmp(key, hash->buckets[bucketIndex].entries[entryIndex].key)) {
			disposeEntry(&hash->buckets[bucketIndex].entries[entryIndex]);
			hash->buckets[bucketIndex].count--;
			for (; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
				hash->buckets[bucketIndex].entries[entryIndex] = hash->buckets[bucketIndex].entries[entryIndex + 1];
			}
			hash->count--;
			return true;
		}
	}
	return false;
}

static struct HashTableEntry * hashGetEntry(HashTable * hash, const char * key) {
	size_t bucketIndex, entryIndex;
	struct HashTableEntry * entry;
	
	bucketIndex = hashlittle(key, strlen(key), 0) % hash->bucketCount;
	for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
		entry = &hash->buckets[bucketIndex].entries[entryIndex];
		if (!strcmp(key, entry->key)) {
			return entry;
		}
	}
	return NULL;
}

DataValue * hashGet(HashTable * hash, const char * key) {
	struct HashTableEntry * entry;
	
	entry = hashGetEntry(hash, key);
	if (entry == NULL) {
		return NULL;
	}
	return &entry->value;
}

void hashSet(HashTable * hash, const char * key, DataValue value) {
	struct HashTableEntry * entry;
	
	entry = hashGetEntry(hash, key);
	if (entry == NULL) {
		size_t bucketIndex = 0, entryIndex = 0;
		
		newEntrySlot(hash, key, &bucketIndex, &entryIndex);
		entry = &hash->buckets[bucketIndex].entries[entryIndex];
		
	} else {
		disposeEntry(entry);
	}
	entry->key = strdup(key);
	entry->value = value;
}

size_t hashGetCount(HashTable * hash) {
	return hash->count;
}

const char ** hashGetKeys(HashTable * hash, size_t * outCount) {
	const char ** keys;
	size_t keyIndex = 0, bucketIndex, entryIndex;
	
	keys = malloc(sizeof(const char *) * hash->count);
	for (bucketIndex = 0; bucketIndex < hash->bucketCount; bucketIndex++) {
		for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
			if (keyIndex >= hash->count) {
				fprintf(stderr, "Internal error: Hash table's key count is less than its actual number of keys!\n");
				return NULL;
			}
			keys[keyIndex++] = hash->buckets[bucketIndex].entries[entryIndex].key;
			if (keys[keyIndex - 1] == NULL) {
				fprintf(stderr, "Internal error: Key " SIZE_T_FORMAT " is NULL!\n", keyIndex - 1);
				return NULL;
			}
		}
	}
	if (keyIndex < hash->count) {
		fprintf(stderr, "Internal error: Hash table's key count is greater than its actual number of keys!\n");
		return NULL;
	}
	if (outCount != NULL) {
		*outCount = hash->count;
	}
	return keys;
}
