#include "collision/CollisionLine2D.h"
#include "collision/CollisionShared.h"
#include "unittest/TestSuite.h"

static void verifyInit(int line, CollisionLine2D * line2D, void * owner, CollisionCallback collisionCallback, Vector2x endpoint1, Vector2x endpoint2, bool doubleSided) {
	TestCase_assert(line2D != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(line2D->dispose == CollisionLine2D_dispose, "Expected %p but got %p (line %d)", CollisionLine2D_dispose, line2D->dispose, line);
	TestCase_assert(line2D->shapeType == COLLISION_SHAPE_LINE_2D, "Expected %d but got %d (line %d)", COLLISION_SHAPE_LINE_2D, line2D->shapeType, line);
	TestCase_assert(line2D->owner == owner, "Expected %p but got %p (line %d)", owner, line2D->owner, line);
	TestCase_assert(line2D->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, line2D->collisionCallback, line);
	TestCase_assert(line2D->interpolate == CollisionLine2D_interpolate, "Expected %p but got %p (line %d)", CollisionLine2D_interpolate, line2D->interpolate, line);
	TestCase_assert(line2D->isStatic == CollisionLine2D_isStatic, "Expected %p but got %p (line %d)", CollisionLine2D_isStatic, line2D->isStatic, line);
	TestCase_assert(line2D->getCollisionBounds == CollisionLine2D_getCollisionBounds, "Expected %p but got %p (line %d)", CollisionLine2D_getCollisionBounds, line2D->getCollisionBounds, line);
	TestCase_assert(line2D->endpoint1.x == endpoint1.x, "Expected 0x%05X but got 0x%05X (line %d)", endpoint1.x, line2D->endpoint1.x, line);
	TestCase_assert(line2D->endpoint1.y == endpoint1.y, "Expected 0x%05X but got 0x%05X (line %d)", endpoint1.y, line2D->endpoint1.y, line);
	TestCase_assert(line2D->endpoint2.x == endpoint2.x, "Expected 0x%05X but got 0x%05X (line %d)", endpoint2.x, line2D->endpoint2.x, line);
	TestCase_assert(line2D->endpoint2.y == endpoint2.y, "Expected 0x%05X but got 0x%05X (line %d)", endpoint2.y, line2D->endpoint2.y, line);
	TestCase_assert(line2D->lastEndpoint1.x == endpoint1.x, "Expected 0x%05X but got 0x%05X (line %d)", endpoint1.x, line2D->lastEndpoint1.x, line);
	TestCase_assert(line2D->lastEndpoint1.y == endpoint1.y, "Expected 0x%05X but got 0x%05X (line %d)", endpoint1.y, line2D->lastEndpoint1.y, line);
	TestCase_assert(line2D->lastEndpoint2.x == endpoint2.x, "Expected 0x%05X but got 0x%05X (line %d)", endpoint2.x, line2D->lastEndpoint2.x, line);
	TestCase_assert(line2D->lastEndpoint2.y == endpoint2.y, "Expected 0x%05X but got 0x%05X (line %d)", endpoint2.y, line2D->lastEndpoint2.y, line);
	TestCase_assert(line2D->doubleSided == doubleSided, "Expected %s but got %s (line %d)", doubleSided ? "true" : "false", line2D->doubleSided ? "true" : "false", line);
}

static void testInit() {
	CollisionLine2D line2D, * line2DPtr;
	
	memset(&line2D, 0x00, sizeof(line2D));
	CollisionLine2D_init(&line2D, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO, false);
	verifyInit(__LINE__, &line2D, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO, false);
	CollisionLine2D_dispose(&line2D);
	
	memset(&line2D, 0xFF, sizeof(line2D));
	CollisionLine2D_init(&line2D, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO, false);
	verifyInit(__LINE__, &line2D, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO, false);
	CollisionLine2D_dispose(&line2D);
	
	memset(&line2D, 0x00, sizeof(line2D));
	CollisionLine2D_init(&line2D, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), VECTOR2x(0x30000, 0x40000), true);
	verifyInit(__LINE__, &line2D, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), VECTOR2x(0x30000, 0x40000), true);
	CollisionLine2D_dispose(&line2D);
	
	line2DPtr = CollisionLine2D_create(NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO, false);
	verifyInit(__LINE__, line2DPtr, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO, false);
	CollisionLine2D_dispose(line2DPtr);
	
	line2DPtr = CollisionLine2D_create((void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), VECTOR2x(0x30000, 0x40000), true);
	verifyInit(__LINE__, line2DPtr, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), VECTOR2x(0x30000, 0x40000), true);
	CollisionLine2D_dispose(line2DPtr);
}

