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

#ifndef __CollisionSphere_H__
#define __CollisionSphere_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionSphere CollisionSphere;

#include "collision/CollisionObject.h"
#include "gamemath/Vector3x.h"

#define CollisionSphere_structContents(self_type) \
	CollisionObject_structContents(self_type) \
	\
	Vector3x position; \
	Vector3x lastPosition; \
	fixed16_16 radius;

stemobject_struct_definition(CollisionSphere)

// position is the center of the sphere.
// radius extends the sphere outward from position.
CollisionSphere * CollisionSphere_create(void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius);
bool CollisionSphere_init(CollisionSphere * self, void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius);
void CollisionSphere_dispose(CollisionSphere * self);

void CollisionSphere_updatePosition(CollisionSphere * self, Vector3x newPosition);
void CollisionSphere_interpolate(CollisionSphere * self, fixed16_16 amount);

#ifdef __cplusplus
}
#endif
#endif
