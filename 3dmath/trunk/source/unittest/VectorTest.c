#include "unittest/TestSuite.h"
#include "3dmath/Vector.h"
#include <math.h>
#include <string.h>

#define assertVector2Exact(vector, expectedX, expectedY) { \
	TestCase_assert(vector.x == expectedX, "Expected %f x but got %f", expectedX, vector.x); \
	TestCase_assert(vector.y == expectedY, "Expected %f y but got %f", expectedY, vector.y); \
}

#define assertVector3Exact(vector, expectedX, expectedY, expectedZ) { \
	TestCase_assert(vector.x == expectedX, "Expected %f x but got %f", expectedX, vector.x); \
	TestCase_assert(vector.y == expectedY, "Expected %f y but got %f", expectedY, vector.y); \
	TestCase_assert(vector.z == expectedZ, "Expected %f z but got %f", expectedZ, vector.z); \
}

#define assertVector4Exact(vector, expectedX, expectedY, expectedZ, expectedW) { \
	TestCase_assert(vector.x == expectedX, "Expected %f x but got %f", expectedX, vector.x); \
	TestCase_assert(vector.y == expectedY, "Expected %f y but got %f", expectedY, vector.y); \
	TestCase_assert(vector.z == expectedZ, "Expected %f z but got %f", expectedZ, vector.z); \
	TestCase_assert(vector.w == expectedW, "Expected %f w but got %f", expectedW, vector.w); \
}

#define assertVector2Approximate(vector, expectedX, expectedY, epsilon) { \
	TestCase_assert(fabs(vector.x - expectedX) < epsilon, "Expected %f x but got %f (tolerance %f)", expectedX, vector.x, epsilon); \
	TestCase_assert(fabs(vector.y - expectedY) < epsilon, "Expected %f y but got %f (tolerance %f)", expectedY, vector.y, epsilon); \
}

#define assertVector3Approximate(vector, expectedX, expectedY, expectedZ, epsilon) { \
	TestCase_assert(fabs(vector.x - expectedX) < epsilon, "Expected %f x but got %f (tolerance %f)", expectedX, vector.x, epsilon); \
	TestCase_assert(fabs(vector.y - expectedY) < epsilon, "Expected %f y but got %f (tolerance %f)", expectedY, vector.y, epsilon); \
	TestCase_assert(fabs(vector.z - expectedZ) < epsilon, "Expected %f z but got %f (tolerance %f)", expectedZ, vector.z, epsilon); \
}

#define assertVector4Approximate(vector, expectedX, expectedY, expectedZ, expectedW, epsilon) { \
	TestCase_assert(fabs(vector.x - expectedX) < epsilon, "Expected %f x but got %f (tolerance %f)", expectedX, vector.x, epsilon); \
	TestCase_assert(fabs(vector.y - expectedY) < epsilon, "Expected %f y but got %f (tolerance %f)", expectedY, vector.y, epsilon); \
	TestCase_assert(fabs(vector.z - expectedZ) < epsilon, "Expected %f z but got %f (tolerance %f)", expectedZ, vector.z, epsilon); \
	TestCase_assert(fabs(vector.w - expectedW) < epsilon, "Expected %f w but got %f (tolerance %f)", expectedW, vector.w, epsilon); \
}

#define EPSILON 0.0001f

static void testInit() {
	Vector2 vector2;
	Vector3 vector3;
	Vector4 vector4;
	
	vector2 = Vector2_init(0.0f, 1.0f);
	assertVector2Exact(vector2, 0.0f, 1.0f);
	vector2 = Vector2_init(3.0f, 2.0f);
	assertVector2Exact(vector2, 3.0f, 2.0f);
	
	vector3 = Vector3_init(0.0f, 1.0f, 2.0f);
	assertVector3Exact(vector3, 0.0f, 1.0f, 2.0f);
	vector3 = Vector3_init(3.0f, 2.0f, 1.0f);
	assertVector3Exact(vector3, 3.0f, 2.0f, 1.0f);
	
	vector4 = Vector4_init(0.0f, 1.0f, 2.0f, 3.0f);
	assertVector4Exact(vector4, 0.0f, 1.0f, 2.0f, 3.0f);
	vector4 = Vector4_init(3.0f, 2.0f, 1.0f, 0.0f);
	assertVector4Exact(vector4, 3.0f, 2.0f, 1.0f, 0.0f);
}

