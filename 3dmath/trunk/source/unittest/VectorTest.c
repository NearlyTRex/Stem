#include "unittest/TestSuite.h"
#include "3dmath/Vector2f.h"
#include "3dmath/Vector3f.h"
#include "3dmath/Vector4f.h"
#include <math.h>
#include <string.h>

#define assertVector2fExact(vector, expectedX, expectedY) { \
	TestCase_assert(vector.x == expectedX, "Expected %f x but got %f", expectedX, vector.x); \
	TestCase_assert(vector.y == expectedY, "Expected %f y but got %f", expectedY, vector.y); \
}

#define assertVector3fExact(vector, expectedX, expectedY, expectedZ) { \
	TestCase_assert(vector.x == expectedX, "Expected %f x but got %f", expectedX, vector.x); \
	TestCase_assert(vector.y == expectedY, "Expected %f y but got %f", expectedY, vector.y); \
	TestCase_assert(vector.z == expectedZ, "Expected %f z but got %f", expectedZ, vector.z); \
}

#define assertVector4fExact(vector, expectedX, expectedY, expectedZ, expectedW) { \
	TestCase_assert(vector.x == expectedX, "Expected %f x but got %f", expectedX, vector.x); \
	TestCase_assert(vector.y == expectedY, "Expected %f y but got %f", expectedY, vector.y); \
	TestCase_assert(vector.z == expectedZ, "Expected %f z but got %f", expectedZ, vector.z); \
	TestCase_assert(vector.w == expectedW, "Expected %f w but got %f", expectedW, vector.w); \
}

#define assertVector2fApproximate(vector, expectedX, expectedY, epsilon) { \
	TestCase_assert(fabs(vector.x - expectedX) < epsilon, "Expected %f x but got %f (tolerance %f)", expectedX, vector.x, epsilon); \
	TestCase_assert(fabs(vector.y - expectedY) < epsilon, "Expected %f y but got %f (tolerance %f)", expectedY, vector.y, epsilon); \
}

#define assertVector3fApproximate(vector, expectedX, expectedY, expectedZ, epsilon) { \
	TestCase_assert(fabs(vector.x - expectedX) < epsilon, "Expected %f x but got %f (tolerance %f)", expectedX, vector.x, epsilon); \
	TestCase_assert(fabs(vector.y - expectedY) < epsilon, "Expected %f y but got %f (tolerance %f)", expectedY, vector.y, epsilon); \
	TestCase_assert(fabs(vector.z - expectedZ) < epsilon, "Expected %f z but got %f (tolerance %f)", expectedZ, vector.z, epsilon); \
}

#define assertVector4fApproximate(vector, expectedX, expectedY, expectedZ, expectedW, epsilon) { \
	TestCase_assert(fabs(vector.x - expectedX) < epsilon, "Expected %f x but got %f (tolerance %f)", expectedX, vector.x, epsilon); \
	TestCase_assert(fabs(vector.y - expectedY) < epsilon, "Expected %f y but got %f (tolerance %f)", expectedY, vector.y, epsilon); \
	TestCase_assert(fabs(vector.z - expectedZ) < epsilon, "Expected %f z but got %f (tolerance %f)", expectedZ, vector.z, epsilon); \
	TestCase_assert(fabs(vector.w - expectedW) < epsilon, "Expected %f w but got %f (tolerance %f)", expectedW, vector.w, epsilon); \
}

#define EPSILON 0.0001f

static void testInit() {
	Vector2f vector2;
	Vector3f vector3;
	Vector4f vector4;
	
	vector2 = VECTOR2f(0.0f, 1.0f);
	assertVector2fExact(vector2, 0.0f, 1.0f);
	vector2 = VECTOR2f(3.0f, 2.0f);
	assertVector2fExact(vector2, 3.0f, 2.0f);
	
	vector3 = VECTOR3f(0.0f, 1.0f, 2.0f);
	assertVector3fExact(vector3, 0.0f, 1.0f, 2.0f);
	vector3 = VECTOR3f(3.0f, 2.0f, 1.0f);
	assertVector3fExact(vector3, 3.0f, 2.0f, 1.0f);
	
	vector4 = VECTOR4f(0.0f, 1.0f, 2.0f, 3.0f);
	assertVector4fExact(vector4, 0.0f, 1.0f, 2.0f, 3.0f);
	vector4 = VECTOR4f(3.0f, 2.0f, 1.0f, 0.0f);
	assertVector4fExact(vector4, 3.0f, 2.0f, 1.0f, 0.0f);
}

