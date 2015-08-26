#include "collision/CollisionObject.h"
#include "unittest/TestSuite.h"
#include <string.h>

static void verifyInit(int line, CollisionObject * collisionObject, void * owner, int shapeType, CollisionCallback collisionCallback) {
	TestCase_assert(collisionObject != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(collisionObject->dispose == CollisionObject_dispose, "Expected %p but got %p (line %d)", CollisionObject_dispose, collisionObject->dispose, line);
	TestCase_assert(collisionObject->shapeType == shapeType, "Expected %d but got %d (line %d)", shapeType, collisionObject->shapeType, line);
	TestCase_assert(collisionObject->owner == owner, "Expected %p but got %p (line %d)", owner, collisionObject->owner, line);
	TestCase_assert(collisionObject->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, collisionObject->collisionCallback, line);
	TestCase_assert(collisionObject->interpolate == NULL, "Expected NULL but got %p", collisionObject->interpolate);
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

TEST_SUITE(CollisionObjectTest,
           testInit)
