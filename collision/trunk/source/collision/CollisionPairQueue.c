/*
  Copyright (c) 2015 Alex Diener
  
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

#include "collision/CollisionPairQueue.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

CollisionPairQueue * CollisionPairQueue_create() {
	stemobject_create_implementation(CollisionPairQueue, init)
}

bool CollisionPairQueue_init(CollisionPairQueue * self) {
	call_super(init, self);
	self->dispose = CollisionPairQueue_dispose;
	self->private_ivar(queueAllocatedSize) = 64;
	self->frontQueue = malloc(sizeof(struct collisionObjectPair) * self->private_ivar(queueAllocatedSize));
	self->backQueue = malloc(sizeof(struct collisionObjectPair) * self->private_ivar(queueAllocatedSize));
	self->frontQueueCount = 0;
	self->backQueueCount = 0;
	self->queuePosition = 0;
	return true;
}

void CollisionPairQueue_dispose(CollisionPairQueue * self) {
	free(self->frontQueue);
	free(self->backQueue);
	call_super(dispose, self);
}

// Adapated from http://googleresearch.blogspot.co.nz/2006/06/extra-extra-read-all-about-it-nearly.html
static size_t bsearchQueue(struct collisionObjectPair * queue, size_t count, CollisionObject * object1, CollisionObject * object2, bool * outFound) {
	size_t low = 0;
	size_t high = count - 1;
	
	while (low <= high && high < count) {
		// Integer overflow is possible if count is high enough, but this can only happen with many orders of magnitude more CollisionObjects than the overall system can handle
		size_t mid = (low + high) / 2;
		
		if (queue[mid].object1 < object1) {
			low = mid + 1;
			
		} else if (queue[mid].object1 > object1) {
			high = mid - 1;
			
		} else if (queue[mid].object2 < object2) {
			low = mid + 1;
			
		} else if (queue[mid].object2 > object2) {
			high = mid - 1;
			
		} else {
			*outFound = true;
			return mid;
		}
	}
	*outFound = false;
	return low;
}

static void addPair(CollisionPairQueue * self, struct collisionObjectPair * queue, size_t * queueCount, CollisionObject * object1, CollisionObject * object2) {
	size_t insertionIndex, index;
	bool found;
	
	if (object2 < object1) {
		CollisionObject * swap = object1;
		object1 = object2;
		object2 = swap;
	}
	
	insertionIndex = bsearchQueue(queue, *queueCount, object1, object2, &found);
	if (found) {
		return;
	}
	if (object1->isStatic(object1) && object2->isStatic(object2)) {
		return;
	}
	if (Box6x_isEmpty(Box6x_union(object1->getCollisionBounds(object1), object2->getCollisionBounds(object2)))) {
		return;
	}
	
	if (*queueCount >= self->private_ivar(queueAllocatedSize)) {
		bool frontQueue = queue == self->frontQueue;
		self->private_ivar(queueAllocatedSize) *= 2;
		self->frontQueue = realloc(self->frontQueue, sizeof(struct collisionObjectPair) * self->private_ivar(queueAllocatedSize));
		self->backQueue = realloc(self->backQueue, sizeof(struct collisionObjectPair) * self->private_ivar(queueAllocatedSize));
		queue = frontQueue ? self->frontQueue : self->backQueue;
	}
	
	for (index = *queueCount; index > insertionIndex; index--) {
		queue[index] = queue[index - 1];
	}
	queue[insertionIndex].object1 = object1;
	queue[insertionIndex].object2 = object2;
	(*queueCount)++;
}

void CollisionPairQueue_addInitialPairs(CollisionPairQueue * self, CollisionObject ** objects, size_t objectCount) {
	size_t objectIndex, objectIndex2;
	
	self->frontQueueCount = self->backQueueCount = self->queuePosition = 0;
	for (objectIndex = 0; objectIndex < objectCount; objectIndex++) {
		for (objectIndex2 = objectIndex + 1; objectIndex2 < objectCount; objectIndex2++) {
			addPair(self, self->frontQueue, &self->frontQueueCount, objects[objectIndex], objects[objectIndex2]);
		}
	}
}

void CollisionPairQueue_addNextPair(CollisionPairQueue * self, CollisionObject * object1, CollisionObject * object2) {
	addPair(self, self->backQueue, &self->backQueueCount, object1, object2);
}

void CollisionPairQueue_addNextPairsForObject(CollisionPairQueue * self, CollisionObject * object, CollisionObject ** objects, size_t objectCount) {
	size_t objectIndex;
	
	for (objectIndex = 0; objectIndex < objectCount; objectIndex++) {
		if (objects[objectIndex] == object) {
			continue;
		}
		addPair(self, self->backQueue, &self->backQueueCount, object, objects[objectIndex]);
	}
}

bool CollisionPairQueue_nextPair(CollisionPairQueue * self, CollisionObject ** outObject1, CollisionObject ** outObject2) {
	if (self->queuePosition >= self->frontQueueCount) {
		return false;
	}
	*outObject1 = self->frontQueue[self->queuePosition].object1;
	*outObject2 = self->frontQueue[self->queuePosition].object2;
	self->queuePosition++;
	return true;
}

bool CollisionPairQueue_nextIteration(CollisionPairQueue * self) {
	struct collisionObjectPair * swap = self->frontQueue;
	self->frontQueue = self->backQueue;
	self->backQueue = swap;
	self->frontQueueCount = self->backQueueCount;
	self->backQueueCount = 0;
	self->queuePosition = 0;
	return self->frontQueueCount > 0;
}
