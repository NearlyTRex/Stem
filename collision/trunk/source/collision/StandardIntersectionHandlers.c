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

#include "collision/CollisionBox3D.h"
#include "collision/CollisionCapsule.h"
#include "collision/CollisionCircle.h"
#include "collision/CollisionRect2D.h"
#include "collision/CollisionSphere.h"
#include "collision/CollisionStaticTrimesh.h"
#include "collision/StandardIntersectionHandlers.h"
#include "gamemath/Barycentric.h"
#include "gamemath/VectorConversions.h"

#define SPATIAL_EPSILON 0x00008

static bool intersectSweptLineSegments(fixed16_16 x1Start, fixed16_16 x1End, fixed16_16 thickness1,
                                       fixed16_16 x2Start, fixed16_16 x2End, fixed16_16 thickness2,
                                       fixed16_16 bottom1Start, fixed16_16 top1Start, fixed16_16 bottom1End, fixed16_16 top1End,
                                       fixed16_16 bottom2Start, fixed16_16 top2Start, fixed16_16 bottom2End, fixed16_16 top2End,
                                       fixed16_16 * outTime, fixed16_16 * outContactArea) {
	fixed16_16 time;
	fixed16_16 bottom1, top1, bottom2, top2;
	
	time = xdiv(x1Start - x2Start, x2End - x2Start - x1End + x1Start);
	if (time > 0x10000) {
		return false;
	}
	if (time < 0x00000) {
		if ((x1End - x2End > x1Start - x2Start && (x1Start - thickness1 < x2Start + thickness2)) ||
		    (x1End - x2End < x1Start - x2Start && (x1Start + thickness1 > x2Start - thickness2))) {
			time = 0x00000;
		} else {
			return false;
		}
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
		*outContactArea = (top1 < top2 ? top1 : top2) - (bottom1 > bottom2 ? bottom1 : bottom2);
		return true;
	}
	return false;
}

static bool intersectSweptQuads(fixed16_16 x1Start, fixed16_16 x1End, fixed16_16 thickness1,
                                fixed16_16 x2Start, fixed16_16 x2End, fixed16_16 thickness2,
                                fixed16_16 bottom1Start, fixed16_16 top1Start, fixed16_16 bottom1End, fixed16_16 top1End,
                                fixed16_16 bottom2Start, fixed16_16 top2Start, fixed16_16 bottom2End, fixed16_16 top2End,
                                fixed16_16 back1Start, fixed16_16 front1Start, fixed16_16 back1End, fixed16_16 front1End,
                                fixed16_16 back2Start, fixed16_16 front2Start, fixed16_16 back2End, fixed16_16 front2End,
                                fixed16_16 * outTime, fixed16_16 * outContactArea) {
	fixed16_16 time;
	fixed16_16 bottom1, top1, bottom2, top2, back1, front1, back2, front2;
	
	time = xdiv(x1Start - x2Start, x2End - x2Start - x1End + x1Start);
	if (time > 0x10000) {
		return false;
	}
	if (time < 0x00000) {
		if ((x1End - x2End > x1Start - x2Start && (x1Start - thickness1 < x2Start + thickness2)) ||
		    (x1End - x2End < x1Start - x2Start && (x1Start + thickness1 > x2Start - thickness2))) {
			time = 0x00000;
		} else {
			return false;
		}
	}
	
	bottom1 = bottom1Start + xmul(bottom1End - bottom1Start, time);
	top1    = top1Start    + xmul(top1End    - top1Start,    time);
	bottom2 = bottom2Start + xmul(bottom2End - bottom2Start, time);
	top2    = top2Start    + xmul(top2End    - top2Start,    time);
	back1   = back1Start   + xmul(back1End   - back1Start,   time);
	front1  = front1Start  + xmul(front1End  - front1Start,  time);
	back2   = back2Start   + xmul(back2End   - back2Start,   time);
	front2  = front2Start  + xmul(front2End  - front2Start,  time);
	
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
	if (back1 > front1) {
		fixed16_16 swap = front1;
		front1 = back1;
		back1 = swap;
	}
	if (back2 > front2) {
		fixed16_16 swap = front2;
		front2 = back2;
		back2 = swap;
	}
	
	if (top1 > bottom2 && bottom1 < top2 && front1 > back2 && back1 < front2) {
		*outTime = time;
		*outContactArea = xmul((top1 < top2 ? top1 : top2) - (bottom1 > bottom2 ? bottom1 : bottom2), (front1 < front2 ? front1 : front2) - (back1 > back2 ? back1 : back2));
		return true;
	}
	return false;
}

static bool intersectSweptPoints2D(fixed16_16 x1Start, fixed16_16 x1End,
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
	Vector2x circle1Vector          = Vector2x_subtract(circle1Position, circle1LastPosition);
	Vector2x circle2Vector          = Vector2x_subtract(circle2Position, circle2LastPosition);
	Vector2x circle1ToCircle2Vector = Vector2x_subtract(circle2LastPosition, circle1LastPosition);
	Vector2x relativeVelocity       = Vector2x_subtract(circle2Vector, circle1Vector);
	fixed16_16 b = Vector2x_dot(relativeVelocity, circle1ToCircle2Vector) * 2;
	
	if (lastDistanceSquare < radiusSumSquare && b < 0x00000) {
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
	
	fixed16_16 a = Vector2x_dot(relativeVelocity, relativeVelocity);
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

// Adapted from http://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=2
static bool intersectSweptSpheres(Vector3x sphere1LastPosition, Vector3x sphere1Position, fixed16_16 sphere1Radius,
                                  Vector3x sphere2LastPosition, Vector3x sphere2Position, fixed16_16 sphere2Radius,
                                  fixed16_16 * outTime, Vector3x * outNormal) {
	fixed16_16 radiusSum = sphere1Radius + sphere2Radius;
	fixed16_16 radiusSumSquare = xmul(radiusSum, radiusSum);
	fixed16_16 lastDistanceSquare = Vector3x_distanceSquared(sphere1LastPosition, sphere2LastPosition);
	Vector3x sphere1Vector          = Vector3x_subtract(sphere1Position, sphere1LastPosition);
	Vector3x sphere2Vector          = Vector3x_subtract(sphere2Position, sphere2LastPosition);
	Vector3x sphere1ToSphere2Vector = Vector3x_subtract(sphere2LastPosition, sphere1LastPosition);
	Vector3x relativeVelocity       = Vector3x_subtract(sphere2Vector, sphere1Vector);
	fixed16_16 b = Vector3x_dot(relativeVelocity, sphere1ToSphere2Vector) * 2;
	
	if (lastDistanceSquare < radiusSumSquare && b < 0x00000) {
		// If already penetrating and trying to penetrate deeper, put a stop to it early
		Vector3x normal;
		
		normal.x = sphere1LastPosition.x - sphere2LastPosition.x;
		normal.y = sphere1LastPosition.y - sphere2LastPosition.y;
		normal.z = 0x00000;
		Vector3x_normalize(&normal);
		*outNormal = normal;
		*outTime = 0x00000;
		return true;
	}
	
	fixed16_16 a = Vector3x_dot(relativeVelocity, relativeVelocity);
	fixed16_16 c = Vector3x_dot(sphere1ToSphere2Vector, sphere1ToSphere2Vector) - radiusSumSquare;
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
			Vector3x position1, position2;
			Vector3x normal;
			
			position1 = Vector3x_interpolate(sphere1LastPosition, sphere1Position, time1);
			position2 = Vector3x_interpolate(sphere2LastPosition, sphere2Position, time1);
			normal.x = position1.x - position2.x;
			normal.y = position1.y - position2.y;
			normal.z = position1.z - position2.z;
			Vector3x_normalize(&normal);
			*outNormal = normal;
			*outTime = time1;
			return true;
		}
	}
	
	return false;
}

static bool intersectSweptCylinderWalls(Vector3x cylinder1LastPosition, Vector3x cylinder1Position, fixed16_16 cylinder1Radius, fixed16_16 cylinder1Height,
                                        Vector3x cylinder2LastPosition, Vector3x cylinder2Position, fixed16_16 cylinder2Radius, fixed16_16 cylinder2Height,
                                        fixed16_16 * outTime, Vector3x * outNormal, fixed16_16 * outContactArea) {
	fixed16_16 time;
	Vector3x normal;
	
	if (intersectSweptCircles(VECTOR2x(cylinder1LastPosition.x, cylinder1LastPosition.z), VECTOR2x(cylinder1Position.x, cylinder1Position.z), cylinder1Radius,
	                          VECTOR2x(cylinder2LastPosition.x, cylinder2LastPosition.z), VECTOR2x(cylinder2Position.x, cylinder2Position.z), cylinder2Radius,
	                          &time, &normal)) {
		fixed16_16 bottom1, top1, bottom2, top2;
		
		bottom1 = cylinder1LastPosition.y + xmul(cylinder1Position.y - cylinder1LastPosition.y, time);
		top1 = bottom1 + cylinder1Height;
		bottom2 = cylinder2LastPosition.y + xmul(cylinder2Position.y - cylinder2LastPosition.y, time);
		top2 = bottom2 + cylinder2Height;
		
		if (top1 > bottom2 && bottom1 < top2) {
			*outTime = time;
			*outNormal = VECTOR3x(normal.x, 0x00000, normal.y);
			*outContactArea = (top1 < top2 ? top1 : top2) - (bottom1 > bottom2 ? bottom1 : bottom2);
			return true;
		}
	}
	return false;
}