static void testPrefabs() {
	Vector2 vector2;
	Vector3 vector3;
	Vector4 vector4;
	
	vector2 = VECTOR2_ZERO;
	assertVector2Exact(vector2, 0.0f, 0.0f);
	vector3 = VECTOR3_ZERO;
	assertVector3Exact(vector3, 0.0f, 0.0f, 0.0f);
	vector4 = VECTOR4_ZERO;
	assertVector4Exact(vector4, 0.0f, 0.0f, 0.0f, 0.0f);
	
	vector3 = VECTOR3_LEFT;
	assertVector3Exact(vector3, -1.0f, 0.0f, 0.0f);
	vector3 = VECTOR3_RIGHT;
	assertVector3Exact(vector3, 1.0f, 0.0f, 0.0f);
	
	vector3 = VECTOR3_DOWN;
	assertVector3Exact(vector3, 0.0f, -1.0f, 0.0f);
	vector3 = VECTOR3_UP;
	assertVector3Exact(vector3, 0.0f, 1.0f, 0.0f);
	
	vector3 = VECTOR3_BACK;
	assertVector3Exact(vector3, 0.0f, 0.0f, -1.0f);
	vector3 = VECTOR3_FRONT;
	assertVector3Exact(vector3, 0.0f, 0.0f, 1.0f);
}

