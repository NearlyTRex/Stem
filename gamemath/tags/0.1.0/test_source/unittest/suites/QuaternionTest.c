#include <math.h>
#include <stdbool.h>
#include "3dmath/Matrix.h"
#include "3dmath/Quaternion.h"
#include "3dmath/Vector.h"
#include "unittest/framework/TestSuite.h"

static void testLoadIdentity() {
	Quaternion quaternion = {1.2f, 3.4f, 5.6f, 7.8f};
	
	Quaternion_loadIdentity(&quaternion);
	TestCase_assert(quaternion.x == 0.0f &&
	                quaternion.y == 0.0f &&
	                quaternion.z == 0.0f &&
	                quaternion.w == 1.0f,
	                "Incorrect quaternion values (expected {0, 0, 0, 0}; got {%f, %f, %f, %f})",
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testIdentity() {
	Quaternion quaternion;
	
	quaternion = Quaternion_identity();
	TestCase_assert(quaternion.x == 0.0f &&
	                quaternion.y == 0.0f &&
	                quaternion.z == 0.0f &&
	                quaternion.w == 1.0f,
	                "Incorrect quaternion values (expected {0, 0, 0, 0}; got {%f, %f, %f, %f})",
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testInit() {
	Quaternion quaternion;
	
	quaternion = Quaternion_init(1.2f, 3.4f, 5.6f, 7.8f);
	TestCase_assert(fabs(quaternion.x - 1.2f) < 0.0001f &&
	                fabs(quaternion.y - 3.4f) < 0.0001f &&
	                fabs(quaternion.z - 5.6f) < 0.0001f &&
	                fabs(quaternion.w - 7.8f) < 0.0001f,
	                "Incorrect quaternion values (expected {1.2, 3.4, 5.6, 7.8}; got {%f, %f, %f, %f})",
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_init(-0.1f, -0.2f, -0.3f, -0.4f);
	TestCase_assert(fabs(quaternion.x - -0.1f) < 0.0001f &&
	                fabs(quaternion.y - -0.2f) < 0.0001f &&
	                fabs(quaternion.z - -0.3f) < 0.0001f &&
	                fabs(quaternion.w - -0.4f) < 0.0001f,
	                "Incorrect quaternion values (expected {0.1, 0.2, 0.3, 0.4}; got {%f, %f, %f, %f})",
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testFromVector() {
	Quaternion quaternion;
	Vector3 normalizedVector = {0.267261241912425f, 0.534522483824849f, 0.801783725737273f};
	Vector3 longVector = {1.0f, 2.0f, 3.0f};
	Vector3 shortVector = {-0.3f, -0.2f, -0.1f};
	
	quaternion = Quaternion_fromVector(normalizedVector);
	TestCase_assert(fabs(quaternion.x - normalizedVector.x) < 0.0001f &&
	                fabs(quaternion.y - normalizedVector.y) < 0.0001f &&
	                fabs(quaternion.z - normalizedVector.z) < 0.0001f &&
	                fabs(quaternion.w - 0.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                normalizedVector.x, normalizedVector.y, normalizedVector.z, 0.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_fromVector(longVector);
	TestCase_assert(fabs(quaternion.x - longVector.x) < 0.0001f &&
	                fabs(quaternion.y - longVector.y) < 0.0001f &&
	                fabs(quaternion.z - longVector.z) < 0.0001f &&
	                fabs(quaternion.w - 0.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                longVector.x, longVector.y, longVector.z, 0.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_fromVector(shortVector);
	TestCase_assert(fabs(quaternion.x - shortVector.x) < 0.0001f &&
	                fabs(quaternion.y - shortVector.y) < 0.0001f &&
	                fabs(quaternion.z - shortVector.z) < 0.0001f &&
	                fabs(quaternion.w - 0.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                shortVector.x, shortVector.y, shortVector.z, 0.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testToVector() {
	Quaternion quaternion1 = {0.267261241912425f, 0.534522483824849f, 0.801783725737273f, 0.0f};
	Quaternion quaternion2 = {1.0f, 2.0f, 3.0f, 4.0f};
	Vector3 vector;
	
	vector = Quaternion_toVector(quaternion1);
	TestCase_assert(fabs(vector.x - quaternion1.x) < 0.0001f &&
	                fabs(vector.y - quaternion1.y) < 0.0001f &&
	                fabs(vector.z - quaternion1.z) < 0.0001f,
	                "Incorrect vector values (expected {%f, %f, %f}; got {%f, %f, %f})",
	                quaternion1.x, quaternion1.y, quaternion1.z,
	                vector.x, vector.y, vector.z);
	
	vector = Quaternion_toVector(quaternion2);
	TestCase_assert(fabs(vector.x - quaternion2.x) < 0.0001f &&
	                fabs(vector.y - quaternion2.y) < 0.0001f &&
	                fabs(vector.z - quaternion2.z) < 0.0001f,
	                "Incorrect vector values (expected {%f, %f, %f}; got {%f, %f, %f})",
	                quaternion2.x, quaternion2.y, quaternion2.z,
	                vector.x, vector.y, vector.z);
}

static void testFromAxisAngle() {
	Quaternion quaternion;
	
	quaternion = Quaternion_fromAxisAngle(Vector3_right(), 0.0f);
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - 0.0f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 1.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, 0.0f, 0.0f, 1.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_fromAxisAngle(Vector3_init(60.0f, 60.0f, 60.0f), 1.0f);
	TestCase_assert(fabs(quaternion.x - 0.276796f) < 0.0001f &&
	                fabs(quaternion.y - 0.276796f) < 0.0001f &&
	                fabs(quaternion.z - 0.276796f) < 0.0001f &&
	                fabs(quaternion.w - 0.877583f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.276796f, 0.276796f, 0.276796f, 0.877583f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_fromAxisAngle(Vector3_init(0.0f, -0.1f, 0.0f), M_PI);
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - -1.0f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, -1.0f, 0.0f, 0.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_fromAxisAngle(Vector3_init(0.0f, -0.1f, 0.0f), M_PI / 2);
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - -0.707107f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.707107f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, -0.707107f, 0.0f, 0.707107f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_fromAxisAngle(Vector3_zero(), 0.0f);
	TestCase_assert(isnan(quaternion.x) &&
	                isnan(quaternion.y) &&
	                isnan(quaternion.z) &&
	                fabs(quaternion.w - 1.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                NAN, NAN, NAN, 1.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testToAxisAngle() {
	Vector3 axis;
	float angle;
	
	Quaternion_toAxisAngle(Quaternion_init(0.0f, 0.0f, 0.0f, 1.0f), &axis, &angle);
	TestCase_assert(fabs(axis.x - 0.0f) < 0.0001f &&
	                fabs(axis.y - 0.0f) < 0.0001f &&
	                fabs(axis.z - 0.0f) < 0.0001f &&
	                fabs(angle - 0.0f) < 0.0001f,
	                "Incorrect axis/angle values (expected {%f, %f, %f}, %f; got {%f, %f, %f}, %f)",
	                0.0f, 0.0f, 0.0f, 0.0f,
	                axis.x, axis.y, axis.z, angle);
	
	Quaternion_toAxisAngle(Quaternion_init(0.276796f, 0.276796f, 0.276796f, 0.877583f), &axis, &angle);
	TestCase_assert(fabs(axis.x - 0.577350269189626f) < 0.0001f &&
	                fabs(axis.y - 0.577350269189626f) < 0.0001f &&
	                fabs(axis.z - 0.577350269189626f) < 0.0001f &&
	                fabs(angle - 1.0f) < 0.0001f,
	                "Incorrect axis/angle values (expected {%f, %f, %f}, %f; got {%f, %f, %f}, %f)",
	                0.577350269189626f, 0.577350269189626f, 0.577350269189626f, 1.0f,
	                axis.x, axis.y, axis.z, angle);
	
	Quaternion_toAxisAngle(Quaternion_init(0.0f, -1.0f, 0.0f, 0.0f), &axis, &angle);
	TestCase_assert(fabs(axis.x - 0.0f) < 0.0001f &&
	                fabs(axis.y - -1.0f) < 0.0001f &&
	                fabs(axis.z - 0.0f) < 0.0001f &&
	                fabs(angle - M_PI) < 0.0001f,
	                "Incorrect axis/angle values (expected {%f, %f, %f}, %f; got {%f, %f, %f}, %f)",
	                0.0f, -1.0f, 0.0f, M_PI,
	                axis.x, axis.y, axis.z, angle);
	
	Quaternion_toAxisAngle(Quaternion_init(0.0f, -0.707107f, 0.0f, 0.707107f), &axis, &angle);
	TestCase_assert(fabs(axis.x - 0.0f) < 0.0001f &&
	                fabs(axis.y - -1.0f) < 0.0001f &&
	                fabs(axis.z - 0.0f) < 0.0001f &&
	                fabs(angle - M_PI / 2) < 0.0001f,
	                "Incorrect axis/angle values (expected {%f, %f, %f}, %f; got {%f, %f, %f}, %f)",
	                0.0f, -1.0f, 0.0f, M_PI / 2,
	                axis.x, axis.y, axis.z, angle);
	
	Quaternion_toAxisAngle(Quaternion_init(0.0f, 0.0f, 0.0f, 0.0f), &axis, &angle);
	TestCase_assert(isnan(axis.x) &&
	                isnan(axis.y) &&
	                isnan(axis.z) &&
	                isnan(angle),
	                "Incorrect axis/angle values (expected {%f, %f, %f}, %f; got {%f, %f, %f}, %f)",
	                NAN, NAN, NAN, NAN,
	                axis.x, axis.y, axis.z, angle);
}

static bool QuaternionTests_matrixCompare(Matrix matrix, float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9, float m10, float m11, float m12, float m13, float m14, float m15) {
	return fabs(matrix.m[0] - m0) < 0.0001f &&
	       fabs(matrix.m[1] - m1) < 0.0001f &&
	       fabs(matrix.m[2] - m2) < 0.0001f &&
	       fabs(matrix.m[3] - m3) < 0.0001f &&
	       fabs(matrix.m[4] - m4) < 0.0001f &&
	       fabs(matrix.m[5] - m5) < 0.0001f &&
	       fabs(matrix.m[6] - m6) < 0.0001f &&
	       fabs(matrix.m[7] - m7) < 0.0001f &&
	       fabs(matrix.m[8] - m8) < 0.0001f &&
	       fabs(matrix.m[9] - m9) < 0.0001f &&
	       fabs(matrix.m[10] - m10) < 0.0001f &&
	       fabs(matrix.m[11] - m11) < 0.0001f &&
	       fabs(matrix.m[12] - m12) < 0.0001f &&
	       fabs(matrix.m[13] - m13) < 0.0001f &&
	       fabs(matrix.m[14] - m14) < 0.0001f &&
	       fabs(matrix.m[15] - m15) < 0.0001f;
}

static void testToMatrix() {
	Matrix matrix;
	
	TestCase_assert(!QuaternionTests_matrixCompare(Matrix_init(0.0f, 1.0f, 1.0f, 1.0f,
	                                                           1.0f, 0.0f, 1.0f, 1.0f,
	                                                           1.0f, 1.0f, 0.0f, 1.0f,
	                                                           1.0f, 1.0f, 1.0f, 0.0f),
	                                               1.0f, 0.0f, 0.0f, 0.0f,
	                                               0.0f, 1.0f, 0.0f, 0.0f,
	                                               0.0f, 0.0f, 1.0f, 0.0f,
	                                               0.0f, 0.0f, 0.0f, 1.0f),
	                "Matrix comparison utility function is broken (gave false positive for known mismatch)");
	
	matrix = Quaternion_toMatrix(Quaternion_init(0.0f, 0.0f, 0.0f, 1.0f));
	TestCase_assert(QuaternionTests_matrixCompare(matrix,
	                                              1.0f, 0.0f, 0.0f, 0.0f,
	                                              0.0f, 1.0f, 0.0f, 0.0f,
	                                              0.0f, 0.0f, 1.0f, 0.0f,
	                                              0.0f, 0.0f, 0.0f, 1.0f),
	                "Incorrect matrix values (expected {{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}}; got {{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}})",
	                1.0f, 0.0f, 0.0f, 0.0f,
	                0.0f, 1.0f, 0.0f, 0.0f,
	                0.0f, 0.0f, 1.0f, 0.0f,
	                0.0f, 0.0f, 0.0f, 1.0f,
	                matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
	                matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
	                matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
	                matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15]);
	
	matrix = Quaternion_toMatrix(Quaternion_init(0.0f, 0.707107f, 0.0f, 0.707107f));
	TestCase_assert(QuaternionTests_matrixCompare(matrix,
	                                              0.0f, 0.0f, -1.0f, 0.0f,
	                                              0.0f, 1.0f, 0.0f, 0.0f,
	                                              1.0f, 0.0f, 0.0f, 0.0f,
	                                              0.0f, 0.0f, 0.0f, 1.0f),
	                "Incorrect matrix values (expected {{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}}; got {{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}})",
	                0.0f, 0.0f, -1.0f, 0.0f,
	                0.0f, 1.0f, 0.0f, 0.0f,
	                1.0f, 0.0f, 0.0f, 0.0f,
	                0.0f, 0.0f, 0.0f, 1.0f,
	                matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
	                matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
	                matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
	                matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15]);
	
	matrix = Quaternion_toMatrix(Quaternion_init(0.5f, 0.5f, 0.5f, 0.5f));
	TestCase_assert(QuaternionTests_matrixCompare(matrix,
	                                              0.0f, 1.0f, 0.0f, 0.0f,
	                                              0.0f, 0.0f, 1.0f, 0.0f,
	                                              1.0f, 0.0f, 0.0f, 0.0f,
	                                              0.0f, 0.0f, 0.0f, 1.0f),
	                "Incorrect matrix values (expected {{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}}; got {{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}})",
	                0.0f, 1.0f, 0.0f, 0.0f,
	                0.0f, 0.0f, 1.0f, 0.0f,
	                1.0f, 0.0f, 0.0f, 0.0f,
	                0.0f, 0.0f, 0.0f, 1.0f,
	                matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
	                matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
	                matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
	                matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15]);
	
	matrix = Quaternion_toMatrix(Quaternion_init(1.0f, 1.0f, 1.0f, 1.0f));
	TestCase_assert(QuaternionTests_matrixCompare(matrix,
	                                              -3.0f, 4.0f, 0.0f, 0.0f,
	                                              0.0f, -3.0f, 4.0f, 0.0f,
	                                              4.0f, 0.0f, -3.0f, 0.0f,
	                                              0.0f, 0.0f, 0.0f, 1.0f),
	                "Incorrect matrix values (expected {{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}}; got {{%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f, %f}})",
	                -3.0f, 4.0f, 0.0f, 0.0f,
	                0.0f, -3.0f, 4.0f, 0.0f,
	                4.0f, 0.0f, -3.0f, 0.0f,
	                0.0f, 0.0f, 0.0f, 1.0f,
	                matrix.m[0], matrix.m[1], matrix.m[2], matrix.m[3],
	                matrix.m[4], matrix.m[5], matrix.m[6], matrix.m[7],
	                matrix.m[8], matrix.m[9], matrix.m[10], matrix.m[11],
	                matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15]);
}

static void testNormalize() {
	Quaternion quaternion;
	
	quaternion = Quaternion_init(1.0f, -1.0f, 1.0f, -1.0f);
	Quaternion_normalize(&quaternion);
	TestCase_assert(fabs(quaternion.x - 0.5f) < 0.0001f &&
	                fabs(quaternion.y - -0.5f) < 0.0001f &&
	                fabs(quaternion.z - 0.5f) < 0.0001f &&
	                fabs(quaternion.w - -0.5f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.5f, -0.5f, 0.5f, -0.5f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	Quaternion_normalize(&quaternion);
	TestCase_assert(fabs(quaternion.x - 0.5f) < 0.0001f &&
	                fabs(quaternion.y - -0.5f) < 0.0001f &&
	                fabs(quaternion.z - 0.5f) < 0.0001f &&
	                fabs(quaternion.w - -0.5f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.5f, -0.5f, 0.5f, -0.5f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testNormalized() {
	Quaternion quaternion;
	
	quaternion = Quaternion_normalized(Quaternion_init(1.0f, -1.0f, 1.0f, -1.0f));
	TestCase_assert(fabs(quaternion.x - 0.5f) < 0.0001f &&
	                fabs(quaternion.y - -0.5f) < 0.0001f &&
	                fabs(quaternion.z - 0.5f) < 0.0001f &&
	                fabs(quaternion.w - -0.5f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.5f, -0.5f, 0.5f, -0.5f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	quaternion = Quaternion_normalized(Quaternion_init(0.5f, -0.5f, 0.5f, -0.5f));
	TestCase_assert(fabs(quaternion.x - 0.5f) < 0.0001f &&
	                fabs(quaternion.y - -0.5f) < 0.0001f &&
	                fabs(quaternion.z - 0.5f) < 0.0001f &&
	                fabs(quaternion.w - -0.5f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.5f, -0.5f, 0.5f, -0.5f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testMultiply() {
	Quaternion quaternion;
	
	quaternion = Quaternion_init(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion_multiply(&quaternion, Quaternion_init(0.0f, 0.0f, 0.0f, 1.0f));
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - 0.0f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 1.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, 0.0f, 0.0f, 1.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_init(-1.0f, -1.0f, 0.0f, 1.0f);
	Quaternion_multiply(&quaternion, Quaternion_init(-0.5f, 0.5f, 0.5f, 0.5f));
	TestCase_assert(fabs(quaternion.x - -1.5f) < 0.0001f &&
	                fabs(quaternion.y - 0.5f) < 0.0001f &&
	                fabs(quaternion.z - -0.5f) < 0.0001f &&
	                fabs(quaternion.w - 0.5f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                -1.5f, 0.5f, -0.5f, 0.5f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testMultiplied() {
	Quaternion quaternion;
	
	quaternion = Quaternion_multiplied(Quaternion_init(0.0f, 0.0f, 0.0f, 1.0f),
	                                   Quaternion_init(0.0f, 0.0f, 0.0f, 1.0f));
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - 0.0f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 1.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, 0.0f, 0.0f, 1.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_multiplied(Quaternion_init(-1.0f, -1.0f, 0.0f, 1.0f),
	                                   Quaternion_init(-0.5f, 0.5f, 0.5f, 0.5f));
	TestCase_assert(fabs(quaternion.x - -1.5f) < 0.0001f &&
	                fabs(quaternion.y - 0.5f) < 0.0001f &&
	                fabs(quaternion.z - -0.5f) < 0.0001f &&
	                fabs(quaternion.w - 0.5f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                -1.5f, 0.5f, -0.5f, 0.5f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testSlerp() {
	Quaternion quaternion;
	
	quaternion = Quaternion_slerp(Quaternion_init(1.0f, 0.0f, 0.0f, 0.0f),
	                              Quaternion_init(0.0f, 1.0f, 0.0f, 0.0f),
	                              0.0f);
	TestCase_assert(fabs(quaternion.x - 1.0f) < 0.0001f &&
	                fabs(quaternion.y - 0.0f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                1.0f, 0.0f, 0.0f, 0.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_slerp(Quaternion_init(1.0f, 0.0f, 0.0f, 0.0f),
	                              Quaternion_init(0.0f, 1.0f, 0.0f, 0.0f),
	                              1.0f);
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - 1.0f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, 1.0f, 0.0f, 0.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_slerp(Quaternion_init(0.707107f, 0.0f, 0.0f, 0.707107f),
	                              Quaternion_init(0.0f, 0.707107f, 0.0f, 0.707107f),
	                              0.5f);
	TestCase_assert(fabs(quaternion.x - 0.408248290463863f) < 0.0001f &&
	                fabs(quaternion.y - 0.408248290463863f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.816496580927725f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.408248290463863f, 0.408248290463863f, 0.0f, 0.816496580927725f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_slerp(Quaternion_init(0.707107f, 0.0f, 0.0f, 0.707107f),
	                              Quaternion_init(0.0f, 0.707107f, 0.0f, 0.707107f),
	                              0.75f);
	TestCase_assert(fabs(quaternion.x - 0.211325f) < 0.0001f &&
	                fabs(quaternion.y - 0.577350f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.788675f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.211325f, 0.577350f, 0.0f, 0.788675f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testRotate() {
	Quaternion quaternion;
	
	quaternion = Quaternion_identity();
	Quaternion_rotate(&quaternion, Vector3_up(), M_PI / 5);
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - 0.309017f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.951057f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, 0.309017f, 0.0f, 0.951057f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	Quaternion_rotate(&quaternion, Vector3_init(1.0f, 0.0f, 0.5f), 1.0f);
	TestCase_assert(fabs(quaternion.x - 0.474079f) < 0.0001f &&
	                fabs(quaternion.y - 0.271188f) < 0.0001f &&
	                fabs(quaternion.z - 0.071402f) < 0.0001f &&
	                fabs(quaternion.w - 0.834631f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.474079f, 0.271188f, 0.071402f, 0.834631f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testRotated() {
	Quaternion quaternion;
	
	quaternion = Quaternion_identity();
	quaternion = Quaternion_rotated(quaternion, Vector3_up(), M_PI / 5);
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - 0.309017f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.951057f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, 0.309017f, 0.0f, 0.951057f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_rotated(quaternion, Vector3_init(1.0f, 0.0f, 0.5f), 1.0f);
	TestCase_assert(fabs(quaternion.x - 0.474079f) < 0.0001f &&
	                fabs(quaternion.y - 0.271188f) < 0.0001f &&
	                fabs(quaternion.z - 0.071402f) < 0.0001f &&
	                fabs(quaternion.w - 0.834631f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.474079f, 0.271188f, 0.071402f, 0.834631f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testInvert() {
	Quaternion quaternion;
	
	quaternion = Quaternion_init(0.0f, 1.0f, 0.0f, 0.0f);
	Quaternion_invert(&quaternion);
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - -1.0f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, -1.0f, 0.0f, 0.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_init(-0.5f, 0.5f, 1.0f, 0.5f);
	Quaternion_invert(&quaternion);
	TestCase_assert(fabs(quaternion.x - 0.285714f) < 0.0001f &&
	                fabs(quaternion.y - -0.285714f) < 0.0001f &&
	                fabs(quaternion.z - -0.571429f) < 0.0001f &&
	                fabs(quaternion.w - 0.285714f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.285714f, -0.285714f, -0.571429f, 0.285714f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	Quaternion_invert(&quaternion);
	TestCase_assert(fabs(quaternion.x - -0.5f) < 0.0001f &&
	                fabs(quaternion.y - 0.5f) < 0.0001f &&
	                fabs(quaternion.z - 1.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.5f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                -0.5f, 0.5f, 1.0f, 0.5f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testInverted() {
	Quaternion quaternion;
	
	quaternion = Quaternion_init(0.0f, 1.0f, 0.0f, 0.0f);
	quaternion = Quaternion_inverted(quaternion);
	TestCase_assert(fabs(quaternion.x - 0.0f) < 0.0001f &&
	                fabs(quaternion.y - -1.0f) < 0.0001f &&
	                fabs(quaternion.z - 0.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.0f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, -1.0f, 0.0f, 0.0f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_init(-0.5f, 0.5f, 1.0f, 0.5f);
	quaternion = Quaternion_inverted(quaternion);
	TestCase_assert(fabs(quaternion.x - 0.285714f) < 0.0001f &&
	                fabs(quaternion.y - -0.285714f) < 0.0001f &&
	                fabs(quaternion.z - -0.571429f) < 0.0001f &&
	                fabs(quaternion.w - 0.285714f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.285714f, -0.285714f, -0.571429f, 0.285714f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	quaternion = Quaternion_inverted(quaternion);
	TestCase_assert(fabs(quaternion.x - -0.5f) < 0.0001f &&
	                fabs(quaternion.y - 0.5f) < 0.0001f &&
	                fabs(quaternion.z - 1.0f) < 0.0001f &&
	                fabs(quaternion.w - 0.5f) < 0.0001f,
	                "Incorrect quaternion values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                -0.5f, 0.5f, 1.0f, 0.5f,
	                quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

static void testMultiplyVector3() {
	Vector3 vector;
	
	vector = Quaternion_multiplyVector3(Quaternion_fromAxisAngle(Vector3_up(), M_PI / 4),
	                                    Vector3_init(1.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(vector.x - 0.707107f) < 0.0001f &&
	                fabs(vector.y - 1.0f) < 0.0001f &&
	                fabs(vector.z - -0.707107f) < 0.0001f,
	                "Incorrect vector values (expected {%f, %f, %f}; got {%f, %f, %f})",
	                0.707107f, 1.0f, -0.707107f,
	                vector.x, vector.y, vector.z);
	
	vector = Quaternion_multiplyVector3(Quaternion_fromAxisAngle(Vector3_right(), M_PI / 4),
	                                    Vector3_init(-2.5f, 2.0f, 0.0f));
	TestCase_assert(fabs(vector.x - -2.5f) < 0.0001f &&
	                fabs(vector.y - 1.414213f) < 0.0001f &&
	                fabs(vector.z - 1.414213f) < 0.0001f,
	                "Incorrect vector values (expected {%f, %f, %f}; got {%f, %f, %f})",
	                -2.5f, 1.414213f, 1.414213f,
	                vector.x, vector.y, vector.z);
	
	vector = Quaternion_multiplyVector3(Quaternion_fromAxisAngle(Vector3_front(), M_PI / 4),
	                                    Vector3_init(1.0f, 1.0f, 0.0f));
	TestCase_assert(fabs(vector.x - 0.0f) < 0.0001f &&
	                fabs(vector.y - 1.414213f) < 0.0001f &&
	                fabs(vector.z - 0.0f) < 0.0001f,
	                "Incorrect vector values (expected {%f, %f, %f}; got {%f, %f, %f})",
	                0.0f, 1.414213f, 0.0f,
	                vector.x, vector.y, vector.z);
}

static void testMultiplyVector4() {
	Vector4 vector;
	
	vector = Quaternion_multiplyVector4(Quaternion_fromAxisAngle(Vector3_up(), M_PI / 4),
	                                    Vector4_init(1.0f, 1.0f, 0.0f, 1.0f));
	TestCase_assert(fabs(vector.x - 0.707107f) < 0.0001f &&
	                fabs(vector.y - 1.0f) < 0.0001f &&
	                fabs(vector.z - -0.707107f) < 0.0001f &&
	                fabs(vector.w - 1.0f) < 0.0001f,
	                "Incorrect vector values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.707107f, 1.0f, -0.707107f, 1.0f,
	                vector.x, vector.y, vector.z, vector.w);
	
	vector = Quaternion_multiplyVector4(Quaternion_fromAxisAngle(Vector3_right(), M_PI / 4),
	                                    Vector4_init(-2.5f, 2.0f, 0.0f, 0.0f));
	TestCase_assert(fabs(vector.x - -2.5f) < 0.0001f &&
	                fabs(vector.y - 1.414213f) < 0.0001f &&
	                fabs(vector.z - 1.414213f) < 0.0001f &&
	                fabs(vector.w - 0.0f) < 0.0001f,
	                "Incorrect vector values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                -2.5f, 1.414213f, 1.414213f, 0.0f,
	                vector.x, vector.y, vector.z, vector.w);
	
	vector = Quaternion_multiplyVector4(Quaternion_fromAxisAngle(Vector3_front(), M_PI / 4),
	                                    Vector4_init(1.0f, 1.0f, 0.0f, 0.0f));
	TestCase_assert(fabs(vector.x - 0.0f) < 0.0001f &&
	                fabs(vector.y - 1.414213f) < 0.0001f &&
	                fabs(vector.z - 0.0f) < 0.0001f &&
	                fabs(vector.w - 0.0f) < 0.0001f,
	                "Incorrect vector values (expected {%f, %f, %f, %f}; got {%f, %f, %f, %f})",
	                0.0f, 1.414213f, 0.0f, 0.0f,
	                vector.x, vector.y, vector.z, vector.w);
}

TEST_SUITE(QuaternionTest,
           testLoadIdentity,
           testIdentity,
           testInit,
           testFromVector,
           testToVector,
           testFromAxisAngle,
           testToAxisAngle,
           testToMatrix,
           testNormalize,
           testNormalized,
           testMultiply,
           testMultiplied,
           testSlerp,
           testRotate,
           testRotated,
           testInvert,
           testInverted,
           testMultiplyVector3,
           testMultiplyVector4)