static bool intersectSweptExtrudedPoints(Vector3x lastPoint1, Vector3x point1, fixed16_16 lastExtrudeY1, fixed16_16 extrudeY1,
                                         Vector3x lastPoint2, Vector3x point2, fixed16_16 lastExtrudeY2, fixed16_16 extrudeY2,
                                         fixed16_16 * outTime) {
	fixed16_16 time;
	
	if (intersectSweptPoints2D(lastPoint1.x, point1.x,
	                           lastPoint1.z, point1.z,
	                           lastPoint2.x, point2.x,
	                           lastPoint2.z, point2.z,
	                           &time)) {
		fixed16_16 bottom1, top1, bottom2, top2;
		
		bottom1 = lastPoint1.y + xmul(point1.y - lastPoint1.y, time);
		top1 = lastPoint1.y + lastExtrudeY1 + xmul((point1.y + extrudeY1) - (lastPoint1.y + lastExtrudeY1), time);
		bottom2 = lastPoint2.y + xmul(point2.y - lastPoint2.y, time);
		top2 = lastPoint2.y + lastExtrudeY2 + xmul((point2.y + extrudeY2) - (lastPoint2.y + lastExtrudeY2), time);
		
		if (top1 > bottom2 && bottom1 < top2) {
			*outTime = time;
			return true;
		}
	}
	return false;
}

static bool intersectSweptPoints3D(fixed16_16 x1Start, fixed16_16 x1End,
                                   fixed16_16 y1Start, fixed16_16 y1End,
                                   fixed16_16 z1Start, fixed16_16 z1End,
                                   fixed16_16 x2Start, fixed16_16 x2End,
                                   fixed16_16 y2Start, fixed16_16 y2End,
                                   fixed16_16 z2Start, fixed16_16 z2End,
                                   fixed16_16 * outTime) {
	fixed16_16 time1, time2, time3;
	
	time1 = xdiv(x1Start - x2Start, x2End - x2Start - x1End + x1Start);
	time2 = xdiv(y1Start - y2Start, y2End - y2Start - y1End + y1Start);
	time3 = xdiv(z1Start - z2Start, z2End - z2Start - z1End + z1Start);
	if (time1 > 0x10000 || time1 < 0x00000 || time2 > 0x10000 || time2 < 0x00000 || time3 > 0x10000 || time3 < 0x00000 || time1 != time2 || time1 != time3) {
		return false;
	}
	*outTime = time1;
	return true;
}

static bool intersectSweptSphereTrimeshEdge(Vector3x lastPosition, Vector3x position, fixed16_16 radius, Vector3x endpoint1, Vector3x endpoint2, Vector3x triangleNormal0, Vector3x triangleNormal1, Quaternionx planarTransform, fixed16_16 length, fixed16_16 * outTime, Vector3x * outNormal) {
	Vector3x normal, objectVector;
	fixed16_16 time;
	
	objectVector = Vector3x_subtract(position, lastPosition);
	if (Vector3x_dot(objectVector, triangleNormal0) >= 0x00000 && Vector3x_dot(objectVector, triangleNormal1) >= 0x00000) {
		return false;
	}
	
	lastPosition = Quaternionx_multiplyVector3x(planarTransform, Vector3x_subtract(lastPosition, endpoint2));
	position = Quaternionx_multiplyVector3x(planarTransform, Vector3x_subtract(position, endpoint2));
	
	if (intersectSweptCircles(Vector3x_xy(lastPosition), Vector3x_xy(position), radius,
	                          VECTOR2x_ZERO, VECTOR2x_ZERO, 0x00000,
	                          &time, &normal)) {
		Vector3x intersectingPosition = Vector3x_interpolate(lastPosition, position, time);
		if (intersectingPosition.z >= 0x00000 && intersectingPosition.z <= length) {
			*outTime = time;
			*outNormal = Quaternionx_multiplyVector3x(Quaternionx_inverted(planarTransform), normal);
			return true;
		}
	}
	
	return false;
}

static bool intersectSweptSphereTriangle(Vector3x lastPosition, Vector3x position, fixed16_16 radius, Vector3x vertex1, Vector3x vertex2, Vector3x vertex3, Vector3x normal, fixed16_16 * outTime) {
	fixed16_16 distance1, distance2;
	fixed16_16 time;
	Vector3x collidingPosition, barycenter;
	
	distance1 = Vector3x_dot(Vector3x_subtract(lastPosition, vertex1), normal);
	distance2 = Vector3x_dot(Vector3x_subtract(position, vertex1), normal);
	if (distance1 < 0x00000 || distance1 <= distance2) {
		return false;
	}
	
	time = xdiv(distance1 - radius, -distance2 + distance1);
	if (time > 0x10000) {
		return false;
	}
	if (time < 0x00000) {
		if (distance1 > radius) {
			return false;
		}
		time = 0x00000;
	}
	
	collidingPosition = Vector3x_interpolate(lastPosition, position, time);
	barycenter = cartesianToBarycentric3x(collidingPosition, vertex1, vertex2, vertex3);
	if (barycenter.x < 0 || barycenter.y < 0 || barycenter.z < 0) {
		return false;
	}
	
	*outTime = time;
	return true;
}

#define PLANAR_LENGTH_MIN 0x00010

static bool intersectSweptCylinderWallTrimeshEdge(Vector3x lastPosition, Vector3x position, fixed16_16 radius, fixed16_16 height, Vector3x endpoint1, Vector3x endpoint2, Vector3x triangleNormal0, Vector3x triangleNormal1, fixed16_16 length, fixed16_16 * outTime, Vector3x * outNormal) {
	Vector3x planarEdgeVector, planarEdgeNormal;
	fixed16_16 planarLength;
	fixed16_16 distance1, distance2, distance3;
	fixed16_16 time;
	Vector3x intersectingCylinderPosition, intersectingEdgePosition;
	
	if (triangleNormal0.y == 0x00000 || triangleNormal1.y == 0x00000 ||
	    (triangleNormal0.y > 0x0000) == (triangleNormal1.y > 0x00000)) {
		return false;
	}
	
	planarEdgeVector = Vector3x_subtract(endpoint2, endpoint1);
	planarEdgeVector.y = 0x00000;
	planarLength = Vector3x_magnitude(planarEdgeVector);
	if (planarLength < PLANAR_LENGTH_MIN) {
		return false;
	}
	planarEdgeVector = Vector3x_divideScalar(planarEdgeVector, planarLength);
	planarEdgeNormal = Vector3x_cross(VECTOR3x_UP, planarEdgeVector);
	
	distance1 = Vector3x_dot(planarEdgeNormal, Vector3x_subtract(lastPosition, endpoint1));
	distance2 = Vector3x_dot(planarEdgeNormal, Vector3x_subtract(position, endpoint1));
	if (distance1 < 0x00000 || distance1 <= distance2) {
		return false;
	}
	
	time = xdiv(distance1 - radius, -distance2 + distance1);
	if (time > 0x10000) {
		return false;
	}
	if (time < 0x00000) {
		if (distance1 > radius) {
			return false;
		}
		time = 0x00000;
	}
	
	intersectingCylinderPosition = Vector3x_interpolate(lastPosition, position, time);
	distance3 = Vector3x_dot(Vector3x_subtract(intersectingCylinderPosition, endpoint1), planarEdgeVector);
	if (distance3 < 0x00000 || distance3 > planarLength) {
		return false;
	}
	
	intersectingEdgePosition = Vector3x_interpolate(endpoint1, endpoint2, xdiv(distance3, planarLength));
	if (intersectingEdgePosition.y < intersectingCylinderPosition.y || intersectingEdgePosition.y > intersectingCylinderPosition.y + height) {
		return false;
	}
	
	*outTime = time;
	*outNormal = planarEdgeNormal;
	return true;
}

static fixed16_16 getRectEdgeThicknessX(CollisionRect2D * rect) {
	if (rect->edgeThickness == EDGE_THICKNESS_DEFAULT) {
		return rect->size.x / 2;
	}
	return rect->edgeThickness;
}

static fixed16_16 getRectEdgeThicknessY(CollisionRect2D * rect) {
	if (rect->edgeThickness == EDGE_THICKNESS_DEFAULT) {
		return rect->size.y / 2;
	}
	return rect->edgeThickness;
}

static fixed16_16 getBoxEdgeThicknessX(CollisionBox3D * box) {
	if (box->edgeThickness == EDGE_THICKNESS_DEFAULT) {
		return box->size.x / 2;
	}
	return box->edgeThickness;
}

static fixed16_16 getBoxEdgeThicknessY(CollisionBox3D * box) {
	if (box->edgeThickness == EDGE_THICKNESS_DEFAULT) {
		return box->size.y / 2;
	}
	return box->edgeThickness;
}

static fixed16_16 getBoxEdgeThicknessZ(CollisionBox3D * box) {
	if (box->edgeThickness == EDGE_THICKNESS_DEFAULT) {
		return box->size.z / 2;
	}
	return box->edgeThickness;
}

#define returnIntersection(time, normal, object1Vector, object2Vector, contactArea) \
	if (outTime != NULL) { \
		*outTime = time; \
	} \
	if (outNormal != NULL) { \
		*outNormal = normal; \
	} \
	if (outObject1Vector != NULL) { \
		*outObject1Vector = object1Vector; \
	} \
	if (outObject2Vector != NULL) { \
		*outObject2Vector = object2Vector; \
	} \
	if (outContactArea != NULL) { \
		*outContactArea = contactArea; \
	} \
	return true

