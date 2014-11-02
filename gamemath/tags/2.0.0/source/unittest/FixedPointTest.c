#include "unittest/TestSuite.h"
#include "3dmath/FixedPoint.h"
#include <stdbool.h>
#include <stdio.h>

static void testFloatToFixed() {
	fixed16_16 result;
	
	result = floatToFixed16_16(0.0f);
	TestCase_assert(result == 0x00000000, "Expected 0x00000000 but got 0x%08X", result);
	result = floatToFixed16_16(1.5f);
	TestCase_assert(result == 0x00018000, "Expected 0x00018000 but got 0x%08X", result);
	result = floatToFixed16_16(-10.25f);
	TestCase_assert(result == (fixed16_16) -0x000A4000, "Expected 0xFFF5C000 but got 0x%08X", result);
	result = floatToFixed16_16(4369.0666656494140625f);
	TestCase_assert(result == 0x11111120, "Expected 0x11111120 but got 0x%08X", result);
}

static void testFixedToFloat() {
	float result;
	
	result = fixed16_16ToFloat(0x00000000);
	TestCase_assert(result == 0.0f, "Expected 0.0 but got %f", result);
	result = fixed16_16ToFloat(0x00018000);
	TestCase_assert(result == 1.5f, "Expected 1.5 but got %f", result);
	result = fixed16_16ToFloat(0xFFF5C000);
	TestCase_assert(result == -10.25f, "Expected -10.25 but got %f", result);
	result = fixed16_16ToFloat(0x11111120);
	TestCase_assert(result == 4369.0666656494140625f, "Expected 4369.0666656494140625 but got %f", result);
}

static void testDoubleToFixed() {
	fixed16_16 result;
	
	result = doubleToFixed16_16(0.0f);
	TestCase_assert(result == 0x00000000, "Expected 0x00000000 but got 0x%08X", result);
	result = doubleToFixed16_16(1.5);
	TestCase_assert(result == 0x00018000, "Expected 0x00018000 but got 0x%08X", result);
	result = doubleToFixed16_16(-10.25);
	TestCase_assert(result == (fixed16_16) -0x000A4000, "Expected 0xFFF5C000 but got 0x%08X", result);
	result = doubleToFixed16_16(4369.0666656494140625);
	TestCase_assert(result == 0x11111111, "Expected 0x11111111 but got 0x%08X", result);
}

static void testFixedToDouble() {
	double result;
	
	result = fixed16_16ToDouble(0x00000000);
	TestCase_assert(result == 0.0, "Expected 0.0 but got %f", result);
	result = fixed16_16ToDouble(0x00018000);
	TestCase_assert(result == 1.5, "Expected 1.5 but got %f", result);
	result = fixed16_16ToDouble(0xFFF5C000);
	TestCase_assert(result == -10.25, "Expected -10.25 but got %f", result);
	result = fixed16_16ToDouble(0x11111111);
	TestCase_assert(result == 4369.0666656494140625, "Expected 4369.0666656494140625 but got %f", result);
}

static void testMultiply() {
	fixed16_16 result;
	
	result = fixed16_16_multiply(0x10000, 0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_multiply(0x20000, 0x4000);
	TestCase_assert(result == 0x8000, "Expected 0x8000 but got 0x%X", result);
	result = fixed16_16_multiply(0x0FFFFFFF, 0x00100000);
	TestCase_assert(result == (fixed16_16) 0xFFFFFFF0, "Expected 0xFFFFFFF0 but got 0x%X", result);
	result = fixed16_16_multiply((fixed16_16) 0xFFFFFFF0, 0x00000100);
	TestCase_assert(result == (fixed16_16) 0xFFFFFFFF, "Expected 0xFFFFFFFF but got 0x%X", result);
}

static void testDivide() {
	fixed16_16 result;
	
	result = fixed16_16_divide(0x10000, 0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_divide(0x20000, 0x4000);
	TestCase_assert(result == 0x80000, "Expected 0x8000 but got 0x%X", result);
	result = fixed16_16_divide(0x7FFFFFFF, 0x00100000);
	TestCase_assert(result == 0x07FFFFFF, "Expected 0x07FFFFFF but got 0x%X", result);
	result = fixed16_16_divide((fixed16_16) 0xFFFFFFF0, 0x00100000);
	TestCase_assert(result == (fixed16_16) 0xFFFFFFFF, "Expected 0xFFFFFFFF but got 0x%X", result);
}

static void testRound() {
	fixed16_16 result;
	
	result = fixed16_16_round(0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_round(0x27FFF);
	TestCase_assert(result == 0x20000, "Expected 0x20000 but got 0x%X", result);
	result = fixed16_16_round(0x28000);
	TestCase_assert(result == 0x30000, "Expected 0x20000 but got 0x%X", result);
	result = fixed16_16_round((fixed16_16) 0xFFFE8000);
	TestCase_assert(result == (fixed16_16) 0xFFFF0000, "Expected 0xFFFF0000 but got 0x%X", result);
	result = fixed16_16_round((fixed16_16) 0xFFFE7FFF);
	TestCase_assert(result == (fixed16_16) 0xFFFE0000, "Expected 0xFFFE0000 but got 0x%X", result);
	result = fixed16_16_round(0x27FFF);
	TestCase_assert(result == 0x20000, "Expected 0x20000 but got 0x%X", result);
	result = fixed16_16_round(0x28000);
	TestCase_assert(result == 0x30000, "Expected 0x20000 but got 0x%X", result);
	result = fixed16_16_round((fixed16_16) 0xFFFF8000);
	TestCase_assert(result == 0x00000, "Expected 0x00000 but got 0x%X", result);
	result = fixed16_16_round((fixed16_16) 0xFFFF7FFF);
	TestCase_assert(result == (fixed16_16) 0xFFFF0000, "Expected 0xFFFF0000 but got 0x%X", result);
}

static void testAbs() {
	fixed16_16 result;
	
	result = fixed16_16_abs(0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_abs((fixed16_16) -0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_abs((fixed16_16) -0x20000);
	TestCase_assert(result == 0x20000, "Expected 0x20000 but got 0x%X", result);
}

static void testSqrt() {
	fixed16_16 result;
	
	result = fixed16_16_sqrt(0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_sqrt(0x90000);
	TestCase_assert(result == 0x30000, "Expected 0x30000 but got 0x%X", result);
	result = fixed16_16_sqrt(0x20000);
	TestCase_assert(result == 0x16A0A, "Expected 0x16A0A but got 0x%X", result);
	result = fixed16_16_sqrt(0x8000);
	TestCase_assert(result == 0xB505, "Expected 0xB505 but got 0x%X", result);
}

TEST_SUITE(FixedPointTest,
           testFloatToFixed,
           testFixedToFloat,
           testDoubleToFixed,
           testFixedToDouble,
           testMultiply,
           testDivide,
           testRound,
           testAbs,
           testSqrt)
