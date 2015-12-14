#include <stdbool.h>
#include "gamemath/Quaternionx.h"
#include "unittest/TestSuite.h"

#define assertQuaternion(quaternion, expectedX, expectedY, expectedZ, expectedW) \
	TestCase_assert(quaternion.x == expectedX && quaternion.y == expectedY && quaternion.z == expectedZ && quaternion.w == expectedW, "Expected {0x%05X, 0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}", expectedX, expectedY, expectedZ, expectedW, quaternion.x, quaternion.y, quaternion.z, quaternion.w)

#define assertAxisAngle(axis, angle, expectedX, expectedY, expectedZ, expectedAngle) \
	TestCase_assert(axis.x == expectedX && axis.y == expectedY && axis.z == expectedZ && angle == expectedAngle, "Expected {0x%05X, 0x%05X, 0x%05X}, 0x%05X but got {0x%05X, 0x%05X, 0x%05X}, 0x%05X", expectedX, expectedY, expectedZ, expectedAngle, axis.x, axis.y, axis.z, angle)

#define assertMatrix(matrix, expected0, expected4, expected8,  expected12, \
                             expected1, expected5, expected9,  expected13, \
                             expected2, expected6, expected10, expected14, \
                             expected3, expected7, expected11, expected15) \
	TestCase_assert(matrix.m[0]  == expected0,  "Expected 0x%05X [0] but got 0x%05X",  expected0,  matrix.m[0]); \
	TestCase_assert(matrix.m[1]  == expected1,  "Expected 0x%05X [1] but got 0x%05X",  expected1,  matrix.m[1]); \
	TestCase_assert(matrix.m[2]  == expected2,  "Expected 0x%05X [2] but got 0x%05X",  expected2,  matrix.m[2]); \
	TestCase_assert(matrix.m[3]  == expected3,  "Expected 0x%05X [3] but got 0x%05X",  expected3,  matrix.m[3]); \
	TestCase_assert(matrix.m[4]  == expected4,  "Expected 0x%05X [4] but got 0x%05X",  expected4,  matrix.m[4]); \
	TestCase_assert(matrix.m[5]  == expected5,  "Expected 0x%05X [5] but got 0x%05X",  expected5,  matrix.m[5]); \
	TestCase_assert(matrix.m[6]  == expected6,  "Expected 0x%05X [6] but got 0x%05X",  expected6,  matrix.m[6]); \
	TestCase_assert(matrix.m[7]  == expected7,  "Expected 0x%05X [7] but got 0x%05X",  expected7,  matrix.m[7]); \
	TestCase_assert(matrix.m[8]  == expected8,  "Expected 0x%05X [8] but got 0x%05X",  expected8,  matrix.m[8]); \
	TestCase_assert(matrix.m[9]  == expected9,  "Expected 0x%05X [9] but got 0x%05X",  expected9,  matrix.m[9]); \
	TestCase_assert(matrix.m[10] == expected10, "Expected 0x%05X [10] but got 0x%05X", expected10, matrix.m[10]); \
	TestCase_assert(matrix.m[11] == expected11, "Expected 0x%05X [11] but got 0x%05X", expected11, matrix.m[11]); \
	TestCase_assert(matrix.m[12] == expected12, "Expected 0x%05X [12] but got 0x%05X", expected12, matrix.m[12]); \
	TestCase_assert(matrix.m[13] == expected13, "Expected 0x%05X [13] but got 0x%05X", expected13, matrix.m[13]); \
	TestCase_assert(matrix.m[14] == expected14, "Expected 0x%05X [14] but got 0x%05X", expected14, matrix.m[14]); \
	TestCase_assert(matrix.m[15] == expected15, "Expected 0x%05X [15] but got 0x%05X", expected15, matrix.m[15])

#define assertVector3x(vector, expectedX, expectedY, expectedZ) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY && vector.z == expectedZ, "Expected {0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X}", expectedX, expectedY, expectedZ, vector.x, vector.y, vector.z);

#define assertVector4x(vector, expectedX, expectedY, expectedZ, expectedW) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY && vector.z == expectedZ && vector.w == expectedW, "Expected {0x%05X, 0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}", expectedX, expectedY, expectedZ, expectedW, vector.x, vector.y, vector.z, vector.w);

static void testLoadIdentity() {
	Quaternionx quaternion;
	
	memset(&quaternion, 0xFF, sizeof(quaternion));
	Quaternionx_loadIdentity(&quaternion);
	assertQuaternion(quaternion, 0x00000, 0x00000, 0x00000, 0x10000);
}

