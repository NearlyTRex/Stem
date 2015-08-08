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
#include "gamemath/FixedPoint.h"
#include "gamemath/Vector3x.h"
#include "stemobject/StemObject.h"

// Callback to be invoked when a collision has been detected and needs to be resolved. This callback is responsible for
// updating the CollisionObject's trajectory or other parameters in some way to resolve the specified collision and ensure
// it would not be detected again immediately. For example, if two CollisionCircles collide, the CollisionCallback on both
// could set their position field to the value of their lastPosition field, which would stop the motion of both and prevent
// further collisions between them.
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
	bool private_ivar(markedForRemoval); \
	bool private_ivar(unresolvable); \
	\
	void (* interpolate)(self_type * self, fixed16_16 amount);

stemobject_struct_definition(CollisionObject)

CollisionObject * CollisionObject_create(void * owner, int shapeType, CollisionCallback collisionCallback);
bool CollisionObject_init(CollisionObject * self, void * owner, int shapeType, CollisionCallback collisionCallback);
void CollisionObject_dispose(CollisionObject * self);

#ifdef __cplusplus
}
#endif
#endif
