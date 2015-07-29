#include "collision/CollisionCircle.h"
#include "collision/CollisionRect2D.h"
#include "collision/CollisionStaticTrimesh.h"
#include "collision/StandardIntersectionHandlers.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

static CollisionRect2D initStationaryRect2D(Vector2x position, Vector2x size) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, position, size);
	return rect;
}

static CollisionRect2D initMovingRect2D(Vector2x lastPosition, Vector2x position, Vector2x size) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, lastPosition, size);
	CollisionRect2D_updatePosition(&rect, position);
	return rect;
}

static CollisionRect2D initResizingRect2D(Vector2x position, Vector2x lastSize, Vector2x size) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, position, lastSize);
	CollisionRect2D_updateSize(&rect, size);
	return rect;
}

static CollisionRect2D initStationaryRect2DWithSolidity(Vector2x position, Vector2x size, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, position, size);
	CollisionRect2D_setSolidity(&rect, solidLeft, solidRight, solidBottom, solidTop);
	return rect;
}

static CollisionRect2D initMovingRect2DWithSolidity(Vector2x lastPosition, Vector2x position, Vector2x size, bool solidLeft, bool solidRight, bool solidBottom, bool solidTop) {
	CollisionRect2D rect;
	
	CollisionRect2D_init(&rect, NULL, NULL, lastPosition, size);
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

#define assertNoCollision(result) \
	TestCase_assert(!result, "Expected no collision, but intersection handler returned true");

#define assertCollision(result, time, normal, expectedTime, expectedNormal, expectedObject1Vector, expectedObject2Vector, expectedContactArea) \
	TestCase_assert(result, "Expected collision, but intersection handler returned false"); \
	TestCase_assert(time == expectedTime, "Expected intersection time " FIXED_16_16_FORMAT ", but got " FIXED_16_16_FORMAT, expectedTime, time); \
	TestCase_assert(normal.x == expectedNormal.x && normal.y == expectedNormal.y && normal.z == expectedNormal.z, "Expected intersection normal {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "} but got {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "}", expectedNormal.x, expectedNormal.y, expectedNormal.z, normal.x, normal.y, normal.z); \
	TestCase_assert(object1Vector.x == expectedObject1Vector.x && object1Vector.y == expectedObject1Vector.y && object1Vector.z == expectedObject1Vector.z, "Expected object1 vector {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "} but got {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "}", expectedObject1Vector.x, expectedObject1Vector.y, expectedObject1Vector.z, object1Vector.x, object1Vector.y, object1Vector.z); \
	TestCase_assert(object2Vector.x == expectedObject2Vector.x && object2Vector.y == expectedObject2Vector.y && object2Vector.z == expectedObject2Vector.z, "Expected object2 vector {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "} but got {" FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT ", " FIXED_16_16_FORMAT "}", expectedObject2Vector.x, expectedObject2Vector.y, expectedObject2Vector.z, object2Vector.x, object2Vector.y, object2Vector.z); \
	TestCase_assert(contactArea == expectedContactArea, "Expected contact area " FIXED_16_16_FORMAT ", but got " FIXED_16_16_FORMAT, expectedContactArea, contactArea);

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
	assertCollision(result, time, normal, 0x00000, VECTOR3x(-0x10000, 0x00000, 0x00000), VECTOR3x(0x20000, 0x00000, 0x00000), VECTOR3x_ZERO, 0x10000);
	
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
	// Differing areas from same size partial contact, different size full contact, changing size
	TestCase_assert(false, "Contact area unimplemented");
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
	
	TestCase_assert(false, "Testing last 3 parameters unimplemented");
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
	//TestCase_assert(false, "Unimplemented");
}

static void testLine2D_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testPolygon_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testBox_box() {
	TestCase_assert(false, "Unimplemented");
}

static void testBox_sphere() {
	TestCase_assert(false, "Unimplemented");
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
	TestCase_assert(false, "Unimplemented");
}

static void testSphere_line3D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testSphere_capsule() {
	//TestCase_assert(false, "Unimplemented");
}

static void testSphere_trimesh() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine3D_line3D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine3D_capsule() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine3D_trimesh() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCapsule_capsule() {
	TestCase_assert(false, "Unimplemented");
}

static void testCapsule_trimesh() {
	TestCase_assert(false, "Unimplemented");
}

static void testTrimesh_trimesh() {
	CollisionStaticTrimesh * trimesh1, * trimesh2;
	Vector3x vertices[3] = {{0x00000, 0x00000, 0x00000}, {0x10000, 0x00000, 0x00000}, {0x00000, 0x10000, 0x00000}};
	bool result;
	
	trimesh1 = CollisionStaticTrimesh_create(NULL, NULL, vertices, 3, false, false);
	trimesh2 = CollisionStaticTrimesh_create(NULL, NULL, vertices, 3, false, false);
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
