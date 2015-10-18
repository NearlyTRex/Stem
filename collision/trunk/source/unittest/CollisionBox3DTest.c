#include "collision/CollisionBox3D.h"
#include "collision/CollisionShared.h"
#include "unittest/TestSuite.h"

static void verifyInit(int line, CollisionBox3D * box, void * owner, CollisionCallback collisionCallback, Vector3x position, Vector3x size, fixed16_16 edgeThickness) {
	TestCase_assert(box != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(box->dispose == CollisionBox3D_dispose, "Expected %p but got %p (line %d)", CollisionBox3D_dispose, box->dispose, line);
	TestCase_assert(box->shapeType == COLLISION_SHAPE_BOX_3D, "Expected %d but got %d (line %d)", COLLISION_SHAPE_BOX_3D, box->shapeType, line);
	TestCase_assert(box->owner == owner, "Expected %p but got %p (line %d)", owner, box->owner, line);
	TestCase_assert(box->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, box->collisionCallback, line);
	TestCase_assert(box->interpolate == CollisionBox3D_interpolate, "Expected %p but got %p (line %d)", CollisionBox3D_interpolate, box->interpolate, line);
	TestCase_assert(box->isStatic == CollisionBox3D_isStatic, "Expected %p but got %p (line %d)", CollisionBox3D_isStatic, box->isStatic, line);
	TestCase_assert(box->getCollisionBounds == CollisionBox3D_getCollisionBounds, "Expected %p but got %p (line %d)", CollisionBox3D_getCollisionBounds, box->getCollisionBounds, line);
	TestCase_assert(box->position.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, box->position.x, line);
	TestCase_assert(box->position.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, box->position.y, line);
	TestCase_assert(box->position.z == position.z, "Expected 0x%05X but got 0x%05X (line %d)", position.z, box->position.z, line);
	TestCase_assert(box->lastPosition.x == position.x, "Expected 0x%05X but got 0x%05X (line %d)", position.x, box->lastPosition.x, line);
	TestCase_assert(box->lastPosition.y == position.y, "Expected 0x%05X but got 0x%05X (line %d)", position.y, box->lastPosition.y, line);
	TestCase_assert(box->lastPosition.z == position.z, "Expected 0x%05X but got 0x%05X (line %d)", position.z, box->lastPosition.z, line);
	TestCase_assert(box->size.x == size.x, "Expected 0x%05X but got 0x%05X (line %d)", size.x, box->size.x, line);
	TestCase_assert(box->size.y == size.y, "Expected 0x%05X but got 0x%05X (line %d)", size.y, box->size.y, line);
	TestCase_assert(box->size.z == size.z, "Expected 0x%05X but got 0x%05X (line %d)", size.z, box->size.z, line);
	TestCase_assert(box->lastSize.x == size.x, "Expected 0x%05X but got 0x%05X (line %d)", size.x, box->lastSize.x, line);
	TestCase_assert(box->lastSize.y == size.y, "Expected 0x%05X but got 0x%05X (line %d)", size.y, box->lastSize.y, line);
	TestCase_assert(box->lastSize.z == size.z, "Expected 0x%05X but got 0x%05X (line %d)", size.z, box->lastSize.z, line);
	TestCase_assert(box->solidLeft, "Expected true but got false (line %d)", line);
	TestCase_assert(box->solidRight, "Expected true but got false (line %d)", line);
	TestCase_assert(box->solidBottom, "Expected true but got false (line %d)", line);
	TestCase_assert(box->solidTop, "Expected true but got false (line %d)", line);
	TestCase_assert(box->solidBack, "Expected true but got false (line %d)", line);
	TestCase_assert(box->solidFront, "Expected true but got false (line %d)", line);
	TestCase_assert(box->edgeThickness == edgeThickness, "Expected 0x%05X but got 0x%05X (line %d)", edgeThickness, box->edgeThickness, line);
}

static void testInit() {
	CollisionBox3D box, * boxPtr;
	
	memset(&box, 0x00, sizeof(box));
	CollisionBox3D_init(&box, NULL, NULL, VECTOR3x_ZERO, VECTOR3x_ZERO, 0x00000);
	verifyInit(__LINE__, &box, NULL, NULL, VECTOR3x_ZERO, VECTOR3x_ZERO, 0x00000);
	CollisionBox3D_dispose(&box);
	
	memset(&box, 0xFF, sizeof(box));
	CollisionBox3D_init(&box, NULL, NULL, VECTOR3x_ZERO, VECTOR3x_ZERO, 0x00000);
	verifyInit(__LINE__, &box, NULL, NULL, VECTOR3x_ZERO, VECTOR3x_ZERO, 0x00000);
	CollisionBox3D_dispose(&box);
	
	memset(&box, 0x00, sizeof(box));
	CollisionBox3D_init(&box, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), VECTOR3x(0x40000, 0x50000, 0x60000), 0x08000);
	verifyInit(__LINE__, &box, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), VECTOR3x(0x40000, 0x50000, 0x60000), 0x08000);
	CollisionBox3D_dispose(&box);
	
	boxPtr = CollisionBox3D_create(NULL, NULL, VECTOR3x_ZERO, VECTOR3x_ZERO, 0x00000);
	verifyInit(__LINE__, boxPtr, NULL, NULL, VECTOR3x_ZERO, VECTOR3x_ZERO, 0x00000);
	CollisionBox3D_dispose(boxPtr);
	
	boxPtr = CollisionBox3D_create((void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), VECTOR3x(0x40000, 0x50000, 0x60000), 0x08000);
	verifyInit(__LINE__, boxPtr, (void *) 0x1234, (CollisionCallback) 0x2345, VECTOR3x(0x10000, 0x20000, 0x30000), VECTOR3x(0x40000, 0x50000, 0x60000), 0x08000);
	CollisionBox3D_dispose(boxPtr);
}