static void testPrefabs() {
	Vector2f vector2;
	Vector3f vector3;
	Vector4f vector4;
	
	vector2 = VECTOR2f_ZERO;
	assertVector2fExact(vector2, 0.0f, 0.0f);
	vector3 = VECTOR3f_ZERO;
	assertVector3fExact(vector3, 0.0f, 0.0f, 0.0f);
	vector4 = VECTOR4f_ZERO;
	assertVector4fExact(vector4, 0.0f, 0.0f, 0.0f, 0.0f);
	
	vector3 = VECTOR3f_LEFT;
	assertVector3fExact(vector3, -1.0f, 0.0f, 0.0f);
	vector3 = VECTOR3f_RIGHT;
	assertVector3fExact(vector3, 1.0f, 0.0f, 0.0f);
	
	vector3 = VECTOR3f_DOWN;
	assertVector3fExact(vector3, 0.0f, -1.0f, 0.0f);
	vector3 = VECTOR3f_UP;
	assertVector3fExact(vector3, 0.0f, 1.0f, 0.0f);
	
	vector3 = VECTOR3f_BACK;
	assertVector3fExact(vector3, 0.0f, 0.0f, -1.0f);
	vector3 = VECTOR3f_FRONT;
	assertVector3fExact(vector3, 0.0f, 0.0f, 1.0f);
}

