#include "collision/CollisionRect2D.h"
#include "collision/CollisionShared.h"
#include "unittest/TestSuite.h"

static void verifyInit(int line, CollisionRect2D * rect, void * owner, CollisionCallback collisionCallback, Vector2x position, Vector2x size) {
	TestCase_assert(rect != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(rect->dispose == CollisionRect2D_dispose, "Expected %p but got %p (line %d)", CollisionRect2D_dispose, rect->dispose, line);
	TestCase_assert(rect->shapeType == COLLISION_SHAPE_RECT_2D, "Expected %d but got %d (line %d)", COLLISION_SHAPE_RECT_2D, rect->shapeType, line);
	TestCase_assert(rect->owner == owner, "Expected %p but got %p (line %d)", owner, rect->owner, line);
	TestCase_assert(rect->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, rect->collisionCallback, line);
	TestCase_assert(rect->interpolate == CollisionRect2D_interpolate, "Expected %p but got %p (line %d)", CollisionRect2D_interpolate, rect->interpolate, line);
	TestCase_assert(rect->isStatic == CollisionRect2D_isStatic, "Expected %p but got %p (line %d)", CollisionRect2D_isStatic, rect->isStatic, line);
	TestCase_assert(rect->getCollisionBounds == CollisionRect2D_getCollisionBounds, "Expected %p but got %p (line %d)", CollisionRect2D_getCollisionBounds, rect->getCollisionBounds, line);
	TestCase_assert(rect->position.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, rect->position.x, line);
	TestCase_assert(rect->position.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, rect->position.y, line);
	TestCase_assert(rect->lastPosition.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, rect->lastPosition.x, line);
	TestCase_assert(rect->lastPosition.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, rect->lastPosition.y, line);
	TestCase_assert(rect->size.x == size.x, "Expected 0x%05X but got 0x%05X (line %d)", size.x, rect->size.x, line);
	TestCase_assert(rect->size.y == size.y, "Expected 0x%05X but got 0x%05X (line %d)", size.y, rect->size.y, line);
	TestCase_assert(rect->lastSize.x == size.x, "Expected 0x%05X but got 0x%05X (line %d)", size.x, rect->lastSize.x, line);
	TestCase_assert(rect->lastSize.y == size.y, "Expected 0x%05X but got 0x%05X (line %d)", size.y, rect->lastSize.y, line);
	TestCase_assert(rect->solidLeft, "Expected true but got false");
	TestCase_assert(rect->solidRight, "Expected true but got false");
	TestCase_assert(rect->solidBottom, "Expected true but got false");
	TestCase_assert(rect->solidTop, "Expected true but got false");
}

static void testInit() {
	CollisionRect2D rect, * rectPtr;
	
	memset(&rect, 0x00, sizeof(rect));
	CollisionRect2D_init(&rect, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO);
	verifyInit(__LINE__, &rect, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO);
	CollisionRect2D_dispose(&rect);
	
	memset(&rect, 0xFF, sizeof(rect));
	CollisionRect2D_init(&rect, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO);
	verifyInit(__LINE__, &rect, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO);
	CollisionRect2D_dispose(&rect);
	
	memset(&rect, 0x00, sizeof(rect));
	CollisionRect2D_init(&rect, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), VECTOR2x(0x30000, 0x40000));
	verifyInit(__LINE__, &rect, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), VECTOR2x(0x30000, 0x40000));
	CollisionRect2D_dispose(&rect);
	
	rectPtr = CollisionRect2D_create(NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO);
	verifyInit(__LINE__, rectPtr, NULL, NULL, VECTOR2x_ZERO, VECTOR2x_ZERO);
	CollisionRect2D_dispose(rectPtr);
	
	rectPtr = CollisionRect2D_create((void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), VECTOR2x(0x30000, 0x40000));
	verifyInit(__LINE__, rectPtr, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR2x(0x10000, 0x20000), VECTOR2x(0x30000, 0x40000));
	CollisionRect2D_dispose(rectPtr);
}

