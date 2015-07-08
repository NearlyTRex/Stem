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

#include "collision/CollisionCircle.h"
#include "collision/CollisionRect2D.h"
#include "collision/StandardIntersectionHandlers.h"
#include <stdio.h>

#define SPATIAL_EPSILON 0x00008

static bool intersectSweptLineSegments(fixed16_16 x1Start, fixed16_16 x1End,
                                       fixed16_16 x2Start, fixed16_16 x2End,
                                       fixed16_16 bottom1Start, fixed16_16 top1Start, fixed16_16 bottom1End, fixed16_16 top1End,
                                       fixed16_16 bottom2Start, fixed16_16 top2Start, fixed16_16 bottom2End, fixed16_16 top2End,
                                       fixed16_16 * outTime) {
	fixed16_16 time;
	fixed16_16 bottom1, top1, bottom2, top2;
	
	time = xdiv(x1Start - x2Start, x2End - x2Start - x1End + x1Start);
	if (time > 0x10000 || time < 0x00000) {
		return false;
	}
	
	bottom1 = bottom1Start + xmul(bottom1End - bottom1Start, time);
	top1    = top1Start    + xmul(top1End    - top1Start,    time);
	bottom2 = bottom2Start + xmul(bottom2End - bottom2Start, time);
	top2    = top2Start    + xmul(top2End    - top2Start,    time);
	
	if (bottom1 > top1) {
		fixed16_16 swap = top1;
		top1 = bottom1;
		bottom1 = swap;
	}
	if (bottom2 > top2) {
		fixed16_16 swap = top2;
		top2 = bottom2;
		bottom2 = swap;
	}
	
	if (top1 > bottom2 && bottom1 < top2) {
		*outTime = time;
		return true;
	}
	return false;
}

/*
static bool intersectSweptQuads(fixed16_16 x1Start, fixed16_16 x1End,
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
*/

static bool intersectSweptPoints(fixed16_16 x1Start, fixed16_16 x1End,
                                 fixed16_16 y1Start, fixed16_16 y1End,
                                 fixed16_16 x2Start, fixed16_16 x2End,
                                 fixed16_16 y2Start, fixed16_16 y2End,
                                 fixed16_16 * outTime) {
	fixed16_16 time1, time2;
	
	time1 = xdiv(x1Start - x2Start, x2End - x2Start - x1End + x1Start);
	time2 = xdiv(y1Start - y2Start, y2End - y2Start - y1End + y1Start);
	if (time1 > 0x10000 || time1 < 0x00000 || time2 > 0x10000 || time2 < 0x00000 || time1 != time2) {
		return false;
	}
	*outTime = time1;
	return true;
}

static inline int64_t fixed48_16_multiply(int64_t lhs, int64_t rhs) {
	int64_t product = lhs * rhs;
	
	if (product < 0) {
		product = -((-product >> 16) + ((-product >> 15) % 2));
	} else {
		product = (product >> 16) + ((product >> 15) % 2);
	}
	return product;
}

static inline int64_t fixed48_16_divide(int64_t lhs, int64_t rhs) {
	if (rhs == 0) {
		return FIXED_16_16_INF;
	}
	lhs <<= 16;
	lhs = lhs / rhs + (((lhs << 1) / rhs) % 2);
	
	return lhs;
}

#define MAX_ITERATIONS 128

static inline int64_t fixed48_16_sqrt(int64_t x) {
	int64_t estimate = 0x10000, lastEstimate = -1, lastEstimate2 = -1;
	unsigned int iteration = 0;
	
	if (x < 0) {
		return FIXED_16_16_NAN;
	}
	if (x == 0) {
		return 0;
	}
	while (estimate != lastEstimate && estimate != lastEstimate2 && iteration < MAX_ITERATIONS) {
		lastEstimate2 = lastEstimate;
		lastEstimate = estimate;
		estimate = (((x << 16) / estimate) + estimate) / 2;
		iteration++;
	}
	return estimate;
}

