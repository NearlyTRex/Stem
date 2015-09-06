#include "collision/CollisionShared.h"
#include "collision/CollisionSphere.h"
#include "unittest/TestSuite.h"

static void verifyInit(int line, CollisionSphere * sphere, void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius) {
	TestCase_assert(sphere != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(sphere->dispose == CollisionSphere_dispose, "Expected %p but got %p (line %d)", CollisionSphere_dispose, sphere->dispose, line);
	TestCase_assert(sphere->shapeType == COLLISION_SHAPE_SPHERE, "Expected %d but got %d (line %d)", COLLISION_SHAPE_SPHERE, sphere->shapeType, line);
	TestCase_assert(sphere->owner == owner, "Expected %p but got %p (line %d)", owner, sphere->owner, line);
	TestCase_assert(sphere->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, sphere->collisionCallback, line);
	TestCase_assert(sphere->interpolate == CollisionSphere_interpolate, "Expected %p but got %p (line %d)", CollisionSphere_interpolate, sphere->interpolate, line);
	TestCase_assert(sphere->isStatic == CollisionSphere_isStatic, "Expected %p but got %p (line %d)", CollisionSphere_isStatic, sphere->isStatic, line);
	TestCase_assert(sphere->getCollisionBounds == CollisionSphere_getCollisionBounds, "Expected %p but got %p (line %d)", CollisionSphere_getCollisionBounds, sphere->getCollisionBounds, line);
	TestCase_assert(sphere->position.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, sphere->position.x, line);
	TestCase_assert(sphere->position.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, sphere->position.y, line);
	TestCase_assert(sphere->position.z == position.z, "Expected 0x%05X but got 0x%05X (line %d)", position.z, sphere->position.z, line);
	TestCase_assert(sphere->lastPosition.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, sphere->lastPosition.x, line);
	TestCase_assert(sphere->lastPosition.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, sphere->lastPosition.y, line);
	TestCase_assert(sphere->lastPosition.z == position.z, "Expected 0x%05X but got 0x%05X (line %d)", position.z, sphere->lastPosition.z, line);
	TestCase_assert(sphere->radius == radius, "Expected 0x%05X but got 0x%05X (line %d)", radius, sphere->radius, line);
}

static void testInit() {
	CollisionSphere sphere, * spherePtr;
	
	memset(&sphere, 0x00, sizeof(sphere));
	CollisionSphere_init(&sphere, NULL, NULL, VECTOR3x_ZERO, 0x00000);
	verifyInit(__LINE__, &sphere, NULL, NULL, VECTOR3x_ZERO, 0x00000);
	CollisionSphere_dispose(&sphere);
	
	memset(&sphere, 0xFF, sizeof(sphere));
	CollisionSphere_init(&sphere, NULL, NULL, VECTOR3x_ZERO, 0x00000);
	verifyInit(__LINE__, &sphere, NULL, NULL, VECTOR3x_ZERO, 0x00000);
	CollisionSphere_dispose(&sphere);
	
	memset(&sphere, 0x00, sizeof(sphere));
	CollisionSphere_init(&sphere, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), 0x40000);
	verifyInit(__LINE__, &sphere, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), 0x40000);
	CollisionSphere_dispose(&sphere);
	
	spherePtr = CollisionSphere_create(NULL, NULL, VECTOR3x_ZERO, 0x00000);
	verifyInit(__LINE__, spherePtr, NULL, NULL, VECTOR3x_ZERO, 0x00000);
	CollisionSphere_dispose(spherePtr);
	
	spherePtr = CollisionSphere_create((void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), 0x40000);
	verifyInit(__LINE__, spherePtr, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), 0x40000);
	CollisionSphere_dispose(spherePtr);
}

