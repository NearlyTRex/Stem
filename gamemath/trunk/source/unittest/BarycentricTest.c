#include "gamemath/Barycentric.h"
#include "unittest/TestSuite.h"
#include <math.h>

#define assertVector2fExact(vector, expectedX, expectedY, expectedZ) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY, "Expected {%f, %f} but got {%f, %f}", expectedX, expectedY, vector.x, vector.y)

#define assertVector3fExact(vector, expectedX, expectedY, expectedZ) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY && vector.z == expectedZ, "Expected {%f, %f, %f} but got {%f, %f, %f}", expectedX, expectedY, expectedZ, vector.x, vector.y, vector.z)

#define assertVector2fApproximate(vector, expectedX, expectedY, expectedZ, epsilon) \
	TestCase_assert(fabs(vector.x - expectedX) < epsilon && fabs(vector.y - expectedY) < epsilon, "Expected {%f, %f} but got {%f, %f} (tolerance %f)", expectedX, expectedY, vector.x, vector.y, epsilon);

#define assertVector3fApproximate(vector, expectedX, expectedY, expectedZ, epsilon) \
	TestCase_assert(fabs(vector.x - expectedX) < epsilon && fabs(vector.y - expectedY) < epsilon && fabs(vector.z - expectedZ) < epsilon, "Expected {%f, %f, %f} but got {%f, %f, %f} (tolerance %f)", expectedX, expectedY, expectedZ, vector.x, vector.y, vector.z, epsilon);

#define assertVector2x(vector, expectedX, expectedY, expectedZ) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY, "Expected {0x%05X, 0x%05X} but got {0x%05X, 0x%05X}", expectedX, expectedY, vector.x, vector.y)

#define assertVector3x(vector, expectedX, expectedY, expectedZ) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY && vector.z == expectedZ, "Expected {0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X}", expectedX, expectedY, expectedZ, vector.x, vector.y, vector.z)

#define EPSILON 0.0001f

static void testCartesianToBarycentric2f() {
	Vector3f result;
	
	// Right triangle bottom left
	result = cartesianToBarycentric2f(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 0.0f), VECTOR2f(0.0f, 1.0f));
	assertVector3fExact(result, 1.0f, 0.0f, 0.0f);
	
	// Right triangle right
	result = cartesianToBarycentric2f(VECTOR2f(1.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 0.0f), VECTOR2f(0.0f, 1.0f));
	assertVector3fExact(result, 0.0f, 1.0f, 0.0f);
	
	// Right triangle top
	result = cartesianToBarycentric2f(VECTOR2f(0.0f, 1.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 0.0f), VECTOR2f(0.0f, 1.0f));
	assertVector3fExact(result, 0.0f, 0.0f, 1.0f);
	
	// Right triangle hypotenuse center
	result = cartesianToBarycentric2f(VECTOR2f(0.5f, 0.5f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 0.0f), VECTOR2f(0.0f, 1.0f));
	assertVector3fApproximate(result, 0.0f, 0.5f, 0.5f, EPSILON);
	
	// Right triangle vertical side center
	result = cartesianToBarycentric2f(VECTOR2f(0.0f, 0.5f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 0.0f), VECTOR2f(0.0f, 1.0f));
	assertVector3fApproximate(result, 0.5f, 0.0f, 0.5f, EPSILON);
	
	// Right triangle horizontal side center
	result = cartesianToBarycentric2f(VECTOR2f(0.5f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 0.0f), VECTOR2f(0.0f, 1.0f));
	assertVector3fApproximate(result, 0.5f, 0.5f, 0.0f, EPSILON);
	
	// Equilateral triangle center
	result = cartesianToBarycentric2f(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 1.0f), VECTOR2f(0.8660254037844384, -0.5f), VECTOR2f(-0.8660254037844384, -0.5f));
	assertVector3fApproximate(result, 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, EPSILON);
	
	// Outside right triangle
	result = cartesianToBarycentric2f(VECTOR2f(1.0f, -1.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 0.0f), VECTOR2f(0.0f, 1.0f));
	assertVector3fExact(result, 1.0f, 1.0f, -1.0f);
}