// Adapted from http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=2
static bool quadraticFormula(fixed16_16 a,
                             fixed16_16 b,
                             fixed16_16 c,
                             fixed16_16 * outR1,
                             fixed16_16 * outR2) {
	// fixed16_16 doesn't have enough range for the quadratic formula
	int64_t q = fixed48_16_multiply(b, b) - 4 * fixed48_16_multiply(a, c);
	
	if (q >= 0) {
		int64_t sq = fixed48_16_sqrt(q);
		// d would normally be xdiv(0x10000, a * 2), but 1/a*2 doesn't have enough precision with only 16 mantissa bits; use 32 and shift instead
		int64_t d = fixed48_16_divide(0x100000000ll, (int64_t) a * 2);
		*outR1 = fixed48_16_multiply(-b + sq, d) >> 16;
		*outR2 = fixed48_16_multiply(-b - sq, d) >> 16;
		return true;
	}
	return false;
}

// Adapted from http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=2
static bool intersectSweptCircles(Vector2x circle1LastPosition, Vector2x circle1Position, fixed16_16 circle1Radius,
                                  Vector2x circle2LastPosition, Vector2x circle2Position, fixed16_16 circle2Radius,
                                  fixed16_16 * outTime, Vector3x * outNormal) {
	
	fixed16_16 radiusSum = circle1Radius + circle2Radius;
	fixed16_16 radiusSumSquare = xmul(radiusSum, radiusSum);
	fixed16_16 lastDistanceSquare = Vector2x_distanceSquared(circle1LastPosition, circle2LastPosition);
	fixed16_16 distanceSquare = Vector2x_distanceSquared(circle1Position, circle2Position);
	
	if (lastDistanceSquare < radiusSumSquare && distanceSquare < radiusSumSquare && lastDistanceSquare > distanceSquare) {
		// If already penetrating and trying to penetrate deeper, put a stop to it early
		Vector3x normal;
		
		normal.x = circle1LastPosition.x - circle2LastPosition.x;
		normal.y = circle1LastPosition.y - circle2LastPosition.y;
		normal.z = 0x00000;
		Vector3x_normalize(&normal);
		*outNormal = normal;
		*outTime = 0x00000;
		return true;
	}
	
	Vector2x circle1Vector          = Vector2x_subtract(circle1Position, circle1LastPosition);
	Vector2x circle2Vector          = Vector2x_subtract(circle2Position, circle2LastPosition);
	Vector2x circle1ToCircle2Vector = Vector2x_subtract(circle2LastPosition, circle1LastPosition);
	Vector2x relativeVelocity       = Vector2x_subtract(circle2Vector, circle1Vector);
	fixed16_16 a = Vector2x_dot(relativeVelocity, relativeVelocity);
	fixed16_16 b = Vector2x_dot(relativeVelocity, circle1ToCircle2Vector) * 2;
	fixed16_16 c = Vector2x_dot(circle1ToCircle2Vector, circle1ToCircle2Vector) - radiusSumSquare;
	fixed16_16 time1 = -1, time2 = -1;
	
	// Very small values of relativeVelocity will evaluate to a = 0 && b != 0, which throws off the calculation
	if (a == 0) {
		return false;
	}
	if (quadraticFormula(a, b, c, &time1, &time2) && time1 != time2) {
		if (time2 < time1) {
			time1 = time2;
		}
		if (time1 < 0x00000) {
			fixed16_16 temporalEpsilon = xdiv(SPATIAL_EPSILON, xsqrt(a));
			if (time1 >= -temporalEpsilon) {
				time1 = 0x00000;
			}
		}
		if (time1 >= 0x00000 && time1 <= 0x10000) {
			Vector2x position1, position2;
			Vector3x normal;
			
			position1 = Vector2x_interpolate(circle1LastPosition, circle1Position, time1);
			position2 = Vector2x_interpolate(circle2LastPosition, circle2Position, time1);
			normal.x = position1.x - position2.x;
			normal.y = position1.y - position2.y;
			normal.z = 0x00000;
			Vector3x_normalize(&normal);
			*outNormal = normal;
			*outTime = time1;
			return true;
		}
	}
	
	return false;
}

