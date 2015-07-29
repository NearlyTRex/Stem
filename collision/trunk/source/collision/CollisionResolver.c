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

CollisionResolver * CollisionResolver_create(IntersectionManager * intersectionManager, bool takeOwnership, size_t maxSimultaneousCollisions, size_t maxIterations) {
	stemobject_create_implementation(CollisionResolver, init, intersectionManager, takeOwnership, maxSimultaneousCollisions, maxIterations)
}

bool CollisionResolver_init(CollisionResolver * self, IntersectionManager * intersectionManager, bool takeOwnership, size_t maxSimultaneousCollisions, size_t maxIterations) {
	call_super(init, self);
	self->dispose = CollisionResolver_dispose;
	self->intersectionManager = intersectionManager;
	self->private_ivar(intersectionManagerOwned) = takeOwnership;
	self->private_ivar(maxSimultaneousCollisions) = maxSimultaneousCollisions;
	self->private_ivar(simultaneousCollisionBuffer) = malloc(sizeof(*self->private_ivar(simultaneousCollisionBuffer)) * self->private_ivar(maxSimultaneousCollisions));
	self->private_ivar(maxIterations) = maxIterations;
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
			self->objectCount--;
			for (; objectIndex < self->objectCount; objectIndex++) {
				self->objects[objectIndex] = self->objects[objectIndex + 1];
			}
		}
	}
}

static bool CollisionResolver_queryPairInternal(CollisionResolver * self, CollisionObject * object1, CollisionObject * object2, CollisionRecord * outCollision) {
	Vector3x normal;
	fixed16_16 frameTime;
	IntersectionHandler handler;
	bool intersectionFound;
	
	intersectionFound = false;
	handler = IntersectionManager_getHandler(self->intersectionManager, object1->shapeType, object2->shapeType);
	if (handler == NULL) {
		handler = IntersectionManager_getHandler(self->intersectionManager, object2->shapeType, object1->shapeType);
		if (handler == NULL) {
#ifdef DEBUG
			fprintf(stderr, "Warning: Collision objects of shapeType %d and %d have been added to CollisionResolver %p with no matching intersection handler for those types\n", object1->shapeType, object2->shapeType, self);
#endif
		} else {
			intersectionFound = handler(object2, object1, &frameTime, &normal, NULL, NULL, NULL);
			Vector3x_invert(&normal);
		}
	} else {
		intersectionFound = handler(object1, object2, &frameTime, &normal, NULL, NULL, NULL);
	}
	
	if (intersectionFound) {
		outCollision->object1 = object1;
		outCollision->object2 = object2;
		outCollision->normal = normal;
		outCollision->time = frameTime;
		return true;
	}
	return false;
}

bool CollisionResolver_querySingle(CollisionResolver * self, CollisionObject * object, CollisionRecord * outCollision) {
	size_t objectIndex;
	CollisionRecord collision, bestCollision = {NULL, NULL, FIXED_16_16_MAX, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	
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
	size_t collisionCount = 0;
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
	return collisionCount;
}

void CollisionResolver_resolveAll(CollisionResolver * self) {
	size_t collisionCount, collisionIndex;
	fixed16_16 timesliceRemaining = 0x10000;
	fixed16_16 collisionTime;
	size_t objectIndex;
	unsigned int iterationCount = 0;
	
	// TODO: Sort findEarliest's list to handle seam collisions? By what criteria?
	while ((collisionCount = CollisionResolver_findEarliest(self, self->private_ivar(simultaneousCollisionBuffer), self->private_ivar(maxSimultaneousCollisions))) > 0) {
		collisionTime = self->private_ivar(simultaneousCollisionBuffer)[0].time;
		
		for (objectIndex = 0; objectIndex < self->objectCount; objectIndex++) {
			if (self->objects[objectIndex]->interpolate != NULL) {
				self->objects[objectIndex]->interpolate(self->objects[objectIndex], collisionTime);
			}
		}
		
		for (collisionIndex = 0; collisionIndex < collisionCount; collisionIndex++) {
			CollisionRecord_resolve(self->private_ivar(simultaneousCollisionBuffer)[collisionIndex], timesliceRemaining);
		}
		
		timesliceRemaining = xmul(0x10000 - collisionTime, timesliceRemaining);
		
		iterationCount++;
		if (iterationCount >= self->private_ivar(maxIterations)) {
			break;
		}
	}
}
