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

#include "collision/CollisionResolver.h"
#include <stdio.h>
#include <stdlib.h>

#define SUPERCLASS StemObject

#define MAX_ITERATIONS_TEMP 512
#define MAX_SIMULTANEOUS_COLLISIONS 128

CollisionResolver * CollisionResolver_create(IntersectionManager * intersectionManager, bool takeOwnership) {
	stemobject_create_implementation(CollisionResolver, init, intersectionManager, takeOwnership)
}

bool CollisionResolver_init(CollisionResolver * self, IntersectionManager * intersectionManager, bool takeOwnership) {
	call_super(init, self);
	self->dispose = CollisionResolver_dispose;
	self->intersectionManager = intersectionManager;
	self->private_ivar(intersectionManagerOwned) = takeOwnership;
	self->private_ivar(inResolveAll) = false;
	self->private_ivar(simultaneousCollisionBuffer) = malloc(sizeof(*self->private_ivar(simultaneousCollisionBuffer)) * MAX_SIMULTANEOUS_COLLISIONS);
	self->private_ivar(cycleDetectionBufferSize) = 128;
	self->private_ivar(cycleDetectionBufferCount) = 0;
	self->private_ivar(cycleDetectionBuffer) = malloc(sizeof(*self->private_ivar(cycleDetectionBuffer)) * self->private_ivar(cycleDetectionBufferSize) * 2);
	self->objectCount = 0;
	self->objectAllocatedCount = 32;
	self->objects = malloc(sizeof(CollisionObject *) * self->objectAllocatedCount);
	return true;
}

void CollisionResolver_dispose(CollisionResolver * self) {
	free(self->objects);
	free(self->private_ivar(simultaneousCollisionBuffer));
	if (self->private_ivar(intersectionManagerOwned)) {
		IntersectionManager_dispose(self->intersectionManager);
	}
	call_super(dispose, self);
}

void CollisionResolver_addObject(CollisionResolver * self, CollisionObject * object) {
	size_t objectIndex;
	
	for (objectIndex = 0; objectIndex < self->objectCount; objectIndex++) {
		if (self->objects[objectIndex] == object) {
			return;
		}
	}
	
	if (self->objectCount >= self->objectAllocatedCount) {
		self->objectAllocatedCount *= 2;
		self->objects = realloc(self->objects, sizeof(CollisionObject *) * self->objectAllocatedCount);
	}
	self->objects[self->objectCount++] = object;
}

void CollisionResolver_removeObject(CollisionResolver * self, CollisionObject * object) {
	size_t objectIndex;
	
	for (objectIndex = 0; objectIndex < self->objectCount; objectIndex++) {
		if (self->objects[objectIndex] == object) {
			if (self->private_ivar(inResolveAll)) {
				self->objects[objectIndex]->private_ivar(markedForRemoval) = true;
				
			} else {
				self->objectCount--;
				for (; objectIndex < self->objectCount; objectIndex++) {
					self->objects[objectIndex] = self->objects[objectIndex + 1];
				}
			}
		}
	}
}

static bool CollisionResolver_queryPairInternal(CollisionResolver * self, CollisionObject * object1, CollisionObject * object2, CollisionRecord * outCollision) {
	Vector3x normal;
	fixed16_16 frameTime, contactArea;
	IntersectionHandler handler;
	bool intersectionFound;
	
	if (object1->private_ivar(markedForRemoval) || object2->private_ivar(markedForRemoval) ||
	    (object1->private_ivar(unresolvable) && object2->private_ivar(unresolvable))) {
		return false;
	}
	
	intersectionFound = false;
	handler = IntersectionManager_getHandler(self->intersectionManager, object1->shapeType, object2->shapeType);
	if (handler == NULL) {
		handler = IntersectionManager_getHandler(self->intersectionManager, object2->shapeType, object1->shapeType);
		if (handler == NULL) {
#ifdef DEBUG
			fprintf(stderr, "Warning: Collision objects of shapeType %d and %d have been added to CollisionResolver %p with no matching intersection handler for those types\n", object1->shapeType, object2->shapeType, self);
#endif
		} else {
			intersectionFound = handler(object2, object1, &frameTime, &normal, NULL, NULL, &contactArea);
			Vector3x_invert(&normal);
		}
	} else {
		intersectionFound = handler(object1, object2, &frameTime, &normal, NULL, NULL, &contactArea);
	}
	
	if (intersectionFound) {
		outCollision->object1 = object1;
		outCollision->object2 = object2;
		outCollision->normal = normal;
		outCollision->time = frameTime;
		outCollision->contactArea = contactArea;
		return true;
	}
	return false;
}

