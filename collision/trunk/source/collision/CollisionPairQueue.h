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

#ifndef __CollisionPairQueue_H__
#define __CollisionPairQueue_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionPairQueue CollisionPairQueue;

#include "collision/CollisionObject.h"
#include "stemobject/StemObject.h"
#include <stdlib.h>

#define CollisionPairQueue_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	CollisionObject ** frontQueue; \
	size_t frontQueueCount; \
	CollisionObject ** backQueue; \
	size_t backQueueCount; \
	size_t queuePosition; \
	size_t private_ivar(queueAllocatedSize);

stemobject_struct_definition(CollisionPairQueue)

CollisionPairQueue * CollisionPairQueue_create();
bool CollisionPairQueue_init(CollisionPairQueue * self);
void CollisionPairQueue_dispose(CollisionPairQueue * self);

// Adds all permutations of objects pairs in the list to the initial queue. Pairs which can safely be ignored
// (static vs. static) will not be added.
void CollisionPairQueue_addInitialPairs(CollisionPairQueue * self, CollisionObject ** objects, size_t objectCount);

// Adds a single pair to the next queue. Does not affect current queue.
void CollisionPairQueue_addNextPair(CollisionPairQueue * self, CollisionObject * object1, CollisionObject * object2);

// Adds pairs for object vs. all other relevant objects in the list to the next queue. Does not affect current queue.
void CollisionPairQueue_addNextPairsForObject(CollisionPairQueue * self, CollisionObject * object, CollisionObject ** objects, size_t objectCount);

// If there are pairs left to process in the current queue, returns true and sets the out parameters to the next
// pair to be processed, removing it from the queue.
bool CollisionPairQueue_nextPair(CollisionPairQueue * self, CollisionObject ** outObject1, CollisionObject ** outObject2);

// Empties the current queue and swaps to the next one, returning true if there are any pairs remaining to be processed.
bool CollisionPairQueue_nextIteration(CollisionPairQueue * self);

#ifdef __cplusplus
}
#endif
#endif