static void testUpdatePosition() {
	CollisionRect2D * rect;
	
	rect = CollisionRect2D_create(NULL, NULL, VECTOR2x(0x10000, 0x20000), VECTOR2x_ZERO);
	TestCase_assert(rect->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->position.x);
	TestCase_assert(rect->position.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->position.y);
	TestCase_assert(rect->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->lastPosition.x);
	TestCase_assert(rect->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->lastPosition.y);
	
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x00000, 0x00000));
	TestCase_assert(rect->position.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->position.x);
	TestCase_assert(rect->position.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->position.y);
	TestCase_assert(rect->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->lastPosition.x);
	TestCase_assert(rect->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->lastPosition.y);
	
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x08000, 0x0C000));
	TestCase_assert(rect->position.x == 0x08000, "Expected 0x08000 but got 0x%05X", rect->position.x);
	TestCase_assert(rect->position.y == 0x0C000, "Expected 0x0C000 but got 0x%05X", rect->position.y);
	TestCase_assert(rect->lastPosition.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastPosition.x);
	TestCase_assert(rect->lastPosition.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastPosition.y);
	
	CollisionRect2D_dispose(rect);
}

static void testUpdateSize() {
	CollisionRect2D * rect;
	
	rect = CollisionRect2D_create(NULL, NULL, VECTOR2x_ZERO, VECTOR2x(0x10000, 0x20000));
	TestCase_assert(rect->size.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->size.x);
	TestCase_assert(rect->size.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->size.y);
	TestCase_assert(rect->lastSize.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->lastSize.x);
	TestCase_assert(rect->lastSize.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->lastSize.y);
	
	CollisionRect2D_updateSize(rect, VECTOR2x(0x00000, 0x00000));
	TestCase_assert(rect->size.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->size.x);
	TestCase_assert(rect->size.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->size.y);
	TestCase_assert(rect->lastSize.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->lastSize.x);
	TestCase_assert(rect->lastSize.y == 0x20000, "Expected 0x20000 but got 0x%05X", rect->lastSize.y);
	
	CollisionRect2D_updateSize(rect, VECTOR2x(0x08000, 0x0C000));
	TestCase_assert(rect->size.x == 0x08000, "Expected 0x08000 but got 0x%05X", rect->size.x);
	TestCase_assert(rect->size.y == 0x0C000, "Expected 0x0C000 but got 0x%05X", rect->size.y);
	TestCase_assert(rect->lastSize.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastSize.x);
	TestCase_assert(rect->lastSize.y == 0x00000, "Expected 0x00000 but got 0x%05X", rect->lastSize.y);
	
	CollisionRect2D_dispose(rect);
}

static void testSetSolidity() {
	CollisionRect2D * rect;
	
	rect = CollisionRect2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_setSolidity(rect, true, false, true, false);
	TestCase_assert(rect->solidLeft, "Expected true but got false");
	TestCase_assert(!rect->solidRight, "Expected false but got true");
	TestCase_assert(rect->solidBottom, "Expected true but got false");
	TestCase_assert(!rect->solidTop, "Expected false but got true");
	
	CollisionRect2D_setSolidity(rect, false, true, false, true);
	TestCase_assert(!rect->solidLeft, "Expected false but got true");
	TestCase_assert(rect->solidRight, "Expected true but got false");
	TestCase_assert(!rect->solidBottom, "Expected false but got true");
	TestCase_assert(rect->solidTop, "Expected true but got false");
	
	CollisionRect2D_dispose(rect);
}

