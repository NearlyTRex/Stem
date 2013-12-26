#include "unittest/TestSuite.h"
#include "glgraphics/Camera.h"
#include <math.h>

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

static void testInit() {
	Camera * cameraPtr;
	Camera cameraStruct;
	bool success;
	
	cameraPtr = Camera_create();
	TestCase_assert(cameraPtr != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(cameraPtr->orientation.x == 0.0f, "Expected 0 but got %f", cameraPtr->orientation.x);
	TestCase_assert(cameraPtr->orientation.y == 0.0f, "Expected 0 but got %f", cameraPtr->orientation.y);
	TestCase_assert(cameraPtr->orientation.z == 0.0f, "Expected 0 but got %f", cameraPtr->orientation.z);
	TestCase_assert(cameraPtr->orientation.w == 1.0f, "Expected 1 but got %f", cameraPtr->orientation.w);
	TestCase_assert(cameraPtr->position.x == 0.0f, "Expected 0 but got %f", cameraPtr->position.x);
	TestCase_assert(cameraPtr->position.y == 0.0f, "Expected 0 but got %f", cameraPtr->position.y);
	TestCase_assert(cameraPtr->position.z == 0.0f, "Expected 0 but got %f", cameraPtr->position.z);
	TestCase_assert(cameraPtr->dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(cameraPtr->getMatrix != NULL, "Method unexpectedly NULL");
	cameraPtr->dispose(cameraPtr);
	
	success = Camera_init(&cameraStruct);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(cameraStruct.orientation.x == 0.0f, "Expected 0 but got %f", cameraStruct.orientation.x);
	TestCase_assert(cameraStruct.orientation.y == 0.0f, "Expected 0 but got %f", cameraStruct.orientation.y);
	TestCase_assert(cameraStruct.orientation.z == 0.0f, "Expected 0 but got %f", cameraStruct.orientation.z);
	TestCase_assert(cameraStruct.orientation.w == 1.0f, "Expected 1 but got %f", cameraStruct.orientation.w);
	TestCase_assert(cameraStruct.position.x == 0.0f, "Expected 0 but got %f", cameraStruct.position.x);
	TestCase_assert(cameraStruct.position.y == 0.0f, "Expected 0 but got %f", cameraStruct.position.y);
	TestCase_assert(cameraStruct.position.z == 0.0f, "Expected 0 but got %f", cameraStruct.position.z);
	TestCase_assert(cameraStruct.dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(cameraStruct.getMatrix != NULL, "Method unexpectedly NULL");
	cameraStruct.dispose(&cameraStruct);
}

void testGetMatrix() {
	Camera camera;
	Matrix matrix;
	
	Camera_init(&camera);
	matrix = camera.getMatrix(&camera);
	assertMatrixApproximate(matrix, 1.0f, 0.0f, 0.0f, 0.0f,
	                                0.0f, 1.0f, 0.0f, 0.0f,
	                                0.0f, 0.0f, 1.0f, 0.0f,
	                                0.0f, 0.0f, 0.0f, 1.0f, 0.00001f);
	
	camera.orientation = Quaternion_fromAxisAngle(Vector3_init(0.0f, 1.0f, 0.0f), M_PI);
	camera.position = Vector3_init(1.0f, -1.0f, 2.0f);
	matrix = camera.getMatrix(&camera);
	assertMatrixApproximate(matrix, -1.0f, 0.0f,  0.0f, 1.0f,
	                                 0.0f, 1.0f,  0.0f, 1.0f,
	                                 0.0f, 0.0f, -1.0f, 2.0f,
	                                 0.0f, 0.0f,  0.0f, 1.0f, 0.00001f);
}

TEST_SUITE(CameraTest, testInit, testGetMatrix)
