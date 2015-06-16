#include "collision/CollisionResolver.h"
#include "unittest/CollisionTestObject.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"
#include <string.h>

static void verifyInit(int line, CollisionResolver * collisionResolver, IntersectionManager * intersectionManager) {
	TestCase_assert(collisionResolver != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(collisionResolver->dispose == CollisionResolver_dispose, "Expected %p but got %p (line %d)", CollisionResolver_dispose, collisionResolver->dispose);
	TestCase_assert(collisionResolver->objectCount == 0, "Expected 0 but got " SIZE_T_FORMAT " (line %d)", collisionResolver->objectCount, line);
	TestCase_assert(collisionResolver->intersectionManager == intersectionManager, "Expected %p but got %p", intersectionManager, collisionResolver->intersectionManager);
}

static void testInit() {
	CollisionResolver collisionResolver, * collisionResolverPtr;
	IntersectionManager * intersectionManager;
	
	memset(&collisionResolver, 0x00, sizeof(collisionResolver));
	CollisionResolver_init(&collisionResolver, NULL);
	verifyInit(__LINE__, &collisionResolver, NULL);
	CollisionResolver_dispose(&collisionResolver);
	
	memset(&collisionResolver, 0xFF, sizeof(collisionResolver));
	CollisionResolver_init(&collisionResolver, NULL);
	verifyInit(__LINE__, &collisionResolver, NULL);
	CollisionResolver_dispose(&collisionResolver);
	
	collisionResolverPtr = CollisionResolver_create(NULL);
	verifyInit(__LINE__, collisionResolverPtr, NULL);
	CollisionResolver_dispose(collisionResolverPtr);
	
	intersectionManager = IntersectionManager_create();
	collisionResolverPtr = CollisionResolver_create(intersectionManager);
	verifyInit(__LINE__, collisionResolverPtr, intersectionManager);
	CollisionResolver_dispose(collisionResolverPtr);
	IntersectionManager_dispose(intersectionManager);
}

static void testAddObject() {
	CollisionResolver * collisionResolver;
	CollisionTestObject * object1, * object2;
	
	object1 = CollisionTestObject_create(NULL, 0, NULL);
	object2 = CollisionTestObject_create(NULL, 1, NULL);
	
	collisionResolver = CollisionResolver_create(NULL);
	verifyInit(__LINE__, collisionResolver, NULL);
	CollisionResolver_addObject(collisionResolver, object1);
	TestCase_assert(collisionResolver->objectCount == 1, "Expected 1 but got " SIZE_T_FORMAT, collisionResolver->objectCount);
	TestCase_assert(collisionResolver->objects[0] == (CollisionObject *) object1, "Expected %p but got %p", object1, collisionResolver->objects[0]);
	
	CollisionResolver_addObject(collisionResolver, object2);
	TestCase_assert(collisionResolver->objectCount == 2, "Expected 2 but got " SIZE_T_FORMAT, collisionResolver->objectCount);
	TestCase_assert(collisionResolver->objects[1] == (CollisionObject *) object2, "Expected %p but got %p", object2, collisionResolver->objects[1]);
	
	CollisionResolver_addObject(collisionResolver, object1);
	TestCase_assert(collisionResolver->objectCount == 2, "Expected 2 but got " SIZE_T_FORMAT, collisionResolver->objectCount);
	TestCase_assert(collisionResolver->objects[0] == (CollisionObject *) object1, "Expected %p but got %p", object1, collisionResolver->objects[0]);
	TestCase_assert(collisionResolver->objects[1] == (CollisionObject *) object2, "Expected %p but got %p", object2, collisionResolver->objects[1]);
	
	CollisionResolver_dispose(collisionResolver);
	CollisionTestObject_dispose(object1);
	CollisionTestObject_dispose(object2);
}

static void testRemoveObject() {
	CollisionResolver * collisionResolver;
	CollisionTestObject * object1, * object2, * object3;
	
	object1 = CollisionTestObject_create(NULL, 0, NULL);
	object2 = CollisionTestObject_create(NULL, 1, NULL);
	object3 = CollisionTestObject_create(NULL, 2, NULL);
	
	collisionResolver = CollisionResolver_create(NULL);
	verifyInit(__LINE__, collisionResolver, NULL);
	CollisionResolver_addObject(collisionResolver, object1);
	CollisionResolver_addObject(collisionResolver, object2);
	CollisionResolver_addObject(collisionResolver, object3);
	TestCase_assert(collisionResolver->objectCount == 3, "Expected 3 but got " SIZE_T_FORMAT, collisionResolver->objectCount);
	
	CollisionResolver_removeObject(collisionResolver, object2);
	TestCase_assert(collisionResolver->objectCount == 2, "Expected 2 but got " SIZE_T_FORMAT, collisionResolver->objectCount);
	TestCase_assert(collisionResolver->objects[0] == (CollisionObject *) object1, "Expected %p but got %p", object1, collisionResolver->objects[0]);
	TestCase_assert(collisionResolver->objects[1] == (CollisionObject *) object3, "Expected %p but got %p", object3, collisionResolver->objects[1]);
	
	CollisionResolver_removeObject(collisionResolver, object3);
	TestCase_assert(collisionResolver->objectCount == 1, "Expected 1 but got " SIZE_T_FORMAT, collisionResolver->objectCount);
	TestCase_assert(collisionResolver->objects[0] == (CollisionObject *) object1, "Expected %p but got %p", object1, collisionResolver->objects[0]);
	
	CollisionResolver_removeObject(collisionResolver, object3);
	TestCase_assert(collisionResolver->objectCount == 1, "Expected 1 but got " SIZE_T_FORMAT, collisionResolver->objectCount);
	
	CollisionResolver_removeObject(collisionResolver, object1);
	TestCase_assert(collisionResolver->objectCount == 0, "Expected 0 but got " SIZE_T_FORMAT, collisionResolver->objectCount);
	
	CollisionResolver_dispose(collisionResolver);
	CollisionTestObject_dispose(object1);
	CollisionTestObject_dispose(object2);
	CollisionTestObject_dispose(object3);
}

/*
static void testQuery_cylinder_cylinder() {
	CollisionResolver * collisionResolver;
	CollisionObject * object1, * object2;
	bool collision;
	CollisionObject * collidingObject;
	Vector3x normal;
	fixed16_16 frameTime;
	
	object1 = CollisionObject_createCylinder(NULL, 0, 0x10000, 0x10000, VECTOR3x_ZERO, NULL);
	object2 = CollisionObject_createCylinder(NULL, 1, 0x10000, 0x10000, VECTOR3x(0x20000, 0, 0), NULL);
	collisionResolver = CollisionResolver_create(NULL);
	CollisionResolver_addObject(collisionResolver, object1);
	
	CollisionObject_updatePosition(object2, VECTOR3x_ZERO);
	collidingObject = NULL;
	normal.x = normal.y = normal.z = -1;
	frameTime = -1;
	collision = CollisionResolver_querySingle(collisionResolver, object2, &collidingObject, &normal, &frameTime);
	TestCase_assert(collision, "Expected true but got false");
	TestCase_assert(collidingObject == object1, "Expected %p but got %p", object1, collidingObject);
	TestCase_assert(normal.x == 0x10000, "Expected 0x10000 but got 0x%05X", normal.x);
	TestCase_assert(normal.y == 0x00000, "Expected 0x00000 but got 0x%05X", normal.y);
	TestCase_assert(normal.z == 0x00000, "Expected 0x00000 but got 0x%05X", normal.z);
	TestCase_assert(frameTime == 0x00000, "Expected 0x00000 but got 0x%05X", frameTime);
	
	CollisionObject_updatePosition(object2, VECTOR3x(0x00000, 0x00000, -0x30000));
	CollisionObject_updatePosition(object2, VECTOR3x_ZERO);
	collidingObject = NULL;
	normal.x = normal.y = normal.z = -1;
	frameTime = -1;
	collision = CollisionResolver_querySingle(collisionResolver, object2, &collidingObject, &normal, &frameTime);
	TestCase_assert(collision, "Expected true but got false");
	TestCase_assert(collidingObject == object1, "Expected %p but got %p", object1, collidingObject);
	TestCase_assert(normal.x == 0x00000, "Expected 0x00000 but got 0x%05X", normal.x);
	TestCase_assert(normal.y == 0x00000, "Expected 0x00000 but got 0x%05X", normal.y);
	TestCase_assert(normal.z == -0x10000, "Expected 0xFFFF0000 but got 0x%05X", normal.z);
	TestCase_assert(frameTime == 0x08000, "Expected 0x08000 but got 0x%05X", frameTime);
	
	CollisionObject_updatePosition(object2, VECTOR3x(0x00000, 0x00000, -0x30000));
	CollisionObject_updatePosition(object2, VECTOR3x_ZERO);
	collision = CollisionResolver_querySingle(collisionResolver, object2, NULL, NULL, NULL);
	TestCase_assert(collision, "Expected true but got false");
	
	CollisionObject_updatePosition(object2, VECTOR3x(-0x30000, 0x00000, 0x00000));
	CollisionObject_updatePosition(object2, VECTOR3x(-0x20000, 0x00000, 0x00000));
	collision = CollisionResolver_querySingle(collisionResolver, object2, &collidingObject, &normal, &frameTime);
	TestCase_assert(!collision, "Expected false but got true");
	
	// Test cylinder XZ angles
	// Test cylinder top and bottom
	// Test closer/farther collisions with multiple objects
	// Test sweeping
	
	TestCase_assert(false, "Unimplemented");
}
*/

static void testIntersectionTest() {
	TestCase_assert(false, "Unimplemented");
}

static void testQuerySingle() {
	TestCase_assert(false, "Unimplemented");
}

static void testResolveAll() {
	TestCase_assert(false, "Unimplemented");
}

TEST_SUITE(CollisionResolverTest,
           testInit,
           testAddObject,
           testRemoveObject,
           testIntersectionTest,
           testQuerySingle,
           testResolveAll)