bool intersectionHandler_rect2D_rect2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	fixed16_16 time, bestTime = FIXED_16_16_MAX;
	fixed16_16 contactArea, bestContactArea = 0x00000;
	Vector3x bestNormal = VECTOR3x_ZERO, bestObject1Vector = VECTOR3x_ZERO, bestObject2Vector = VECTOR3x_ZERO;
	CollisionRect2D * rect1 = (CollisionRect2D *) object1, * rect2 = (CollisionRect2D *) object2;
	
	// rect1 right vs. rect2 left
	if (rect1->solidRight && rect2->solidLeft &&
	    (rect1->position.x + rect1->size.x) - (rect1->lastPosition.x + rect1->lastSize.x) > rect2->position.x - rect2->lastPosition.x &&
	    intersectSweptLineSegments(rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x + rect1->size.x, getRectEdgeThicknessX(rect1),
	                               rect2->lastPosition.x, rect2->position.x, getRectEdgeThicknessX(rect2),
	                               rect1->lastPosition.y, rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y, rect1->position.y + rect1->size.y,
	                               rect2->lastPosition.y, rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y, rect2->position.y + rect2->size.y,
	                               &time, &contactArea)) {
		bestTime = time;
		bestNormal = VECTOR3x_LEFT;
		bestObject1Vector = VECTOR3x((rect1->position.x + rect1->size.x) - (rect1->lastPosition.x + rect1->lastSize.x), rect1->position.y - rect1->lastPosition.y, 0x00000);
		bestObject2Vector = VECTOR3x(rect2->position.x - rect2->lastPosition.x, rect2->position.y - rect2->lastPosition.y, 0x00000);
		bestContactArea = contactArea;
	}
	
	// rect1 left vs. rect2 right
	if (rect1->solidLeft && rect2->solidRight &&
	    rect1->position.x - rect1->lastPosition.x < (rect2->position.x + rect2->size.x) - (rect2->lastPosition.x + rect2->lastSize.x) &&
	    intersectSweptLineSegments(rect1->lastPosition.x, rect1->position.x, getRectEdgeThicknessX(rect1),
	                               rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x + rect2->size.x, getRectEdgeThicknessX(rect2),
	                               rect1->lastPosition.y, rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y, rect1->position.y + rect1->size.y,
	                               rect2->lastPosition.y, rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y, rect2->position.y + rect2->size.y,
	                               &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_RIGHT;
			bestObject1Vector = VECTOR3x(rect1->position.x - rect1->lastPosition.x, rect1->position.y - rect1->lastPosition.y, 0x00000);
			bestObject2Vector = VECTOR3x((rect2->position.x + rect2->size.x) - (rect2->lastPosition.x + rect2->lastSize.x), rect2->position.y - rect2->lastPosition.y, 0x00000);
			bestContactArea = contactArea;
		}
	}
	
	// rect1 top vs. rect2 bottom
	if (rect1->solidTop && rect2->solidBottom &&
	    (rect1->position.y + rect1->size.y) - (rect1->lastPosition.y + rect1->lastSize.y) > rect2->position.y - rect2->lastPosition.y &&
	    intersectSweptLineSegments(rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y + rect1->size.y, getRectEdgeThicknessY(rect1),
	                               rect2->lastPosition.y, rect2->position.y, getRectEdgeThicknessY(rect2),
	                               rect1->lastPosition.x, rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x, rect1->position.x + rect1->size.x,
	                               rect2->lastPosition.x, rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x, rect2->position.x + rect2->size.x,
	                               &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_DOWN;
			bestObject1Vector = VECTOR3x(rect1->position.x - rect1->lastPosition.x, (rect1->position.y + rect1->size.y) - (rect1->lastPosition.y + rect1->lastSize.y), 0x00000);
			bestObject2Vector = VECTOR3x(rect2->position.x - rect2->lastPosition.x, rect2->position.y - rect2->lastPosition.y, 0x00000);
			bestContactArea = contactArea;
		}
	}
	
	// rect1 bottom vs. rect2 top
	if (rect1->solidBottom && rect2->solidTop &&
	    rect1->position.y - rect1->lastPosition.y < (rect2->position.y + rect2->size.y) - (rect2->lastPosition.y + rect2->lastSize.y) &&
	    intersectSweptLineSegments(rect1->lastPosition.y, rect1->position.y, getRectEdgeThicknessY(rect1),
	                               rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y + rect2->size.y, getRectEdgeThicknessY(rect2),
	                               rect1->lastPosition.x, rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x, rect1->position.x + rect1->size.x,
	                               rect2->lastPosition.x, rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x, rect2->position.x + rect2->size.x,
	                               &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_UP;
			bestObject1Vector = VECTOR3x(rect1->position.x - rect1->lastPosition.x, rect1->position.y - rect1->lastPosition.y, 0x00000);
			bestObject2Vector = VECTOR3x(rect2->position.x - rect2->lastPosition.x, (rect2->position.y + rect2->size.y) - (rect2->lastPosition.y + rect2->lastSize.y), 0x00000);
			bestContactArea = contactArea;
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		returnIntersection(bestTime, bestNormal, bestObject1Vector, bestObject2Vector, bestContactArea);
	}
	
	// rect1 top right corner vs. rect2 bottom left corner
	if (rect1->solidRight && rect1->solidTop && rect2->solidLeft && rect2->solidBottom &&
	    (rect1->position.x + rect1->size.x) - (rect1->lastPosition.x + rect1->lastSize.x) > rect2->position.x - rect2->lastPosition.x &&
	    (rect1->position.y + rect1->size.y) - (rect1->lastPosition.y + rect1->lastSize.y) > rect2->position.y - rect2->lastPosition.y &&
	    intersectSweptPoints2D(rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x + rect1->size.x,
	                           rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y + rect1->size.y,
	                           rect2->lastPosition.x, rect2->position.x,
	                           rect2->lastPosition.y, rect2->position.y,
	                           &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((rect1->position.x + rect1->size.x) - (rect1->lastPosition.x + rect1->lastSize.x), (rect1->position.y + rect1->size.y) - (rect1->lastPosition.y + rect1->lastSize.y), 0x00000), VECTOR3x(rect2->position.x - rect2->lastPosition.x, rect2->position.y - rect2->lastPosition.y, 0x00000), 0x00000);
	}
	
	// rect1 bottom right corner vs. rect2 top left corner
	if (rect1->solidRight && rect1->solidBottom && rect2->solidLeft && rect2->solidTop &&
	    (rect1->position.x + rect1->size.x) - (rect1->lastPosition.x + rect1->lastSize.x) > rect2->position.x - rect2->lastPosition.x &&
	    rect1->position.y - rect1->lastPosition.y < (rect2->position.y + rect2->size.y) - (rect2->lastPosition.y + rect2->lastSize.y) &&
	    intersectSweptPoints2D(rect1->lastPosition.x + rect1->lastSize.x, rect1->position.x + rect1->size.x,
	                           rect1->lastPosition.y, rect1->position.y,
	                           rect2->lastPosition.x, rect2->position.x,
	                           rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y + rect2->size.y,
	                           &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((rect1->position.x + rect1->size.x) - (rect1->lastPosition.x + rect1->lastSize.x), rect1->position.y - rect1->lastPosition.y, 0x00000), VECTOR3x(rect2->position.x - rect2->lastPosition.x, (rect2->position.y + rect2->size.y) - (rect2->lastPosition.y + rect2->lastSize.y), 0x00000), 0x00000);
	}
	
	// rect1 top left corner vs. rect2 bottom right corner
	if (rect1->solidLeft && rect1->solidTop && rect2->solidRight && rect2->solidBottom &&
	    rect1->position.x - rect1->lastPosition.x < (rect2->position.x + rect2->size.x) - (rect2->lastPosition.x + rect2->lastSize.x) &&
	    (rect1->position.y + rect1->size.y) - (rect1->lastPosition.y + rect1->lastSize.y) > rect2->position.y - rect2->lastPosition.y &&
	    intersectSweptPoints2D(rect1->lastPosition.x, rect1->position.x,
	                           rect1->lastPosition.y + rect1->lastSize.y, rect1->position.y + rect1->size.y,
	                           rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x + rect2->size.x,
	                           rect2->lastPosition.y, rect2->position.y,
	                           &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(rect1->position.x - rect1->lastPosition.x, (rect1->position.y + rect1->size.y) - (rect1->lastPosition.y + rect1->lastSize.y), 0x00000), VECTOR3x((rect2->position.x + rect2->size.x) - (rect2->lastPosition.x + rect2->lastSize.x), rect2->position.y - rect2->lastPosition.y, 0x00000), 0x00000);
	}
	
	// rect1 bottom left corner vs. rect2 top right corner
	if (rect1->solidLeft && rect1->solidBottom && rect2->solidRight && rect2->solidTop &&
	    rect1->position.x - rect1->lastPosition.x < (rect2->position.x + rect2->size.x) - (rect2->lastPosition.x + rect2->lastSize.x) &&
	    rect1->position.y - rect1->lastPosition.y < (rect2->position.y + rect2->size.y) - (rect2->lastPosition.y + rect2->lastSize.y) &&
	    intersectSweptPoints2D(rect1->lastPosition.x, rect1->position.x,
	                           rect1->lastPosition.y, rect1->position.y,
	                           rect2->lastPosition.x + rect2->lastSize.x, rect2->position.x + rect2->size.x,
	                           rect2->lastPosition.y + rect2->lastSize.y, rect2->position.y + rect2->size.y,
	                           &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(rect1->position.x - rect1->lastPosition.x, rect1->position.y - rect1->lastPosition.y, 0x00000), VECTOR3x((rect2->position.x + rect2->size.x) - (rect2->lastPosition.x + rect2->lastSize.x), (rect2->position.y + rect2->size.y) - (rect2->lastPosition.y + rect2->lastSize.y), 0x00000), 0x00000);
	}
	
	return false;
}

