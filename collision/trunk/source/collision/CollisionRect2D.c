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
#include "gamemath/Rect4x.h"
#include <stdlib.h>

#define SUPERCLASS CollisionObject

CollisionRect2D * CollisionRect2D_create(void * owner, CollisionCallback collisionCallback, Vector2x position, Vector2x size) {
	stemobject_create_implementation(CollisionRect2D, init, owner, collisionCallback, position, size)
}

bool CollisionRect2D_init(CollisionRect2D * self, void * owner, CollisionCallback collisionCallback, Vector2x position, Vector2x size) {
	call_super(init, self, owner, COLLISION_SHAPE_RECT_2D, collisionCallback);
	self->dispose = CollisionRect2D_dispose;
	self->interpolate = CollisionRect2D_interpolate;
	self->isStatic = CollisionRect2D_isStatic;
	self->getCollisionBounds = CollisionRect2D_getCollisionBounds;
	self->position = position;
	self->lastPosition = position;
	self->size = size;
	self->lastSize = size;
	self->solidLeft = true;
	self->solidRight = true;
	self->solidBottom = true;
	self->solidTop = true;
	return true;
}

void CollisionRect2D_dispose(CollisionRect2D * self) {
	call_super(dispose, self);
}

void CollisionRect2D_updatePosition(CollisionRect2D * self, Vector2x newPosition) {
	self->lastPosition = self->position;
	self->position = newPosition;
}

void CollisionRect2D_updateSize(CollisionRect2D * self, Vector2x newSize) {
	self->lastSize = self->size;
	self->size = newSize;
}

void CollisionRect2D_setSolidity(CollisionRect2D * self, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop) {
	self->solidLeft = solidLeft;
	self->solidRight = solidRight;
	self->solidBottom = solidBottom;
	self->solidTop = solidTop;
}

void CollisionRect2D_interpolate(CollisionRect2D * self, fixed16_16 amount) {
	self->lastPosition = Vector2x_interpolate(self->lastPosition, self->position, amount);
	self->lastSize = Vector2x_interpolate(self->lastSize, self->size, amount);
}

bool CollisionRect2D_isStatic(CollisionRect2D * self) {
	return self->position.x == self->lastPosition.x && self->position.y == self->lastPosition.y &&
	       self->size.x == self->lastSize.x && self->size.y == self->lastSize.y;
}

Box6x CollisionRect2D_getCollisionBounds(CollisionRect2D * self) {
	Rect4x unionRect;
	
	unionRect = Rect4x_union(RECT4x(self->lastPosition.x, self->lastPosition.x + self->lastSize.x, self->lastPosition.y, self->lastPosition.y + self->lastSize.y),
	                         RECT4x(self->position.x, self->position.x + self->size.x, self->position.y, self->position.y + self->size.y));
	return BOX6x(unionRect.left, unionRect.right, unionRect.bottom, unionRect.top, 0x00000, 0x10000);
}
