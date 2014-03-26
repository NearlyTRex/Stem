#include "unittest/TestSuite.h"
#include "3dmath/Matrix.h"
#include <math.h>

#define assertMatrixExact(matrix, expected0, expected4, expected8,  expected12, \
                                  expected1, expected5, expected9,  expected13, \
                                  expected2, expected6, expected10, expected14, \
                                  expected3, expected7, expected11, expected15) { \
	TestCase_assert(matrix.m[0]  == expected0,  "Expected %f [0] but got %f",  expected0,  matrix.m[0]); \
	TestCase_assert(matrix.m[1]  == expected1,  "Expected %f [1] but got %f",  expected1,  matrix.m[1]); \
	TestCase_assert(matrix.m[2]  == expected2,  "Expected %f [2] but got %f",  expected2,  matrix.m[2]); \
	TestCase_assert(matrix.m[3]  == expected3,  "Expected %f [3] but got %f",  expected3,  matrix.m[3]); \
	TestCase_assert(matrix.m[4]  == expected4,  "Expected %f [4] but got %f",  expected4,  matrix.m[4]); \
	TestCase_assert(matrix.m[5]  == expected5,  "Expected %f [5] but got %f",  expected5,  matrix.m[5]); \
	TestCase_assert(matrix.m[6]  == expected6,  "Expected %f [6] but got %f",  expected6,  matrix.m[6]); \
	TestCase_assert(matrix.m[7]  == expected7,  "Expected %f [7] but got %f",  expected7,  matrix.m[7]); \
	TestCase_assert(matrix.m[8]  == expected8,  "Expected %f [8] but got %f",  expected8,  matrix.m[8]); \
	TestCase_assert(matrix.m[9]  == expected9,  "Expected %f [9] but got %f",  expected9,  matrix.m[9]); \
	TestCase_assert(matrix.m[10] == expected10, "Expected %f [10] but got %f", expected10, matrix.m[10]); \
	TestCase_assert(matrix.m[11] == expected11, "Expected %f [11] but got %f", expected11, matrix.m[11]); \
	TestCase_assert(matrix.m[12] == expected12, "Expected %f [12] but got %f", expected12, matrix.m[12]); \
	TestCase_assert(matrix.m[13] == expected13, "Expected %f [13] but got %f", expected13, matrix.m[13]); \
	TestCase_assert(matrix.m[14] == expected14, "Expected %f [14] but got %f", expected14, matrix.m[14]); \
	TestCase_assert(matrix.m[15] == expected15, "Expected %f [15] but got %f", expected15, matrix.m[15]); \
}

