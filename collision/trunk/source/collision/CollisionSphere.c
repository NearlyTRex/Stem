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

#include "collision/CollisionShared.h"
#include "collision/CollisionSphere.h"
#include <stdlib.h>

#define SUPERCLASS CollisionObject

CollisionSphere * CollisionSphere_create(void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius) {
	stemobject_create_implementation(CollisionSphere, init, owner, collisionCallback, position, radius)
}

bool CollisionSphere_init(CollisionSphere * self, void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius) {
	call_super(init, self, owner, COLLISION_SHAPE_SPHERE, collisionCallback);
	self->dispose = CollisionSphere_dispose;
	self->interpolate = CollisionSphere_interpolate;
	self->isStatic = CollisionSphere_isStatic;
	self->getCollisionBounds = CollisionSphere_getCollisionBounds;
	self->position = position;
	self->lastPosition = position;
	self->radius = radius;
	return true;
}

void CollisionSphere_dispose(CollisionSphere * self) {
	call_super(dispose, self);
}

void CollisionSphere_updatePosition(CollisionSphere * self, Vector3x newPosition) {
	self->lastPosition = self->position;
	self->position = newPosition;
}

void CollisionSphere_interpolate(CollisionSphere * self, fixed16_16 amount) {
	self->lastPosition = Vector3x_interpolate(self->lastPosition, self->position, amount);
}

bool CollisionSphere_isStatic(CollisionSphere * self) {
	return self->position.x == self->lastPosition.x && self->position.y == self->lastPosition.y && self->position.z == self->lastPosition.z;
}

Box6x CollisionSphere_getCollisionBounds(CollisionSphere * self) {
	return Box6x_union(BOX6x(self->lastPosition.x - self->radius, self->lastPosition.x + self->radius, self->lastPosition.y - self->radius, self->lastPosition.y + self->radius, self->lastPosition.z - self->radius, self->lastPosition.z + self->radius), BOX6x(self->position.x - self->radius, self->position.x + self->radius, self->position.y - self->radius, self->position.y + self->radius, self->position.z - self->radius, self->position.z + self->radius));
}
