#include "collision/CollisionBox3D.h"
#include "collision/CollisionCapsule.h"
#include "collision/CollisionCircle.h"
#include "collision/CollisionLine2D.h"
#include "collision/CollisionRect2D.h"
#include "collision/CollisionSphere.h"
#include "collision/CollisionStaticTrimesh.h"
#include "collision/StandardIntersectionHandlers.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

static CollisionRect2D initStationaryRect2D(Vector2x position, Vector2x size) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, position, size, 0x00000);
	return rect;
}

static CollisionRect2D initMovingRect2D(Vector2x lastPosition, Vector2x position, Vector2x size) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, lastPosition, size, 0x00000);
	CollisionRect2D_updatePosition(&rect, position);
	return rect;
}

static CollisionRect2D initResizingRect2D(Vector2x position, Vector2x lastSize, Vector2x size) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, position, lastSize, 0x00000);
	CollisionRect2D_updateSize(&rect, size);
	return rect;
}

static CollisionRect2D initStationaryRect2DWithSolidity(Vector2x position, Vector2x size, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, position, size, 0x00000);
	CollisionRect2D_setSolidity(&rect, solidLeft, solidRight, solidBottom, solidTop);
	return rect;
}

static CollisionRect2D initStationaryRect2DWithThickness(Vector2x position, Vector2x size, fixed16_16 thickness) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, position, size, thickness);
	return rect;
}

static CollisionRect2D initMovingRect2DWithSolidity(Vector2x lastPosition, Vector2x position, Vector2x size, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, lastPosition, size, 0x00000);
	CollisionRect2D_updatePosition(&rect, position);
	CollisionRect2D_setSolidity(&rect, solidLeft, solidRight, solidBottom, solidTop);
	return rect;
}

static CollisionCircle initStationaryCircle(Vector2x position, fixed16_16 radius) {
	CollisionCircle circle;
	
	CollisionCircle_init(&circle, NULL, NULL, position, radius);
	return circle;
}

static CollisionCircle initMovingCircle(Vector2x lastPosition, Vector2x position, fixed16_16 radius) {
	CollisionCircle circle;
	
	CollisionCircle_init(&circle, NULL, NULL, lastPosition, radius);
	CollisionCircle_updatePosition(&circle, position);
	return circle;
}

static CollisionBox3D initStationaryBox3D(Vector3x position, Vector3x size) {
	CollisionBox3D box;
	
	CollisionBox3D_init(&box, NULL, NULL, position, size, 0x00000);
	return box;
}

static CollisionBox3D initMovingBox3D(Vector3x lastPosition, Vector3x position, Vector3x size) {
	CollisionBox3D box;
	
	CollisionBox3D_init(&box, NULL, NULL, lastPosition, size, 0x00000);
	CollisionBox3D_updatePosition(&box, position);
	return box;
}

static CollisionBox3D initResizingBox3D(Vector3x position, Vector3x lastSize, Vector3x size) {
	CollisionBox3D box;
	
	CollisionBox3D_init(&box, NULL, NULL, position, lastSize, 0x00000);
	CollisionBox3D_updateSize(&box, size);
	return box;
}

static CollisionBox3D initStationaryBox3DWithSolidity(Vector3x position, Vector3x size, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop, bool solidBack, bool solidFront) {
	CollisionBox3D box;
	
	CollisionBox3D_init(&box, NULL, NULL, position, size, 0x00000);
	CollisionBox3D_setSolidity(&box, solidLeft, solidRight, solidBottom, solidTop, solidBack, solidFront);
	return box;
}

static CollisionBox3D initMovingBox3DWithSolidity(Vector3x lastPosition, Vector3x position, Vector3x size, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop, bool solidBack, bool solidFront) {
	CollisionBox3D box;
	
	CollisionBox3D_init(&box, NULL, NULL, lastPosition, size, 0x00000);
	CollisionBox3D_updatePosition(&box, position);
	CollisionBox3D_setSolidity(&box, solidLeft, solidRight, solidBottom, solidTop, solidBack, solidFront);
	return box;
}

static CollisionBox3D initStationaryBox3DWithThickness(Vector3x position, Vector3x size, fixed16_16 thickness) {
	CollisionBox3D box;
	
	CollisionBox3D_init(&box, NULL, NULL, position, size, thickness);
	return box;
}

static CollisionSphere initStationarySphere(Vector3x position, fixed16_16 radius) {
	CollisionSphere sphere;
	
	CollisionSphere_init(&sphere, NULL, NULL, position, radius);
	return sphere;
}

static CollisionSphere initMovingSphere(Vector3x lastPosition, Vector3x position, fixed16_16 radius) {
	CollisionSphere sphere;
	
	CollisionSphere_init(&sphere, NULL, NULL, lastPosition, radius);
	CollisionSphere_updatePosition(&sphere, position);
	return sphere;
}

static CollisionCapsule initStationaryCapsule(Vector3x position, fixed16_16 radius, fixed16_16 cylinderHeight) {
	CollisionCapsule capsule;
	
	CollisionCapsule_init(&capsule, NULL, NULL, position, radius, cylinderHeight);
	return capsule;
}

static CollisionCapsule initMovingCapsule(Vector3x lastPosition, Vector3x position, fixed16_16 radius, fixed16_16 cylinderHeight) {
	CollisionCapsule capsule;
	
	CollisionCapsule_init(&capsule, NULL, NULL, lastPosition, radius, cylinderHeight);
	CollisionCapsule_updatePosition(&capsule, position);
	return capsule;
}

static CollisionLine2D initStaticLine2D(Vector2x endpoint1, Vector2x endpoint2, bool doubleSided) {
	CollisionLine2D line;
	
	CollisionLine2D_init(&line, NULL, NULL, endpoint1, endpoint2, doubleSided);
	return line;
}

static CollisionLine2D initMovingLine2D(Vector2x lastEndpoint1, Vector2x lastEndpoint2, Vector2x endpoint1, Vector2x endpoint2, bool doubleSided) {
	CollisionLine2D line;
	
	CollisionLine2D_init(&line, NULL, NULL, lastEndpoint1, lastEndpoint2, doubleSided);
	CollisionLine2D_updateEndpoints(&line, endpoint1, endpoint2);
	return line;
}

#define assertNoCollision(result) \
	TestCase_assert(!result, "Expected no collision, but intersection handler returned true");

#define assertCollision(result, time, normal, expectedTime, expectedNormal, expectedObject1Vector, expectedObject2Vector, expectedContactArea) \
	TestCase_assert(result, "Expected collision, but intersection handler returned false"); \
	TestCase_assert(time == expectedTime, "Expected intersection time " FIXED_16_16_FORMAT ", but got " FIXED_16_16_FORMAT, expectedTime, time); \
	TestCase_assert(normal.x == expectedNormal.x && normal.y == expectedNormal.y && normal.z == expectedNormal.z, "Expected intersection normal {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "} but got {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "}", expectedNormal.x, expectedNormal.y, expectedNormal.z, normal.x, normal.y, normal.z); \
	TestCase_assert(object1Vector.x == expectedObject1Vector.x && object1Vector.y == expectedObject1Vector.y && object1Vector.z == expectedObject1Vector.z, "Expected object1 vector {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "} but got {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "}", expectedObject1Vector.x, expectedObject1Vector.y, expectedObject1Vector.z, object1Vector.x, object1Vector.y, object1Vector.z); \
	TestCase_assert(object2Vector.x == expectedObject2Vector.x && object2Vector.y == expectedObject2Vector.y && object2Vector.z == expectedObject2Vector.z, "Expected object2 vector {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "} but got {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "}", expectedObject2Vector.x, expectedObject2Vector.y, expectedObject2Vector.z, object2Vector.x, object2Vector.y, object2Vector.z); \
	TestCase_assert(contactArea == expectedContactArea, "Expected contact area " FIXED_16_16_FORMAT ", but got " FIXED_16_16_FORMAT, expectedContactArea, contactArea)

#define resetOutParameters() \
	time = -1; \
	memset(&normal, 0xFF, sizeof(normal)); \
	memset(&object1Vector, 0xFF, sizeof(object1Vector)); \
	memset(&object2Vector, 0xFF, sizeof(object2Vector)); \
	contactArea = -1

