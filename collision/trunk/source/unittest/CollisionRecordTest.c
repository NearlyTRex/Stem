#include "collision/CollisionRecord.h"
#include "unittest/TestSuite.h"

static void testInverse() {
	CollisionRecord record, inverse;
	
	record.object1 = (CollisionObject *) (void *) 0x0;
	record.object2 = (CollisionObject *) (void *) 0x1;
	record.normal.x = 0x10000;
	record.normal.y = 0x00000;
	record.normal.z = 0x00000;
	record.time = 0;
	
	inverse = CollisionRecord_inverted(record);
	
	TestCase_assert(inverse.object1 == (void *) 0x1, "Expected 0x1 but got %p", inverse.object1);
	TestCase_assert(inverse.object2 == (void *) 0x0, "Expected 0x0 but got %p", inverse.object2);
	TestCase_assert(inverse.normal.x == (fixed16_16) 0xFFFF0000, "Expected 0xFFFF0000 but got 0x%05X", inverse.normal.x);
	TestCase_assert(inverse.normal.y == 0x00000, "Expected 0x00000 but got 0x%05X", inverse.normal.y);
	TestCase_assert(inverse.normal.z == 0x00000, "Expected 0x00000 but got 0x%05X", inverse.normal.z);
	TestCase_assert(inverse.time == 0, "Expected 0 but got %u", inverse.time);
	
	record.object1 = (CollisionObject *) (void *) 0x3;
	record.object2 = (CollisionObject *) (void *) 0x2;
	record.normal.x = 0x00000;
	record.normal.y = 0x0B504;
	record.normal.z = 0xFFFF4AFC;
	record.time = 2;
	
	inverse = CollisionRecord_inverted(record);
	
	TestCase_assert(inverse.object1 == (void *) 0x2, "Expected 0x2 but got %p", inverse.object1);
	TestCase_assert(inverse.object2 == (void *) 0x3, "Expected 0x3 but got %p", inverse.object2);
	TestCase_assert(inverse.normal.x == 0x00000, "Expected 0x00000 but got 0x%05X", inverse.normal.x);
	TestCase_assert(inverse.normal.y == (fixed16_16) 0xFFFF4AFC, "Expected 0xFFFF4AFC but got 0x%05X", inverse.normal.y);
	TestCase_assert(inverse.normal.z == 0x0B504, "Expected 0x0B504 but got 0x%05X", inverse.normal.z);
	TestCase_assert(inverse.time == 2, "Expected 0x00000 but got %u", inverse.time);
}

static unsigned int collisionCallback1Calls, collisionCallback2Calls;
static CollisionRecord lastCollision1, lastCollision2;
static fixed16_16 lastTimesliceSize1, lastTimesliceSize2;

void resolveCallback1(CollisionRecord collision, fixed16_16 timesliceSize) {
	collisionCallback1Calls++;
	lastCollision1 = collision;
	lastTimesliceSize1 = timesliceSize;
}

void resolveCallback2(CollisionRecord collision, fixed16_16 timesliceSize) {
	collisionCallback2Calls++;
	lastCollision2 = collision;
	lastTimesliceSize2 = timesliceSize;
}

