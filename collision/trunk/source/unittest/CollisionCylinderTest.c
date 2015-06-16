#include "collision/CollisionCylinder.h"
#include "unittest/TestSuite.h"
#include <string.h>

static void verifyInitCylinder(int line, CollisionCylinder * collisionCylinder, void * owner, fixed16_16 radius, fixed16_16 height, Vector3x bottomCenter, CollisionCallback collisionCallback) {
	TestCase_assert(collisionCylinder != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(collisionCylinder->dispose == CollisionCylinder_dispose, "Expected %p but got %p (line %d)", CollisionCylinder_dispose, collisionCylinder->dispose);
	TestCase_assert(collisionCylinder->shapeType == COLLISION_SHAPE_CYLINDER, "Expected %d but got %d (line %d)", COLLISION_SHAPE_CYLINDER, collisionCylinder->shapeType, line);
	TestCase_assert(collisionCylinder->owner == owner, "Expected %p but got %p (line %d)", owner, collisionCylinder->owner, line);
	TestCase_assert(collisionCylinder->shape.cylinder.radius == radius, "Expected 0x%05X but got 0x%05X (line %d)", radius, collisionCylinder->shape.cylinder.radius, line);
	TestCase_assert(collisionCylinder->shape.cylinder.height == height, "Expected 0x%05X but got 0x%05X (line %d)", height, collisionCylinder->shape.cylinder.height, line);
	TestCase_assert(collisionCylinder->shape.cylinder.bottomCenter.x == bottomCenter.x, "Expected 0x%05X but got 0x%05X (line %d)", bottomCenter.x, collisionCylinder->shape.cylinder.bottomCenter.x, line);
	TestCase_assert(collisionCylinder->shape.cylinder.bottomCenter.y == bottomCenter.y, "Expected 0x%05X but got 0x%05X (line %d)", bottomCenter.y, collisionCylinder->shape.cylinder.bottomCenter.y, line);
	TestCase_assert(collisionCylinder->shape.cylinder.bottomCenter.z == bottomCenter.z, "Expected 0x%05X but got 0x%05X (line %d)", bottomCenter.z, collisionCylinder->shape.cylinder.bottomCenter.z, line);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.x == bottomCenter.x, "Expected 0x%05X but got 0x%05X (line %d)", bottomCenter.x, collisionCylinder->shape.cylinder.lastBottomCenter.x, line);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.y == bottomCenter.y, "Expected 0x%05X but got 0x%05X (line %d)", bottomCenter.y, collisionCylinder->shape.cylinder.lastBottomCenter.y, line);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.z == bottomCenter.z, "Expected 0x%05X but got 0x%05X (line %d)", bottomCenter.z, collisionCylinder->shape.cylinder.lastBottomCenter.z, line);
	TestCase_assert(collisionCylinder->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, collisionCylinder->collisionCallback, line);
}

static void testInitCylinder() {
	CollisionCylinder collisionCylinder, * collisionCylinderPtr;
	
	memset(&collisionCylinder, 0x00, sizeof(collisionCylinder));
	CollisionCylinder_initCylinder(&collisionCylinder, NULL, 0, 0, VECTOR3x_ZERO, NULL);
	verifyInitCylinder(__LINE__, &collisionCylinder, NULL, 0, 0, VECTOR3x_ZERO, NULL);
	CollisionCylinder_dispose(&collisionCylinder);
	
	memset(&collisionCylinder, 0xFF, sizeof(collisionCylinder));
	CollisionCylinder_initCylinder(&collisionCylinder, NULL, 0, 0, VECTOR3x_ZERO, NULL);
	verifyInitCylinder(__LINE__, &collisionCylinder, NULL, 0, 0, VECTOR3x_ZERO, NULL);
	CollisionCylinder_dispose(&collisionCylinder);
	
	memset(&collisionCylinder, 0x00, sizeof(collisionCylinder));
	CollisionCylinder_initCylinder(&collisionCylinder, (void *) 0x1234, 2, 3, VECTOR3x(4, 5, 6), (CollisionCallback) 0x2345);
	verifyInitCylinder(__LINE__, &collisionCylinder, (void *) 0x1234, 2, 3, VECTOR3x(4, 5, 6), (CollisionCallback) 0x2345);
	CollisionCylinder_dispose(&collisionCylinder);
	
	collisionCylinderPtr = CollisionCylinder_createCylinder((void *) 0x1234, 2, 3, VECTOR3x(4, 5, 6), (CollisionCallback) 0x2345);
	verifyInitCylinder(__LINE__, collisionCylinderPtr, (void *) 0x1234, 2, 3, VECTOR3x(4, 5, 6), (CollisionCallback) 0x2345);
	CollisionCylinder_dispose(collisionCylinderPtr);
}

