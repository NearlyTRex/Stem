#include "gamemath/MouseCoordinateTransforms.h"
#include "unittest/TestSuite.h"

static void testSignedCenter() {
	Vector2f position;
	
	position = transformMousePosition_signedCenter(VECTOR2f(1.0f, 1.0f), 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.0f, "Expected {0.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenter(VECTOR2f(0.0f, 0.0f), 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == -1.0f && position.y == 1.0f, "Expected {-1.0, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenter(VECTOR2f(1.0f, 1.0f), 4.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == -1.0f && position.y == 0.0f, "Expected {-1.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenter(VECTOR2f(1.0f, 1.0f), 2.0f, 4.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.5f, "Expected {0.0, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenter(VECTOR2f(0.0f, 0.0f), 2.0f, 2.0f, 2.0f);
	TestCase_assert(position.x == -2.0f && position.y == 2.0f, "Expected {-2.0, 2.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenter(VECTOR2f(1.5f, 1.5f), 2.0f, 2.0f, 4.0f);
	TestCase_assert(position.x == 2.0f && position.y == -2.0f, "Expected {2.0, -2.0} but got {%f, %f}", position.x, position.y);
}

static void testSignedCenterFlipped() {
	Vector2f position;
	
	position = transformMousePosition_signedCenterFlipped(VECTOR2f(1.0f, 1.0f), 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.0f, "Expected {0.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterFlipped(VECTOR2f(0.0f, 0.0f), 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == -1.0f && position.y == -1.0f, "Expected {-1.0, -1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterFlipped(VECTOR2f(1.0f, 1.0f), 4.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == -1.0f && position.y == 0.0f, "Expected {-1.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterFlipped(VECTOR2f(1.0f, 1.0f), 2.0f, 4.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == -0.5f, "Expected {0.0, -0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterFlipped(VECTOR2f(0.0f, 0.0f), 2.0f, 2.0f, 2.0f);
	TestCase_assert(position.x == -2.0f && position.y == -2.0f, "Expected {-2.0, -2.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterFlipped(VECTOR2f(1.5f, 1.5f), 2.0f, 2.0f, 4.0f);
	TestCase_assert(position.x == 2.0f && position.y == 2.0f, "Expected {2.0, 2.0} but got {%f, %f}", position.x, position.y);
}

static void testSignedCenterAspect() {
	Vector2f position;
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(0.0f, 0.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == -0.5f && position.y == 0.5f, "Expected {-0.5, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(1.0f, 1.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.0f, "Expected {0.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(2.0f, 2.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.5f && position.y == -0.5f, "Expected {0.5, -0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(1.0f, 2.0f), 4.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == -0.5f && position.y == -0.5f, "Expected {-0.5, -0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(3.0f, 0.0f), 4.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.5f && position.y == 0.5f, "Expected {0.5, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(0.0f, 3.0f), 2.0f, 4.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == -0.5f && position.y == -0.5f, "Expected {-0.5, -0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(2.0f, 1.0f), 2.0f, 4.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.5f && position.y == 0.5f, "Expected {0.5, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(0.0f, 1.5f), 2.0f, 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == -1.0f && position.y == -0.5f, "Expected {-1.0, -0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(2.0f, 0.5f), 2.0f, 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == 1.0f && position.y == 0.5f, "Expected {1.0, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(0.5f, 2.0f), 2.0f, 2.0f, 1.0f, 2.0f);
	TestCase_assert(position.x == -0.5f && position.y == -1.0f, "Expected {-0.5, -1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspect(VECTOR2f(1.5f, 0.0f), 2.0f, 2.0f, 1.0f, 2.0f);
	TestCase_assert(position.x == 0.5f && position.y == 1.0f, "Expected {0.5, 1.0} but got {%f, %f}", position.x, position.y);
}

static void testSignedCenterAspectFlipped() {
	Vector2f position;
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(0.0f, 0.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == -0.5f && position.y == -0.5f, "Expected {-0.5, -0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(1.0f, 1.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.0f, "Expected {0.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(2.0f, 2.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.5f && position.y == 0.5f, "Expected {0.5, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(1.0f, 2.0f), 4.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == -0.5f && position.y == 0.5f, "Expected {-0.5, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(3.0f, 0.0f), 4.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.5f && position.y == -0.5f, "Expected {0.5, -0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(0.0f, 3.0f), 2.0f, 4.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == -0.5f && position.y == 0.5f, "Expected {-0.5, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(2.0f, 1.0f), 2.0f, 4.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.5f && position.y == -0.5f, "Expected {0.5, -0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(0.0f, 1.5f), 2.0f, 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == -1.0f && position.y == 0.5f, "Expected {-1.0, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(2.0f, 0.5f), 2.0f, 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == 1.0f && position.y == -0.5f, "Expected {1.0, -0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(0.5f, 2.0f), 2.0f, 2.0f, 1.0f, 2.0f);
	TestCase_assert(position.x == -0.5f && position.y == 1.0f, "Expected {-0.5, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_signedCenterAspectFlipped(VECTOR2f(1.5f, 0.0f), 2.0f, 2.0f, 1.0f, 2.0f);
	TestCase_assert(position.x == 0.5f && position.y == -1.0f, "Expected {0.5, -1.0} but got {%f, %f}", position.x, position.y);
}

static void testAspectBoxLowerLeft() {
	Vector2f position;
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(0.0f, 0.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 1.0f, "Expected {0.0, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(1.0f, 1.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.5f && position.y == 0.5f, "Expected {0.5, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(2.0f, 2.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 1.0f && position.y == 0.0f, "Expected {1.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(1.0f, 2.0f), 4.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.0f, "Expected {0.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(3.0f, 0.0f), 4.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 1.0f && position.y == 1.0f, "Expected {1.0, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(0.0f, 3.0f), 2.0f, 4.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.0f, "Expected {0.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(2.0f, 1.0f), 2.0f, 4.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 1.0f && position.y == 1.0f, "Expected {1.0, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(0.0f, 1.5f), 2.0f, 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.0f, "Expected {0.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(2.0f, 0.5f), 2.0f, 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == 2.0f && position.y == 1.0f, "Expected {2.0, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(0.5f, 2.0f), 2.0f, 2.0f, 1.0f, 2.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.0f, "Expected {0.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxLowerLeft(VECTOR2f(1.5f, 0.0f), 2.0f, 2.0f, 1.0f, 2.0f);
	TestCase_assert(position.x == 1.0f && position.y == 2.0f, "Expected {1.0, 2.0} but got {%f, %f}", position.x, position.y);
}

static void testAspectBoxUpperLeft() {
	Vector2f position;
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(0.0f, 0.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 0.0f, "Expected {0.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(1.0f, 1.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.5f && position.y == 0.5f, "Expected {0.5, 0.5} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(2.0f, 2.0f), 2.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 1.0f && position.y == 1.0f, "Expected {1.0, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(1.0f, 2.0f), 4.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 1.0f, "Expected {0.0, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(3.0f, 0.0f), 4.0f, 2.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 1.0f && position.y == 0.0f, "Expected {1.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(0.0f, 3.0f), 2.0f, 4.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 1.0f, "Expected {0.0, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(2.0f, 1.0f), 2.0f, 4.0f, 1.0f, 1.0f);
	TestCase_assert(position.x == 1.0f && position.y == 0.0f, "Expected {1.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(0.0f, 1.5f), 2.0f, 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == 0.0f && position.y == 1.0f, "Expected {0.0, 1.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(2.0f, 0.5f), 2.0f, 2.0f, 2.0f, 1.0f);
	TestCase_assert(position.x == 2.0f && position.y == 0.0f, "Expected {2.0, 0.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(0.5f, 2.0f), 2.0f, 2.0f, 1.0f, 2.0f);
	TestCase_assert(position.x == 0.0f && position.y == 2.0f, "Expected {0.0, 2.0} but got {%f, %f}", position.x, position.y);
	
	position = transformMousePosition_aspectBoxUpperLeft(VECTOR2f(1.5f, 0.0f), 2.0f, 2.0f, 1.0f, 2.0f);
	TestCase_assert(position.x == 1.0f && position.y == 0.0f, "Expected {1.0, 0.0} but got {%f, %f}", position.x, position.y);
}

TEST_SUITE(MouseCoordinateTransformsTest,
           testSignedCenter,
           testSignedCenterFlipped,
           testSignedCenterAspect,
           testSignedCenterAspectFlipped,
           testAspectBoxLowerLeft,
           testAspectBoxUpperLeft)
