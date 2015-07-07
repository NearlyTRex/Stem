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
	
	// Temporal position within the timeslice being evaluated at which the intersection first occurs, from 0x00000 (beginning
	// of timeslice) to 0x10000 (end of timeslice).
	fixed16_16 time;
	
	// Normal vector of the surface of object2 at the first point of intersection by object1
	Vector3x normal;
	
	// Calculated vector of motion of the point on object1 that contacted object2. Relative to timeslice size.
	Vector3x object1Vector;
	
	// Calculated vector of motion of the point on object2 that contacted object1. Relative to timeslice size.
	Vector3x object2Vector;
};

// Returns a CollisionRecord with object1 and object2 swapped, and the normal reversed.
CollisionRecord CollisionRecord_inverted(CollisionRecord collision);

// Calls the appropriate CollisionCallback for object1 and object2 in collision. It is the responsibility of each
// CollisionCallback to adjust the CollisionObject's trajectory to resolve the detected collision.
void CollisionRecord_resolve(CollisionRecord collision, fixed16_16 timesliceSize);

#ifdef __cplusplus
}
#endif
#endif
