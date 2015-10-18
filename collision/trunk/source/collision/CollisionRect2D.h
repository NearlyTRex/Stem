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
	fixed16_16 edgeThickness;

stemobject_struct_definition(CollisionRect2D)

// position is the corner of the rect with the lowest x and y axis values.
// size extends the rect from position in the +x and +y directions.
// edgeThickness specifies how far inside (or outside, if concave) the edges of the rect should be considered to be solid.
// Two rects with an edgeThickness of 0 may be able to phase through each other in some cases due to rounding.
// If you specify EDGE_THICKNESS_DEFAULT, thickness will automatically adjust to the rect's size on each axis, such
// that any geometry already penetrating the rect and moving inward will be considered to have collided with it, and geometry
// penetrating but moving outward will be allowed to pass through unhindered.
// Concave rects treat EDGE_THICKNESS_DEFAULT as a value of 0.
CollisionRect2D * CollisionRect2D_create(void * owner, CollisionCallback collisionCallback, Vector2x position, Vector2x size, fixed16_16 edgeThickness);
bool CollisionRect2D_init(CollisionRect2D * self, void * owner, CollisionCallback collisionCallback, Vector2x position, Vector2x size, fixed16_16 edgeThickness);
void CollisionRect2D_dispose(CollisionRect2D * self);

// TODO (maybe): updatePosition inside a collision callback will do the wrong thing with lastPosition; must access position directly to modify
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
