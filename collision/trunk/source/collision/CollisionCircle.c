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
	self->position = position;
	self->lastPosition = position;
	self->radius = radius;
	self->lastRadius = radius;
	return true;
}

void CollisionCircle_dispose(CollisionCircle * self) {
	call_super(dispose, self);
}

void CollisionCircle_updatePosition(CollisionCircle * self, Vector2x newPosition) {
	self->lastPosition = self->position;
	self->position = newPosition;
}

void CollisionCircle_updateRadius(CollisionCircle * self, fixed16_16 newRadius) {
	self->lastRadius = self->radius;
	self->radius = newRadius;
}

void CollisionCircle_interpolate(CollisionCircle * self, fixed16_16 amount) {
	self->lastPosition = Vector2x_interpolate(self->lastPosition, self->position, amount);
	self->lastRadius = xmul(self->lastRadius, (0x10000 - amount)) + xmul(self->radius, amount);
}
