#include "collision/CollisionObject.h"
#include "unittest/TestSuite.h"
#include <string.h>

static void verifyInit(int line, CollisionObject * collisionObject, void * owner, int shapeType, CollisionCallback collisionCallback) {
	TestCase_assert(collisionObject != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(collisionObject->dispose == CollisionObject_dispose, "Expected %p but got %p (line %d)", CollisionObject_dispose, collisionObject->dispose, line);
	TestCase_assert(collisionObject->shapeType == shapeType, "Expected %d but got %d (line %d)", shapeType, collisionObject->shapeType, line);
	TestCase_assert(collisionObject->owner == owner, "Expected %p but got %p (line %d)", owner, collisionObject->owner, line);
	TestCase_assert(collisionObject->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, collisionObject->collisionCallback, line);
	TestCase_assert(collisionObject->ownMask == 0x1, "Expected 0x1 but got 0x%X (line %d)", collisionObject->ownMask, line);
	TestCase_assert(collisionObject->collidableMask == 0xFFFFFFFF, "Expected 0xFFFFFFFF but got 0x%X (line %d)", collisionObject->collidableMask, line);
	TestCase_assert(collisionObject->interpolate == NULL, "Expected NULL but got %p (line %d)", collisionObject->interpolate, line);
	TestCase_assert(collisionObject->isStatic == CollisionObject_isStatic, "Expected %p but got %p (line %d)", CollisionObject_isStatic, collisionObject->isStatic, line);
	TestCase_assert(collisionObject->getCollisionBounds == CollisionObject_getCollisionBounds, "Expected %p but got %p (line %d)", CollisionObject_getCollisionBounds, collisionObject->getCollisionBounds, line);
}

static void testInit() {
	CollisionObject collisionObject, * collisionObjectPtr;
	
	memset(&collisionObject, 0x00, sizeof(collisionObject));
	CollisionObject_init(&collisionObject, NULL, 0, NULL);
	verifyInit(__LINE__, &collisionObject, NULL, 0, NULL);
	CollisionObject_dispose(&collisionObject);
	
	memset(&collisionObject, 0xFF, sizeof(collisionObject));
	CollisionObject_init(&collisionObject, NULL, 1, NULL);
	verifyInit(__LINE__, &collisionObject, NULL, 1, NULL);
	CollisionObject_dispose(&collisionObject);
	
	memset(&collisionObject, 0x00, sizeof(collisionObject));
	CollisionObject_init(&collisionObject, (void *) 0x1234, 2, (CollisionCallback) 0x2345);
	verifyInit(__LINE__, &collisionObject, (void *) 0x1234, 2, (CollisionCallback) 0x2345);
	CollisionObject_dispose(&collisionObject);
	
	collisionObjectPtr = CollisionObject_create(NULL, 0, NULL);
	verifyInit(__LINE__, collisionObjectPtr, NULL, 0, NULL);
	CollisionObject_dispose(collisionObjectPtr);
	
	collisionObjectPtr = CollisionObject_create((void *) 0x1234, 2, (CollisionCallback) 0x2345);
	verifyInit(__LINE__, collisionObjectPtr, (void *) 0x1234, 2, (CollisionCallback) 0x2345);
	CollisionObject_dispose(collisionObjectPtr);
}

static void testIsStatic() {
	CollisionObject * object;
	bool result;
	
	object = CollisionObject_create(NULL, 0, NULL);
	result = CollisionObject_isStatic(object);
	TestCase_assert(!result, "Expected false but got true");
	CollisionObject_dispose(object);
}

static void testGetCollisionBounds() {
	CollisionObject * object;
	Box6x bounds;
	
	object = CollisionObject_create(NULL, 0, NULL);
	bounds = CollisionObject_getCollisionBounds(object);
	TestCase_assert(bounds.left == 0x00000 && bounds.right == 0x10000 && bounds.bottom == 0x00000 && bounds.top == 0x10000 && bounds.back == 0x00000 && bounds.front == 0x10000, "Expected {0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}", bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	CollisionObject_dispose(object);
}

static void testSetMasks() {
	CollisionObject * object, * result;
	
	object = CollisionObject_create(NULL, 0, NULL);
	result = CollisionObject_setMasks(object, 0x0, 0x0);
	TestCase_assert(result == object, "Expected %p but got %p", object, result);
	TestCase_assert(object->ownMask == 0x0, "Expected 0x0 but got 0x%X", object->ownMask);
	TestCase_assert(object->collidableMask == 0x0, "Expected 0x0 but got 0x%X", object->collidableMask);
	CollisionObject_dispose(object);
	
	object = CollisionObject_create(NULL, 0, NULL);
	result = CollisionObject_setMasks(object, 0x01234567, 0x89ABCDEF);
	TestCase_assert(result == object, "Expected %p but got %p", object, result);
	TestCase_assert(object->ownMask == 0x01234567, "Expected 0x01234567 but got 0x%X", object->ownMask);
	TestCase_assert(object->collidableMask == 0x89ABCDEF, "Expected 0x89ABCDEF but got 0x%X", object->collidableMask);
	CollisionObject_dispose(object);
}

TEST_SUITE(CollisionObjectTest,
           testInit,
           testIsStatic,
           testGetCollisionBounds,
           testSetMasks)
