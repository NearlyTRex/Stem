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

#include "collision/CollisionLine2D.h"
#include "collision/CollisionShared.h"
#include <stdlib.h>

#define SUPERCLASS CollisionObject

CollisionLine2D * CollisionLine2D_create(void * owner, CollisionCallback collisionCallback, Vector2x endpoint1, Vector2x endpoint2, bool doubleSided) {
	stemobject_create_implementation(CollisionLine2D, init, owner, collisionCallback, endpoint1, endpoint2, doubleSided)
}

bool CollisionLine2D_init(CollisionLine2D * self, void * owner, CollisionCallback collisionCallback, Vector2x endpoint1, Vector2x endpoint2, bool doubleSided) {
	call_super(init, self, owner, COLLISION_SHAPE_LINE_2D, collisionCallback);
	self->dispose = CollisionLine2D_dispose;
	self->interpolate = CollisionLine2D_interpolate;
	self->isStatic = CollisionLine2D_isStatic;
	self->getCollisionBounds = CollisionLine2D_getCollisionBounds;
	self->endpoint1 = self->lastEndpoint1 = endpoint1;
	self->endpoint2 = self->lastEndpoint2 = endpoint2;
	self->doubleSided = doubleSided;
	return true;
}

void CollisionLine2D_dispose(CollisionLine2D * self) {
	call_super(dispose, self);
}

void CollisionLine2D_updateEndpoints(CollisionLine2D * self, Vector2x newEndpoint1, Vector2x newEndpoint2) {
	self->lastEndpoint1 = self->endpoint1;
	self->lastEndpoint2 = self->endpoint2;
	self->endpoint1 = newEndpoint1;
	self->endpoint2 = newEndpoint2;
}

void CollisionLine2D_interpolate(CollisionLine2D * self, fixed16_16 amount) {
	self->lastEndpoint1 = Vector2x_interpolate(self->lastEndpoint1, self->endpoint1, amount);
	self->lastEndpoint2 = Vector2x_interpolate(self->lastEndpoint2, self->endpoint2, amount);
}

bool CollisionLine2D_isStatic(CollisionLine2D * self) {
	return self->endpoint1.x == self->lastEndpoint1.x && self->endpoint1.y == self->lastEndpoint1.y &&
	       self->endpoint2.x == self->lastEndpoint2.x && self->endpoint2.y == self->lastEndpoint2.y;
}

static inline fixed16_16 min4(fixed16_16 value1, fixed16_16 value2, fixed16_16 value3, fixed16_16 value4) {
	fixed16_16 result;
	
	result = value1;
	if (value2 < result) {
		result = value2;
	}
	if (value3 < result) {
		result = value3;
	}
	if (value4 < result) {
		result = value4;
	}
	return result;
}

static inline fixed16_16 max4(fixed16_16 value1, fixed16_16 value2, fixed16_16 value3, fixed16_16 value4) {
	fixed16_16 result;
	
	result = value1;
	if (value2 > result) {
		result = value2;
	}
	if (value3 > result) {
		result = value3;
	}
	if (value4 > result) {
		result = value4;
	}
	return result;
}

Box6x CollisionLine2D_getCollisionBounds(CollisionLine2D * self) {
	Box6x bounds;
	
	bounds.left   = min4(self->lastEndpoint1.x, self->lastEndpoint2.x, self->endpoint1.x, self->endpoint2.x);
	bounds.right  = max4(self->lastEndpoint1.x, self->lastEndpoint2.x, self->endpoint1.x, self->endpoint2.x);
	bounds.bottom = min4(self->lastEndpoint1.y, self->lastEndpoint2.y, self->endpoint1.y, self->endpoint2.y);
	bounds.top    = max4(self->lastEndpoint1.y, self->lastEndpoint2.y, self->endpoint1.y, self->endpoint2.y);
	bounds.back   = 0x00000;
	bounds.front  = 0x10000;
	
	return bounds;
}
