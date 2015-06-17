#include "collision/CollisionResolver.h"
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
	CollisionResolver_init(&collisionResolver, NULL, false);
	verifyInit(__LINE__, &collisionResolver, NULL);
	CollisionResolver_dispose(&collisionResolver);
	
	memset(&collisionResolver, 0xFF, sizeof(collisionResolver));
	CollisionResolver_init(&collisionResolver, NULL, false);
	verifyInit(__LINE__, &collisionResolver, NULL);
	CollisionResolver_dispose(&collisionResolver);
	
	collisionResolverPtr = CollisionResolver_create(NULL, false);
	verifyInit(__LINE__, collisionResolverPtr, NULL);
	CollisionResolver_dispose(collisionResolverPtr);
	
	intersectionManager = IntersectionManager_create();
	collisionResolverPtr = CollisionResolver_create(intersectionManager, false);
	verifyInit(__LINE__, collisionResolverPtr, intersectionManager);
	CollisionResolver_dispose(collisionResolverPtr);
	IntersectionManager_dispose(intersectionManager);
}

static void testAddObject() {
	CollisionResolver * collisionResolver;
	CollisionObject * object1, * object2;
	
	object1 = CollisionObject_create(NULL, 0, NULL);
	object2 = CollisionObject_create(NULL, 1, NULL);
	
	collisionResolver = CollisionResolver_create(NULL, false);
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
	CollisionObject_dispose(object1);
	CollisionObject_dispose(object2);
}

static void testRemoveObject() {
	CollisionResolver * collisionResolver;
	CollisionObject * object1, * object2, * object3;
	
	object1 = CollisionObject_create(NULL, 0, NULL);
	object2 = CollisionObject_create(NULL, 1, NULL);
	object3 = CollisionObject_create(NULL, 2, NULL);
	
	collisionResolver = CollisionResolver_create(NULL, false);
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
	CollisionObject_dispose(object1);
	CollisionObject_dispose(object2);
	CollisionObject_dispose(object3);
}

static CollisionObject * testObject1, * testObject2, * testObject3, * testObject4;

static bool querySingleHandler1(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	if (object2 == testObject1) {
		*outTime = 0x00000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		return true;
	}
	if (object2 == testObject2) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		return true;
	}
	if (object2 == testObject3) {
		*outTime = 0x0B000;
		*outNormal = VECTOR3x(0x00000, 0x10000, 0x00000);
		return true;
	}
	if (object2 == testObject4) {
		*outTime = 0x04000;
		*outNormal = VECTOR3x(0x00000, 0x00000, 0x10000);
		return true;
	}
	return false;
}

static bool querySingleHandler2(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	return false;
}

static void testQuerySingle() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	bool result;
	CollisionRecord collisionRecord;
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 1, querySingleHandler1);
	IntersectionManager_setHandler(intersectionManager, 1, 1, querySingleHandler2);
	
	testObject1 = CollisionObject_create(NULL, 0, NULL);
	testObject2 = CollisionObject_create(NULL, 1, NULL);
	testObject3 = CollisionObject_create(NULL, 1, NULL);
	testObject4 = CollisionObject_create(NULL, 1, NULL);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	
	// Verify no collision with only object1 added
	CollisionResolver_addObject(collisionResolver, testObject1);
	result = CollisionResolver_querySingle(collisionResolver, testObject1, &collisionRecord);
	TestCase_assert(!result, "Expected false but got true");
	
	// Verify collision with object2
	CollisionResolver_addObject(collisionResolver, testObject2);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObject1, &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObject2, "Expected %p but got %p", testObject2, collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x10000 && collisionRecord.normal.y == 0x00000 && collisionRecord.normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify still collision with object2 after object3 (with later collision) added
	CollisionResolver_addObject(collisionResolver, testObject3);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObject1, &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObject2, "Expected %p but got %p", testObject2, collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x10000 && collisionRecord.normal.y == 0x00000 && collisionRecord.normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify collision with earliest object4 takes priority over object2
	CollisionResolver_addObject(collisionResolver, testObject4);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObject1, &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObject4, "Expected %p but got %p", testObject4, collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x04000, "Expected 0x04000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x00000 && collisionRecord.normal.y == 0x00000 && collisionRecord.normal.z == 0x10000, "Expected {0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify collision with latest collision object3 when object2 and object4 are no longer present
	CollisionResolver_removeObject(collisionResolver, testObject2);
	CollisionResolver_removeObject(collisionResolver, testObject4);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObject1, &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObject3, "Expected %p but got %p", testObject3, collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x0B000, "Expected 0x0B000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x00000 && collisionRecord.normal.y == 0x10000 && collisionRecord.normal.z == 0x00000, "Expected {0x00000, 0x10000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify intersection test shape type order is swapped correctly and normals reversed
	CollisionResolver_addObject(collisionResolver, testObject1);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObject3, &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObject3, "Expected %p but got %p", testObject3, collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObject1, "Expected %p but got %p", testObject1, collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x0B000, "Expected 0x0B000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x00000 && collisionRecord.normal.y == (fixed16_16) 0xFFFF0000 && collisionRecord.normal.z == 0x00000, "Expected {0x00000, 0xFFFF0000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify no collision occurs using querySingleHandler2 by changing object1's shape type
	testObject1->shapeType = 1;
	result = CollisionResolver_querySingle(collisionResolver, testObject1, &collisionRecord);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObject1);
	CollisionObject_dispose(testObject2);
	CollisionObject_dispose(testObject3);
	CollisionObject_dispose(testObject4);
}

static bool findEarliestHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	if (object2 == testObject2) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		return true;
	}
	if (object2 == testObject3) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		return true;
	}
	if (object2 == testObject4) {
		*outTime = 0x08001;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		return true;
	}
	return false;
}

