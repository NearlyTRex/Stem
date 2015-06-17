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

TEST_SUITE(CollisionRecordTest, testInverse)
