#include "collision/CollisionRect2D.h"
#include "collision/CollisionShared.h"
#include "unittest/TestSuite.h"

static void verifyInit(int line, CollisionRect2D * rect, void * owner, CollisionCallback collisionCallback) {
	TestCase_assert(rect != NULL, "Expected non-NULL but got NULL", rect);
	TestCase_assert(rect->dispose == CollisionRect2D_dispose, "Expected %p but got %p", CollisionRect2D_dispose, rect->dispose);
	TestCase_assert(rect->shapeType == COLLISION_SHAPE_RECT_2D, "Expected %d but got %d (line %d)", COLLISION_SHAPE_RECT_2D, rect->shapeType, line);
	TestCase_assert(rect->owner == owner, "Expected %p but got %p (line %d)", owner, rect->owner, line);
	TestCase_assert(rect->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, rect->collisionCallback, line);
	TestCase_assert(rect->interpolate == CollisionRect2D_interpolate, "Expected %p but got %p", CollisionRect2D_interpolate, rect->interpolate);
	TestCase_assert(rect->position.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->position.x);
	TestCase_assert(rect->position.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->position.y);
	TestCase_assert(rect->lastPosition.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastPosition.x);
	TestCase_assert(rect->lastPosition.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastPosition.y);
	TestCase_assert(rect->size.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->size.x);
	TestCase_assert(rect->size.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->size.y);
	TestCase_assert(rect->lastSize.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastSize.x);
	TestCase_assert(rect->lastSize.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastSize.y);
}

static void testInit() {
	CollisionRect2D rect, * rectPtr;
	
	memset(&rect, 0x00, sizeof(rect));
	CollisionRect2D_init(&rect, NULL, NULL);
	verifyInit(__LINE__, &rect, NULL, NULL);
	CollisionRect2D_dispose(&rect);
	
	memset(&rect, 0xFF, sizeof(rect));
	CollisionRect2D_init(&rect, NULL, NULL);
	verifyInit(__LINE__, &rect, NULL, NULL);
	CollisionRect2D_dispose(&rect);
	
	memset(&rect, 0x00, sizeof(rect));
	CollisionRect2D_init(&rect, (void *) 0x1234, (CollisionCallback) 0x2345);
	verifyInit(__LINE__, &rect, (void *) 0x1234, (CollisionCallback) 0x2345);
	CollisionRect2D_dispose(&rect);
	
	rectPtr = CollisionRect2D_create(NULL, NULL);
	verifyInit(__LINE__, rectPtr, NULL, NULL);
	CollisionRect2D_dispose(rectPtr);
	
	rectPtr = CollisionRect2D_create((void *) 0x1234, (CollisionCallback) 0x2345);
	verifyInit(__LINE__, rectPtr, (void *) 0x1234, (CollisionCallback) 0x2345);
	CollisionRect2D_dispose(rectPtr);
}

static void testUpdatePosition() {
	CollisionRect2D * rect;
	
	rect = CollisionRect2D_create(NULL, NULL);
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x10000, 0x20000));
	TestCase_assert(rect->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->position.x);
	TestCase_assert(rect->position.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->position.y);
	TestCase_assert(rect->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->lastPosition.x);
	TestCase_assert(rect->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->lastPosition.y);
	
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x00000, 0x00000));
	TestCase_assert(rect->position.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->position.x);
	TestCase_assert(rect->position.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->position.y);
	TestCase_assert(rect->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->lastPosition.x);
	TestCase_assert(rect->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->lastPosition.y);
	
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x08000, 0x0B000));
	TestCase_assert(rect->position.x == 0x08000, "Expected 0x08000 but got 0x%05X", rect->position.x);
	TestCase_assert(rect->position.y == 0x0B000, "Expected 0x0B000 but got 0x%05X", rect->position.y);
	TestCase_assert(rect->lastPosition.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastPosition.x);
	TestCase_assert(rect->lastPosition.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastPosition.y);
}

static void testUpdateSize() {
	CollisionRect2D * rect;
	
	rect = CollisionRect2D_create(NULL, NULL);
	CollisionRect2D_updateSize(rect, VECTOR2x(0x10000, 0x20000));
	TestCase_assert(rect->size.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->size.x);
	TestCase_assert(rect->size.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->size.y);
	TestCase_assert(rect->lastSize.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->lastSize.x);
	TestCase_assert(rect->lastSize.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->lastSize.y);
	
	CollisionRect2D_updateSize(rect, VECTOR2x(0x00000, 0x00000));
	TestCase_assert(rect->size.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->size.x);
	TestCase_assert(rect->size.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->size.y);
	TestCase_assert(rect->lastSize.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->lastSize.x);
	TestCase_assert(rect->lastSize.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->lastSize.y);
	
	CollisionRect2D_updateSize(rect, VECTOR2x(0x08000, 0x0B000));
	TestCase_assert(rect->size.x == 0x08000, "Expected 0x08000 but got 0x%05X", rect->size.x);
	TestCase_assert(rect->size.y == 0x0B000, "Expected 0x0B000 but got 0x%05X", rect->size.y);
	TestCase_assert(rect->lastSize.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastSize.x);
	TestCase_assert(rect->lastSize.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastSize.y);
}

static void testUpdateBounds() {
	TestCase_assert(false, "Unimplemented");
}

static void testInterpolate() {
	TestCase_assert(false, "Unimplemented");
}

TEST_SUITE(CollisionRect2DTest,
           testInit,
           testUpdatePosition,
           testUpdateSize,
           testUpdateBounds,
           testInterpolate)
