#include "collision/CollisionCircle.h"
#include "collision/CollisionShared.h"
#include "unittest/TestSuite.h"

static void verifyInit(int line, CollisionCircle * circle, void * owner, CollisionCallback collisionCallback, Vector2x position, fixed16_16 radius) {
	TestCase_assert(circle != NULL, "Expected non-NULL but got NULL (line %d)", circle, line);
	TestCase_assert(circle->dispose == CollisionCircle_dispose, "Expected %p but got %p (line %d)", CollisionCircle_dispose, circle->dispose, line);
	TestCase_assert(circle->shapeType == COLLISION_SHAPE_CIRCLE, "Expected %d but got %d (line %d)", COLLISION_SHAPE_CIRCLE, circle->shapeType, line);
	TestCase_assert(circle->owner == owner, "Expected %p but got %p (line %d)", owner, circle->owner, line);
	TestCase_assert(circle->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, circle->collisionCallback, line);
	TestCase_assert(circle->interpolate == CollisionCircle_interpolate, "Expected %p but got %p (line %d)", CollisionCircle_interpolate, circle->interpolate, line);
	TestCase_assert(circle->position.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, circle->position.x, line);
	TestCase_assert(circle->position.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, circle->position.y, line);
	TestCase_assert(circle->lastPosition.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, circle->lastPosition.x, line);
	TestCase_assert(circle->lastPosition.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, circle->lastPosition.y, line);
	TestCase_assert(circle->radius == radius, "Expected 0x%05X but got 0x%05X (line %d)", radius, circle->radius, line);
}

static void testInit() {
	CollisionCircle circle, * circlePtr;
	
	memset(&circle, 0x00, sizeof(circle));
	CollisionCircle_init(&circle, NULL, NULL, VECTOR2x_ZERO, 0x00000);
	verifyInit(__LINE__, &circle, NULL, NULL, VECTOR2x_ZERO, 0x00000);
	CollisionCircle_dispose(&circle);
	
	memset(&circle, 0xFF, sizeof(circle));
	CollisionCircle_init(&circle, NULL, NULL, VECTOR2x_ZERO, 0x00000);
	verifyInit(__LINE__, &circle, NULL, NULL, VECTOR2x_ZERO, 0x00000);
	CollisionCircle_dispose(&circle);
	
	memset(&circle, 0x00, sizeof(circle));
	CollisionCircle_init(&circle, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), 0x30000);
	verifyInit(__LINE__, &circle, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), 0x30000);
	CollisionCircle_dispose(&circle);
	
	circlePtr = CollisionCircle_create(NULL, NULL, VECTOR2x_ZERO, 0x00000);
	verifyInit(__LINE__, circlePtr, NULL, NULL, VECTOR2x_ZERO, 0x00000);
	CollisionCircle_dispose(circlePtr);
	
	circlePtr = CollisionCircle_create((void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), 0x30000);
	verifyInit(__LINE__, circlePtr, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), 0x30000);
	CollisionCircle_dispose(circlePtr);
}

static void testUpdatePosition() {
	CollisionCircle * circle;
	
	circle = CollisionCircle_create(NULL, NULL, VECTOR2x(0x10000, 0x20000), 0x00000);
	TestCase_assert(circle->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", circle->position.x);
	TestCase_assert(circle->position.y == 0x20000, "Expected 0x20000 but got 0x%05X", circle->position.y);
	TestCase_assert(circle->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", circle->lastPosition.x);
	TestCase_assert(circle->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", circle->lastPosition.y);
	
	CollisionCircle_updatePosition(circle, VECTOR2x(0x00000, 0x00000));
	TestCase_assert(circle->position.x == 0x00000, "Expected 0x00000 but got 0x%05X", circle->position.x);
	TestCase_assert(circle->position.y == 0x00000, "Expected 0x00000 but got 0x%05X", circle->position.y);
	TestCase_assert(circle->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", circle->lastPosition.x);
	TestCase_assert(circle->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", circle->lastPosition.y);
	
	CollisionCircle_updatePosition(circle, VECTOR2x(0x08000, 0x0C000));
	TestCase_assert(circle->position.x == 0x08000, "Expected 0x08000 but got 0x%05X", circle->position.x);
	TestCase_assert(circle->position.y == 0x0C000, "Expected 0x0C000 but got 0x%05X", circle->position.y);
	TestCase_assert(circle->lastPosition.x == 0x00000, "Expected 0x00000 but got 0x%05X", circle->lastPosition.x);
	TestCase_assert(circle->lastPosition.y == 0x00000, "Expected 0x00000 but got 0x%05X", circle->lastPosition.y);
	
	CollisionCircle_dispose(circle);
}

static void testInterpolate() {
	CollisionCircle * circle;
	
	circle = CollisionCircle_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), 0x00000);
	CollisionCircle_updatePosition(circle, VECTOR2x(0x10000, 0x10000));
	CollisionCircle_interpolate(circle, 0x08000);
	
	TestCase_assert(circle->lastPosition.x == 0x08000, "Expected 0x08000 but got 0x%05X", circle->lastPosition.x);
	TestCase_assert(circle->lastPosition.y == 0x08000, "Expected 0x08000 but got 0x%05X", circle->lastPosition.y);
	TestCase_assert(circle->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", circle->position.x);
	TestCase_assert(circle->position.y == 0x10000, "Expected 0x10000 but got 0x%05X", circle->position.y);
	
	CollisionCircle_updatePosition(circle, VECTOR2x(0x20000, 0x30000));
	CollisionCircle_interpolate(circle, 0x04000);
	
	TestCase_assert(circle->lastPosition.x == 0x14000, "Expected 0x14000 but got 0x%05X", circle->lastPosition.x);
	TestCase_assert(circle->lastPosition.y == 0x18000, "Expected 0x18000 but got 0x%05X", circle->lastPosition.y);
	TestCase_assert(circle->position.x == 0x20000, "Expected 0x20000 but got 0x%05X", circle->position.x);
	TestCase_assert(circle->position.y == 0x30000, "Expected 0x30000 but got 0x%05X", circle->position.y);
	
	CollisionCircle_dispose(circle);
}

TEST_SUITE(CollisionCircleTest,
           testInit,
           testUpdatePosition,
           testInterpolate)