static void testNormalize() {
	Vector2f vector2, vector2Normalized;
	Vector3f vector3, vector3Normalized;
	Vector4f vector4, vector4Normalized;
	bool success;
	
	vector2 = VECTOR2f(5.0f, 0.0f);
	vector2Normalized = Vector2f_normalized(vector2);
	success = Vector2f_normalize(&vector2);
	TestCase_assert(!memcmp(&vector2, &vector2Normalized, sizeof(Vector2f)), "Vector2f_normalize produced different results (%f, %f) than Vector2f_normalized (%f, %f)", vector2.x, vector2.y, vector2Normalized.x, vector2Normalized.y);
	TestCase_assert(success, "Expected true but got false");
	assertVector2fApproximate(vector2, 1.0f, 0.0f, EPSILON);
	
	vector2 = VECTOR2f(1.0f, 1.0f);
	vector2Normalized = Vector2f_normalized(vector2);
	success = Vector2f_normalize(&vector2);
	TestCase_assert(!memcmp(&vector2, &vector2Normalized, sizeof(Vector2f)), "Vector2f_normalize produced different results (%f, %f) than Vector2f_normalized (%f, %f)", vector2.x, vector2.y, vector2Normalized.x, vector2Normalized.y);
	TestCase_assert(success, "Expected true but got false");
	assertVector2fApproximate(vector2, 0.707106781186548f, 0.707106781186548f, EPSILON);
	
	vector2 = VECTOR2f(0.0f, 0.0f);
	vector2Normalized = Vector2f_normalized(vector2);
	success = Vector2f_normalize(&vector2);
	TestCase_assert(!memcmp(&vector2, &vector2Normalized, sizeof(Vector2f)), "Vector2f_normalize produced different results (%f, %f) than Vector2f_normalized (%f, %f)", vector2.x, vector2.y, vector2Normalized.x, vector2Normalized.y);
	TestCase_assert(!success, "Expected false but got true");
	assertVector2fExact(vector2, 0.0f, 0.0f);
	
	vector3 = VECTOR3f(0.0f, 5.0f, 0.0f);
	vector3Normalized = Vector3f_normalized(vector3);
	success = Vector3f_normalize(&vector3);
	TestCase_assert(!memcmp(&vector3, &vector3Normalized, sizeof(Vector3f)), "Vector3f_normalize produced different results (%f, %f, %f) than Vector3f_normalized (%f, %f, %f)", vector3.x, vector3.y, vector3.z, vector3Normalized.x, vector3Normalized.y, vector3Normalized.z);
	TestCase_assert(success, "Expected true but got false");
	assertVector3fApproximate(vector3, 0.0f, 1.0f, 0.0f, EPSILON);
	
	vector3 = VECTOR3f(1.0f, 1.0f, 1.0f);
	vector3Normalized = Vector3f_normalized(vector3);
	success = Vector3f_normalize(&vector3);
	TestCase_assert(!memcmp(&vector3, &vector3Normalized, sizeof(Vector3f)), "Vector3f_normalize produced different results (%f, %f, %f) than Vector3f_normalized (%f, %f, %f)", vector3.x, vector3.y, vector3.z, vector3Normalized.x, vector3Normalized.y, vector3Normalized.z);
	TestCase_assert(success, "Expected true but got false");
	assertVector3fApproximate(vector3, 0.577350269189626f, 0.577350269189626f, 0.577350269189626f, EPSILON);
	
	vector3 = VECTOR3f(0.0f, 0.0f, 0.0f);
	vector3Normalized = Vector3f_normalized(vector3);
	success = Vector3f_normalize(&vector3);
	TestCase_assert(!memcmp(&vector3, &vector3Normalized, sizeof(Vector3f)), "Vector3f_normalize produced different results (%f, %f, %f) than Vector3f_normalized (%f, %f, %f)", vector3.x, vector3.y, vector3.z, vector3Normalized.x, vector3Normalized.y, vector3Normalized.z);
	TestCase_assert(!success, "Expected false but got true");
	assertVector3fExact(vector3, 0.0f, 0.0f, 0.0f);
	
	vector4 = VECTOR4f(0.0f, 0.0f, 5.0f, 0.0f);
	vector4Normalized = Vector4f_normalized(vector4);
	success = Vector4f_normalize(&vector4);
	TestCase_assert(!memcmp(&vector4, &vector4Normalized, sizeof(Vector4f)), "Vector4f_normalize produced different results (%f, %f, %f, %f) than Vector4f_normalized (%f, %f, %f, %f)", vector4.x, vector4.y, vector4.z, vector4.w, vector4Normalized.x, vector4Normalized.y, vector4Normalized.z, vector4Normalized.w);
	TestCase_assert(success, "Expected true but got false");
	assertVector4fApproximate(vector4, 0.0f, 0.0f, 1.0f, 0.0f, EPSILON);
	
	vector4 = VECTOR4f(1.0f, 1.0f, 1.0f, 1.0f);
	vector4Normalized = Vector4f_normalized(vector4);
	success = Vector4f_normalize(&vector4);
	TestCase_assert(!memcmp(&vector4, &vector4Normalized, sizeof(Vector4f)), "Vector4f_normalize produced different results (%f, %f, %f, %f) than Vector4f_normalized (%f, %f, %f, %f)", vector4.x, vector4.y, vector4.z, vector4.w, vector4Normalized.x, vector4Normalized.y, vector4Normalized.z, vector4Normalized.w);
	TestCase_assert(success, "Expected true but got false");
	assertVector4fApproximate(vector4, 0.5f, 0.5f, 0.5f, 0.5f, EPSILON);
	
	vector4 = VECTOR4f(0.0f, 0.0f, 0.0f, 0.0f);
	vector4Normalized = Vector4f_normalized(vector4);
	success = Vector4f_normalize(&vector4);
	TestCase_assert(!memcmp(&vector4, &vector4Normalized, sizeof(Vector4f)), "Vector4f_normalize produced different results (%f, %f, %f, %f) than Vector4f_normalized (%f, %f, %f, %f)", vector4.x, vector4.y, vector4.z, vector4.w, vector4Normalized.x, vector4Normalized.y, vector4Normalized.z, vector4Normalized.w);
	TestCase_assert(!success, "Expected false but got true");
	assertVector4fExact(vector4, 0.0f, 0.0f, 0.0f, 0.0f);
}

