#include "collision/CollisionResolver.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"
#include <string.h>

static void verifyInit(int line, CollisionResolver * collisionResolver, IntersectionManager * intersectionManager) {
	TestCase_assert(collisionResolver != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(collisionResolver->dispose == CollisionResolver_dispose, "Expected %p but got %p (line %d)", CollisionResolver_dispose, collisionResolver->dispose, line);
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

static CollisionObject * testObjects[4];

static unsigned int getTestObjectIndex(CollisionObject * object) {
	unsigned int objectIndex;
	
	for (objectIndex = 0; objectIndex < sizeof(testObjects) / sizeof(testObjects[0]); objectIndex++) {
		if (object == testObjects[objectIndex]) {
			break;
		}
	}
	return objectIndex;
}

static bool querySingleIntersectionHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	if (object2 == testObjects[0]) {
		*outTime = 0x00000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		*outContactArea = 0x00000;
		return true;
	}
	if (object2 == testObjects[1]) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		*outContactArea = 0x00000;
		return true;
	}
	if (object2 == testObjects[2]) {
		*outTime = 0x0C000;
		*outNormal = VECTOR3x(0x00000, 0x10000, 0x00000);
		*outContactArea = 0x00000;
		return true;
	}
	if (object2 == testObjects[3]) {
		*outTime = 0x04000;
		*outNormal = VECTOR3x(0x00000, 0x00000, 0x10000);
		*outContactArea = 0x00000;
		return true;
	}
	return false;
}

static bool nullIntersectionHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	return false;
}

