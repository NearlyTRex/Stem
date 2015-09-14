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

#ifndef __CollisionRect2D_H__
#define __CollisionRect2D_H__
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
	Vector2x size; \
	Vector2x lastSize; \
	bool solidLeft; \
	bool solidRight; \
	bool solidBottom; \
	bool solidTop; \
	fixed16_16 thickness;

stemobject_struct_definition(CollisionRect2D)

// position is the corner of the rect with the lowest x and y axis values.
// size extends the rect from position in the +x and +y directions.
CollisionRect2D * CollisionRect2D_create(void * owner, CollisionCallback collisionCallback, Vector2x position, Vector2x size);
bool CollisionRect2D_init(CollisionRect2D * self, void * owner, CollisionCallback collisionCallback, Vector2x position, Vector2x size);
void CollisionRect2D_dispose(CollisionRect2D * self);

// Problem (?): updatePosition inside a collision callback will do the wrong thing with lastPosition
void CollisionRect2D_updatePosition(CollisionRect2D * self, Vector2x newPosition);
void CollisionRect2D_updateSize(CollisionRect2D * self, Vector2x newSize);
void CollisionRect2D_setSolidity(CollisionRect2D * self, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop);
void CollisionRect2D_interpolate(CollisionRect2D * self, fixed16_16 amount);

bool CollisionRect2D_isStatic(CollisionRect2D * self);
Box6x CollisionRect2D_getCollisionBounds(CollisionRect2D * self);

#ifdef __cplusplus
}
#endif
#endif