static void testUpdatePosition() {
	CollisionBox3D * box;
	
	box = CollisionBox3D_create(NULL, NULL, VECTOR3x(0x10000, 0x20000, 0x30000), VECTOR3x_ZERO, 0x00000);
	TestCase_assert(box->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", box->position.x);
	TestCase_assert(box->position.y == 0x20000, "Expected 0x20000 but got 0x%05X", box->position.y);
	TestCase_assert(box->position.z == 0x30000, "Expected 0x30000 but got 0x%05X", box->position.z);
	TestCase_assert(box->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", box->lastPosition.x);
	TestCase_assert(box->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", box->lastPosition.y);
	TestCase_assert(box->lastPosition.z == 0x30000, "Expected 0x30000 but got 0x%05X", box->lastPosition.z);
	
	CollisionBox3D_updatePosition(box, VECTOR3x(0x00000, 0x00000, 0x00000));
	TestCase_assert(box->position.x == 0x00000, "Expected 0x00000 but got 0x%05X", box->position.x);
	TestCase_assert(box->position.y == 0x00000, "Expected 0x00000 but got 0x%05X", box->position.y);
	TestCase_assert(box->position.z == 0x00000, "Expected 0x00000 but got 0x%05X", box->position.z);
	TestCase_assert(box->lastPosition.x == 0x10000, "Expected 0x10000 but got 0x%05X", box->lastPosition.x);
	TestCase_assert(box->lastPosition.y == 0x20000, "Expected 0x20000 but got 0x%05X", box->lastPosition.y);
	TestCase_assert(box->lastPosition.z == 0x30000, "Expected 0x30000 but got 0x%05X", box->lastPosition.z);
	
	CollisionBox3D_updatePosition(box, VECTOR3x(0x08000, 0x0C000, 0x04000));
	TestCase_assert(box->position.x == 0x08000, "Expected 0x08000 but got 0x%05X", box->position.x);
	TestCase_assert(box->position.y == 0x0C000, "Expected 0x0C000 but got 0x%05X", box->position.y);
	TestCase_assert(box->position.z == 0x04000, "Expected 0x04000 but got 0x%05X", box->position.z);
	TestCase_assert(box->lastPosition.x == 0x00000, "Expected 0x00000 but got 0x%05X", box->lastPosition.x);
	TestCase_assert(box->lastPosition.y == 0x00000, "Expected 0x00000 but got 0x%05X", box->lastPosition.y);
	TestCase_assert(box->lastPosition.z == 0x00000, "Expected 0x00000 but got 0x%05X", box->lastPosition.z);
	
	CollisionBox3D_dispose(box);
}

static void testUpdateSize() {
	CollisionBox3D * box;
	
	box = CollisionBox3D_create(NULL, NULL, VECTOR3x_ZERO, VECTOR3x(0x10000, 0x20000, 0x30000), 0x00000);
	TestCase_assert(box->size.x == 0x10000, "Expected 0x10000 but got 0x%05X", box->size.x);
	TestCase_assert(box->size.y == 0x20000, "Expected 0x20000 but got 0x%05X", box->size.y);
	TestCase_assert(box->size.z == 0x30000, "Expected 0x30000 but got 0x%05X", box->size.z);
	TestCase_assert(box->lastSize.x == 0x10000, "Expected 0x10000 but got 0x%05X", box->lastSize.x);
	TestCase_assert(box->lastSize.y == 0x20000, "Expected 0x20000 but got 0x%05X", box->lastSize.y);
	TestCase_assert(box->lastSize.z == 0x30000, "Expected 0x30000 but got 0x%05X", box->lastSize.z);
	
	CollisionBox3D_updateSize(box, VECTOR3x(0x00000, 0x00000, 0x00000));
	TestCase_assert(box->size.x == 0x00000, "Expected 0x00000 but got 0x%05X", box->size.x);
	TestCase_assert(box->size.y == 0x00000, "Expected 0x00000 but got 0x%05X", box->size.y);
	TestCase_assert(box->size.z == 0x00000, "Expected 0x00000 but got 0x%05X", box->size.z);
	TestCase_assert(box->lastSize.x == 0x10000, "Expected 0x10000 but got 0x%05X", box->lastSize.x);
	TestCase_assert(box->lastSize.y == 0x20000, "Expected 0x20000 but got 0x%05X", box->lastSize.y);
	TestCase_assert(box->lastSize.z == 0x30000, "Expected 0x30000 but got 0x%05X", box->lastSize.z);
	
	CollisionBox3D_updateSize(box, VECTOR3x(0x08000, 0x0C000, 0x04000));
	TestCase_assert(box->size.x == 0x08000, "Expected 0x08000 but got 0x%05X", box->size.x);
	TestCase_assert(box->size.y == 0x0C000, "Expected 0x0C000 but got 0x%05X", box->size.y);
	TestCase_assert(box->size.z == 0x04000, "Expected 0x0C000 but got 0x%05X", box->size.z);
	TestCase_assert(box->lastSize.x == 0x00000, "Expected 0x00000 but got 0x%05X", box->lastSize.x);
	TestCase_assert(box->lastSize.y == 0x00000, "Expected 0x00000 but got 0x%05X", box->lastSize.y);
	TestCase_assert(box->lastSize.z == 0x00000, "Expected 0x00000 but got 0x%05X", box->lastSize.z);
	
	CollisionBox3D_dispose(box);
}

static void testSetSolidity() {
	CollisionBox3D * box;
	
	box = CollisionBox3D_create(NULL, NULL, VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), 0x00000);
	CollisionBox3D_setSolidity(box, true, false, true, false, true, false);
	TestCase_assert(box->solidLeft, "Expected true but got false");
	TestCase_assert(!box->solidRight, "Expected false but got true");
	TestCase_assert(box->solidBottom, "Expected true but got false");
	TestCase_assert(!box->solidTop, "Expected false but got true");
	TestCase_assert(box->solidBack, "Expected true but got false");
	TestCase_assert(!box->solidFront, "Expected false but got true");
	
	CollisionBox3D_setSolidity(box, false, true, false, true, false, true);
	TestCase_assert(!box->solidLeft, "Expected false but got true");
	TestCase_assert(box->solidRight, "Expected true but got false");
	TestCase_assert(!box->solidBottom, "Expected false but got true");
	TestCase_assert(box->solidTop, "Expected true but got false");
	TestCase_assert(!box->solidBack, "Expected false but got true");
	TestCase_assert(box->solidFront, "Expected true but got false");
	
	CollisionBox3D_dispose(box);
}

static void testInterpolate() {
	CollisionBox3D * box;
	
	box = CollisionBox3D_create(NULL, NULL, VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), 0x00000);
	CollisionBox3D_updatePosition(box, VECTOR3x(0x10000, 0x10000, 0x10000));
	CollisionBox3D_updateSize(box, VECTOR3x(0x10000, 0x10000, 0x10000));
	CollisionBox3D_interpolate(box, 0x08000);
	
	TestCase_assert(box->lastPosition.x == 0x08000, "Expected 0x08000 but got 0x%05X", box->lastPosition.x);
	TestCase_assert(box->lastPosition.y == 0x08000, "Expected 0x08000 but got 0x%05X", box->lastPosition.y);
	TestCase_assert(box->lastPosition.z == 0x08000, "Expected 0x08000 but got 0x%05X", box->lastPosition.z);
	TestCase_assert(box->position.x == 0x10000, "Expected 0x10000 but got 0x%05X", box->position.x);
	TestCase_assert(box->position.y == 0x10000, "Expected 0x10000 but got 0x%05X", box->position.y);
	TestCase_assert(box->position.z == 0x10000, "Expected 0x10000 but got 0x%05X", box->position.z);
	TestCase_assert(box->lastSize.x == 0x08000, "Expected 0x08000 but got 0x%05X", box->lastSize.x);
	TestCase_assert(box->lastSize.y == 0x08000, "Expected 0x08000 but got 0x%05X", box->lastSize.y);
	TestCase_assert(box->lastSize.z == 0x08000, "Expected 0x08000 but got 0x%05X", box->lastSize.z);
	TestCase_assert(box->size.x == 0x10000, "Expected 0x10000 but got 0x%05X", box->size.x);
	TestCase_assert(box->size.y == 0x10000, "Expected 0x10000 but got 0x%05X", box->size.y);
	TestCase_assert(box->size.z == 0x10000, "Expected 0x10000 but got 0x%05X", box->size.z);
	
	CollisionBox3D_updatePosition(box, VECTOR3x(0x20000, 0x30000, 0x40000));
	CollisionBox3D_updateSize(box, VECTOR3x(0x00000, 0x18000, 0x08000));
	CollisionBox3D_interpolate(box, 0x04000);
	
	TestCase_assert(box->lastPosition.x == 0x14000, "Expected 0x14000 but got 0x%05X", box->lastPosition.x);
	TestCase_assert(box->lastPosition.y == 0x18000, "Expected 0x18000 but got 0x%05X", box->lastPosition.y);
	TestCase_assert(box->lastPosition.z == 0x1C000, "Expected 0x1C000 but got 0x%05X", box->lastPosition.z);
	TestCase_assert(box->position.x == 0x20000, "Expected 0x20000 but got 0x%05X", box->position.x);
	TestCase_assert(box->position.y == 0x30000, "Expected 0x30000 but got 0x%05X", box->position.y);
	TestCase_assert(box->position.z == 0x40000, "Expected 0x40000 but got 0x%05X", box->position.z);
	TestCase_assert(box->lastSize.x == 0x0C000, "Expected 0x0C000 but got 0x%05X", box->lastSize.x);
	TestCase_assert(box->lastSize.y == 0x12000, "Expected 0x12000 but got 0x%05X", box->lastSize.y);
	TestCase_assert(box->lastSize.z == 0x0E000, "Expected 0x0E000 but got 0x%05X", box->lastSize.z);
	TestCase_assert(box->size.x == 0x00000, "Expected 0x00000 but got 0x%05X", box->size.x);
	TestCase_assert(box->size.y == 0x18000, "Expected 0x18000 but got 0x%05X", box->size.y);
	TestCase_assert(box->size.z == 0x08000, "Expected 0x08000 but got 0x%05X", box->size.z);
	
	CollisionBox3D_dispose(box);
}

static void testIsStatic() {
	CollisionBox3D * box;
	bool result;
	
	box = CollisionBox3D_create(NULL, NULL, VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), 0x00000);
	result = CollisionBox3D_isStatic(box);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionBox3D_updatePosition(box, VECTOR3x(0x10000, 0x10000, 0x10000));
	result = CollisionBox3D_isStatic(box);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionBox3D_updatePosition(box, VECTOR3x(0x10000, 0x10000, 0x10000));
	result = CollisionBox3D_isStatic(box);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionBox3D_updateSize(box, VECTOR3x(0x20000, 0x20000, 0x20000));
	result = CollisionBox3D_isStatic(box);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionBox3D_updateSize(box, VECTOR3x(0x20000, 0x20000, 0x20000));
	result = CollisionBox3D_isStatic(box);
	TestCase_assert(result, "Expected true but got false");
	
	CollisionBox3D_dispose(box);
}