#define assertMatrixApproximate(matrix, expected0, expected4, expected8,  expected12, \
                                        expected1, expected5, expected9,  expected13, \
                                        expected2, expected6, expected10, expected14, \
                                        expected3, expected7, expected11, expected15, epsilon) { \
	TestCase_assert(fabs(matrix.m[0]  - expected0)  < epsilon, "Expected %f [0] but got %f (tolerance %f)",  expected0,  matrix.m[0],  epsilon); \
	TestCase_assert(fabs(matrix.m[1]  - expected1)  < epsilon, "Expected %f [1] but got %f (tolerance %f)",  expected1,  matrix.m[1],  epsilon); \
	TestCase_assert(fabs(matrix.m[2]  - expected2)  < epsilon, "Expected %f [2] but got %f (tolerance %f)",  expected2,  matrix.m[2],  epsilon); \
	TestCase_assert(fabs(matrix.m[3]  - expected3)  < epsilon, "Expected %f [3] but got %f (tolerance %f)",  expected3,  matrix.m[3],  epsilon); \
	TestCase_assert(fabs(matrix.m[4]  - expected4)  < epsilon, "Expected %f [4] but got %f (tolerance %f)",  expected4,  matrix.m[4],  epsilon); \
	TestCase_assert(fabs(matrix.m[5]  - expected5)  < epsilon, "Expected %f [5] but got %f (tolerance %f)",  expected5,  matrix.m[5],  epsilon); \
	TestCase_assert(fabs(matrix.m[6]  - expected6)  < epsilon, "Expected %f [6] but got %f (tolerance %f)",  expected6,  matrix.m[6],  epsilon); \
	TestCase_assert(fabs(matrix.m[7]  - expected7)  < epsilon, "Expected %f [7] but got %f (tolerance %f)",  expected7,  matrix.m[7],  epsilon); \
	TestCase_assert(fabs(matrix.m[8]  - expected8)  < epsilon, "Expected %f [8] but got %f (tolerance %f)",  expected8,  matrix.m[8],  epsilon); \
	TestCase_assert(fabs(matrix.m[9]  - expected9)  < epsilon, "Expected %f [9] but got %f (tolerance %f)",  expected9,  matrix.m[9],  epsilon); \
	TestCase_assert(fabs(matrix.m[10] - expected10) < epsilon, "Expected %f [10] but got %f (tolerance %f)", expected10, matrix.m[10], epsilon); \
	TestCase_assert(fabs(matrix.m[11] - expected11) < epsilon, "Expected %f [11] but got %f (tolerance %f)", expected11, matrix.m[11], epsilon); \
	TestCase_assert(fabs(matrix.m[12] - expected12) < epsilon, "Expected %f [12] but got %f (tolerance %f)", expected12, matrix.m[12], epsilon); \
	TestCase_assert(fabs(matrix.m[13] - expected13) < epsilon, "Expected %f [13] but got %f (tolerance %f)", expected13, matrix.m[13], epsilon); \
	TestCase_assert(fabs(matrix.m[14] - expected14) < epsilon, "Expected %f [14] but got %f (tolerance %f)", expected14, matrix.m[14], epsilon); \
	TestCase_assert(fabs(matrix.m[15] - expected15) < epsilon, "Expected %f [15] but got %f (tolerance %f)", expected15, matrix.m[15], epsilon); \
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
	Matrix matrix;
	
	matrix = MATRIX(1.0f, 0.0f, 0.0f, 0.0f,
	                0.0f, 1.0f, 0.0f, 0.0f,
	                0.0f, 0.0f, 1.0f, 0.0f,
	                0.0f, 0.0f, 0.0f, 1.0f);
	assertMatrixExact(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                          0.0f, 1.0f, 0.0f, 0.0f,
	                          0.0f, 0.0f, 1.0f, 0.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
	
	matrix = MATRIX(0.0f, 4.0f, 8.0f,  12.0f,
	                1.0f, 5.0f, 9.0f,  13.0f,
	                2.0f, 6.0f, 10.0f, 14.0f,
	                3.0f, 7.0f, 11.0f, 15.0f);
	assertMatrixExact(matrix, 0.0f, 4.0f, 8.0f,  12.0f,
	                          1.0f, 5.0f, 9.0f,  13.0f,
	                          2.0f, 6.0f, 10.0f, 14.0f,
	                          3.0f, 7.0f, 11.0f, 15.0f);
	
	matrix = Matrix_fromDirectionVectors(VECTOR3f(1.0f, 0.0f, 0.0f),
	                                     VECTOR3f(0.0f, 1.0f, 0.0f),
	                                     VECTOR3f(0.0f, 0.0f, 1.0f));
	assertMatrixExact(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                          0.0f, 1.0f, 0.0f, 0.0f,
	                          0.0f, 0.0f, 1.0f, 0.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
	
	matrix = Matrix_fromDirectionVectors(VECTOR3f(0.0f, 1.0f, 0.0f),
	                                     VECTOR3f(0.0f, 0.0f, 1.0f),
	                                     VECTOR3f(1.0f, 0.0f, 0.0f));
	assertMatrixExact(matrix, 0.0f, 0.0f, 1.0f, 0.0f,
	                          1.0f, 0.0f, 0.0f, 0.0f,
	                          0.0f, 1.0f, 0.0f, 0.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
}

static void testIdentity() {
	Matrix matrix;
	
	matrix = MATRIX_IDENTITY;
	assertMatrixExact(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                          0.0f, 1.0f, 0.0f, 0.0f,
	                          0.0f, 0.0f, 1.0f, 0.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
	
	memset(matrix.m, sizeof(float) * 16, 0);
	Matrix_loadIdentity(&matrix);
	assertMatrixExact(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                          0.0f, 1.0f, 0.0f, 0.0f,
	                          0.0f, 0.0f, 1.0f, 0.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
}

static void testMultiply() {
	Matrix matrix;
	
	matrix = Matrix_multiplied(MATRIX(2.0f, 0.0f, 0.0f, 0.0f,
	                                  0.0f, 2.0f, 0.0f, 0.0f,
	                                  0.0f, 0.0f, 2.0f, 0.0f,
	                                  0.0f, 0.0f, 0.0f, 2.0f),
	                           MATRIX(2.0f, 0.0f, 0.0f, 0.0f,
	                                  0.0f, 2.0f, 0.0f, 0.0f,
	                                  0.0f, 0.0f, 2.0f, 0.0f,
	                                  0.0f, 0.0f, 0.0f, 2.0f));
	assertMatrixApproximate(matrix, 4.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 4.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 4.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 4.0f, EPSILON);
	
	matrix = Matrix_multiplied(MATRIX(0.0f, 4.0f, 8.0f,  12.0f,
	                                  1.0f, 5.0f, 9.0f,  13.0f,
	                                  2.0f, 6.0f, 10.0f, 14.0f,
	                                  3.0f, 7.0f, 11.0f, 15.0f),
	                           MATRIX(1.0f, 7.0f,  19.0f, 37.0f,
	                                  2.0f, 11.0f, 23.0f, 41.0f,
	                                  3.0f, 13.0f, 29.0f, 43.0f,
	                                  5.0f, 17.0f, 31.0f, 47.0f));
	assertMatrixApproximate(matrix, 92.0f,  352.0f, 696.0f,  1072.0f,
	                                103.0f, 400.0f, 798.0f,  1240.0f,
	                                114.0f, 448.0f, 900.0f,  1408.0f,
	                                125.0f, 496.0f, 1002.0f, 1576.0f, EPSILON);
	
	matrix = MATRIX(2.0f, 0.0f, 0.0f, 0.0f,
	                0.0f, 2.0f, 0.0f, 0.0f,
	                0.0f, 0.0f, 2.0f, 0.0f,
	                0.0f, 0.0f, 0.0f, 2.0f),
	Matrix_multiply(&matrix, MATRIX(2.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 2.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 2.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 2.0f));
	assertMatrixApproximate(matrix, 4.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 4.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 4.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 4.0f, EPSILON);
	
	matrix = MATRIX(0.0f, 4.0f, 8.0f,  12.0f,
	                1.0f, 5.0f, 9.0f,  13.0f,
	                2.0f, 6.0f, 10.0f, 14.0f,
	                3.0f, 7.0f, 11.0f, 15.0f),
	Matrix_multiply(&matrix, MATRIX(1.0f, 7.0f,  19.0f, 37.0f,
	                                2.0f, 11.0f, 23.0f, 41.0f,
	                                3.0f, 13.0f, 29.0f, 43.0f,
	                                5.0f, 17.0f, 31.0f, 47.0f));
	assertMatrixApproximate(matrix, 92.0f,  352.0f, 696.0f,  1072.0f,
	                                103.0f, 400.0f, 798.0f,  1240.0f,
	                                114.0f, 448.0f, 900.0f,  1408.0f,
	                                125.0f, 496.0f, 1002.0f, 1576.0f, EPSILON);
}

static void testTranslate() {
	Matrix matrix;
	
	matrix = Matrix_translated(MATRIX_IDENTITY, 3.0f, -1.5f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 3.0f,
	                                0.0f, 1.0f, 0.0f, -1.5f,
	                                0.0f, 0.0f, 1.0f, 1.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_translated(Matrix_fromDirectionVectors(VECTOR3f(0.0f, 1.0f, 0.0f),
	                                                       VECTOR3f(0.0f, 0.0f, 1.0f),
	                                                       VECTOR3f(1.0f, 0.0f, 0.0f)),
	                           2.0f, 3.0f, -1.0f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f, 1.0f, -1.0f,
	                                1.0f, 0.0f, 0.0f, 2.0f,
	                                0.0f, 1.0f, 0.0f, 3.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_translate(&matrix, 3.0f, -1.5f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 3.0f,
	                                0.0f, 1.0f, 0.0f, -1.5f,
	                                0.0f, 0.0f, 1.0f, 1.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_fromDirectionVectors(VECTOR3f(0.0f, 1.0f, 0.0f),
	                                     VECTOR3f(0.0f, 0.0f, 1.0f),
	                                     VECTOR3f(1.0f, 0.0f, 0.0f));
	Matrix_translate(&matrix, 2.0f, 3.0f, -1.0f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f, 1.0f, -1.0f,
	                                1.0f, 0.0f, 0.0f, 2.0f,
	                                0.0f, 1.0f, 0.0f, 3.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
}

static void testScale() {
	Matrix matrix;
	
	matrix = Matrix_scaled(MATRIX_IDENTITY, 2.0f, -1.0f, 0.5f);
	assertMatrixApproximate(matrix, 2.0f, 0.0f,  0.0f, 0.0f,
	                                0.0f, -1.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f,  0.5f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_scaled(MATRIX(0.0f, 0.0f, -1.0f, 1.0f,
	                              2.0f, 0.0f,  0.0f, 2.0f,
	                              0.0f, 1.5f,  0.0f, 3.0f,
	                              0.0f, 0.0f,  0.0f, 1.0f),
	                       3.0f, 1.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f,  0.5f, 1.0f,
	                                6.0f, 0.0f,  0.0f, 2.0f,
	                                0.0f, 2.25f, 0.0f, 3.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_scale(&matrix, 2.0f, -1.0f, 0.5f);
	assertMatrixApproximate(matrix, 2.0f, 0.0f,  0.0f, 0.0f,
	                                0.0f, -1.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f,  0.5f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = MATRIX(0.0f, 0.0f, -1.0f, 1.0f,
	               2.0f, 0.0f,  0.0f, 2.0f,
	               0.0f, 1.5f,  0.0f, 3.0f,
	               0.0f, 0.0f,  0.0f, 1.0f);
	Matrix_scale(&matrix, 3.0f, 1.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f,  0.5f, 1.0f,
	                                6.0f, 0.0f,  0.0f, 2.0f,
	                                0.0f, 2.25f, 0.0f, 3.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
}

static void testRotate() {
	Matrix matrix;
	
	matrix = Matrix_rotated(MATRIX_IDENTITY, VECTOR3f(0.0f, 1.0f, 0.0f), M_PI);
	assertMatrixApproximate(matrix, -1.0f, 0.0f,  0.0f, 0.0f,
	                                 0.0f, 1.0f,  0.0f, 0.0f,
	                                 0.0f, 0.0f, -1.0f, 0.0f,
	                                 0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_rotated(Matrix_scaled(MATRIX_IDENTITY, 2.0f, 2.0f, 2.0f),
	                        VECTOR3f(-1.0f, 0.0f, 0.0f),
	                        M_PI * 0.5f);
	assertMatrixApproximate(matrix, 2.0f,  0.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 2.0f, 0.0f,
	                                0.0f, -2.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_rotate(&matrix, VECTOR3f(0.0f, 1.0f, 0.0f), M_PI);
	assertMatrixApproximate(matrix, -1.0f, 0.0f,  0.0f, 0.0f,
	                                 0.0f, 1.0f,  0.0f, 0.0f,
	                                 0.0f, 0.0f, -1.0f, 0.0f,
	                                 0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_scaled(MATRIX_IDENTITY, 2.0f, 2.0f, 2.0f);
	Matrix_rotate(&matrix, VECTOR3f(-1.0f, 0.0f, 0.0f), M_PI * 0.5f);
	assertMatrixApproximate(matrix, 2.0f,  0.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 2.0f, 0.0f,
	                                0.0f, -2.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 0.0f, 1.0f, EPSILON);
}

static void testShear() {
	Matrix matrix;
	
	matrix = Matrix_shearedX(MATRIX_IDENTITY, 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                                1.0f, 1.0f, 0.0f, 0.0f,
	                                1.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedX(Matrix_fromDirectionVectors(VECTOR3f(0.0f, 1.0f, 0.0f),
	                                                     VECTOR3f(0.0f, 0.0f, 1.0f),
	                                                     VECTOR3f(1.0f, 0.0f, 0.0f)),
	                         0.5f, -0.5f);
	assertMatrixApproximate(matrix, -0.5f, 0.0f, 1.0f, 0.0f,
	                                 1.0f, 0.0f, 0.0f, 0.0f,
	                                 0.5f, 1.0f, 0.0f, 0.0f,
	                                 0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedY(MATRIX_IDENTITY, 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedY(Matrix_fromDirectionVectors(VECTOR3f(0.0f, 1.0f, 0.0f),
	                                                     VECTOR3f(0.0f, 0.0f, 1.0f),
	                                                     VECTOR3f(1.0f, 0.0f, 0.0f)),
	                         0.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, -0.5f, 1.0f, 0.0f,
	                                1.0f,  0.5f, 0.0f, 0.0f,
	                                0.0f,  1.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedZ(MATRIX_IDENTITY, 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 1.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedZ(Matrix_fromDirectionVectors(VECTOR3f(0.0f, 1.0f, 0.0f),
	                                                     VECTOR3f(0.0f, 0.0f, 1.0f),
	                                                     VECTOR3f(1.0f, 0.0f, 0.0f)),
	                         0.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f,  1.0f, 0.0f,
	                                1.0f, 0.0f,  0.5f, 0.0f,
	                                0.0f, 1.0f, -0.5f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_shearX(&matrix, 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                                1.0f, 1.0f, 0.0f, 0.0f,
	                                1.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_fromDirectionVectors(VECTOR3f(0.0f, 1.0f, 0.0f),
	                                     VECTOR3f(0.0f, 0.0f, 1.0f),
	                                     VECTOR3f(1.0f, 0.0f, 0.0f));
	Matrix_shearX(&matrix, 0.5f, -0.5f);
	assertMatrixApproximate(matrix, -0.5f, 0.0f, 1.0f, 0.0f,
	                                 1.0f, 0.0f, 0.0f, 0.0f,
	                                 0.5f, 1.0f, 0.0f, 0.0f,
	                                 0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_shearY(&matrix, 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_fromDirectionVectors(VECTOR3f(0.0f, 1.0f, 0.0f),
	                                     VECTOR3f(0.0f, 0.0f, 1.0f),
	                                     VECTOR3f(1.0f, 0.0f, 0.0f));
	Matrix_shearY(&matrix, 0.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, -0.5f, 1.0f, 0.0f,
	                                1.0f,  0.5f, 0.0f, 0.0f,
	                                0.0f,  1.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_shearZ(&matrix, 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 1.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_fromDirectionVectors(VECTOR3f(0.0f, 1.0f, 0.0f),
	                                     VECTOR3f(0.0f, 0.0f, 1.0f),
	                                     VECTOR3f(1.0f, 0.0f, 0.0f));
	Matrix_shearZ(&matrix, 0.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f,  1.0f, 0.0f,
	                                1.0f, 0.0f,  0.5f, 0.0f,
	                                0.0f, 1.0f, -0.5f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
}

static void testPerspective() {
	Matrix matrix;
	
	matrix = Matrix_perspective(MATRIX_IDENTITY, 90.0f, 1.0f, 1.0f, 2.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f,  0.0f,  0.0f,
	                                0.0f, 1.0f,  0.0f,  0.0f,
	                                0.0f, 0.0f, -3.0f, -4.0f,
	                                0.0f, 0.0f, -1.0f,  0.0f, EPSILON);
	
	matrix = Matrix_perspective(MATRIX_IDENTITY, 45.0f, 2.0f, 0.5f, 4.0f);
	assertMatrixApproximate(matrix, 1.20710678118655f, 0.0f,               0.0f,               0.0f,
	                                0.0f,              2.41421356237309f,  0.0f,               0.0f,
	                                0.0f,              0.0f,              -1.28571428571429f, -1.14285714285714f,
	                                0.0f,              0.0f,              -1.0f,               0.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_applyPerspective(&matrix, 90.0f, 1.0f, 1.0f, 2.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f,  0.0f,  0.0f,
	                                0.0f, 1.0f,  0.0f,  0.0f,
	                                0.0f, 0.0f, -3.0f, -4.0f,
	                                0.0f, 0.0f, -1.0f,  0.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_applyPerspective(&matrix, 45.0f, 2.0f, 0.5f, 4.0f);
	assertMatrixApproximate(matrix, 1.20710678118655f, 0.0f,               0.0f,               0.0f,
	                                0.0f,              2.41421356237309f,  0.0f,               0.0f,
	                                0.0f,              0.0f,              -1.28571428571429f, -1.14285714285714f,
	                                0.0f,              0.0f,              -1.0f,               0.0f, EPSILON);
}

static void testOrtho() {
	Matrix matrix;
	
	matrix = Matrix_ortho(MATRIX_IDENTITY, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f,  0.0f, 0.0f,
	                                0.0f, 1.0f,  0.0f, 0.0f,
	                                0.0f, 0.0f, -1.0f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_ortho(MATRIX_IDENTITY, 0.0f, 4.0f, 8.0f, 0.0f, 0.0f, 10.0f);
	assertMatrixApproximate(matrix, 0.5f,  0.0f,   0.0f, -1.0f,
	                                0.0f, -0.25f,  0.0f,  1.0f,
	                                0.0f,  0.0f,  -0.2f, -1.0f,
	                                0.0f,  0.0f,   0.0f,  1.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_applyOrtho(&matrix, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f,  0.0f, 0.0f,
	                                0.0f, 1.0f,  0.0f, 0.0f,
	                                0.0f, 0.0f, -1.0f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_applyOrtho(&matrix, 0.0f, 4.0f, 8.0f, 0.0f, 0.0f, 10.0f);
	assertMatrixApproximate(matrix, 0.5f,  0.0f,   0.0f, -1.0f,
	                                0.0f, -0.25f,  0.0f,  1.0f,
	                                0.0f,  0.0f,  -0.2f, -1.0f,
	                                0.0f,  0.0f,   0.0f,  1.0f, EPSILON);
}

static void testTranspose() {
	Matrix matrix;
	
	matrix = Matrix_transposed(MATRIX(1.0f, 0.0f, 0.0f, 0.0f,
	                                  1.0f, 1.0f, 0.0f, 0.0f,
	                                  1.0f, 1.0f, 1.0f, 0.0f,
	                                  1.0f, 1.0f, 1.0f, 1.0f));
	assertMatrixExact(matrix, 1.0f, 1.0f, 1.0f, 1.0f,
	                          0.0f, 1.0f, 1.0f, 1.0f,
	                          0.0f, 0.0f, 1.0f, 1.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
	
	matrix = Matrix_transposed(MATRIX(0.0f, 0.4f, 0.8f, 1.2f,
	                                  0.1f, 0.5f, 0.9f, 1.3f,
	                                  0.2f, 0.6f, 1.0f, 1.4f,
	                                  0.3f, 0.7f, 1.1f, 1.5f));
	assertMatrixExact(matrix, 0.0f, 0.1f, 0.2f, 0.3f,
	                          0.4f, 0.5f, 0.6f, 0.7f,
	                          0.8f, 0.9f, 1.0f, 1.1f,
	                          1.2f, 1.3f, 1.4f, 1.5f);
	
	matrix = MATRIX(1.0f, 0.0f, 0.0f, 0.0f,
	                1.0f, 1.0f, 0.0f, 0.0f,
	                1.0f, 1.0f, 1.0f, 0.0f,
	                1.0f, 1.0f, 1.0f, 1.0f);
	Matrix_transpose(&matrix);
	assertMatrixExact(matrix, 1.0f, 1.0f, 1.0f, 1.0f,
	                          0.0f, 1.0f, 1.0f, 1.0f,
	                          0.0f, 0.0f, 1.0f, 1.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
	
	matrix = MATRIX(0.0f, 0.4f, 0.8f, 1.2f,
	                0.1f, 0.5f, 0.9f, 1.3f,
	                0.2f, 0.6f, 1.0f, 1.4f,
	                0.3f, 0.7f, 1.1f, 1.5f);
	Matrix_transpose(&matrix);
	assertMatrixExact(matrix, 0.0f, 0.1f, 0.2f, 0.3f,
	                          0.4f, 0.5f, 0.6f, 0.7f,
	                          0.8f, 0.9f, 1.0f, 1.1f,
	                          1.2f, 1.3f, 1.4f, 1.5f);
}

static void testDeterminant() {
	float determinant;
	
	determinant = Matrix_determinant(MATRIX_IDENTITY);
	TestCase_assert(fabs(determinant - 1.0f) < EPSILON, "Expected 1.0 but got %f", determinant);
	
	determinant = Matrix_determinant(MATRIX(0.0f, 0.0f, 2.0f, 2.0f,
	                                        2.0f, 0.0f, 0.0f, 3.0f,
	                                        0.0f, 2.0f, 0.0f, 1.0f,
	                                        0.0f, 0.0f, 0.0f, 1.0f));
	TestCase_assert(fabs(determinant - 8.0f) < EPSILON, "Expected 8.0 but got %f", determinant);
}

static void testInvert() {
	Matrix matrix;
	
	matrix = Matrix_inverted(MATRIX_IDENTITY);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_inverted(MATRIX(0.0f, 0.0f, 2.0f, 2.0f,
	                                2.0f, 0.0f, 0.0f, 3.0f,
	                                0.0f, 2.0f, 0.0f, 1.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f));
	assertMatrixApproximate(matrix, 0.0f, 0.5f, 0.0f, -1.5f,
	                                0.0f, 0.0f, 0.5f, -0.5f,
	                                0.5f, 0.0f, 0.0f, -1.0f,
	                                0.0f, 0.0f, 0.0f,  1.0f, EPSILON);
	
	matrix = MATRIX_IDENTITY;
	Matrix_invert(&matrix);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = MATRIX(0.0f, 0.0f, 2.0f, 2.0f,
	                2.0f, 0.0f, 0.0f, 3.0f,
	                0.0f, 2.0f, 0.0f, 1.0f,
	                0.0f, 0.0f, 0.0f, 1.0f);
	Matrix_invert(&matrix);
	assertMatrixApproximate(matrix, 0.0f, 0.5f, 0.0f, -1.5f,
	                                0.0f, 0.0f, 0.5f, -0.5f,
	                                0.5f, 0.0f, 0.0f, -1.0f,
	                                0.0f, 0.0f, 0.0f,  1.0f, EPSILON);
}

static void testInterpolate() {
	Matrix matrixLeft, matrixRight, matrixInterpolated;
	
	matrixLeft = MATRIX_IDENTITY;
	matrixRight = MATRIX(0.0f, 1.0f, 1.0f, 1.0f,
	                     1.0f, 0.0f, 1.0f, 1.0f,
	                     1.0f, 1.0f, 0.0f, 1.0f,
	                     1.0f, 1.0f, 1.0f, 0.0f);
	
	matrixInterpolated = Matrix_interpolated(matrixLeft, matrixRight, 0.0f);
	assertMatrixApproximate(matrixInterpolated, 1.0f, 0.0f, 0.0f, 0.0f,
	                                            0.0f, 1.0f, 0.0f, 0.0f,
	                                            0.0f, 0.0f, 1.0f, 0.0f,
	                                            0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrixInterpolated = Matrix_interpolated(matrixLeft, matrixRight, 0.25f);
	assertMatrixApproximate(matrixInterpolated, 0.75f, 0.25f, 0.25f, 0.25f,
	                                            0.25f, 0.75f, 0.25f, 0.25f,
	                                            0.25f, 0.25f, 0.75f, 0.25f,
	                                            0.25f, 0.25f, 0.25f, 0.75f, EPSILON);
	
	matrixInterpolated = matrixLeft;
	Matrix_interpolate(&matrixInterpolated, matrixRight, 0.0f);
	assertMatrixApproximate(matrixInterpolated, 1.0f, 0.0f, 0.0f, 0.0f,
	                                            0.0f, 1.0f, 0.0f, 0.0f,
	                                            0.0f, 0.0f, 1.0f, 0.0f,
	                                            0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrixInterpolated = matrixLeft;
	Matrix_interpolate(&matrixInterpolated, matrixRight, 0.25f);
	assertMatrixApproximate(matrixInterpolated, 0.75f, 0.25f, 0.25f, 0.25f,
	                                            0.25f, 0.75f, 0.25f, 0.25f,
	                                            0.25f, 0.25f, 0.75f, 0.25f,
	                                            0.25f, 0.25f, 0.25f, 0.75f, EPSILON);
	
	matrixLeft = MATRIX(0.0f, 4.0f,  8.0f, 12.0f,
	                    1.0f, 5.0f,  9.0f, 13.0f,
	                    2.0f, 6.0f, 10.0f, 14.0f,
	                    3.0f, 7.0f, 11.0f, 15.0f);
	matrixRight = MATRIX_IDENTITY;
	
	matrixInterpolated = Matrix_interpolated(matrixLeft, matrixRight, -1.0f);
	assertMatrixApproximate(matrixInterpolated, -1.0f,  8.0f, 16.0f, 24.0f,
	                                             2.0f,  9.0f, 18.0f, 26.0f,
	                                             4.0f, 12.0f, 19.0f, 28.0f,
	                                             6.0f, 14.0f, 22.0f, 29.0f, EPSILON);
	
	matrixInterpolated = Matrix_interpolated(matrixLeft, matrixRight, 2.0f);
	assertMatrixApproximate(matrixInterpolated,  2.0f, -4.0f,  -8.0f, -12.0f,
	                                            -1.0f, -3.0f,  -9.0f, -13.0f,
	                                            -2.0f, -6.0f,  -8.0f, -14.0f,
	                                            -3.0f, -7.0f, -11.0f, -13.0f, EPSILON);
	
	matrixInterpolated = matrixLeft;
	Matrix_interpolate(&matrixInterpolated, matrixRight, -1.0f);
	assertMatrixApproximate(matrixInterpolated, -1.0f,  8.0f, 16.0f, 24.0f,
	                                             2.0f,  9.0f, 18.0f, 26.0f,
	                                             4.0f, 12.0f, 19.0f, 28.0f,
	                                             6.0f, 14.0f, 22.0f, 29.0f, EPSILON);
	
	matrixInterpolated = matrixLeft;
	Matrix_interpolate(&matrixInterpolated, matrixRight, 2.0f);
	assertMatrixApproximate(matrixInterpolated,  2.0f, -4.0f,  -8.0f, -12.0f,
	                                            -1.0f, -3.0f,  -9.0f, -13.0f,
	                                            -2.0f, -6.0f,  -8.0f, -14.0f,
	                                            -3.0f, -7.0f, -11.0f, -13.0f, EPSILON);
}

static void testMultiplyVector() {
	Matrix matrix;
	Vector2f vector2;
	Vector3f vector3;
	Vector4f vector4;
	
	matrix = MATRIX_IDENTITY;
	vector2 = Matrix_multiplyVector2f(matrix, VECTOR2f(1.0f, 0.0f));
	assertVector2fApproximate(vector2, 1.0f, 0.0f, EPSILON);
	
	vector3 = Matrix_multiplyVector3f(matrix, VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fApproximate(vector3, 0.0f, 1.0f, 0.0f, EPSILON);
	
	vector3 = Matrix_multiplyVector3f_rotationOnly(matrix, VECTOR3f(0.0f, 1.0f, 0.0f));
	assertVector3fApproximate(vector3, 0.0f, 1.0f, 0.0f, EPSILON);
	
	vector4 = Matrix_multiplyVector4f(matrix, VECTOR4f(0.0f, 0.0f, 1.0f, 0.0f));
	assertVector4fApproximate(vector4, 0.0f, 0.0f, 1.0f, 0.0f, EPSILON);
	
	matrix = MATRIX(0.0f, 2.0f,  0.0f, -1.0f,
	                2.0f, 0.0f,  0.0f,  1.0f,
	                0.0f, 0.0f, -2.0f,  2.0f,
	                0.0f, 0.0f,  0.0f,  1.0f);
	vector2 = Matrix_multiplyVector2f(matrix, VECTOR2f(-1.0f, 0.0f));
	assertVector2fApproximate(vector2, -1.0f, -1.0f, EPSILON);
	
	vector3 = Matrix_multiplyVector3f(matrix, VECTOR3f(0.0f, -1.0f, 0.0f));
	assertVector3fApproximate(vector3, -3.0f, 1.0f, 2.0f, EPSILON);
	
	vector3 = Matrix_multiplyVector3f_rotationOnly(matrix, VECTOR3f(0.0f, -1.0f, 0.0f));
	assertVector3fApproximate(vector3, -2.0f, 0.0f, 0.0f, EPSILON);
	
	vector4 = Matrix_multiplyVector4f(matrix, VECTOR4f(0.0f, 0.0f, -1.0f, 1.0f));
	assertVector4fApproximate(vector4, -1.0f, 1.0f, 4.0f, 1.0f, EPSILON);
}

TEST_SUITE(MatrixTest,
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
