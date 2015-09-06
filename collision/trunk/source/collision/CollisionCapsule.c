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

#include "collision/CollisionCapsule.h"
#include "collision/CollisionShared.h"
#include <stdlib.h>

#define SUPERCLASS CollisionObject

CollisionCapsule * CollisionCapsule_create(void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius, fixed16_16 cylinderHeight) {
	stemobject_create_implementation(CollisionCapsule, init, owner, collisionCallback, position, radius, cylinderHeight)
}

bool CollisionCapsule_init(CollisionCapsule * self, void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius, fixed16_16 cylinderHeight) {
	call_super(init, self, owner, COLLISION_SHAPE_CAPSULE, collisionCallback);
	self->dispose = CollisionCapsule_dispose;
	self->interpolate = CollisionCapsule_interpolate;
	self->isStatic = CollisionCapsule_isStatic;
	self->getCollisionBounds = CollisionCapsule_getCollisionBounds;
	self->position = position;
	self->lastPosition = position;
	self->radius = radius;
	self->cylinderHeight = cylinderHeight;
	return true;
}

void CollisionCapsule_dispose(CollisionCapsule * self) {
	call_super(dispose, self);
}

void CollisionCapsule_updatePosition(CollisionCapsule * self, Vector3x newPosition) {
	self->lastPosition = self->position;
	self->position = newPosition;
}

void CollisionCapsule_interpolate(CollisionCapsule * self, fixed16_16 amount) {
	self->lastPosition = Vector3x_interpolate(self->lastPosition, self->position, amount);
}

bool CollisionCapsule_isStatic(CollisionCapsule * self) {
	return self->position.x == self->lastPosition.x && self->position.y == self->lastPosition.y && self->position.z == self->lastPosition.z;
}

Box6x CollisionCapsule_getCollisionBounds(CollisionCapsule * self) {
	return Box6x_union(BOX6x(self->lastPosition.x - self->radius, self->lastPosition.x + self->radius, self->lastPosition.y, self->lastPosition.y + self->radius * 2 + self->cylinderHeight, self->lastPosition.z - self->radius, self->lastPosition.z + self->radius), BOX6x(self->position.x - self->radius, self->position.x + self->radius, self->position.y, self->position.y + self->radius * 2 + self->cylinderHeight, self->position.z - self->radius, self->position.z + self->radius));
}
