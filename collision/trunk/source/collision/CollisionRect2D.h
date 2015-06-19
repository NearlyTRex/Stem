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
	bool concave; \
	Vector2x position; \
	Vector2x lastPosition; \
	Vector2x size; \
	Vector2x lastSize;

stemobject_struct_definition(CollisionRect2D)

// position is the corner of the rect with the lowest x and y axis values.
// size extends the rect from position in the +x and +y directions.
// If concave is true, this rect will be considered an enclosed empty space, and collisions will be detected with its inner edges
// for objects moving outward. Objects moving from outside a concave rect into it will not collide with its edges.
// If concave is false, this rect will be considered solid, and collisions will be detected with its outer edges for
// objects moving inward. Objects moving out of convex rect from inside it will not collide with its edges.
CollisionRect2D * CollisionRect2D_create(void * owner, CollisionCallback collisionCallback, Vector2x position, Vector2x size, bool concave);
bool CollisionRect2D_init(CollisionRect2D * self, void * owner, CollisionCallback collisionCallback, Vector2x position, Vector2x size, bool concave);
void CollisionRect2D_dispose(CollisionRect2D * self);

// Problem (?): updatePosition inside a collision callback will do the wrong thing with lastPosition
void CollisionRect2D_updatePosition(CollisionRect2D * self, Vector2x newPosition);
void CollisionRect2D_updateSize(CollisionRect2D * self, Vector2x newSize);
void CollisionRect2D_interpolate(CollisionRect2D * self, fixed16_16 amount);

#ifdef __cplusplus
}
#endif
#endif