static void testNormalize() {
	Vector2 vector2, vector2Normalized;
	Vector3 vector3, vector3Normalized;
	Vector4 vector4, vector4Normalized;
	bool success;
	
	vector2 = Vector2_init(5.0f, 0.0f);
	vector2Normalized = Vector2_normalized(vector2);
	success = Vector2_normalize(&vector2);
	TestCase_assert(!memcmp(&vector2, &vector2Normalized, sizeof(Vector2)), "Vector2_normalize produced different results (%f, %f) than Vector2_normalized (%f, %f)", vector2.x, vector2.y, vector2Normalized.x, vector2Normalized.y);
	TestCase_assert(success, "Expected true but got false");
	assertVector2Approximate(vector2, 1.0f, 0.0f, EPSILON);
	
	vector2 = Vector2_init(1.0f, 1.0f);
	vector2Normalized = Vector2_normalized(vector2);
	success = Vector2_normalize(&vector2);
	TestCase_assert(!memcmp(&vector2, &vector2Normalized, sizeof(Vector2)), "Vector2_normalize produced different results (%f, %f) than Vector2_normalized (%f, %f)", vector2.x, vector2.y, vector2Normalized.x, vector2Normalized.y);
	TestCase_assert(success, "Expected true but got false");
	assertVector2Approximate(vector2, 0.707106781186548f, 0.707106781186548f, EPSILON);
	
	vector2 = Vector2_init(0.0f, 0.0f);
	vector2Normalized = Vector2_normalized(vector2);
	success = Vector2_normalize(&vector2);
	TestCase_assert(!memcmp(&vector2, &vector2Normalized, sizeof(Vector2)), "Vector2_normalize produced different results (%f, %f) than Vector2_normalized (%f, %f)", vector2.x, vector2.y, vector2Normalized.x, vector2Normalized.y);
	TestCase_assert(!success, "Expected false but got true");
	assertVector2Exact(vector2, 0.0f, 0.0f);
	
	vector3 = Vector3_init(0.0f, 5.0f, 0.0f);
	vector3Normalized = Vector3_normalized(vector3);
	success = Vector3_normalize(&vector3);
	TestCase_assert(!memcmp(&vector3, &vector3Normalized, sizeof(Vector3)), "Vector3_normalize produced different results (%f, %f, %f) than Vector3_normalized (%f, %f, %f)", vector3.x, vector3.y, vector3.z, vector3Normalized.x, vector3Normalized.y, vector3Normalized.z);
	TestCase_assert(success, "Expected true but got false");
	assertVector3Approximate(vector3, 0.0f, 1.0f, 0.0f, EPSILON);
	
	vector3 = Vector3_init(1.0f, 1.0f, 1.0f);
	vector3Normalized = Vector3_normalized(vector3);
	success = Vector3_normalize(&vector3);
	TestCase_assert(!memcmp(&vector3, &vector3Normalized, sizeof(Vector3)), "Vector3_normalize produced different results (%f, %f, %f) than Vector3_normalized (%f, %f, %f)", vector3.x, vector3.y, vector3.z, vector3Normalized.x, vector3Normalized.y, vector3Normalized.z);
	TestCase_assert(success, "Expected true but got false");
	assertVector3Approximate(vector3, 0.577350269189626f, 0.577350269189626f, 0.577350269189626f, EPSILON);
	
	vector3 = Vector3_init(0.0f, 0.0f, 0.0f);
	vector3Normalized = Vector3_normalized(vector3);
	success = Vector3_normalize(&vector3);
	TestCase_assert(!memcmp(&vector3, &vector3Normalized, sizeof(Vector3)), "Vector3_normalize produced different results (%f, %f, %f) than Vector3_normalized (%f, %f, %f)", vector3.x, vector3.y, vector3.z, vector3Normalized.x, vector3Normalized.y, vector3Normalized.z);
	TestCase_assert(!success, "Expected false but got true");
	assertVector3Exact(vector3, 0.0f, 0.0f, 0.0f);
	
	vector4 = Vector4_init(0.0f, 0.0f, 5.0f, 0.0f);
	vector4Normalized = Vector4_normalized(vector4);
	success = Vector4_normalize(&vector4);
	TestCase_assert(!memcmp(&vector4, &vector4Normalized, sizeof(Vector4)), "Vector4_normalize produced different results (%f, %f, %f, %f) than Vector4_normalized (%f, %f, %f, %f)", vector4.x, vector4.y, vector4.z, vector4.w, vector4Normalized.x, vector4Normalized.y, vector4Normalized.z, vector4Normalized.w);
	TestCase_assert(success, "Expected true but got false");
	assertVector4Approximate(vector4, 0.0f, 0.0f, 1.0f, 0.0f, EPSILON);
	
	vector4 = Vector4_init(1.0f, 1.0f, 1.0f, 1.0f);
	vector4Normalized = Vector4_normalized(vector4);
	success = Vector4_normalize(&vector4);
	TestCase_assert(!memcmp(&vector4, &vector4Normalized, sizeof(Vector4)), "Vector4_normalize produced different results (%f, %f, %f, %f) than Vector4_normalized (%f, %f, %f, %f)", vector4.x, vector4.y, vector4.z, vector4.w, vector4Normalized.x, vector4Normalized.y, vector4Normalized.z, vector4Normalized.w);
	TestCase_assert(success, "Expected true but got false");
	assertVector4Approximate(vector4, 0.5f, 0.5f, 0.5f, 0.5f, EPSILON);
	
	vector4 = Vector4_init(0.0f, 0.0f, 0.0f, 0.0f);
	vector4Normalized = Vector4_normalized(vector4);
	success = Vector4_normalize(&vector4);
	TestCase_assert(!memcmp(&vector4, &vector4Normalized, sizeof(Vector4)), "Vector4_normalize produced different results (%f, %f, %f, %f) than Vector4_normalized (%f, %f, %f, %f)", vector4.x, vector4.y, vector4.z, vector4.w, vector4Normalized.x, vector4Normalized.y, vector4Normalized.z, vector4Normalized.w);
	TestCase_assert(!success, "Expected false but got true");
	assertVector4Exact(vector4, 0.0f, 0.0f, 0.0f, 0.0f);
}

