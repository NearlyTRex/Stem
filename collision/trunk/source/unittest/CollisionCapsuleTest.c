#include "collision/CollisionCapsule.h"
#include "collision/CollisionShared.h"
#include "unittest/TestSuite.h"

static void verifyInit(int line, CollisionCapsule * capsule, void * owner, CollisionCallback collisionCallback, Vector3x position, fixed16_16 radius, fixed16_16 cylinderHeight) {
	TestCase_assert(capsule != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(capsule->dispose == CollisionCapsule_dispose, "Expected %p but got %p (line %d)", CollisionCapsule_dispose, capsule->dispose, line);
	TestCase_assert(capsule->shapeType == COLLISION_SHAPE_CAPSULE, "Expected %d but got %d (line %d)", COLLISION_SHAPE_CAPSULE, capsule->shapeType, line);
	TestCase_assert(capsule->owner == owner, "Expected %p but got %p (line %d)", owner, capsule->owner, line);
	TestCase_assert(capsule->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, capsule->collisionCallback, line);
	TestCase_assert(capsule->interpolate == CollisionCapsule_interpolate, "Expected %p but got %p (line %d)", CollisionCapsule_interpolate, capsule->interpolate, line);
	TestCase_assert(capsule->position.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, capsule->position.x, line);
	TestCase_assert(capsule->position.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, capsule->position.y, line);
	TestCase_assert(capsule->position.z == position.z, "Expected 0x%05X but got 0x%05X (line %d)", position.z, capsule->position.z, line);
	TestCase_assert(capsule->lastPosition.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, capsule->lastPosition.x, line);
	TestCase_assert(capsule->lastPosition.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, capsule->lastPosition.y, line);
	TestCase_assert(capsule->lastPosition.z == position.z, "Expected 0x%05X but got 0x%05X (line %d)", position.z, capsule->lastPosition.z, line);
	TestCase_assert(capsule->radius == radius, "Expected 0x%05X but got 0x%05X (line %d)", radius, capsule->radius, line);
	TestCase_assert(capsule->cylinderHeight == cylinderHeight, "Expected 0x%05X but got 0x%05X (line %d)", cylinderHeight, capsule->cylinderHeight, line);
}

static void testInit() {
	CollisionCapsule capsule, * capsulePtr;
	
	memset(&capsule, 0x00, sizeof(capsule));
	CollisionCapsule_init(&capsule, NULL, NULL, VECTOR3x_ZERO, 0x00000, 0x00000);
	verifyInit(__LINE__, &capsule, NULL, NULL, VECTOR3x_ZERO, 0x00000, 0x00000);
	CollisionCapsule_dispose(&capsule);
	
	memset(&capsule, 0xFF, sizeof(capsule));
	CollisionCapsule_init(&capsule, NULL, NULL, VECTOR3x_ZERO, 0x00000, 0x00000);
	verifyInit(__LINE__, &capsule, NULL, NULL, VECTOR3x_ZERO, 0x00000, 0x00000);
	CollisionCapsule_dispose(&capsule);
	
	memset(&capsule, 0x00, sizeof(capsule));
	CollisionCapsule_init(&capsule, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), 0x40000, 0x50000);
	verifyInit(__LINE__, &capsule, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), 0x40000, 0x50000);
	CollisionCapsule_dispose(&capsule);
	
	capsulePtr = CollisionCapsule_create(NULL, NULL, VECTOR3x_ZERO, 0x00000, 0x00000);
	verifyInit(__LINE__, capsulePtr, NULL, NULL, VECTOR3x_ZERO, 0x00000, 0x00000);
	CollisionCapsule_dispose(capsulePtr);
	
	capsulePtr = CollisionCapsule_create((void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), 0x40000, 0x50000);
	verifyInit(__LINE__, capsulePtr, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), 0x40000, 0x50000);
	CollisionCapsule_dispose(capsulePtr);
}