static void testResolve() {
	CollisionRecord collisionRecord;
	CollisionObject * object1, * object2;
	
	collisionCallback1Calls = 0;
	collisionCallback2Calls = 0;
	object1 = CollisionObject_create(NULL, 0, resolveCallback1);
	object2 = CollisionObject_create(NULL, 0, resolveCallback2);
	
	// Verify callbacks are called, and normals/object[12] flipped for object2's callback
	collisionRecord.object1 = object1;
	collisionRecord.object2 = object2;
	collisionRecord.time = 0x10000;
	collisionRecord.normal = VECTOR3x(0x10000, 0x00000, 0x00000);
	CollisionRecord_resolve(collisionRecord, 0x10000);
	
	TestCase_assert(collisionCallback1Calls == 1, "Expected 1 but got %u", collisionCallback1Calls);
	TestCase_assert(collisionCallback2Calls == 1, "Expected 1 but got %u", collisionCallback2Calls);
	TestCase_assert(lastCollision1.object1 == object1, "Expected %p but got %p", object1, lastCollision1.object1);
	TestCase_assert(lastCollision1.object2 == object2, "Expected %p but got %p", object2, lastCollision1.object2);
	TestCase_assert(lastCollision1.time == 0x10000, "Expected 0x10000 but got 0x%05X", object1, lastCollision1.time);
	TestCase_assert(lastCollision1.normal.x == 0x10000 && lastCollision1.normal.y == 0x00000 && lastCollision1.normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", lastCollision1.normal.x, lastCollision1.normal.y, lastCollision1.normal.z);
	TestCase_assert(lastTimesliceSize1 == 0x10000, "Expected 0x10000 but got 0x%05X", lastTimesliceSize1);
	TestCase_assert(lastCollision2.object1 == object2, "Expected %p but got %p", object2, lastCollision2.object1);
	TestCase_assert(lastCollision2.object2 == object1, "Expected %p but got %p", object1, lastCollision2.object2);
	TestCase_assert(lastCollision2.time == 0x10000, "Expected 0x10000 but got 0x%05X", object1, lastCollision1.time);
	TestCase_assert(lastCollision2.normal.x == (fixed16_16) 0xFFFF0000 && lastCollision2.normal.y == 0x00000 && lastCollision2.normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", lastCollision2.normal.x, lastCollision2.normal.y, lastCollision2.normal.z);
	TestCase_assert(lastTimesliceSize2 == 0x10000, "Expected 0x10000 but got 0x%05X", lastTimesliceSize2);
	
	collisionRecord.object1 = object2;
	collisionRecord.object2 = object1;
	object1->collisionCallback = resolveCallback2;
	object2->collisionCallback = resolveCallback1;
	collisionRecord.time = 0x08000;
	collisionRecord.normal = VECTOR3x(0x00000, 0x0B504, 0xFFFF4AFC);
	CollisionRecord_resolve(collisionRecord, 0x06000);
	
	TestCase_assert(collisionCallback1Calls == 2, "Expected 2 but got %u", collisionCallback1Calls);
	TestCase_assert(collisionCallback2Calls == 2, "Expected 2 but got %u", collisionCallback2Calls);
	TestCase_assert(lastCollision1.object1 == object2, "Expected %p but got %p", object2, lastCollision1.object1);
	TestCase_assert(lastCollision1.object2 == object1, "Expected %p but got %p", object1, lastCollision1.object2);
	TestCase_assert(lastCollision1.time == 0x08000, "Expected 0x08000 but got 0x%05X", object1, lastCollision1.time);
	TestCase_assert(lastCollision1.normal.x == 0x00000 && lastCollision1.normal.y == 0x0B504 && lastCollision1.normal.z == (fixed16_16) 0xFFFF4AFC, "Expected {0x00000, 0x0B504, 0xFFFF4AFC} but got {0x%05X, 0x%05X, 0x%05X}", lastCollision1.normal.x, lastCollision1.normal.y, lastCollision1.normal.z);
	TestCase_assert(lastTimesliceSize1 == 0x06000, "Expected 0x06000 but got 0x%05X", lastTimesliceSize1);
	TestCase_assert(lastCollision2.object1 == object1, "Expected %p but got %p", object1, lastCollision2.object1);
	TestCase_assert(lastCollision2.object2 == object2, "Expected %p but got %p", object2, lastCollision2.object2);
	TestCase_assert(lastCollision2.time == 0x08000, "Expected 0x08000 but got 0x%05X", object1, lastCollision1.time);
	TestCase_assert(lastCollision2.normal.x == 0x00000 && lastCollision2.normal.y == (fixed16_16) 0xFFFF4AFC && lastCollision2.normal.z == 0x0B504, "Expected {0x00000, 0xFFFF4AFC, 0x0B504} but got {0x%05X, 0x%05X, 0x%05X}", lastCollision2.normal.x, lastCollision2.normal.y, lastCollision2.normal.z);
	TestCase_assert(lastTimesliceSize2 == 0x06000, "Expected 0x06000 but got 0x%05X", lastTimesliceSize2);
	
	// Verify NULL callbacks don't crash, and the other callback is called if one is NULL
	object1->collisionCallback = NULL;
	CollisionRecord_resolve(collisionRecord, 0x06000);
	TestCase_assert(collisionCallback1Calls == 3, "Expected 3 but got %u", collisionCallback1Calls);
	TestCase_assert(collisionCallback2Calls == 2, "Expected 2 but got %u", collisionCallback2Calls);
	
	object1->collisionCallback = resolveCallback2;
	object2->collisionCallback = NULL;
	CollisionRecord_resolve(collisionRecord, 0x06000);
	TestCase_assert(collisionCallback1Calls == 3, "Expected 3 but got %u", collisionCallback1Calls);
	TestCase_assert(collisionCallback2Calls == 3, "Expected 3 but got %u", collisionCallback2Calls);
	
	object1->collisionCallback = NULL;
	CollisionRecord_resolve(collisionRecord, 0x06000);
	TestCase_assert(collisionCallback1Calls == 3, "Expected 3 but got %u", collisionCallback1Calls);
	TestCase_assert(collisionCallback2Calls == 3, "Expected 3 but got %u", collisionCallback2Calls);
}

TEST_SUITE(CollisionRecordTest,
           testInverse,
           testResolve)