static void testIdentity() {
	Quaternionx quaternion;
	
	quaternion = QUATERNIONx_IDENTITY;
	assertQuaternion(quaternion, 0x00000, 0x00000, 0x00000, 0x10000);
}

static void testInit() {
	Quaternionx quaternion;
	
	quaternion = QUATERNIONx(0x12000, 0x34000, 0x56000, 0x78000);
	assertQuaternion(quaternion, 0x12000, 0x34000, 0x56000, 0x78000);
	
	quaternion = QUATERNIONx(-0x01000, -0x02000, -0x03000, -0x04000);
	assertQuaternion(quaternion, -0x01000, -0x02000, -0x03000, -0x04000);
}

static void testFromAxisAngle() {
	Quaternionx quaternion;
	
	quaternion = Quaternionx_fromAxisAngle(VECTOR3x_RIGHT, 0x00000);
	assertQuaternion(quaternion, 0x00000, 0x00000, 0x00000, 0x10000);
	
	quaternion = Quaternionx_fromAxisAngle(VECTOR3x(0x3C0000, 0x3C0000, 0x3C0000), 0x10000);
	assertQuaternion(quaternion, 0x046DC, 0x046DC, 0x046DC, 0x0E0A5);
	
	quaternion = Quaternionx_fromAxisAngle(VECTOR3x(0x00000, -0x01000, 0x00000), X_PI);
	assertQuaternion(quaternion, 0x00000, -0x10000, 0x00000, 0x00000);
	
	quaternion = Quaternionx_fromAxisAngle(VECTOR3x(0x00000, -0x01000, 0x00000), X_PI_2);
	assertQuaternion(quaternion, 0x00000, -0x0B504, 0x00000, 0x0B505);
	
	quaternion = Quaternionx_fromAxisAngle(VECTOR3x_ZERO, 0x00000);
	assertQuaternion(quaternion, FIXED_16_16_NAN, FIXED_16_16_NAN, FIXED_16_16_NAN, 0x10000);
}

static void testToAxisAngle() {
	Vector3x axis;
	fixed16_16 angle;
	
	Quaternionx_toAxisAngle(QUATERNIONx(0x00000, 0x00000, 0x00000, 0x10000), &axis, &angle);
	assertAxisAngle(axis, angle, 0x00000, 0x00000, 0x00000, 0x00000);
	
	Quaternionx_toAxisAngle(QUATERNIONx(0x046DC, 0x046DC, 0x046DC, 0x0E0A9), &axis, &angle);
	assertAxisAngle(axis, angle, 0x093CF, 0x093CF, 0x093CF, 0x0FFFE);
	
	Quaternionx_toAxisAngle(QUATERNIONx(0x00000, -0x10000, 0x00000, 0x00000), &axis, &angle);
	assertAxisAngle(axis, angle, 0x00000, -0x10000, 0x00000, 0x32436);
	
	Quaternionx_toAxisAngle(QUATERNIONx(0x00000, -0x0B504, 0x00000, 0x0B504), &axis, &angle);
	assertAxisAngle(axis, angle, 0x00000, -0x10001, 0x00000, 0x1921E);
	
	Quaternionx_toAxisAngle(QUATERNIONx(0x00000, 0x00000, 0x00000, 0x00000), &axis, &angle);
	assertAxisAngle(axis, angle, FIXED_16_16_NAN, FIXED_16_16_NAN, FIXED_16_16_NAN, FIXED_16_16_NAN);
}

