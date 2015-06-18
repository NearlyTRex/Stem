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

#ifndef __CollisionRect_H__
#define __CollisionRect_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionRect2D CollisionRect2D;

#include "collision/CollisionObject.h"
#include "gamemath/Vector2x.h"

#define CollisionRect2D_structContents(self_type) \
	CollisionObject_structContents(self_type) \
	\
	Vector2x position; \
	Vector2x lastPosition; \
	bool private_ivar(positionInited); \
	Vector2x size; \
	Vector2x lastSize; \
	bool private_ivar(sizeInited);

stemobject_struct_definition(CollisionRect2D)

CollisionRect2D * CollisionRect2D_create(void * owner, CollisionCallback collisionCallback);
bool CollisionRect2D_init(CollisionRect2D * self, void * owner, CollisionCallback collisionCallback);
void CollisionRect2D_dispose(CollisionRect2D * self);

void CollisionRect2D_updatePosition(CollisionRect2D * self, Vector2x newPosition);
void CollisionRect2D_updateSize(CollisionRect2D * self, Vector2x newSize);
void CollisionRect2D_updateBounds(CollisionRect2D * self, fixed16_16 left, fixed16_16 bottom, fixed16_16 right, fixed16_16 top);
void CollisionRect2D_interpolate(CollisionRect2D * self, fixed16_16 amount);

#ifdef __cplusplus
}
#endif
#endif
