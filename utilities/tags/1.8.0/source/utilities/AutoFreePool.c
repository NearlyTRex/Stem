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

#include "utilities/AutoFreePool.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct AutoFreePool_callback {
	void (* callback)(void * context);
	void * context;
};

struct AutoFreePool {
	unsigned int numberOfAddresses;
	unsigned int addressListSize;
	void ** addresses;
	
	unsigned int numberOfCallbacks;
	unsigned int callbackListSize;
	struct AutoFreePool_callback * callbacks;
};

#define MAX_POOL_STACK_DEPTH 16

static struct AutoFreePool poolStack[MAX_POOL_STACK_DEPTH];
static int poolStackDepth = -1;
static MUTEX_TYPE mutex;
static void (* lockMutex)(MUTEX_TYPE);
static void (* unlockMutex)(MUTEX_TYPE);

void AutoFreePool_initMutex(MUTEX_TYPE (* createMutexFunc)(void), void (* lockMutexFunc)(MUTEX_TYPE), void (* unlockMutexFunc)(MUTEX_TYPE)) {
	if (mutex == NULL) {
		mutex = createMutexFunc();
		lockMutex = lockMutexFunc;
		unlockMutex = unlockMutexFunc;
	}
}

void AutoFreePool_push() {
	if (mutex != NULL) {
		lockMutex(mutex);
	}
	
	if (poolStackDepth >= MAX_POOL_STACK_DEPTH - 1) {
		fprintf(stderr, "Warning: AutoFreePool maximum stack depth (%d) exceeded; couldn't push another pool as requested\n", MAX_POOL_STACK_DEPTH);
		if (mutex != NULL) {
			unlockMutex(mutex);
		}
		return;
	}
	
	poolStackDepth++;
	poolStack[poolStackDepth].addresses = NULL;
	poolStack[poolStackDepth].callbacks = NULL;
	
	if (mutex != NULL) {
		unlockMutex(mutex);
	}
}

void AutoFreePool_pop() {
	if (mutex != NULL) {
		lockMutex(mutex);
	}
	
	if (poolStackDepth < 0) {
		fprintf(stderr, "Warning: AutoFreePool stack underflow; you've popped more times than you've pushed\n");
		if (mutex != NULL) {
			unlockMutex(mutex);
		}
		return;
	}
	
	AutoFreePool_empty();
	free(poolStack[poolStackDepth].addresses);
	free(poolStack[poolStackDepth].callbacks);
	poolStackDepth--;
	if (mutex != NULL) {
		unlockMutex(mutex);
	}
}

void * AutoFreePool_add(void * address) {
	if (mutex != NULL) {
		lockMutex(mutex);
	}
	
	if (poolStackDepth == -1) {
		if (mutex != NULL) {
			unlockMutex(mutex);
		}
		AutoFreePool_push();
		if (mutex != NULL) {
			lockMutex(mutex);
		}
	}
	
	if (poolStack[poolStackDepth].addresses == NULL) {
		poolStack[poolStackDepth].numberOfAddresses = 0;
		poolStack[poolStackDepth].addressListSize = 1;
		poolStack[poolStackDepth].addresses = malloc(sizeof(void *) * poolStack[poolStackDepth].addressListSize);
		
	} else if (poolStack[poolStackDepth].numberOfAddresses >= poolStack[poolStackDepth].addressListSize) {
		poolStack[poolStackDepth].addressListSize *= 2;
		poolStack[poolStackDepth].addresses = realloc(poolStack[poolStackDepth].addresses, sizeof(void *) * poolStack[poolStackDepth].addressListSize);
	}
	
	poolStack[poolStackDepth].addresses[poolStack[poolStackDepth].numberOfAddresses++] = address;
	if (mutex != NULL) {
		unlockMutex(mutex);
	}
	
	return address;
}

void AutoFreePool_addCallback(void (* callback)(void * context), void * context) {
	if (mutex != NULL) {
		lockMutex(mutex);
	}
	
	if (poolStackDepth == -1) {
		if (mutex != NULL) {
			unlockMutex(mutex);
		}
		AutoFreePool_push();
		if (mutex != NULL) {
			lockMutex(mutex);
		}
	}
	
	if (poolStack[poolStackDepth].callbacks == NULL) {
		poolStack[poolStackDepth].numberOfCallbacks = 0;
		poolStack[poolStackDepth].callbackListSize = 1;
		poolStack[poolStackDepth].callbacks = malloc(sizeof(struct AutoFreePool_callback) * poolStack[poolStackDepth].callbackListSize);
		
	} else if (poolStack[poolStackDepth].numberOfCallbacks >= poolStack[poolStackDepth].callbackListSize) {
		poolStack[poolStackDepth].callbackListSize *= 2;
		poolStack[poolStackDepth].callbacks = realloc(poolStack[poolStackDepth].callbacks, sizeof(struct AutoFreePool_callback) * poolStack[poolStackDepth].callbackListSize);
	}
	
	poolStack[poolStackDepth].callbacks[poolStack[poolStackDepth].numberOfCallbacks].callback = callback;
	poolStack[poolStackDepth].callbacks[poolStack[poolStackDepth].numberOfCallbacks].context = context;
	poolStack[poolStackDepth].numberOfCallbacks++;
	if (mutex != NULL) {
		unlockMutex(mutex);
	}
}

void AutoFreePool_empty() {
	if (mutex != NULL) {
		lockMutex(mutex);
	}
	
	if (poolStackDepth == -1) {
		if (mutex != NULL) {
			unlockMutex(mutex);
		}
		return;
	}
	
	if (poolStack[poolStackDepth].addresses != NULL) {
		unsigned int addressIndex;
		
		for (addressIndex = 0; addressIndex < poolStack[poolStackDepth].numberOfAddresses; addressIndex++) {
			free(poolStack[poolStackDepth].addresses[addressIndex]);
		}
		poolStack[poolStackDepth].numberOfAddresses = 0;
	}
	
	if (poolStack[poolStackDepth].callbacks != NULL) {
		unsigned int callbackIndex;
		
		for (callbackIndex = 0; callbackIndex < poolStack[poolStackDepth].numberOfCallbacks; callbackIndex++) {
			poolStack[poolStackDepth].callbacks[callbackIndex].callback(poolStack[poolStackDepth].callbacks[callbackIndex].context);
		}
		poolStack[poolStackDepth].numberOfCallbacks = 0;
	}
	if (mutex != NULL) {
		unlockMutex(mutex);
	}
}