static void testUpdatePosition() {
	CollisionCapsule * capsule;
	
	capsule = CollisionCapsule_create(NULL, NULL, VECTOR3x(0x10000, 0x20000, 0x30000), 0x00000, 0x00000);
	TestCase_assert(capsule->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", capsule->position.x);
	TestCase_assert(capsule->position.y == 0x20000, "Expected 0x20000 but got 0x%05X", capsule->position.y);
	TestCase_assert(capsule->position.z == 0x30000, "Expected 0x30000 but got 0x%05X", capsule->position.z);
	TestCase_assert(capsule->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", capsule->lastPosition.x);
	TestCase_assert(capsule->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", capsule->lastPosition.y);
	TestCase_assert(capsule->lastPosition.z == 0x30000, "Expected 0x30000 but got 0x%05X", capsule->lastPosition.z);
	
	CollisionCapsule_updatePosition(capsule, VECTOR3x(0x00000, 0x00000, 0x00000));
	TestCase_assert(capsule->position.x == 0x00000, "Expected 0x00000 but got 0x%05X", capsule->position.x);
	TestCase_assert(capsule->position.y == 0x00000, "Expected 0x00000 but got 0x%05X", capsule->position.y);
	TestCase_assert(capsule->position.z == 0x00000, "Expected 0x00000 but got 0x%05X", capsule->position.z);
	TestCase_assert(capsule->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", capsule->lastPosition.x);
	TestCase_assert(capsule->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", capsule->lastPosition.y);
	TestCase_assert(capsule->lastPosition.z == 0x30000, "Expected 0x30000 but got 0x%05X", capsule->lastPosition.z);
	
	CollisionCapsule_updatePosition(capsule, VECTOR3x(0x08000, 0x0C000, 0x04000));
	TestCase_assert(capsule->position.x == 0x08000, "Expected 0x08000 but got 0x%05X", capsule->position.x);
	TestCase_assert(capsule->position.y == 0x0C000, "Expected 0x0C000 but got 0x%05X", capsule->position.y);
	TestCase_assert(capsule->position.z == 0x04000, "Expected 0x04000 but got 0x%05X", capsule->position.z);
	TestCase_assert(capsule->lastPosition.x == 0x00000, "Expected 0x00000 but got 0x%05X", capsule->lastPosition.x);
	TestCase_assert(capsule->lastPosition.y == 0x00000, "Expected 0x00000 but got 0x%05X", capsule->lastPosition.y);
	TestCase_assert(capsule->lastPosition.z == 0x00000, "Expected 0x00000 but got 0x%05X", capsule->lastPosition.z);
	
	CollisionCapsule_dispose(capsule);
}

static void testInterpolate() {
	CollisionCapsule * capsule;
	
	capsule = CollisionCapsule_create(NULL, NULL, VECTOR3x(0x00000, 0x00000, 0x00000), 0x00000, 0x00000);
	CollisionCapsule_updatePosition(capsule, VECTOR3x(0x10000, 0x10000, 0x10000));
	CollisionCapsule_interpolate(capsule, 0x08000);
	
	TestCase_assert(capsule->lastPosition.x == 0x08000, "Expected 0x08000 but got 0x%05X", capsule->lastPosition.x);
	TestCase_assert(capsule->lastPosition.y == 0x08000, "Expected 0x08000 but got 0x%05X", capsule->lastPosition.y);
	TestCase_assert(capsule->lastPosition.z == 0x08000, "Expected 0x08000 but got 0x%05X", capsule->lastPosition.z);
	TestCase_assert(capsule->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", capsule->position.x);
	TestCase_assert(capsule->position.y == 0x10000, "Expected 0x10000 but got 0x%05X", capsule->position.y);
	TestCase_assert(capsule->position.z == 0x10000, "Expected 0x10000 but got 0x%05X", capsule->position.z);
	
	CollisionCapsule_updatePosition(capsule, VECTOR3x(0x20000, 0x30000, 0x40000));
	CollisionCapsule_interpolate(capsule, 0x04000);
	
	TestCase_assert(capsule->lastPosition.x == 0x14000, "Expected 0x14000 but got 0x%05X", capsule->lastPosition.x);
	TestCase_assert(capsule->lastPosition.y == 0x18000, "Expected 0x18000 but got 0x%05X", capsule->lastPosition.y);
	TestCase_assert(capsule->lastPosition.z == 0x1C000, "Expected 0x1C000 but got 0x%05X", capsule->lastPosition.z);
	TestCase_assert(capsule->position.x == 0x20000, "Expected 0x20000 but got 0x%05X", capsule->position.x);
	TestCase_assert(capsule->position.y == 0x30000, "Expected 0x30000 but got 0x%05X", capsule->position.y);
	TestCase_assert(capsule->position.z == 0x40000, "Expected 0x40000 but got 0x%05X", capsule->position.z);
	
	CollisionCapsule_dispose(capsule);
}

TEST_SUITE(CollisionCapsuleTest,
           testInit,
           testUpdatePosition,
           testInterpolate)
