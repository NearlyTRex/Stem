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

#ifndef __CollisionCapsule_H__
#define __CollisionCapsule_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionCapsule CollisionCapsule;

#include "collision/CollisionObject.h"
#include "gamemath/Vector3x.h"

#define CollisionCapsule_structContents(self_type) \
	CollisionObject_structContents(self_type) \
	\
	Vector3x position; \
	Vector3x lastPosition; \
	fixed16_16 radius; \
	fixed16_16 cylinderHeight;

stemobject_struct_definition(CollisionCapsule)

// position is the bottom center of the capsule.
// radius affects both the endpoints and the middle section of the capsule. Endpoints are always perfect half spheres.
// cylinderHeight specifies the length of the capsule with straight vertical walls between the sphere halves.
// A capsule with a cylinderHeight of 0 is a sphere. The overall height of a capsule is radius * 2 + cylinderHeight.
CollisionCapsule * CollisionCapsule_create(void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius, fixed16_16 cylinderHeight);
bool CollisionCapsule_init(CollisionCapsule * self, void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius, fixed16_16 cylinderHeight);
void CollisionCapsule_dispose(CollisionCapsule * self);

void CollisionCapsule_updatePosition(CollisionCapsule * self, Vector3x newPosition);
void CollisionCapsule_interpolate(CollisionCapsule * self, fixed16_16 amount);

bool CollisionCapsule_isStatic(CollisionCapsule * self);
Box6x CollisionCapsule_getCollisionBounds(CollisionCapsule * self);

#ifdef __cplusplus
}
#endif
#endif
