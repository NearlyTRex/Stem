#include "collision/CollisionRect2D.h"
#include "collision/StandardIntersectionHandlers.h"
#include "unittest/TestSuite.h"

static void testRect2D_rect2D() {
	//CollisionRect2D * rect1, * rect2;
	
	// Verify basic collision between two convex rects (rect1 moving +x, rect2 stationary)
	//rect1 = CollisionRect2D_create(NULL, NULL, false);
	//rect2 = CollisionRect2D_create(NULL, NULL, false);
	//CollisionRect2D_updateSize(rect1, 
	
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

static void testCircle_circle() {
	TestCase_assert(false, "Unimplemented");
}

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
           testRect2D_rect2D/*,
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
           testTrimesh_trimesh*/)
