#include "unittest/TestSuite.h"
#include "gamemath/Matrix4x4x.h"
#include <math.h>

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

#define assertVector2x(vector, expectedX, expectedY) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY, "Expected {0x%05X, 0x%05X} but got {0x%05X, 0x%05X}", expectedX, expectedY, vector.x, vector.y);

#define assertVector3x(vector, expectedX, expectedY, expectedZ) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY && vector.z == expectedZ, "Expected {0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X}", expectedX, expectedY, expectedZ, vector.x, vector.y, vector.z);

#define assertVector4x(vector, expectedX, expectedY, expectedZ, expectedW) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY && vector.z == expectedZ && vector.w == expectedW, "Expected {0x%05X, 0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}", expectedX, expectedY, expectedZ, expectedW, vector.x, vector.y, vector.z, vector.w);

static void testInit() {
	Matrix4x4x matrix;
	
	matrix = MATRIX4x4x(0x10000, 0x00000, 0x00000, 0x00000,
	                    0x00000, 0x10000, 0x00000, 0x00000,
	                    0x00000, 0x00000, 0x10000, 0x00000,
	                    0x00000, 0x00000, 0x00000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = MATRIX4x4x(0x00000, 0x40000, 0x80000, 0xC0000,
	                    0x10000, 0x50000, 0x90000, 0xD0000,
	                    0x20000, 0x60000, 0xA0000, 0xE0000,
	                    0x30000, 0x70000, 0xB0000, 0xF0000);
	assertMatrix(matrix, 0x00000, 0x40000, 0x80000, 0xC0000,
	                     0x10000, 0x50000, 0x90000, 0xD0000,
	                     0x20000, 0x60000, 0xA0000, 0xE0000,
	                     0x30000, 0x70000, 0xB0000, 0xF0000);
	
	matrix = Matrix4x4x_fromDirectionVectors(VECTOR3x(0x10000, 0x00000, 0x00000),
	                                         VECTOR3x(0x00000, 0x10000, 0x00000),
	                                         VECTOR3x(0x00000, 0x00000, 0x10000));
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_fromDirectionVectors(VECTOR3x(0x00000, 0x10000, 0x00000),
	                                         VECTOR3x(0x00000, 0x00000, 0x10000),
	                                         VECTOR3x(0x10000, 0x00000, 0x00000));
	assertMatrix(matrix, 0x00000, 0x00000, 0x10000, 0x00000,
	                     0x10000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
}

static void testIdentity() {
	Matrix4x4x matrix;
	
	matrix = MATRIX4x4x_IDENTITY;
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	memset(matrix.m, sizeof(fixed16_16) * 16, 0);
	Matrix4x4x_loadIdentity(&matrix);
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
}

static void testMultiply() {
	Matrix4x4x matrix;
	
	matrix = Matrix4x4x_multiplied(MATRIX4x4x(0x20000, 0x00000, 0x00000, 0x00000,
	                                          0x00000, 0x20000, 0x00000, 0x00000,
	                                          0x00000, 0x00000, 0x20000, 0x00000,
	                                          0x00000, 0x00000, 0x00000, 0x20000),
	                               MATRIX4x4x(0x20000, 0x00000, 0x00000, 0x00000,
	                                          0x00000, 0x20000, 0x00000, 0x00000,
	                                          0x00000, 0x00000, 0x20000, 0x00000,
	                                          0x00000, 0x00000, 0x00000, 0x20000));
	assertMatrix(matrix, 0x40000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x40000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x40000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x40000);
	
	matrix = Matrix4x4x_multiplied(MATRIX4x4x(0x00000, 0x40000, 0x80000, 0xC0000,
	                                          0x10000, 0x50000, 0x90000, 0xD0000,
	                                          0x20000, 0x60000, 0xA0000, 0xE0000,
	                                          0x30000, 0x70000, 0xB0000, 0xF0000),
	                               MATRIX4x4x(0x10000, 0x70000,  0x130000, 0x250000,
	                                          0x20000, 0xB0000,  0x170000, 0x290000,
	                                          0x30000, 0xD0000,  0x1D0000, 0x2B0000,
	                                          0x50000, 0x110000, 0x1F0000, 0x2F0000));
	assertMatrix(matrix, 0x5C0000, 0x1600000, 0x2B80000, 0x4300000,
	                     0x670000, 0x1900000, 0x31E0000, 0x4D80000,
	                     0x720000, 0x1C00000, 0x3840000, 0x5800000,
	                     0x7D0000, 0x1F00000, 0x3EA0000, 0x6280000);
	
	matrix = MATRIX4x4x(0x20000, 0x00000, 0x00000, 0x00000,
	                    0x00000, 0x20000, 0x00000, 0x00000,
	                    0x00000, 0x00000, 0x20000, 0x00000,
	                    0x00000, 0x00000, 0x00000, 0x20000),
	Matrix4x4x_multiply(&matrix, MATRIX4x4x(0x20000, 0x00000, 0x00000, 0x00000,
	                                        0x00000, 0x20000, 0x00000, 0x00000,
	                                        0x00000, 0x00000, 0x20000, 0x00000,
	                                        0x00000, 0x00000, 0x00000, 0x20000));
	assertMatrix(matrix, 0x40000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x40000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x40000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x40000);
	
	matrix = MATRIX4x4x(0x00000, 0x40000, 0x80000,  0xC0000,
	                    0x10000, 0x50000, 0x90000,  0xD0000,
	                    0x20000, 0x60000, 0xA0000, 0xE0000,
	                    0x30000, 0x70000, 0xB0000, 0xF0000),
	Matrix4x4x_multiply(&matrix, MATRIX4x4x(0x10000, 0x70000,  0x130000, 0x250000,
	                                        0x20000, 0xB0000, 0x170000, 0x290000,
	                                        0x30000, 0xD0000, 0x1D0000, 0x2B0000,
	                                        0x50000, 0x110000, 0x1F0000, 0x2F0000));
	assertMatrix(matrix, 0x5C0000, 0x1600000, 0x2B80000, 0x4300000,
	                     0x670000, 0x1900000, 0x31E0000, 0x4D80000,
	                     0x720000, 0x1C00000, 0x3840000, 0x5800000,
	                     0x7D0000, 0x1F00000, 0x3EA0000, 0x6280000);
	
	matrix = MATRIX4x4x(0x10000, 0x70000,  0x130000, 0x250000,
	                    0x20000, 0xB0000, 0x170000, 0x290000,
	                    0x30000, 0xD0000, 0x1D0000, 0x2B0000,
	                    0x50000, 0x110000, 0x1F0000, 0x2F0000),
	Matrix4x4x_leftMultiply(&matrix, MATRIX4x4x(0x00000, 0x40000, 0x80000,  0xC0000,
	                                            0x10000, 0x50000, 0x90000,  0xD0000,
	                                            0x20000, 0x60000, 0xA0000, 0xE0000,
	                                            0x30000, 0x70000, 0xB0000, 0xF0000));
	assertMatrix(matrix, 0x5C0000, 0x1600000, 0x2B80000, 0x4300000,
	                     0x670000, 0x1900000, 0x31E0000, 0x4D80000,
	                     0x720000, 0x1C00000, 0x3840000, 0x5800000,
	                     0x7D0000, 0x1F00000, 0x3EA0000, 0x6280000);
}

static void testTranslate() {
	Matrix4x4x matrix;
	
	matrix = Matrix4x4x_translated(MATRIX4x4x_IDENTITY, 0x30000, -0x18000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x30000,
	                     0x00000, 0x10000, 0x00000, -0x18000,
	                     0x00000, 0x00000, 0x10000, 0x10000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_translated(Matrix4x4x_fromDirectionVectors(VECTOR3x(0x00000, 0x10000, 0x00000),
	                                                               VECTOR3x(0x00000, 0x00000, 0x10000),
	                                                               VECTOR3x(0x10000, 0x00000, 0x00000)),
	                               0x20000, 0x30000, -0x10000);
	assertMatrix(matrix, 0x00000, 0x00000, 0x10000, -0x10000,
	                     0x10000, 0x00000, 0x00000, 0x20000,
	                     0x00000, 0x10000, 0x00000, 0x30000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_translate(&matrix, 0x30000, -0x18000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000,  0x30000,
	                     0x00000, 0x10000, 0x00000, -0x18000,
	                     0x00000, 0x00000, 0x10000,  0x10000,
	                     0x00000, 0x00000, 0x00000,  0x10000);
	
	matrix = Matrix4x4x_fromDirectionVectors(VECTOR3x(0x00000, 0x10000, 0x00000),
	                                         VECTOR3x(0x00000, 0x00000, 0x10000),
	                                         VECTOR3x(0x10000, 0x00000, 0x00000));
	Matrix4x4x_translate(&matrix, 0x20000, 0x30000, -0x10000);
	assertMatrix(matrix, 0x00000, 0x00000, 0x10000, -0x10000,
	                     0x10000, 0x00000, 0x00000,  0x20000,
	                     0x00000, 0x10000, 0x00000,  0x30000,
	                     0x00000, 0x00000, 0x00000,  0x10000);
}

static void testScale() {
	Matrix4x4x matrix;
	
	matrix = Matrix4x4x_scaled(MATRIX4x4x_IDENTITY, 0x20000, -0x10000, 0x08000);
	assertMatrix(matrix, 0x20000,  0x00000, 0x00000, 0x00000,
	                     0x00000, -0x10000, 0x00000, 0x00000,
	                     0x00000,  0x00000, 0x08000, 0x00000,
	                     0x00000,  0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_scaled(MATRIX4x4x(0x00000, 0x00000, -0x10000, 0x10000,
	                                      0x20000, 0x00000,  0x00000, 0x20000,
	                                      0x00000, 0x18000,  0x00000, 0x30000,
	                                      0x00000, 0x00000,  0x00000, 0x10000),
	                           0x30000, 0x18000, -0x08000);
	assertMatrix(matrix, 0x00000, 0x00000, 0x08000, 0x10000,
	                     0x60000, 0x00000, 0x00000, 0x20000,
	                     0x00000, 0x24000, 0x00000, 0x30000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_scale(&matrix, 0x20000, -0x10000, 0x08000);
	assertMatrix(matrix, 0x20000,  0x00000, 0x00000, 0x00000,
	                     0x00000, -0x10000, 0x00000, 0x00000,
	                     0x00000,  0x00000, 0x08000, 0x00000,
	                     0x00000,  0x00000, 0x00000, 0x10000);
	
	matrix = MATRIX4x4x(0x00000, 0x00000, -0x10000, 0x10000,
	                    0x20000, 0x00000,  0x00000, 0x20000,
	                    0x00000, 0x18000,  0x00000, 0x30000,
	                    0x00000, 0x00000,  0x00000, 0x10000);
	Matrix4x4x_scale(&matrix, 0x30000, 0x18000, -0x08000);
	assertMatrix(matrix, 0x00000, 0x00000, 0x08000, 0x10000,
	                     0x60000, 0x00000, 0x00000, 0x20000,
	                     0x00000, 0x24000, 0x00000, 0x30000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
}

static void testRotate() {
	Matrix4x4x matrix;
	
	matrix = Matrix4x4x_rotated(MATRIX4x4x_IDENTITY, VECTOR3x(0x00000, 0x10000, 0x00000), X_PI);
	assertMatrix(matrix, -0x10000, 0x00000,  0x00000, 0x00000,
	                      0x00000, 0x10000,  0x00000, 0x00000,
	                      0x00000, 0x00000, -0x10000, 0x00000,
	                      0x00000, 0x00000,  0x00000, 0x10000);
	
	matrix = Matrix4x4x_rotated(Matrix4x4x_scaled(MATRIX4x4x_IDENTITY, 0x20000, 0x20000, 0x20000),
	                            VECTOR3x(-0x10000, 0x00000, 0x00000),
	                            xmul(X_PI, 0x08000));
	assertMatrix(matrix, 0x20000,  0x00000, 0x00000, 0x00000,
	                     0x00000,  0x00000, 0x1FF9C, 0x00000,
	                     0x00000, -0x1FF9C, 0x00000, 0x00000,
	                     0x00000,  0x00000, 0x00000, 0x10000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_rotate(&matrix, VECTOR3x(0x00000, 0x10000, 0x00000), X_PI);
	assertMatrix(matrix, -0x10000, 0x00000,  0x00000, 0x00000,
	                      0x00000, 0x10000,  0x00000, 0x00000,
	                      0x00000, 0x00000, -0x10000, 0x00000,
	                      0x00000, 0x00000,  0x00000, 0x10000);
	
	matrix = Matrix4x4x_scaled(MATRIX4x4x_IDENTITY, 0x20000, 0x20000, 0x20000);
	Matrix4x4x_rotate(&matrix, VECTOR3x(-0x10000, 0x00000, 0x00000), xmul(X_PI, 0x08000));
	assertMatrix(matrix, 0x20000,  0x00000, 0x00000, 0x00000,
	                     0x00000,  0x00000, 0x1FF9C, 0x00000,
	                     0x00000, -0x1FF9C, 0x00000, 0x00000,
	                     0x00000,  0x00000, 0x00000, 0x10000);
}

static void testShear() {
	Matrix4x4x matrix;
	
	matrix = Matrix4x4x_shearedX(MATRIX4x4x_IDENTITY, 0x10000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x00000,
	                     0x10000, 0x10000, 0x00000, 0x00000,
	                     0x10000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_shearedX(Matrix4x4x_fromDirectionVectors(VECTOR3x(0x00000, 0x10000, 0x00000),
	                                                             VECTOR3x(0x00000, 0x00000, 0x10000),
	                                                             VECTOR3x(0x10000, 0x00000, 0x00000)),
	                             0x08000, -0x08000);
	assertMatrix(matrix, -0x08000, 0x00000, 0x10000, 0x00000,
	                      0x10000, 0x00000, 0x00000, 0x00000,
	                      0x08000, 0x10000, 0x00000, 0x00000,
	                      0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_shearedY(MATRIX4x4x_IDENTITY, 0x10000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_shearedY(Matrix4x4x_fromDirectionVectors(VECTOR3x(0x00000, 0x10000, 0x00000),
	                                                             VECTOR3x(0x00000, 0x00000, 0x10000),
	                                                             VECTOR3x(0x10000, 0x00000, 0x00000)),
	                             0x08000, -0x08000);
	assertMatrix(matrix, 0x00000, -0x08000, 0x10000, 0x00000,
	                     0x10000,  0x08000, 0x00000, 0x00000,
	                     0x00000,  0x10000, 0x00000, 0x00000,
	                     0x00000,  0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_shearedZ(MATRIX4x4x_IDENTITY, 0x10000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x10000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_shearedZ(Matrix4x4x_fromDirectionVectors(VECTOR3x(0x00000, 0x10000, 0x00000),
	                                                             VECTOR3x(0x00000, 0x00000, 0x10000),
	                                                             VECTOR3x(0x10000, 0x00000, 0x00000)),
	                             0x08000, -0x08000);
	assertMatrix(matrix, 0x00000, 0x00000,  0x10000, 0x00000,
	                     0x10000, 0x00000,  0x08000, 0x00000,
	                     0x00000, 0x10000, -0x08000, 0x00000,
	                     0x00000, 0x00000,  0x00000, 0x10000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_shearX(&matrix, 0x10000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x00000,
	                     0x10000, 0x10000, 0x00000, 0x00000,
	                     0x10000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_fromDirectionVectors(VECTOR3x(0x00000, 0x10000, 0x00000),
	                                         VECTOR3x(0x00000, 0x00000, 0x10000),
	                                         VECTOR3x(0x10000, 0x00000, 0x00000));
	Matrix4x4x_shearX(&matrix, 0x08000, -0x08000);
	assertMatrix(matrix, -0x08000, 0x00000, 0x10000, 0x00000,
	                      0x10000, 0x00000, 0x00000, 0x00000,
	                      0x08000, 0x10000, 0x00000, 0x00000,
	                      0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_shearY(&matrix, 0x10000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_fromDirectionVectors(VECTOR3x(0x00000, 0x10000, 0x00000),
	                                         VECTOR3x(0x00000, 0x00000, 0x10000),
	                                         VECTOR3x(0x10000, 0x00000, 0x00000));
	Matrix4x4x_shearY(&matrix, 0x08000, -0x08000);
	assertMatrix(matrix, 0x00000, -0x08000, 0x10000, 0x00000,
	                     0x10000,  0x08000, 0x00000, 0x00000,
	                     0x00000,  0x10000, 0x00000, 0x00000,
	                     0x00000,  0x00000, 0x00000, 0x10000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_shearZ(&matrix, 0x10000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x10000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_fromDirectionVectors(VECTOR3x(0x00000, 0x10000, 0x00000),
	                                     VECTOR3x(0x00000, 0x00000, 0x10000),
	                                     VECTOR3x(0x10000, 0x00000, 0x00000));
	Matrix4x4x_shearZ(&matrix, 0x08000, -0x08000);
	assertMatrix(matrix, 0x00000, 0x00000,  0x10000, 0x00000,
	                     0x10000, 0x00000,  0x08000, 0x00000,
	                     0x00000, 0x10000, -0x08000, 0x00000,
	                     0x00000, 0x00000,  0x00000, 0x10000);
}

static void testPerspective() {
	Matrix4x4x matrix;
	
	matrix = Matrix4x4x_perspective(MATRIX4x4x_IDENTITY, 0x5A0000, 0x10000, 0x10000, 0x20000);
	assertMatrix(matrix, 0x10001, 0x00000,  0x00000,  0x00000,
	                     0x00000, 0x10001,  0x00000,  0x00000,
	                     0x00000, 0x00000, -0x30000, -0x40000,
	                     0x00000, 0x00000, -0x10000,  0x00000);
	
	matrix = Matrix4x4x_perspective(MATRIX4x4x_IDENTITY, 0x2D0000, 0x20000, 0x08000, 0x40000);
	assertMatrix(matrix, 0x13506, 0x00000,  0x00000,  0x00000,
	                     0x00000, 0x26A0C,  0x00000,  0x00000,
	                     0x00000, 0x00000, -0x14925, -0x12492,
	                     0x00000, 0x00000, -0x10000,  0x00000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_applyPerspective(&matrix, 0x5A0000, 0x10000, 0x10000, 0x20000);
	assertMatrix(matrix, 0x10001, 0x00000,  0x00000,  0x00000,
	                     0x00000, 0x10001,  0x00000,  0x00000,
	                     0x00000, 0x00000, -0x30000, -0x40000,
	                     0x00000, 0x00000, -0x10000,  0x00000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_applyPerspective(&matrix, 0x2D0000, 0x20000, 0x08000, 0x40000);
	assertMatrix(matrix, 0x13506, 0x00000,  0x00000,  0x00000,
	                     0x00000, 0x26A0C,  0x00000,  0x00000,
	                     0x00000, 0x00000, -0x14925, -0x12492,
	                     0x00000, 0x00000, -0x10000,  0x00000);
}

static void testOrtho() {
	Matrix4x4x matrix;
	
	matrix = Matrix4x4x_ortho(MATRIX4x4x_IDENTITY, -0x10000, 0x10000, -0x10000, 0x10000, -0x10000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x00000,  0x00000, 0x00000,
	                     0x00000, 0x10000,  0x00000, 0x00000,
	                     0x00000, 0x00000, -0x10000, 0x00000,
	                     0x00000, 0x00000,  0x00000, 0x10000);
	
	matrix = Matrix4x4x_ortho(MATRIX4x4x_IDENTITY, 0x00000, 0x40000, 0x80000, 0x00000, 0x00000, 0xA0000);
	assertMatrix(matrix, 0x08000,  0x00000,  0x00000, -0x10000,
	                     0x00000, -0x04000,  0x00000,  0x10000,
	                     0x00000,  0x00000, -0x03333, -0x10000,
	                     0x00000,  0x00000,  0x00000,  0x10000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_applyOrtho(&matrix, -0x10000, 0x10000, -0x10000, 0x10000, -0x10000, 0x10000);
	assertMatrix(matrix, 0x10000, 0x00000,  0x00000, 0x00000,
	                     0x00000, 0x10000,  0x00000, 0x00000,
	                     0x00000, 0x00000, -0x10000, 0x00000,
	                     0x00000, 0x00000,  0x00000, 0x10000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_applyOrtho(&matrix, 0x00000, 0x40000, 0x80000, 0x00000, 0x00000, 0xA0000);
	assertMatrix(matrix, 0x08000,  0x00000,  0x00000, -0x10000,
	                     0x00000, -0x04000,  0x00000,  0x10000,
	                     0x00000,  0x00000, -0x03333, -0x10000,
	                     0x00000,  0x00000,  0x00000,  0x10000);
}

static void testTranspose() {
	Matrix4x4x matrix;
	
	matrix = Matrix4x4x_transposed(MATRIX4x4x(0x10000, 0x00000, 0x00000, 0x00000,
	                                          0x10000, 0x10000, 0x00000, 0x00000,
	                                          0x10000, 0x10000, 0x10000, 0x00000,
	                                          0x10000, 0x10000, 0x10000, 0x10000));
	assertMatrix(matrix, 0x10000, 0x10000, 0x10000, 0x10000,
	                     0x00000, 0x10000, 0x10000, 0x10000,
	                     0x00000, 0x00000, 0x10000, 0x10000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_transposed(MATRIX4x4x(0x00000, 0x06666, 0x0CCCC, 0x13333,
	                                          0x01999, 0x08000, 0x0E666, 0x14CCC,
	                                          0x03333, 0x09999, 0x10000, 0x16666,
	                                          0x04CCC, 0x0B333, 0x11999, 0x18000));
	assertMatrix(matrix, 0x00000, 0x01999, 0x03333, 0x04CCC,
	                     0x06666, 0x08000, 0x09999, 0x0B333,
	                     0x0CCCC, 0x0E666, 0x10000, 0x11999,
	                     0x13333, 0x14CCC, 0x16666, 0x18000);
	
	matrix = MATRIX4x4x(0x10000, 0x00000, 0x00000, 0x00000,
	                    0x10000, 0x10000, 0x00000, 0x00000,
	                    0x10000, 0x10000, 0x10000, 0x00000,
	                    0x10000, 0x10000, 0x10000, 0x10000);
	Matrix4x4x_transpose(&matrix);
	assertMatrix(matrix, 0x10000, 0x10000, 0x10000, 0x10000,
	                     0x00000, 0x10000, 0x10000, 0x10000,
	                     0x00000, 0x00000, 0x10000, 0x10000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = MATRIX4x4x(0x00000, 0x06666, 0x0CCCC, 0x13333,
	                    0x01999, 0x08000, 0x0E666, 0x14CCC,
	                    0x03333, 0x09999, 0x10000, 0x16666,
	                    0x04CCC, 0x0B333, 0x11999, 0x18000);
	Matrix4x4x_transpose(&matrix);
	assertMatrix(matrix, 0x00000, 0x01999, 0x03333, 0x04CCC,
	                     0x06666, 0x08000, 0x09999, 0x0B333,
	                     0x0CCCC, 0x0E666, 0x10000, 0x11999,
	                     0x13333, 0x14CCC, 0x16666, 0x18000);
}

static void testDeterminant() {
	fixed16_16 determinant;
	
	determinant = Matrix4x4x_determinant(MATRIX4x4x_IDENTITY);
	TestCase_assert(determinant == 0x10000, "Expected 1.0 but got 0x%05X", determinant);
	
	determinant = Matrix4x4x_determinant(MATRIX4x4x(0x00000, 0x00000, 0x20000, 0x20000,
	                                                0x20000, 0x00000, 0x00000, 0x30000,
	                                                0x00000, 0x20000, 0x00000, 0x10000,
	                                                0x00000, 0x00000, 0x00000, 0x10000));
	TestCase_assert(determinant == 0x80000, "Expected 8.0 but got 0x%05X", determinant);
}

static void testInvert() {
	Matrix4x4x matrix;
	
	matrix = Matrix4x4x_inverted(MATRIX4x4x_IDENTITY);
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = Matrix4x4x_inverted(MATRIX4x4x(0x00000, 0x00000, 0x20000, 0x20000,
	                                        0x20000, 0x00000, 0x00000, 0x30000,
	                                        0x00000, 0x20000, 0x00000, 0x10000,
	                                        0x00000, 0x00000, 0x00000, 0x10000));
	assertMatrix(matrix, 0x00000, 0x08000, 0x00000, -0x18000,
	                     0x00000, 0x00000, 0x08000, -0x08000,
	                     0x08000, 0x00000, 0x00000, -0x10000,
	                     0x00000, 0x00000, 0x00000,  0x10000);
	
	matrix = MATRIX4x4x_IDENTITY;
	Matrix4x4x_invert(&matrix);
	assertMatrix(matrix, 0x10000, 0x00000, 0x00000, 0x00000,
	                     0x00000, 0x10000, 0x00000, 0x00000,
	                     0x00000, 0x00000, 0x10000, 0x00000,
	                     0x00000, 0x00000, 0x00000, 0x10000);
	
	matrix = MATRIX4x4x(0x00000, 0x00000, 0x20000, 0x20000,
	                    0x20000, 0x00000, 0x00000, 0x30000,
	                    0x00000, 0x20000, 0x00000, 0x10000,
	                    0x00000, 0x00000, 0x00000, 0x10000);
	Matrix4x4x_invert(&matrix);
	assertMatrix(matrix, 0x00000, 0x08000, 0x00000, -0x18000,
	                     0x00000, 0x00000, 0x08000, -0x08000,
	                     0x08000, 0x00000, 0x00000, -0x10000,
	                     0x00000, 0x00000, 0x00000,  0x10000);
}

static void testInterpolate() {
	Matrix4x4x matrixLeft, matrixRight, matrixInterpolated;
	
	matrixLeft = MATRIX4x4x_IDENTITY;
	matrixRight = MATRIX4x4x(0x00000, 0x10000, 0x10000, 0x10000,
	                         0x10000, 0x00000, 0x10000, 0x10000,
	                         0x10000, 0x10000, 0x00000, 0x10000,
	                         0x10000, 0x10000, 0x10000, 0x00000);
	
	matrixInterpolated = Matrix4x4x_interpolated(matrixLeft, matrixRight, 0x00000);
	assertMatrix(matrixInterpolated, 0x10000, 0x00000, 0x00000, 0x00000,
	                                 0x00000, 0x10000, 0x00000, 0x00000,
	                                 0x00000, 0x00000, 0x10000, 0x00000,
	                                 0x00000, 0x00000, 0x00000, 0x10000);
	
	matrixInterpolated = Matrix4x4x_interpolated(matrixLeft, matrixRight, 0x04000);
	assertMatrix(matrixInterpolated, 0x0C000, 0x04000, 0x04000, 0x04000,
	                                 0x04000, 0x0C000, 0x04000, 0x04000,
	                                 0x04000, 0x04000, 0x0C000, 0x04000,
	                                 0x04000, 0x04000, 0x04000, 0x0C000);
	
	matrixInterpolated = matrixLeft;
	Matrix4x4x_interpolate(&matrixInterpolated, matrixRight, 0x00000);
	assertMatrix(matrixInterpolated, 0x10000, 0x00000, 0x00000, 0x00000,
	                                 0x00000, 0x10000, 0x00000, 0x00000,
	                                 0x00000, 0x00000, 0x10000, 0x00000,
	                                 0x00000, 0x00000, 0x00000, 0x10000);
	
	matrixInterpolated = matrixLeft;
	Matrix4x4x_interpolate(&matrixInterpolated, matrixRight, 0x04000);
	assertMatrix(matrixInterpolated, 0x0C000, 0x04000, 0x04000, 0x04000,
	                                 0x04000, 0x0C000, 0x04000, 0x04000,
	                                 0x04000, 0x04000, 0x0C000, 0x04000,
	                                 0x04000, 0x04000, 0x04000, 0x0C000);
	
	matrixLeft = MATRIX4x4x(0x00000, 0x40000, 0x80000, 0xC0000,
	                        0x10000, 0x50000, 0x90000, 0xD0000,
	                        0x20000, 0x60000, 0xA0000, 0xE0000,
	                        0x30000, 0x70000, 0xB0000, 0xF0000);
	matrixRight = MATRIX4x4x_IDENTITY;
	
	matrixInterpolated = Matrix4x4x_interpolated(matrixLeft, matrixRight, -0x10000);
	assertMatrix(matrixInterpolated, -0x10000, 0x80000, 0x100000, 0x180000,
	                                  0x20000, 0x90000, 0x120000, 0x1A0000,
	                                  0x40000, 0xC0000, 0x130000, 0x1C0000,
	                                  0x60000, 0xE0000, 0x160000, 0x1D0000);
	
	matrixInterpolated = Matrix4x4x_interpolated(matrixLeft, matrixRight, 0x20000);
	assertMatrix(matrixInterpolated,  0x20000, -0x40000, -0x80000, -0xC0000,
	                                 -0x10000, -0x30000, -0x90000, -0xD0000,
	                                 -0x20000, -0x60000, -0x80000, -0xE0000,
	                                 -0x30000, -0x70000, -0xB0000, -0xD0000);
	
	matrixInterpolated = matrixLeft;
	Matrix4x4x_interpolate(&matrixInterpolated, matrixRight, -0x10000);
	assertMatrix(matrixInterpolated, -0x10000, 0x80000, 0x100000, 0x180000,
	                                  0x20000, 0x90000, 0x120000, 0x1A0000,
	                                  0x40000, 0xC0000, 0x130000, 0x1C0000,
	                                  0x60000, 0xE0000, 0x160000, 0x1D0000);
	
	matrixInterpolated = matrixLeft;
	Matrix4x4x_interpolate(&matrixInterpolated, matrixRight, 0x20000);
	assertMatrix(matrixInterpolated,  0x20000, -0x40000, -0x80000, -0xC0000,
	                                 -0x10000, -0x30000, -0x90000, -0xD0000,
	                                 -0x20000, -0x60000, -0x80000, -0xE0000,
	                                 -0x30000, -0x70000, -0xB0000, -0xD0000);
}

static void testMultiplyVector() {
	Matrix4x4x matrix;
	Vector2x vector2;
	Vector3x vector3;
	Vector4x vector4;
	
	matrix = MATRIX4x4x_IDENTITY;
	vector2 = Matrix4x4x_multiplyVector2x(matrix, VECTOR2x(0x10000, 0x00000));
	assertVector2x(vector2, 0x10000, 0x00000);
	
	vector3 = Matrix4x4x_multiplyVector3x(matrix, VECTOR3x(0x00000, 0x10000, 0x00000));
	assertVector3x(vector3, 0x00000, 0x10000, 0x00000);
	
	vector3 = Matrix4x4x_multiplyVector3x_rotationOnly(matrix, VECTOR3x(0x00000, 0x10000, 0x00000));
	assertVector3x(vector3, 0x00000, 0x10000, 0x00000);
	
	vector4 = Matrix4x4x_multiplyVector4x(matrix, VECTOR4x(0x00000, 0x00000, 0x10000, 0x00000));
	assertVector4x(vector4, 0x00000, 0x00000, 0x10000, 0x00000);
	
	matrix = MATRIX4x4x(0x00000, 0x20000,  0x00000, -0x10000,
	                    0x20000, 0x00000,  0x00000,  0x10000,
	                    0x00000, 0x00000, -0x20000,  0x20000,
	                    0x00000, 0x00000,  0x00000,  0x10000);
	vector2 = Matrix4x4x_multiplyVector2x(matrix, VECTOR2x(-0x10000, 0x00000));
	assertVector2x(vector2, -0x10000, -0x10000);
	
	vector3 = Matrix4x4x_multiplyVector3x(matrix, VECTOR3x(0x00000, -0x10000, 0x00000));
	assertVector3x(vector3, -0x30000, 0x10000, 0x20000);
	
	vector3 = Matrix4x4x_multiplyVector3x_rotationOnly(matrix, VECTOR3x(0x00000, -0x10000, 0x00000));
	assertVector3x(vector3, -0x20000, 0x00000, 0x00000);
	
	vector4 = Matrix4x4x_multiplyVector4x(matrix, VECTOR4x(0x00000, 0x00000, -0x10000, 0x10000));
	assertVector4x(vector4, -0x10000, 0x10000, 0x40000, 0x10000);
}

TEST_SUITE(Matrix4x4xTest,
           testInit,
           testIdentity,
           testMultiply,
           testTranslate,
           testScale,
           testRotate,
           testShear,
           testPerspective,
           testOrtho,
           testTranspose,
           testDeterminant,
           testInvert,
           testInterpolate,
           testMultiplyVector)