static void testGetCollisionBounds() {
	CollisionBox3D * box;
	Box6x bounds;
	
	box = CollisionBox3D_create(NULL, NULL, VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), 0x00000);
	bounds = CollisionBox3D_getCollisionBounds(box);
	TestCase_assert(bounds.left == 0x00000 && bounds.right == 0x10000 &&
	                bounds.bottom == 0x00000 && bounds.top == 0x10000 &&
	                bounds.back == 0x00000 && bounds.front == 0x10000,
	                "Expected {0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionBox3D_updatePosition(box, VECTOR3x(0x10000, 0x10000, 0x10000));
	bounds = CollisionBox3D_getCollisionBounds(box);
	TestCase_assert(bounds.left == 0x00000 && bounds.right == 0x20000 &&
	                bounds.bottom == 0x00000 && bounds.top == 0x20000 &&
	                bounds.back == 0x00000 && bounds.front == 0x20000,
	                "Expected {0x00000, 0x20000, 0x00000, 0x20000, 0x00000, 0x20000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionBox3D_updatePosition(box, VECTOR3x(-0x10000, -0x10000, -0x10000));
	bounds = CollisionBox3D_getCollisionBounds(box);
	TestCase_assert(bounds.left == -0x10000 && bounds.right == 0x20000 &&
	                bounds.bottom == -0x10000 && bounds.top == 0x20000 &&
	                bounds.back == -0x10000 && bounds.front == 0x20000,
	                "Expected {0xFFFF0000, 0x20000, 0xFFFF0000, 0x20000, 0xFFFF0000, 0x20000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionBox3D_updateSize(box, VECTOR3x(0x20000, 0x20000, 0x20000));
	CollisionBox3D_updateSize(box, VECTOR3x(0x20000, 0x20000, 0x20000));
	bounds = CollisionBox3D_getCollisionBounds(box);
	TestCase_assert(bounds.left == -0x10000 && bounds.right == 0x30000 &&
	                bounds.bottom == -0x10000 && bounds.top == 0x30000 &&
	                bounds.back == -0x10000 && bounds.front == 0x30000,
	                "Expected {0xFFFF0000, 0x30000, 0xFFFF0000, 0x30000, 0xFFFF0000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	
	CollisionBox3D_dispose(box);
}

TEST_SUITE(CollisionBox3DTest,
           testInit,
           testUpdatePosition,
           testUpdateSize,
           testSetSolidity,
           testInterpolate,
           testIsStatic,
           testGetCollisionBounds)
