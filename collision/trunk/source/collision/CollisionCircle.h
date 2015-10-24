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

#ifndef __CollisionCircle_H__
#define __CollisionCircle_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionCircle CollisionCircle;

#include "collision/CollisionObject.h"
#include "gamemath/Vector2x.h"

#define CollisionCircle_structContents(self_type) \
	CollisionObject_structContents(self_type) \
	\
	Vector2x position; \
	Vector2x lastPosition; \
	fixed16_16 radius;

stemobject_struct_definition(CollisionCircle)

// position is the center of the circle.
// radius extends the circle outward from position.
CollisionCircle * CollisionCircle_create(void * owner, CollisionCallback collisionCallback, Vector2x position, fixed16_16 radius);
bool CollisionCircle_init(CollisionCircle * self, void * owner, CollisionCallback collisionCallback, Vector2x position, fixed16_16 radius);
void CollisionCircle_dispose(CollisionCircle * self);

void CollisionCircle_updatePosition(CollisionCircle * self, Vector2x newPosition);
void CollisionCircle_interpolate(CollisionCircle * self, fixed16_16 amount);

bool CollisionCircle_isStatic(CollisionCircle * self);
Box6x CollisionCircle_getCollisionBounds(CollisionCircle * self);

#ifdef __cplusplus
}
#endif
#endif