static void testQuerySingle() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	bool result;
	CollisionRecord collisionRecord;
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 1, querySingleIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 1, 1, nullIntersectionHandler);
	
	testObjects[0] = CollisionObject_create(NULL, 0, NULL);
	testObjects[1] = CollisionObject_create(NULL, 1, NULL);
	testObjects[2] = CollisionObject_create(NULL, 1, NULL);
	testObjects[3] = CollisionObject_create(NULL, 1, NULL);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	
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
	TestCase_assert(collisionRecord.time == 0x0C000, "Expected 0x0C000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x00000 && collisionRecord.normal.y == 0x10000 && collisionRecord.normal.z == 0x00000, "Expected {0x00000, 0x10000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify intersection test shape type order is swapped correctly and normals reversed
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	memset(&collisionRecord, 0xFF, sizeof(collisionRecord));
	result = CollisionResolver_querySingle(collisionResolver, testObjects[2], &collisionRecord);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(collisionRecord.object1 == testObjects[2], "Expected %p but got %p", testObjects[2], collisionRecord.object1);
	TestCase_assert(collisionRecord.object2 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecord.object2);
	TestCase_assert(collisionRecord.time == 0x0C000, "Expected 0x0C000 but got 0x%05X", collisionRecord.time);
	TestCase_assert(collisionRecord.normal.x == 0x00000 && collisionRecord.normal.y == (fixed16_16) 0xFFFF0000 && collisionRecord.normal.z == 0x00000, "Expected {0x00000, 0xFFFF0000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecord.normal.x, collisionRecord.normal.y, collisionRecord.normal.z);
	
	// Verify no collision occurs using nullIntersectionHandler by changing object1's shape type
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

static bool findEarliestIntersectionHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	if (object2 == testObjects[1] || object2 == testObjects[2]) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		*outContactArea = 0x00000;
		return true;
	}
	if (object2 == testObjects[3]) {
		*outTime = 0x08001;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		*outContactArea = 0x00000;
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
	IntersectionManager_setHandler(intersectionManager, 0, 1, querySingleIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 1, 1, nullIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 2, 1, findEarliestIntersectionHandler);
	
	testObjects[0] = CollisionObject_create(NULL, 0, NULL);
	testObjects[1] = CollisionObject_create(NULL, 1, NULL);
	testObjects[2] = CollisionObject_create(NULL, 1, NULL);
	testObjects[3] = CollisionObject_create(NULL, 1, NULL);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	
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
	TestCase_assert(collisionRecords[0].time == 0x0C000, "Expected 0x0C000 but got 0x%05X", collisionRecords[0].time);
	TestCase_assert(collisionRecords[0].normal.x == 0x00000 && collisionRecords[0].normal.y == 0x10000 && collisionRecords[0].normal.z == 0x00000, "Expected {0x00000, 0x10000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", collisionRecords[0].normal.x, collisionRecords[0].normal.y, collisionRecords[0].normal.z);
	
	// Verify no collision occurs using nullIntersectionHandler by changing object1's shape type
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

static fixed16_16 contactAreas[2];

static bool contactAreaIntersectionHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	if (object2 == testObjects[1]) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		*outContactArea = contactAreas[0];
		return true;
	}
	if (object2 == testObjects[2]) {
		*outTime = 0x08000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		*outContactArea = contactAreas[1];
		return true;
	}
	return false;
}

static void testFindEarliestSortsByContactArea() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	size_t resultCount;
	CollisionRecord collisionRecords[2];
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 1, contactAreaIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 0, 2, contactAreaIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 1, 2, nullIntersectionHandler);
	
	testObjects[0] = CollisionObject_create(NULL, 0, NULL);
	testObjects[1] = CollisionObject_create(NULL, 1, NULL);
	testObjects[2] = CollisionObject_create(NULL, 2, NULL);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	
	contactAreas[0] = 1;
	contactAreas[1] = 0;
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 2);
	TestCase_assert(resultCount == 2, "Expected 2 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObjects[1], "Expected %p but got %p", testObjects[1], collisionRecords[0].object2);
	TestCase_assert(collisionRecords[1].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[1].object1);
	TestCase_assert(collisionRecords[1].object2 == testObjects[2], "Expected %p but got %p", testObjects[2], collisionRecords[1].object2);
	
	contactAreas[0] = 0;
	contactAreas[1] = 1;
	resultCount = CollisionResolver_findEarliest(collisionResolver, collisionRecords, 2);
	TestCase_assert(resultCount == 2, "Expected 2 but got " SIZE_T_FORMAT, resultCount);
	TestCase_assert(collisionRecords[0].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[0].object1);
	TestCase_assert(collisionRecords[0].object2 == testObjects[2], "Expected %p but got %p", testObjects[2], collisionRecords[0].object2);
	TestCase_assert(collisionRecords[1].object1 == testObjects[0], "Expected %p but got %p", testObjects[0], collisionRecords[1].object1);
	TestCase_assert(collisionRecords[1].object2 == testObjects[1], "Expected %p but got %p", testObjects[1], collisionRecords[1].object2);
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObjects[0]);
	CollisionObject_dispose(testObjects[1]);
	CollisionObject_dispose(testObjects[2]);
}

static unsigned int collisionCallbackCalls[4];
static unsigned int interpolateMethodCalls[4];
static unsigned int resolveAllIterations[4];
static int resolveAllLine;
static fixed16_16 intersectionTime;
static fixed16_16 lastTimesliceSizes[4];
static fixed16_16 lastSubframeTimes[4];
static fixed16_16 lastInterpolationAmounts[4];
static bool interpolateCalled, resolveCalledAfterInterpolate;

static bool resolveAllIntersectionHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	unsigned int object1Index, object2Index;
	
	object1Index = getTestObjectIndex(object1);
	TestCase_assert(object1Index < 4, "Intersection handler called with unknown object1 %p (line %d)", object1, resolveAllLine);
	object2Index = getTestObjectIndex(object2);
	TestCase_assert(object2Index < 4, "Intersection handler called with unknown object2 %p (line %d)", object2, resolveAllLine);
	
	if (resolveAllIterations[object1Index] > 0 && resolveAllIterations[object2Index] > 0) {
		*outTime = intersectionTime;
		*outContactArea = 0x00000;
		return true;
	}
	
	return false;
}

