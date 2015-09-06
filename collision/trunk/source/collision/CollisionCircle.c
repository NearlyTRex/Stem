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

#include "collision/CollisionCircle.h"
#include "collision/CollisionShared.h"
#include <stdlib.h>

#define SUPERCLASS CollisionObject

CollisionCircle * CollisionCircle_create(void * owner, CollisionCallback collisionCallback, Vector2x position, fixed16_16 radius) {
	stemobject_create_implementation(CollisionCircle, init, owner, collisionCallback, position, radius)
}

bool CollisionCircle_init(CollisionCircle * self, void * owner, CollisionCallback collisionCallback, Vector2x position, fixed16_16 radius) {
	call_super(init, self, owner, COLLISION_SHAPE_CIRCLE, collisionCallback);
	self->dispose = CollisionCircle_dispose;
	self->interpolate = CollisionCircle_interpolate;
	self->isStatic = CollisionCircle_isStatic;
	self->getCollisionBounds = CollisionCircle_getCollisionBounds;
	self->position = position;
	self->lastPosition = position;
	self->radius = radius;
	return true;
}

void CollisionCircle_dispose(CollisionCircle * self) {
	call_super(dispose, self);
}

void CollisionCircle_updatePosition(CollisionCircle * self, Vector2x newPosition) {
	self->lastPosition = self->position;
	self->position = newPosition;
}

void CollisionCircle_interpolate(CollisionCircle * self, fixed16_16 amount) {
	self->lastPosition = Vector2x_interpolate(self->lastPosition, self->position, amount);
}

bool CollisionCircle_isStatic(CollisionCircle * self) {
	return self->position.x == self->lastPosition.x && self->position.y == self->lastPosition.y;
}

Box6x CollisionCircle_getCollisionBounds(CollisionCircle * self) {
	return Box6x_union(BOX6x(self->lastPosition.x - self->radius, self->lastPosition.x + self->radius, self->lastPosition.y - self->radius, self->lastPosition.y + self->radius, 0x00000, 0x10000), BOX6x(self->position.x - self->radius, self->position.x + self->radius, self->position.y - self->radius, self->position.y + self->radius, 0x00000, 0x10000));
}