bool intersectionHandler_rect2D_circle(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	CollisionRect2D * rect = (CollisionRect2D *) object1;
	CollisionCircle * circle = (CollisionCircle *) object2;
	fixed16_16 time, bestTime = FIXED_16_16_MAX;
	fixed16_16 contactArea;
	Vector3x normal, bestNormal = VECTOR3x_ZERO, bestObject1Vector = VECTOR3x_ZERO;
	
	// TODO: Change definition of solidity to include entire span of circle from edge to center
	// rect right vs. circle left
	if (rect->solidRight &&
	    (rect->position.x + rect->size.x) - (rect->lastPosition.x + rect->lastSize.x) > circle->position.x - circle->lastPosition.x &&
	    intersectSweptLineSegments(rect->lastPosition.x + rect->lastSize.x, rect->position.x + rect->size.x, 0x00000,
	                               circle->lastPosition.x - circle->radius, circle->position.x - circle->radius, circle->radius,
	                               rect->lastPosition.y, rect->lastPosition.y + rect->lastSize.y, rect->position.y, rect->position.y + rect->size.y,
	                               circle->lastPosition.y, circle->lastPosition.y, circle->position.y, circle->position.y,
	                               &time, &contactArea)) {
		bestTime = time;
		bestNormal = VECTOR3x_LEFT;
		bestObject1Vector = VECTOR3x((rect->position.x + rect->size.x) - (rect->lastPosition.x + rect->lastSize.x), rect->position.y - rect->lastPosition.y, 0x00000);
	}
	
	// rect left vs. circle right
	if (rect->solidLeft &&
	    rect->position.x - rect->lastPosition.x < circle->position.x - circle->lastPosition.x &&
	    intersectSweptLineSegments(rect->lastPosition.x, rect->position.x, 0x00000,
	                               circle->lastPosition.x + circle->radius, circle->position.x + circle->radius, circle->radius,
	                               rect->lastPosition.y, rect->lastPosition.y + rect->lastSize.y, rect->position.y, rect->position.y + rect->size.y,
	                               circle->lastPosition.y, circle->lastPosition.y, circle->position.y, circle->position.y,
	                               &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_RIGHT;
			bestObject1Vector = VECTOR3x(rect->position.x - rect->lastPosition.x, rect->position.y - rect->lastPosition.y, 0x00000);
		}
	}
	
	// rect top vs. circle bottom
	if (rect->solidTop &&
	    (rect->position.y + rect->size.y) - (rect->lastPosition.y + rect->lastSize.y) > circle->position.y - circle->lastPosition.y &&
	    intersectSweptLineSegments(rect->lastPosition.y + rect->lastSize.y, rect->position.y + rect->size.y, 0x00000,
	                               circle->lastPosition.y - circle->radius, circle->position.y - circle->radius, circle->radius,
	                               rect->lastPosition.x, rect->lastPosition.x + rect->lastSize.x, rect->position.x, rect->position.x + rect->size.x,
	                               circle->lastPosition.x, circle->lastPosition.x, circle->position.x, circle->position.x,
	                               &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_DOWN;
			bestObject1Vector = VECTOR3x(rect->position.x - rect->lastPosition.x, (rect->position.y + rect->size.y) - (rect->lastPosition.y + rect->lastSize.y), 0x00000);
		}
	}
	
	// rect bottom vs. circle top
	if (rect->solidBottom &&
	    rect->position.y - rect->lastPosition.y < circle->position.y - circle->lastPosition.y &&
	    intersectSweptLineSegments(rect->lastPosition.y, rect->position.y, 0x00000,
	                               circle->lastPosition.y + circle->radius, circle->position.y + circle->radius, circle->radius,
	                               rect->lastPosition.x, rect->lastPosition.x + rect->lastSize.x, rect->position.x, rect->position.x + rect->size.x,
	                               circle->lastPosition.x, circle->lastPosition.x, circle->position.x, circle->position.x,
	                               &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_UP;
			bestObject1Vector = VECTOR3x(rect->position.x - rect->lastPosition.x, rect->position.y - rect->lastPosition.y, 0x00000);
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		returnIntersection(bestTime, bestNormal, bestObject1Vector, VECTOR3x(circle->position.x - circle->lastPosition.x, circle->position.y - circle->lastPosition.y, 0x00000), 0x00000);
	}
	
	// rect bottom left corner vs. circle
	if ((rect->solidLeft || rect->solidBottom) &&
	    intersectSweptCircles(rect->lastPosition, rect->position, 0x00000,
	                          circle->lastPosition, circle->position, circle->radius,
	                          &time, &normal)) {
		if (normal.x >= 0 && normal.y >= 0 && time < bestTime) {
			bestTime = time;
			bestNormal = normal;
			bestObject1Vector = VECTOR3x(rect->position.x - rect->lastPosition.x, rect->position.y - rect->lastPosition.y, 0x00000);
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
			bestObject1Vector = VECTOR3x((rect->position.x + rect->size.x) - (rect->lastPosition.x + rect->lastSize.y), rect->position.y - rect->lastPosition.y, 0x00000);
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
			bestObject1Vector = VECTOR3x(rect->position.x - rect->lastPosition.x, (rect->position.y + rect->size.y) - (rect->lastPosition.y + rect->lastSize.y), 0x00000);
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
			bestObject1Vector = VECTOR3x((rect->position.x + rect->size.x) - (rect->lastPosition.x + rect->lastSize.y), (rect->position.y + rect->size.y) - (rect->lastPosition.y + rect->lastSize.y), 0x00000);
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		returnIntersection(bestTime, bestNormal, bestObject1Vector, VECTOR3x(circle->position.x - circle->lastPosition.x, circle->position.y - circle->lastPosition.y, 0x00000), 0x00000);
	}
	
	return false;
}

bool intersectionHandler_rect2D_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_rect2D_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_circle_circle(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	CollisionCircle * circle1 = (CollisionCircle *) object1, * circle2 = (CollisionCircle *) object2;
	fixed16_16 time;
	Vector3x normal;
	
	if (intersectSweptCircles(circle1->lastPosition, circle1->position, circle1->radius,
	                          circle2->lastPosition, circle2->position, circle2->radius,
	                          &time, &normal)) {
		returnIntersection(time, normal, VECTOR3x(circle1->position.x - circle1->lastPosition.x, circle1->position.y - circle1->lastPosition.y, 0x00000), VECTOR3x(circle2->position.x - circle2->lastPosition.x, circle2->position.y - circle2->lastPosition.y, 0x00000), 0x00000);
	}
	return false;
}

bool intersectionHandler_circle_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_circle_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_line2D_line2D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	// Already intersecting and moving inward (need test!)
	// Endpoints of line1 pass through line2
	// Endpoints of line2 pass through line1
	return false;
}

bool intersectionHandler_line2D_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_polygon_polygon(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_box3D_box3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	fixed16_16 time, bestTime = FIXED_16_16_MAX;
	fixed16_16 contactArea, bestContactArea = 0x00000;
	Vector3x bestNormal = VECTOR3x_ZERO, bestObject1Vector = VECTOR3x_ZERO, bestObject2Vector = VECTOR3x_ZERO;
	CollisionBox3D * box1 = (CollisionBox3D *) object1, * box2 = (CollisionBox3D *) object2;
	
#pragma mark Faces
	// box1 right vs. box2 left
	if (box1->solidRight && box2->solidLeft &&
	    (box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x) > box2->position.x - box2->lastPosition.x &&
	    intersectSweptQuads(box1->lastPosition.x + box1->lastSize.x, box1->position.x + box1->size.x, getBoxEdgeThicknessX(box1),
	                        box2->lastPosition.x, box2->position.x, getBoxEdgeThicknessX(box2),
	                        box1->lastPosition.y, box1->lastPosition.y + box1->lastSize.y, box1->position.y, box1->position.y + box1->size.y,
	                        box2->lastPosition.y, box2->lastPosition.y + box2->lastSize.y, box2->position.y, box2->position.y + box2->size.y,
	                        box1->lastPosition.z, box1->lastPosition.z + box1->lastSize.z, box1->position.z, box1->position.z + box1->size.z,
	                        box2->lastPosition.z, box2->lastPosition.z + box2->lastSize.z, box2->position.z, box2->position.z + box2->size.z,
	                        &time, &contactArea)) {
		bestTime = time;
		bestNormal = VECTOR3x_LEFT;
		bestObject1Vector = VECTOR3x((box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x), box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z);
		bestObject2Vector = VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z);
		bestContactArea = contactArea;
	}
	
	// box1 left vs. box2 right
	if (box1->solidLeft && box2->solidRight &&
	    box1->position.x - box1->lastPosition.x < (box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x) &&
	    intersectSweptQuads(box1->lastPosition.x, box1->position.x, getBoxEdgeThicknessX(box1),
	                        box2->lastPosition.x + box2->lastSize.x, box2->position.x + box2->size.x, getBoxEdgeThicknessX(box2),
	                        box1->lastPosition.y, box1->lastPosition.y + box1->lastSize.y, box1->position.y, box1->position.y + box1->size.y,
	                        box2->lastPosition.y, box2->lastPosition.y + box2->lastSize.y, box2->position.y, box2->position.y + box2->size.y,
	                        box1->lastPosition.z, box1->lastPosition.z + box1->lastSize.z, box1->position.z, box1->position.z + box1->size.z,
	                        box2->lastPosition.z, box2->lastPosition.z + box2->lastSize.z, box2->position.z, box2->position.z + box2->size.z,
	                        &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_RIGHT;
			bestObject1Vector = VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z);
			bestObject2Vector = VECTOR3x((box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x), box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z);
			bestContactArea = contactArea;
		}
	}
	
	// box1 top vs. box2 bottom
	if (box1->solidTop && box2->solidBottom &&
	    (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y) > box2->position.y - box2->lastPosition.y &&
	    intersectSweptQuads(box1->lastPosition.y + box1->lastSize.y, box1->position.y + box1->size.y, getBoxEdgeThicknessY(box1),
	                        box2->lastPosition.y, box2->position.y, getBoxEdgeThicknessY(box2),
	                        box1->lastPosition.x, box1->lastPosition.x + box1->lastSize.x, box1->position.x, box1->position.x + box1->size.x,
	                        box2->lastPosition.x, box2->lastPosition.x + box2->lastSize.x, box2->position.x, box2->position.x + box2->size.x,
	                        box1->lastPosition.z, box1->lastPosition.z + box1->lastSize.z, box1->position.z, box1->position.z + box1->size.z,
	                        box2->lastPosition.z, box2->lastPosition.z + box2->lastSize.z, box2->position.z, box2->position.z + box2->size.z,
	                        &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_DOWN;
			bestObject1Vector = VECTOR3x(box1->position.x - box1->lastPosition.x, (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y), box1->position.z - box1->lastPosition.z);
			bestObject2Vector = VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z);
			bestContactArea = contactArea;
		}
	}
	
	// box1 bottom vs. box2 top
	if (box1->solidBottom && box2->solidTop &&
	    box1->position.y - box1->lastPosition.y < (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y) &&
	    intersectSweptQuads(box1->lastPosition.y, box1->position.y, getBoxEdgeThicknessY(box1),
	                        box2->lastPosition.y + box2->lastSize.y, box2->position.y + box2->size.y, getBoxEdgeThicknessY(box2),
	                        box1->lastPosition.x, box1->lastPosition.x + box1->lastSize.x, box1->position.x, box1->position.x + box1->size.x,
	                        box2->lastPosition.x, box2->lastPosition.x + box2->lastSize.x, box2->position.x, box2->position.x + box2->size.x,
	                        box1->lastPosition.z, box1->lastPosition.z + box1->lastSize.z, box1->position.z, box1->position.z + box1->size.z,
	                        box2->lastPosition.z, box2->lastPosition.z + box2->lastSize.z, box2->position.z, box2->position.z + box2->size.z,
	                        &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_UP;
			bestObject1Vector = VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z);
			bestObject2Vector = VECTOR3x(box2->position.x - box2->lastPosition.x, (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y), box2->position.z - box2->lastPosition.z);
			bestContactArea = contactArea;
		}
	}
	
	// box1 front vs. box2 back
	if (box1->solidFront && box2->solidBack &&
	    (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z) > box2->position.z - box2->lastPosition.z &&
	    intersectSweptQuads(box1->lastPosition.z + box1->lastSize.z, box1->position.z + box1->size.z, getBoxEdgeThicknessZ(box1),
	                        box2->lastPosition.z, box2->position.z, getBoxEdgeThicknessZ(box2),
	                        box1->lastPosition.x, box1->lastPosition.x + box1->lastSize.x, box1->position.x, box1->position.x + box1->size.x,
	                        box2->lastPosition.x, box2->lastPosition.x + box2->lastSize.x, box2->position.x, box2->position.x + box2->size.x,
	                        box1->lastPosition.y, box1->lastPosition.y + box1->lastSize.y, box1->position.y, box1->position.y + box1->size.y,
	                        box2->lastPosition.y, box2->lastPosition.y + box2->lastSize.y, box2->position.y, box2->position.y + box2->size.y,
	                        &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_BACK;
			bestObject1Vector = VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z));
			bestObject2Vector = VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z);
			bestContactArea = contactArea;
		}
	}
	
	// box1 back vs. box2 front
	if (box1->solidBack && box2->solidFront &&
	    box1->position.z - box1->lastPosition.z < (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z) &&
	    intersectSweptQuads(box1->lastPosition.z, box1->position.z, getBoxEdgeThicknessZ(box1),
	                        box2->lastPosition.z + box2->lastSize.z, box2->position.z + box2->size.z, getBoxEdgeThicknessZ(box2),
	                        box1->lastPosition.x, box1->lastPosition.x + box1->lastSize.x, box1->position.x, box1->position.x + box1->size.x,
	                        box2->lastPosition.x, box2->lastPosition.x + box2->lastSize.x, box2->position.x, box2->position.x + box2->size.x,
	                        box1->lastPosition.y, box1->lastPosition.y + box1->lastSize.y, box1->position.y, box1->position.y + box1->size.y,
	                        box2->lastPosition.y, box2->lastPosition.y + box2->lastSize.y, box2->position.y, box2->position.y + box2->size.y,
	                        &time, &contactArea)) {
		if (time < bestTime) {
			bestTime = time;
			bestNormal = VECTOR3x_FRONT;
			bestObject1Vector = VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z);
			bestObject2Vector = VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z));
			bestContactArea = contactArea;
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		returnIntersection(bestTime, bestNormal, bestObject1Vector, bestObject2Vector, bestContactArea);
	}
	
