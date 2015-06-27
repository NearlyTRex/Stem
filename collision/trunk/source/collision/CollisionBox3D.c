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

#include "collision/CollisionBox3D.h"
#include "collision/CollisionShared.h"
#include <stdlib.h>

#define SUPERCLASS CollisionObject

CollisionBox3D * CollisionBox3D_create(void * owner, CollisionCallback collisionCallback, Vector3x position, Vector3x size) {
	stemobject_create_implementation(CollisionBox3D, init, owner, collisionCallback, position, size)
}

bool CollisionBox3D_init(CollisionBox3D * self, void * owner, CollisionCallback collisionCallback, Vector3x position, Vector3x size) {
	call_super(init, self, owner, COLLISION_SHAPE_BOX_3D, collisionCallback);
	self->dispose = CollisionBox3D_dispose;
	self->interpolate = CollisionBox3D_interpolate;
	self->position = position;
	self->lastPosition = position;
	self->size = size;
	self->lastSize = size;
	return true;
}

void CollisionBox3D_dispose(CollisionBox3D * self) {
	call_super(dispose, self);
}

void CollisionBox3D_updatePosition(CollisionBox3D * self, Vector3x newPosition) {
	self->lastPosition = self->position;
	self->position = newPosition;
}

void CollisionBox3D_updateSize(CollisionBox3D * self, Vector3x newSize) {
	self->lastSize = self->size;
	self->size = newSize;
}

void CollisionBox3D_setSolidity(CollisionBox3D * self, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop) {
}

void CollisionBox3D_interpolate(CollisionBox3D * self, fixed16_16 amount) {
	self->lastPosition = Vector3x_interpolate(self->lastPosition, self->position, amount);
	self->lastSize = Vector3x_interpolate(self->lastSize, self->size, amount);
}