static void testUpdateEndpoints() {
	CollisionLine2D * line2D;
	
	line2D = CollisionLine2D_create(NULL, NULL, VECTOR2x(0x10000, 0x20000), VECTOR2x(0x30000, 0x40000), false);
	TestCase_assert(line2D->endpoint1.x == 0x10000, "Expected 0x10000 but got 0x%05X", line2D->endpoint1.x);
	TestCase_assert(line2D->endpoint1.y == 0x20000, "Expected 0x20000 but got 0x%05X", line2D->endpoint1.y);
	TestCase_assert(line2D->endpoint2.x == 0x30000, "Expected 0x30000 but got 0x%05X", line2D->endpoint2.x);
	TestCase_assert(line2D->endpoint2.y == 0x40000, "Expected 0x40000 but got 0x%05X", line2D->endpoint2.y);
	TestCase_assert(line2D->lastEndpoint1.x == 0x10000, "Expected 0x10000 but got 0x%05X", line2D->lastEndpoint1.x);
	TestCase_assert(line2D->lastEndpoint1.y == 0x20000, "Expected 0x20000 but got 0x%05X", line2D->lastEndpoint1.y);
	TestCase_assert(line2D->lastEndpoint2.x == 0x30000, "Expected 0x30000 but got 0x%05X", line2D->lastEndpoint2.x);
	TestCase_assert(line2D->lastEndpoint2.y == 0x40000, "Expected 0x40000 but got 0x%05X", line2D->lastEndpoint2.y);
	
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	TestCase_assert(line2D->endpoint1.x == 0x00000, "Expected 0x00000 but got 0x%05X", line2D->endpoint1.x);
	TestCase_assert(line2D->endpoint1.y == 0x00000, "Expected 0x00000 but got 0x%05X", line2D->endpoint1.y);
	TestCase_assert(line2D->endpoint2.x == 0x10000, "Expected 0x10000 but got 0x%05X", line2D->endpoint2.x);
	TestCase_assert(line2D->endpoint2.y == 0x10000, "Expected 0x10000 but got 0x%05X", line2D->endpoint2.y);
	TestCase_assert(line2D->lastEndpoint1.x == 0x10000, "Expected 0x10000 but got 0x%05X", line2D->lastEndpoint1.x);
	TestCase_assert(line2D->lastEndpoint1.y == 0x20000, "Expected 0x20000 but got 0x%05X", line2D->lastEndpoint1.y);
	TestCase_assert(line2D->lastEndpoint2.x == 0x30000, "Expected 0x30000 but got 0x%05X", line2D->lastEndpoint2.x);
	TestCase_assert(line2D->lastEndpoint2.y == 0x40000, "Expected 0x40000 but got 0x%05X", line2D->lastEndpoint2.y);
	
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(0x08000, 0x0C000), VECTOR2x(0x04000, 0x00000));
	TestCase_assert(line2D->endpoint1.x == 0x08000, "Expected 0x08000 but got 0x%05X", line2D->endpoint1.x);
	TestCase_assert(line2D->endpoint1.y == 0x0C000, "Expected 0x0C000 but got 0x%05X", line2D->endpoint1.y);
	TestCase_assert(line2D->endpoint2.x == 0x04000, "Expected 0x04000 but got 0x%05X", line2D->endpoint2.x);
	TestCase_assert(line2D->endpoint2.y == 0x00000, "Expected 0x00000 but got 0x%05X", line2D->endpoint2.y);
	TestCase_assert(line2D->lastEndpoint1.x == 0x00000, "Expected 0x00000 but got 0x%05X", line2D->lastEndpoint1.x);
	TestCase_assert(line2D->lastEndpoint1.y == 0x00000, "Expected 0x00000 but got 0x%05X", line2D->lastEndpoint1.y);
	TestCase_assert(line2D->lastEndpoint2.x == 0x10000, "Expected 0x10000 but got 0x%05X", line2D->lastEndpoint2.x);
	TestCase_assert(line2D->lastEndpoint2.y == 0x10000, "Expected 0x10000 but got 0x%05X", line2D->lastEndpoint2.y);
	
	CollisionLine2D_dispose(line2D);
}

