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

#include "collision/CollisionObject.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

CollisionObject * CollisionObject_create(void * owner, int shapeType, CollisionCallback collisionCallback) {
	stemobject_create_implementation(CollisionObject, init, owner, shapeType, collisionCallback)
}

bool CollisionObject_init(CollisionObject * self, void * owner, int shapeType, CollisionCallback collisionCallback) {
	call_super(init, self);
	self->dispose = CollisionObject_dispose;
	self->interpolate = NULL;
	self->isStatic = CollisionObject_isStatic;
	self->getCollisionBounds = CollisionObject_getCollisionBounds;
	self->owner = owner;
	self->shapeType = shapeType;
	self->collisionCallback = collisionCallback;
	self->ownMask = 0x1;
	self->collidableMask = 0xFFFFFFFF;
	self->private_ivar(markedForRemoval) = false;
	self->private_ivar(unresolvable) = false;
	return true;
}

void CollisionObject_dispose(CollisionObject * self) {
	call_super(dispose, self);
}

bool CollisionObject_isStatic(CollisionObject * self) {
	return false;
}

Box6x CollisionObject_getCollisionBounds(CollisionObject * self) {
	return BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000);
}

CollisionObject * CollisionObject_setMasks(CollisionObject * self, uint32_t ownMask, uint32_t collidableMask) {
	self->ownMask = ownMask;
	self->collidableMask = collidableMask;
	return self;
}
