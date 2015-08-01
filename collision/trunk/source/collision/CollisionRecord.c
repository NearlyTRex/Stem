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

#include "collision/CollisionRecord.h"

CollisionRecord CollisionRecord_inverted(CollisionRecord collision) {
	CollisionObject * swapObject = collision.object1;
	collision.object1 = collision.object2;
	collision.object2 = swapObject;
	Vector3x_invert(&collision.normal);
	Vector3x swapVector = collision.object1Vector;
	collision.object1Vector = collision.object2Vector;
	collision.object2Vector = swapVector;
	return collision;
}

void CollisionRecord_resolve(CollisionRecord collision, fixed16_16 timesliceSize) {
	if (collision.object1->collisionCallback != NULL) {
		collision.object1->collisionCallback(collision, timesliceSize);
	}
	if (collision.object2->collisionCallback != NULL) {
		collision.object2->collisionCallback(CollisionRecord_inverted(collision), timesliceSize);
	}
}