static void resolveAllCollisionCallback(CollisionRecord collision, fixed16_16 timesliceSize, fixed16_16 subframeTime) {
	unsigned int object1Index;
	
	object1Index = getTestObjectIndex(collision.object1);
	TestCase_assert(object1Index < 4, "Resolve callback called with unknown object1 %p (line %d)", collision.object1, resolveAllLine);
	
	resolveAllIterations[object1Index]--;
	collisionCallbackCalls[object1Index]++;
	lastTimesliceSizes[object1Index] = timesliceSize;
	lastSubframeTimes[object1Index] = subframeTime;
	if (interpolateCalled) {
		resolveCalledAfterInterpolate = true;
		interpolateCalled = false;
	}
}

static void resolveAllTest_CollisionObject_interpolate(CollisionObject * self, fixed16_16 amount) {
	unsigned int object1Index;
	
	object1Index = getTestObjectIndex(self);
	TestCase_assert(object1Index < 4, "Interpolate method called with unknown self %p (line %d)", self, resolveAllLine);
	
	interpolateMethodCalls[object1Index]++;
	lastInterpolationAmounts[object1Index] = amount;
	interpolateCalled = true;
	resolveCalledAfterInterpolate = false;
}

static void testResolveAll() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 1, resolveAllIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 0, 2, resolveAllIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 1, 2, nullIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 0, 3, nullIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 1, 3, nullIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 2, 3, resolveAllIntersectionHandler);
	
	testObjects[0] = CollisionObject_create(NULL, 0, resolveAllCollisionCallback);
	testObjects[1] = CollisionObject_create(NULL, 1, resolveAllCollisionCallback);
	testObjects[2] = CollisionObject_create(NULL, 2, resolveAllCollisionCallback);
	testObjects[3] = CollisionObject_create(NULL, 3, resolveAllCollisionCallback);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	CollisionResolver_addObject(collisionResolver, testObjects[3]);
	
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
	TestCase_assert(collisionCallbackCalls[0] == 3, "Expected 3 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 2, "Expected 2 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 1, "Expected 1 but got %u", collisionCallbackCalls[2]);
	
	// Verify timesliceSize and subframeTime are passed correctly (one iteration, any intersection time)
	resolveAllIterations[0] = 1;
	resolveAllIterations[1] = 1;
	intersectionTime = 0x00000;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 1, "Expected 1 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 1, "Expected 1 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(lastTimesliceSizes[0] == 0x10000, "Expected 0x10000 but got 0x%05X", lastTimesliceSizes[0]);
	TestCase_assert(lastTimesliceSizes[1] == 0x10000, "Expected 0x10000 but got 0x%05X", lastTimesliceSizes[1]);
	TestCase_assert(lastSubframeTimes[0] == 0x00000, "Expected 0x00000 but got 0x%05X", lastSubframeTimes[0]);
	TestCase_assert(lastSubframeTimes[1] == 0x00000, "Expected 0x00000 but got 0x%05X", lastSubframeTimes[1]);
	
	// Verify timesliceSize and subframeTime are passed correctly (two iterations, intersection time 0.25)
	resolveAllIterations[0] = 2;
	resolveAllIterations[1] = 2;
	intersectionTime = 0x04000;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 2, "Expected 2 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 2, "Expected 2 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(lastTimesliceSizes[0] == 0x0C000, "Expected 0x0C000 but got 0x%05X", lastTimesliceSizes[0]);
	TestCase_assert(lastTimesliceSizes[1] == 0x0C000, "Expected 0x0C000 but got 0x%05X", lastTimesliceSizes[1]);
	TestCase_assert(lastSubframeTimes[0] == 0x07000, "Expected 0x07000 but got 0x%05X", lastSubframeTimes[0]);
	TestCase_assert(lastSubframeTimes[1] == 0x07000, "Expected 0x07000 but got 0x%05X", lastSubframeTimes[1]);
	
	// Verify timesliceSize and subframeTime are passed correctly (three iterations, intersection time 0.5)
	resolveAllIterations[0] = 3;
	resolveAllIterations[1] = 3;
	intersectionTime = 0x08000;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 3, "Expected 3 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 3, "Expected 3 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(lastTimesliceSizes[0] == 0x04000, "Expected 0x04000 but got 0x%05X", lastTimesliceSizes[0]);
	TestCase_assert(lastTimesliceSizes[1] == 0x04000, "Expected 0x04000 but got 0x%05X", lastTimesliceSizes[1]);
	TestCase_assert(lastSubframeTimes[0] == 0x0E000, "Expected 0x0E000 but got 0x%05X", lastSubframeTimes[0]);
	TestCase_assert(lastSubframeTimes[1] == 0x0E000, "Expected 0x0E000 but got 0x%05X", lastSubframeTimes[1]);
	
	// Verify interpolate is called on CollisionObjects (one iteration)
	testObjects[0]->interpolate = resolveAllTest_CollisionObject_interpolate;
	testObjects[1]->interpolate = resolveAllTest_CollisionObject_interpolate;
	testObjects[2]->interpolate = resolveAllTest_CollisionObject_interpolate;
	testObjects[3]->interpolate = resolveAllTest_CollisionObject_interpolate;
	resolveAllIterations[0] = 1;
	resolveAllIterations[1] = 1;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	memset(interpolateMethodCalls, 0, sizeof(interpolateMethodCalls));
	intersectionTime = 0x04000;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 1, "Expected 1 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 1, "Expected 1 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 0, "Expected 0 but got %u", collisionCallbackCalls[2]);
	TestCase_assert(interpolateMethodCalls[0] == 1, "Expected 1 but got %u", interpolateMethodCalls[0]);
	TestCase_assert(interpolateMethodCalls[1] == 1, "Expected 1 but got %u", interpolateMethodCalls[1]);
	TestCase_assert(interpolateMethodCalls[2] == 1, "Expected 1 but got %u", interpolateMethodCalls[2]);
	TestCase_assert(lastInterpolationAmounts[0] == 0x04000, "Expected 0x04000 but got 0x%05X", lastInterpolationAmounts[0]);
	TestCase_assert(lastInterpolationAmounts[1] == 0x04000, "Expected 0x04000 but got 0x%05X", lastInterpolationAmounts[1]);
	
	// Verify interpolate is called on CollisionObjects (two iterations)
	resolveAllIterations[0] = 2;
	resolveAllIterations[1] = 2;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	memset(interpolateMethodCalls, 0, sizeof(interpolateMethodCalls));
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 2, "Expected 2 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 2, "Expected 2 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 0, "Expected 0 but got %u", collisionCallbackCalls[2]);
	TestCase_assert(interpolateMethodCalls[0] == 2, "Expected 2 but got %u", interpolateMethodCalls[0]);
	TestCase_assert(interpolateMethodCalls[1] == 2, "Expected 2 but got %u", interpolateMethodCalls[1]);
	TestCase_assert(interpolateMethodCalls[2] == 2, "Expected 2 but got %u", interpolateMethodCalls[2]);
	TestCase_assert(lastInterpolationAmounts[0] == 0x04000, "Expected 0x04000 but got 0x%05X", lastInterpolationAmounts[0]);
	TestCase_assert(lastInterpolationAmounts[1] == 0x04000, "Expected 0x04000 but got 0x%05X", lastInterpolationAmounts[1]);
	
	// Verify interpolate is called per iteration, not per collision
	IntersectionManager_setHandler(intersectionManager, 0, 2, nullIntersectionHandler);
	resolveAllIterations[0] = 2;
	resolveAllIterations[1] = 2;
	resolveAllIterations[2] = 2;
	resolveAllIterations[3] = 2;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	memset(interpolateMethodCalls, 0, sizeof(interpolateMethodCalls));
	intersectionTime = 0x08000;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionCallbackCalls[0] == 2, "Expected 2 but got %u", collisionCallbackCalls[0]);
	TestCase_assert(collisionCallbackCalls[1] == 2, "Expected 2 but got %u", collisionCallbackCalls[1]);
	TestCase_assert(collisionCallbackCalls[2] == 2, "Expected 2 but got %u", collisionCallbackCalls[2]);
	TestCase_assert(collisionCallbackCalls[3] == 2, "Expected 2 but got %u", collisionCallbackCalls[3]);
	TestCase_assert(interpolateMethodCalls[0] == 2, "Expected 2 but got %u", interpolateMethodCalls[0]);
	TestCase_assert(interpolateMethodCalls[1] == 2, "Expected 2 but got %u", interpolateMethodCalls[1]);
	TestCase_assert(interpolateMethodCalls[2] == 2, "Expected 2 but got %u", interpolateMethodCalls[2]);
	TestCase_assert(interpolateMethodCalls[3] == 2, "Expected 2 but got %u", interpolateMethodCalls[3]);
	TestCase_assert(lastInterpolationAmounts[0] == 0x08000, "Expected 0x08000 but got 0x%05X", lastInterpolationAmounts[0]);
	TestCase_assert(lastInterpolationAmounts[1] == 0x08000, "Expected 0x08000 but got 0x%05X", lastInterpolationAmounts[1]);
	IntersectionManager_setHandler(intersectionManager, 0, 2, resolveAllIntersectionHandler);
	
	// Verify interpolate is called before resolution, not after
	resolveAllIterations[0] = 1;
	resolveAllIterations[1] = 1;
	memset(collisionCallbackCalls, 0, sizeof(collisionCallbackCalls));
	memset(interpolateMethodCalls, 0, sizeof(interpolateMethodCalls));
	interpolateCalled = false;
	resolveCalledAfterInterpolate = false;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(resolveCalledAfterInterpolate, "Expected true but got false");
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObjects[0]);
	CollisionObject_dispose(testObjects[1]);
	CollisionObject_dispose(testObjects[2]);
	CollisionObject_dispose(testObjects[3]);
}

