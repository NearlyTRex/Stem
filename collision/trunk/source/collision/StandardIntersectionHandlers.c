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

#include "collision/StandardIntersectionHandlers.h"

/*
static bool testPlaneIntersectionX(fixed16_16 x1Start, fixed16_16 x1End,
                                   fixed16_16 x2Start, fixed16_16 x2End,
                                   fixed16_16 bottom1Start, fixed16_16 top1Start, fixed16_16 bottom1End, fixed16_16 top1End,
                                   fixed16_16 bottom2Start, fixed16_16 top2Start, fixed16_16 bottom2End, fixed16_16 top2End,
                                   fixed16_16 back1Start, fixed16_16 front1Start, fixed16_16 back1End, fixed16_16 front1End,
                                   fixed16_16 back2Start, fixed16_16 front2Start, fixed16_16 back2End, fixed16_16 front2End,
                                   fixed16_16 * outTime) {
	fixed16_16 time;
	fixed16_16 bottom1, top1, bottom2, top2, back1, front1, back2, front2;
	
	time = xdiv(x1Start - x2Start, x2End - x2Start - x1End + x1Start);
	if (time > 0x10000) {
		return false;
	}
	bottom1 = bottom1Start + xmul(bottom1End - bottom1Start, time);
	top1    = top1Start    + xmul(top1End    - top1Start,    time);
	bottom2 = bottom2Start + xmul(bottom2End - bottom2Start, time);
	top2    = top2Start    + xmul(top2End    - top2Start,    time);
	back1   = back1Start   + xmul(back1End   - back1Start,   time);
	front1  = front1Start  + xmul(front1End  - front1Start,  time);
	back2   = back2Start   + xmul(back2End   - back2Start,   time);
	front2  = front2Start  + xmul(front2End  - front2Start,  time);
	if (top1 > bottom2 && bottom1 < top2 && front1 > back2 && back1 < front2) {
		if (time < 0) {
			time = 0;
		}
		*outTime = time;
		return true;
	}
	return false;
}

#define collision_test_axis(xLow, xHigh, yLow, yHigh, zLow, zHigh, solidityBitLow, solidityBitHigh, intersectionTimeVar, directionVar) \
	if ((object1->solidity & solidityBitHigh) && (object2->solidity & solidityBitLow) && \
	    (object1->lastBounds.xLow != object1->bounds.xLow || object2->lastBounds.xHigh != object2->bounds.xHigh) && \
	    object1->lastBounds.xLow > object2->bounds.xHigh - INTERSECTION_EPSILON && object1->bounds.xLow <= object2->bounds.xHigh - INTERSECTION_EPSILON) { \
		if (testPlaneIntersectionX(object1->lastBounds.xLow, object1->bounds.xLow, \
		                           object2->lastBounds.xHigh, object2->bounds.xHigh, \
		                           object1->lastBounds.yLow, object1->lastBounds.yHigh, object1->bounds.yLow, object1->bounds.yHigh, \
		                           object2->lastBounds.yLow, object2->lastBounds.yHigh, object2->bounds.yLow, object2->bounds.yHigh, \
		                           object1->lastBounds.zLow, object1->lastBounds.zHigh, object1->bounds.zLow, object1->bounds.zHigh, \
		                           object2->lastBounds.zLow, object2->lastBounds.zHigh, object2->bounds.zLow, object2->bounds.zHigh, \
		                           &intersectionTimeVar)) { \
			directionVar = -1; \
		} \
		\
	} else if ((object1->solidity & solidityBitLow) && (object2->solidity & solidityBitHigh) && \
	           (object1->lastBounds.xHigh != object1->bounds.xHigh || object2->lastBounds.xLow != object2->bounds.xLow) && \
	           object1->lastBounds.xHigh < object2->lastBounds.xLow + INTERSECTION_EPSILON && object1->bounds.xHigh >= object2->bounds.xLow + INTERSECTION_EPSILON) { \
		if (testPlaneIntersectionX(object1->lastBounds.xHigh, object1->bounds.xHigh, \
		                           object2->lastBounds.xLow, object2->bounds.xLow, \
		                           object1->lastBounds.yLow, object1->lastBounds.yHigh, object1->bounds.yLow, object1->bounds.yHigh, \
		                           object2->lastBounds.yLow, object2->lastBounds.yHigh, object2->bounds.yLow, object2->bounds.yHigh, \
		                           object1->lastBounds.zLow, object1->lastBounds.zHigh, object1->bounds.zLow, object1->bounds.zHigh, \
		                           object2->lastBounds.zLow, object2->lastBounds.zHigh, object2->bounds.zLow, object2->bounds.zHigh, \
		                           &intersectionTimeVar)) { \
			directionVar = 1; \
		} \
	}

static bool collisionTest(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, CollisionSide * outSide) {
	fixed16_16 intersectionTimeX = FIXED_16_16_MAX, intersectionTimeY = FIXED_16_16_MAX, intersectionTimeZ = FIXED_16_16_MAX;
	int directionX = 0, directionY = 0, directionZ = 0;
	
	collision_test_axis(left, right, bottom, top, back, front, SOLID_LEFT, SOLID_RIGHT, intersectionTimeX, directionX)
	collision_test_axis(bottom, top, back, front, left, right, SOLID_BOTTOM, SOLID_TOP, intersectionTimeY, directionY)
	collision_test_axis(back, front, left, right, bottom, top, SOLID_BACK, SOLID_FRONT, intersectionTimeZ, directionZ)
	
	if (directionX != 0 || directionY != 0 || directionZ != 0) {
		if (intersectionTimeX <= intersectionTimeY && intersectionTimeX <= intersectionTimeZ) {
			*outTime = intersectionTimeX;
			*outSide = directionX == -1 ? SIDE_LEFT : SIDE_RIGHT;
			
		} else if (intersectionTimeY <= intersectionTimeX && intersectionTimeY <= intersectionTimeZ) {
			*outTime = intersectionTimeY;
			*outSide = directionY == -1 ? SIDE_BOTTOM : SIDE_TOP;
			
		} else {
			*outTime = intersectionTimeZ;
			*outSide = directionZ == -1 ? SIDE_BACK : SIDE_FRONT;
		}
		
		return true;
	}
	
	return false;
}
*/

bool intersectionHandler_rect_rect(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_rect_circle(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_rect_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_rect_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_circle_circle(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_circle_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_circle_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_line2D_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_line2D_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_polygon_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box_box(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box_sphere(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box_cylinder(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_sphere_sphere(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_sphere_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_sphere_cylinder(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_sphere_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_sphere_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_line3D_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_line3D_cylinder(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_line3D_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_line3D_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_cylinder_cylinder(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_cylinder_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_cylinder_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_capsule_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_capsule_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_trimesh_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}