#pragma mark Edges
	// box1 top right edge vs. box2 bottom left edge
	if (box1->solidRight && box1->solidTop && box2->solidLeft && box2->solidBottom &&
	    (box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x) > box2->position.x - box2->lastPosition.x &&
	    (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y) > box2->position.y - box2->lastPosition.y &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.x + box1->lastSize.x, box1->lastPosition.z, box1->lastPosition.y + box1->lastSize.y), VECTOR3x(box1->position.x + box1->size.x, box1->position.z, box1->position.y + box1->size.y), box1->lastSize.z, box1->size.z,
	                                 VECTOR3x(box2->lastPosition.x, box2->lastPosition.z, box2->lastPosition.y), VECTOR3x(box2->position.x, box2->position.z, box2->position.y), box2->lastSize.z, box2->size.z,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x), (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y), box1->position.z - box1->lastPosition.z), VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 bottom right edge vs. box2 top left edge
	if (box1->solidRight && box1->solidBottom && box2->solidLeft && box2->solidTop &&
	    (box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x) > box2->position.x - box2->lastPosition.x &&
	    box1->position.y - box1->lastPosition.y < (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y) &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.x + box1->lastSize.x, box1->lastPosition.z, box1->lastPosition.y), VECTOR3x(box1->position.x + box1->size.x, box1->position.z, box1->position.y), box1->lastSize.z, box1->size.z,
	                                 VECTOR3x(box2->lastPosition.x, box2->lastPosition.z, box2->lastPosition.y + box2->lastSize.y), VECTOR3x(box2->position.x, box2->position.z, box2->position.y + box2->size.y), box2->lastSize.z, box2->size.z,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x), box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z), VECTOR3x(box2->position.x - box2->lastPosition.x, (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y), box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 top left edge vs. box2 bottom right edge
	if (box1->solidLeft && box1->solidTop && box2->solidRight && box2->solidBottom &&
	    box1->position.x - box1->lastPosition.x < (box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x) &&
	    (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y) > box2->position.y - box2->lastPosition.y &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.x, box1->lastPosition.z, box1->lastPosition.y + box1->lastSize.y), VECTOR3x(box1->position.x, box1->position.z, box1->position.y + box1->size.y), box1->lastSize.z, box1->size.z,
	                                 VECTOR3x(box2->lastPosition.x + box2->lastSize.x, box2->lastPosition.z, box2->lastPosition.y), VECTOR3x(box2->position.x + box2->size.x, box2->position.z, box2->position.y), box2->lastSize.z, box2->size.z,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(box1->position.x - box1->lastPosition.x, (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y), box1->position.z - box1->lastPosition.z), VECTOR3x((box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x), box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 bottom left edge vs. box2 top right edge
	if (box1->solidLeft && box1->solidBottom && box2->solidRight && box2->solidTop &&
	    box1->position.x - box1->lastPosition.x < (box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x) &&
	    box1->position.y - box1->lastPosition.y < (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y) &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.x, box1->lastPosition.z, box1->lastPosition.y), VECTOR3x(box1->position.x, box1->position.z, box1->position.y), box1->lastSize.z, box1->size.z,
	                                 VECTOR3x(box2->lastPosition.x + box2->lastSize.x, box2->lastPosition.z, box2->lastPosition.y + box2->lastSize.y), VECTOR3x(box2->position.x + box2->size.x, box2->position.z, box2->position.y + box2->size.y), box2->lastSize.z, box2->size.z,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z), VECTOR3x((box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x), (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y), box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 front right edge vs. box2 back left edge
	if (box1->solidRight && box1->solidFront && box2->solidLeft && box2->solidBack &&
	    (box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x) > box2->position.x - box2->lastPosition.x &&
	    (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z) > box2->position.z - box2->lastPosition.z &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.x + box1->lastSize.x, box1->lastPosition.y, box1->lastPosition.z + box1->lastSize.z), VECTOR3x(box1->position.x + box1->size.x, box1->position.y, box1->position.z + box1->size.z), box1->lastSize.y, box1->size.y,
	                                 VECTOR3x(box2->lastPosition.x, box2->lastPosition.y, box2->lastPosition.z), VECTOR3x(box2->position.x, box2->position.y, box2->position.z), box2->lastSize.y, box2->size.y,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x), box1->position.y - box1->lastPosition.y, (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z)), VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 back right edge vs. box2 front left edge
	if (box1->solidRight && box1->solidBack && box2->solidLeft && box2->solidFront &&
	    (box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x) > box2->position.x - box2->lastPosition.x &&
	    box1->position.z - box1->lastPosition.z < (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z) &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.x + box1->lastSize.x, box1->lastPosition.y, box1->lastPosition.z), VECTOR3x(box1->position.x + box1->size.x, box1->position.y, box1->position.z), box1->lastSize.y, box1->size.y,
	                                 VECTOR3x(box2->lastPosition.x, box2->lastPosition.y, box2->lastPosition.z + box2->lastSize.z), VECTOR3x(box2->position.x, box2->position.y, box2->position.z + box2->size.z), box2->lastSize.y, box2->size.y,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x), box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z), VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z)), 0x00000);
	}
	
	// box1 front left edge vs. box2 back right edge
	if (box1->solidLeft && box1->solidFront && box2->solidRight && box2->solidBack &&
	    box1->position.x - box1->lastPosition.x < (box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x) &&
	    (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z) > box2->position.z - box2->lastPosition.z &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.x, box1->lastPosition.y, box1->lastPosition.z + box1->lastSize.z), VECTOR3x(box1->position.x, box1->position.y, box1->position.z + box1->size.z), box1->lastSize.y, box1->size.y,
	                                 VECTOR3x(box2->lastPosition.x + box2->lastSize.x, box2->lastPosition.y, box2->lastPosition.z), VECTOR3x(box2->position.x + box2->size.x, box2->position.y, box2->position.z), box2->lastSize.y, box2->size.y,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z)), VECTOR3x((box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x), box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 back left edge vs. box2 front right edge
	if (box1->solidLeft && box1->solidBack && box2->solidRight && box2->solidFront &&
	    box1->position.x - box1->lastPosition.x < (box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x) &&
	    box1->position.z - box1->lastPosition.z < (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z) &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.x, box1->lastPosition.y, box1->lastPosition.z), VECTOR3x(box1->position.x, box1->position.y, box1->position.z), box1->lastSize.y, box1->size.y,
	                                 VECTOR3x(box2->lastPosition.x + box2->lastSize.x, box2->lastPosition.y, box2->lastPosition.z + box2->lastSize.z), VECTOR3x(box2->position.x + box2->size.x, box2->position.y, box2->position.z + box2->size.z), box2->lastSize.y, box2->size.y,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z), VECTOR3x((box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x), box2->position.y - box2->lastPosition.y, (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z)), 0x00000);
	}
	
	// box1 front top edge vs. box2 back bottom edge
	if (box1->solidFront && box1->solidTop && box2->solidBack && box2->solidBottom &&
	    (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y) > box2->position.y - box2->lastPosition.y &&
	    (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z) > box2->position.z - box2->lastPosition.z &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.y + box1->lastSize.y, box1->lastPosition.x, box1->lastPosition.z + box1->lastSize.z), VECTOR3x(box1->position.y + box1->size.y, box1->position.x, box1->position.z + box1->size.z), box1->lastSize.x, box1->size.x,
	                                 VECTOR3x(box2->lastPosition.y, box2->lastPosition.x, box2->lastPosition.z), VECTOR3x(box2->position.y, box2->position.x, box2->position.z), box2->lastSize.x, box2->size.x,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_BACK, VECTOR3x(box1->position.x - box1->lastPosition.x, (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y), (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z)), VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 back top edge vs. box2 front bottom edge
	if (box1->solidFront && box1->solidBottom && box2->solidBack && box2->solidTop &&
	    (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y) > box2->position.y - box2->lastPosition.y &&
	    box1->position.z - box1->lastPosition.z < (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z) &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.y + box1->lastSize.y, box1->lastPosition.x, box1->lastPosition.z), VECTOR3x(box1->position.y + box1->size.y, box1->position.x, box1->position.z), box1->lastSize.x, box1->size.x,
	                                 VECTOR3x(box2->lastPosition.y, box2->lastPosition.x, box2->lastPosition.z + box2->lastSize.z), VECTOR3x(box2->position.y, box2->position.x, box2->position.z + box2->size.z), box2->lastSize.x, box2->size.x,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_BACK, VECTOR3x(box1->position.x - box1->lastPosition.x, (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y), box1->position.z - box1->lastPosition.z), VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z)), 0x00000);
	}
	
	// box1 front bottom edge vs. box2 back top edge
	if (box1->solidBack && box1->solidTop && box2->solidFront && box2->solidBottom &&
	    box1->position.y - box1->lastPosition.y < (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y) &&
	    (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z) > box2->position.z - box2->lastPosition.z &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.y, box1->lastPosition.x, box1->lastPosition.z + box1->lastSize.z), VECTOR3x(box1->position.y, box1->position.x, box1->position.z + box1->size.z), box1->lastSize.x, box1->size.x,
	                                 VECTOR3x(box2->lastPosition.y + box2->lastSize.y, box2->lastPosition.x, box2->lastPosition.z), VECTOR3x(box2->position.y + box2->size.y, box2->position.x, box2->position.z), box2->lastSize.x, box2->size.x,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_FRONT, VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z)), VECTOR3x(box2->position.x - box2->lastPosition.x, (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y), box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 back bottom edge vs. box2 front top edge
	if (box1->solidBack && box1->solidBottom && box2->solidFront && box2->solidTop &&
	    box1->position.y - box1->lastPosition.y < (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y) &&
	    box1->position.z - box1->lastPosition.z < (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z) &&
	    intersectSweptExtrudedPoints(VECTOR3x(box1->lastPosition.y, box1->lastPosition.x, box1->lastPosition.z), VECTOR3x(box1->position.y, box1->position.x, box1->position.z), box1->lastSize.x, box1->size.x,
	                                 VECTOR3x(box2->lastPosition.y + box2->lastSize.y, box2->lastPosition.x, box2->lastPosition.z + box2->lastSize.z), VECTOR3x(box2->position.y + box2->size.y, box2->position.x, box2->position.z + box2->size.z), box2->lastSize.x, box2->size.x,
	                                 &time)) {
		returnIntersection(time, VECTOR3x_FRONT, VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z), VECTOR3x(box2->position.x - box2->lastPosition.x, (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y), (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z)), 0x00000);
	}
	