static unsigned int mutateListCollisionCallbackCalls[2];

static bool mutateListIntersectionHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	if (object2->shapeType == 1) {
		*outTime = 0x08000;
		*outContactArea = 0x00000;
		return true;
		
	} else if (object2->shapeType == 2 && mutateListCollisionCallbackCalls[1] < 2) {
		*outTime = 0x00000;
		*outContactArea = 0x00000;
		return true;
	}
	
	return false;
}

static void mutateListCollisionCallback(CollisionRecord collision, fixed16_16 timesliceSize, fixed16_16 subframeTime) {
	if (collision.object2->shapeType == 1) {
		CollisionResolver_addObject(collision.object1->owner, testObjects[2]);
		mutateListCollisionCallbackCalls[0]++;
		
	} else if (collision.object2->shapeType == 2) {
		if (mutateListCollisionCallbackCalls[1] == 0) {
			CollisionResolver_removeObject(collision.object1->owner, testObjects[1]);
		} else {
			TestCase_assert(((CollisionResolver *) collision.object1->owner)->objectCount == 3, "Expected 3 but got " SIZE_T_FORMAT, ((CollisionResolver *) collision.object1->owner)->objectCount);
		}
		mutateListCollisionCallbackCalls[1]++;
	}
}

static void testListMutationDuringResolution() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 1, mutateListIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 0, 2, mutateListIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 1, 2, nullIntersectionHandler);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	testObjects[0] = CollisionObject_create(collisionResolver, 0, mutateListCollisionCallback);
	testObjects[1] = CollisionObject_create(collisionResolver, 1, NULL);
	testObjects[2] = CollisionObject_create(collisionResolver, 2, NULL);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	
	// Collision 0<->1 adds 2
	// Collision 0<->2 removes 1
	// Test 0<->2 returns false
	
	mutateListCollisionCallbackCalls[0] = 0;
	mutateListCollisionCallbackCalls[1] = 0;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collisionResolver->objectCount == 2, "Expected 2 but got " SIZE_T_FORMAT, collisionResolver->objectCount);
	TestCase_assert(collisionResolver->objects[0] == testObjects[0], "Expected %p but got %p", testObjects[0], collisionResolver->objects[0]);
	TestCase_assert(collisionResolver->objects[1] == testObjects[2], "Expected %p but got %p", testObjects[2], collisionResolver->objects[1]);
	TestCase_assert(mutateListCollisionCallbackCalls[0] == 1, "Expected 1 but got %u", mutateListCollisionCallbackCalls[0]);
	TestCase_assert(mutateListCollisionCallbackCalls[1] == 2, "Expected 2 but got %u", mutateListCollisionCallbackCalls[1]);
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObjects[0]);
	CollisionObject_dispose(testObjects[1]);
	CollisionObject_dispose(testObjects[2]);
}

