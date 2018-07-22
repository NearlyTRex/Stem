#include "unittest/TestSuite.h"
#include "gamemath/Interpolation.h"
#include <math.h>
#include <stdio.h>

static void testInit() {
	struct interpolationContext context;
	
	context = interpolationContextInit(1.0f, 2.0f, 3.0, false, InterpolationCurveFunc_linear, NULL);
	TestCase_assert(context.startValue == 1.0f, "Expected 1.0 but got %f", context.startValue);
	TestCase_assert(context.endValue == 2.0f, "Expected 2.0 but got %f", context.endValue);
	TestCase_assert(context.currentValue == 1.0f, "Expected 1.0 but got %f", context.currentValue);
	TestCase_assert(context.currentProgress == 0.0f, "Expected 0.0 but got %f", context.currentProgress);
	TestCase_assert(!context.allowExtrapolation, "Expected false but got true");
	TestCase_assert(context.interval == 3.0, "Expected 3.0 but got %f", context.interval);
	TestCase_assert(context.startTime == 0.0f, "Expected 0.0 but got %f", context.startTime);
	TestCase_assert(context.currentTime == 0.0f, "Expected 0.0 but got %f", context.currentTime);
	TestCase_assert(context.curveFunc == InterpolationCurveFunc_linear, "Expected %p but got %p", InterpolationCurveFunc_linear, context.curveFunc);
	TestCase_assert(context.curveFuncContext == NULL, "Expected NULL but got %p", context.curveFuncContext);
	
	context = interpolationContextInit(2.0f, 1.0f, 5.0, true, InterpolationCurveFunc_easeInSin, (void *) 0x1);
	TestCase_assert(context.startValue == 2.0f, "Expected 2.0 but got %f", context.startValue);
	TestCase_assert(context.endValue == 1.0f, "Expected 1.0 but got %f", context.endValue);
	TestCase_assert(context.currentValue == 2.0f, "Expected 2.0 but got %f", context.currentValue);
	TestCase_assert(context.currentProgress == 0.0f, "Expected 0.0 but got %f", context.currentProgress);
	TestCase_assert(context.allowExtrapolation, "Expected true but got false");
	TestCase_assert(context.interval == 5.0, "Expected 5.0 but got %f", context.interval);
	TestCase_assert(context.startTime == 0.0f, "Expected 0.0 but got %f", context.startTime);
	TestCase_assert(context.currentTime == 0.0f, "Expected 0.0 but got %f", context.currentTime);
	TestCase_assert(context.curveFunc == InterpolationCurveFunc_easeInSin, "Expected %p but got %p", InterpolationCurveFunc_easeInSin, context.curveFunc);
	TestCase_assert(context.curveFuncContext == (void *) 0x1, "Expected 0x1 but got %p", context.curveFuncContext);
}