static void testUpdatePosition() {
	CollisionSphere * sphere;
	
	sphere = CollisionSphere_create(NULL, NULL, VECTOR3x(0x10000, 0x20000, 0x30000), 0x00000);
	TestCase_assert(sphere->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", sphere->position.x);
	TestCase_assert(sphere->position.y == 0x20000, "Expected 0x20000 but got 0x%05X", sphere->position.y);
	TestCase_assert(sphere->position.z == 0x30000, "Expected 0x30000 but got 0x%05X", sphere->position.z);
	TestCase_assert(sphere->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", sphere->lastPosition.x);
	TestCase_assert(sphere->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", sphere->lastPosition.y);
	TestCase_assert(sphere->lastPosition.z == 0x30000, "Expected 0x30000 but got 0x%05X", sphere->lastPosition.z);
	
	CollisionSphere_updatePosition(sphere, VECTOR3x(0x00000, 0x00000, 0x00000));
	TestCase_assert(sphere->position.x == 0x00000, "Expected 0x00000 but got 0x%05X", sphere->position.x);
	TestCase_assert(sphere->position.y == 0x00000, "Expected 0x00000 but got 0x%05X", sphere->position.y);
	TestCase_assert(sphere->position.z == 0x00000, "Expected 0x00000 but got 0x%05X", sphere->position.z);
	TestCase_assert(sphere->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", sphere->lastPosition.x);
	TestCase_assert(sphere->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", sphere->lastPosition.y);
	TestCase_assert(sphere->lastPosition.z == 0x30000, "Expected 0x30000 but got 0x%05X", sphere->lastPosition.z);
	
	CollisionSphere_updatePosition(sphere, VECTOR3x(0x08000, 0x0C000, 0x04000));
	TestCase_assert(sphere->position.x == 0x08000, "Expected 0x08000 but got 0x%05X", sphere->position.x);
	TestCase_assert(sphere->position.y == 0x0C000, "Expected 0x0C000 but got 0x%05X", sphere->position.y);
	TestCase_assert(sphere->position.z == 0x04000, "Expected 0x04000 but got 0x%05X", sphere->position.z);
	TestCase_assert(sphere->lastPosition.x == 0x00000, "Expected 0x00000 but got 0x%05X", sphere->lastPosition.x);
	TestCase_assert(sphere->lastPosition.y == 0x00000, "Expected 0x00000 but got 0x%05X", sphere->lastPosition.y);
	TestCase_assert(sphere->lastPosition.z == 0x00000, "Expected 0x00000 but got 0x%05X", sphere->lastPosition.z);
	
	CollisionSphere_dispose(sphere);
}

static void testInterpolate() {
	CollisionSphere * sphere;
	
	sphere = CollisionSphere_create(NULL, NULL, VECTOR3x(0x00000, 0x00000, 0x00000), 0x00000);
	CollisionSphere_updatePosition(sphere, VECTOR3x(0x10000, 0x10000, 0x10000));
	CollisionSphere_interpolate(sphere, 0x08000);
	
	TestCase_assert(sphere->lastPosition.x == 0x08000, "Expected 0x08000 but got 0x%05X", sphere->lastPosition.x);
	TestCase_assert(sphere->lastPosition.y == 0x08000, "Expected 0x08000 but got 0x%05X", sphere->lastPosition.y);
	TestCase_assert(sphere->lastPosition.z == 0x08000, "Expected 0x08000 but got 0x%05X", sphere->lastPosition.z);
	TestCase_assert(sphere->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", sphere->position.x);
	TestCase_assert(sphere->position.y == 0x10000, "Expected 0x10000 but got 0x%05X", sphere->position.y);
	TestCase_assert(sphere->position.z == 0x10000, "Expected 0x10000 but got 0x%05X", sphere->position.z);
	
	CollisionSphere_updatePosition(sphere, VECTOR3x(0x20000, 0x30000, 0x40000));
	CollisionSphere_interpolate(sphere, 0x04000);
	
	TestCase_assert(sphere->lastPosition.x == 0x14000, "Expected 0x14000 but got 0x%05X", sphere->lastPosition.x);
	TestCase_assert(sphere->lastPosition.y == 0x18000, "Expected 0x18000 but got 0x%05X", sphere->lastPosition.y);
	TestCase_assert(sphere->lastPosition.z == 0x1C000, "Expected 0x1C000 but got 0x%05X", sphere->lastPosition.z);
	TestCase_assert(sphere->position.x == 0x20000, "Expected 0x20000 but got 0x%05X", sphere->position.x);
	TestCase_assert(sphere->position.y == 0x30000, "Expected 0x30000 but got 0x%05X", sphere->position.y);
	TestCase_assert(sphere->position.z == 0x40000, "Expected 0x40000 but got 0x%05X", sphere->position.z);
	
	CollisionSphere_dispose(sphere);
}

static void testIsStatic() {
	CollisionSphere * sphere;
	bool result;
	
	sphere = CollisionSphere_create(NULL, NULL, VECTOR3x(0x00000, 0x00000, 0x00000), 0x10000);
	result = CollisionSphere_isStatic(sphere);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionSphere_updatePosition(sphere, VECTOR3x(0x10000, 0x10000, 0x10000));
	result = CollisionSphere_isStatic(sphere);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionSphere_updatePosition(sphere, VECTOR3x(0x10000, 0x10000, 0x10000));
	result = CollisionSphere_isStatic(sphere);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionSphere_dispose(sphere);
}

static void testGetCollisionBounds() {
	CollisionSphere * sphere;
	Box6x bounds;
	
	sphere = CollisionSphere_create(NULL, NULL, VECTOR3x_ZERO, 0x10000);
	bounds = CollisionSphere_getCollisionBounds(sphere);
	TestCase_assert(bounds.left == -0x10000 && bounds.right == 0x10000 &&
	                bounds.bottom == -0x10000 && bounds.top == 0x10000 &&
	                bounds.back == -0x10000 && bounds.front == 0x10000,
	                "Expected {0xFFFF0000, 0x10000, 0xFFFF0000, 0x10000, 0xFFFF0000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionSphere_updatePosition(sphere, VECTOR3x(0x10000, 0x10000, 0x10000));
	bounds = CollisionSphere_getCollisionBounds(sphere);
	TestCase_assert(bounds.left == -0x10000 && bounds.right == 0x20000 &&
	                bounds.bottom == -0x10000 && bounds.top == 0x20000 &&
	                bounds.back == -0x10000 && bounds.front == 0x20000,
	                "Expected {0xFFFF0000, 0x20000, 0xFFFF0000, 0x20000, 0xFFFF0000, 0x20000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionSphere_updatePosition(sphere, VECTOR3x(-0x10000, -0x10000, -0x10000));
	bounds = CollisionSphere_getCollisionBounds(sphere);
	TestCase_assert(bounds.left == -0x20000 && bounds.right == 0x20000 &&
	                bounds.bottom == -0x20000 && bounds.top == 0x20000 &&
	                bounds.back == -0x20000 && bounds.front == 0x20000,
	                "Expected {0xFFFE0000, 0x20000, 0xFFFE0000, 0x20000, 0xFFFE0000, 0x20000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	sphere->radius = 0x08000;
	bounds = CollisionSphere_getCollisionBounds(sphere);
	TestCase_assert(bounds.left == -0x18000 && bounds.right == 0x18000 &&
	                bounds.bottom == -0x18000 && bounds.top == 0x18000 &&
	                bounds.back == -0x18000 && bounds.front == 0x18000,
	                "Expected {0xFFFE8000, 0x18000, 0xFFFE8000, 0x18000, 0xFFFE8000, 0x18000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionSphere_dispose(sphere);
}

TEST_SUITE(CollisionSphereTest,
           testInit,
           testUpdatePosition,
           testInterpolate,
           testIsStatic,
           testGetCollisionBounds)