static unsigned int simultaneousRetestCollisionCallbackCalls[2];

static bool simultaneousRetestIntersectionHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	if (simultaneousRetestCollisionCallbackCalls[0] == 0) {
		*outTime = 0x08000;
		*outContactArea = 0x00000;
		return true;
	}
	
	return false;
}

static void simultaneousRetestCollisionCallback(CollisionRecord collision, fixed16_16 timesliceSize, fixed16_16 subframeTime) {
	if (collision.object2->shapeType == 1) {
		simultaneousRetestCollisionCallbackCalls[0]++;
		
	} else if (collision.object2->shapeType == 2) {
		simultaneousRetestCollisionCallbackCalls[1]++;
	}
}

static void testSimultaneousCollisionRetestsObjectsAlreadyResolvedInSameIteration() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 1, simultaneousRetestIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 0, 2, simultaneousRetestIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 1, 2, nullIntersectionHandler);
	
	testObjects[0] = CollisionObject_create(NULL, 0, simultaneousRetestCollisionCallback);
	testObjects[1] = CollisionObject_create(NULL, 1, NULL);
	testObjects[2] = CollisionObject_create(NULL, 2, NULL);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	
	simultaneousRetestCollisionCallbackCalls[0] = 0;
	simultaneousRetestCollisionCallbackCalls[1] = 0;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(simultaneousRetestCollisionCallbackCalls[0] == 1, "Expected 1 but got %u", simultaneousRetestCollisionCallbackCalls[0]); // TODO: This failed once and passed next run with no code changes. Why?
	TestCase_assert(simultaneousRetestCollisionCallbackCalls[1] == 0, "Expected 0 but got %u", simultaneousRetestCollisionCallbackCalls[1]);
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObjects[0]);
	CollisionObject_dispose(testObjects[1]);
	CollisionObject_dispose(testObjects[2]);
}