static void testAdvance() {
	struct interpolationContext context;
	
	context = interpolationContextInit(2.0f, 4.0f, 8.0, false, InterpolationCurveFunc_linear, NULL);
	
	interpolationAdvance(&context, 2.0);
	TestCase_assert(context.currentTime == 2.0, "Expected 2.0 but got %f", context.currentTime);
	TestCase_assert(context.currentProgress == 0.25f, "Expected 0.25 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 2.5f, "Expected 2.5 but got %f", context.currentValue);
	
	interpolationAdvance(&context, 4.0);
	TestCase_assert(context.currentTime == 6.0, "Expected 6.0 but got %f", context.currentTime);
	TestCase_assert(context.currentProgress == 0.75f, "Expected 0.75 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 3.5f, "Expected 3.5 but got %f", context.currentValue);
}

static void testSetCurrentTime() {
	struct interpolationContext context;
	
	context = interpolationContextInit(1.0f, 3.0f, 2.0, false, InterpolationCurveFunc_linear, NULL);
	
	interpolationStart(&context, 1.0);
	TestCase_assert(context.startTime == 1.0, "Expected 1.0 but got %f", context.startTime);
	TestCase_assert(context.currentTime == 1.0, "Expected 1.0 but got %f", context.currentTime);
	
	interpolationSetCurrentTime(&context, 1.5);
	TestCase_assert(context.currentTime == 1.5, "Expected 1.5 but got %f", context.currentTime);
	TestCase_assert(context.currentProgress == 0.25f, "Expected 0.25 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 1.5f, "Expected 1.5 but got %f", context.currentValue);
	
	interpolationSetCurrentTime(&context, 2.5);
	TestCase_assert(context.currentTime == 2.5, "Expected 2.5 but got %f", context.currentTime);
	TestCase_assert(context.currentProgress == 0.75f, "Expected 0.75 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 2.5f, "Expected 2.5 but got %f", context.currentValue);
	
	interpolationSetCurrentTime(&context, 2.0);
	TestCase_assert(context.currentTime == 2.0, "Expected 2.0 but got %f", context.currentTime);
	TestCase_assert(context.currentProgress == 0.5f, "Expected 0.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 2.0f, "Expected 2.0 but got %f", context.currentValue);
}

static void testSetProgress() {
	struct interpolationContext context;
	
	context = interpolationContextInit(2.0f, 0.0f, 1.0, false, InterpolationCurveFunc_linear, NULL);
	
	interpolationStart(&context, 3.0);
	interpolationSetProgress(&context, 0.5f);
	TestCase_assert(context.currentProgress == 0.5f, "Expected 0.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 1.0f, "Expected 1.0 but got %f", context.currentValue);
	TestCase_assert(context.currentTime == 3.5, "Expected 3.5 but got %f", context.currentTime);
	
	interpolationSetProgress(&context, 0.75f);
	TestCase_assert(context.currentProgress == 0.75f, "Expected 0.75 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 0.5f, "Expected 0.5 but got %f", context.currentValue);
	TestCase_assert(context.currentTime == 3.75, "Expected 3.75 but got %f", context.currentTime);
	
	interpolationSetProgress(&context, 0.25f);
	TestCase_assert(context.currentProgress == 0.25f, "Expected 0.25 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 1.5f, "Expected 1.5 but got %f", context.currentValue);
	TestCase_assert(context.currentTime == 3.25, "Expected 3.25 but got %f", context.currentTime);
}

static void testClamping() {
	struct interpolationContext context;
	
	context = interpolationContextInit(3.0f, 1.0f, 1.5, false, InterpolationCurveFunc_linear, NULL);
	interpolationAdvance(&context, -1.5);
	TestCase_assert(context.currentProgress == 0.0, "Expected 0.0 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 3.0f, "Expected 3.0f but got %f", context.currentValue);
	
	interpolationAdvance(&context, 2.25);
	TestCase_assert(context.currentProgress == 0.5, "Expected 0.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 2.0f, "Expected 2.0f but got %f", context.currentValue);
	
	interpolationAdvance(&context, 1.5);
	TestCase_assert(context.currentProgress == 1.0, "Expected 1.0 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 1.0f, "Expected 1.0f but got %f", context.currentValue);
	
	interpolationStart(&context, 1.0);
	interpolationSetCurrentTime(&context, -0.5);
	TestCase_assert(context.currentProgress == 0.0, "Expected 0.0 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 3.0f, "Expected 3.0f but got %f", context.currentValue);
	
	interpolationSetCurrentTime(&context, 1.75);
	TestCase_assert(context.currentProgress == 0.5, "Expected 0.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 2.0f, "Expected 2.0f but got %f", context.currentValue);
	
	interpolationSetCurrentTime(&context, 3.25);
	TestCase_assert(context.currentProgress == 1.0, "Expected 1.0 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 1.0f, "Expected 1.0f but got %f", context.currentValue);
	
	interpolationSetProgress(&context, -1.0f);
	TestCase_assert(context.currentProgress == 0.0, "Expected 0.0 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 3.0f, "Expected 3.0f but got %f", context.currentValue);
	
	interpolationSetProgress(&context, 0.5f);
	TestCase_assert(context.currentProgress == 0.5, "Expected 0.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 2.0f, "Expected 2.0f but got %f", context.currentValue);
	
	interpolationSetProgress(&context, 1.5f);
	TestCase_assert(context.currentProgress == 1.0, "Expected 1.0 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 1.0f, "Expected 1.0f but got %f", context.currentValue);
}

static void testExtrapolation() {
	struct interpolationContext context;
	
	context = interpolationContextInit(3.0f, 1.0f, 1.5, true, InterpolationCurveFunc_linear, NULL);
	interpolationAdvance(&context, -1.5);
	TestCase_assert(context.currentProgress == -1.0, "Expected -1.0 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 5.0f, "Expected 5.0f but got %f", context.currentValue);
	
	interpolationAdvance(&context, 2.25);
	TestCase_assert(context.currentProgress == 0.5, "Expected 0.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 2.0f, "Expected 2.0f but got %f", context.currentValue);
	
	interpolationAdvance(&context, 1.5);
	TestCase_assert(context.currentProgress == 1.5, "Expected 1.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 0.0f, "Expected 0.0f but got %f", context.currentValue);
	
	interpolationStart(&context, 1.0);
	interpolationSetCurrentTime(&context, -0.5);
	TestCase_assert(context.currentProgress == -1.0, "Expected -1.0 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 5.0f, "Expected 5.0f but got %f", context.currentValue);
	
	interpolationSetCurrentTime(&context, 1.75);
	TestCase_assert(context.currentProgress == 0.5, "Expected 0.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 2.0f, "Expected 2.0f but got %f", context.currentValue);
	
	interpolationSetCurrentTime(&context, 3.25);
	TestCase_assert(context.currentProgress == 1.5, "Expected 1.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 0.0f, "Expected 0.0f but got %f", context.currentValue);
	
	interpolationSetProgress(&context, -1.0f);
	TestCase_assert(context.currentProgress == -1.0, "Expected -1.0 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 5.0f, "Expected 5.0f but got %f", context.currentValue);
	
	interpolationSetProgress(&context, 0.5f);
	TestCase_assert(context.currentProgress == 0.5, "Expected 0.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 2.0f, "Expected 2.0f but got %f", context.currentValue);
	
	interpolationSetProgress(&context, 1.5f);
	TestCase_assert(context.currentProgress == 1.5, "Expected 1.5 but got %f", context.currentProgress);
	TestCase_assert(context.currentValue == 0.0f, "Expected 0.0f but got %f", context.currentValue);
}

static float curveFuncExtend(float value, void * context) {
	return -1.0f + value * 3.0f;
}

static float curveFuncReverse(float value, void * context) {
	return 1.0f - value;
}

static void testCurveFunc() {
	struct interpolationContext context;
	
	context = interpolationContextInit(2.0f, 4.0f, 2.0, false, curveFuncExtend, NULL);
	TestCase_assert(context.currentValue == 0.0f, "Expected 0.0 but got %f", context.currentValue);
	interpolationSetProgress(&context, 0.25f);
	TestCase_assert(context.currentValue == 1.5f, "Expected 1.5 but got %f", context.currentValue);
	interpolationAdvance(&context, 0.5f);
	TestCase_assert(context.currentValue == 3.0f, "Expected 3.0 but got %f", context.currentValue);
	interpolationSetCurrentTime(&context, 1.5f);
	TestCase_assert(context.currentValue == 4.5f, "Expected 4.5 but got %f", context.currentValue);
	interpolationStart(&context, 0.0);
	TestCase_assert(context.currentValue == 0.0f, "Expected 0.0 but got %f", context.currentValue);
	
	context = interpolationContextInit(2.0f, 4.0f, 2.0, false, curveFuncReverse, NULL);
	TestCase_assert(context.currentValue == 4.0f, "Expected 4.0 but got %f", context.currentValue);
	interpolationAdvance(&context, 0.5f);
	TestCase_assert(context.currentValue == 3.5f, "Expected 3.5 but got %f", context.currentValue);
	interpolationSetProgress(&context, 0.5f);
	TestCase_assert(context.currentValue == 3.0f, "Expected 3.0 but got %f", context.currentValue);
	interpolationSetCurrentTime(&context, 1.5f);
	TestCase_assert(context.currentValue == 2.5f, "Expected 2.5 but got %f", context.currentValue);
	interpolationStart(&context, 0.0);
	TestCase_assert(context.currentValue == 4.0f, "Expected 4.0 but got %f", context.currentValue);
}

#define EPSILON 0.00001

static void testBuiltInCurveFuncs() {
	float value;
	
	value = InterpolationCurveFunc_linear(0.0f, NULL);
	TestCase_assert(value == 0.0f, "Expected 0.0 but got %f", value);
	value = InterpolationCurveFunc_linear(0.5f, NULL);
	TestCase_assert(value == 0.5f, "Expected 0.5 but got %f", value);
	value = InterpolationCurveFunc_linear(1.0f, NULL);
	TestCase_assert(value == 1.0f, "Expected 1.0 but got %f", value);
	
	value = InterpolationCurveFunc_easeInSin(0.0f, NULL);
	TestCase_assert(value == 0.0f, "Expected 0.0 but got %f", value);
	value = InterpolationCurveFunc_easeInSin(0.5f, NULL);
	TestCase_assert(fabs(value - (1.0 - cos(M_PI * 0.25))) < EPSILON, "Expected %f but got %f", 1.0 - cos(M_PI * 0.25), value);
	value = InterpolationCurveFunc_easeInSin(1.0f, NULL);
	TestCase_assert(value == 1.0f, "Expected 1.0 but got %f", value);
	
	value = InterpolationCurveFunc_easeOutSin(0.0f, NULL);
	TestCase_assert(value == 0.0f, "Expected 0.0 but got %f", value);
	value = InterpolationCurveFunc_easeOutSin(0.5f, NULL);
	TestCase_assert(fabs(value - sin(M_PI * 0.25)) < EPSILON, "Expected %f but got %f", sin(M_PI * 0.25), value);
	value = InterpolationCurveFunc_easeOutSin(1.0f, NULL);
	TestCase_assert(value == 1.0f, "Expected 1.0 but got %f", value);
	
	value = InterpolationCurveFunc_easeInOutSin(0.0f, NULL);
	TestCase_assert(value == 0.0f, "Expected 0.0 but got %f", value);
	value = InterpolationCurveFunc_easeInOutSin(0.25f, NULL);
	TestCase_assert(fabs(value - (1.0 - cos(M_PI * 0.25)) * 0.5) < EPSILON, "Expected %f but got %f", (1.0 - cos(M_PI * 0.25)) * 0.5, value);
	value = InterpolationCurveFunc_easeInOutSin(0.5f, NULL);
	TestCase_assert(fabs(value - 0.5f) < EPSILON, "Expected 0.5 but got %f", value);
	value = InterpolationCurveFunc_easeInOutSin(0.75f, NULL);
	TestCase_assert(fabs(value - (0.5 + sin(M_PI * 0.25) * 0.5)) < EPSILON, "Expected %f but got %f", 0.5 + sin(M_PI * 0.25) * 0.5, value);
	value = InterpolationCurveFunc_easeInOutSin(1.0f, NULL);
	TestCase_assert(value == 1.0f, "Expected 1.0 but got %f", value);
}

TEST_SUITE(InterpolationTest,
           testInit,
           testAdvance,
           testSetCurrentTime,
           testSetProgress,
           testClamping,
           testExtrapolation,
           testCurveFunc,
           testBuiltInCurveFuncs)