static void testInterpolate() {
	CollisionLine2D * line2D;
	
	line2D = CollisionLine2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false);
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(0x10000, 0x10000), VECTOR2x(0x20000, 0x20000));
	CollisionLine2D_interpolate(line2D, 0x08000);
	
	TestCase_assert(line2D->lastEndpoint1.x == 0x08000, "Expected 0x08000 but got 0x%05X", line2D->lastEndpoint1.x);
	TestCase_assert(line2D->lastEndpoint1.y == 0x08000, "Expected 0x08000 but got 0x%05X", line2D->lastEndpoint1.y);
	TestCase_assert(line2D->lastEndpoint2.x == 0x18000, "Expected 0x18000 but got 0x%05X", line2D->lastEndpoint2.x);
	TestCase_assert(line2D->lastEndpoint2.y == 0x18000, "Expected 0x18000 but got 0x%05X", line2D->lastEndpoint2.y);
	TestCase_assert(line2D->endpoint1.x == 0x10000, "Expected 0x10000 but got 0x%05X", line2D->endpoint1.x);
	TestCase_assert(line2D->endpoint1.y == 0x10000, "Expected 0x10000 but got 0x%05X", line2D->endpoint1.y);
	TestCase_assert(line2D->endpoint2.x == 0x20000, "Expected 0x20000 but got 0x%05X", line2D->endpoint2.x);
	TestCase_assert(line2D->endpoint2.y == 0x20000, "Expected 0x20000 but got 0x%05X", line2D->endpoint2.y);
	
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(0x20000, 0x30000), VECTOR2x(0x40000, 0x50000));
	CollisionLine2D_interpolate(line2D, 0x04000);
	
	TestCase_assert(line2D->lastEndpoint1.x == 0x14000, "Expected 0x14000 but got 0x%05X", line2D->lastEndpoint1.x);
	TestCase_assert(line2D->lastEndpoint1.y == 0x18000, "Expected 0x18000 but got 0x%05X", line2D->lastEndpoint1.y);
	TestCase_assert(line2D->lastEndpoint2.x == 0x28000, "Expected 0x28000 but got 0x%05X", line2D->lastEndpoint2.x);
	TestCase_assert(line2D->lastEndpoint2.y == 0x2C000, "Expected 0x2C000 but got 0x%05X", line2D->lastEndpoint2.y);
	TestCase_assert(line2D->endpoint1.x == 0x20000, "Expected 0x20000 but got 0x%05X", line2D->endpoint1.x);
	TestCase_assert(line2D->endpoint1.y == 0x30000, "Expected 0x30000 but got 0x%05X", line2D->endpoint1.y);
	TestCase_assert(line2D->endpoint2.x == 0x40000, "Expected 0x40000 but got 0x%05X", line2D->endpoint2.x);
	TestCase_assert(line2D->endpoint2.y == 0x50000, "Expected 0x50000 but got 0x%05X", line2D->endpoint2.y);
	
	CollisionLine2D_dispose(line2D);
}

static void testIsStatic() {
	CollisionLine2D * line2D;
	bool result;
	
	line2D = CollisionLine2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), false);
	result = CollisionLine2D_isStatic(line2D);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(0x10000, 0x10000), VECTOR2x(0x20000, 0x10000));
	result = CollisionLine2D_isStatic(line2D);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(0x10000, 0x10000), VECTOR2x(0x20000, 0x10000));
	result = CollisionLine2D_isStatic(line2D);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionLine2D_dispose(line2D);
}

static void testGetCollisionBounds() {
	CollisionLine2D * line2D;
	Box6x bounds;
	
	line2D = CollisionLine2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false);
	bounds = CollisionLine2D_getCollisionBounds(line2D);
	TestCase_assert(bounds.left == 0x00000 && bounds.right == 0x10000 &&
	                bounds.bottom == 0x00000 && bounds.top == 0x10000 &&
	                bounds.back == 0x00000 && bounds.front == 0x10000,
	                "Expected {0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(0x10000, 0x10000), VECTOR2x(0x40000, 0x30000));
	bounds = CollisionLine2D_getCollisionBounds(line2D);
	TestCase_assert(bounds.left == 0x00000 && bounds.right == 0x40000 &&
	                bounds.bottom == 0x00000 && bounds.top == 0x30000 &&
	                bounds.back == 0x00000 && bounds.front == 0x10000,
	                "Expected {0x00000, 0x40000, 0x00000, 0x30000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(-0x10000, -0x10000), VECTOR2x(-0x20000, -0x20000));
	bounds = CollisionLine2D_getCollisionBounds(line2D);
	TestCase_assert(bounds.left == -0x20000 && bounds.right == 0x40000 &&
	                bounds.bottom == -0x20000 && bounds.top == 0x30000 &&
	                bounds.back == 0x00000 && bounds.front == 0x10000,
	                "Expected {0xFFFE0000, 0x40000, 0xFFFE0000, 0x30000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x00000));
	CollisionLine2D_updateEndpoints(line2D, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x00000));
	bounds = CollisionLine2D_getCollisionBounds(line2D);
	TestCase_assert(bounds.left == -0x00001 && bounds.right == 0x00001 &&
	                bounds.bottom == -0x00001 && bounds.top == 0x00001 &&
	                bounds.back == 0x00000 && bounds.front == 0x10000,
	                "Expected {0xFFFFFFFF, 0x00001, 0xFFFFFFFF, 0x00001, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionLine2D_dispose(line2D);
}

TEST_SUITE(CollisionLine2DTest,
           testInit,
           testUpdateEndpoints,
           testInterpolate,
           testIsStatic,
           testGetCollisionBounds)
