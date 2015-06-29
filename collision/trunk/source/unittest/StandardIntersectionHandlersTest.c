#include "collision/CollisionCircle.h"
#include "collision/CollisionRect2D.h"
#include "collision/CollisionStaticTrimesh.h"
#include "collision/StandardIntersectionHandlers.h"
#include "unittest/TestSuite.h"

static void testRect2D_rect2D() {
	CollisionRect2D * rect1, * rect2;
	bool result;
	fixed16_16 time;
	Vector3x normal;
	
	// No collision for no movement
	rect1 = CollisionRect2D_create(NULL, NULL, VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x10000, 0x10000));
	rect2 = CollisionRect2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, NULL, NULL);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving +x, rect2 stationary
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 moving +x, rect2 stationary (different speed/collision time)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x04000, "Expected 0x04000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 moving +x, rect2 stationary (rect1 starts butted against rect2)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x10000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x00000, "Expected 0x00000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 moving +x, rect2 stationary (rect1 starts barely inside rect2, so passes through)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x0FFFF, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 stationary, rect2 moving -x
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect2, VECTOR2x(0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect2, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 moving +x, rect2 moving -x
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect2, VECTOR2x(0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect2, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x0C000, "Expected 0x0C000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 moving -x, rect2 stationary
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect2, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect2, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 moving +y, rect2 stationary
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, -0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x00000 && normal.y == -0x10000 && normal.z == 0x00000, "Expected {0x00000, 0xFFFF0000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 moving -y, rect2 stationary
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x00000 && normal.y == 0x10000 && normal.z == 0x00000, "Expected {0x00000, 0x10000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// Corner collision (bottom left)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, -0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// Corner collision (top left)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// Corner collision (bottom right)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, -0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// Corner collision (top right)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// Miss (left, above)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x10000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Miss (left, below)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, -0x10000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, -0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Miss (right, above)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x10000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Miss (right, below)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, -0x10000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, -0x10000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Miss (bottom, left)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x10000, -0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x10000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Miss (bottom, right)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x10000, -0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x10000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Miss (top, left)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x10000, 0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x10000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Miss (top, right)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x10000, 0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x10000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving -x from inside rect2
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving +x from inside rect2
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving -y from inside rect2
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, -0x20000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving +y from inside rect2
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x20000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Corner collision (bottom left, from inside)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, -0x20000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (top left, from inside)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x20000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (bottom right, from inside)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, -0x20000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected true but got false");
	
	// Corner collision (top right, from inside)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x20000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected true but got false");
	
	// rect1 size increasing, rect2 stationary
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x00000));
	CollisionRect2D_updateSize(rect1, VECTOR2x(0x10000, 0x10000));
	CollisionRect2D_updateSize(rect1, VECTOR2x(0x30000, 0x10000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 stationary, rect2 size increasing
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x00000));
	CollisionRect2D_updateSize(rect1, VECTOR2x(0x10000, 0x10000));
	CollisionRect2D_updateSize(rect1, VECTOR2x(0x10000, 0x10000));
	CollisionRect2D_updateSize(rect2, VECTOR2x(0x10000, 0x10000));
	CollisionRect2D_updateSize(rect2, VECTOR2x(0x30000, 0x10000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 moving +x, rect2 stationary (unsolid left)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_updateSize(rect2, VECTOR2x(0x10000, 0x10000));
	CollisionRect2D_updateSize(rect2, VECTOR2x(0x10000, 0x10000));
	CollisionRect2D_setSolidity(rect2, false, true, true, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving +x (unsolid right), rect2 stationary
	CollisionRect2D_setSolidity(rect1, true, false, true, true);
	CollisionRect2D_setSolidity(rect2, true, true, true, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving -x, rect2 stationary (unsolid right)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x00000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_setSolidity(rect1, true, true, true, true);
	CollisionRect2D_setSolidity(rect2, true, false, true, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving -x (unsolid left), rect2 stationary
	CollisionRect2D_setSolidity(rect1, false, true, true, true);
	CollisionRect2D_setSolidity(rect2, true, true, true, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving +y, rect2 stationary (unsolid bottom)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, -0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_setSolidity(rect2, true, true, false, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving +y (unsolid top), rect2 stationary
	CollisionRect2D_setSolidity(rect1, true, true, true, false);
	CollisionRect2D_setSolidity(rect2, true, true, true, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving -y, rect2 stationary (unsolid top)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	CollisionRect2D_setSolidity(rect2, true, true, true, false);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// rect1 moving -y (unsolid bottom), rect2 stationary
	CollisionRect2D_setSolidity(rect1, true, true, false, true);
	CollisionRect2D_setSolidity(rect2, true, true, true, true);
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Corner collision (bottom left, not solid)
	CollisionRect2D_setSolidity(rect2, false, false, false, false);
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, -0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Corner collision (top left, not solid)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(-0x20000, 0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Corner collision (bottom right, not solid)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, -0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	// Corner collision (top right, not solid)
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x20000, 0x20000));
	CollisionRect2D_updatePosition(rect1, VECTOR2x(0x00000, 0x00000));
	result = intersectionHandler_rect2D_rect2D((CollisionObject *) rect1, (CollisionObject *) rect2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionRect2D_dispose(rect1);
	CollisionRect2D_dispose(rect2);
}

static void testRect2D_circle() {
	CollisionRect2D * rect;
	CollisionCircle * circle;
	bool result;
	fixed16_16 time;
	Vector3x normal;
	
	// No collision for no movement
	rect = CollisionRect2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000));
	circle = CollisionCircle_create(NULL, NULL, VECTOR2x(-0x20000, 0x00000), 0x10000);
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, NULL, NULL);
	TestCase_assert(!result, "Expected false but got true");
	
	// circle moving +x, rect stationary
	CollisionCircle_updatePosition(circle, VECTOR2x(-0x20000, 0x08000));
	CollisionCircle_updatePosition(circle, VECTOR2x(0x00000, 0x08000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle moving +x, rect stationary (different speed/collision time)
	CollisionCircle_updatePosition(circle, VECTOR2x(-0x20000, 0x08000));
	CollisionCircle_updatePosition(circle, VECTOR2x(0x20000, 0x08000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x04000, "Expected 0x04000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle moving -x, rect stationary
	CollisionCircle_updatePosition(circle, VECTOR2x(0x20000, 0x08000));
	CollisionCircle_updatePosition(circle, VECTOR2x(0x00000, 0x08000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle moving +y, rect stationary
	CollisionCircle_updatePosition(circle, VECTOR2x(0x08000, -0x28000));
	CollisionCircle_updatePosition(circle, VECTOR2x(0x08000, 0x08000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x00000 && normal.y == -0x10000 && normal.z == 0x00000, "Expected {0x00000, 0xFFFF0000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle moving -y, rect stationary
	CollisionCircle_updatePosition(circle, VECTOR2x(0x08000, 0x28000));
	CollisionCircle_updatePosition(circle, VECTOR2x(0x08000, 0x08000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x00000 && normal.y == 0x10000 && normal.z == 0x00000, "Expected {0x00000, 0x10000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect moving +x, circle stationary
	CollisionCircle_updatePosition(circle, VECTOR2x(0x00000, 0x00000));
	CollisionCircle_updatePosition(circle, VECTOR2x(0x00000, 0x00000));
	// TODO: These numbers are wrong!
	CollisionRect2D_updatePosition(rect, VECTOR2x(-0x20000, 0x08000));
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x00000, 0x08000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect moving -x, circle stationary
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x20000, 0x08000));
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x00000, 0x08000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0x10000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect moving +y, circle stationary
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x08000, -0x28000));
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x08000, 0x08000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x00000 && normal.y == -0x10000 && normal.z == 0x00000, "Expected {0x00000, 0xFFFF0000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect moving -y, circle stationary
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x08000, 0x28000));
	CollisionRect2D_updatePosition(rect, VECTOR2x(0x08000, 0x08000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_rect2D_circle((CollisionObject *) rect, (CollisionObject *) circle, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == 0x00000 && normal.y == 0x10000 && normal.z == 0x00000, "Expected {0x00000, 0x10000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	CollisionRect2D_dispose(rect);
	CollisionCircle_dispose(circle);
	
	TestCase_assert(false, "Unimplemented");
}

static void testRect2D_line2D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testRect2D_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCircle_circle() {
	CollisionCircle * circle1, * circle2;
	bool result;
	fixed16_16 time;
	Vector3x normal;
	
	// No collision for no movement (no contact)
	circle1 = CollisionCircle_create(NULL, NULL, VECTOR2x(-0x40000, 0x00000), 0x10000);
	circle2 = CollisionCircle_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), 0x10000);
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, NULL, NULL);
	TestCase_assert(!result, "Expected false but got true");
	
	// circle1 moving +x (level), circle2 stationary
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x40000, 0x00000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle1 moving +x (level), circle2 stationary (different speed/collision time)
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x40000, 0x00000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x40000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x04000, "Expected 0x04000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle1 moving +x (level), circle2 stationary (different radius)
	circle2->radius = 0x20000;
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x40000, 0x00000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x04000, "Expected 0x04000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle1 moving +x (level), circle2 stationary (circle1 starts butted against circle2)
	circle2->radius = 0x10000;
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x20000, 0x00000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x00000, "Expected 0x00000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle1 moving +x (45 degrees up), circle2 stationary
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x40000, 0x16A09));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x00000, 0x16A09));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x0A57D, "Expected 0x0A57D but got 0x%05X", time);
	TestCase_assert(normal.x == (fixed16_16) 0xFFFF4AFA && normal.y == 0x0B505 && normal.z == 0x00000, "Expected {0xFFFF4AFA, 0x0B505, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle1 moving -y, circle2 moving +x
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x00000, 0x20000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x00000, -0x20000));
	CollisionCircle_updatePosition(circle2, VECTOR2x(-0x20000, 0x00000));
	CollisionCircle_updatePosition(circle2, VECTOR2x(0x20000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x0257E, "Expected 0x0257E but got 0x%05X", time);
	TestCase_assert(normal.x == 0x0B505 && normal.y == 0x0B505 && normal.z == 0x00000, "Expected {0x0B505, 0x0B505, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// No collision for no movement (contact)
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x10000, 0x00000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x00000, 0x00000));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, NULL, NULL);
	TestCase_assert(!result, "Expected false but got true");
	
	// Miss (circle1 above moving +x, circle2 below moving -x)
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x10000, 0x10000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x10000, 0x10000));
	CollisionCircle_updatePosition(circle2, VECTOR2x(0x10000, -0x10000));
	CollisionCircle_updatePosition(circle2, VECTOR2x(-0x10000, -0x10000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(!result, "Expected false but got true");
	TestCase_assert(time == -1, "Expected -1 but got 0x%05X", time);
	TestCase_assert(normal.x == -1 && normal.y == -1 && normal.x == -1, "Expected {-1, -1, -1} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	CollisionCircle_dispose(circle1);
	CollisionCircle_dispose(circle2);
}

static void testCircle_line2D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCircle_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine2D_line2D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine2D_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testPolygon_polygon() {
	//TestCase_assert(false, "Unimplemented");
}

static void testBox_box() {
	TestCase_assert(false, "Unimplemented");
}

static void testBox_sphere() {
	TestCase_assert(false, "Unimplemented");
}

static void testBox_line3D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testBox_cylinder() {
	//TestCase_assert(false, "Unimplemented");
}

static void testBox_capsule() {
	//TestCase_assert(false, "Unimplemented");
}

static void testBox_trimesh() {
	//TestCase_assert(false, "Unimplemented");
}

static void testSphere_sphere() {
	TestCase_assert(false, "Unimplemented");
}

static void testSphere_line3D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testSphere_cylinder() {
	//TestCase_assert(false, "Unimplemented");
}

static void testSphere_capsule() {
	//TestCase_assert(false, "Unimplemented");
}

static void testSphere_trimesh() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine3D_line3D() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine3D_cylinder() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine3D_capsule() {
	//TestCase_assert(false, "Unimplemented");
}

static void testLine3D_trimesh() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCylinder_cylinder() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCylinder_capsule() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCylinder_trimesh() {
	//TestCase_assert(false, "Unimplemented");
}

static void testCapsule_capsule() {
	TestCase_assert(false, "Unimplemented");
}

static void testCapsule_trimesh() {
	TestCase_assert(false, "Unimplemented");
}

static void testTrimesh_trimesh() {
	CollisionStaticTrimesh * trimesh1, * trimesh2;
	Vector3x vertices[3] = {{0x00000, 0x00000, 0x00000}, {0x10000, 0x00000, 0x00000}, {0x00000, 0x10000, 0x00000}};
	bool result;
	
	trimesh1 = CollisionStaticTrimesh_create(NULL, NULL, vertices, 3, false, false);
	trimesh2 = CollisionStaticTrimesh_create(NULL, NULL, vertices, 3, false, false);
	result = intersectionHandler_trimesh_trimesh((CollisionObject *) trimesh1, (CollisionObject *) trimesh2, NULL, NULL);
	TestCase_assert(!result, "Expected false but got true");
	
	CollisionStaticTrimesh_dispose(trimesh1);
	CollisionStaticTrimesh_dispose(trimesh2);
}

TEST_SUITE(StandardIntersectionHandlersTest,
           testRect2D_rect2D,
           testRect2D_circle,
           testRect2D_line2D,
           testRect2D_polygon,
           testCircle_circle,
           testCircle_line2D,
           testCircle_polygon,
           testLine2D_line2D,
           testLine2D_polygon,
           testPolygon_polygon,
           testBox_box,
           testBox_sphere,
           testBox_line3D,
           testBox_cylinder,
           testBox_capsule,
           testBox_trimesh,
           testSphere_sphere,
           testSphere_line3D,
           testSphere_cylinder,
           testSphere_capsule,
           testSphere_trimesh,
           testLine3D_line3D,
           testLine3D_cylinder,
           testLine3D_capsule,
           testLine3D_trimesh,
           testCylinder_cylinder,
           testCylinder_capsule,
           testCylinder_trimesh,
           testCapsule_capsule,
           testCapsule_trimesh,
           testTrimesh_trimesh)