bool CollisionResolver_querySingle(CollisionResolver * self, CollisionObject * object, CollisionRecord * outCollision) {
	size_t objectIndex;
	CollisionRecord collision, bestCollision = {NULL, NULL, FIXED_16_16_MAX, 0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	
	for (objectIndex = 0; objectIndex < self->objectCount; objectIndex++) {
		if (self->objects[objectIndex] == object) {
			continue;
		}
		
		if (CollisionResolver_queryPairInternal(self, object, self->objects[objectIndex], &collision) && collision.time < bestCollision.time) {
			bestCollision = collision;
		}
	}
	
	if (bestCollision.object2 != NULL) {
		*outCollision = bestCollision;
		return true;
	}
	
	return false;
}

size_t CollisionResolver_findEarliest(CollisionResolver * self, CollisionRecord * outCollisions, size_t collisionCountMax) {
	unsigned int objectIndex, objectIndex2;
	bool queryResult;
	CollisionRecord collision;
	size_t collisionCount = 0, collisionIndex, collisionIndex2;
	fixed16_16 bestTime = FIXED_16_16_MAX;
	
	if (collisionCountMax == 0) {
		return 0;
	}
	for (objectIndex = 0; objectIndex < self->objectCount; objectIndex++) {
		for (objectIndex2 = objectIndex + 1; objectIndex2 < self->objectCount; objectIndex2++) {
			queryResult = CollisionResolver_queryPairInternal(self, self->objects[objectIndex], self->objects[objectIndex2], &collision);
			if (queryResult && collision.time <= bestTime) {
				if (collision.time < bestTime) {
					bestTime = collision.time;
					outCollisions[0] = collision;
					collisionCount = 1;
					
				} else {
					if (collisionCount < collisionCountMax) {
						outCollisions[collisionCount++] = collision;
					}
				}
			}
		}
	}
	
	for (collisionIndex = 1; collisionIndex < collisionCount; collisionIndex++) {
		collision = outCollisions[collisionIndex];
		for (collisionIndex2 = collisionIndex; collisionIndex2 > 0 && outCollisions[collisionIndex2 - 1].contactArea < collision.contactArea; collisionIndex2--) {
			outCollisions[collisionIndex2] = outCollisions[collisionIndex2 - 1];
			outCollisions[collisionIndex2 - 1] = collision;
		}
	}
	
	return collisionCount;
}

static void resetCycleDetection(CollisionResolver * self) {
	self->private_ivar(cycleDetectionBufferCount) = 0;
}

static void resetObjectsMarkedUnresolvable(CollisionResolver * self) {
	size_t objectIndex;
	
	for (objectIndex = 0; objectIndex < self->objectCount; objectIndex++) {
		self->objects[objectIndex]->private_ivar(unresolvable) = false;
	}
}

static void pushObjectPairToCycleBuffer(CollisionResolver * self, CollisionObject * object1, CollisionObject * object2) {
	if (self->private_ivar(cycleDetectionBufferSize) <= self->private_ivar(cycleDetectionBufferCount)) {
		self->private_ivar(cycleDetectionBufferSize) *= 2;
		self->private_ivar(cycleDetectionBuffer) = realloc(self->private_ivar(cycleDetectionBuffer), sizeof(*self->private_ivar(cycleDetectionBuffer)) * self->private_ivar(cycleDetectionBufferSize) * 2);
	}
	self->private_ivar(cycleDetectionBuffer)[self->private_ivar(cycleDetectionBufferCount) * 2] = object1;
	self->private_ivar(cycleDetectionBuffer)[self->private_ivar(cycleDetectionBufferCount) * 2 + 1] = object2;
	self->private_ivar(cycleDetectionBufferCount)++;
}

static void checkForCycles(CollisionResolver * self) {
	unsigned int objectPairIndex, objectPairIndex2;
	
	for (objectPairIndex = 0; objectPairIndex < self->private_ivar(cycleDetectionBufferCount); objectPairIndex++) {
		bool matchFound = false;
		
		for (objectPairIndex2 = objectPairIndex + 1; objectPairIndex2 < self->private_ivar(cycleDetectionBufferCount); objectPairIndex2++) {
			if (self->private_ivar(cycleDetectionBuffer)[objectPairIndex * 2 + 0] == self->private_ivar(cycleDetectionBuffer)[objectPairIndex2 * 2 + 0] &&
			    self->private_ivar(cycleDetectionBuffer)[objectPairIndex * 2 + 1] == self->private_ivar(cycleDetectionBuffer)[objectPairIndex2 * 2 + 1]) {
				matchFound = true;
				break;
			}
		}
		
		if (matchFound) {
			self->private_ivar(cycleDetectionBuffer)[objectPairIndex * 2 + 0]->private_ivar(unresolvable) = true;
			self->private_ivar(cycleDetectionBuffer)[objectPairIndex * 2 + 1]->private_ivar(unresolvable) = true;
			continue;
		}
	}
}

void CollisionResolver_resolveAll(CollisionResolver * self) {
	size_t collisionCount, collisionIndex, collisionIndex2;
	fixed16_16 timesliceRemaining = 0x10000;
	size_t objectIndex;
	unsigned int iterationCount = 0;
	
	self->private_ivar(inResolveAll) = true;
	while ((collisionCount = CollisionResolver_findEarliest(self, self->private_ivar(simultaneousCollisionBuffer), MAX_SIMULTANEOUS_COLLISIONS)) > 0) {
		fixed16_16 collisionTime = self->private_ivar(simultaneousCollisionBuffer)[0].time;
		fixed16_16 nextTimeslice = xmul(0x10000 - collisionTime, timesliceRemaining);
		
		if (collisionTime > 0) {
			resetCycleDetection(self);
			resetObjectsMarkedUnresolvable(self);
		} else {
			checkForCycles(self);
		}
		
		for (objectIndex = 0; objectIndex < self->objectCount; objectIndex++) {
			if (self->objects[objectIndex]->interpolate != NULL) {
				self->objects[objectIndex]->interpolate(self->objects[objectIndex], collisionTime);
			}
		}
		
		for (collisionIndex = 0; collisionIndex < collisionCount; collisionIndex++) {
			bool objectAlreadyCollided = false;
			
			// If an object has already been resolved during a higher-priority simultaneous collision at this timepoint, boot it to the next iteration
			for (collisionIndex2 = 0; collisionIndex2 < collisionIndex; collisionIndex2++) {
				if (self->private_ivar(simultaneousCollisionBuffer)[collisionIndex2].object1 == self->private_ivar(simultaneousCollisionBuffer)[collisionIndex].object1 ||
				    self->private_ivar(simultaneousCollisionBuffer)[collisionIndex2].object1 == self->private_ivar(simultaneousCollisionBuffer)[collisionIndex].object2 ||
				    self->private_ivar(simultaneousCollisionBuffer)[collisionIndex2].object2 == self->private_ivar(simultaneousCollisionBuffer)[collisionIndex].object1 ||
				    self->private_ivar(simultaneousCollisionBuffer)[collisionIndex2].object2 == self->private_ivar(simultaneousCollisionBuffer)[collisionIndex].object2) {
					objectAlreadyCollided = true;
					break;
				}
			}
			
			if (!objectAlreadyCollided) {
				if (collisionTime == 0) {
					pushObjectPairToCycleBuffer(self, self->private_ivar(simultaneousCollisionBuffer)[collisionIndex].object1, self->private_ivar(simultaneousCollisionBuffer)[collisionIndex].object2);
				}
				CollisionRecord_resolve(self->private_ivar(simultaneousCollisionBuffer)[collisionIndex], timesliceRemaining, 0x10000 - nextTimeslice);
			}
		}
		
		timesliceRemaining = nextTimeslice;
		
		iterationCount++;
		if (iterationCount >= MAX_ITERATIONS_TEMP) {
			fprintf(stderr, "Warning: Max iterations (%u) exceeded\n", MAX_ITERATIONS_TEMP);
			break;
		}
	}
	resetCycleDetection(self);
	resetObjectsMarkedUnresolvable(self);
	self->private_ivar(inResolveAll) = false;
	
	size_t removedObjectCount = 0;
	for (objectIndex = 0; objectIndex < self->objectCount; objectIndex++) {
		self->objects[objectIndex] = self->objects[objectIndex + removedObjectCount];
		while (objectIndex < self->objectCount && self->objects[objectIndex]->private_ivar(markedForRemoval)) {
			self->objectCount--;
			removedObjectCount++;
			if (objectIndex < self->objectCount) {
				self->objects[objectIndex] = self->objects[objectIndex + removedObjectCount];
			}
		}
	}
}
