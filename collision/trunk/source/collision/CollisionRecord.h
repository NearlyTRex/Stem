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

#ifndef __CollisionRecord_H__
#define __CollisionRecord_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "collision/CollisionObject.h"
#include "gamemath/FixedPoint.h"
#include "gamemath/Vector3x.h"

struct CollisionRecord {
	// Target colliding object
	CollisionObject * object1;
	
	// Object with which target collided
	CollisionObject * object2;
	
	// Normal vector of the surface of object2 at the first point of intersection by object1
	Vector3x normal;
	
	// Temporal position within the timeslice being evaluated at which the intersection first occurs, from 0x00000 (beginning
	// of timeslice) to 0x10000 (end of timeslice).
	fixed16_16 time;
	// TODO: Doesn't this require an interpolate() method on owners of CollisionObjects?
};

// Returns a CollisionRecord with object1 and object2 swapped, and the normal reversed.
static inline CollisionRecord CollisionRecord_inverted(CollisionRecord collision) {
	CollisionObject * swap = collision.object1;
	collision.object1 = collision.object2;
	collision.object2 = swap;
	Vector3x_invert(&collision.normal);
	return collision;
}

#ifdef __cplusplus
}
#endif
#endif