static unsigned int unresolvableCollisionCallbackCalls[4];
static unsigned int resolutionFailureCallbackCalls[2];

static bool unresolvableIntersectionHandler1(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	if (object2 == testObjects[1]) {
		*outTime = 0x00000;
		*outContactArea = 0x00000;
		return true;
	}
	if (object2 == testObjects[2] && unresolvableCollisionCallbackCalls[2] == 0) {
		*outTime = 0x08000;
		*outContactArea = 0x00000;
		return true;
	}
	return false;
}

static bool unresolvableIntersectionHandler2(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	if (unresolvableCollisionCallbackCalls[3] == 0 && 
	    ((object2 == testObjects[1] && !(unresolvableCollisionCallbackCalls[0] % 2)) ||
	     (object2 == testObjects[2] && unresolvableCollisionCallbackCalls[0] % 2))) {
		*outTime = 0x00000;
		*outContactArea = 0x00000;
		return true;
	}
	if (unresolvableCollisionCallbackCalls[3] == 0 && object2 == testObjects[3]) {
		*outTime = 0x08000;
		*outContactArea = 0x00000;
		return true;
	}
	return false;
}

static void unresolvableCollisionCallback(CollisionRecord collision, fixed16_16 timesliceSize, fixed16_16 subframeTime) {
	unsigned int object1Index, object2Index;
	
	object1Index = getTestObjectIndex(collision.object1);
	TestCase_assert(object1Index < 4, "Collision handler called with unknown object1 %p (line %d)", collision.object1, resolveAllLine);
	object2Index = getTestObjectIndex(collision.object2);
	TestCase_assert(object2Index < 4, "Collision handler called with unknown object2 %p (line %d)", collision.object2, resolveAllLine);
	
	unresolvableCollisionCallbackCalls[object1Index]++;
	unresolvableCollisionCallbackCalls[object2Index]++;
}

