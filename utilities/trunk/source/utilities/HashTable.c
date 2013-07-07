#include "utilities/HashTable.h"
#include "utilities/lookup3.h"
#include <string.h>

#define DEFAULT_BUCKET_COUNT 128

HashTable * hashCreate() {
	HashTable * hash;
	
	hash = malloc(sizeof(HashTable));
	hash->keyCount = 0;
	hash->bucketCount = DEFAULT_BUCKET_COUNT;
	hash->buckets = calloc(sizeof(struct HashTableBucket), hash->bucketCount);
	return hash;
}

HashTable * hashCopy(HashTable * hash) {
	return NULL;
}

void hashDispose(HashTable * hash) {
	size_t bucketIndex, entryIndex;
	
	for (bucketIndex = 0; bucketIndex < hash->bucketCount; bucketIndex++) {
		for (entryIndex = 0; entryIndex < hash->buckets[bucketIndex].count; entryIndex++) {
			switch (hash->buckets[bucketIndex].entries[entryIndex].type) {
				case HASH_TYPE_STRING:
					free(hash->buckets[bucketIndex].entries[entryIndex].value.string.bytes);
					break;
					
				case HASH_TYPE_BLOB:
					free(hash->buckets[bucketIndex].entries[entryIndex].value.blob.bytes);
					break;
					
				case HASH_TYPE_HASH:
					hashDispose(hash->buckets[bucketIndex].entries[entryIndex].value.hashTable);
					break;
					
				default:
					break;
			}
			free((char *) hash->buckets[bucketIndex].entries[entryIndex].key);
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
	return NULL;
}

static void hashSet(HashTable * hash, struct HashTableEntry entry) {
	size_t bucketIndex;
	
	bucketIndex = hashlittle(entry.key, strlen(entry.key), 0) % hash->bucketCount;
	if (hash->buckets[bucketIndex].allocatedCount == 0) {
		hash->buckets[bucketIndex].allocatedCount = 1;
	} else {
		hash->buckets[bucketIndex].allocatedCount *= 2;
	}
	hash->buckets[bucketIndex].entries = realloc(hash->buckets[bucketIndex].entries, sizeof(struct HashTableEntry) * hash->buckets[bucketIndex].allocatedCount);
	entry.key = strdup(entry.key);
	hash->buckets[bucketIndex].entries[hash->buckets[bucketIndex].count++] = entry;
	hash->keyCount++;
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

void hashSetPointer(HashTable * hash, const char * key, void * value) {
}

void hashSetString(HashTable * hash, const char * key, const char * value, size_t length) {
}

void hashSetBlob(HashTable * hash, const char * key, void * value,       size_t length) {
}

void hashSetHashTable(HashTable * hash, const char * key, HashTable * value) {
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

void * hashGetPointer(HashTable * hash, const char * key) {
	return NULL;
}

const char * hashGetString(HashTable * hash, const char * key, size_t * outLength) {
	return NULL;
}

void * hashGetBlob(HashTable * hash, const char * key, size_t * outLength) {
	return NULL;
}

HashTable * hashGetHashTable(HashTable * hash, const char * key) {
	return NULL;
}