bool intersectionHandler_rect2D_rect2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	fixed16_16 time, bestTime = FIXED_16_16_MAX;
	Vector3x bestNormal = VECTOR3x_ZERO;
	CollisionRect2D * rect1 = (CollisionRect2D *) object1, * rect2 = (CollisionRect2D *) object2;
	
	// rect1 right vs. rect2 left
	if (rect1->solidRight && rect2->solidLeft &&
	    (rect1->position.x + rect1->size.x) - (rect1->lastPosition.x + rect1->lastSize.x) > rect2->position.x - rect2->lastPosition.x && 
	    intersectSweptLineSegments(rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x + rect1->size.x,
	                               rect2->lastPosition.x, rect2->position.x,
	                               rect1->lastPosition.y, rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y, rect1->position.y + rect1->size.y,
	                               rect2->lastPosition.y, rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y, rect2->position.y + rect2->size.y,
	                               &time)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_LEFT;
		}
	}
	
	// rect1 left vs. rect2 right
	if (rect1->solidLeft && rect2->solidRight &&
	    rect1->position.x - rect1->lastPosition.x < (rect2->position.x + rect2->size.x) - (rect2->lastPosition.x + rect2->lastSize.x) && 
	    intersectSweptLineSegments(rect1->lastPosition.x, rect1->position.x,
	                               rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x + rect2->size.x,
	                               rect1->lastPosition.y, rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y, rect1->position.y + rect1->size.y,
	                               rect2->lastPosition.y, rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y, rect2->position.y + rect2->size.y,
	                               &time)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_RIGHT;
		}
	}
	
	// rect1 top vs. rect2 bottom
	if (rect1->solidTop && rect2->solidBottom &&
	    (rect1->position.y + rect1->size.y) - (rect1->lastPosition.y + rect1->lastSize.y) > rect2->position.y - rect2->lastPosition.y && 
	    intersectSweptLineSegments(rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y + rect1->size.y,
	                               rect2->lastPosition.y, rect2->position.y,
	                               rect1->lastPosition.x, rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x, rect1->position.x + rect1->size.x,
	                               rect2->lastPosition.x, rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x, rect2->position.x + rect2->size.x,
	                               &time)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_DOWN;
		}
	}
	
	// rect1 bottom vs. rect2 top
	if (rect1->solidBottom && rect2->solidTop &&
	    rect1->position.y - rect1->lastPosition.y < (rect2->position.y + rect2->size.y) - (rect2->lastPosition.y + rect2->lastSize.y) && 
	    intersectSweptLineSegments(rect1->lastPosition.y, rect1->position.y,
	                               rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y + rect2->size.y,
	                               rect1->lastPosition.x, rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x, rect1->position.x + rect1->size.x,
	                               rect2->lastPosition.x, rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x, rect2->position.x + rect2->size.x,
	                               &time)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_UP;
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		if (outNormal != NULL) {
			*outNormal = bestNormal;
		}
		if (outTime != NULL) {
			*outTime = bestTime;
		}
		return true;
	}
	
	// rect1 top right corner vs. rect2 bottom left corner
	if (rect1->solidRight && rect1->solidTop && rect2->solidLeft && rect2->solidBottom &&
	    (rect1->position.x + rect1->size.x) - (rect1->lastPosition.x + rect1->lastSize.x) > rect2->position.x - rect2->lastPosition.x && 
	    (rect1->position.y + rect1->size.y) - (rect1->lastPosition.y + rect1->lastSize.y) > rect2->position.y - rect2->lastPosition.y && 
	    intersectSweptPoints(rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x + rect1->size.x,
	                         rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y + rect1->size.y,
	                         rect2->lastPosition.x, rect2->position.x,
	                         rect2->lastPosition.y, rect2->position.y,
	                         &time)) {
		if (outTime != NULL) {
			*outTime = time;
		}
		if (outNormal != NULL) {
			*outNormal = VECTOR3x_LEFT;
		}
		return true;
	}
	
	// rect1 bottom right corner vs. rect2 top left corner
	if (rect1->solidRight && rect1->solidBottom && rect2->solidLeft && rect2->solidTop &&
	    (rect1->position.x + rect1->size.x) - (rect1->lastPosition.x + rect1->lastSize.x) > rect2->position.x - rect2->lastPosition.x && 
	    rect1->position.y - rect1->lastPosition.y < (rect2->position.y + rect2->size.y) - (rect2->lastPosition.y + rect2->lastSize.y) && 
	    intersectSweptPoints(rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x + rect1->size.x,
	                         rect1->lastPosition.y, rect1->position.y,
	                         rect2->lastPosition.x, rect2->position.x,
	                         rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y + rect2->size.y,
	                         &time)) {
		if (outTime != NULL) {
			*outTime = time;
		}
		if (outNormal != NULL) {
			*outNormal = VECTOR3x_LEFT;
		}
		return true;
	}
	
	// rect1 top left corner vs. rect2 bottom right corner
	if (rect1->solidLeft && rect1->solidTop && rect2->solidRight && rect2->solidBottom &&
	    rect1->position.x - rect1->lastPosition.x < (rect2->position.x + rect2->size.x) - (rect2->lastPosition.x + rect2->lastSize.x) && 
	    (rect1->position.y + rect1->size.y) - (rect1->lastPosition.y + rect1->lastSize.y) > rect2->position.y - rect2->lastPosition.y && 
	    intersectSweptPoints(rect1->lastPosition.x, rect1->position.x,
	                         rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y + rect1->size.y,
	                         rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x + rect2->size.x,
	                         rect2->lastPosition.y, rect2->position.y,
	                         &time)) {
		if (outTime != NULL) {
			*outTime = time;
		}
		if (outNormal != NULL) {
			*outNormal = VECTOR3x_RIGHT;
		}
		return true;
	}
	
	// rect1 bottom left corner vs. rect2 top right corner
	if (rect1->solidLeft && rect1->solidBottom && rect2->solidRight && rect2->solidTop &&
	    rect1->position.x - rect1->lastPosition.x < (rect2->position.x + rect2->size.x) - (rect2->lastPosition.x + rect2->lastSize.x) && 
	    rect1->position.y - rect1->lastPosition.y < (rect2->position.y + rect2->size.y) - (rect2->lastPosition.y + rect2->lastSize.y) && 
	    intersectSweptPoints(rect1->lastPosition.x, rect1->position.x,
	                         rect1->lastPosition.y, rect1->position.y,
	                         rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x + rect2->size.x,
	                         rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y + rect2->size.y,
	                         &time)) {
		if (outTime != NULL) {
			*outTime = time;
		}
		if (outNormal != NULL) {
			*outNormal = VECTOR3x_RIGHT;
		}
		return true;
	}
	
	return false;
}