static void testMagnitude() {
	float magnitude;
	
	magnitude = Vector2_magnitude(Vector2_init(1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector2_magnitude(Vector2_init(1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 2.0f) < EPSILON, "Expected 2.0 but got %f", magnitude);
	
	magnitude = Vector3_magnitude(Vector3_init(0.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector3_magnitude(Vector3_init(1.4142135623731f, 1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 2.44948974278318f) < EPSILON, "Expected 2.44948974278318 but got %f", magnitude);
	
	magnitude = Vector4_magnitude(Vector4_init(0.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector4_magnitude(Vector4_init(1.4142135623731f, 1.4142135623731f, 1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 2.82842712474619f) < EPSILON, "Expected 2.82842712474619 but got %f", magnitude);
	
	magnitude = Vector2_magnitudeSquared(Vector2_init(1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector2_magnitudeSquared(Vector2_init(1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 4.0f) < EPSILON, "Expected 4.0 but got %f", magnitude);
	
	magnitude = Vector3_magnitudeSquared(Vector3_init(0.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector3_magnitudeSquared(Vector3_init(1.4142135623731f, 1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 6.0f) < EPSILON, "Expected 6.0 but got %f", magnitude);
	
	magnitude = Vector4_magnitudeSquared(Vector4_init(0.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(magnitude - 1.0f) < EPSILON, "Expected 1.0 but got %f", magnitude);
	magnitude = Vector4_magnitudeSquared(Vector4_init(1.4142135623731f, 1.4142135623731f, 1.4142135623731f, 1.4142135623731f));
	TestCase_assert(fabs(magnitude - 8.0f) < EPSILON, "Expected 8.0 but got %f", magnitude);
}

static void testArithmetic() {
	Vector2 vector2;
	Vector3 vector3;
	Vector4 vector4;
	
	vector2 = Vector2_add(Vector2_init(0.0f, 1.0f), Vector2_init(2.0f, 3.0f));
	assertVector2Approximate(vector2, 2.0f, 4.0f, EPSILON);
	vector2 = Vector2_add(Vector2_init(1.5f, 0.5f), Vector2_init(0.25f, -0.25f));
	assertVector2Approximate(vector2, 1.75f, 0.25f, EPSILON);
	
	vector2 = Vector2_subtract(Vector2_init(0.0f, 1.0f), Vector2_init(2.0f, 3.0f));
	assertVector2Approximate(vector2, -2.0f, -2.0f, EPSILON);
	vector2 = Vector2_subtract(Vector2_init(1.5f, 0.5f), Vector2_init(0.25f, -0.25f));
	assertVector2Approximate(vector2, 1.25f, 0.75f, EPSILON);
	
	vector2 = Vector2_multiplyScalar(Vector2_init(1.0f, 2.0f), 0.5f);
	assertVector2Approximate(vector2, 0.5f, 1.0f, EPSILON);
	vector2 = Vector2_multiplyScalar(Vector2_init(3.0f, 4.0f), -2.0f);
	assertVector2Approximate(vector2, -6.0f, -8.0f, EPSILON);
	
	vector2 = Vector2_divideScalar(Vector2_init(1.0f, 2.0f), 0.5f);
	assertVector2Approximate(vector2, 2.0f, 4.0f, EPSILON);
	vector2 = Vector2_divideScalar(Vector2_init(3.0f, 4.0f), -2.0f);
	assertVector2Approximate(vector2, -1.5f, -2.0f, EPSILON);
	
	vector3 = Vector3_add(Vector3_init(0.0f, 1.0f, 2.0f), Vector3_init(2.0f, 3.0f, 4.0f));
	assertVector3Approximate(vector3, 2.0f, 4.0f, 6.0f, EPSILON);
	vector3 = Vector3_add(Vector3_init(1.5f, 0.5f, -0.5f), Vector3_init(0.25f, -0.25f, 0.0f));
	assertVector3Approximate(vector3, 1.75f, 0.25f, -0.5f, EPSILON);
	
	vector3 = Vector3_subtract(Vector3_init(0.0f, 1.0f, 2.0f), Vector3_init(2.0f, 3.0f, 4.0f));
	assertVector3Approximate(vector3, -2.0f, -2.0f, -2.0, EPSILON);
	vector3 = Vector3_subtract(Vector3_init(1.5f, 0.5f, -0.5f), Vector3_init(0.25f, -0.25f, 0.0f));
	assertVector3Approximate(vector3, 1.25f, 0.75f, -0.5f, EPSILON);
	
	vector3 = Vector3_multiplyScalar(Vector3_init(1.0f, 2.0f, 3.0f), 0.5f);
	assertVector3Approximate(vector3, 0.5f, 1.0f, 1.5f, EPSILON);
	vector3 = Vector3_multiplyScalar(Vector3_init(3.0f, 4.0f, 5.0f), -2.0f);
	assertVector3Approximate(vector3, -6.0f, -8.0f, -10.0f, EPSILON);
	
	vector3 = Vector3_divideScalar(Vector3_init(1.0f, 2.0f, 3.0f), 0.5f);
	assertVector3Approximate(vector3, 2.0f, 4.0f, 6.0f, EPSILON);
	vector3 = Vector3_divideScalar(Vector3_init(3.0f, 4.0f, 5.0f), -2.0f);
	assertVector3Approximate(vector3, -1.5f, -2.0f, -2.5f, EPSILON);
	
	vector4 = Vector4_add(Vector4_init(0.0f, 1.0f, 2.0f, 3.0f), Vector4_init(2.0f, 3.0f, 4.0f, 5.0f));
	assertVector4Approximate(vector4, 2.0f, 4.0f, 6.0f, 8.0f, EPSILON);
	vector4 = Vector4_add(Vector4_init(1.5f, 0.5f, -0.5f, -1.5f), Vector4_init(0.25f, -0.25f, 0.0f, 0.1f));
	assertVector4Approximate(vector4, 1.75f, 0.25f, -0.5f, -1.4f, EPSILON);
	
	vector4 = Vector4_subtract(Vector4_init(0.0f, 1.0f, 2.0f, 3.0f), Vector4_init(2.0f, 3.0f, 4.0f, 5.0f));
	assertVector4Approximate(vector4, -2.0f, -2.0f, -2.0, -2.0f, EPSILON);
	vector4 = Vector4_subtract(Vector4_init(1.5f, 0.5f, -0.5f, -1.5f), Vector4_init(0.25f, -0.25f, 0.0f, 0.1f));
	assertVector4Approximate(vector4, 1.25f, 0.75f, -0.5f, -1.6f, EPSILON);
	
	vector4 = Vector4_multiplyScalar(Vector4_init(1.0f, 2.0f, 3.0f, 4.0f), 0.5f);
	assertVector4Approximate(vector4, 0.5f, 1.0f, 1.5f, 2.0f, EPSILON);
	vector4 = Vector4_multiplyScalar(Vector4_init(3.0f, 4.0f, 5.0f, 6.0f), -2.0f);
	assertVector4Approximate(vector4, -6.0f, -8.0f, -10.0f, -12.0f, EPSILON);
	
	vector4 = Vector4_divideScalar(Vector4_init(1.0f, 2.0f, 3.0f, 4.0f), 0.5f);
	assertVector4Approximate(vector4, 2.0f, 4.0f, 6.0f, 8.0f, EPSILON);
	vector4 = Vector4_divideScalar(Vector4_init(3.0f, 4.0f, 5.0f, 6.0f), -2.0f);
	assertVector4Approximate(vector4, -1.5f, -2.0f, -2.5f, -3.0f, EPSILON);
}

static void testInterpolate() {
	Vector2 vector2;
	Vector3 vector3;
	Vector4 vector4;
	
	vector2 = Vector2_interpolate(Vector2_init(0.0f, 1.0f), Vector2_init(1.0f, 0.0f), 0.25f);
	assertVector2Approximate(vector2, 0.25f, 0.75f, EPSILON);
	vector2 = Vector2_interpolate(Vector2_init(3.0f, -3.0f), Vector2_init(0.0f, 0.0f), 1.5f);
	assertVector2Approximate(vector2, -1.5f, 1.5f, EPSILON);
	
	vector3 = Vector3_interpolate(Vector3_init(0.0f, 1.0f, 2.0f), Vector3_init(1.0f, 0.0f, -2.0f), 0.25f);
	assertVector3Approximate(vector3, 0.25f, 0.75f, 1.0f, EPSILON);
	vector3 = Vector3_interpolate(Vector3_init(3.0f, -3.0f, 1.0f), Vector3_init(0.0f, 0.0f, 1.5f), -0.5f);
	assertVector3Approximate(vector3, 4.5f, -4.5f, 0.75f, EPSILON);
	
	vector4 = Vector4_interpolate(Vector4_init(0.0f, 1.0f, 2.0f, 3.0f), Vector4_init(1.0f, 0.0f, -2.0f, -3.0f), 0.5f);
	assertVector4Approximate(vector4, 0.5f, 0.5f, 0.0f, 0.0f, EPSILON);
	vector4 = Vector4_interpolate(Vector4_init(4.0f, -3.0f, 1.0f, -2.0f), Vector4_init(0.0f, -1.0f, 1.5f, -1.0), 0.75f);
	assertVector4Approximate(vector4, 1.0f, -1.5f, 1.375f, -1.25f, EPSILON);
}

static void testReflect() {
	Vector2 vector2;
	Vector3 vector3;
	Vector4 vector4;
	
	vector2 = Vector2_reflect(Vector2_init(0.0f, 1.0f), Vector2_init(sqrt(0.5f), sqrt(0.5f)));
	assertVector2Approximate(vector2, 1.0f, 0.0f, EPSILON);
	vector2 = Vector2_reflect(Vector2_init(-5.0f, 1.0f), Vector2_init(-1.0f, 0.0f));
	assertVector2Approximate(vector2, -5.0f, -1.0f, EPSILON);
	
	vector3 = Vector3_reflect(Vector3_init(0.0f, 1.0f, 0.0f), Vector3_init(0.0f, sqrt(0.5f), sqrt(0.5f)));
	assertVector3Approximate(vector3, 0.0f, 0.0f, 1.0f, EPSILON);
	vector3 = Vector3_reflect(Vector3_init(-5.0f, 1.0f, 0.0f), Vector3_init(-1.0f, 0.0f, 0.0f));
	assertVector3Approximate(vector3, -5.0f, -1.0f, 0.0f, EPSILON);
	
	vector4 = Vector4_reflect(Vector4_init(0.0f, 1.0f, 0.0f, 0.0f), Vector4_init(0.0f, 0.0f, sqrt(0.5f), sqrt(0.5f)));
	assertVector4Approximate(vector4, 0.0f, -1.0f, 0.0f, 0.0f, EPSILON);
	vector4 = Vector4_reflect(Vector4_init(-5.0f, 1.0f, 0.0f, 0.0f), Vector4_init(-1.0f, 0.0f, 0.0f, 0.0f));
	assertVector4Approximate(vector4, -5.0f, -1.0f, 0.0f, 0.0f, EPSILON);
}

static void testDot() {
	float dot;
	
	dot = Vector2_dot(Vector2_init(1.0f, 0.0f), Vector2_init(0.5f, 1.0f));
	TestCase_assert(fabs(dot - 0.5f) < EPSILON, "Expected 0.5 but got %f", dot);
	dot = Vector2_dot(Vector2_init(0.0f, 2.0f), Vector2_init(0.0f, -1.0f));
	TestCase_assert(fabs(dot - -2.0f) < EPSILON, "Expected -2.0 but got %f", dot);
	
	dot = Vector3_dot(Vector3_init(1.0f, 0.0f, 0.0f), Vector3_init(0.5f, 1.0f, 10.0f));
	TestCase_assert(fabs(dot - 0.5f) < EPSILON, "Expected 0.5 but got %f", dot);
	dot = Vector3_dot(Vector3_init(0.0f, 2.0f, 4.0f), Vector3_init(0.0f, -1.0f, 0.5f));
	TestCase_assert(fabs(dot - 0.0f) < EPSILON, "Expected 1.0 but got %f", dot);
	
	dot = Vector4_dot(Vector4_init(1.0f, 0.0f, 0.0f, 0.0f), Vector4_init(0.5f, 1.0f, 10.0f, -2.0f));
	TestCase_assert(fabs(dot - 0.5f) < EPSILON, "Expected 0.5 but got %f", dot);
	dot = Vector4_dot(Vector4_init(0.0f, 2.0f, 4.0f, 0.5f), Vector4_init(0.0f, -1.0f, 0.5f, 2.0f));
	TestCase_assert(fabs(dot - 1.0f) < EPSILON, "Expected 1.0 but got %f", dot);
}

static void testCross() {
	float cross2;
	Vector3 cross3;
	
	cross2 = Vector2_cross(Vector2_init(1.0f, 0.0f), Vector2_init(0.0f, 2.0f));
	TestCase_assert(fabs(cross2 - 2.0f) < EPSILON, "Expected 2.0 but got %f", cross2);
	cross2 = Vector2_cross(Vector2_init(-1.0f, 0.0f), Vector2_init(0.0f, 1.0f));
	TestCase_assert(fabs(cross2 - -1.0f) < EPSILON, "Expected -1.0 but got %f", cross2);
	
	cross3 = Vector3_cross(Vector3_init(1.0f, 0.0f, 0.0f), Vector3_init(0.0f, 1.0f, 0.0f));
	assertVector3Approximate(cross3, 0.0f, 0.0f, 1.0f, EPSILON);
	cross3 = Vector3_cross(Vector3_init(0.0f, 0.0f, -1.0f), Vector3_init(0.0f, -2.0f, 0.0f));
	assertVector3Approximate(cross3, -2.0f, 0.0f, 0.0f, EPSILON);
}

TEST_SUITE(VectorTest,
           testInit,
           testPrefabs,
           testNormalize,
           testMagnitude,
           testArithmetic,
           testInterpolate,
           testReflect,
           testDot,
           testCross)