static void resolutionFailureCallback1(CollisionObject * self, CollisionObject * collidingObject) {
	TestCase_assert(self == testObjects[0], "Expected %p but got %p", testObjects[0], self);
	TestCase_assert(collidingObject == testObjects[1], "Expected %p but got %p", testObjects[1], collidingObject);
	resolutionFailureCallbackCalls[0]++;
}

static void resolutionFailureCallback2(CollisionObject * self, CollisionObject * collidingObject) {
	TestCase_assert(self == testObjects[1], "Expected %p but got %p", testObjects[1], self);
	TestCase_assert(collidingObject == testObjects[0], "Expected %p but got %p", testObjects[0], collidingObject);
	resolutionFailureCallbackCalls[1]++;
}

static void testUnresolvableDetection() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 1, unresolvableIntersectionHandler1);
	IntersectionManager_setHandler(intersectionManager, 0, 2, unresolvableIntersectionHandler1);
	IntersectionManager_setHandler(intersectionManager, 1, 2, nullIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 1, 3, nullIntersectionHandler);
	IntersectionManager_setHandler(intersectionManager, 2, 3, nullIntersectionHandler);
	
	testObjects[0] = CollisionObject_create(NULL, 0, NULL);
	testObjects[1] = CollisionObject_create(NULL, 1, unresolvableCollisionCallback);
	testObjects[2] = CollisionObject_create(NULL, 2, unresolvableCollisionCallback);
	testObjects[3] = CollisionObject_create(NULL, 3, unresolvableCollisionCallback);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	CollisionResolver_addObject(collisionResolver, testObjects[2]);
	
	// Objects 0 and 1 collide at time 0 and don't resolve themselves. Objects 0 and 2 want to collide at time 0.5, but won't get a chance if 0 and 1 use all the iterations.
	memset(unresolvableCollisionCallbackCalls, 0, sizeof(unresolvableCollisionCallbackCalls));
	memset(resolutionFailureCallbackCalls, 0, sizeof(resolutionFailureCallbackCalls));
	testObjects[0]->resolutionFailureCallback = resolutionFailureCallback1;
	testObjects[1]->resolutionFailureCallback = resolutionFailureCallback2;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(unresolvableCollisionCallbackCalls[2] == 1, "Expected 1 but got %u", unresolvableCollisionCallbackCalls[2]);
	TestCase_assert(resolutionFailureCallbackCalls[0] == 2, "Expected 2 but got %u\n", resolutionFailureCallbackCalls[0]);
	TestCase_assert(resolutionFailureCallbackCalls[1] == 2, "Expected 2 but got %u\n", resolutionFailureCallbackCalls[1]);
	
	// Pairs 0<->1 and 0<->2 alternately intersect each iteration at time 0. Pair 0<->3 intersects at time 0.5.
	IntersectionManager_setHandler(intersectionManager, 0, 1, unresolvableIntersectionHandler2);
	IntersectionManager_setHandler(intersectionManager, 0, 2, unresolvableIntersectionHandler2);
	IntersectionManager_setHandler(intersectionManager, 0, 3, unresolvableIntersectionHandler2);
	CollisionResolver_addObject(collisionResolver, testObjects[3]);
	memset(unresolvableCollisionCallbackCalls, 0, sizeof(unresolvableCollisionCallbackCalls));
	testObjects[0]->resolutionFailureCallback = testObjects[1]->resolutionFailureCallback = NULL;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(unresolvableCollisionCallbackCalls[3] == 1, "Expected 1 but got %u", unresolvableCollisionCallbackCalls[3]);
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObjects[0]);
	CollisionObject_dispose(testObjects[1]);
	CollisionObject_dispose(testObjects[2]);
	CollisionObject_dispose(testObjects[3]);
}

static unsigned int collidableMasksCollisionCallbackCalls[2];

static bool collidableMasksIntersectionHandler(CollisionObject * object1, CollisionObject * object2, fixed16_16 * outTime, Vector3x * outNormal, Vector3x * outObject1Vector, Vector3x * outObject2Vector, fixed16_16 * outContactArea) {
	if (collidableMasksCollisionCallbackCalls[0] == 0) {
		*outTime = 0x00000;
		*outNormal = VECTOR3x(0x10000, 0x00000, 0x00000);
		*outContactArea = 0x00000;
		return true;
	}
	return false;
}