static void testFindEarliest() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	size_t resultCount;
	CollisionRecord collisionRecords[3], unmodifiedCollisionRecord;
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 1, querySingleHandler1);
	IntersectionManager_setHandler(intersectionManager, 1, 1, querySingleHandler2);
	IntersectionManager_setHandler(intersectionManager, 2, 1, findEarliestHandler);
	
	testObject1 = CollisionObject_create(NULL, 0, NULL);
	testObject2 = CollisionObject_create(NULL, 1, NULL);
	testObject3 = CollisionObject_create(NULL, 1, NULL);
	testObject4 = CollisionObject_create(NULL, 1, NULL);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	
	// Verify no collision with only object1 added
	CollisionResolver_addObject(collisionResolver, testObject1);
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 0, "Expected 0 but got " SIZE_T_FORMAT, resultCount);
	
	// Verify collision with object2
	CollisionResolver_addObject(collisionResolver, testObject2);
	memset(collisionRecords, 0xFF, sizeof(collisionRecords));
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObject2, "Expected %p but got %p", testObject2, collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x10000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	
	// Verify still collision with object2 after object3 (with later collision) added
	CollisionResolver_addObject(collisionResolver, testObject3);
	memset(&collisionRecords[0], 0xFF, sizeof(collisionRecords[0]));
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObject2, "Expected %p but got %p", testObject2, collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x10000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	
	// Verify collision with earliest object4 takes priority over object2
	CollisionResolver_addObject(collisionResolver, testObject4);
	memset(&collisionRecords[0], 0xFF, sizeof(collisionRecords[0]));
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObject4, "Expected %p but got %p", testObject4, collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x04000, "Expected 0x04000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x00000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x10000, "Expected {0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	
	// Verify collision with latest collision object3 when object2 and object4 are no longer present
	CollisionResolver_removeObject(collisionResolver, testObject2);
	CollisionResolver_removeObject(collisionResolver, testObject4);
	memset(&collisionRecords[0], 0xFF, sizeof(collisionRecords[0]));
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObject3, "Expected %p but got %p", testObject3, collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x0B000, "Expected 0x0B000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x00000 && collisionRecords[0].normal.y == 0x10000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x00000, 0x10000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	
	// Verify no collision occurs using querySingleHandler2 by changing object1's shape type
	testObject1->shapeType = 1;
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 0, "Expected 0 but got " SIZE_T_FORMAT, resultCount);
	
	// Verify multiple simultaneous collisions are reported (and only of earliest time; testObject4 collides later than 2 and 3)
	testObject1->shapeType = 2;
	CollisionResolver_addObject(collisionResolver, testObject4);
	CollisionResolver_addObject(collisionResolver, testObject2);
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 2, "Expected 2 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObject2 || collisionRecords[0].object2 == testObject3, "Expected %p or %p but got %p", testObject2, testObject3, collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x10000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	TestCase_assert(collisionRecords[1].object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecords[1].object1);
	if (collisionRecords[0].object2 == testObject2) {
		TestCase_assert(collisionRecords[1].object2 == testObject3, "Expected %p but got %p", testObject3, collisionRecords[1].object2);
	} else {
		TestCase_assert(collisionRecords[1].object2 == testObject2, "Expected %p but got %p", testObject2, collisionRecords[1].object2);
	}
	TestCase_assert(collisionRecords[1].time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecords[1].time);
	TestCase_assert(collisionRecords[1].normal.x == 0x10000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[1].normal.x, collisionRecords[1].normal.y, collisionRecords[1].normal.z);
	
	// Verify returned results are truncated if they don't fit
	memset(&collisionRecords[1], 0xFF, sizeof(collisionRecords[1]));
	unmodifiedCollisionRecord = collisionRecords[1];
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 1);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObject1, "Expected %p but got %p", testObject1, collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObject2 || collisionRecords[0].object2 == testObject3, "Expected %p or %p but got %p", testObject2, testObject3, collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x10000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	TestCase_assert(!memcmp(&collisionRecords[1], &unmodifiedCollisionRecord, sizeof(collisionRecords[1])), "Array element 1 was unexpectedly modified");
	
	// Verify returned results are truncated if they don't fit
	memset(&collisionRecords[0], 0xFF, sizeof(collisionRecords[0]));
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 0);
	TestCase_assert(resultCount == 0, "Expected 0 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(!memcmp(&collisionRecords[0], &unmodifiedCollisionRecord, sizeof(collisionRecords[0])), "Array element 0 was unexpectedly modified");
	TestCase_assert(!memcmp(&collisionRecords[1], &unmodifiedCollisionRecord, sizeof(collisionRecords[1])), "Array element 1 was unexpectedly modified");
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObject1);
	CollisionObject_dispose(testObject2);
	CollisionObject_dispose(testObject3);
	CollisionObject_dispose(testObject4);
}

static void testResolveAll() {
	TestCase_assert(false, "Unimplemented");
}

TEST_SUITE(CollisionResolverTest,
           testInit,
           testAddObject,
           testRemoveObject,
           testQuerySingle,
           testFindEarliest,
           testResolveAll)
