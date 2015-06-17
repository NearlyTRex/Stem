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
//#define INTERSECTION_EPSILON 0x00008

CollisionResolver * CollisionResolver_create(IntersectionManager * intersectionManager, bool takeOwnership) {
	stemobject_create_implementation(CollisionResolver, init, intersectionManager, takeOwnership)
}

bool CollisionResolver_init(CollisionResolver * self, IntersectionManager * intersectionManager, bool takeOwnership) {
	call_super(init, self);
	self->dispose = CollisionResolver_dispose;
	self->intersectionManager = intersectionManager;
	self->private_ivar(intersectionManagerOwned) = takeOwnership;
	self->objectCount = 0;
	self->objectAllocatedCount = 32;
	self->objects = malloc(sizeof(CollisionObject *) * self->objectAllocatedCount);
	return true;
}

void CollisionResolver_dispose(CollisionResolver * self) {
	free(self->objects);
	if (self->private_ivar(intersectionManagerOwned)) {
		IntersectionManager_dispose(self->intersectionManager);
	}
	call_super(dispose, self);
}

void CollisionResolver_addObject(CollisionResolver * self, compat_type(CollisionObject *) object) {
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

void CollisionResolver_removeObject(CollisionResolver * self, compat_type(CollisionObject *) object) {
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

bool CollisionResolver_intersectionTest(CollisionResolver * self, compat_type(CollisionObject *) object, compat_type(CollisionObject *) object2, CollisionRecord * outCollision) {
	return false;
}

bool CollisionResolver_querySingle(CollisionResolver * self, compat_type(CollisionObject *) object, CollisionRecord * outCollision) {
	size_t objectIndex;
	CollisionObject * object1 = object;
	CollisionObject * object2, * bestObject2 = NULL;
	Vector3x normal, bestNormal = VECTOR3x_ZERO;
	fixed16_16 frameTime, bestFrameTime = FIXED_16_16_MAX;
	IntersectionHandler handler;
	bool intersectionFound;
	
	for (objectIndex = 0; objectIndex < self->objectCount; objectIndex++) {
		object2 = self->objects[objectIndex];
		if (object2 == object1) {
			continue;
		}
		
		intersectionFound = false;
		handler = IntersectionManager_getHandler(self->intersectionManager, object1->shapeType, object2->shapeType);
		if (handler == NULL) {
			handler = IntersectionManager_getHandler(self->intersectionManager, object2->shapeType, object1->shapeType);
			intersectionFound = handler(object2, object1, &frameTime, &normal);
			Vector3x_invert(&normal);
		} else {
			intersectionFound = handler(object1, object2, &frameTime, &normal);
		}
		
		if (intersectionFound && frameTime < bestFrameTime) {
			bestObject2 = object2;
			bestNormal = normal;
			bestFrameTime = frameTime;
		}
	}
	
	if (bestObject2 != NULL) {
		outCollision->object1 = object1;
		outCollision->object2 = bestObject2;
		outCollision->time = bestFrameTime;
		outCollision->normal = bestNormal;
		return true;
	}
	
	return false;
}

size_t CollisionResolver_findEarliest(CollisionResolver * self, CollisionRecord * outCollisions, size_t collisionCountMax) {
	return 0;
}

void CollisionResolver_resolveSingle(CollisionResolver * self, CollisionRecord collision) {
}

void CollisionResolver_resolveAll(CollisionResolver * self, size_t maxSimultaneousCollisions, size_t maxIterations) {
	/*fixed16_16 timesliceRemaining = 0x10000;
	unsigned int iterationCount = 0;
	struct collision collision;
	bool anyCollisions;
	
	while (timesliceRemaining > 0) {
		anyCollisions = findEarliestCollision(self, &collision);
		if (!anyCollisions) {
			break;
		}
		
		if (collision.time > 0) {
			CollisionObject_interpolate(collision.object1, collision.time);
			CollisionObject_interpolate(collision.object2, collision.time);
		}
		
		if (collision.object1->collisionCallback != NULL) {
			collision.object1->collisionCallback(collision.object1, collision.object2, collision.side);
		}
		if (collision.object2->collisionCallback != NULL) {
			collision.object2->collisionCallback(collision.object2, collision.object1, CollisionSide_opposite(collision.side));
		}
		
		iterationCount++;
		if (iterationCount >= MAX_COLLISION_ITERATIONS) {
			fprintf(stderr, "Warning: Incomplete collision resolution after %u iterations\n", MAX_COLLISION_ITERATIONS);
			break;
		}
		timesliceRemaining = xmul(0x10000 - collision.time, timesliceRemaining);
	}*/
}
