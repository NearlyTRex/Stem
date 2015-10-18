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

#ifndef __CollisionBox3D_H__
#define __CollisionBox3D_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionBox3D CollisionBox3D;

#include "collision/CollisionObject.h"
#include "gamemath/Vector3x.h"

#define CollisionBox3D_structContents(self_type) \
	CollisionObject_structContents(self_type) \
	\
	Vector3x position; \
	Vector3x lastPosition; \
	Vector3x size; \
	Vector3x lastSize; \
	bool solidLeft; \
	bool solidRight; \
	bool solidBottom; \
	bool solidTop; \
	bool solidBack; \
	bool solidFront; \
	fixed16_16 edgeThickness;

stemobject_struct_definition(CollisionBox3D)

// position is the corner of the rect with the lowest x and y axis values.
// size extends the rect from position in the +x and +y directions.
// edgeThickness specifies how far inside (or outside, if concave) the faces of the box should be considered to be solid.
// Two boxes with an edgeThickness of 0 may be able to phase through each other in some cases due to rounding.
// If you specify EDGE_THICKNESS_DEFAULT, thickness will automatically adjust to the box's size on each axis, such
// that any geometry already penetrating the box and moving inward will be considered to have collided with it, and geometry
// penetrating but moving outward will be allowed to pass through unhindered.
// Concave rects treat EDGE_THICKNESS_DEFAULT as a value of 0.
CollisionBox3D * CollisionBox3D_create(void * owner, CollisionCallback collisionCallback, Vector3x position, Vector3x size, fixed16_16 edgeThickness);
bool CollisionBox3D_init(CollisionBox3D * self, void * owner, CollisionCallback collisionCallback, Vector3x position, Vector3x size, fixed16_16 edgeThickness);
void CollisionBox3D_dispose(CollisionBox3D * self);

void CollisionBox3D_updatePosition(CollisionBox3D * self, Vector3x newPosition);
void CollisionBox3D_updateSize(CollisionBox3D * self, Vector3x newSize);
void CollisionBox3D_setSolidity(CollisionBox3D * self, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop, bool solidBack, bool solidFront);
void CollisionBox3D_interpolate(CollisionBox3D * self, fixed16_16 amount);

bool CollisionBox3D_isStatic(CollisionBox3D * self);
Box6x CollisionBox3D_getCollisionBounds(CollisionBox3D * self);

#ifdef __cplusplus
}
#endif
#endif
