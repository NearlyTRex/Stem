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

#ifndef __CollisionObject_H__
#define __CollisionObject_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionObject CollisionObject;

#include "collision/CollisionResolver.h"
#include "gamemath/Box6x.h"
#include "gamemath/FixedPoint.h"
#include "gamemath/Vector3x.h"
#include "stemobject/StemObject.h"

// Callback to be invoked when a collision has been detected and needs to be resolved. This callback is responsible for
// updating the CollisionObject's trajectory or other parameters in some way to resolve the specified collision and ensure
// it would not be detected again immediately. For example, if two CollisionCircles collide, the CollisionCallback on both
// could set their position field to the value of their lastPosition field, which would stop the motion of both and prevent
// further collisions between them. (Note that during collision resolution, object trajectories must not be modified at any
// time other than a collision callback for that specific object.)
// 
// timesliceSize specifies the scale of the timeslice that was being evaluated when this collision was detected. This may
// include some amount of time before and/or after the collision occurred. CollisionRecord's time, object1Vector, and
// object2Vector fields are relative to timeslice size, so you may want to divide by this value if you need to rescale any
// of those values to whole-frame time.
// 
// subframeTime specifies the temporal position within the whole frame being evaluated at which the collision occurs. This
// is in contrast to CollisionRecord's time field, which is relative to the size of the timeslice, not the entire frame.
// At the time a CollisionCallback is called, its CollisionObject will have been interpolated to this position, so the
// remaining trajectory is relative to the remaining time in the frame.
typedef void (* CollisionCallback)(CollisionRecord collision, fixed16_16 timesliceSize, fixed16_16 subframeTime);

#define CollisionObject_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	void * owner; \
	int shapeType; \
	CollisionCallback collisionCallback; \
	\
	/* If normal resolution fails to prevent object penetration, this function (if not NULL, which is the default) is called, */ \
	/* and this object will no longer be able to collide with collidingObject for the rest of the timeslice being processed. */ \
	/* This a debugging facility to indicate a faulty collisionCallback implementation, and should not be used for normal logic. */ \
	void (* resolutionFailureCallback)(CollisionObject * self, CollisionObject * collidingObject); \
	\
	bool private_ivar(markedForRemoval); \
	bool private_ivar(unresolvable); \
	\
	/* See CollisionObject_setMasks() below for explanation */ \
	uint32_t ownMask; \
	uint32_t collidableMask; \
	\
	/* Advance lastPosition toward position by the specified amount, or the equivalent depending on how this subclass's position is specified */ \
	void (* interpolate)(self_type * self, fixed16_16 amount); \
	\
	/* If true, this object will not be tested against other objects that return true from isStatic */ \
	bool (* isStatic)(self_type * self); \
	\
	/* A bounding box fully enclosing this object at both the previous and current timepoints, and everything in between */ \
	Box6x (* getCollisionBounds)(self_type * self);

stemobject_struct_definition(CollisionObject)

CollisionObject * CollisionObject_create(void * owner, int shapeType, CollisionCallback collisionCallback);
bool CollisionObject_init(CollisionObject * self, void * owner, int shapeType, CollisionCallback collisionCallback);
void CollisionObject_dispose(CollisionObject * self);

bool CollisionObject_isStatic(CollisionObject * self);
Box6x CollisionObject_getCollisionBounds(CollisionObject * self);

// These two masks are used to determine which objects are able to intercollide with which other objects.
// In contrast to shapeType, which is used to look up the appropriate intersection handler for a pair of objects,
// these values are user-defined and allowed/expected to be changed at runtime, including during collision
// resolution.
// 
// Semantically, ownMask indicates the "group type" to which this object belongs (or group types, if more than
// one bit is set), while collidableMask indicates the group types of objects that are considered solid from this
// object's viewpoint, and should be tested against it.
// 
// Mechanically, when an object pair is considered for an intersection test, object1's collidableMask is bitwise
// ANDed with object2's ownMask, and vice versa. If either operation results in 0, the pair is discarded for
// testing and the objects will behave as nonsolid with respect to each other.
// 
// self is returned to allow function chaining.
CollisionObject * CollisionObject_setMasks(CollisionObject * self, uint32_t ownMask, uint32_t collidableMask);

#ifdef __cplusplus
}
#endif
#endif
