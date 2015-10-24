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

#ifndef __CollisionLine2D_H__
#define __CollisionLine2D_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionLine2D CollisionLine2D;

#include "collision/CollisionObject.h"
#include "gamemath/Vector2x.h"

#define CollisionLine2D_structContents(self_type) \
	CollisionObject_structContents(self_type) \
	\
	Vector2x endpoint1; \
	Vector2x endpoint2; \
	Vector2x lastEndpoint1; \
	Vector2x lastEndpoint2; \
	bool doubleSided;

stemobject_struct_definition(CollisionLine2D)

// If doubleSided, both flat surfaces of the line will be collidable.
// If not doubleSided, only the side counterclockwise from endpoint1 to endpoint2 will be collidable.
// (example: if endpoint2 is to the right of endpoint1, the top of a single-sided line will be solid)
CollisionLine2D * CollisionLine2D_create(void * owner, CollisionCallback collisionCallback, Vector2x endpoint1, Vector2x endpoint2, bool doubleSided);
bool CollisionLine2D_init(CollisionLine2D * self, void * owner, CollisionCallback collisionCallback, Vector2x endpoint1, Vector2x endpoint2, bool doubleSided);
void CollisionLine2D_dispose(CollisionLine2D * self);

void CollisionLine2D_updateEndpoints(CollisionLine2D * self, Vector2x newEndpoint1, Vector2x newEndpoint2);
void CollisionLine2D_interpolate(CollisionLine2D * self, fixed16_16 amount);

bool CollisionLine2D_isStatic(CollisionLine2D * self);
Box6x CollisionLine2D_getCollisionBounds(CollisionLine2D * self);

#ifdef __cplusplus
}
#endif
#endif