static void testUpdatePosition() {
	CollisionCylinder * collisionCylinder;
	
	collisionCylinder = CollisionCylinder_createCylinder(NULL, 0, 0, VECTOR3x_ZERO, NULL);
	verifyInitCylinder(__LINE__, collisionCylinder, NULL, 0, 0, VECTOR3x_ZERO, NULL);
	
	CollisionCylinder_updatePosition(collisionCylinder, VECTOR3x(1, 2, 3));
	TestCase_assert(collisionCylinder->shape.cylinder.bottomCenter.x == 1, "Expected 1 but got %d", collisionCylinder->shape.cylinder.bottomCenter.x);
	TestCase_assert(collisionCylinder->shape.cylinder.bottomCenter.y == 2, "Expected 2 but got %d", collisionCylinder->shape.cylinder.bottomCenter.y);
	TestCase_assert(collisionCylinder->shape.cylinder.bottomCenter.z == 3, "Expected 3 but got %d", collisionCylinder->shape.cylinder.bottomCenter.z);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.x == 0, "Expected 0 but got %d", collisionCylinder->shape.cylinder.lastBottomCenter.x);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.y == 0, "Expected 0 but got %d", collisionCylinder->shape.cylinder.lastBottomCenter.y);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.z == 0, "Expected 0 but got %d", collisionCylinder->shape.cylinder.lastBottomCenter.z);
	
	CollisionCylinder_updatePosition(collisionCylinder, VECTOR3x(-1, 4, 2));
	TestCase_assert(collisionCylinder->shape.cylinder.bottomCenter.x == -1, "Expected -1 but got %d", collisionCylinder->shape.cylinder.bottomCenter.x);
	TestCase_assert(collisionCylinder->shape.cylinder.bottomCenter.y == 4, "Expected 4 but got %d", collisionCylinder->shape.cylinder.bottomCenter.y);
	TestCase_assert(collisionCylinder->shape.cylinder.bottomCenter.z == 2, "Expected 2 but got %d", collisionCylinder->shape.cylinder.bottomCenter.z);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.x == 1, "Expected 1 but got %d", collisionCylinder->shape.cylinder.lastBottomCenter.x);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.y == 2, "Expected 2 but got %d", collisionCylinder->shape.cylinder.lastBottomCenter.y);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.z == 3, "Expected 3 but got %d", collisionCylinder->shape.cylinder.lastBottomCenter.z);
	
	CollisionCylinder_dispose(collisionCylinder);
}

static void testInterpolate() {
	CollisionCylinder * collisionCylinder;
	
	collisionCylinder = CollisionCylinder_createCylinder(NULL, 0, 0, VECTOR3x_ZERO, NULL);
	verifyInitCylinder(__LINE__, collisionCylinder, NULL, 0, 0, VECTOR3x_ZERO, NULL);
	CollisionCylinder_updatePosition(collisionCylinder, VECTOR3x(0x10000, 0x20000, 0x40000));
	
	CollisionCylinder_interpolate(collisionCylinder, 0x08000);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.x == 0x08000, "Expected 0x08000 but got 0x%05X", collisionCylinder->shape.cylinder.lastBottomCenter.x);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.y == 0x10000, "Expected 0x10000 but got 0x%05X", collisionCylinder->shape.cylinder.lastBottomCenter.y);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.z == 0x20000, "Expected 0x20000 but got 0x%05X", collisionCylinder->shape.cylinder.lastBottomCenter.z);
	
	CollisionCylinder_interpolate(collisionCylinder, 0x04000);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.x == 0x0A000, "Expected 0x0A000 but got 0x%05X", collisionCylinder->shape.cylinder.lastBottomCenter.x);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.y == 0x14000, "Expected 0x14000 but got 0x%05X", collisionCylinder->shape.cylinder.lastBottomCenter.y);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.z == 0x28000, "Expected 0x28000 but got 0x%05X", collisionCylinder->shape.cylinder.lastBottomCenter.z);
	
	CollisionCylinder_dispose(collisionCylinder);
	
	collisionCylinder = CollisionCylinder_createCylinder(NULL, 0, 0, VECTOR3x(0x10000, 0x10000, 0x10000), NULL);
	verifyInitCylinder(__LINE__, collisionCylinder, NULL, 0, 0, VECTOR3x(0x10000, 0x10000, 0x10000), NULL);
	CollisionCylinder_updatePosition(collisionCylinder, VECTOR3x(0x00000, 0x20000, 0x00000));
	
	CollisionCylinder_interpolate(collisionCylinder, 0x08000);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.x == 0x08000, "Expected 0x08000 but got 0x%05X", collisionCylinder->shape.cylinder.lastBottomCenter.x);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.y == 0x18000, "Expected 0x18000 but got 0x%05X", collisionCylinder->shape.cylinder.lastBottomCenter.y);
	TestCase_assert(collisionCylinder->shape.cylinder.lastBottomCenter.z == 0x08000, "Expected 0x08000 but got 0x%05X", collisionCylinder->shape.cylinder.lastBottomCenter.z);
	
	CollisionCylinder_dispose(collisionCylinder);
}

TEST_SUITE(CollisionCylinderTest,
           testInitCylinder,
           testUpdatePosition,
           testInterpolate)
