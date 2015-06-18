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
	CollisionResolver_init(&collisionResolver, NULL, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	verifyInit(__LINE__, &collisionResolver, NULL);
	CollisionResolver_dispose(&collisionResolver);
	
	memset(&collisionResolver, 0xFF, sizeof(collisionResolver));
	CollisionResolver_init(&collisionResolver, NULL, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	verifyInit(__LINE__, &collisionResolver, NULL);
	CollisionResolver_dispose(&collisionResolver);
	
	collisionResolverPtr = CollisionResolver_create(NULL, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	verifyInit(__LINE__, collisionResolverPtr, NULL);
	CollisionResolver_dispose(collisionResolverPtr);
	
	intersectionManager = IntersectionManager_create();
	collisionResolverPtr = CollisionResolver_create(intersectionManager, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	verifyInit(__LINE__, collisionResolverPtr, intersectionManager);
	CollisionResolver_dispose(collisionResolverPtr);
	IntersectionManager_dispose(intersectionManager);
}

static void testAddObject() {
	CollisionResolver * collisionResolver;
	CollisionObject * object1, * object2;
	
	object1 = CollisionObject_create(NULL, 0, NULL);
	object2 = CollisionObject_create(NULL, 1, NULL);
	
	collisionResolver = CollisionResolver_create(NULL, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
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
	
	collisionResolver = CollisionResolver_create(NULL, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
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

static CollisionObject * testObjects[4];

static bool querySingleHandler1(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	if (object2 == testObjects[0]) {
		*outTime = 0x00000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		return true;
	}
	if (object2 == testObjects[1]) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		return true;
	}
	if (object2 == testObjects[2]) {
		*outTime = 0x0B000;
		*outNormal = VECTOR3x(0x00000, 0x10000, 0x00000);
		return true;
	}
	if (object2 == testObjects[3]) {
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
	
	testObjects[0] = CollisionObject_create(NULL, 0, NULL);
	testObjects[1] = CollisionObject_create(NULL, 1, NULL);
	testObjects[2] = CollisionObject_create(NULL, 1, NULL);
	testObjects[3] = CollisionObject_create(NULL, 1, NULL);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	
	// Verify no collision with only object1 added
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	result = CollisionResolver_querySingle(collisionResolver, testObjects[0], &collisionRecord);
	TestCase_assert(!result, "Expected false but got true");
	
	// Verify collision with object2
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObjects[0], &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObjects[1], "Expected %p but got %p", testObjects[1], collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x10000 && collisionRecord.normal.y == 0x00000 && collisionRecord.normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify still collision with object2 after object3 (with later collision) added
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObjects[0], &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObjects[1], "Expected %p but got %p", testObjects[1], collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x10000 && collisionRecord.normal.y == 0x00000 && collisionRecord.normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify collision with earliest object4 takes priority over object2
	CollisionResolver_addObject(collisionResolver, testObjects[3]);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObjects[0], &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObjects[3], "Expected %p but got %p", testObjects[3], collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x04000, "Expected 0x04000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x00000 && collisionRecord.normal.y == 0x00000 && collisionRecord.normal.z == 0x10000, "Expected {0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify collision with latest collision object3 when object2 and object4 are no longer present
	CollisionResolver_removeObject(collisionResolver, testObjects[1]);
	CollisionResolver_removeObject(collisionResolver, testObjects[3]);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObjects[0], &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObjects[2], "Expected %p but got %p", testObjects[2], collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x0B000, "Expected 0x0B000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x00000 && collisionRecord.normal.y == 0x10000 && collisionRecord.normal.z == 0x00000, "Expected {0x00000, 0x10000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify intersection test shape type order is swapped correctly and normals reversed
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObjects[2], &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObjects[2], "Expected %p but got %p", testObjects[2], collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x0B000, "Expected 0x0B000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x00000 && collisionRecord.normal.y == (fixed16_16) 0xFFFF0000 && collisionRecord.normal.z == 0x00000, "Expected {0x00000, 0xFFFF0000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify no collision occurs using querySingleHandler2 by changing object1's shape type
	testObjects[0]->shapeType = 1;
	result = CollisionResolver_querySingle(collisionResolver, testObjects[0], &collisionRecord);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObjects[0]);
	CollisionObject_dispose(testObjects[1]);
	CollisionObject_dispose(testObjects[2]);
	CollisionObject_dispose(testObjects[3]);
}

static bool findEarliestHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	if (object2 == testObjects[1]) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		return true;
	}
	if (object2 == testObjects[2]) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		return true;
	}
	if (object2 == testObjects[3]) {
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
	
	testObjects[0] = CollisionObject_create(NULL, 0, NULL);
	testObjects[1] = CollisionObject_create(NULL, 1, NULL);
	testObjects[2] = CollisionObject_create(NULL, 1, NULL);
	testObjects[3] = CollisionObject_create(NULL, 1, NULL);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	
	// Verify no collision with only object1 added
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 0, "Expected 0 but got " SIZE_T_FORMAT, resultCount);
	
	// Verify collision with object2
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	memset(collisionRecords, 0xFF, sizeof(collisionRecords));
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObjects[1], "Expected %p but got %p", testObjects[1], collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x10000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	
	// Verify still collision with object2 after object3 (with later collision) added
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	memset(&collisionRecords[0], 0xFF, sizeof(collisionRecords[0]));
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObjects[1], "Expected %p but got %p", testObjects[1], collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x10000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	
	// Verify collision with earliest object4 takes priority over object2
	CollisionResolver_addObject(collisionResolver, testObjects[3]);
	memset(&collisionRecords[0], 0xFF, sizeof(collisionRecords[0]));
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObjects[3], "Expected %p but got %p", testObjects[3], collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x04000, "Expected 0x04000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x00000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x10000, "Expected {0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	
	// Verify collision with latest collision object3 when object2 and object4 are no longer present
	CollisionResolver_removeObject(collisionResolver, testObjects[1]);
	CollisionResolver_removeObject(collisionResolver, testObjects[3]);
	memset(&collisionRecords[0], 0xFF, sizeof(collisionRecords[0]));
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObjects[2], "Expected %p but got %p", testObjects[2], collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x0B000, "Expected 0x0B000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x00000 && collisionRecords[0].normal.y == 0x10000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x00000, 0x10000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	
	// Verify no collision occurs using querySingleHandler2 by changing object1's shape type
	testObjects[0]->shapeType = 1;
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 0, "Expected 0 but got " SIZE_T_FORMAT, resultCount);
	
	// Verify multiple simultaneous collisions are reported (and only of earliest time; testObjects[3] collides later than 2 and 3)
	testObjects[0]->shapeType = 2;
	CollisionResolver_addObject(collisionResolver, testObjects[3]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 3);
	TestCase_assert(resultCount == 2, "Expected 2 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObjects[1] || collisionRecords[0].object2 == testObjects[2], "Expected %p or %p but got %p", testObjects[1], testObjects[2], collisionRecords[0].object2);
	TestCase_assert(collisionRecords[0].time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x10000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	TestCase_assert(collisionRecords[1].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[1].object1);
	if (collisionRecords[0].object2 == testObjects[1]) {
		TestCase_assert(collisionRecords[1].object2 == testObjects[2], "Expected %p but got %p", testObjects[2], collisionRecords[1].object2);
	} else {
		TestCase_assert(collisionRecords[1].object2 == testObjects[1], "Expected %p but got %p", testObjects[1], collisionRecords[1].object2);
	}
	TestCase_assert(collisionRecords[1].time == 0x08000, "Expected 0x08000 but got 0x%05X", collisionRecords[1].time);
	TestCase_assert(collisionRecords[1].normal.x == 0x10000 && collisionRecords[0].normal.y == 0x00000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[1].normal.x, collisionRecords[1].normal.y, collisionRecords[1].normal.z);
	
	// Verify returned results are truncated if they don't fit
	memset(&collisionRecords[1], 0xFF, sizeof(collisionRecords[1]));
	unmodifiedCollisionRecord = collisionRecords[1];
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 1);
	TestCase_assert(resultCount == 1, "Expected 1 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObjects[1] || collisionRecords[0].object2 == testObjects[2], "Expected %p or %p but got %p", testObjects[1], testObjects[2], collisionRecords[0].object2);
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
	CollisionObject_dispose(testObjects[0]);
	CollisionObject_dispose(testObjects[1]);
	CollisionObject_dispose(testObjects[2]);
	CollisionObject_dispose(testObjects[3]);
}

static unsigned int collisionCallbackCalls[3];
static unsigned int interpolateMethodCalls[3];
static unsigned int resolveAllIterations[3];
static int resolveAllLine;
static fixed16_16 intersectionTime;
static fixed16_16 lastTimesliceSizes[3];
static fixed16_16 lastInterpolationAmounts[3];

static unsigned int getTestObjectIndex(CollisionObject * object) {
	unsigned int objectIndex;
	
	for (objectIndex = 0; objectIndex < 3; objectIndex++) {
		if (object == testObjects[objectIndex]) {
			break;
		}
	}
	return objectIndex;
}

static bool resolveAllIntersectionHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal) {
	unsigned int object1Index, object2Index;
	
	object1Index = getTestObjectIndex(object1);
	TestCase_assert(object1Index < 3, "Intersection handler called with unknown object1 %p (line %d)", object1, resolveAllLine);
	object2Index = getTestObjectIndex(object2);
	TestCase_assert(object2Index < 3, "Intersection handler called with unknown object2 %p (line %d)", object2, resolveAllLine);
	
	if (resolveAllIterations[object1Index] > 0 && resolveAllIterations[object2Index] > 0) {
		resolveAllIterations[object1Index]--;
		resolveAllIterations[object2Index]--;
		*outTime = intersectionTime;
		return true;
	}
	
	return false;
}

static void resolveAllCollisionCallback(CollisionRecord collision, fixed16_16 timesliceSize) {
	unsigned int object1Index;
	
	object1Index = getTestObjectIndex(collision.object1);
	TestCase_assert(object1Index < 3, "Resolve callback called with unknown object1 %p (line %d)", collision.object1, resolveAllLine);
	
	collisionCallbackCalls[object1Index]++;
	lastTimesliceSizes[object1Index] = timesliceSize;
}

static void resolveAllTest_CollisionObject_interpolate(CollisionObject * self, fixed16_16 amount) {
	unsigned int object1Index;
	
	object1Index = getTestObjectIndex(self);
	TestCase_assert(object1Index < 3, "Interpolate method called with unknown self %p (line %d)", self, resolveAllLine);
	
	interpolateMethodCalls[object1Index]++;
	lastInterpolationAmounts[object1Index] = amount;
}

static void testResolveAll() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 1, resolveAllIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 0, 2, resolveAllIntersectionHandler);
	
	testObjects[0] = CollisionObject_create(NULL, 0, resolveAllCollisionCallback);
	testObjects[1] = CollisionObject_create(NULL, 1, resolveAllCollisionCallback);
	testObjects[2] = CollisionObject_create(NULL, 2, resolveAllCollisionCallback);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	
	// Verify simplest case (two objects, one collision, one iteration)
	resolveAllIterations[0] = 1;
	resolveAllIterations[1] = 1;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 1, "Expected 1 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 1, "Expected 1 but got %u", collisionCallbackCalls[1]);
	
	// Verify harder case (three objects, three collisions, two iterations)
	resolveAllIterations[0] = 3;
	resolveAllIterations[1] = 2;
	resolveAllIterations[2] = 1;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 2, "Expected 2 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 2, "Expected 2 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 1, "Expected 1 but got %u", collisionCallbackCalls[2]);
	
	// Verify timesliceSize is passed correctly (one iteration, any intersection time)
	resolveAllIterations[0] = 1;
	resolveAllIterations[1] = 1;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 1, "Expected 1 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 1, "Expected 1 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(lastTimesliceSizes[0] == 0x10000, "Expected 0x10000 but got 0x%05X", lastTimesliceSizes[0]);
	TestCase_assert(lastTimesliceSizes[1] == 0x10000, "Expected 0x10000 but got 0x%05X", lastTimesliceSizes[1]);
	
	// Verify timesliceSize is passed correctly (two iterations, intersection time 0.25)
	resolveAllIterations[0] = 2;
	resolveAllIterations[1] = 2;
	intersectionTime = 0x04000;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 2, "Expected 2 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 2, "Expected 2 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(lastTimesliceSizes[0] == 0x0B000, "Expected 0x0B000 but got 0x%05X", lastTimesliceSizes[0]);
	TestCase_assert(lastTimesliceSizes[1] == 0x0B000, "Expected 0x0B000 but got 0x%05X", lastTimesliceSizes[1]);
	
	// Verify timesliceSize is passed correctly (three iterations, intersection time 0.5)
	resolveAllIterations[0] = 3;
	resolveAllIterations[1] = 3;
	intersectionTime = 0x08000;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 3, "Expected 3 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 3, "Expected 3 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(lastTimesliceSizes[0] == 0x04000, "Expected 0x04000 but got 0x%05X", lastTimesliceSizes[0]);
	TestCase_assert(lastTimesliceSizes[1] == 0x04000, "Expected 0x04000 but got 0x%05X", lastTimesliceSizes[1]);
	
	// Verify interpolate is called on CollisionObjects
	testObjects[0]->interpolate = resolveAllTest_CollisionObject_interpolate;
	testObjects[1]->interpolate = resolveAllTest_CollisionObject_interpolate;
	testObjects[2]->interpolate = resolveAllTest_CollisionObject_interpolate;
	resolveAllIterations[0] = 2;
	resolveAllIterations[1] = 2;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	memset(interpolateMethodCalls, 0, sizeof(interpolateMethodCalls));
	intersectionTime = 0x04000;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 2, "Expected 2 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 2, "Expected 2 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 0, "Expected 0 but got %u", collisionCallbackCalls[2]);
	TestCase_assert(interpolateMethodCalls[0] == 1, "Expected 1 but got %u", interpolateMethodCalls[0]);
	TestCase_assert(interpolateMethodCalls[1] == 1, "Expected 1 but got %u", interpolateMethodCalls[1]);
	TestCase_assert(interpolateMethodCalls[2] == 1, "Expected 1 but got %u", interpolateMethodCalls[2]);
	TestCase_assert(lastInterpolationAmounts[0] == 0x04000, "Expected 0x04000 but got 0x%05X", lastInterpolationAmounts[0]);
	TestCase_assert(lastInterpolationAmounts[1] == 0x04000, "Expected 0x04000 but got 0x%05X", lastInterpolationAmounts[1]);
	
	// Verify interpolate is not called for a single iteration
	resolveAllIterations[0] = 1;
	resolveAllIterations[1] = 1;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	memset(interpolateMethodCalls, 0, sizeof(interpolateMethodCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 1, "Expected 1 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 1, "Expected 1 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 0, "Expected 0 but got %u", collisionCallbackCalls[2]);
	TestCase_assert(interpolateMethodCalls[0] == 0, "Expected 0 but got %u", interpolateMethodCalls[0]);
	TestCase_assert(interpolateMethodCalls[1] == 0, "Expected 0 but got %u", interpolateMethodCalls[1]);
	TestCase_assert(interpolateMethodCalls[2] == 0, "Expected 0 but got %u", interpolateMethodCalls[2]);
	
	// Verify interpolate is passed the correct amount parameter
	resolveAllIterations[0] = 3;
	resolveAllIterations[1] = 3;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	memset(interpolateMethodCalls, 0, sizeof(interpolateMethodCalls));
	intersectionTime = 0x08000;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 3, "Expected 3 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 3, "Expected 3 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 0, "Expected 0 but got %u", collisionCallbackCalls[2]);
	TestCase_assert(interpolateMethodCalls[0] == 2, "Expected 2 but got %u", interpolateMethodCalls[0]);
	TestCase_assert(interpolateMethodCalls[1] == 2, "Expected 2 but got %u", interpolateMethodCalls[1]);
	TestCase_assert(interpolateMethodCalls[2] == 2, "Expected 2 but got %u", interpolateMethodCalls[2]);
	TestCase_assert(lastInterpolationAmounts[0] == 0x08000, "Expected 0x08000 but got 0x%05X", lastInterpolationAmounts[0]);
	TestCase_assert(lastInterpolationAmounts[1] == 0x08000, "Expected 0x08000 but got 0x%05X", lastInterpolationAmounts[1]);
	
	// Verify maxIterations is respected (without simultaneous collisions)
	CollisionResolver_dispose(collisionResolver);
	collisionResolver = CollisionResolver_create(intersectionManager, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, 1);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	resolveAllIterations[0] = 2;
	resolveAllIterations[1] = 2;
	resolveAllIterations[2] = 0;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 1, "Expected 1 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 1, "Expected 1 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 0, "Expected 0 but got %u", collisionCallbackCalls[2]);
	
	// Verify maxIterations is respected (with simultaneous collisions)
	resolveAllIterations[0] = 3;
	resolveAllIterations[1] = 2;
	resolveAllIterations[2] = 1;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 2, "Expected 2 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 1, "Expected 1 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 1, "Expected 1 but got %u", collisionCallbackCalls[2]);
	
	// Verify maxSimultaneousCollisions is respected (maxIterations = 1)
	CollisionResolver_dispose(collisionResolver);
	collisionResolver = CollisionResolver_create(intersectionManager, false, 1, 1);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	resolveAllIterations[0] = 3;
	resolveAllIterations[1] = 2;
	resolveAllIterations[2] = 1;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 1, "Expected 1 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 1, "Expected 1 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 0, "Expected 0 but got %u", collisionCallbackCalls[2]);
	
	// Verify maxSimultaneousCollisions is respected (maxIterations = 2)
	CollisionResolver_dispose(collisionResolver);
	collisionResolver = CollisionResolver_create(intersectionManager, false, 1, 2);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	resolveAllIterations[0] = 3;
	resolveAllIterations[1] = 2;
	resolveAllIterations[2] = 1;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 2, "Expected 2 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 1 || collisionCallbackCalls[1] == 2, "Expected 1 or 2 but got %u", collisionCallbackCalls[1]);
	if (collisionCallbackCalls[1] == 1) {
		TestCase_assert(collisionCallbackCalls[2] == 1, "Expected 1 but got %u", collisionCallbackCalls[2]);
	} else {
		TestCase_assert(collisionCallbackCalls[2] == 0, "Expected 0 but got %u", collisionCallbackCalls[2]);
	}
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObjects[0]);
	CollisionObject_dispose(testObjects[1]);
	CollisionObject_dispose(testObjects[2]);
}

TEST_SUITE(CollisionResolverTest,
           testInit,
           testAddObject,
           testRemoveObject,
           testQuerySingle,
           testFindEarliest,
           testResolveAll)