static void testMagnitude() {
	float magnitude;
	
	magnitude = Vector2f_magnitude(VECTOR2f(1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector2f_magnitude(VECTOR2f(1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 2.0f) < EPSILON, "Expected 2.0 but got %f", magnitude);
	
	magnitude = Vector3f_magnitude(VECTOR3f(0.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector3f_magnitude(VECTOR3f(1.4142135623731f, 1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 2.44948974278318f) < EPSILON, "Expected 2.44948974278318 but got %f", magnitude);
	
	magnitude = Vector4f_magnitude(VECTOR4f(0.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector4f_magnitude(VECTOR4f(1.4142135623731f, 1.4142135623731f, 1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 2.82842712474619f) < EPSILON, "Expected 2.82842712474619 but got %f", magnitude);
	
	magnitude = Vector2f_magnitudeSquared(VECTOR2f(1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector2f_magnitudeSquared(VECTOR2f(1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 4.0f) < EPSILON, "Expected 4.0 but got %f", magnitude);
	
	magnitude = Vector3f_magnitudeSquared(VECTOR3f(0.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector3f_magnitudeSquared(VECTOR3f(1.4142135623731f, 1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 6.0f) < EPSILON, "Expected 6.0 but got %f", magnitude);
	
	magnitude = Vector4f_magnitudeSquared(VECTOR4f(0.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector4f_magnitudeSquared(VECTOR4f(1.4142135623731f, 1.4142135623731f, 1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 8.0f) < EPSILON, "Expected 8.0 but got %f", magnitude);
}

static void testArithmetic() {
	Vector2f vector2;
	Vector3f vector3;
	Vector4f vector4;
	
	vector2 = Vector2f_add(VECTOR2f(0.0f, 1.0f), VECTOR2f(2.0f, 3.0f));
	assertVector2fApproximate(vector2, 2.0f, 4.0f, EPSILON);
	vector2 = Vector2f_add(VECTOR2f(1.5f, 0.5f), VECTOR2f(0.25f, -0.25f));
	assertVector2fApproximate(vector2, 1.75f, 0.25f, EPSILON);
	
	vector2 = Vector2f_subtract(VECTOR2f(0.0f, 1.0f), VECTOR2f(2.0f, 3.0f));
	assertVector2fApproximate(vector2, -2.0f, -2.0f, EPSILON);
	vector2 = Vector2f_subtract(VECTOR2f(1.5f, 0.5f), VECTOR2f(0.25f, -0.25f));
	assertVector2fApproximate(vector2, 1.25f, 0.75f, EPSILON);
	
	vector2 = Vector2f_multiplyScalar(VECTOR2f(1.0f, 2.0f), 0.5f);
	assertVector2fApproximate(vector2, 0.5f, 1.0f, EPSILON);
	vector2 = Vector2f_multiplyScalar(VECTOR2f(3.0f, 4.0f), -2.0f);
	assertVector2fApproximate(vector2, -6.0f, -8.0f, EPSILON);
	
	vector2 = Vector2f_divideScalar(VECTOR2f(1.0f, 2.0f), 0.5f);
	assertVector2fApproximate(vector2, 2.0f, 4.0f, EPSILON);
	vector2 = Vector2f_divideScalar(VECTOR2f(3.0f, 4.0f), -2.0f);
	assertVector2fApproximate(vector2, -1.5f, -2.0f, EPSILON);
	
	vector3 = Vector3f_add(VECTOR3f(0.0f, 1.0f, 2.0f), VECTOR3f(2.0f, 3.0f, 4.0f));
	assertVector3fApproximate(vector3, 2.0f, 4.0f, 6.0f, EPSILON);
	vector3 = Vector3f_add(VECTOR3f(1.5f, 0.5f, -0.5f), VECTOR3f(0.25f, -0.25f, 0.0f));
	assertVector3fApproximate(vector3, 1.75f, 0.25f, -0.5f, EPSILON);
	
	vector3 = Vector3f_subtract(VECTOR3f(0.0f, 1.0f, 2.0f), VECTOR3f(2.0f, 3.0f, 4.0f));
	assertVector3fApproximate(vector3, -2.0f, -2.0f, -2.0, EPSILON);
	vector3 = Vector3f_subtract(VECTOR3f(1.5f, 0.5f, -0.5f), VECTOR3f(0.25f, -0.25f, 0.0f));
	assertVector3fApproximate(vector3, 1.25f, 0.75f, -0.5f, EPSILON);
	
	vector3 = Vector3f_multiplyScalar(VECTOR3f(1.0f, 2.0f, 3.0f), 0.5f);
	assertVector3fApproximate(vector3, 0.5f, 1.0f, 1.5f, EPSILON);
	vector3 = Vector3f_multiplyScalar(VECTOR3f(3.0f, 4.0f, 5.0f), -2.0f);
	assertVector3fApproximate(vector3, -6.0f, -8.0f, -10.0f, EPSILON);
	
	vector3 = Vector3f_divideScalar(VECTOR3f(1.0f, 2.0f, 3.0f), 0.5f);
	assertVector3fApproximate(vector3, 2.0f, 4.0f, 6.0f, EPSILON);
	vector3 = Vector3f_divideScalar(VECTOR3f(3.0f, 4.0f, 5.0f), -2.0f);
	assertVector3fApproximate(vector3, -1.5f, -2.0f, -2.5f, EPSILON);
	
	vector4 = Vector4f_add(VECTOR4f(0.0f, 1.0f, 2.0f, 3.0f), VECTOR4f(2.0f, 3.0f, 4.0f, 5.0f));
	assertVector4fApproximate(vector4, 2.0f, 4.0f, 6.0f, 8.0f, EPSILON);
	vector4 = Vector4f_add(VECTOR4f(1.5f, 0.5f, -0.5f, -1.5f), VECTOR4f(0.25f, -0.25f, 0.0f, 0.1f));
	assertVector4fApproximate(vector4, 1.75f, 0.25f, -0.5f, -1.4f, EPSILON);
	
	vector4 = Vector4f_subtract(VECTOR4f(0.0f, 1.0f, 2.0f, 3.0f), VECTOR4f(2.0f, 3.0f, 4.0f, 5.0f));
	assertVector4fApproximate(vector4, -2.0f, -2.0f, -2.0, -2.0f, EPSILON);
	vector4 = Vector4f_subtract(VECTOR4f(1.5f, 0.5f, -0.5f, -1.5f), VECTOR4f(0.25f, -0.25f, 0.0f, 0.1f));
	assertVector4fApproximate(vector4, 1.25f, 0.75f, -0.5f, -1.6f, EPSILON);
	
	vector4 = Vector4f_multiplyScalar(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f), 0.5f);
	assertVector4fApproximate(vector4, 0.5f, 1.0f, 1.5f, 2.0f, EPSILON);
	vector4 = Vector4f_multiplyScalar(VECTOR4f(3.0f, 4.0f, 5.0f, 6.0f), -2.0f);
	assertVector4fApproximate(vector4, -6.0f, -8.0f, -10.0f, -12.0f, EPSILON);
	
	vector4 = Vector4f_divideScalar(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f), 0.5f);
	assertVector4fApproximate(vector4, 2.0f, 4.0f, 6.0f, 8.0f, EPSILON);
	vector4 = Vector4f_divideScalar(VECTOR4f(3.0f, 4.0f, 5.0f, 6.0f), -2.0f);
	assertVector4fApproximate(vector4, -1.5f, -2.0f, -2.5f, -3.0f, EPSILON);
}

static void testInterpolate() {
	Vector2f vector2;
	Vector3f vector3;
	Vector4f vector4;
	
	vector2 = Vector2f_interpolate(VECTOR2f(0.0f, 1.0f), VECTOR2f(1.0f, 0.0f), 0.25f);
	assertVector2fApproximate(vector2, 0.25f, 0.75f, EPSILON);
	vector2 = Vector2f_interpolate(VECTOR2f(3.0f, -3.0f), VECTOR2f(0.0f, 0.0f), 1.5f);
	assertVector2fApproximate(vector2, -1.5f, 1.5f, EPSILON);
	
	vector3 = Vector3f_interpolate(VECTOR3f(0.0f, 1.0f, 2.0f), VECTOR3f(1.0f, 0.0f, -2.0f), 0.25f);
	assertVector3fApproximate(vector3, 0.25f, 0.75f, 1.0f, EPSILON);
	vector3 = Vector3f_interpolate(VECTOR3f(3.0f, -3.0f, 1.0f), VECTOR3f(0.0f, 0.0f, 1.5f), -0.5f);
	assertVector3fApproximate(vector3, 4.5f, -4.5f, 0.75f, EPSILON);
	
	vector4 = Vector4f_interpolate(VECTOR4f(0.0f, 1.0f, 2.0f, 3.0f), VECTOR4f(1.0f, 0.0f, -2.0f, -3.0f), 0.5f);
	assertVector4fApproximate(vector4, 0.5f, 0.5f, 0.0f, 0.0f, EPSILON);
	vector4 = Vector4f_interpolate(VECTOR4f(4.0f, -3.0f, 1.0f, -2.0f), VECTOR4f(0.0f, -1.0f, 1.5f, -1.0), 0.75f);
	assertVector4fApproximate(vector4, 1.0f, -1.5f, 1.375f, -1.25f, EPSILON);
}

static void testReflect() {
	Vector2f vector2;
	Vector3f vector3;
	Vector4f vector4;
	
	vector2 = Vector2f_reflect(VECTOR2f(0.0f, 1.0f), VECTOR2f(sqrt(0.5f), sqrt(0.5f)));
	assertVector2fApproximate(vector2, -1.0f, 0.0f, EPSILON);
	vector2 = Vector2f_reflect(VECTOR2f(-5.0f, 1.0f), VECTOR2f(-1.0f, 0.0f));
	assertVector2fApproximate(vector2, 5.0f, 1.0f, EPSILON);
	
	vector3 = Vector3f_reflect(VECTOR3f(0.0f, 1.0f, 0.0f), VECTOR3f(0.0f, sqrt(0.5f), sqrt(0.5f)));
	assertVector3fApproximate(vector3, 0.0f, 0.0f, -1.0f, EPSILON);
	vector3 = Vector3f_reflect(VECTOR3f(-5.0f, 1.0f, 0.0f), VECTOR3f(-1.0f, 0.0f, 0.0f));
	assertVector3fApproximate(vector3, 5.0f, 1.0f, 0.0f, EPSILON);
	
	vector4 = Vector4f_reflect(VECTOR4f(0.0f, 1.0f, 0.0f, 0.0f), VECTOR4f(0.0f, 0.0f, sqrt(0.5f), sqrt(0.5f)));
	assertVector4fApproximate(vector4, 0.0f, 1.0f, 0.0f, 0.0f, EPSILON);
	vector4 = Vector4f_reflect(VECTOR4f(-5.0f, 1.0f, 0.0f, 0.0f), VECTOR4f(-1.0f, 0.0f, 0.0f, 0.0f));
	assertVector4fApproximate(vector4, 5.0f, 1.0f, 0.0f, 0.0f, EPSILON);
}

static void testProject() {
	Vector2f vector2;
	Vector3f vector3;
	Vector4f vector4;
	
	vector2 = Vector2f_project(VECTOR2f(0.0f, 1.0f), VECTOR2f(sqrt(0.5f), sqrt(0.5f)));
	assertVector2fApproximate(vector2, -0.5f, 0.5f, EPSILON);
	vector2 = Vector2f_project(VECTOR2f(-1.0f, -2.0f), VECTOR2f(sqrt(0.5f), sqrt(0.5f)));
	assertVector2fApproximate(vector2, 0.5f, -0.5f, EPSILON);
	vector2 = Vector2f_project(VECTOR2f(-1.0f, -2.0f), VECTOR2f(0.0f, 1.0f));
	assertVector2fApproximate(vector2, -1.0f, 0.0f, EPSILON);
	
	vector3 = Vector3f_project(VECTOR3f(0.0f, 0.0f, 1.0f), VECTOR3f(0.0f, sqrt(0.5f), sqrt(0.5f)));
	assertVector3fApproximate(vector3, 0.0f, -0.5f, 0.5f, EPSILON);
	vector3 = Vector3f_project(VECTOR3f(-1.0f, -2.0f, 0.0f), VECTOR3f(sqrt(0.5f), sqrt(0.5f), 0.0f));
	assertVector3fApproximate(vector3, 0.5f, -0.5f, 0.0f, EPSILON);
	vector3 = Vector3f_project(VECTOR3f(-1.0f, -2.0f, 1.0f), VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fApproximate(vector3, -1.0f, 0.0f, 1.0f, EPSILON);
	
	vector4 = Vector4f_project(VECTOR4f(0.0f, 0.0f, 0.0f, 1.0f), VECTOR4f(0.0f, 0.0f, sqrt(0.5f), sqrt(0.5f)));
	assertVector4fApproximate(vector4, 0.0f, 0.0f, -0.5f, 0.5f, EPSILON);
	vector4 = Vector4f_project(VECTOR4f(-1.0f, 0.0f, -2.0f, 0.0f), VECTOR4f(sqrt(0.5f), 0.0f, sqrt(0.5f), 0.0f));
	assertVector4fApproximate(vector4, 0.5f, 0.0f, -0.5f, 0.0f, EPSILON);
	vector4 = Vector4f_project(VECTOR4f(-1.0f, -2.0f, 1.0f, 4.0f), VECTOR4f(0.0f, 1.0f, 0.0f, 0.0f));
	assertVector4fApproximate(vector4, -1.0f, 0.0f, 1.0f, 4.0f, EPSILON);
}

static void testDot() {
	float dot;
	
	dot = Vector2f_dot(VECTOR2f(1.0f, 0.0f), VECTOR2f(0.5f, 1.0f));
	TestCase_assert(fabs(dot - 0.5f) < EPSILON, "Expected 0.5 but got %f", dot);
	dot = Vector2f_dot(VECTOR2f(0.0f, 2.0f), VECTOR2f(0.0f, -1.0f));
	TestCase_assert(fabs(dot - -2.0f) < EPSILON, "Expected -2.0 but got %f", dot);
	
	dot = Vector3f_dot(VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.5f, 1.0f, 10.0f));
	TestCase_assert(fabs(dot - 0.5f) < EPSILON, "Expected 0.5 but got %f", dot);
	dot = Vector3f_dot(VECTOR3f(0.0f, 2.0f, 4.0f), VECTOR3f(0.0f, -1.0f, 0.5f));
	TestCase_assert(fabs(dot - 0.0f) < EPSILON, "Expected 1.0 but got %f", dot);
	
	dot = Vector4f_dot(VECTOR4f(1.0f, 0.0f, 0.0f, 0.0f), VECTOR4f(0.5f, 1.0f, 10.0f, -2.0f));
	TestCase_assert(fabs(dot - 0.5f) < EPSILON, "Expected 0.5 but got %f", dot);
	dot = Vector4f_dot(VECTOR4f(0.0f, 2.0f, 4.0f, 0.5f), VECTOR4f(0.0f, -1.0f, 0.5f, 2.0f));
	TestCase_assert(fabs(dot - 1.0f) < EPSILON, "Expected 1.0 but got %f", dot);
}

static void testCross() {
	float cross2;
	Vector3f cross3;
	
	cross2 = Vector2f_cross(VECTOR2f(1.0f, 0.0f), VECTOR2f(0.0f, 2.0f));
	TestCase_assert(fabs(cross2 - 2.0f) < EPSILON, "Expected 2.0 but got %f", cross2);
	cross2 = Vector2f_cross(VECTOR2f(-1.0f, 0.0f), VECTOR2f(0.0f, 1.0f));
	TestCase_assert(fabs(cross2 - -1.0f) < EPSILON, "Expected -1.0 but got %f", cross2);
	
	cross3 = Vector3f_cross(VECTOR3f(1.0f, 0.0f, 0.0f), VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fApproximate(cross3, 0.0f, 0.0f, 1.0f, EPSILON);
	cross3 = Vector3f_cross(VECTOR3f(0.0f, 0.0f, -1.0f), VECTOR3f(0.0f, -2.0f, 0.0f));
	assertVector3fApproximate(cross3, -2.0f, 0.0f, 0.0f, EPSILON);
}

TEST_SUITE(VectorTest,
           testInit,
           testPrefabs,
           testNormalize,
           testMagnitude,
           testArithmetic,
           testInterpolate,
           testReflect,
           testProject,
           testDot,
           testCross)