static void testInterpolate() {
	CollisionRect2D * rect;
	
	rect = CollisionRect2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x10000, 0x10000));
	CollisionRect2D_updateSize(rect, VECTOR2x(0x10000, 0x10000));
	CollisionRect2D_interpolate(rect, 0x08000);
	
	TestCase_assert(rect->lastPosition.x == 0x08000, "Expected 0x08000 but got 0x%05X", rect->lastPosition.x);
	TestCase_assert(rect->lastPosition.y == 0x08000, "Expected 0x08000 but got 0x%05X", rect->lastPosition.y);
	TestCase_assert(rect->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->position.x);
	TestCase_assert(rect->position.y == 0x10000, "Expected 0x10000 but got 0x%05X", rect->position.y);
	TestCase_assert(rect->lastSize.x == 0x08000, "Expected 0x08000 but got 0x%05X", rect->lastSize.x);
	TestCase_assert(rect->lastSize.y == 0x08000, "Expected 0x08000 but got 0x%05X", rect->lastSize.y);
	TestCase_assert(rect->size.x == 0x10000, "Expected 0x10000 but got 0x%05X", rect->size.x);
	TestCase_assert(rect->size.y == 0x10000, "Expected 0x10000 but got 0x%05X", rect->size.y);
	
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x20000, 0x30000));
	CollisionRect2D_updateSize(rect, VECTOR2x(0x00000, 0x18000));
	CollisionRect2D_interpolate(rect, 0x04000);
	
	TestCase_assert(rect->lastPosition.x == 0x14000, "Expected 0x14000 but got 0x%05X", rect->lastPosition.x);
	TestCase_assert(rect->lastPosition.y == 0x18000, "Expected 0x18000 but got 0x%05X", rect->lastPosition.y);
	TestCase_assert(rect->position.x == 0x20000, "Expected 0x20000 but got 0x%05X", rect->position.x);
	TestCase_assert(rect->position.y == 0x30000, "Expected 0x30000 but got 0x%05X", rect->position.y);
	TestCase_assert(rect->lastSize.x == 0x0C000, "Expected 0x0C000 but got 0x%05X", rect->lastSize.x);
	TestCase_assert(rect->lastSize.y == 0x12000, "Expected 0x12000 but got 0x%05X", rect->lastSize.y);
	TestCase_assert(rect->size.x == 0x00000, "Expected 0x00000 but got 0x%05X", rect->size.x);
	TestCase_assert(rect->size.y == 0x18000, "Expected 0x18000 but got 0x%05X", rect->size.y);
	
	CollisionRect2D_dispose(rect);
}

static void testIsStatic() {
	CollisionRect2D * rect;
	bool result;
	
	rect = CollisionRect2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = CollisionRect2D_isStatic(rect);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x10000, 0x10000));
	result = CollisionRect2D_isStatic(rect);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x10000, 0x10000));
	result = CollisionRect2D_isStatic(rect);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionRect2D_updateSize(rect, VECTOR2x(0x20000, 0x20000));
	result = CollisionRect2D_isStatic(rect);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionRect2D_updateSize(rect, VECTOR2x(0x20000, 0x20000));
	result = CollisionRect2D_isStatic(rect);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionRect2D_dispose(rect);
}

static void testGetCollisionBounds() {
	CollisionRect2D * rect;
	Box6x bounds;
	
	rect = CollisionRect2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	bounds = CollisionRect2D_getCollisionBounds(rect);
	TestCase_assert(bounds.left == 0x00000 && bounds.right == 0x10000 &&
	                bounds.bottom == 0x00000 && bounds.top == 0x10000 &&
	                bounds.back == 0x00000 && bounds.front == 0x10000,
	                "Expected {0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x10000, 0x10000));
	bounds = CollisionRect2D_getCollisionBounds(rect);
	TestCase_assert(bounds.left == 0x00000 && bounds.right == 0x20000 &&
	                bounds.bottom == 0x00000 && bounds.top == 0x20000 &&
	                bounds.back == 0x00000 && bounds.front == 0x10000,
	                "Expected {0x00000, 0x20000, 0x00000, 0x20000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionRect2D_updatePosition(rect, VECTOR2x(-0x10000, -0x10000));
	bounds = CollisionRect2D_getCollisionBounds(rect);
	TestCase_assert(bounds.left == -0x10000 && bounds.right == 0x20000 &&
	                bounds.bottom == -0x10000 && bounds.top == 0x20000 &&
	                bounds.back == 0x00000 && bounds.front == 0x10000,
	                "Expected {0xFFFF0000, 0x20000, 0xFFFF0000, 0x20000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionRect2D_updateSize(rect, VECTOR2x(0x20000, 0x20000));
	CollisionRect2D_updateSize(rect, VECTOR2x(0x20000, 0x20000));
	bounds = CollisionRect2D_getCollisionBounds(rect);
	TestCase_assert(bounds.left == -0x10000 && bounds.right == 0x30000 &&
	                bounds.bottom == -0x10000 && bounds.top == 0x30000 &&
	                bounds.back == 0x00000 && bounds.front == 0x10000,
	                "Expected {0xFFFF0000, 0x30000, 0xFFFF0000, 0x30000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionRect2D_dispose(rect);
}

TEST_SUITE(CollisionRect2DTest,
           testInit,
           testUpdatePosition,
           testUpdateSize,
           testSetSolidity,
           testInterpolate,
           testIsStatic,
           testGetCollisionBounds)