static void collidableMasksCollisionCallback(CollisionRecord collision, fixed16_16 timesliceSize, fixed16_16 subframeTime) {
	unsigned int object1Index, object2Index;
	
	object1Index = getTestObjectIndex(collision.object1);
	TestCase_assert(object1Index < 2, "Collision handler called with unknown object1 %p (line %d)", collision.object1, resolveAllLine);
	object2Index = getTestObjectIndex(collision.object2);
	TestCase_assert(object2Index < 2, "Collision handler called with unknown object2 %p (line %d)", collision.object2, resolveAllLine);
	
	collidableMasksCollisionCallbackCalls[object1Index]++;
	collidableMasksCollisionCallbackCalls[object2Index]++;
}

static void testCollidableMasks() {
	CollisionResolver * collisionResolver;
	IntersectionManager * intersectionManager;
	
	intersectionManager = IntersectionManager_create();
	IntersectionManager_setHandler(intersectionManager, 0, 0, collidableMasksIntersectionHandler);
	
	collisionResolver = CollisionResolver_create(intersectionManager, false);
	testObjects[0] = CollisionObject_create(collisionResolver, 0, collidableMasksCollisionCallback);
	testObjects[1] = CollisionObject_create(collisionResolver, 0, collidableMasksCollisionCallback);
	CollisionResolver_addObject(collisionResolver, testObjects[0]);
	CollisionResolver_addObject(collisionResolver, testObjects[1]);
	
	CollisionObject_setMasks(testObjects[0], 0x1, 0x3);
	CollisionObject_setMasks(testObjects[1], 0x2, 0x3);
	collidableMasksCollisionCallbackCalls[0] = collidableMasksCollisionCallbackCalls[1] = 0;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collidableMasksCollisionCallbackCalls[0] == 2, "Expected 2 but got %u", collidableMasksCollisionCallbackCalls[0]);
	TestCase_assert(collidableMasksCollisionCallbackCalls[1] == 2, "Expected 2 but got %u", collidableMasksCollisionCallbackCalls[1]);
	
	CollisionObject_setMasks(testObjects[0], 0x1, 0x1);
	CollisionObject_setMasks(testObjects[1], 0x2, 0x3);
	collidableMasksCollisionCallbackCalls[0] = collidableMasksCollisionCallbackCalls[1] = 0;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collidableMasksCollisionCallbackCalls[0] == 0, "Expected 0 but got %u", collidableMasksCollisionCallbackCalls[0]);
	TestCase_assert(collidableMasksCollisionCallbackCalls[1] == 0, "Expected 0 but got %u", collidableMasksCollisionCallbackCalls[1]);
	
	CollisionObject_setMasks(testObjects[0], 0x1, 0x3);
	CollisionObject_setMasks(testObjects[1], 0x2, 0x2);
	collidableMasksCollisionCallbackCalls[0] = collidableMasksCollisionCallbackCalls[1] = 0;
	resolveAllLine = __LINE__; CollisionResolver_resolveAll(collisionResolver);
	TestCase_assert(collidableMasksCollisionCallbackCalls[0] == 0, "Expected 0 but got %u", collidableMasksCollisionCallbackCalls[0]);
	TestCase_assert(collidableMasksCollisionCallbackCalls[1] == 0, "Expected 0 but got %u", collidableMasksCollisionCallbackCalls[1]);
	
	CollisionResolver_dispose(collisionResolver);
	IntersectionManager_dispose(intersectionManager);
	CollisionObject_dispose(testObjects[0]);
	CollisionObject_dispose(testObjects[1]);
}

TEST_SUITE(CollisionResolverTest,
           testInit,
           testAddObject,
           testRemoveObject,
           testQuerySingle,
           testFindEarliest,
           testFindEarliestSortsByContactArea,
           testResolveAll,
           testListMutationDuringResolution,
           testSimultaneousCollisionRetestsObjectsAlreadyResolvedInSameIteration,
           testUnresolvableDetection,
           testCollidableMasks)
