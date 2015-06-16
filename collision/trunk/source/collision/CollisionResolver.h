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

#ifndef __CollisionResolver_H__
#define __CollisionResolver_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionResolver CollisionResolver;
typedef struct CollisionRecord CollisionRecord;

#include "collision/CollisionObject.h"
#include "collision/IntersectionManager.h"
#include "gamemath/Vector3x.h"
#include "stemobject/StemObject.h"
#include <stdlib.h>

#define CollisionResolver_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	IntersectionManager * intersectionManager; \
	CollisionObject ** objects; \
	size_t objectCount; \
	size_t objectAllocatedCount;

struct CollisionRecord {
	// Target colliding object
	CollisionObject * object1;
	
	// Object with which target collided
	CollisionObject * object2;
	
	// Normal vector of the surface of object2 at the first point of intersection by object1
	Vector3x normal;
	
	// Temporal position within the timeslice being evaluated at which the intersection first occurs, from 0x00000 (beginning
	// of timeslice) to 0x10000 (end of timeslice).
	fixed16_16 time;
	// TODO: Doesn't this require an interpolate() method on owners of CollisionObjects?
};

stemobject_struct_definition(CollisionResolver)

CollisionResolver * CollisionResolver_create(IntersectionManager * intersectionManager);
bool CollisionResolver_init(CollisionResolver * self, IntersectionManager * intersectionManager);
void CollisionResolver_dispose(CollisionResolver * self);

// Returns a CollisionRecord with object1 and object2 swapped, and the normal reversed.
CollisionRecord CollisionRecord_inverted(CollisionRecord collision);

void CollisionResolver_addObject(CollisionResolver * self, compat_type(CollisionObject *) object);
void CollisionResolver_removeObject(CollisionResolver * self, compat_type(CollisionObject *) object);

// Performs an intersection test between object1 and object2, returning true on intersection.
bool CollisionResolver_intersectionTest(CollisionResolver * self, compat_type(CollisionObject *) object, compat_type(CollisionObject *) object2, CollisionRecord * outCollision);

// Performs an intersection test between object and all other collidable objects in list, returning true on intersection.
// Works regardless of whether object has been added to CollisionResolver's list.
// Does not call CollisionCallback; only returns the information of a potential collision if one exists.
bool CollisionResolver_querySingle(CollisionResolver * self, compat_type(CollisionObject *) object, CollisionRecord * outCollision);

// Performs intersection tests on all pairs of objects in list, and returns information about the earliest available
// collisions, if any. If multiple collisions occur at the same time, this function will write all of them to
// outCollisions (up to collisionCountMax), and return the number of CollisionRecords written.
// Does not call CollisionCallback; only returns the information of potential collisions if any exist.
size_t CollisionResolver_findEarliest(CollisionResolver * self, CollisionRecord * outCollisions, size_t collisionCountMax);

// Calls the appropriate CollisionCallback for object1 and object2 in collision. It is the responsibility of each
// CollisionCallback to adjust 
void CollisionResolver_resolveSingle(CollisionResolver * self, CollisionRecord collision);

// Collision tests all objects in list against all other objects in list, calling the CollisionCallback for each.
// Fully resolves one frame.
void CollisionResolver_resolveAll(CollisionResolver * self, size_t maxSimultaneousCollisions, size_t maxIterations);

#ifdef __cplusplus
}
#endif
#endif
