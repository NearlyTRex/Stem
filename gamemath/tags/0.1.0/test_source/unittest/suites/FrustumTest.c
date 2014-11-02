#include <math.h>
#include <stdio.h>
#include "3dmath/Frustum.h"
#include "3dmath/Matrix.h"
#include "unittest/framework/TestSuite.h"

void assertVector4Approximate(Vector4 lhs, Vector4 rhs, float epsilon) {
	TestCase_assert(fabs(lhs.x - rhs.x) < epsilon, "%f not within %f of expected %f", lhs.x, epsilon, rhs.x);
	TestCase_assert(fabs(lhs.y - rhs.y) < epsilon, "%f not within %f of expected %f", lhs.y, epsilon, rhs.y);
	TestCase_assert(fabs(lhs.z - rhs.z) < epsilon, "%f not within %f of expected %f", lhs.z, epsilon, rhs.z);
	TestCase_assert(fabs(lhs.w - rhs.w) < epsilon, "%f not within %f of expected %f", lhs.w, epsilon, rhs.w);
}

static void testFromMatrix() {
	Frustum frustum;
	
	frustum = Frustum_fromMatrix(Matrix_identity());
	assertVector4Approximate(frustum.left,   Vector4_init(1.0f, 0.0f, 0.0f, 1.0f), 0.0001f);
	assertVector4Approximate(frustum.right,  Vector4_init(-1.0f, 0.0f, 0.0f, 1.0f), 0.0001f);
	assertVector4Approximate(frustum.bottom, Vector4_init(0.0f, 1.0f, 0.0f, 1.0f), 0.0001f);
	assertVector4Approximate(frustum.top,    Vector4_init(0.0f, -1.0f, 0.0f, 1.0f), 0.0001f);
	assertVector4Approximate(frustum.back,   Vector4_init(0.0f, 0.0f, 1.0f, 1.0f), 0.0001f);
	assertVector4Approximate(frustum.front,  Vector4_init(0.0f, 0.0f, -1.0f, 1.0f), 0.0001f);
	
	frustum = Frustum_fromMatrix(Matrix_perspective(Matrix_identity(), 90.0f, 1.0f, 2, 10));
	assertVector4Approximate(frustum.left,   Vector4_init(0.707107f, 0.0f, -0.707107f, 0.0f), 0.0001f);
	assertVector4Approximate(frustum.right,  Vector4_init(-0.707107f, 0.0f, -0.707107f, 0.0f), 0.0001f);
	assertVector4Approximate(frustum.bottom, Vector4_init(0.0f, 0.707107f, -0.707107f, 0.0f), 0.0001f);
	assertVector4Approximate(frustum.top,    Vector4_init(0.0f, -0.707107f, -0.707107f, 0.0f), 0.0001f);
	assertVector4Approximate(frustum.back,   Vector4_init(0.0f, 0.0f, -1.0f, -2.0f), 0.0001f);
	assertVector4Approximate(frustum.front,  Vector4_init(0.0f, 0.0f, 1.0f, 10.0f), 0.0001f);
}

static void testContainsSphere() {
	Frustum frustum;
	
	frustum = Frustum_fromMatrix(Matrix_identity());
	TestCase_assert(Frustum_containsSphere(frustum, Vector3_init(0.0f, 0.0f, 0.0f), 1.0f), "Expected to contain sphere");
	TestCase_assert(Frustum_containsSphere(frustum, Vector3_init(-1.5f, 0.0f, 0.0f), 1.0f), "Expected to contain sphere");
	TestCase_assert(!Frustum_containsSphere(frustum, Vector3_init(-1.5f, 0.0f, 0.0f), 0.1f), "Expected not to contain sphere");
	
	frustum = Frustum_fromMatrix(Matrix_perspective(Matrix_identity(), 90.0f, 1.0f, 2, 10));
	TestCase_assert(Frustum_containsSphere(frustum, Vector3_init(0.0f, 0.0f, -6.0f), 1.0f), "Expected to contain sphere");
	TestCase_assert(Frustum_containsSphere(frustum, Vector3_init(10.5f, 0.0f, -10.0f), 1.0f), "Expected to contain sphere");
	TestCase_assert(!Frustum_containsSphere(frustum, Vector3_init(10.5f, 0.0f, -10.0f), 0.1f), "Expected not to contain sphere");
}

static void testSphereDistanceFromNearClippingPlane() {
	Frustum frustum;
	float distance;
	
	frustum = Frustum_fromMatrix(Matrix_identity());
	distance = Frustum_sphereDistanceFromNearClippingPlane(frustum, Vector3_init(0.0f, 0.0f, 0.5f), 0.0f);
	TestCase_assert(fabs(distance - 0.5f) < 0.0001f, "Distance %f not within 0.0001 of 0.5", distance);
	distance = Frustum_sphereDistanceFromNearClippingPlane(frustum, Vector3_init(0.0f, 0.0f, 0.5f), 0.25f);
	TestCase_assert(fabs(distance - 0.25f) < 0.0001f, "Distance %f not within 0.0001 of 0.25", distance);
	distance = Frustum_sphereDistanceFromNearClippingPlane(frustum, Vector3_init(0.0f, 0.0f, 1.5f), 0.0f);
	TestCase_assert(fabs(distance - -0.5f) < 0.0001f, "Distance %f not within 0.0001 of -0.5", distance);
	
	frustum = Frustum_fromMatrix(Matrix_rotated(Matrix_identity(), Vector3_init(0.0f, 1.0f, 0.0f), M_PI * 0.25f));
	distance = Frustum_sphereDistanceFromNearClippingPlane(frustum, Vector3_init(-0.3535535f, 0.0f, 0.3535535f), 0.0f);
	TestCase_assert(fabs(distance - 0.5f) < 0.0001f, "Distance %f not within 0.0001 of 0.5", distance);
	distance = Frustum_sphereDistanceFromNearClippingPlane(frustum, Vector3_init(-0.3535535f, 0.0f, 0.3535535f), 0.25f);
	TestCase_assert(fabs(distance - 0.25f) < 0.0001f, "Distance %f not within 0.0001 of 0.25", distance);
	distance = Frustum_sphereDistanceFromNearClippingPlane(frustum, Vector3_init(0.707107f, 0.0f, 0.707107f), 0.0f);
	TestCase_assert(fabs(distance - -1.0f) < 0.0001f, "Distance %f not within 0.0001 of -1.0", distance);
}

TEST_SUITE(FrustumTest, testFromMatrix, testContainsSphere, testSphereDistanceFromNearClippingPlane)