static void testRect2D_rect2D() {
	CollisionRect2D rect1, rect2;
	bool result;
	fixed16_16 time, contactArea;
	Vector3x normal, object1Vector, object2Vector;
	
	// No collision for no movement
	rect1 = initStationaryRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
#pragma mark Side collisions
	// rect1 moving +x, rect2 stationary
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving +x, rect2 stationary (different speed/collision time)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x20000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving +x, rect2 stationary (rect1 starts butted against rect2)
	rect1 = initMovingRect2D(VECTOR2x(-0x10000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving +x, rect2 stationary (rect1 starts barely inside rect2, so passes through)
	rect1 = initMovingRect2D(VECTOR2x(-0x0FFFF, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 stationary, rect2 moving -x
	rect1 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initMovingRect2D(VECTOR2x(0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x20000, 0x00000, 0x00000), 0x10000);
	
	// rect1 moving +x, rect2 moving -x
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initMovingRect2D(VECTOR2x(0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0C000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, 0x00000), 0x10000);
	
	// rect1 moving -x, rect2 stationary
	rect1 = initMovingRect2D(VECTOR2x(0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving +y, rect2 stationary
	rect1 = initMovingRect2D(VECTOR2x(0x00000, -0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving -y, rect2 stationary
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
#pragma mark Corner collisions
	// Corner collision (bottom left)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, -0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (top left)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, -0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (bottom right)
	rect1 = initMovingRect2D(VECTOR2x(0x20000, -0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (top right)
	rect1 = initMovingRect2D(VECTOR2x(0x20000, 0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, -0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
#pragma mark Outer non-collisions
	// Miss (left, above)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x10000), VECTOR2x(0x00000, 0x10000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (left, below)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, -0x10000), VECTOR2x(0x00000, -0x10000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (right, above)
	rect1 = initMovingRect2D(VECTOR2x(0x20000, 0x10000), VECTOR2x(0x00000, 0x10000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (right, below)
	rect1 = initMovingRect2D(VECTOR2x(0x20000, -0x10000), VECTOR2x(0x00000, -0x10000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (bottom, left)
	rect1 = initMovingRect2D(VECTOR2x(-0x10000, -0x20000), VECTOR2x(-0x10000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (bottom, right)
	rect1 = initMovingRect2D(VECTOR2x(0x10000, -0x20000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (top, left)
	rect1 = initMovingRect2D(VECTOR2x(-0x10000, 0x20000), VECTOR2x(-0x10000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (top, right)
	rect1 = initMovingRect2D(VECTOR2x(0x10000, 0x20000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Inner non-collisions (sides)
	// rect1 moving -x from inside rect2
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving +x from inside rect2
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x20000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving -y from inside rect2
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, -0x20000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving +y from inside rect2
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x20000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Inner non-collisions (corners)
	// Corner collision (bottom left, from inside)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x20000, -0x20000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (top left, from inside)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x20000, 0x20000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (bottom right, from inside)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x20000, -0x20000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (top right, from inside)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x20000, 0x20000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
#pragma mark Size-changing collisions
	// rect1 size increasing, rect2 stationary
	rect1 = initResizingRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x10000, 0x10000), VECTOR2x(0x30000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 stationary, rect2 size increasing
	rect1 = initStationaryRect2D(VECTOR2x(0x20000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initResizingRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), VECTOR2x(0x30000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x20000, 0x00000, 0x00000), 0x10000);
	
#pragma mark Solidity (sides)
	// rect1 moving +x, rect2 stationary (unsolid left)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2DWithSolidity(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false, true, true, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving +x (unsolid right), rect2 stationary
	rect1 = initMovingRect2DWithSolidity(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), true, false, true, true);
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving -x, rect2 stationary (unsolid right)
	rect1 = initMovingRect2D(VECTOR2x(0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2DWithSolidity(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), true, false, true, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving -x (unsolid left), rect2 stationary
	rect1 = initMovingRect2DWithSolidity(VECTOR2x(0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false, true, true, true);
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving +y, rect2 stationary (unsolid bottom)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, -0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2DWithSolidity(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), true, true, false, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving +y (unsolid top), rect2 stationary
	rect1 = initMovingRect2DWithSolidity(VECTOR2x(0x00000, -0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), true, true, true, false);
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving -y, rect2 stationary (unsolid top)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2DWithSolidity(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), true, true, true, false);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving -y (unsolid bottom), rect2 stationary
	rect1 = initMovingRect2DWithSolidity(VECTOR2x(0x00000, 0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), true, true, false, true);
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Solidity (corners)
	// Corner collision (bottom left, not solid)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, -0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2DWithSolidity(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false, false, false, false);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (top left, not solid)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2DWithSolidity(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false, false, false, false);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (bottom right, not solid)
	rect1 = initMovingRect2D(VECTOR2x(0x20000, -0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2DWithSolidity(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false, false, false, false);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (top right, not solid)
	rect1 = initMovingRect2D(VECTOR2x(0x20000, 0x20000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2DWithSolidity(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false, false, false, false);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Concavity
	// rect2 moving -x from inside concave rect1
	rect1 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	rect2 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(-0x18000, -0x08000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x10000, 0x00000, 0x00000), 0x10000);
	
	// rect2 moving +x from inside concave rect1
	rect1 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	rect2 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(0x08000, -0x08000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x10000, 0x00000, 0x00000), 0x10000);
	
	// rect1 moving -y from inside concave rect2
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(-0x08000, -0x18000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving +y from inside concave rect2
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(-0x08000, 0x08000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
#pragma mark Concave side priority
	// rect1 moving -x -y from inside concave rect2 (left collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(-0x18000, -0x17000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x10000, -0x0F000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving -x -y from inside concave rect2 (bottom collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(-0x17000, -0x18000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(-0x0F000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving +x -y from inside concave rect2 (right collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(0x08000, -0x17000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, -0x0F000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving +x -y from inside concave rect2 (bottom collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(0x07000, -0x18000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x0F000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving -x +y from inside concave rect2 (left collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(-0x18000, 0x07000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x10000, 0x0F000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving -x +y from inside concave rect2 (top collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(-0x17000, 0x08000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(-0x0F000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving +x +y from inside concave rect2 (right collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(0x08000, 0x07000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x0F000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// rect1 moving +x +y from inside concave rect2 (top collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x08000, -0x08000), VECTOR2x(0x07000, 0x08000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x0F000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
#pragma mark Contact area
	// rect1 moving +x at small size, rect2 stationary at normal size
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x08000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// rect1 moving +x at normal size, rect2 stationary at small size
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x06000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x06000);
	
	// rect1 moving +x at normal size, rect2 stationary at normal size (half contact bottom)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x08000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// rect1 moving +x at normal size, rect2 stationary at normal size (quarter contact top)
	rect1 = initMovingRect2D(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, -0x0C000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x04000);
	
	// rect1 resizing +x, rect2 stationary at normal size
	rect1 = initResizingRect2D(VECTOR2x(-0x08000, 0x00000), VECTOR2x(0x04000, 0x04000), VECTOR2x(0x0C000, 0x0C000));
	rect2 = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
#pragma mark Thickness
	// rect1 moving +x inside thin rect2
	rect1 = initMovingRect2D(VECTOR2x(-0x10000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), 0x00000);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving +x inside thick rect2 (collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x09000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x09000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// rect1 moving +x inside thick rect2 (no collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x07000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving +x inside default thickness rect2 (collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x09000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), EDGE_THICKNESS_DEFAULT);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x09000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// rect1 moving +x inside default thickness rect2 (no collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x07000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), EDGE_THICKNESS_DEFAULT);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving +x inside resized default thickness rect2 (collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x09000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x04000, 0x04000), EDGE_THICKNESS_DEFAULT);
	CollisionRect2D_updateSize(&rect2, VECTOR2x(0x20000, 0x20000));
	CollisionRect2D_updateSize(&rect2, VECTOR2x(0x20000, 0x20000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x09000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// rect1 moving +x inside resized default thickness rect2 (no collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x07000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x04000, 0x04000), EDGE_THICKNESS_DEFAULT);
	CollisionRect2D_updateSize(&rect2, VECTOR2x(0x20000, 0x20000));
	CollisionRect2D_updateSize(&rect2, VECTOR2x(0x20000, 0x20000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving -x inside thick rect2 (collision)
	rect1 = initMovingRect2D(VECTOR2x(0x01000, 0x00000), VECTOR2x(-0x08000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x09000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// rect1 moving -x inside thick rect2 (no collision)
	rect1 = initMovingRect2D(VECTOR2x(-0x01000, 0x00000), VECTOR2x(-0x08000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving +y inside thick rect2 (collision)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, -0x09000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x09000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// rect1 moving +y inside thick rect2 (no collision)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, -0x07000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// rect1 moving -y inside thick rect2 (collision)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, 0x01000), VECTOR2x(0x00000, -0x08000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x09000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// rect1 moving -y inside thick rect2 (no collision)
	rect1 = initMovingRect2D(VECTOR2x(0x00000, -0x01000), VECTOR2x(0x00000, -0x08000), VECTOR2x(0x08000, 0x08000));
	rect2 = initStationaryRect2DWithThickness(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) &rect1, (CollisionObject *) &rect2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
}

static void testRect2D_circle() {
	CollisionRect2D rect;
	CollisionCircle circle;
	bool result;
	fixed16_16 time, contactArea;
	Vector3x normal, object1Vector, object2Vector;
	
	// No collision for no movement
	rect = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = initStationaryCircle(VECTOR2x(-0x20000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
#pragma mark Side collisions
	// circle moving +x, rect stationary
	rect = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = initMovingCircle(VECTOR2x(-0x20000, 0x08000), VECTOR2x(0x00000, 0x08000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x20000, 0x00000, 0x00000), 0x00000);
	
	// circle moving +x, rect stationary (different speed/collision time)
	rect = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = initMovingCircle(VECTOR2x(-0x20000, 0x08000), VECTOR2x(0x20000, 0x08000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x40000, 0x00000, 0x00000), 0x00000);
	
	// circle moving -x, rect stationary
	rect = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = initMovingCircle(VECTOR2x(0x30000, 0x08000), VECTOR2x(0x10000, 0x08000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x20000, 0x00000, 0x00000), 0x00000);
	
	// circle moving +y, rect stationary
	rect = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = initMovingCircle(VECTOR2x(0x08000, -0x20000), VECTOR2x(0x08000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x00000, 0x20000, 0x00000), 0x00000);
	
	// circle moving -y, rect stationary
	rect = initStationaryRect2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = initMovingCircle(VECTOR2x(0x08000, 0x30000), VECTOR2x(0x08000, 0x10000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x00000, -0x20000, 0x00000), 0x00000);
	
	// rect moving +x, circle stationary
	rect = initMovingRect2D(VECTOR2x(-0x30000, -0x08000), VECTOR2x(-0x10000, -0x08000), VECTOR2x(0x10000, 0x10000));
	circle = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// rect moving -x, circle stationary
	rect = initMovingRect2D(VECTOR2x(0x20000, -0x08000), VECTOR2x(0x00000, -0x08000), VECTOR2x(0x10000, 0x10000));
	circle = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// rect moving +y, circle stationary
	rect = initMovingRect2D(VECTOR2x(-0x08000, -0x30000), VECTOR2x(-0x08000, -0x10000), VECTOR2x(0x10000, 0x10000));
	circle = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// rect moving -y, circle stationary
	rect = initMovingRect2D(VECTOR2x(-0x08000, 0x20000), VECTOR2x(-0x08000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
#pragma mark Corner collisions
	// rect bottom left corner collision
	rect = initMovingRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04AFB, VECTOR3x(0x0B505, 0x0B505, 0x00000), VECTOR3x(-0x10000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// rect bottom right corner collision
	rect = initMovingRect2D(VECTOR2x(-0x20000, 0x10000), VECTOR2x(-0x10000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04AFB, VECTOR3x(-0x0B505, 0x0B505, 0x00000), VECTOR3x(0x10000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// rect top left corner collision
	rect = initMovingRect2D(VECTOR2x(0x10000, -0x20000), VECTOR2x(0x00000, -0x10000), VECTOR2x(0x10000, 0x10000));
	circle = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04AFB, VECTOR3x(0x0B505, -0x0B505, 0x00000), VECTOR3x(-0x10000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// rect top right corner collision
	rect = initMovingRect2D(VECTOR2x(-0x20000, -0x20000), VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x10000, 0x10000));
	circle = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04AFB, VECTOR3x(-0x0B505, -0x0B505, 0x00000), VECTOR3x(0x10000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
#pragma mark Inner non-collisions (corner)
	// circle inside rect moving -x -y (miss inner bottom left corner)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x20000, -0x20000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle inside rect moving +x -y (miss inner bottom right corner)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x20000, -0x20000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle inside rect moving -x -y (miss inner top left corner)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle inside rect moving +x -y (miss inner top right corner)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x20000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// SOLIDITY BEHAVIOR:
	// Edge collisions with unsolid side pass through
	// Corner collisions with corner with neither side solid pass through
	// Corner collisions with corner with one solid side collide *only* if dot product is positive between collision normal and solid side's normal
	
#pragma mark Solidity (sides)
	// Miss unsolid rect left
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), false, true, true, true);
	circle = initMovingCircle(VECTOR2x(-0x30000, 0x00000), VECTOR2x(-0x18000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss unsolid rect right
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, false, true, true);
	circle = initMovingCircle(VECTOR2x(0x30000, 0x00000), VECTOR2x(0x18000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss unsolid rect bottom
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, false, true);
	circle = initMovingCircle(VECTOR2x(0x00000, -0x30000), VECTOR2x(0x00000, -0x08000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss unsolid rect top
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, true, false);
	circle = initMovingCircle(VECTOR2x(0x00000, 0x30000), VECTOR2x(0x00000, 0x08000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Solidity (corners)
	// Miss unsolid rect bottom left corner
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), false, true, false, true);
	circle = initMovingCircle(VECTOR2x(-0x30000, -0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss unsolid rect bottom right corner
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, false, false, true);
	circle = initMovingCircle(VECTOR2x(0x30000, -0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss unsolid rect top left corner
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), false, true, true, false);
	circle = initMovingCircle(VECTOR2x(-0x30000, 0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss unsolid rect top right corner
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, false, true, false);
	circle = initMovingCircle(VECTOR2x(0x30000, 0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Partial corner solidity (bottom left)
	// Hit left-unsolid rect bottom left corner (positive dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), false, true, true, true);
	circle = initMovingCircle(VECTOR2x(-0x30000, -0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x06E53, VECTOR3x(0x0B506, 0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x30000, 0x30000, 0x00000), 0x00000);
	
	// Miss left-unsolid rect bottom left corner (negative dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), false, true, true, true);
	circle = initMovingCircle(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, -0x20000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Hit bottom-unsolid rect bottom left corner (positive dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, false, true);
	circle = initMovingCircle(VECTOR2x(-0x30000, -0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x06E53, VECTOR3x(0x0B506, 0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x30000, 0x30000, 0x00000), 0x00000);
	
	// Miss bottom-unsolid rect bottom left corner (negative dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, false, true);
	circle = initMovingCircle(VECTOR2x(0x00000, -0x20000), VECTOR2x(-0x20000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Partial corner solidity (bottom right)
	// Hit right-unsolid rect bottom right corner (positive dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, false, true, true);
	circle = initMovingCircle(VECTOR2x(0x30000, -0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x06E53, VECTOR3x(-0x0B506, 0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x30000, 0x30000, 0x00000), 0x00000);
	
	// Miss right-unsolid rect bottom right corner (negative dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, false, true, true);
	circle = initMovingCircle(VECTOR2x(0x20000, 0x00000), VECTOR2x(0x00000, -0x20000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Hit bottom-unsolid rect bottom right corner (positive dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, false, true);
	circle = initMovingCircle(VECTOR2x(0x30000, -0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x06E53, VECTOR3x(-0x0B506, 0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x30000, 0x30000, 0x00000), 0x00000);
	
	// Miss bottom-unsolid rect bottom right corner (negative dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, false, true);
	circle = initMovingCircle(VECTOR2x(0x00000, -0x20000), VECTOR2x(0x20000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Partial corner solidity (top left)
	// Hit left-unsolid rect top left corner (positive dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), false, true, true, true);
	circle = initMovingCircle(VECTOR2x(-0x30000, 0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x06E53, VECTOR3x(0x0B506, -0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x30000, -0x30000, 0x00000), 0x00000);
	
	// Miss left-unsolid rect top left corner (negative dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), false, true, true, true);
	circle = initMovingCircle(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Hit bottom-unsolid rect top left corner (positive dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, true, false);
	circle = initMovingCircle(VECTOR2x(-0x30000, 0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x06E53, VECTOR3x(0x0B506, -0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x30000, -0x30000, 0x00000), 0x00000);
	
	// Miss bottom-unsolid rect top left corner (negative dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, true, false);
	circle = initMovingCircle(VECTOR2x(0x00000, 0x20000), VECTOR2x(-0x20000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Partial corner solidity (top right)
	// Hit right-unsolid rect top right corner (positive dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, false, true, true);
	circle = initMovingCircle(VECTOR2x(0x30000, 0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x06E53, VECTOR3x(-0x0B506, -0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x30000, -0x30000, 0x00000), 0x00000);
	
	// Miss right-unsolid rect top right corner (negative dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, false, true, true);
	circle = initMovingCircle(VECTOR2x(0x20000, 0x00000), VECTOR2x(0x00000, 0x20000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Hit bottom-unsolid rect top right corner (positive dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, true, false);
	circle = initMovingCircle(VECTOR2x(0x30000, 0x30000), VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x06E53, VECTOR3x(-0x0B506, -0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x30000, -0x30000, 0x00000), 0x00000);
	
	// Miss bottom-unsolid rect top right corner (negative dot product)
	rect = initStationaryRect2DWithSolidity(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000), true, true, true, false);
	circle = initMovingCircle(VECTOR2x(0x00000, 0x20000), VECTOR2x(0x20000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Concavity (rect)
	// circle moving -x from inside concave rect
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x10000, 0x00000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x10000, 0x00000, 0x00000), 0x00000);
	
	// circle moving +x from inside concave rect
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x10000, 0x00000, 0x00000), 0x00000);
	
	// circle moving -y from inside concave rect
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, -0x10000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x00000, -0x10000, 0x00000), 0x00000);
	
	// circle moving +y from inside concave rect
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x10000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x00000, 0x10000, 0x00000), 0x00000);
	
#pragma mark Concave side priority
	// circle moving -x -y from inside concave rect (left collision)
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x10000, -0x0F000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x10000, -0x0F000, 0x00000), 0x00000);
	
	// circle moving -x -y from inside concave rect (bottom collision)
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x0F000, -0x10000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x0F000, -0x10000, 0x00000), 0x00000);
	
	// circle moving +x -y from inside concave rect (right collision)
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, -0x0F000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x10000, -0x0F000, 0x00000), 0x00000);
	
	// circle moving +x -y from inside concave rect (bottom collision)
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x0F000, -0x10000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x0F000, -0x10000, 0x00000), 0x00000);
	
	// circle moving -x +y from inside concave rect (left collision)
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x10000, 0x0F000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x10000, 0x0F000, 0x00000), 0x00000);
	
	// circle moving -x +y from inside concave rect (top collision)
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(-0x0F000, 0x10000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x0F000, 0x10000, 0x00000), 0x00000);
	
	// circle moving +x +y from inside concave rect (right collision)
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x0F000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x10000, 0x0F000, 0x00000), 0x00000);
	
	// circle moving +x +y from inside concave rect (top collision)
	rect = initStationaryRect2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, -0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x0F000, 0x10000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x0F000, 0x10000, 0x00000), 0x00000);
	
#pragma mark Inner solidity (edges)
	// circle moving +x partially inside rect (less than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(-0x14000, 0x00000), VECTOR2x(-0x04000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x10000, 0x00000, 0x00000), 0x00000);
	
	// circle moving +x partially inside rect (more than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(-0x0C000, 0x00000), VECTOR2x(0x04000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle moving -x partially inside rect (less than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x14000, 0x00000), VECTOR2x(0x04000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x10000, 0x00000, 0x00000), 0x00000);
	
	// circle moving -x partially inside rect (more than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x0C000, 0x00000), VECTOR2x(-0x04000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle moving +y partially inside rect (less than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, -0x14000), VECTOR2x(0x00000, -0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x00000, 0x10000, 0x00000), 0x00000);
	
	// circle moving +y partially inside rect (more than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, -0x0C000), VECTOR2x(0x00000, 0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle moving -y partially inside rect (less than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x14000), VECTOR2x(0x00000, 0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x00000, -0x10000, 0x00000), 0x00000);
	
	// circle moving -y partially inside rect (more than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x00000, 0x0C000), VECTOR2x(0x00000, -0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Inner solidity (corners)
	// circle moving +x +y partially inside rect (less than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(-0x14000, -0x14000), VECTOR2x(-0x04000, -0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x0B506, 0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x10000, 0x10000, 0x00000), 0x00000);
	
	// circle moving +x +y partially inside rect (more than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(-0x0C000, -0x0C000), VECTOR2x(0x04000, 0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle moving -x +y partially inside rect (less than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x14000, -0x14000), VECTOR2x(0x04000, -0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x0B506, 0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x10000, 0x10000, 0x00000), 0x00000);
	
	// circle moving -x +y partially inside rect (more than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x0C000, -0x0C000), VECTOR2x(-0x04000, 0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle moving +x -y partially inside rect (less than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(-0x14000, 0x14000), VECTOR2x(-0x04000, 0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x0B506, -0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x10000, -0x10000, 0x00000), 0x00000);
	
	// circle moving +x -y partially inside rect (more than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(-0x0C000, 0x0C000), VECTOR2x(0x04000, -0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle moving -x -y partially inside rect (less than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x14000, 0x14000), VECTOR2x(0x04000, 0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x0B506, -0x0B506, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x10000, -0x10000, 0x00000), 0x00000);
	
	// circle moving -x -y partially inside rect (more than half)
	rect = initStationaryRect2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, 0x20000));
	circle = initMovingCircle(VECTOR2x(0x0C000, 0x0C000), VECTOR2x(-0x04000, -0x04000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_rect2D_circle((CollisionObject *) &rect, (CollisionObject *) &circle, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
}

static void testRect2D_line2D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testRect2D_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCircle_circle() {
	CollisionCircle circle1, circle2;
	bool result;
	fixed16_16 time, contactArea;
	Vector3x normal, object1Vector, object2Vector;
	
	// No collision for no movement (no contact)
	circle1 = initStationaryCircle(VECTOR2x(-0x40000, 0x00000), 0x10000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
	// circle1 moving +x (level), circle2 stationary
	circle1 = initMovingCircle(VECTOR2x(-0x40000, 0x00000), VECTOR2x(0x00000, 0x00000), 0x10000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// circle1 moving +x (level), circle2 stationary (different speed/collision time)
	circle1 = initMovingCircle(VECTOR2x(-0x40000, 0x00000), VECTOR2x(0x40000, 0x00000), 0x10000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x80000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// circle1 moving +x (level), circle2 stationary (different radius)
	circle1 = initMovingCircle(VECTOR2x(-0x40000, 0x00000), VECTOR2x(0x00000, 0x00000), 0x10000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x20000);
	resetOutParameters();
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// circle1 moving +x (level), circle2 stationary (circle1 starts butted against circle2)
	circle1 = initMovingCircle(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x00000, 0x00000), 0x10000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// circle1 moving +x (45 degrees up), circle2 stationary
	circle1 = initMovingCircle(VECTOR2x(-0x40000, 0x16A09), VECTOR2x(0x00000, 0x16A09), 0x10000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0A57D, VECTOR3x(-0x0B506, 0x0B505, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// circle1 moving -y, circle2 moving +x
	circle1 = initMovingCircle(VECTOR2x(0x00000, 0x20000), VECTOR2x(0x00000, -0x20000), 0x10000);
	circle2 = initMovingCircle(VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x20000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0257D, VECTOR3x(0x0B505, 0x0B505, 0x00000), VECTOR3x(0x00000, -0x40000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), 0x00000);
	
	// No collision for no movement (contact)
	circle1 = initStationaryCircle(VECTOR2x(-0x10000, 0x00000), 0x10000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
	// Miss (circle1 above moving +x, circle2 below moving -x)
	circle1 = initMovingCircle(VECTOR2x(-0x10000, 0x10000), VECTOR2x(0x10000, 0x10000), 0x10000);
	circle2 = initMovingCircle(VECTOR2x(0x10000, -0x10000), VECTOR2x(-0x10000, -0x10000), 0x10000);
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// circle1 moving +x (level), circle2 stationary (circle1 starts penetrating circle2 by less than half)
	circle1 = initMovingCircle(VECTOR2x(-0x08000, 0x00000), VECTOR2x(0x00000, 0x00000), 0x10000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// circle1 moving +x (level), circle2 stationary (circle1 starts penetrating circle2 by more than half)
	circle1 = initMovingCircle(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x08000, 0x00000), 0x10000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
}

static void testCircle_circle_farawayBug()  {
	// The limits of fixed point precision and range can cause circles far away from each other moving at certain angles
	// to give false positives (or false negatives) if not accounted for. Each test case here uses real numbers from a
	// false positive (or negative) that occurred.
	CollisionCircle circle1, circle2;
	bool result;
	
	// Inadequate numeric precision in quadraticFormula
	circle1 = initMovingCircle(VECTOR2x(0xF7047, 0x47479), VECTOR2x(0xF6EE9, 0x003E0), 0x08000);
	circle2 = initStationaryCircle(VECTOR2x(0xFFF2CBEC, 0xFFFFDD8E), 0x10000);
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
	// Inadequate numeric precision in quadraticFormula
	circle1 = initMovingCircle(VECTOR2x(0x3060A, 0x69C85), VECTOR2x(0x30751, 0xFFF6F20C), 0x08000);
	circle2 = initStationaryCircle(VECTOR2x(0xFFFD0000, 0x00000), 0x10000);
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
	// Inadequate numeric range in quadraticFormula
	circle1 = initMovingCircle(VECTOR2x(-0x80000, 0x00000), VECTOR2x(0x80000, 0x00000), 0x08000);
	circle2 = initStationaryCircle(VECTOR2x(0x00000, 0x00000), 0x10000);
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, NULL, NULL, NULL, NULL, NULL);
	TestCase_assert(result, "Expected true but got false");
	
	// Inadequate numeric precision in calculations for arguments to quadraticFormula
	circle1 = initMovingCircle(VECTOR2x(0x31744, 0xFFF5DD0C), VECTOR2x(0x317C8, 0xFFF5DDA3), 0x0D2E8);
	circle2 = initStationaryCircle(VECTOR2x(-0x30000, 0x00000), 0x10000);
	result = intersectionHandler_circle_circle((CollisionObject *) &circle1, (CollisionObject *) &circle2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
}

static void testCircle_line2D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCircle_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine2D_line2D() {
	CollisionLine2D line1, line2;
	bool result;
	fixed16_16 time, contactArea;
	Vector3x normal, object1Vector, object2Vector;
	
	return;
	// vertical line1 moving +x toward static vertical line2 (both forward, single sided)
	line1 = initMovingLine2D(VECTOR2x(0x00000, 0x10000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x20000, 0x10000), VECTOR2x(0x20000, 0x00000), false);
	line2 = initStaticLine2D(VECTOR2x(0x10000, 0x00000), VECTOR2x(0x10000, 0x10000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// vertical line1 moving +x toward static vertical line2 (line1 backward, single sided)
	line1 = initMovingLine2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x10000), VECTOR2x(0x20000, 0x00000), VECTOR2x(0x20000, 0x10000), false);
	line2 = initStaticLine2D(VECTOR2x(0x10000, 0x00000), VECTOR2x(0x10000, 0x10000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// vertical line1 moving +x toward static vertical line2 (line2 backward, single sided)
	line1 = initMovingLine2D(VECTOR2x(0x00000, 0x10000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x20000, 0x10000), VECTOR2x(0x20000, 0x00000), false);
	line2 = initStaticLine2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(0x10000, 0x00000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// vertical line1 moving +x toward static vertical line2 (both backward, single sided)
	line1 = initMovingLine2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x10000), VECTOR2x(0x20000, 0x00000), VECTOR2x(0x20000, 0x10000), false);
	line2 = initStaticLine2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(0x10000, 0x00000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// vertical line1 moving +x toward static vertical line2 (both forward, both double sided)
	line1 = initMovingLine2D(VECTOR2x(0x00000, 0x10000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x20000, 0x10000), VECTOR2x(0x20000, 0x00000), true);
	line2 = initStaticLine2D(VECTOR2x(0x10000, 0x00000), VECTOR2x(0x10000, 0x10000), true);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// vertical line1 moving +x toward static vertical line2 (line1 backward, double sided)
	line1 = initMovingLine2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x10000), VECTOR2x(0x20000, 0x00000), VECTOR2x(0x20000, 0x10000), true);
	line2 = initStaticLine2D(VECTOR2x(0x10000, 0x00000), VECTOR2x(0x10000, 0x10000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// vertical line1 moving +x toward static vertical line2 (line2 backward, double sided)
	line1 = initMovingLine2D(VECTOR2x(0x00000, 0x10000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x20000, 0x10000), VECTOR2x(0x20000, 0x00000), false);
	line2 = initStaticLine2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(0x10000, 0x00000), true);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// vertical line1 moving +x toward static vertical line2 (both backward, both double sided)
	line1 = initMovingLine2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x10000), VECTOR2x(0x20000, 0x00000), VECTOR2x(0x20000, 0x10000), true);
	line2 = initStaticLine2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(0x10000, 0x00000), true);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// diagonal line1 moving -y toward static horizontal line2 (line1 endpoint1 collides with line2 surface)
	line1 = initMovingLine2D(VECTOR2x(0x08000, 0x08000), VECTOR2x(0x00000, 0x10000), VECTOR2x(0x08000, -0x08000), VECTOR2x(0x00000, 0x00000), false);
	line2 = initStaticLine2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// diagonal line1 moving -y toward static horizontal line2 (line1 endpoint2 collides with line2 surface)
	line1 = initMovingLine2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(0x08000, 0x08000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x08000, -0x08000), false);
	line2 = initStaticLine2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// diagonal line2 moving -y toward static horizontal line1 (line2 endpoint1 collides with line1 surface)
	line1 = initStaticLine2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), false);
	line2 = initMovingLine2D(VECTOR2x(0x08000, 0x08000), VECTOR2x(0x00000, 0x10000), VECTOR2x(0x08000, -0x08000), VECTOR2x(0x00000, 0x00000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x00000, 0x10000, 0x00000), 0x00000);
	
	// diagonal line2 moving -y toward static horizontal line1 (line2 endpoint2 collides with line1 surface)
	line1 = initStaticLine2D(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), false);
	line2 = initMovingLine2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(0x08000, 0x08000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x08000, -0x08000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x00000, 0x10000, 0x00000), 0x00000);
	
	// line1 and line2 rotating through each other from horizontal to vertical
	line1 = initMovingLine2D(VECTOR2x(-0x10000, -0x10000), VECTOR2x(0x20000, -0x10000), VECTOR2x(-0x10000, -0x10000), VECTOR2x(-0x10000, 0x20000), false);
	line2 = initMovingLine2D(VECTOR2x(0x10000, 0x10000), VECTOR2x(-0x20000, 0x10000), VECTOR2x(0x10000, 0x10000), VECTOR2x(0x10000, -0x20000), false);
	resetOutParameters();
	result = intersectionHandler_line2D_line2D((CollisionObject *) &line1, (CollisionObject *) &line2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x00000, 0x10000, 0x00000), 0x00000);
	
	TestCase_assert(false, "Unimplemented");
}

static void testLine2D_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testPolygon_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testBox_box() {
	CollisionBox3D box1, box2;
	bool result;
	fixed16_16 time, contactArea;
	Vector3x normal, object1Vector, object2Vector;
	
	// No collision for no movement
	box1 = initStationaryBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
#pragma mark Side collisions
	// box1 moving +x, box2 stationary
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x, box2 stationary (different speed/collision time)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x, box2 stationary (box1 starts butted against box2)
	box1 = initMovingBox3D(VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x, box2 stationary (box1 starts barely inside box2, so passes through)
	box1 = initMovingBox3D(VECTOR3x(-0x0FFFF, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 stationary, box2 moving -x
	box1 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initMovingBox3D(VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x20000, 0x00000, 0x00000), 0x10000);
	
	// box1 moving +x, box2 moving -x
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initMovingBox3D(VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0C000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, 0x00000), 0x10000);
	
	// box1 moving -x, box2 stationary
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +y, box2 stationary
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x20000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -y, box2 stationary
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +z, box2 stationary
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x00000, 0x20000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -z, box2 stationary
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x00000, 0x00000, -0x20000), VECTOR3x_ZERO, 0x10000);
	
#pragma mark Edge collisions
	// Edge collision (bottom left)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, -0x20000, 0x08000), VECTOR3x(0x00000, 0x00000, 0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (top left)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x20000, 0x08000), VECTOR3x(0x00000, 0x00000, 0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, -0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (bottom right)
	box1 = initMovingBox3D(VECTOR3x(0x20000, -0x20000, 0x08000), VECTOR3x(0x00000, 0x00000, 0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (top right)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x20000, 0x08000), VECTOR3x(0x00000, 0x00000, 0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, -0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (back left)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x08000, -0x20000), VECTOR3x(0x00000, 0x08000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (front left)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x08000, 0x20000), VECTOR3x(0x00000, 0x08000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, -0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (back right)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x08000, -0x20000), VECTOR3x(0x00000, 0x08000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, 0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (front right)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x08000, 0x20000), VECTOR3x(0x00000, 0x08000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, -0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (back bottom)
	box1 = initMovingBox3D(VECTOR3x(0x08000, -0x20000, -0x20000), VECTOR3x(0x08000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x20000, 0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (front bottom)
	box1 = initMovingBox3D(VECTOR3x(0x08000, -0x20000, 0x20000), VECTOR3x(0x08000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x20000, -0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (back top)
	box1 = initMovingBox3D(VECTOR3x(0x08000, 0x20000, -0x20000), VECTOR3x(0x08000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x00000, -0x20000, 0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Edge collision (front top)
	box1 = initMovingBox3D(VECTOR3x(0x08000, 0x20000, 0x20000), VECTOR3x(0x08000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x00000, -0x20000, -0x20000), VECTOR3x_ZERO, 0x00000);
	
#pragma mark Corner collisions
	// Corner collision (back bottom left)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, -0x20000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x20000, 0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (back top left)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x20000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, -0x20000, 0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (back bottom right)
	box1 = initMovingBox3D(VECTOR3x(0x20000, -0x20000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x20000, 0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (back top right)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x20000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, -0x20000, 0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (front bottom left)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, -0x20000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x20000, -0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (front top left)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x20000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, -0x20000, -0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (front bottom right)
	box1 = initMovingBox3D(VECTOR3x(0x20000, -0x20000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x20000, -0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Corner collision (front top right)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x20000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, -0x20000, -0x20000), VECTOR3x_ZERO, 0x00000);
	
#pragma mark Outer non-collisions
	// Miss (left, above)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x10000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (left, below)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, -0x10000, 0x00000), VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x0000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (left, in front)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x10000), VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x0000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (left, behind)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (right, above)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x10000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (right, below)
	box1 = initMovingBox3D(VECTOR3x(0x20000, -0x10000, 0x00000), VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (right, in front)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x00000, 0x10000), VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (right, behind)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (bottom, right)
	box1 = initMovingBox3D(VECTOR3x(0x10000, -0x20000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (bottom, left)
	box1 = initMovingBox3D(VECTOR3x(-0x10000, -0x20000, 0x00000), VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (bottom, in front)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x20000, 0x10000), VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (bottom, behind)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x20000, -0x10000), VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (top, right)
	box1 = initMovingBox3D(VECTOR3x(0x10000, 0x20000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (top, left)
	box1 = initMovingBox3D(VECTOR3x(-0x10000, 0x20000, 0x00000), VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (top, in front)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x20000, 0x10000), VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (top, behind)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x20000, -0x10000), VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (back, right)
	box1 = initMovingBox3D(VECTOR3x(0x10000, 0x00000, -0x20000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (back, left)
	box1 = initMovingBox3D(VECTOR3x(-0x10000, 0x00000, -0x20000), VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (back, above)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x10000, -0x20000), VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (back, below)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x10000, -0x20000), VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (front, right)
	box1 = initMovingBox3D(VECTOR3x(0x10000, 0x00000, 0x20000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (front, left)
	box1 = initMovingBox3D(VECTOR3x(-0x10000, 0x00000, 0x20000), VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (front, above)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x10000, 0x20000), VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Miss (front, below)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x10000, 0x20000), VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Inner non-collisions (sides)
	// box1 moving -x from inside box2
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +x from inside box2
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -y from inside box2
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +y from inside box2
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -z from inside box2
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, -0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +z from inside box2
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Inner non-collisions (edges)
	// Edge collision (bottom left, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(-0x20000, -0x20000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (top left, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x20000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (bottom right, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x20000, -0x20000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (top right, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x20000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (back left, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, -0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (front left, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, 0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (back right, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, -0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (front right, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (back bottom, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, -0x20000, -0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (front bottom, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (back top, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x20000, -0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Edge collision (front top, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x20000, 0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
#pragma mark Inner non-collisions (corners)
	// Corner collision (back bottom left, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(-0x20000, -0x20000, -0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (back top left, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x20000, -0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (back bottom right, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x20000, -0x20000, -0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (back top right, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x20000, -0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (front bottom left, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(-0x20000, -0x20000, 0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (front top left, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x20000, 0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (front bottom right, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x20000, -0x20000, 0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (front top right, from inside)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x20000, 0x20000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	TestCase_assert(!result, "Expected true but got false");
	
#pragma mark Size-changing collisions
	// box1 size increasing, box2 stationary
	box1 = initResizingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(0x30000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 stationary, box2 size increasing
	box1 = initStationaryBox3D(VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initResizingBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(0x30000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x20000, 0x00000, 0x00000), 0x10000);
	
#pragma mark Solidity (sides)
	// box1 moving +x, box2 stationary (unsolid left)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, true, true, true, true, true);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +x (unsolid right), box2 stationary
	box1 = initMovingBox3DWithSolidity(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, false, true, true, true, true);
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -x, box2 stationary (unsolid right)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, false, true, true, true, true);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -x (unsolid left), box2 stationary
	box1 = initMovingBox3DWithSolidity(VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, true, true, true, true, true);
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +y, box2 stationary (unsolid bottom)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x20000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, true, false, true, true, true);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +y (unsolid top), box2 stationary
	box1 = initMovingBox3DWithSolidity(VECTOR3x(0x00000, -0x20000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, true, true, false, true, true);
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -y, box2 stationary (unsolid top)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, true, true, false, true, true);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -y (unsolid bottom), box2 stationary
	box1 = initMovingBox3DWithSolidity(VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, true, false, true, true, true);
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +z, box2 stationary (unsolid back)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, true, true, true, false, true);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +z (unsolid front), box2 stationary
	box1 = initMovingBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, true, true, true, true, false);
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -z, box2 stationary (unsolid front)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, true, true, true, true, false);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -z (unsolid back), box2 stationary
	box1 = initMovingBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), true, true, true, true, false, true);
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Solidity (corners)
	// Corner collision (back bottom left, not solid)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, -0x20000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, false, false, false, false, false);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (back top left, not solid)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x20000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, false, false, false, false, false);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (back bottom right, not solid)
	box1 = initMovingBox3D(VECTOR3x(0x20000, -0x20000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, false, false, false, false, false);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (back top right, not solid)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x20000, -0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, false, false, false, false, false);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (front bottom left, not solid)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, -0x20000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, false, false, false, false, false);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (front top left, not solid)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x20000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, false, false, false, false, false);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (front bottom right, not solid)
	box1 = initMovingBox3D(VECTOR3x(0x20000, -0x20000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, false, false, false, false, false);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Corner collision (front top right, not solid)
	box1 = initMovingBox3D(VECTOR3x(0x20000, 0x20000, 0x20000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3DWithSolidity(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), false, false, false, false, false, false);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
#pragma mark Concavity
	// box2 moving -x from inside concave box1
	box1 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	box2 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, -0x08000), VECTOR3x(-0x18000, -0x08000, -0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(-0x10000, 0x00000, 0x00000), 0x10000);
	
	// box2 moving +x from inside concave box1
	box1 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	box2 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, -0x08000), VECTOR3x(0x08000, -0x08000, -0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, VECTOR3x(0x10000, 0x00000, 0x00000), 0x10000);
	
	// box1 moving -y from inside concave box2
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, -0x08000), VECTOR3x(-0x08000, -0x18000, -0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +y from inside concave box2
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, -0x08000), VECTOR3x(-0x08000, 0x08000, -0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -z from inside concave box2
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, -0x08000), VECTOR3x(-0x08000, -0x08000, -0x18000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +z from inside concave box2
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, -0x08000), VECTOR3x(-0x08000, -0x08000, 0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x_ZERO, 0x10000);
	
#pragma mark Concave side priority
	// box1 moving -x -y from inside concave box2 (left collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, 0x00000), VECTOR3x(-0x18000, -0x17000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x10000, -0x0F000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -x -y from inside concave box2 (bottom collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, 0x00000), VECTOR3x(-0x17000, -0x18000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(-0x0F000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x -y from inside concave box2 (right collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, 0x00000), VECTOR3x(0x08000, -0x17000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, -0x0F000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x -y from inside concave box2 (bottom collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, 0x00000), VECTOR3x(0x07000, -0x18000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x0F000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -x +y from inside concave box2 (left collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, 0x00000), VECTOR3x(-0x18000, 0x07000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x10000, 0x0F000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -x +y from inside concave box2 (top collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, 0x00000), VECTOR3x(-0x17000, 0x08000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(-0x0F000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x +y from inside concave box2 (right collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, 0x00000), VECTOR3x(0x08000, 0x07000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x0F000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x +y from inside concave box2 (top collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, -0x08000, 0x00000), VECTOR3x(0x07000, 0x08000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x0F000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -x -z from inside concave box2 (left collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, 0x00000, -0x08000), VECTOR3x(-0x18000, 0x00000, -0x17000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x10000, 0x00000, -0x0F000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -x -z from inside concave box2 (back collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, 0x00000, -0x08000), VECTOR3x(-0x17000, 0x00000, -0x18000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(-0x0F000, 0x00000, -0x10000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x -z from inside concave box2 (right collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, 0x00000, -0x08000), VECTOR3x(0x08000, 0x00000, -0x17000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, -0x0F000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x -z from inside concave box2 (back collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, 0x00000, -0x08000), VECTOR3x(0x07000, 0x00000, -0x18000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x0F000, 0x00000, -0x10000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -x +z from inside concave box2 (left collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, 0x00000, -0x08000), VECTOR3x(-0x18000, 0x00000, 0x07000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x10000, 0x00000, 0x0F000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -x +z from inside concave box2 (front collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, 0x00000, -0x08000), VECTOR3x(-0x17000, 0x00000, 0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(-0x0F000, 0x00000, 0x10000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x +z from inside concave box2 (right collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, 0x00000, -0x08000), VECTOR3x(0x08000, 0x00000, 0x07000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x0F000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +x +z from inside concave box2 (front collision)
	box1 = initMovingBox3D(VECTOR3x(-0x08000, 0x00000, -0x08000), VECTOR3x(0x07000, 0x00000, 0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x0F000, 0x00000, 0x10000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -y -z from inside concave box2 (bottom collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x08000, -0x08000), VECTOR3x(0x00000, -0x18000, -0x17000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x10000, -0x0F000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -y -z from inside concave box2 (back collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x08000, -0x08000), VECTOR3x(0x00000, -0x17000, -0x18000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x00000, -0x0F000, -0x10000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +y -z from inside concave box2 (top collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x08000, -0x08000), VECTOR3x(0x00000, 0x08000, -0x17000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x10000, -0x0F000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +y -z from inside concave box2 (back collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x08000, -0x08000), VECTOR3x(0x00000, 0x07000, -0x18000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x00000, 0x0F000, -0x10000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -y +z from inside concave box2 (bottom collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x08000, -0x08000), VECTOR3x(0x00000, -0x18000, 0x07000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x10000, 0x0F000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving -y +z from inside concave box2 (front collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x08000, -0x08000), VECTOR3x(0x00000, -0x17000, 0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x00000, -0x0F000, 0x10000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +y +z from inside concave box2 (top collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x08000, -0x08000), VECTOR3x(0x00000, 0x08000, 0x07000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x0F000), VECTOR3x_ZERO, 0x10000);
	
	// box1 moving +y +z from inside concave box2 (front collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x08000, -0x08000), VECTOR3x(0x00000, 0x07000, 0x08000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x20000, -0x20000, -0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x0F000, 0x10000), VECTOR3x_ZERO, 0x10000);
	
#pragma mark Contact area
	// box1 moving +x at small size, box2 stationary at normal size
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x08000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// box1 moving +x at normal size, box2 stationary at small size
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x06000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x06000);
	
	// box1 moving +x at normal size (deeper), box2 stationary at small size (deeper)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x20000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x06000, 0x20000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x0C000);
	
	// box1 moving +x at normal size, box2 stationary at normal size (half contact bottom)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x08000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
	// box1 moving +x at normal size, box2 stationary at normal size (quarter contact top)
	box1 = initMovingBox3D(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, -0x0C000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x04000);
	
	// box1 resizing +x, box2 stationary at normal size
	box1 = initResizingBox3D(VECTOR3x(-0x08000, 0x00000, 0x00000), VECTOR3x(0x04000, 0x04000, 0x40000), VECTOR3x(0x0C000, 0x0C000, 0xC0000));
	box2 = initStationaryBox3D(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000));
	resetOutParameters();
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x08000);
	
#pragma mark Thickness
	// box1 moving +x inside thin box2
	box1 = initMovingBox3D(VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x00000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +x inside thick box2 (collision)
	box1 = initMovingBox3D(VECTOR3x(-0x09000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x09000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x04000);
	
	// box1 moving +x inside thick box2 (no collision)
	box1 = initMovingBox3D(VECTOR3x(-0x07000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +x inside default thickness box2 (collision)
	box1 = initMovingBox3D(VECTOR3x(-0x09000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), EDGE_THICKNESS_DEFAULT);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x09000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x04000);
	
	// box1 moving +x inside default thickness box2 (no collision)
	box1 = initMovingBox3D(VECTOR3x(-0x07000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), EDGE_THICKNESS_DEFAULT);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +x inside resized default thickness box2 (collision)
	box1 = initMovingBox3D(VECTOR3x(-0x09000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x04000, 0x04000, 0x04000), EDGE_THICKNESS_DEFAULT);
	CollisionBox3D_updateSize(&box2, VECTOR3x(0x20000, 0x20000, 0x20000));
	CollisionBox3D_updateSize(&box2, VECTOR3x(0x20000, 0x20000, 0x20000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x09000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x04000);
	
	// box1 moving +x inside resized default thickness box2 (no collision)
	box1 = initMovingBox3D(VECTOR3x(-0x07000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x04000, 0x04000, 0x04000), EDGE_THICKNESS_DEFAULT);
	CollisionBox3D_updateSize(&box2, VECTOR3x(0x20000, 0x20000, 0x20000));
	CollisionBox3D_updateSize(&box2, VECTOR3x(0x20000, 0x20000, 0x20000));
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -x inside thick box2 (collision)
	box1 = initMovingBox3D(VECTOR3x(0x01000, 0x00000, 0x00000), VECTOR3x(-0x08000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x09000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x04000);
	
	// box1 moving -x inside thick box2 (no collision)
	box1 = initMovingBox3D(VECTOR3x(-0x01000, 0x00000, 0x00000), VECTOR3x(-0x08000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +y inside thick box2 (collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x09000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x09000, 0x00000), VECTOR3x_ZERO, 0x04000);
	
	// box1 moving +y inside thick box2 (no collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, -0x07000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -y inside thick box2 (collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x01000), VECTOR3x(0x00000, 0x00000, -0x08000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x00000, 0x00000, -0x09000), VECTOR3x_ZERO, 0x04000);
	
	// box1 moving -y inside thick box2 (no collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, -0x01000), VECTOR3x(0x00000, 0x00000, -0x08000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving +z inside thick box2 (collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, -0x09000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x00000, 0x09000), VECTOR3x_ZERO, 0x04000);
	
	// box1 moving +z inside thick box2 (no collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, -0x07000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// box1 moving -z inside thick box2 (collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, 0x01000), VECTOR3x(0x00000, 0x00000, -0x08000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x00000, 0x00000, -0x09000), VECTOR3x_ZERO, 0x04000);
	
	// box1 moving -z inside thick box2 (no collision)
	box1 = initMovingBox3D(VECTOR3x(0x00000, 0x00000, -0x01000), VECTOR3x(0x00000, 0x00000, -0x08000), VECTOR3x(0x08000, 0x08000, 0x08000));
	box2 = initStationaryBox3DWithThickness(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x20000, 0x20000, 0x20000), 0x10000);
	result = intersectionHandler_box3D_box3D((CollisionObject *) &box1, (CollisionObject *) &box2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
}

static void testBox_sphere() {
	//TestCase_assert(false, "Unimplemented");
}

static void testBox_line3D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testBox_capsule() {
	//TestCase_assert(false, "Unimplemented");
}

static void testBox_trimesh() {
	//TestCase_assert(false, "Unimplemented");
}

static void testSphere_sphere() {
	CollisionSphere sphere1, sphere2;
	bool result;
	fixed16_16 time, contactArea;
	Vector3x normal, object1Vector, object2Vector;
	
	// No collision for no movement (no contact)
	sphere1 = initStationarySphere(VECTOR3x(-0x40000, 0x00000, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
	// sphere1 moving +x (level), sphere2 stationary
	sphere1 = initMovingSphere(VECTOR3x(-0x40000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// sphere1 moving +x (level), sphere2 stationary (different speed/collision time)
	sphere1 = initMovingSphere(VECTOR3x(-0x40000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x80000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// sphere1 moving +x (level), sphere2 stationary (different radius)
	sphere1 = initMovingSphere(VECTOR3x(-0x40000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x20000);
	resetOutParameters();
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// sphere1 moving +x (level), sphere2 stationary (sphere1 starts butted against sphere2)
	sphere1 = initMovingSphere(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// sphere1 moving +x (45 degrees up), sphere2 stationary
	sphere1 = initMovingSphere(VECTOR3x(-0x40000, 0x16A09, 0x00000), VECTOR3x(0x00000, 0x16A09, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0A57D, VECTOR3x(-0x0B506, 0x0B505, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// sphere1 moving -y, sphere2 moving +x
	sphere1 = initMovingSphere(VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x00000), 0x10000);
	sphere2 = initMovingSphere(VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0257D, VECTOR3x(0x0B505, 0x0B505, 0x00000), VECTOR3x(0x00000, -0x40000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), 0x00000);
	
	// sphere1 moving +z (level), sphere2 stationary
	sphere1 = initMovingSphere(VECTOR3x(0x00000, 0x00000, -0x40000), VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x00000, 0x40000), VECTOR3x_ZERO, 0x00000);
	
	// No collision for no movement (contact)
	sphere1 = initStationarySphere(VECTOR3x(-0x10000, 0x00000, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
	// Miss (sphere1 above moving +x, sphere2 below moving -x)
	sphere1 = initMovingSphere(VECTOR3x(-0x10000, 0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x00000), 0x10000);
	sphere2 = initMovingSphere(VECTOR3x(0x10000, -0x10000, 0x00000), VECTOR3x(-0x10000, -0x10000, 0x00000), 0x10000);
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// sphere1 moving +x (level), sphere2 stationary (sphere1 starts penetrating sphere2 by less than half)
	sphere1 = initMovingSphere(VECTOR3x(-0x08000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// sphere1 moving +x (level), sphere2 stationary (sphere1 starts penetrating sphere2 by more than half)
	sphere1 = initMovingSphere(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x08000, 0x00000, 0x00000), 0x10000);
	sphere2 = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	result = intersectionHandler_sphere_sphere((CollisionObject *) &sphere1, (CollisionObject *) &sphere2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
}

static void testSphere_line3D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testSphere_capsule() {
	//TestCase_assert(false, "Unimplemented");
}

static void testSphere_trimesh() {
	CollisionSphere sphere;
	CollisionStaticTrimesh trimesh;
	bool result;
	fixed16_16 time, contactArea;
	Vector3x normal, object1Vector, object2Vector;
	Vector3x vertices[] = {{0x00000, 0x00000, 0x00000}, {0x20000, 0x00000, 0x00000}, {0x00000, 0x00000, 0x20000},
	                       {0x00000, 0x00000, 0x00000}, {0x00000, 0x00000, 0x20000}, {0x00000, 0x20000, 0x00000},
	                       {0x00000, 0x00000, 0x00000}, {0x00000, 0x20000, 0x00000}, {0x20000, 0x00000, 0x00000},
	                       {0x00000, 0x20000, 0x00000}, {0x00000, 0x00000, 0x20000}, {0x20000, 0x00000, 0x00000}};
	
	CollisionStaticTrimesh_init(&trimesh, NULL, NULL, vertices, 12);
	
	// No collision for no movement
	sphere = initStationarySphere(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Face collision from left
	sphere = initMovingSphere(VECTOR3x(-0x10000, 0x08000, 0x08000), VECTOR3x(0x00000, 0x08000, 0x08000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Face collision from left (different time)
	sphere = initMovingSphere(VECTOR3x(-0x10000, 0x08000, 0x08000), VECTOR3x(0x10000, 0x08000, 0x08000), 0x04000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x06000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Face collision from bottom
	sphere = initMovingSphere(VECTOR3x(0x08000, -0x10000, 0x08000), VECTOR3x(0x08000, 0x00000, 0x08000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Face collision from back
	sphere = initMovingSphere(VECTOR3x(0x08000, 0x08000, -0x10000), VECTOR3x(0x08000, 0x08000, 0x00000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x00000, -0x10000), VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x_ZERO, 0x00000);
	
	// Face collision from front top right
	sphere = initMovingSphere(VECTOR3x(0x18000, 0x18000, 0x18000), VECTOR3x(0x08000, 0x08000, 0x08000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08B6F, VECTOR3x(0x093CD, 0x093CD, 0x093CD), VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x_ZERO, 0x00000);
	
	// Vertex collision from right
	sphere = initMovingSphere(VECTOR3x(0x30000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Vertex collision from top
	sphere = initMovingSphere(VECTOR3x(0x00000, 0x30000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Vertex collision from front
	sphere = initMovingSphere(VECTOR3x(0x00000, 0x00000, 0x30000), VECTOR3x(0x00000, 0x00000, 0x10000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(0x00000, 0x00000, 0x10000), VECTOR3x(0x00000, 0x00000, -0x20000), VECTOR3x_ZERO, 0x00000);
	
	// Vertex collision from left bottom back
	sphere = initMovingSphere(VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x(0x00000, 0x00000, 0x00000), 0x08000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0B619, VECTOR3x(-0x093CD, -0x093CD, -0x093CD), VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x_ZERO, 0x00000);
	
	// Bottom left edge collision
	sphere = initMovingSphere(VECTOR3x(-0x10000, -0x10000, 0x08000), VECTOR3x(0x00000, 0x00000, 0x08000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04AFB, VECTOR3x(-0x0B505, -0x0B505, 0x00000), VECTOR3x(0x10000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Back left edge collision
	sphere = initMovingSphere(VECTOR3x(-0x10000, 0x08000, -0x10000), VECTOR3x(0x00000, 0x08000, 0x00000), 0x10000);
	resetOutParameters();
	result = intersectionHandler_sphere_trimesh((CollisionObject *) &sphere, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04AFB, VECTOR3x(-0x0B506, 0x00000, -0x0B506), VECTOR3x(0x10000, 0x00000, 0x10000), VECTOR3x_ZERO, 0x00000);
	
	CollisionStaticTrimesh_dispose(&trimesh);
}

static void testLine3D_line3D() {
	// Always false
	//TestCase_assert(false, "Unimplemented");
}

static void testLine3D_capsule() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine3D_trimesh() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCapsule_capsule() {
	CollisionCapsule capsule1, capsule2;
	bool result;
	fixed16_16 time, contactArea;
	Vector3x normal, object1Vector, object2Vector;
	
	// No collision for no movement (no contact)
	capsule1 = initStationaryCapsule(VECTOR3x(-0x40000, 0x00000, 0x00000), 0x10000, 0x10000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
	// capsule1 moving +x (level), capsule2 stationary
	capsule1 = initMovingCapsule(VECTOR3x(-0x40000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// capsule1 moving +x (level), capsule2 stationary (different speed/collision time)
	capsule1 = initMovingCapsule(VECTOR3x(-0x40000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), 0x10000, 0x10000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x80000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
	// capsule1 moving +x (bottom of cylinder), capsule2 stationary
	capsule1 = initMovingCapsule(VECTOR3x(-0x40000, -0x20000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x00000), 0x10000, 0x20000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x20000);
	resetOutParameters();
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// capsule1 moving +x (top of cylinder), capsule2 stationary
	capsule1 = initMovingCapsule(VECTOR3x(-0x40000, 0x20000, 0x00000), VECTOR3x(0x00000, 0x20000, 0x00000), 0x10000, 0x20000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x20000);
	resetOutParameters();
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// capsule1 moving -y, capsule2 stationary
	capsule1 = initMovingCapsule(VECTOR3x(0x00000, 0x40000, 0x00000), VECTOR3x(0x00000, 0x20000, 0x00000), 0x10000, 0x10000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// capsule1 moving +y, capsule2 stationary
	capsule1 = initMovingCapsule(VECTOR3x(0x00000, -0x40000, 0x00000), VECTOR3x(0x00000, -0x20000, 0x00000), 0x10000, 0x10000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// capsule1 moving +x (45 degrees below), capsule2 stationary
	capsule1 = initMovingCapsule(VECTOR3x(-0x40000, -0x26A09, 0x00000), VECTOR3x(0x00000, -0x26A09, 0x00000), 0x10000, 0x10000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0A57D, VECTOR3x(-0x0B506, -0x0B505, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// capsule1 moving +x (45 degrees above), capsule2 stationary
	capsule1 = initMovingCapsule(VECTOR3x(-0x40000, 0x26A09, 0x00000), VECTOR3x(0x00000, 0x26A09, 0x00000), 0x10000, 0x10000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0A57D, VECTOR3x(-0x0B506, 0x0B505, 0x00000), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// capsule1 moving +x (45 degrees in front), capsule2 stationary
	capsule1 = initMovingCapsule(VECTOR3x(-0x40000, 0x00000, 0x16A09), VECTOR3x(0x00000, 0x00000, 0x16A09), 0x10000, 0x10000);
	capsule2 = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_capsule((CollisionObject *) &capsule1, (CollisionObject *) &capsule2, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0A57D, VECTOR3x(-0x0B506, 0x00000, 0x0B505), VECTOR3x(0x40000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
}

static void testCapsule_trimesh() {
	CollisionCapsule capsule;
	CollisionStaticTrimesh trimesh;
	bool result;
	fixed16_16 time, contactArea;
	Vector3x normal, object1Vector, object2Vector;
	Vector3x vertices[] = {{0x00000, 0x00000, 0x00000}, {0x20000, 0x00000, 0x00000}, {0x00000, 0x00000, 0x20000},
	                       {0x00000, 0x00000, 0x00000}, {0x00000, 0x00000, 0x20000}, {0x00000, 0x20000, 0x00000},
	                       {0x00000, 0x00000, 0x00000}, {0x00000, 0x20000, 0x00000}, {0x20000, 0x00000, 0x00000},
	                       {0x00000, 0x20000, 0x00000}, {0x00000, 0x00000, 0x20000}, {0x20000, 0x00000, 0x00000}};
	Vector3x vertices2[] = {{0x01000, 0x01000, 0x01000}, {0x20000, 0x00000, 0x00000}, {0x00000, 0x00000, 0x20000},
	                        {0x01000, 0x01000, 0x01000}, {0x00000, 0x00000, 0x20000}, {0x00000, 0x20000, 0x00000},
	                        {0x01000, 0x01000, 0x01000}, {0x00000, 0x20000, 0x00000}, {0x20000, 0x00000, 0x00000},
	                        {0x00000, 0x20000, 0x00000}, {0x00000, 0x00000, 0x20000}, {0x20000, 0x00000, 0x00000}};
	
	CollisionStaticTrimesh_init(&trimesh, NULL, NULL, vertices, 12);
	
	// No collision for no movement
	capsule = initStationaryCapsule(VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Face/cap collision from bottom
	capsule = initMovingCapsule(VECTOR3x(0x08000, -0x20000, 0x08000), VECTOR3x(0x08000, -0x10000, 0x08000), 0x08000, 0x08000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Face/cap collision from bottom (different time)
	capsule = initMovingCapsule(VECTOR3x(0x08000, -0x20000, 0x08000), VECTOR3x(0x08000, 0x00000, 0x08000), 0x08000, 0x08000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04000, VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x20000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Face/cap collision from front top right
	capsule = initMovingCapsule(VECTOR3x(0x18000, 0x10000, 0x18000), VECTOR3x(0x08000, 0x00000, 0x08000), 0x08000, 0x08000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08B6F, VECTOR3x(0x093CD, 0x093CD, 0x093CD), VECTOR3x(-0x10000, -0x10000, -0x10000), VECTOR3x_ZERO, 0x00000);
	
	// Vertex/cap collision from top
	capsule = initMovingCapsule(VECTOR3x(0x00000, 0x28000, 0x00000), VECTOR3x(0x00000, 0x18000, 0x00000), 0x08000, 0x08000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Vertex/cap collision from left bottom back
	capsule = initMovingCapsule(VECTOR3x(-0x10000, -0x20000, -0x10000), VECTOR3x(0x00000, -0x10000, 0x00000), 0x08000, 0x08000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0B619, VECTOR3x(-0x093CD, -0x093CD, -0x093CD), VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x_ZERO, 0x00000);
	
	// Vertex/cylinder collision from right
	capsule = initMovingCapsule(VECTOR3x(0x30000, -0x10000, 0x00000), VECTOR3x(0x20000, -0x10000, 0x00000), 0x08000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x08000, VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Edge/cap collision from left bottom
	capsule = initMovingCapsule(VECTOR3x(-0x10000, -0x30000, 0x08000), VECTOR3x(0x00000, -0x20000, 0x08000), 0x10000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04AFB, VECTOR3x(-0x0B505, -0x0B505, 0x00000), VECTOR3x(0x10000, 0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Edge/cap collision from right back
	capsule = initMovingCapsule(VECTOR3x(0x20000, 0x18000, 0x00000), VECTOR3x(0x10000, 0x08000, 0x00000), 0x08000, 0x08000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0A57D, VECTOR3x(0x0B506, 0x0B506, 0x00000), VECTOR3x(-0x10000, -0x10000, 0x00000), VECTOR3x_ZERO, 0x00000);
	
	// Edge/cylinder collision from right front
	capsule = initMovingCapsule(VECTOR3x(0x20000, -0x10000, 0x20000), VECTOR3x(0x10000, -0x10000, 0x10000), 0x08000, 0x10000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0A57E, VECTOR3x(0x0B505, 0x00000, 0x0B505), VECTOR3x(-0x10000, 0x00000, -0x10000), VECTOR3x_ZERO, 0x00000);
	
	// Bug: Returned collision time much too early (was 0x080FB in test case; 0x0939F expected)
	capsule = initMovingCapsule(VECTOR3x(0x2DD1A, 0x082A0, 0x79134), VECTOR3x(0x0FA1C, 0xFFFE6396, 0xFFFC0E06), 0x08000, 0x14000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x09392, VECTOR3x(0x0B505, 0x00000, 0x0B505), VECTOR3x(-0x1E2FE, -0x21F0A, -0xB832E), VECTOR3x_ZERO, 0x00000);
	
	CollisionStaticTrimesh_dispose(&trimesh);
	CollisionStaticTrimesh_init(&trimesh, NULL, NULL, vertices2, 12);
	
	// Bug: Returned collision time too late (was 0x04FE5 in test case; 0x042CD expected)
	capsule = initMovingCapsule(VECTOR3x(0x2043F, 0x04479, 0xFFFBF3E7), VECTOR3x(0xFFFE112B, 0x0180F, 0x98C13), 0x08000, 0x14000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x042CD, VECTOR3x(0x00000, 0x00000, -0x10000), Vector3x_subtract(capsule.position, capsule.lastPosition), VECTOR3x_ZERO, 0x00000);
	
	// Bug: False positive for nonintersecting movement
	capsule = initMovingCapsule(VECTOR3x(0xFFFDFB65, 0x1C1D4, 0xFFFAFE79), VECTOR3x(0xFFFFB3EE, 0xFFFEBEE0, 0x37D55), 0x08000, 0x14000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Bug: Returned collision time too early (was 0x074CE in test case; 0x0798D expected)
	capsule = initMovingCapsule(VECTOR3x(0xFFFAE296, 0x1FFFD, 0xFFFB3F5A), VECTOR3x(0x4FFEC, 0xFFFF2024, 0x46F40), 0x08000, 0x14000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0798D, VECTOR3x(0xFFFF600D, 0x00000, 0xFFFF381E), Vector3x_subtract(capsule.position, capsule.lastPosition), VECTOR3x_ZERO, 0x00000);
	
	// Bug: Returned collision time too late (was 0x08434 in test case; 0x04E41 expected)
	capsule = initMovingCapsule(VECTOR3x(0xFFFEBF04, 0xFFFF47CE, 0x326F7), VECTOR3x(0x13659, 0x0EBA5, 0xFFFE19C7), 0x08000, 0x14000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x04E41, VECTOR3x(-0x10000, 0x00000, 0x00000), Vector3x_subtract(capsule.position, capsule.lastPosition), VECTOR3x_ZERO, 0x00000);
	
	// Bug: Returned collision time too late (was 0x08193 in test case; 0x05D4F expected)
	capsule = initMovingCapsule(VECTOR3x(0xFFFFE253, 0xFFFEB351, 0xFFFDB2C1), VECTOR3x(0x328CB, 0x0386D, 0x2A436), 0x08000, 0x14000);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x05D4F, VECTOR3x(0x00000, 0x00000, -0x10000), Vector3x_subtract(capsule.position, capsule.lastPosition), VECTOR3x_ZERO, 0x00000);
	
	// Capsule moving outward through face should not collide
	capsule = initMovingCapsule(VECTOR3x(0x043DD, 0x03AC9, 0x0EE2E), VECTOR3x(0xFFFF5FB1, 0x04888, 0x0C4E9), 0x01DB3, 0x03F25);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Capsule moving outward through edge (via cap) should not collide
	capsule = initMovingCapsule(VECTOR3x(0x043DD, 0x03AC9, 0x0EE2E), VECTOR3x(0xFFFF8611, 0xFFFEF661, 0x0DDD0), 0x01DB3, 0x03F25);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Capsule moving outward through edge (via cylinder) should not collide
	capsule = initMovingCapsule(VECTOR3x(0x0536B, 0xFFFFAEE2, 0x0FD3F), VECTOR3x(0x1A1C0, 0xFFFFCD6C, 0x0D3E4), 0x01DB3, 0x03F25);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Capsule moving outward through vertex (via cap) should not collide
	capsule = initMovingCapsule(VECTOR3x(0x04185, 0x03851, 0x0EF4C), VECTOR3x(0xFFFFFF80, 0x1C842, 0xFFFFF985), 0x01DB3, 0x03F25);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Capsule moving outward through vertex (via cylinder) should not collide
	capsule = initMovingCapsule(VECTOR3x(0x043DD, 0x03AC9, 0x0EE2E), VECTOR3x(0xFFFFFEBB, 0xFFFFCC3C, 0x1FB1F), 0x01DB3, 0x03F25);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Capsule moving outward through edge (via cap) should not collide
	capsule = initMovingCapsule(VECTOR3x(0x04185, 0x03851, 0x0EF4C), VECTOR3x(0xFFFFD4C6, 0x1C0A8, 0xFFFFDD57), 0x01DB3, 0x03F25);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertNoCollision(result);
	
	// Bug: False negative for bottom cap passing through vertex
	capsule = initMovingCapsule(VECTOR3x(0x0DA13, 0x1F0C1, 0x0FD51), VECTOR3x(0xFFFF952B, 0x1F8D3, 0xFFFF8E84), 0x01DB3, 0x03F25);
	resetOutParameters();
	result = intersectionHandler_capsule_trimesh((CollisionObject *) &capsule, (CollisionObject *) &trimesh, &time, &normal, &object1Vector, &object2Vector, &contactArea);
	assertCollision(result, time, normal, 0x0A334, VECTOR3x(0x05E4C, 0x0A8D7, 0x0A7CC), Vector3x_subtract(capsule.position, capsule.lastPosition), VECTOR3x_ZERO, 0x00000);
	
	CollisionStaticTrimesh_dispose(&trimesh);
}

static void testTrimesh_trimesh() {
	CollisionStaticTrimesh * trimesh1, * trimesh2;
	Vector3x vertices[3] = {{0x00000, 0x00000, 0x00000}, {0x10000, 0x00000, 0x00000}, {0x00000, 0x10000, 0x00000}};
	bool result;
	
	trimesh1 = CollisionStaticTrimesh_create(NULL, NULL, vertices, 3);
	trimesh2 = CollisionStaticTrimesh_create(NULL, NULL, vertices, 3);
	result = intersectionHandler_trimesh_trimesh((CollisionObject *) trimesh1, (CollisionObject *) trimesh2, NULL, NULL, NULL, NULL, NULL);
	assertNoCollision(result);
	
	CollisionStaticTrimesh_dispose(trimesh1);
	CollisionStaticTrimesh_dispose(trimesh2);
}

TEST_SUITE(StandardIntersectionHandlersTest,
           testRect2D_rect2D,
           testRect2D_circle,
           testRect2D_line2D,
           testRect2D_polygon,
           testCircle_circle,
           testCircle_circle_farawayBug,
           testCircle_line2D,
           testCircle_polygon,
           testLine2D_line2D,
           testLine2D_polygon,
           testPolygon_polygon,
           testBox_box,
           testBox_sphere,
           testBox_line3D,
           testBox_capsule,
           testBox_trimesh,
           testSphere_sphere,
           testSphere_line3D,
           testSphere_capsule,
           testSphere_trimesh,
           testLine3D_line3D,
           testLine3D_capsule,
           testLine3D_trimesh,
           testCapsule_capsule,
           testCapsule_trimesh,
           testTrimesh_trimesh)
