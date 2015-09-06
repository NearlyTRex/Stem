#include "collision/CollisionPairQueue.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

static void verifyInit(CollisionPairQueue * queue, int line) {
	TestCase_assert(queue != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(queue->dispose == CollisionPairQueue_dispose, "Expected %p but got %p (line %d)", CollisionPairQueue_dispose, queue->dispose, line);
	TestCase_assert(queue->frontQueueCount == 0, "Expected 0 but got " SIZE_T_FORMAT " (line %d)", queue->frontQueueCount, line);
	TestCase_assert(queue->backQueueCount == 0, "Expected 0 but got " SIZE_T_FORMAT " (line %d)", queue->backQueueCount, line);
}

static void testInit() {
	CollisionPairQueue queue, * queuePtr;
	
	memset(&queue, 0x00, sizeof(queue));
	CollisionPairQueue_init(&queue);
	verifyInit(&queue, __LINE__);
	CollisionPairQueue_dispose(&queue);
	
	memset(&queue, 0xFF, sizeof(queue));
	CollisionPairQueue_init(&queue);
	verifyInit(&queue, __LINE__);
	CollisionPairQueue_dispose(&queue);
	
	queuePtr = CollisionPairQueue_create();
	verifyInit(queuePtr, __LINE__);
	CollisionPairQueue_dispose(queuePtr);
}

static void testAddInitialPairs() {
	CollisionPairQueue * queue;
	CollisionObject * objects[3];
	CollisionObject * object1, * object2;
	bool success;
	
	queue = CollisionPairQueue_create();
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(!success, "Expected false but got true");
	
	objects[0] = CollisionObject_create(NULL, 0, NULL);
	objects[1] = CollisionObject_create(NULL, 0, NULL);
	objects[2] = CollisionObject_create(NULL, 0, NULL);
	CollisionPairQueue_addInitialPairs(queue, objects, 3);
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(object1 == objects[0], "Expected %p but got %p", objects[0], object1);
	TestCase_assert(object2 == objects[1], "Expected %p but got %p", objects[1], object2);
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(object1 == objects[0], "Expected %p but got %p", objects[0], object1);
	TestCase_assert(object2 == objects[2], "Expected %p but got %p", objects[2], object2);
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(object1 == objects[1], "Expected %p but got %p", objects[1], object1);
	TestCase_assert(object2 == objects[2], "Expected %p but got %p", objects[2], object2);
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(!success, "Expected false but got true");
	
	CollisionObject_dispose(objects[0]);
	CollisionObject_dispose(objects[1]);
	CollisionObject_dispose(objects[2]);
}

static void testAddNextPairsForObject() {
	CollisionPairQueue * queue;
	CollisionObject * objects[3];
	CollisionObject * object1, * object2;
	bool success;
	
	queue = CollisionPairQueue_create();
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(!success, "Expected false but got true");
	
	objects[0] = CollisionObject_create(NULL, 0, NULL);
	objects[1] = CollisionObject_create(NULL, 0, NULL);
	objects[2] = CollisionObject_create(NULL, 0, NULL);
	
	CollisionPairQueue_addNextPairsForObject(queue, objects[0], objects, 3);
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(!success, "Expected false but got true");
	success = CollisionPairQueue_nextIteration(queue);
	TestCase_assert(success, "Expected true but got false");
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(object1 == objects[0], "Expected %p but got %p", objects[0], object1);
	TestCase_assert(object2 == objects[1], "Expected %p but got %p", objects[1], object2);
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(object1 == objects[0], "Expected %p but got %p", objects[0], object1);
	TestCase_assert(object2 == objects[2], "Expected %p but got %p", objects[2], object2);
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(!success, "Expected false but got true");
	
	CollisionPairQueue_addNextPairsForObject(queue, objects[1], objects, 3);
	CollisionPairQueue_addNextPairsForObject(queue, objects[1], objects, 3);
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(!success, "Expected false but got true");
	success = CollisionPairQueue_nextIteration(queue);
	TestCase_assert(success, "Expected true but got false");
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(object1 == objects[1], "Expected %p but got %p", objects[1], object1);
	TestCase_assert(object2 == objects[0], "Expected %p but got %p", objects[0], object2);
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(object1 == objects[1], "Expected %p but got %p", objects[1], object1);
	TestCase_assert(object2 == objects[2], "Expected %p but got %p", objects[2], object2);
	
	success = CollisionPairQueue_nextPair(queue, &object1, &object2);
	TestCase_assert(!success, "Expected false but got true");
	
	success = CollisionPairQueue_nextIteration(queue);
	TestCase_assert(!success, "Expected false but got true");
	
	CollisionObject_dispose(objects[0]);
	CollisionObject_dispose(objects[1]);
	CollisionObject_dispose(objects[2]);
}

TEST_SUITE(CollisionPairQueueTest,
           testInit,
           testAddInitialPairs,
           testAddNextPairsForObject)
