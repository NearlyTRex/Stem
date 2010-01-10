#include "unittest/framework/TestSuite.h"
#include "glgraphics/Camera.h"

static void testInit() {
	Camera * cameraPtr;
	Camera cameraStruct;
	
	cameraPtr = Camera_create();
	TestCase_assert(cameraPtr->orientation.x == 0.0f, "Expected 0 but got %f", cameraPtr->orientation.x);
	TestCase_assert(cameraPtr->orientation.y == 0.0f, "Expected 0 but got %f", cameraPtr->orientation.y);
	TestCase_assert(cameraPtr->orientation.z == 0.0f, "Expected 0 but got %f", cameraPtr->orientation.z);
	TestCase_assert(cameraPtr->orientation.w == 1.0f, "Expected 1 but got %f", cameraPtr->orientation.w);
	TestCase_assert(cameraPtr->position.x == 0.0f, "Expected 0 but got %f", cameraPtr->position.x);
	TestCase_assert(cameraPtr->position.y == 0.0f, "Expected 0 but got %f", cameraPtr->position.y);
	TestCase_assert(cameraPtr->position.z == 0.0f, "Expected 0 but got %f", cameraPtr->position.z);
	TestCase_assert(cameraPtr->dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(cameraPtr->apply != NULL, "Method unexpectedly NULL");
	
	Camera_init(&cameraStruct);
	TestCase_assert(cameraStruct.orientation.x == 0.0f, "Expected 0 but got %f", cameraStruct.orientation.x);
	TestCase_assert(cameraStruct.orientation.y == 0.0f, "Expected 0 but got %f", cameraStruct.orientation.y);
	TestCase_assert(cameraStruct.orientation.z == 0.0f, "Expected 0 but got %f", cameraStruct.orientation.z);
	TestCase_assert(cameraStruct.orientation.w == 1.0f, "Expected 1 but got %f", cameraStruct.orientation.w);
	TestCase_assert(cameraStruct.position.x == 0.0f, "Expected 0 but got %f", cameraStruct.position.x);
	TestCase_assert(cameraStruct.position.y == 0.0f, "Expected 0 but got %f", cameraStruct.position.y);
	TestCase_assert(cameraStruct.position.z == 0.0f, "Expected 0 but got %f", cameraStruct.position.z);
	TestCase_assert(cameraStruct.dispose != NULL, "Method unexpectedly NULL");
	TestCase_assert(cameraStruct.apply != NULL, "Method unexpectedly NULL");
}

TEST_SUITE(CameraTest, testInit)
