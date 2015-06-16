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

#ifndef __CollisionObject_H__
#define __CollisionObject_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionObject CollisionObject;

#include "collision/CollisionResolver.h"
#include "gamemath/FixedPoint.h"
#include "gamemath/Vector3x.h"
#include "stemobject/StemObject.h"

typedef void (* CollisionCallback)(CollisionRecord collision, fixed16_16 timesliceSize);

#define CollisionObject_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	void * owner; \
	int shapeType; \
	CollisionCallback collisionCallback; \
	\
	void (* updatePosition)(self_type * self, Vector3x newPosition); \
	void (* interpolate)(self_type * self, fixed16_16 frameTime);

stemobject_struct_definition(CollisionObject)

bool CollisionObject_init(CollisionObject * self, void * owner, int shapeType, CollisionCallback collisionCallback);
void CollisionObject_dispose(CollisionObject * self);

#ifdef __cplusplus
}
#endif
#endif
