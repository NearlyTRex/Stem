#include "utilities/AutoFreePool.h"

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

void AutoFreePool_push() {
	if (poolStackDepth >= MAX_POOL_STACK_DEPTH - 1) {
		fprintf(stderr, "Warning: AutoFreePool maximum stack depth (%d) exceeded; couldn't push another pool as requested\n", MAX_POOL_STACK_DEPTH);
		return;
	}
	
	poolStackDepth++;
	poolStack[poolStackDepth].addresses = NULL;
	poolStack[poolStackDepth].callbacks = NULL;
}

void AutoFreePool_pop() {
	if (poolStackDepth < 0) {
		fprintf(stderr, "Warning: AutoFreePool stack underflow; you've popped more times than you've pushed\n");
		return;
	}
	
	AutoFreePool_empty();
	free(poolStack[poolStackDepth].addresses);
	free(poolStack[poolStackDepth].callbacks);
	poolStackDepth--;
}

void * AutoFreePool_add(void * address) {
	if (poolStackDepth == -1) {
		AutoFreePool_push();
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
	
	return address;
}

void AutoFreePool_addCallback(void (* callback)(void * context), void * context) {
	if (poolStackDepth == -1) {
		AutoFreePool_push();
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
}

void AutoFreePool_empty() {
	if (poolStackDepth == -1) {
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
}