static void testCartesianToBarycentric3f() {
	Vector3f result;
	
	// Right triangle bottom left
	result = cartesianToBarycentric3f(VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fExact(result, 1.0f, 0.0f, 0.0f);
	
	// Right triangle right
	result = cartesianToBarycentric3f(VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fExact(result, 0.0f, 1.0f, 0.0f);
	
	// Right triangle top
	result = cartesianToBarycentric3f(VECTOR3f(0.0f, 1.0f, 0.0f), VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fExact(result, 0.0f, 0.0f, 1.0f);
	
	// Right triangle hypotenuse center
	result = cartesianToBarycentric3f(VECTOR3f(0.5f, 0.5f, 0.0f), VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fApproximate(result, 0.0f, 0.5f, 0.5f, EPSILON);
	
	// Right triangle vertical side center
	result = cartesianToBarycentric3f(VECTOR3f(0.0f, 0.5f, 0.0f), VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fApproximate(result, 0.5f, 0.0f, 0.5f, EPSILON);
	
	// Right triangle horizontal side center
	result = cartesianToBarycentric3f(VECTOR3f(0.5f, 0.0f, 0.0f), VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fApproximate(result, 0.5f, 0.5f, 0.0f, EPSILON);
	
	// Equilateral triangle center
	result = cartesianToBarycentric3f(VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 1.0f, 0.0f), VECTOR3f(0.8660254037844384, -0.5f, 0.0f), VECTOR3f(-0.8660254037844384, -0.5f, 0.0f));
	assertVector3fApproximate(result, 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, EPSILON);
	
	// Outside right triangle
	result = cartesianToBarycentric3f(VECTOR3f(1.0f, -1.0f, 0.0f), VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fExact(result, 1.0f, 1.0f, -1.0f);
	
	// Right triangle hypotenuse center (different orientation)
	result = cartesianToBarycentric3f(VECTOR3f(0.5f, 0.5f, 0.5f), VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 0.0f, 1.0f));
	assertVector3fApproximate(result, 0.0f, 0.5f, 0.5f, EPSILON);
	
	// Right triangle bottom left (different orientation)
	result = cartesianToBarycentric3f(VECTOR3f(-1.0f, 1.0f, 1.0f), VECTOR3f(0.0f, 0.0f, 0.0f), VECTOR3f(1.0f, 1.0f, 0.0f), VECTOR3f(0.0f, 1.0f, -1.0f));
	assertVector3fExact(result, 1.0f, 0.0f, 0.0f);
}

static void testCartesianToBarycentric2x() {
	Vector3x result;
	
	// Right triangle bottom left
	result = cartesianToBarycentric2x(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x00000, 0x10000));
	assertVector3x(result, 0x10000, 0x00000, 0x00000);
	
	// Right triangle right
	result = cartesianToBarycentric2x(VECTOR2x(0x10000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x00000, 0x10000));
	assertVector3x(result, 0x00000, 0x10000, 0x00000);
	
	// Right triangle top
	result = cartesianToBarycentric2x(VECTOR2x(0x00000, 0x10000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x00000, 0x10000));
	assertVector3x(result, 0x00000, 0x00000, 0x10000);
	
	// Right triangle hypotenuse center
	result = cartesianToBarycentric2x(VECTOR2x(0x08000, 0x08000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x00000, 0x10000));
	assertVector3x(result, 0x00000, 0x08000, 0x08000);
	
	// Right triangle vertical side center
	result = cartesianToBarycentric2x(VECTOR2x(0x00000, 0x08000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x00000, 0x10000));
	assertVector3x(result, 0x08000, 0x00000, 0x08000);
	
	// Right triangle horizontal side center
	result = cartesianToBarycentric2x(VECTOR2x(0x08000, 0x00000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x00000, 0x10000));
	assertVector3x(result, 0x08000, 0x08000, 0x00000);
	
	// Equilateral triangle center
	result = cartesianToBarycentric2x(VECTOR2x(0x00000, 0x00000), VECTOR2x(0x00000, 0x10000), VECTOR2x(0x0DDB4, -0x08000), VECTOR2x(-0x0DDB4, -0x08000));
	assertVector3x(result, 0x05556, 0x05555, 0x05555);
	
	// Outside right triangle
	result = cartesianToBarycentric2x(VECTOR2x(0x10000, -0x10000), VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x00000), VECTOR2x(0x00000, 0x10000));
	assertVector3x(result, 0x10000, 0x10000, -0x10000);
}

static void testCartesianToBarycentric3x() {
	Vector3x result;
	
	// Right triangle bottom left
	result = cartesianToBarycentric3x(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000));
	assertVector3x(result, 0x10000, 0x00000, 0x00000);
	
	// Right triangle right
	result = cartesianToBarycentric3x(VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000));
	assertVector3x(result, 0x00000, 0x10000, 0x00000);
	
	// Right triangle top
	result = cartesianToBarycentric3x(VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000));
	assertVector3x(result, 0x00000, 0x00000, 0x10000);
	
	// Right triangle hypotenuse center
	result = cartesianToBarycentric3x(VECTOR3x(0x08000, 0x08000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000));
	assertVector3x(result, 0x00000, 0x08000, 0x08000);
	
	// Right triangle vertical side center
	result = cartesianToBarycentric3x(VECTOR3x(0x00000, 0x08000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000));
	assertVector3x(result, 0x08000, 0x00000, 0x08000);
	
	// Right triangle horizontal side center
	result = cartesianToBarycentric3x(VECTOR3x(0x08000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000));
	assertVector3x(result, 0x08000, 0x08000, 0x00000);
	
	// Equilateral triangle center
	result = cartesianToBarycentric3x(VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x0DDB4, -0x08000, 0x00000), VECTOR3x(-0x0DDB4, -0x08000, 0x00000));
	assertVector3x(result, 0x05556, 0x05555, 0x05555);
	
	// Outside right triangle
	result = cartesianToBarycentric3x(VECTOR3x(0x10000, -0x10000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x10000, 0x00000));
	assertVector3x(result, 0x10000, 0x10000, -0x10000);
	
	// Right triangle hypotenuse center (different orientation)
	result = cartesianToBarycentric3x(VECTOR3x(0x08000, 0x08000, 0x08000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x10000));
	assertVector3x(result, 0x00000, 0x08000, 0x08000);
	
	// Right triangle bottom left (different orientation)
	result = cartesianToBarycentric3x(VECTOR3x(-0x10000, 0x10000, 0x10000), VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x00000), VECTOR3x(0x00000, 0x10000, -0x10000));
	assertVector3x(result, 0x10000, 0x00000, 0x00000);
}

TEST_SUITE(BarycentricTest,
           testCartesianToBarycentric2f,
           testCartesianToBarycentric3f,
           testCartesianToBarycentric2x,
           testCartesianToBarycentric3x)
