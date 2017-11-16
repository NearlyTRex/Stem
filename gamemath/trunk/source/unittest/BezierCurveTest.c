#include "gamemath/BezierCurve.h"
#include "unittest/TestSuite.h"
#include <math.h>

#define assertVector2fApproximate(vector, expectedX, expectedY, epsilon) { \
	TestCase_assert(fabs(vector.x - expectedX) < epsilon, "Expected %f x but got %f (tolerance %f)", expectedX, vector.x, epsilon); \
	TestCase_assert(fabs(vector.y - expectedY) < epsilon, "Expected %f y but got %f (tolerance %f)", expectedY, vector.y, epsilon); \
}

#define EPSILON 0.00001f

static void testSample() {
	Vector2f result;
	
	result = BezierCurve_sample(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 1.0f), VECTOR2f(1.0f, 1.0f), 0.0f);
	assertVector2fApproximate(result, 0.0f, 0.0f, EPSILON);
	
	result = BezierCurve_sample(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 1.0f), VECTOR2f(1.0f, 1.0f), 0.5f);
	assertVector2fApproximate(result, 0.5f, 0.5f, EPSILON);
	
	result = BezierCurve_sample(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.0f), VECTOR2f(1.0f, 1.0f), VECTOR2f(1.0f, 1.0f), 1.0f);
	assertVector2fApproximate(result, 1.0f, 1.0f, EPSILON);
	
	result = BezierCurve_sample(VECTOR2f(5.0f, -1.0f), VECTOR2f(5.0f, -1.0f), VECTOR2f(-15.0f, 3.0f), VECTOR2f(-15.0f, 3.0f), 0.0f);
	assertVector2fApproximate(result, 5.0f, -1.0f, EPSILON);
	
	result = BezierCurve_sample(VECTOR2f(5.0f, -1.0f), VECTOR2f(5.0f, -1.0f), VECTOR2f(-15.0f, 3.0f), VECTOR2f(-15.0f, 3.0f), 0.5f);
	assertVector2fApproximate(result, -5.0f, 1.0f, EPSILON);
	
	result = BezierCurve_sample(VECTOR2f(5.0f, -1.0f), VECTOR2f(5.0f, -1.0f), VECTOR2f(-15.0f, 3.0f), VECTOR2f(-15.0f, 3.0f), 1.0f);
	assertVector2fApproximate(result, -15.0f, 3.0f, EPSILON);
	
	result = BezierCurve_sample(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.75f), VECTOR2f(0.25f, 1.0f), VECTOR2f(1.0f, 1.0f), 0.0f);
	assertVector2fApproximate(result, 0.0f, 0.0f, EPSILON);
	
	// {0.0, 0.1875}, {0.0625, 0.8125}, {0.4375, 1.0} = {0.015625, 0.34375}, {0.15625, 0.859375} = {0.05078125, 0.47265625}
	result = BezierCurve_sample(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.75f), VECTOR2f(0.25f, 1.0f), VECTOR2f(1.0f, 1.0f), 0.25f);
	assertVector2fApproximate(result, 0.05078125f, 0.47265625f, EPSILON);
	
	// {0.0, 0.375}, {0.125, 0.875}, {0.625, 1.0} = {0.0625, 0.625}, {0.375, 0.9375} = {0.21875, 0.78125}
	result = BezierCurve_sample(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.75f), VECTOR2f(0.25f, 1.0f), VECTOR2f(1.0f, 1.0f), 0.5f);
	assertVector2fApproximate(result, 0.21875f, 0.78125f, EPSILON);
	
	// {0.0, 0.5625}, {0.1875, 0.9375}, {0.8125, 1.0} = {0.140625, 0.84375}, {0.65625, 0.984375} = {0.52734375, 0.94921875}
	result = BezierCurve_sample(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.75f), VECTOR2f(0.25f, 1.0f), VECTOR2f(1.0f, 1.0f), 0.75f);
	assertVector2fApproximate(result, 0.52734375f, 0.94921875f, EPSILON);
	
	result = BezierCurve_sample(VECTOR2f(0.0f, 0.0f), VECTOR2f(0.0f, 0.75f), VECTOR2f(0.25f, 1.0f), VECTOR2f(1.0f, 1.0f), 1.0f);
	assertVector2fApproximate(result, 1.0f, 1.0f, EPSILON);
}

TEST_SUITE(BezierCurveTest, testSample)
