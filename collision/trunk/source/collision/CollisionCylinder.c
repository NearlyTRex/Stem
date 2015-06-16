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

#include "collision/CollisionCylinder.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

CollisionCylinder * CollisionCylinder_create(void * owner, fixed16_16 radius, fixed16_16 height, Vector3x bottomCenter, CollisionCallback collisionCallback) {
	stemobject_create_implementation(CollisionCylinder, init, owner, radius, height, bottomCenter, collisionCallback)
}

bool CollisionCylinder_init(CollisionCylinder * self, void * owner, fixed16_16 radius, fixed16_16 height, Vector3x bottomCenter, CollisionCallback collisionCallback) {
	call_super(init, self);
	self->dispose = CollisionCylinder_dispose;
	return true;
}

void CollisionCylinder_dispose(CollisionCylinder * self) {
	call_super(dispose, self);
}

void CollisionCylinder_updatePosition(CollisionCylinder * self, Vector3x newPosition) {
	//self->shape.cylinder.lastBottomCenter = self->shape.cylinder.bottomCenter;
	//self->shape.cylinder.bottomCenter = newPosition;
}

void CollisionCylinder_interpolate(CollisionCylinder * self, fixed16_16 frameTime) {
	//self->shape.cylinder.lastBottomCenter = Vector3x_interpolate(self->shape.cylinder.lastBottomCenter, self->shape.cylinder.bottomCenter, frameTime);
}