#pragma mark Corners
	// box1 front top right corner vs. box2 back bottom left corner
	if (box1->solidRight && box1->solidTop && box1->solidFront && box2->solidLeft && box2->solidBottom && box2->solidBack &&
	    (box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x) > box2->position.x - box2->lastPosition.x &&
	    (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y) > box2->position.y - box2->lastPosition.y &&
	    (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z) > box2->position.z - box2->lastPosition.z &&
	    intersectSweptPoints3D(box1->lastPosition.x + box1->lastSize.x, box1->position.x + box1->size.x,
	                           box1->lastPosition.y + box1->lastSize.y, box1->position.y + box1->size.y,
	                           box1->lastPosition.z + box1->lastSize.z, box1->position.z + box1->size.z,
	                           box2->lastPosition.x, box2->position.x,
	                           box2->lastPosition.y, box2->position.y,
	                           box2->lastPosition.z, box2->position.z,
	                           &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x), (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y), (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z)), VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 front bottom right corner vs. box2 back top left corner
	if (box1->solidRight && box1->solidBottom && box1->solidFront && box2->solidLeft && box2->solidTop && box2->solidBack &&
	    (box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x) > box2->position.x - box2->lastPosition.x &&
	    box1->position.y - box1->lastPosition.y < (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y) &&
	    (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z) > box2->position.z - box2->lastPosition.z &&
	    intersectSweptPoints3D(box1->lastPosition.x + box1->lastSize.x, box1->position.x + box1->size.x,
	                           box1->lastPosition.y, box1->position.y,
	                           box1->lastPosition.z + box1->lastSize.z, box1->position.z + box1->size.z,
	                           box2->lastPosition.x, box2->position.x,
	                           box2->lastPosition.y + box2->lastSize.y, box2->position.y + box2->size.y,
	                           box2->lastPosition.z, box2->position.z,
	                           &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x), box1->position.y - box1->lastPosition.y, (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z)), VECTOR3x(box2->position.x - box2->lastPosition.x, (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y), box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 front top left corner vs. box2 back bottom right corner
	if (box1->solidLeft && box1->solidTop && box1->solidFront && box2->solidRight && box2->solidBottom && box2->solidBack &&
	    box1->position.x - box1->lastPosition.x < (box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x) &&
	    (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y) > box2->position.y - box2->lastPosition.y &&
	    (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z) > box2->position.z - box2->lastPosition.z &&
	    intersectSweptPoints3D(box1->lastPosition.x, box1->position.x,
	                           box1->lastPosition.y + box1->lastSize.y, box1->position.y + box1->size.y,
	                           box1->lastPosition.z + box1->lastSize.z, box1->position.z + box1->size.z,
	                           box2->lastPosition.x + box2->lastSize.x, box2->position.x + box2->size.x,
	                           box2->lastPosition.y, box2->position.y,
	                           box2->lastPosition.z, box2->position.z,
	                           &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(box1->position.x - box1->lastPosition.x, (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y), (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z)), VECTOR3x((box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x), box2->position.y - box2->lastPosition.y, box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 front bottom left corner vs. box2 back top right corner
	if (box1->solidLeft && box1->solidBottom && box1->solidFront && box2->solidRight && box2->solidTop && box2->solidBack &&
	    box1->position.x - box1->lastPosition.x < (box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x) &&
	    box1->position.y - box1->lastPosition.y < (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y) &&
	    (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z) > box2->position.z - box2->lastPosition.z &&
	    intersectSweptPoints3D(box1->lastPosition.x, box1->position.x,
	                           box1->lastPosition.y, box1->position.y,
	                           box1->lastPosition.z + box1->lastSize.z, box1->position.z + box1->size.z,
	                           box2->lastPosition.x + box2->lastSize.x, box2->position.x + box2->size.x,
	                           box2->lastPosition.y + box2->lastSize.y, box2->position.y + box2->size.y,
	                           box2->lastPosition.z, box2->position.z,
	                           &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, (box1->position.z + box1->size.z) - (box1->lastPosition.z + box1->lastSize.z)), VECTOR3x((box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x), (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y), box2->position.z - box2->lastPosition.z), 0x00000);
	}
	
	// box1 back top right corner vs. box2 front bottom left corner
	if (box1->solidRight && box1->solidTop && box1->solidBack && box2->solidLeft && box2->solidBottom && box2->solidFront &&
	    (box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x) > box2->position.x - box2->lastPosition.x &&
	    (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y) > box2->position.y - box2->lastPosition.y &&
	    box1->position.z - box1->lastPosition.z < (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z) &&
	    intersectSweptPoints3D(box1->lastPosition.x + box1->lastSize.x, box1->position.x + box1->size.x,
	                           box1->lastPosition.y + box1->lastSize.y, box1->position.y + box1->size.y,
	                           box1->lastPosition.z, box1->position.z,
	                           box2->lastPosition.x, box2->position.x,
	                           box2->lastPosition.y, box2->position.y,
	                           box2->lastPosition.z + box2->lastSize.z, box2->position.z + box2->size.z,
	                           &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x), (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y), box1->position.z - box1->lastPosition.z), VECTOR3x(box2->position.x - box2->lastPosition.x, box2->position.y - box2->lastPosition.y, (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z)), 0x00000);
	}
	
	// box1 back bottom right corner vs. box2 front top left corner
	if (box1->solidRight && box1->solidBottom && box1->solidBack && box2->solidLeft && box2->solidTop && box2->solidFront &&
	    (box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x) > box2->position.x - box2->lastPosition.x &&
	    box1->position.y - box1->lastPosition.y < (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y) &&
	    box1->position.z - box1->lastPosition.z < (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z) &&
	    intersectSweptPoints3D(box1->lastPosition.x + box1->lastSize.x, box1->position.x + box1->size.x,
	                           box1->lastPosition.y, box1->position.y,
	                           box1->lastPosition.z, box1->position.z,
	                           box2->lastPosition.x, box2->position.x,
	                           box2->lastPosition.y + box2->lastSize.y, box2->position.y + box2->size.y,
	                           box2->lastPosition.z + box2->lastSize.z, box2->position.z + box2->size.z,
	                           &time)) {
		returnIntersection(time, VECTOR3x_LEFT, VECTOR3x((box1->position.x + box1->size.x) - (box1->lastPosition.x + box1->lastSize.x), box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z), VECTOR3x(box2->position.x - box2->lastPosition.x, (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y), (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z)), 0x00000);
	}
	
	// box1 back top left corner vs. box2 front bottom right corner
	if (box1->solidLeft && box1->solidTop && box1->solidBack && box2->solidRight && box2->solidBottom && box2->solidFront &&
	    box1->position.x - box1->lastPosition.x < (box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x) &&
	    (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y) > box2->position.y - box2->lastPosition.y &&
	    box1->position.z - box1->lastPosition.z < (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z) &&
	    intersectSweptPoints3D(box1->lastPosition.x, box1->position.x,
	                           box1->lastPosition.y + box1->lastSize.y, box1->position.y + box1->size.y,
	                           box1->lastPosition.z, box1->position.z,
	                           box2->lastPosition.x + box2->lastSize.x, box2->position.x + box2->size.x,
	                           box2->lastPosition.y, box2->position.y,
	                           box2->lastPosition.z + box2->lastSize.z, box2->position.z + box2->size.z,
	                           &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(box1->position.x - box1->lastPosition.x, (box1->position.y + box1->size.y) - (box1->lastPosition.y + box1->lastSize.y), box1->position.z - box1->lastPosition.z), VECTOR3x((box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x), box2->position.y - box2->lastPosition.y, (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z)), 0x00000);
	}
	
	// box1 back bottom left corner vs. box2 front top right corner
	if (box1->solidLeft && box1->solidBottom && box1->solidBack && box2->solidRight && box2->solidTop && box2->solidFront &&
	    box1->position.x - box1->lastPosition.x < (box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x) &&
	    box1->position.y - box1->lastPosition.y < (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y) &&
	    box1->position.z - box1->lastPosition.z < (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z) &&
	    intersectSweptPoints3D(box1->lastPosition.x, box1->position.x,
	                           box1->lastPosition.y, box1->position.y,
	                           box1->lastPosition.z, box1->position.z,
	                           box2->lastPosition.x + box2->lastSize.x, box2->position.x + box2->size.x,
	                           box2->lastPosition.y + box2->lastSize.y, box2->position.y + box2->size.y,
	                           box2->lastPosition.z + box2->lastSize.z, box2->position.z + box2->size.z,
	                           &time)) {
		returnIntersection(time, VECTOR3x_RIGHT, VECTOR3x(box1->position.x - box1->lastPosition.x, box1->position.y - box1->lastPosition.y, box1->position.z - box1->lastPosition.z), VECTOR3x((box2->position.x + box2->size.x) - (box2->lastPosition.x + box2->lastSize.x), (box2->position.y + box2->size.y) - (box2->lastPosition.y + box2->lastSize.y), (box2->position.z + box2->size.z) - (box2->lastPosition.z + box2->lastSize.z)), 0x00000);
	}
	
	return false;
}

