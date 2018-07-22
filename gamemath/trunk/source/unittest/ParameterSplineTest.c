#include "gamemath/ParameterSpline.h"
#include "unittest/TestSuite.h"
#include <math.h>

static void testInit() {
	ParameterSpline spline;
	struct ParameterSpline_curve curves1[] = {{1.0f, 0.0f, 1.0f, VECTOR2f_ZERO, VECTOR2f_ZERO}};
	struct ParameterSpline_curve curves2[] = {{2.0f, 1.0f, 3.0f, {-1.0f, -1.0f}, {1.0f, 1.0f}}, {3.0f, 0.0f, 1.0f, VECTOR2f_ZERO, VECTOR2f_ZERO}};
	
	spline = ParameterSpline_init(sizeof(curves1) / sizeof(curves1[0]), curves1);
	TestCase_assert(spline.curveCount == 1, "Expected 1 but got %u", spline.curveCount);
	TestCase_assert(spline.curves != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(spline.curveSampleIterations == 8, "Expected 8 but got %u", spline.curveSampleIterations);
	TestCase_assert(spline.computedLength == 1.0f, "Expected 1.0 but got %f", spline.computedLength);
	TestCase_assert(spline.curves[0].width == 1.0f, "Expected 1.0 but got %f", spline.curves[0].width);
	TestCase_assert(spline.curves[0].originY == 0.0f, "Expected 0.0 but got %f", spline.curves[0].originY);
	TestCase_assert(spline.curves[0].endpointY == 1.0f, "Expected 1.0 but got %f", spline.curves[0].endpointY);
	TestCase_assert(spline.curves[0].originHandle.x == 0.0f, "Expected 0.0 but got %f", spline.curves[0].originHandle.x);
	TestCase_assert(spline.curves[0].originHandle.y == 0.0f, "Expected 0.0 but got %f", spline.curves[0].originHandle.y);
	TestCase_assert(spline.curves[0].endpointHandle.x == 0.0f, "Expected 0.0 but got %f", spline.curves[0].endpointHandle.x);
	TestCase_assert(spline.curves[0].endpointHandle.y == 0.0f, "Expected 0.0 but got %f", spline.curves[0].endpointHandle.y);
	ParameterSpline_dispose(&spline);
	
	spline = ParameterSpline_init(sizeof(curves2) / sizeof(curves2[0]), curves2);
	TestCase_assert(spline.curveCount == 2, "Expected 2 but got %u", spline.curveCount);
	TestCase_assert(spline.curves != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(spline.curveSampleIterations == 8, "Expected 8 but got %u", spline.curveSampleIterations);
	TestCase_assert(spline.computedLength == 5.0f, "Expected 5.0 but got %f", spline.computedLength);
	TestCase_assert(spline.curves[0].width == 2.0f, "Expected 2.0 but got %f", spline.curves[0].width);
	TestCase_assert(spline.curves[0].originY == 1.0f, "Expected 1.0 but got %f", spline.curves[0].originY);
	TestCase_assert(spline.curves[0].endpointY == 3.0f, "Expected 3.0 but got %f", spline.curves[0].endpointY);
	TestCase_assert(spline.curves[0].originHandle.x == -1.0f, "Expected -1.0 but got %f", spline.curves[0].originHandle.x);
	TestCase_assert(spline.curves[0].originHandle.y == -1.0f, "Expected -1.0 but got %f", spline.curves[0].originHandle.y);
	TestCase_assert(spline.curves[0].endpointHandle.x == 1.0f, "Expected 1.0 but got %f", spline.curves[0].endpointHandle.x);
	TestCase_assert(spline.curves[0].endpointHandle.y == 1.0f, "Expected 1.0 but got %f", spline.curves[0].endpointHandle.y);
	TestCase_assert(spline.curves[1].width == 3.0f, "Expected 3.0 but got %f", spline.curves[1].width);
	TestCase_assert(spline.curves[1].originY == 0.0f, "Expected 0.0 but got %f", spline.curves[1].originY);
	TestCase_assert(spline.curves[1].endpointY == 1.0f, "Expected 1.0 but got %f", spline.curves[1].endpointY);
	TestCase_assert(spline.curves[1].originHandle.x == 0.0f, "Expected 0.0 but got %f", spline.curves[1].originHandle.x);
	TestCase_assert(spline.curves[1].originHandle.y == 0.0f, "Expected 0.0 but got %f", spline.curves[1].originHandle.y);
	TestCase_assert(spline.curves[1].endpointHandle.x == 0.0f, "Expected 0.0 but got %f", spline.curves[1].endpointHandle.x);
	TestCase_assert(spline.curves[1].endpointHandle.y == 0.0f, "Expected 0.0 but got %f", spline.curves[1].endpointHandle.y);
	ParameterSpline_dispose(&spline);
}

#define THRESHOLD 0.00001f

void testSample() {
	ParameterSpline spline;
	struct ParameterSpline_curve curves1[] = {{1.0f, 2.0f, -2.0f, VECTOR2f_ZERO, VECTOR2f_ZERO}};
	struct ParameterSpline_curve curves2[] = {
		{0.5f, 0.0f, 1.0f, VECTOR2f_ZERO, VECTOR2f_ZERO},
		{0.5f, 1.0f, 0.0f, {0.5f, 0.0f}, {-0.5f, 0.0f}}
	};
	float sample;
	
	spline = ParameterSpline_init(sizeof(curves1) / sizeof(curves1[0]), curves1);
	
	sample = ParameterSpline_sample(&spline, 0.0f);
	TestCase_assert(fabs(sample - 2.0f) < THRESHOLD, "Expected 2.0 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.25f);
	TestCase_assert(fabs(sample - 1.0f) < THRESHOLD, "Expected 1.0 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.5f);
	TestCase_assert(fabs(sample - 0.0f) < THRESHOLD, "Expected 0.0 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.75f);
	TestCase_assert(fabs(sample - -1.0f) < THRESHOLD, "Expected -1.0 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 1.0f);
	TestCase_assert(fabs(sample - -2.0f) < THRESHOLD, "Expected -2.0 but got %f\n", sample);
	
	ParameterSpline_dispose(&spline);
	spline = ParameterSpline_init(sizeof(curves2) / sizeof(curves2[0]), curves2);
	
	sample = ParameterSpline_sample(&spline, 0.0f);
	TestCase_assert(fabs(sample - 0.0f) < THRESHOLD, "Expected 0.0 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.125f);
	TestCase_assert(fabs(sample - 0.25f) < THRESHOLD, "Expected 0.25 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.25f);
	TestCase_assert(fabs(sample - 0.5f) < THRESHOLD, "Expected 0.5 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.375f);
	TestCase_assert(fabs(sample - 0.75f) < THRESHOLD, "Expected 0.75 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.5f);
	TestCase_assert(fabs(sample - 1.0f) < THRESHOLD, "Expected 1.0 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.625f);
	TestCase_assert(fabs(sample - 0.894099f) < THRESHOLD, "Expected 0.894099 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.75f);
	TestCase_assert(fabs(sample - 0.5f) < THRESHOLD, "Expected 0.5 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 0.875f);
	TestCase_assert(fabs(sample - 0.105901f) < THRESHOLD, "Expected 0.105901 but got %f\n", sample);
	sample = ParameterSpline_sample(&spline, 1.0f);
	TestCase_assert(fabs(sample - 0.0f) < THRESHOLD, "Expected 0.0 but got %f\n", sample);
	
	ParameterSpline_dispose(&spline);
}

TEST_SUITE(ParameterSplineTest,
           testInit,
           testSample)