bool intersectionHandler_rect2D_circle(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	CollisionRect2D * rect = (CollisionRect2D *) object1;
	CollisionCircle * circle = (CollisionCircle *) object2;
	fixed16_16 time, bestTime = FIXED_16_16_MAX;
	Vector3x normal, bestNormal = VECTOR3x_ZERO;
	
	// rect right vs. circle left
	if (rect->solidRight &&
	    (rect->position.x + rect->size.x) - (rect->lastPosition.x + rect->lastSize.x) > circle->position.x - circle->lastPosition.x && 
	    intersectSweptLineSegments(rect->lastPosition.x + rect->lastSize.x, rect->position.x + rect->size.x,
	                               circle->lastPosition.x - circle->radius, circle->position.x - circle->radius,
	                               rect->lastPosition.y, rect->lastPosition.y + rect->lastSize.y, rect->position.y, rect->position.y + rect->size.y,
	                               circle->lastPosition.y, circle->lastPosition.y, circle->position.y, circle->position.y,
	                               &time)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_LEFT;
		}
	}
	
	// rect left vs. circle right
	if (rect->solidLeft &&
	    rect->position.x - rect->lastPosition.x < circle->position.x - circle->lastPosition.x && 
	    intersectSweptLineSegments(rect->lastPosition.x, rect->position.x,
	                               circle->lastPosition.x + circle->radius, circle->position.x + circle->radius,
	                               rect->lastPosition.y, rect->lastPosition.y + rect->lastSize.y, rect->position.y, rect->position.y + rect->size.y,
	                               circle->lastPosition.y, circle->lastPosition.y, circle->position.y, circle->position.y,
	                               &time)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_RIGHT;
		}
	}
	
	// rect top vs. circle bottom
	if (rect->solidTop &&
	    (rect->position.y + rect->size.y) - (rect->lastPosition.y + rect->lastSize.y) > circle->position.y - circle->lastPosition.y && 
	    intersectSweptLineSegments(rect->lastPosition.y + rect->lastSize.y, rect->position.y + rect->size.y,
	                               circle->lastPosition.y - circle->radius, circle->position.y - circle->radius,
	                               rect->lastPosition.x, rect->lastPosition.x + rect->lastSize.x, rect->position.x, rect->position.x + rect->size.x,
	                               circle->lastPosition.x, circle->lastPosition.x, circle->position.x, circle->position.x,
	                               &time)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_DOWN;
		}
	}
	
	// rect bottom vs. circle top
	if (rect->solidBottom &&
	    rect->position.y - rect->lastPosition.y < circle->position.y - circle->lastPosition.y && 
	    intersectSweptLineSegments(rect->lastPosition.y, rect->position.y,
	                               circle->lastPosition.y + circle->radius, circle->position.y + circle->radius,
	                               rect->lastPosition.x, rect->lastPosition.x + rect->lastSize.x, rect->position.x, rect->position.x + rect->size.x,
	                               circle->lastPosition.x, circle->lastPosition.x, circle->position.x, circle->position.x,
	                               &time)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_UP;
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		if (outNormal != NULL) {
			*outNormal = bestNormal;
		}
		if (outTime != NULL) {
			*outTime = bestTime;
		}
		return true;
	}
	
	// rect bottom left corner vs. circle
	if ((rect->solidLeft || rect->solidBottom) &&
	    intersectSweptCircles(rect->lastPosition, rect->position, 0x00000,
	                          circle->lastPosition, circle->position, circle->radius,
	                          &time, &normal)) {
		if (normal.x >= 0 && normal.y >= 0 && time < bestTime) {
			bestTime = time;
			bestNormal = normal;
		}
	}
	
	// rect bottom right corner vs. circle
	if ((rect->solidRight || rect->solidBottom) &&
	    intersectSweptCircles(VECTOR2x(rect->lastPosition.x + rect->lastSize.x, rect->lastPosition.y), VECTOR2x(rect->position.x + rect->size.x, rect->position.y), 0x00000,
	                          circle->lastPosition, circle->position, circle->radius,
	                          &time, &normal)) {
		if (normal.x <= 0 && normal.y >= 0 && time < bestTime) {
			bestTime = time;
			bestNormal = normal;
		}
	}
	
	// rect top left corner vs. circle
	if ((rect->solidLeft || rect->solidTop) &&
	    intersectSweptCircles(VECTOR2x(rect->lastPosition.x, rect->lastPosition.y + rect->lastSize.y), VECTOR2x(rect->position.x, rect->position.y + rect->size.y), 0x00000,
	                          circle->lastPosition, circle->position, circle->radius,
	                          &time, &normal)) {
		if (normal.x >= 0 && normal.y <= 0 && time < bestTime) {
			bestTime = time;
			bestNormal = normal;
		}
	}
	
	// rect top right corner vs. circle
	if ((rect->solidRight || rect->solidTop) &&
	    intersectSweptCircles(VECTOR2x(rect->lastPosition.x + rect->lastSize.x, rect->lastPosition.y + rect->lastSize.y), VECTOR2x(rect->position.x + rect->size.x, rect->position.y + rect->size.y), 0x00000,
	                          circle->lastPosition, circle->position, circle->radius,
	                          &time, &normal)) {
		if (normal.x <= 0 && normal.y <= 0 && time < bestTime) {
			bestTime = time;
			bestNormal = normal;
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		if (outNormal != NULL) {
			*outNormal = bestNormal;
		}
		if (outTime != NULL) {
			*outTime = bestTime;
		}
		return true;
	}
	
	return false;
}

bool intersectionHandler_rect2D_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_rect2D_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_circle_circle(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	CollisionCircle * circle1 = (CollisionCircle *) object1, * circle2 = (CollisionCircle *) object2;
	fixed16_16 time;
	Vector3x normal;
	
	if (intersectSweptCircles(circle1->lastPosition, circle1->position, circle1->radius,
	                          circle2->lastPosition, circle2->position, circle2->radius,
	                          &time, &normal)) {
		if (outNormal != NULL) {
			*outNormal = normal;
		}
		if (outTime != NULL) {
			*outTime = time;
		}
		return true;
	}
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

bool intersectionHandler_box3D_box3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box3D_sphere(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box3D_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box3D_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_box3D_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_sphere_sphere(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_sphere_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
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

bool intersectionHandler_line3D_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

bool intersectionHandler_line3D_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
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