static void testToMatrix() {
	Matrix4x4x matrix;
	
	matrix = Quaternionx_toMatrix(QUATERNIONx(0x00000, 0x00000, 0x00000, 0x10000));
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Quaternionx_toMatrix(QUATERNIONx(0x00000, 0x0B504, 0x00000, 0x0B504));
	assertMatrix(matrix,  0x00002, 0x00000, 0x0FFFE, 0x00000,
	                      0x00000, 0x10000, 0x00000, 0x00000,
	                     -0x0FFFE, 0x00000, 0x00002, 0x00000,
	                      0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Quaternionx_toMatrix(QUATERNIONx(0x08000, 0x08000, 0x08000, 0x08000));
	assertMatrix(matrix, 0x00000, 0x00000, 0x10000, 0x00000,
	                     0x10000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Quaternionx_toMatrix(QUATERNIONx(0x10000, 0x10000, 0x10000, 0x10000));
	assertMatrix(matrix, -0x30000,  0x00000,  0x40000, 0x00000,
	                      0x40000, -0x30000,  0x00000, 0x00000,
	                      0x00000,  0x40000, -0x30000, 0x00000,
	                      0x00000,  0x00000,  0x00000, 0x10000);
}

static void testNormalize() {
	Quaternionx quaternion;
	
	quaternion = QUATERNIONx(0x10000, -0x10000, 0x10000, -0x10000);
	Quaternionx_normalize(&quaternion);
	assertQuaternion(quaternion, 0x08000, -0x08000, 0x08000, -0x08000);
	Quaternionx_normalize(&quaternion);
	assertQuaternion(quaternion, 0x08000, -0x08000, 0x08000, -0x08000);
}

static void testNormalized() {
	Quaternionx quaternion;
	
	quaternion = Quaternionx_normalized(QUATERNIONx(0x10000, -0x10000, 0x10000, -0x10000));
	assertQuaternion(quaternion, 0x08000, -0x08000, 0x08000, -0x08000);
	quaternion = Quaternionx_normalized(QUATERNIONx(0x08000, -0x08000, 0x08000, -0x08000));
	assertQuaternion(quaternion, 0x08000, -0x08000, 0x08000, -0x08000);
}

static void testMultiply() {
	Quaternionx quaternion;
	
	quaternion = QUATERNIONx(0x00000, 0x00000, 0x00000, 0x10000);
	Quaternionx_multiply(&quaternion, QUATERNIONx(0x00000, 0x00000, 0x00000, 0x10000));
	assertQuaternion(quaternion, 0x00000, 0x00000, 0x00000, 0x10000);
	
	quaternion = QUATERNIONx(-0x10000, -0x10000, 0x00000, 0x10000);
	Quaternionx_multiply(&quaternion, QUATERNIONx(-0x08000, 0x08000, 0x08000, 0x08000));
	assertQuaternion(quaternion, -0x18000, 0x08000, -0x08000, 0x08000);
}

static void testMultiplied() {
	Quaternionx quaternion;
	
	quaternion = Quaternionx_multiplied(QUATERNIONx(0x00000, 0x00000, 0x00000, 0x10000),
	                                    QUATERNIONx(0x00000, 0x00000, 0x00000, 0x10000));
	assertQuaternion(quaternion, 0x00000, 0x00000, 0x00000, 0x10000);
	
	quaternion = Quaternionx_multiplied(QUATERNIONx(-0x10000, -0x10000, 0x00000, 0x10000),
	                                    QUATERNIONx(-0x08000, 0x08000, 0x08000, 0x08000));
	assertQuaternion(quaternion, -0x18000, 0x08000, -0x08000, 0x08000);
}

static void testSlerp() {
	Quaternionx quaternion;
	
	quaternion = Quaternionx_slerp(QUATERNIONx(0x10000, 0x00000, 0x00000, 0x00000),
	                               QUATERNIONx(0x00000, 0x10000, 0x00000, 0x00000),
	                               0x00000);
	assertQuaternion(quaternion, 0x10000, 0x00000, 0x00000, 0x00000);
	
	quaternion = Quaternionx_slerp(QUATERNIONx(0x10000, 0x00000, 0x00000, 0x00000),
	                               QUATERNIONx(0x00000, 0x10000, 0x00000, 0x00000),
	                               0x10000);
	assertQuaternion(quaternion, 0x00000, 0x10000, 0x00000, 0x00000);
	
	quaternion = Quaternionx_slerp(QUATERNIONx(0x0B504, 0x00000, 0x00000, 0x0B504),
	                               QUATERNIONx(0x00000, 0x0B504, 0x00000, 0x0B504),
	                               0x08000);
	assertQuaternion(quaternion, 0x06883, 0x06883, 0x00000, 0x0D107);
	
	quaternion = Quaternionx_slerp(QUATERNIONx(0x0B504, 0x00000, 0x00000, 0x0B504),
	                               QUATERNIONx(0x00000, 0x0B504, 0x00000, 0x0B504),
	                               0x0C000);
	assertQuaternion(quaternion, 0x034E7, 0x094AD, 0x00000, 0x0C994);
}

static void testRotate() {
	Quaternionx quaternion;
	
	quaternion = QUATERNIONx_IDENTITY;
	Quaternionx_rotate(&quaternion, VECTOR3x_UP, X_PI / 5);
	assertQuaternion(quaternion, 0x00000, 0x04F1B, 0x00000, 0x0F36C);
	
	Quaternionx_rotate(&quaternion, VECTOR3x(0x10000, 0x00000, 0x08000), 0x10000);
	assertQuaternion(quaternion, 0x07958, 0x0456B, 0x01246, 0x0D59B);
}

static void testRotated() {
	Quaternionx quaternion;
	
	quaternion = QUATERNIONx_IDENTITY;
	quaternion = Quaternionx_rotated(quaternion, VECTOR3x_UP, X_PI / 5);
	assertQuaternion(quaternion, 0x00000, 0x04F1B, 0x00000, 0x0F36C);
	
	quaternion = Quaternionx_rotated(quaternion, VECTOR3x(0x10000, 0x00000, 0x08000), 0x10000);
	assertQuaternion(quaternion, 0x07958, 0x0456B, 0x01246, 0x0D59B);
}

static void testInvert() {
	Quaternionx quaternion;
	
	quaternion = QUATERNIONx(0x00000, 0x10000, 0x00000, 0x00000);
	Quaternionx_invert(&quaternion);
	assertQuaternion(quaternion, 0x00000, -0x10000, 0x00000, 0x00000);
	
	quaternion = QUATERNIONx(-0x08000, 0x08000, 0x10000, 0x08000);
	Quaternionx_invert(&quaternion);
	assertQuaternion(quaternion, 0x04925, -0x04925, -0x09249, 0x04925);
	
	Quaternionx_invert(&quaternion);
	assertQuaternion(quaternion, -0x08001, 0x08001, 0x10000, 0x08001);
}

static void testInverted() {
	Quaternionx quaternion;
	
	quaternion = QUATERNIONx(0x00000, 0x10000, 0x00000, 0x00000);
	quaternion = Quaternionx_inverted(quaternion);
	assertQuaternion(quaternion, 0x00000, -0x10000, 0x00000, 0x00000);
	
	quaternion = QUATERNIONx(-0x08000, 0x08000, 0x10000, 0x08000);
	quaternion = Quaternionx_inverted(quaternion);
	assertQuaternion(quaternion, 0x04925, -0x04925, -0x09249, 0x04925);
	
	quaternion = Quaternionx_inverted(quaternion);
	assertQuaternion(quaternion, -0x08001, 0x08001, 0x10000, 0x08001);
}

static void testMultiplyVector3x() {
	Vector3x vector;
	
	vector = Quaternionx_multiplyVector3x(Quaternionx_fromAxisAngle(VECTOR3x_UP, X_PI_4),
	                                      VECTOR3x(0x10000, 0x10000, 0x00000));
	assertVector3x(vector, 0x0B506, 0x10000, -0x0B504);
	
	vector = Quaternionx_multiplyVector3x(Quaternionx_fromAxisAngle(VECTOR3x_RIGHT, X_PI_4),
	                                      VECTOR3x(-0x28000, 0x20000, 0x00000));
	assertVector3x(vector, -0x28000, 0x16A0B, 0x16A08);
	
	vector = Quaternionx_multiplyVector3x(Quaternionx_fromAxisAngle(VECTOR3x_FRONT, X_PI_4),
	                                      VECTOR3x(0x10000, 0x10000, 0x00000));
	assertVector3x(vector, 0x00002, 0x16A0A, 0x00000);
}

static void testMultiplyVector4x() {
	Vector4x vector;
	
	vector = Quaternionx_multiplyVector4x(Quaternionx_fromAxisAngle(VECTOR3x_UP, X_PI_4),
	                                      VECTOR4x(0x10000, 0x10000, 0x00000, 0x10000));
	assertVector4x(vector, 0x0B506, 0x10000, -0x0B504, 0x10000);
	
	vector = Quaternionx_multiplyVector4x(Quaternionx_fromAxisAngle(VECTOR3x_RIGHT, X_PI_4),
	                                      VECTOR4x(-0x28000, 0x20000, 0x00000, 0x00000));
	assertVector4x(vector, -0x28000, 0x16A0B, 0x16A08, 0x00000);
	
	vector = Quaternionx_multiplyVector4x(Quaternionx_fromAxisAngle(VECTOR3x_FRONT, X_PI_4),
	                                      VECTOR4x(0x10000, 0x10000, 0x00000, 0x00000));
	assertVector4x(vector, 0x00002, 0x16A0A, 0x00000, 0x00000);
}

TEST_SUITE(QuaternionxTest,
           testLoadIdentity,
           testIdentity,
           testInit,
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
           testMultiplyVector3x,
           testMultiplyVector4x)
