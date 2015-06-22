#include "collision/CollisionCircle.h"
#include "collision/CollisionRect2D.h"
#include "collision/StandardIntersectionHandlers.h"
#include "unittest/TestSuite.h"

static void testRect2D_rect2D() {
	CollisionRect2D * rect1, * rect2;
	bool result;
	fixed16_16 time;
	Vector3x normal;
	
	// No collision for no movement
	rect1 = CollisionRect2D_create(NULL, NULL, VECTOR2x(-0x20000, 0x00000), VECTOR2x(0x10000, 0x10000), false);
	rect2 = CollisionRect2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false);
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
	
	// Scrap above; new tests should verify basic functionality and important edge cases without trying to be exhaustive
	// * rect1 moving +x, rect2 stationary
	// * rect1 stationary, rect2 moving -x
	// * rect1 moving +x, rect2 moving -x
	// * rect1 -x, rect2 stationary
	// * rect1 +y, rect2 stationary
	// * rect1 -y, rect2 stationary
	// * Corner collision
	// * Misses that pass through each plane
	// Variations: Size, changing size, concavity, side solidity (need API)
	
	TestCase_assert(false, "Unimplemented");
}
/*
static void testRect2D_circle() {
	TestCase_assert(false, "Unimplemented");
}

static void testRect2D_line2D() {
	TestCase_assert(false, "Unimplemented");
}

static void testRect2D_polygon() {
	TestCase_assert(false, "Unimplemented");
}
*/
static void testCircle_circle() {
	CollisionCircle * circle1, * circle2;
	bool result;
	fixed16_16 time;
	Vector3x normal;
	
	// No collision for no movement
	circle1 = CollisionCircle_create(NULL, NULL, VECTOR2x(-0x20000, 0x00000), 0x08000, false);
	circle2 = CollisionCircle_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), 0x08000, false);
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, NULL, NULL);
	TestCase_assert(!result, "Expected false but got true");
	
	// circle1 moving +x (level), circle2 stationary
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x20000, 0x00000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle1 moving +x (level), circle2 stationary (different speed/collision time)
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x20000, 0x00000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x20000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x04000, "Expected 0x04000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle1 moving +x (level), circle2 stationary (circle1 starts butted against circle2)
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x10000, 0x00000));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x00000, 0x00000));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x00000, "Expected 0x00000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x10000 && normal.y == 0x00000 && normal.z == 0x00000, "Expected {0xFFFF0000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// circle1 moving +x (45 degrees up), circle2 stationary
	CollisionCircle_updatePosition(circle1, VECTOR2x(-0x14AFC, 0x0B504));
	CollisionCircle_updatePosition(circle1, VECTOR2x(0x0B504, 0x0B504));
	time = -1;
	memset(&normal, 0xFF, sizeof(normal));
	result = intersectionHandler_circle_circle((CollisionObject *) circle1, (CollisionObject *) circle2, &time, &normal);
	TestCase_assert(result, "Expected true but got false");
	TestCase_assert(time == 0x08000, "Expected 0x08000 but got 0x%05X", time);
	TestCase_assert(normal.x == -0x0B504 && normal.y == 0x0B504 && normal.z == 0x00000, "Expected {0xFFFF4AFC, 0x0B504, 0x00000} but got {0x%05X, 0x%05X, 0x%05X}", normal.x, normal.y, normal.z);
	
	// rect1 stationary, rect2 moving -x
	// rect1 moving +x, rect2 moving -x
	// rect1 moving -x, rect2 stationary
	// rect1 moving +y, rect2 stationary
	// rect1 moving -y, rect2 stationary
	// Corner collision (bottom left)
	// Corner collision (top left)
	// Corner collision (bottom right)
	// Corner collision (top right)
	// Miss (left, above)
	// Miss (left, below)
	// Miss (right, above)
	// Miss (right, below)
	// Miss (bottom, left)
	// Miss (bottom, right)
	// Miss (top, left)
	// Miss (top, right)
	
	TestCase_assert(false, "Unimplemented");
}
/*
static void testCircle_line2D() {
	TestCase_assert(false, "Unimplemented");
}

static void testCircle_polygon() {
	TestCase_assert(false, "Unimplemented");
}

static void testLine2D_line2D() {
	TestCase_assert(false, "Unimplemented");
}

static void testLine2D_polygon() {
	TestCase_assert(false, "Unimplemented");
}

static void testPolygon_polygon() {
	TestCase_assert(false, "Unimplemented");
}

static void testBox_box() {
	TestCase_assert(false, "Unimplemented");
}

static void testBox_sphere() {
	TestCase_assert(false, "Unimplemented");
}

static void testBox_line3D() {
	TestCase_assert(false, "Unimplemented");
}

static void testBox_cylinder() {
	TestCase_assert(false, "Unimplemented");
}

static void testBox_capsule() {
	TestCase_assert(false, "Unimplemented");
}

static void testBox_trimesh() {
	TestCase_assert(false, "Unimplemented");
}

static void testSphere_sphere() {
	TestCase_assert(false, "Unimplemented");
}

static void testSphere_line3D() {
	TestCase_assert(false, "Unimplemented");
}

static void testSphere_cylinder() {
	TestCase_assert(false, "Unimplemented");
}

static void testSphere_capsule() {
	TestCase_assert(false, "Unimplemented");
}

static void testSphere_trimesh() {
	TestCase_assert(false, "Unimplemented");
}

static void testLine3D_line3D() {
	TestCase_assert(false, "Unimplemented");
}

static void testLine3D_cylinder() {
	TestCase_assert(false, "Unimplemented");
}

static void testLine3D_capsule() {
	TestCase_assert(false, "Unimplemented");
}

static void testLine3D_trimesh() {
	TestCase_assert(false, "Unimplemented");
}

static void testCylinder_cylinder() {
	TestCase_assert(false, "Unimplemented");
}

static void testCylinder_capsule() {
	TestCase_assert(false, "Unimplemented");
}

static void testCylinder_trimesh() {
	TestCase_assert(false, "Unimplemented");
}

static void testCapsule_capsule() {
	TestCase_assert(false, "Unimplemented");
}

static void testCapsule_trimesh() {
	TestCase_assert(false, "Unimplemented");
}

static void testTrimesh_trimesh() {
	TestCase_assert(false, "Unimplemented");
}
*/
TEST_SUITE(StandardIntersectionHandlersTest,
           testRect2D_rect2D,/*
           testRect2D_circle,
           testRect2D_line2D,
           testRect2D_polygon,*/
           testCircle_circle/*,
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
           testTrimesh_trimesh*/)
