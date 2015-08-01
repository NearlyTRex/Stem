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
#include "collision/CollisionRecord.h"
#include "collision/IntersectionManager.h"
#include "gamemath/Vector3x.h"
#include "stemobject/StemObject.h"
#include <stdlib.h>

#define CollisionResolver_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	IntersectionManager * intersectionManager; \
	bool private_ivar(intersectionManagerOwned); \
	bool private_ivar(inResolveAll); \
	size_t private_ivar(maxSimultaneousCollisions); \
	size_t private_ivar(maxIterations); \
	CollisionRecord * private_ivar(simultaneousCollisionBuffer); \
	CollisionObject ** objects; \
	size_t objectCount; \
	size_t objectAllocatedCount;

stemobject_struct_definition(CollisionResolver)

#define MAX_SIMULTANEOUS_COLLISIONS_DEFAULT 128
#define MAX_ITERATIONS_DEFAULT 128

// Creates and initializes a CollisionResolver.
// intersectionManager will be used for all intersection tests in querySingle, findEarliest, and resolveAll.
// If takeOwnership is true, intersectionManager will be freed when this CollisionResolver is freed.
// If takeOwnership is false, the caller retains ownership of intersectionManager and is responsible for freeing it.
// maxSimultaneousCollisions specifies the number of collisions that can be processed in a single resolveAll iteration.
// maxIterations specifies the number of iterations resolveAll will attempt before giving up if resolution is incomplete.
CollisionResolver * CollisionResolver_create(IntersectionManager * intersectionManager, bool takeOwnership, size_t maxSimultaneousCollisions, size_t maxIterations);
bool CollisionResolver_init(CollisionResolver * self, IntersectionManager * intersectionManager, bool takeOwnership, size_t maxSimultaneousCollisions, size_t maxIterations);

// Disposes the CollisionResolver and all objects owned by it. Objects added with addObject are not owned by CollisionResolver.
void CollisionResolver_dispose(CollisionResolver * self);

// Adds/removes objects to/from the list of collidable objects tracked by CollisionResolver.
// Objects in this list are used by querySingle, findEarliest, and resolveAll.
// If addObject is called in a collision callback during resolveAll, it will be included immediately and tested against
// in the next iteration.
// If removeObject is called in a collision callback during resolveAll, the removed object will immediately be ignored
// for the rest of resolveAll, but will not actually be removed from the list until resolveAll has completed.
void CollisionResolver_addObject(CollisionResolver * self, CollisionObject * object);
void CollisionResolver_removeObject(CollisionResolver * self, CollisionObject * object);

// Performs an intersection test between object and all other collidable objects in list, returning true on intersection.
// Works regardless of whether object has been added to CollisionResolver's list.
// Does not call CollisionCallback; only returns the information of a potential collision if one exists.
bool CollisionResolver_querySingle(CollisionResolver * self, CollisionObject * object, CollisionRecord * outCollision);

// Performs intersection tests on all pairs of objects in list, and returns information about the earliest available
// collisions, if any. If multiple collisions occur at the same time, this function will write all of them to
// outCollisions (up to collisionCountMax), and return the number of CollisionRecords written.
// Does not call CollisionCallback; only returns the information of potential collisions if any exist.
size_t CollisionResolver_findEarliest(CollisionResolver * self, CollisionRecord * outCollisions, size_t collisionCountMax);

// Collision tests all objects in list against all other objects in list, calling the CollisionCallback for each.
// Fully resolves one frame. Not reentrant (but should never need to be).
void CollisionResolver_resolveAll(CollisionResolver * self);

#ifdef __cplusplus
}
#endif
#endif
