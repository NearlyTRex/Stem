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

#include "utilities/Atom.h"
#include "utilities/lookup3.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ATOM_HASH_TABLE_SIZE 1024

struct AtomEntry {
	Atom atom;
	uint32_t hash;
};

struct AtomBucket {
	size_t count;
	size_t allocatedSize;
	struct AtomEntry * entries;
};

static struct AtomBucket atomBuckets[ATOM_HASH_TABLE_SIZE];
static MUTEX_TYPE mutex;
static void (* lockMutex)(MUTEX_TYPE);
static void (* unlockMutex)(MUTEX_TYPE);

void Atom_initMutex(MUTEX_TYPE (* createMutexFunc)(void), void (* lockMutexFunc)(MUTEX_TYPE), void (* unlockMutexFunc)(MUTEX_TYPE)) {
	if (mutex == NULL) {
		mutex = createMutexFunc();
		lockMutex = lockMutexFunc;
		unlockMutex = unlockMutexFunc;
	}
}

Atom Atom_fromString(const char * string) {
	unsigned int bucketIndex, entryIndex;
	char * newAtom;
	size_t length;
	uint32_t hash;
	
	if (string == NULL) {
		return NULL;
	}
	
	length = strlen(string);
	hash = hashlittle(string, length, 0);
	bucketIndex = hash % ATOM_HASH_TABLE_SIZE;
	
	if (mutex != NULL) {
		lockMutex(mutex);
	}
	
	for (entryIndex = 0; entryIndex < atomBuckets[bucketIndex].count; entryIndex++) {
		if (atomBuckets[bucketIndex].entries[entryIndex].hash == hash && !strcmp(atomBuckets[bucketIndex].entries[entryIndex].atom, string)) {
			if (mutex != NULL) {
				unlockMutex(mutex);
			}
			return atomBuckets[bucketIndex].entries[entryIndex].atom;
		}
	}
	
	if (atomBuckets[bucketIndex].count >= atomBuckets[bucketIndex].allocatedSize) {
		if (atomBuckets[bucketIndex].allocatedSize == 0) {
			atomBuckets[bucketIndex].allocatedSize = 1;
		} else {
			atomBuckets[bucketIndex].allocatedSize *= 2;
		}
		atomBuckets[bucketIndex].entries = realloc(atomBuckets[bucketIndex].entries, sizeof(struct AtomEntry) * atomBuckets[bucketIndex].allocatedSize);
	}
	
	newAtom = strdup(string);
	atomBuckets[bucketIndex].entries[atomBuckets[bucketIndex].count].atom = newAtom;
	atomBuckets[bucketIndex].entries[atomBuckets[bucketIndex].count].hash = hash;
	atomBuckets[bucketIndex].count++;
	
	if (mutex != NULL) {
		unlockMutex(mutex);
	}
	return newAtom;
}