bool intersectionHandler_box3D_sphere(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_box3D_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_box3D_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_box3D_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_sphere_sphere(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	CollisionSphere * sphere1 = (CollisionSphere *) object1, * sphere2 = (CollisionSphere *) object2;
	fixed16_16 time;
	Vector3x normal;
	
	if (intersectSweptSpheres(sphere1->lastPosition, sphere1->position, sphere1->radius,
	                          sphere2->lastPosition, sphere2->position, sphere2->radius,
	                          &time, &normal)) {
		returnIntersection(time, normal, VECTOR3x(sphere1->position.x - sphere1->lastPosition.x, sphere1->position.y - sphere1->lastPosition.y, sphere1->position.z - sphere1->lastPosition.z), VECTOR3x(sphere2->position.x - sphere2->lastPosition.x, sphere2->position.y - sphere2->lastPosition.y, sphere2->position.z - sphere2->lastPosition.z), 0x00000);
	}
	return false;
}

bool intersectionHandler_sphere_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_sphere_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

static Box6x box6xEnclosing2Points(Vector3x point1, Vector3x point2) {
	Box6x result;
	
	if (point1.x < point2.x) {
		result.left = point1.x;
		result.right = point2.x;
	} else {
		result.left = point2.x;
		result.right = point1.x;
	}
	if (point1.y < point2.y) {
		result.bottom = point1.y;
		result.top = point2.y;
	} else {
		result.bottom = point2.y;
		result.top = point1.y;
	}
	if (point1.z < point2.z) {
		result.back = point1.z;
		result.front = point2.z;
	} else {
		result.back = point2.z;
		result.front = point1.z;
	}
	
	if (result.right == result.left) {
		result.left -= 1;
		result.right += 1;
	}
	if (result.top == result.bottom) {
		result.bottom -= 1;
		result.top += 1;
	}
	if (result.front == result.back) {
		result.back -= 1;
		result.front += 1;
	}
	return result;
}

static inline fixed16_16 min3(fixed16_16 value1, fixed16_16 value2, fixed16_16 value3) {
	fixed16_16 result;
	
	result = value1;
	if (value2 < result) {
		result = value2;
	}
	if (value3 < result) {
		result = value3;
	}
	return result;
}

static inline fixed16_16 max3(fixed16_16 value1, fixed16_16 value2, fixed16_16 value3) {
	fixed16_16 result;
	
	result = value1;
	if (value2 > result) {
		result = value2;
	}
	if (value3 > result) {
		result = value3;
	}
	return result;
}

static Box6x box6xEnclosing3Points(Vector3x point1, Vector3x point2, Vector3x point3) {
	Box6x result;
	
	result.left =   min3(point1.x, point2.x, point3.x);
	result.right =  max3(point1.x, point2.x, point3.x);
	result.bottom = min3(point1.y, point2.y, point3.y);
	result.top =    max3(point1.y, point2.y, point3.y);
	result.back =   min3(point1.z, point2.z, point3.z);
	result.front =  max3(point1.z, point2.z, point3.z);
	
	if (result.right == result.left) {
		result.left -= 1;
		result.right += 1;
	}
	if (result.top == result.bottom) {
		result.bottom -= 1;
		result.top += 1;
	}
	if (result.front == result.back) {
		result.back -= 1;
		result.front += 1;
	}
	return result;
}

bool intersectionHandler_sphere_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	fixed16_16 time, bestTime = FIXED_16_16_MAX;
	Vector3x bestNormal = VECTOR3x_ZERO;
	CollisionSphere * sphere = (CollisionSphere *) object1;
	CollisionStaticTrimesh * trimesh = (CollisionStaticTrimesh *) object2;
	Vector3x vertices[3];
	Vector3x normal;
	unsigned int triangleIndex, edgeIndex, vertexIndex;
	Box6x sphereBounds;
	
	sphereBounds = CollisionSphere_getCollisionBounds(sphere);
	for (vertexIndex = 0; vertexIndex < trimesh->vertexCount; vertexIndex++) {
		if (trimesh->vertices[vertexIndex].convex &&
		    Box6x_containsVector3x(sphereBounds, trimesh->vertices[vertexIndex].position) &&
		    intersectSweptSpheres(sphere->lastPosition, sphere->position, sphere->radius,
		                          trimesh->vertices[vertexIndex].position, trimesh->vertices[vertexIndex].position, 0x00000,
		                          &time, &normal)) {
			if (time < bestTime) {
				bestTime = time;
				bestNormal = normal;
			}
		}
	}
	for (edgeIndex = 0; edgeIndex < trimesh->edgeCount; edgeIndex++) {
		vertices[0] = trimesh->vertices[trimesh->edges[edgeIndex].vertexIndexes[0]].position;
		vertices[1] = trimesh->vertices[trimesh->edges[edgeIndex].vertexIndexes[1]].position;
		if (Box6x_intersectsBox6x(sphereBounds, box6xEnclosing2Points(vertices[0], vertices[1])) &&
		    intersectSweptSphereTrimeshEdge(sphere->lastPosition, sphere->position, sphere->radius,
		                                    vertices[0], vertices[1], trimesh->edges[edgeIndex].triangleNormals[0], trimesh->edges[edgeIndex].triangleNormals[1], trimesh->edges[edgeIndex].planarTransform, trimesh->edges[edgeIndex].length,
		                                    &time, &normal)) {
			if (time < bestTime) {
				bestTime = time;
				bestNormal = normal;
			}
		}
	}
	for (triangleIndex = 0; triangleIndex < trimesh->triangleCount; triangleIndex++) {
		vertices[0] = trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[0]].position;
		vertices[1] = trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[1]].position;
		vertices[2] = trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[2]].position;
		if (Box6x_intersectsBox6x(sphereBounds, box6xEnclosing3Points(vertices[0], vertices[1], vertices[2])) &&
		    intersectSweptSphereTriangle(sphere->lastPosition, sphere->position, sphere->radius,
		                                 vertices[0], vertices[1], vertices[2], trimesh->triangles[triangleIndex].normal,
		                                 &time)) {
			if (time < bestTime) {
				bestTime = time;
				bestNormal = trimesh->triangles[triangleIndex].normal;
			}
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		returnIntersection(bestTime, bestNormal, Vector3x_subtract(sphere->position, sphere->lastPosition), VECTOR3x_ZERO, 0x00000);
	}
	
	return false;
}

