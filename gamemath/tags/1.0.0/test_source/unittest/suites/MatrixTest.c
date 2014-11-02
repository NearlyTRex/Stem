#include "unittest/framework/TestSuite.h"
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
	Matrix matrix;
	
	matrix = Matrix_init(1.0f, 0.0f, 0.0f, 0.0f,
	                     0.0f, 1.0f, 0.0f, 0.0f,
	                     0.0f, 0.0f, 1.0f, 0.0f,
	                     0.0f, 0.0f, 0.0f, 1.0f);
	assertMatrixExact(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                          0.0f, 1.0f, 0.0f, 0.0f,
	                          0.0f, 0.0f, 1.0f, 0.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
	
	matrix = Matrix_init(0.0f, 4.0f, 8.0f,  12.0f,
	                     1.0f, 5.0f, 9.0f,  13.0f,
	                     2.0f, 6.0f, 10.0f, 14.0f,
	                     3.0f, 7.0f, 11.0f, 15.0f);
	assertMatrixExact(matrix, 0.0f, 4.0f, 8.0f,  12.0f,
	                          1.0f, 5.0f, 9.0f,  13.0f,
	                          2.0f, 6.0f, 10.0f, 14.0f,
	                          3.0f, 7.0f, 11.0f, 15.0f);
	
	matrix = Matrix_fromDirectionVectors(Vector3_init(1.0f, 0.0f, 0.0f),
	                                     Vector3_init(0.0f, 1.0f, 0.0f),
	                                     Vector3_init(0.0f, 0.0f, 1.0f));
	assertMatrixExact(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                          0.0f, 1.0f, 0.0f, 0.0f,
	                          0.0f, 0.0f, 1.0f, 0.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
	
	matrix = Matrix_fromDirectionVectors(Vector3_init(0.0f, 1.0f, 0.0f),
	                                     Vector3_init(0.0f, 0.0f, 1.0f),
	                                     Vector3_init(1.0f, 0.0f, 0.0f));
	assertMatrixExact(matrix, 0.0f, 0.0f, 1.0f, 0.0f,
	                          1.0f, 0.0f, 0.0f, 0.0f,
	                          0.0f, 1.0f, 0.0f, 0.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
}

static void testIdentity() {
	Matrix matrix;
	
	matrix = Matrix_identity();
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
	
	matrix = Matrix_multiplied(Matrix_init(2.0f, 0.0f, 0.0f, 0.0f,
	                                       0.0f, 2.0f, 0.0f, 0.0f,
	                                       0.0f, 0.0f, 2.0f, 0.0f,
	                                       0.0f, 0.0f, 0.0f, 2.0f),
	                           Matrix_init(2.0f, 0.0f, 0.0f, 0.0f,
	                                       0.0f, 2.0f, 0.0f, 0.0f,
	                                       0.0f, 0.0f, 2.0f, 0.0f,
	                                       0.0f, 0.0f, 0.0f, 2.0f));
	assertMatrixApproximate(matrix, 4.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 4.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 4.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 4.0f, EPSILON);
	
	matrix = Matrix_multiplied(Matrix_init(0.0f, 4.0f, 8.0f,  12.0f,
	                                       1.0f, 5.0f, 9.0f,  13.0f,
	                                       2.0f, 6.0f, 10.0f, 14.0f,
	                                       3.0f, 7.0f, 11.0f, 15.0f),
	                           Matrix_init(1.0f, 7.0f,  19.0f, 37.0f,
	                                       2.0f, 11.0f, 23.0f, 41.0f,
	                                       3.0f, 13.0f, 29.0f, 43.0f,
	                                       5.0f, 17.0f, 31.0f, 47.0f));
	assertMatrixApproximate(matrix, 92.0f,  352.0f, 696.0f,  1072.0f,
	                                103.0f, 400.0f, 798.0f,  1240.0f,
	                                114.0f, 448.0f, 900.0f,  1408.0f,
	                                125.0f, 496.0f, 1002.0f, 1576.0f, EPSILON);
	
	matrix = Matrix_init(2.0f, 0.0f, 0.0f, 0.0f,
	                     0.0f, 2.0f, 0.0f, 0.0f,
	                     0.0f, 0.0f, 2.0f, 0.0f,
	                     0.0f, 0.0f, 0.0f, 2.0f),
	Matrix_multiply(&matrix, Matrix_init(2.0f, 0.0f, 0.0f, 0.0f,
	                                     0.0f, 2.0f, 0.0f, 0.0f,
	                                     0.0f, 0.0f, 2.0f, 0.0f,
	                                     0.0f, 0.0f, 0.0f, 2.0f));
	assertMatrixApproximate(matrix, 4.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 4.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 4.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 4.0f, EPSILON);
	
	matrix = Matrix_init(0.0f, 4.0f, 8.0f,  12.0f,
	                     1.0f, 5.0f, 9.0f,  13.0f,
	                     2.0f, 6.0f, 10.0f, 14.0f,
	                     3.0f, 7.0f, 11.0f, 15.0f),
	Matrix_multiply(&matrix, Matrix_init(1.0f, 7.0f,  19.0f, 37.0f,
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
	
	matrix = Matrix_translated(Matrix_identity(), 3.0f, -1.5f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 3.0f,
	                                0.0f, 1.0f, 0.0f, -1.5f,
	                                0.0f, 0.0f, 1.0f, 1.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_translated(Matrix_fromDirectionVectors(Vector3_init(0.0f, 1.0f, 0.0f),
	                                                       Vector3_init(0.0f, 0.0f, 1.0f),
	                                                       Vector3_init(1.0f, 0.0f, 0.0f)),
	                           2.0f, 3.0f, -1.0f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f, 1.0f, -1.0f,
	                                1.0f, 0.0f, 0.0f, 2.0f,
	                                0.0f, 1.0f, 0.0f, 3.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_translate(&matrix, 3.0f, -1.5f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 3.0f,
	                                0.0f, 1.0f, 0.0f, -1.5f,
	                                0.0f, 0.0f, 1.0f, 1.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_fromDirectionVectors(Vector3_init(0.0f, 1.0f, 0.0f),
	                                     Vector3_init(0.0f, 0.0f, 1.0f),
	                                     Vector3_init(1.0f, 0.0f, 0.0f));
	Matrix_translate(&matrix, 2.0f, 3.0f, -1.0f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f, 1.0f, -1.0f,
	                                1.0f, 0.0f, 0.0f, 2.0f,
	                                0.0f, 1.0f, 0.0f, 3.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
}

static void testScale() {
	Matrix matrix;
	
	matrix = Matrix_scaled(Matrix_identity(), 2.0f, -1.0f, 0.5f);
	assertMatrixApproximate(matrix, 2.0f, 0.0f,  0.0f, 0.0f,
	                                0.0f, -1.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f,  0.5f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_scaled(Matrix_init(0.0f, 0.0f, -1.0f, 1.0f,
	                                   2.0f, 0.0f,  0.0f, 2.0f,
	                                   0.0f, 1.5f,  0.0f, 3.0f,
	                                   0.0f, 0.0f,  0.0f, 1.0f),
	                       3.0f, 1.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f,  0.5f, 1.0f,
	                                6.0f, 0.0f,  0.0f, 2.0f,
	                                0.0f, 2.25f, 0.0f, 3.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_scale(&matrix, 2.0f, -1.0f, 0.5f);
	assertMatrixApproximate(matrix, 2.0f, 0.0f,  0.0f, 0.0f,
	                                0.0f, -1.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f,  0.5f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_init(0.0f, 0.0f, -1.0f, 1.0f,
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
	
	matrix = Matrix_rotated(Matrix_identity(), Vector3_init(0.0f, 1.0f, 0.0f), M_PI);
	assertMatrixApproximate(matrix, -1.0f, 0.0f,  0.0f, 0.0f,
	                                 0.0f, 1.0f,  0.0f, 0.0f,
	                                 0.0f, 0.0f, -1.0f, 0.0f,
	                                 0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_rotated(Matrix_scaled(Matrix_identity(), 2.0f, 2.0f, 2.0f),
	                        Vector3_init(-1.0f, 0.0f, 0.0f),
	                        M_PI * 0.5f);
	assertMatrixApproximate(matrix, 2.0f,  0.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 2.0f, 0.0f,
	                                0.0f, -2.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_rotate(&matrix, Vector3_init(0.0f, 1.0f, 0.0f), M_PI);
	assertMatrixApproximate(matrix, -1.0f, 0.0f,  0.0f, 0.0f,
	                                 0.0f, 1.0f,  0.0f, 0.0f,
	                                 0.0f, 0.0f, -1.0f, 0.0f,
	                                 0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_scaled(Matrix_identity(), 2.0f, 2.0f, 2.0f);
	Matrix_rotate(&matrix, Vector3_init(-1.0f, 0.0f, 0.0f), M_PI * 0.5f);
	assertMatrixApproximate(matrix, 2.0f,  0.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 2.0f, 0.0f,
	                                0.0f, -2.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 0.0f, 1.0f, EPSILON);
}

static void testShear() {
	Matrix matrix;
	
	matrix = Matrix_shearedX(Matrix_identity(), 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                                1.0f, 1.0f, 0.0f, 0.0f,
	                                1.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedX(Matrix_fromDirectionVectors(Vector3_init(0.0f, 1.0f, 0.0f),
	                                                     Vector3_init(0.0f, 0.0f, 1.0f),
	                                                     Vector3_init(1.0f, 0.0f, 0.0f)),
	                         0.5f, -0.5f);
	assertMatrixApproximate(matrix, -0.5f, 0.0f, 1.0f, 0.0f,
	                                 1.0f, 0.0f, 0.0f, 0.0f,
	                                 0.5f, 1.0f, 0.0f, 0.0f,
	                                 0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedY(Matrix_identity(), 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedY(Matrix_fromDirectionVectors(Vector3_init(0.0f, 1.0f, 0.0f),
	                                                     Vector3_init(0.0f, 0.0f, 1.0f),
	                                                     Vector3_init(1.0f, 0.0f, 0.0f)),
	                         0.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, -0.5f, 1.0f, 0.0f,
	                                1.0f,  0.5f, 0.0f, 0.0f,
	                                0.0f,  1.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedZ(Matrix_identity(), 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 1.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_shearedZ(Matrix_fromDirectionVectors(Vector3_init(0.0f, 1.0f, 0.0f),
	                                                     Vector3_init(0.0f, 0.0f, 1.0f),
	                                                     Vector3_init(1.0f, 0.0f, 0.0f)),
	                         0.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f,  1.0f, 0.0f,
	                                1.0f, 0.0f,  0.5f, 0.0f,
	                                0.0f, 1.0f, -0.5f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_shearX(&matrix, 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                                1.0f, 1.0f, 0.0f, 0.0f,
	                                1.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_fromDirectionVectors(Vector3_init(0.0f, 1.0f, 0.0f),
	                                     Vector3_init(0.0f, 0.0f, 1.0f),
	                                     Vector3_init(1.0f, 0.0f, 0.0f));
	Matrix_shearX(&matrix, 0.5f, -0.5f);
	assertMatrixApproximate(matrix, -0.5f, 0.0f, 1.0f, 0.0f,
	                                 1.0f, 0.0f, 0.0f, 0.0f,
	                                 0.5f, 1.0f, 0.0f, 0.0f,
	                                 0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_shearY(&matrix, 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_fromDirectionVectors(Vector3_init(0.0f, 1.0f, 0.0f),
	                                     Vector3_init(0.0f, 0.0f, 1.0f),
	                                     Vector3_init(1.0f, 0.0f, 0.0f));
	Matrix_shearY(&matrix, 0.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, -0.5f, 1.0f, 0.0f,
	                                1.0f,  0.5f, 0.0f, 0.0f,
	                                0.0f,  1.0f, 0.0f, 0.0f,
	                                0.0f,  0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_shearZ(&matrix, 1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 1.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_fromDirectionVectors(Vector3_init(0.0f, 1.0f, 0.0f),
	                                     Vector3_init(0.0f, 0.0f, 1.0f),
	                                     Vector3_init(1.0f, 0.0f, 0.0f));
	Matrix_shearZ(&matrix, 0.5f, -0.5f);
	assertMatrixApproximate(matrix, 0.0f, 0.0f,  1.0f, 0.0f,
	                                1.0f, 0.0f,  0.5f, 0.0f,
	                                0.0f, 1.0f, -0.5f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
}

static void testPerspective() {
	Matrix matrix;
	
	matrix = Matrix_perspective(Matrix_identity(), 90.0f, 1.0f, 1.0f, 2.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f,  0.0f,  0.0f,
	                                0.0f, 1.0f,  0.0f,  0.0f,
	                                0.0f, 0.0f, -3.0f, -4.0f,
	                                0.0f, 0.0f, -1.0f,  0.0f, EPSILON);
	
	matrix = Matrix_perspective(Matrix_identity(), 45.0f, 2.0f, 0.5f, 4.0f);
	assertMatrixApproximate(matrix, 1.20710678118655f, 0.0f,               0.0f,               0.0f,
	                                0.0f,              2.41421356237309f,  0.0f,               0.0f,
	                                0.0f,              0.0f,              -1.28571428571429f, -1.14285714285714f,
	                                0.0f,              0.0f,              -1.0f,               0.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_applyPerspective(&matrix, 90.0f, 1.0f, 1.0f, 2.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f,  0.0f,  0.0f,
	                                0.0f, 1.0f,  0.0f,  0.0f,
	                                0.0f, 0.0f, -3.0f, -4.0f,
	                                0.0f, 0.0f, -1.0f,  0.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_applyPerspective(&matrix, 45.0f, 2.0f, 0.5f, 4.0f);
	assertMatrixApproximate(matrix, 1.20710678118655f, 0.0f,               0.0f,               0.0f,
	                                0.0f,              2.41421356237309f,  0.0f,               0.0f,
	                                0.0f,              0.0f,              -1.28571428571429f, -1.14285714285714f,
	                                0.0f,              0.0f,              -1.0f,               0.0f, EPSILON);
}

static void testOrtho() {
	Matrix matrix;
	
	matrix = Matrix_ortho(Matrix_identity(), -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f,  0.0f, 0.0f,
	                                0.0f, 1.0f,  0.0f, 0.0f,
	                                0.0f, 0.0f, -1.0f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_ortho(Matrix_identity(), 0.0f, 4.0f, 8.0f, 0.0f, 0.0f, 10.0f);
	assertMatrixApproximate(matrix, 0.5f,  0.0f,   0.0f, -1.0f,
	                                0.0f, -0.25f,  0.0f,  1.0f,
	                                0.0f,  0.0f,  -0.2f, -1.0f,
	                                0.0f,  0.0f,   0.0f,  1.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_applyOrtho(&matrix, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	assertMatrixApproximate(matrix, 1.0f, 0.0f,  0.0f, 0.0f,
	                                0.0f, 1.0f,  0.0f, 0.0f,
	                                0.0f, 0.0f, -1.0f, 0.0f,
	                                0.0f, 0.0f,  0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_applyOrtho(&matrix, 0.0f, 4.0f, 8.0f, 0.0f, 0.0f, 10.0f);
	assertMatrixApproximate(matrix, 0.5f,  0.0f,   0.0f, -1.0f,
	                                0.0f, -0.25f,  0.0f,  1.0f,
	                                0.0f,  0.0f,  -0.2f, -1.0f,
	                                0.0f,  0.0f,   0.0f,  1.0f, EPSILON);
}

static void testTranspose() {
	Matrix matrix;
	
	matrix = Matrix_transposed(Matrix_init(1.0f, 0.0f, 0.0f, 0.0f,
	                                       1.0f, 1.0f, 0.0f, 0.0f,
	                                       1.0f, 1.0f, 1.0f, 0.0f,
	                                       1.0f, 1.0f, 1.0f, 1.0f));
	assertMatrixExact(matrix, 1.0f, 1.0f, 1.0f, 1.0f,
	                          0.0f, 1.0f, 1.0f, 1.0f,
	                          0.0f, 0.0f, 1.0f, 1.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
	
	matrix = Matrix_transposed(Matrix_init(0.0f, 0.4f, 0.8f, 1.2f,
	                                       0.1f, 0.5f, 0.9f, 1.3f,
	                                       0.2f, 0.6f, 1.0f, 1.4f,
	                                       0.3f, 0.7f, 1.1f, 1.5f));
	assertMatrixExact(matrix, 0.0f, 0.1f, 0.2f, 0.3f,
	                          0.4f, 0.5f, 0.6f, 0.7f,
	                          0.8f, 0.9f, 1.0f, 1.1f,
	                          1.2f, 1.3f, 1.4f, 1.5f);
	
	matrix = Matrix_init(1.0f, 0.0f, 0.0f, 0.0f,
	                     1.0f, 1.0f, 0.0f, 0.0f,
	                     1.0f, 1.0f, 1.0f, 0.0f,
	                     1.0f, 1.0f, 1.0f, 1.0f);
	Matrix_transpose(&matrix);
	assertMatrixExact(matrix, 1.0f, 1.0f, 1.0f, 1.0f,
	                          0.0f, 1.0f, 1.0f, 1.0f,
	                          0.0f, 0.0f, 1.0f, 1.0f,
	                          0.0f, 0.0f, 0.0f, 1.0f);
	
	matrix = Matrix_init(0.0f, 0.4f, 0.8f, 1.2f,
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
	
	determinant = Matrix_determinant(Matrix_identity());
	TestCase_assert(fabs(determinant - 1.0f) < EPSILON, "Expected 1.0 but got %f", determinant);
	
	determinant = Matrix_determinant(Matrix_init(0.0f, 0.0f, 2.0f, 2.0f,
	                                             2.0f, 0.0f, 0.0f, 3.0f,
	                                             0.0f, 2.0f, 0.0f, 1.0f,
	                                             0.0f, 0.0f, 0.0f, 1.0f));
	TestCase_assert(fabs(determinant - 8.0f) < EPSILON, "Expected 8.0 but got %f", determinant);
}

static void testInvert() {
	Matrix matrix;
	
	matrix = Matrix_inverted(Matrix_identity());
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_inverted(Matrix_init(0.0f, 0.0f, 2.0f, 2.0f,
	                                     2.0f, 0.0f, 0.0f, 3.0f,
	                                     0.0f, 2.0f, 0.0f, 1.0f,
	                                     0.0f, 0.0f, 0.0f, 1.0f));
	assertMatrixApproximate(matrix, 0.0f, 0.5f, 0.0f, -1.5f,
	                                0.0f, 0.0f, 0.5f, -0.5f,
	                                0.5f, 0.0f, 0.0f, -1.0f,
	                                0.0f, 0.0f, 0.0f,  1.0f, EPSILON);
	
	matrix = Matrix_identity();
	Matrix_invert(&matrix);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, EPSILON);
	
	matrix = Matrix_init(0.0f, 0.0f, 2.0f, 2.0f,
	                     2.0f, 0.0f, 0.0f, 3.0f,
	                     0.0f, 2.0f, 0.0f, 1.0f,
	                     0.0f, 0.0f, 0.0f, 1.0f);
	Matrix_invert(&matrix);
	assertMatrixApproximate(matrix, 0.0f, 0.5f, 0.0f, -1.5f,
	                                0.0f, 0.0f, 0.5f, -0.5f,
	                                0.5f, 0.0f, 0.0f, -1.0f,
	                                0.0f, 0.0f, 0.0f,  1.0f, EPSILON);
}

static void testMultiplyVector() {
	Matrix matrix;
	Vector2 vector2;
	Vector3 vector3;
	Vector4 vector4;
	
	matrix = Matrix_identity();
	vector2 = Matrix_multiplyVector2(matrix, Vector2_init(1.0f, 0.0f));
	assertVector2Approximate(vector2, 1.0f, 0.0f, EPSILON);
	
	vector3 = Matrix_multiplyVector3(matrix, Vector3_init(0.0f, 1.0f, 0.0f));
	assertVector3Approximate(vector3, 0.0f, 1.0f, 0.0f, EPSILON);
	
	vector3 = Matrix_multiplyVector3_rotationOnly(matrix, Vector3_init(0.0f, 1.0f, 0.0f));
	assertVector3Approximate(vector3, 0.0f, 1.0f, 0.0f, EPSILON);
	
	vector4 = Matrix_multiplyVector4(matrix, Vector4_init(0.0f, 0.0f, 1.0f, 0.0f));
	assertVector4Approximate(vector4, 0.0f, 0.0f, 1.0f, 0.0f, EPSILON);
	
	matrix = Matrix_init(0.0f, 2.0f,  0.0f, -1.0f,
	                     2.0f, 0.0f,  0.0f,  1.0f,
	                     0.0f, 0.0f, -2.0f,  2.0f,
	                     0.0f, 0.0f,  0.0f,  1.0f);
	vector2 = Matrix_multiplyVector2(matrix, Vector2_init(-1.0f, 0.0f));
	assertVector2Approximate(vector2, -1.0f, -1.0f, EPSILON);
	
	vector3 = Matrix_multiplyVector3(matrix, Vector3_init(0.0f, -1.0f, 0.0f));
	assertVector3Approximate(vector3, -3.0f, 1.0f, 2.0f, EPSILON);
	
	vector3 = Matrix_multiplyVector3_rotationOnly(matrix, Vector3_init(0.0f, -1.0f, 0.0f));
	assertVector3Approximate(vector3, -2.0f, 0.0f, 0.0f, EPSILON);
	
	vector4 = Matrix_multiplyVector4(matrix, Vector4_init(0.0f, 0.0f, -1.0f, 1.0f));
	assertVector4Approximate(vector4, -1.0f, 1.0f, 4.0f, 1.0f, EPSILON);
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
           testMultiplyVector)
