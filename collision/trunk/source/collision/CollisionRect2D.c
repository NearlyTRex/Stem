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

#include "collision/CollisionRect2D.h"
#include "collision/CollisionShared.h"
#include <stdlib.h>

#define SUPERCLASS CollisionObject

CollisionRect2D * CollisionRect2D_create(void * owner, CollisionCallback collisionCallback) {
	stemobject_create_implementation(CollisionRect2D, init, owner, collisionCallback)
}

bool CollisionRect2D_init(CollisionRect2D * self, void * owner, CollisionCallback collisionCallback) {
	call_super(init, self, owner, COLLISION_SHAPE_RECT_2D, collisionCallback);
	self->dispose = CollisionRect2D_dispose;
	self->interpolate = CollisionRect2D_interpolate;
	self->position = VECTOR2x_ZERO;
	self->lastPosition = VECTOR2x_ZERO;
	self->private_ivar(positionInited) = false;
	self->size = VECTOR2x_ZERO;
	self->lastSize = VECTOR2x_ZERO;
	self->private_ivar(sizeInited) = false;
	return true;
}

void CollisionRect2D_dispose(CollisionRect2D * self) {
	call_super(dispose, self);
}

void CollisionRect2D_updatePosition(CollisionRect2D * self, Vector2x newPosition) {
	if (self->private_ivar(positionInited)) {
		self->lastPosition = self->position;
	} else {
		self->lastPosition = newPosition;
		self->private_ivar(positionInited) = true;
	}
	self->position = newPosition;
}

void CollisionRect2D_updateSize(CollisionRect2D * self, Vector2x newSize) {
	if (self->private_ivar(sizeInited)) {
		self->lastSize = self->size;
	} else {
		self->lastSize = newSize;
		self->private_ivar(sizeInited) = true;
	}
	self->size = newSize;
}

void CollisionRect2D_updateBounds(CollisionRect2D * self, fixed16_16 left, fixed16_16 bottom, fixed16_16 right, fixed16_16 top) {
}

void CollisionRect2D_interpolate(CollisionRect2D * self, fixed16_16 amount) {
}