bool intersectionHandler_line3D_line3D(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_line3D_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_line3D_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

bool intersectionHandler_capsule_capsule(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	CollisionCapsule * capsule1 = (CollisionCapsule *) object1, * capsule2 = (CollisionCapsule *) object2;
	fixed16_16 time, bestTime = FIXED_16_16_MAX;
	fixed16_16 contactArea, bestContactArea = 0x00000;
	Vector3x normal, bestNormal = VECTOR3x_ZERO;
	
	if (intersectSweptCylinderWalls(capsule1->lastPosition, capsule1->position, capsule1->radius, capsule1->cylinderHeight,
	                                capsule2->lastPosition, capsule2->position, capsule2->radius, capsule2->cylinderHeight,
	                                &time, &normal, &contactArea)) {
		bestTime = time;
		bestNormal = normal;
		bestContactArea = contactArea;
		
	} else {
		if (intersectSweptSpheres(capsule1->lastPosition, capsule1->position, capsule1->radius,
		                          VECTOR3x(capsule2->lastPosition.x, capsule2->lastPosition.y + capsule2->cylinderHeight, capsule2->lastPosition.z), VECTOR3x(capsule2->position.x, capsule2->position.y + capsule2->cylinderHeight, capsule2->position.z), capsule2->radius,
		                          &time, &normal)) {
			if (normal.y >= 0x00000 && time < bestTime) {
				bestTime = time;
				bestNormal = normal;
				bestContactArea = 0x00000;
			}
		}
		
		if (intersectSweptSpheres(VECTOR3x(capsule1->lastPosition.x, capsule1->lastPosition.y + capsule1->cylinderHeight, capsule1->lastPosition.z), VECTOR3x(capsule1->position.x, capsule1->position.y + capsule1->cylinderHeight, capsule1->position.z), capsule1->radius,
		                          capsule2->lastPosition, capsule2->position, capsule2->radius,
		                          &time, &normal)) {
			if (normal.y <= 0x00000 && time < bestTime) {
				bestTime = time;
				bestNormal = normal;
				bestContactArea = 0x00000;
			}
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		returnIntersection(bestTime, bestNormal, Vector3x_subtract(capsule1->position, capsule1->lastPosition), Vector3x_subtract(capsule2->position, capsule2->lastPosition), bestContactArea);
	}
	return false;
}

bool intersectionHandler_capsule_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	fixed16_16 time, bestTime = FIXED_16_16_MAX;
	Vector3x bestNormal = VECTOR3x_ZERO;
	CollisionCapsule * capsule = (CollisionCapsule *) object1;
	CollisionStaticTrimesh * trimesh = (CollisionStaticTrimesh *) object2;
	Vector3x vertices[3];
	Vector3x normal;
	Vector3x lastBottomCapPosition, lastTopCapPosition, bottomCapPosition, topCapPosition;
	unsigned int triangleIndex, edgeIndex, vertexIndex;
	Box6x capsuleBounds;
	bool intersection;
	
	lastBottomCapPosition = capsule->lastPosition;
	lastTopCapPosition = capsule->lastPosition;
	bottomCapPosition = capsule->position;
	topCapPosition = capsule->position;
	
	lastBottomCapPosition.y += capsule->radius;
	bottomCapPosition.y += capsule->radius;
	lastTopCapPosition.y += capsule->radius + capsule->cylinderHeight;
	topCapPosition.y += capsule->radius + capsule->cylinderHeight;
	
	capsuleBounds = CollisionCapsule_getCollisionBounds(capsule);
	
	for (vertexIndex = 0; vertexIndex < trimesh->vertexCount; vertexIndex++) {
		if (trimesh->vertices[vertexIndex].convex &&
		    Box6x_containsVector3x(capsuleBounds, trimesh->vertices[vertexIndex].position)) {
			
			intersection = intersectSweptSpheres(lastBottomCapPosition, bottomCapPosition, capsule->radius,
			                                     trimesh->vertices[vertexIndex].position, trimesh->vertices[vertexIndex].position, 0x00000,
			                                     &time, &normal);
			if (normal.y < 0x00000) {
				intersection = false;
			}
			
			if (!intersection) {
				intersection = intersectSweptSpheres(lastTopCapPosition, topCapPosition, capsule->radius,
				                                     trimesh->vertices[vertexIndex].position, trimesh->vertices[vertexIndex].position, 0x00000,
				                                     &time, &normal);
				if (normal.y > 0x00000) {
					intersection = false;
				}
			}
			
			if (!intersection) {
				fixed16_16 contactArea;
				
				intersection = intersectSweptCylinderWalls(lastBottomCapPosition, bottomCapPosition, capsule->radius, capsule->cylinderHeight,
				                                           trimesh->vertices[vertexIndex].position, trimesh->vertices[vertexIndex].position, 0x00000, 0x00000,
				                                           &time, &normal, &contactArea);
			}
			
			// This test is wrong, but what's the right one?
			// CollisionStaticTrimesh finds triangle normal with the lowest dot product result from vertex normal, that angle can be used in this test somehow?
			if (intersection && time < bestTime && Vector3x_dot(normal, trimesh->vertices[vertexIndex].normal) >= 0x00000) {
				bestTime = time;
				bestNormal = normal;
			}
		}
	}
	
	for (edgeIndex = 0; edgeIndex < trimesh->edgeCount; edgeIndex++) {
		vertices[0] = trimesh->vertices[trimesh->edges[edgeIndex].vertexIndexes[0]].position;
		vertices[1] = trimesh->vertices[trimesh->edges[edgeIndex].vertexIndexes[1]].position;
		if (Box6x_intersectsBox6x(capsuleBounds, box6xEnclosing2Points(vertices[0], vertices[1]))) {
			
			intersection = intersectSweptSphereTrimeshEdge(lastBottomCapPosition, bottomCapPosition, capsule->radius,
			                                               vertices[0], vertices[1], trimesh->edges[edgeIndex].triangleNormals[0], trimesh->edges[edgeIndex].triangleNormals[1], trimesh->edges[edgeIndex].planarTransform, trimesh->edges[edgeIndex].length,
			                                               &time, &normal);
			if (normal.y < 0x00000) {
				intersection = false;
			}
			
			if (!intersection) {
				intersection = intersectSweptSphereTrimeshEdge(lastTopCapPosition, topCapPosition, capsule->radius,
				                                               vertices[0], vertices[1], trimesh->edges[edgeIndex].triangleNormals[0], trimesh->edges[edgeIndex].triangleNormals[1], trimesh->edges[edgeIndex].planarTransform, trimesh->edges[edgeIndex].length,
				                                               &time, &normal);
				if (normal.y > 0x00000) {
					intersection = false;
				}
			}
			
			if (!intersection) {
				intersection = intersectSweptCylinderWallTrimeshEdge(lastBottomCapPosition, bottomCapPosition, capsule->radius, capsule->cylinderHeight,
				                                                     vertices[0], vertices[1], trimesh->edges[edgeIndex].triangleNormals[0], trimesh->edges[edgeIndex].triangleNormals[1], trimesh->edges[edgeIndex].length,
				                                                     &time, &normal);
			}
			
			if (intersection && time < bestTime) {
				bestTime = time;
				bestNormal = normal;
			}
		}
	}
	
	for (triangleIndex = 0; triangleIndex < trimesh->triangleCount; triangleIndex++) {
		vertices[0] = trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[0]].position;
		vertices[1] = trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[1]].position;
		vertices[2] = trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[2]].position;
		if (Box6x_intersectsBox6x(capsuleBounds, box6xEnclosing3Points(vertices[0], vertices[1], vertices[2]))) {
			intersection = false;
			if (trimesh->triangles[triangleIndex].normal.y >= 0x00000) {
				intersection = intersectSweptSphereTriangle(lastBottomCapPosition, bottomCapPosition, capsule->radius,
				                                            vertices[0], vertices[1], vertices[2], trimesh->triangles[triangleIndex].normal,
				                                            &time);
			}
			if (!intersection && trimesh->triangles[triangleIndex].normal.y <= 0x00000) {
				intersection = intersectSweptSphereTriangle(lastTopCapPosition, topCapPosition, capsule->radius,
				                                            vertices[0], vertices[1], vertices[2], trimesh->triangles[triangleIndex].normal,
				                                            &time);
			}
			
			if (intersection && time < bestTime) {
				bestTime = time;
				bestNormal = trimesh->triangles[triangleIndex].normal;
			}
		}
	}
	
	if (bestTime < FIXED_16_16_MAX) {
		returnIntersection(bestTime, bestNormal, Vector3x_subtract(capsule->position, capsule->lastPosition), VECTOR3x_ZERO, 0x00000);
	}
	
	return false;
}

bool intersectionHandler_trimesh_trimesh(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}
