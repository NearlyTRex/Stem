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
	TestCase_assert(result == (fixed16_16) 0xFFF5C000, "Expected 0xFFF5C000 but got 0x%08X", result);
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
	TestCase_assert(result == (fixed16_16) 0xFFF5C000, "Expected 0xFFF5C000 but got 0x%08X", result);
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
	result = fixed16_16_divide(FIXED_16_16_MAX, 0x00100000);
	TestCase_assert(result == 0x07FFFFFF, "Expected 0x07FFFFFF but got 0x%X", result);
	result = fixed16_16_divide((fixed16_16) 0xFFFFFFF0, 0x00100000);
	TestCase_assert(result == (fixed16_16) 0xFFFFFFFF, "Expected 0xFFFFFFFF but got 0x%X", result);
	result = fixed16_16_divide(0x00010000, 0x0);
	TestCase_assert(result == FIXED_16_16_INF, "Expected 0x%X but got 0x%X", FIXED_16_16_INF, result);
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
	
	result = fixed16_16_sqrt(0x0);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_sqrt(0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_sqrt(0x90000);
	TestCase_assert(result == 0x30000, "Expected 0x30000 but got 0x%X", result);
	result = fixed16_16_sqrt(0x20000);
	TestCase_assert(result == 0x16A09, "Expected 0x16A09 but got 0x%X", result);
	result = fixed16_16_sqrt(0x8000);
	TestCase_assert(result == 0xB504, "Expected 0xB504 but got 0x%X", result);
	result = fixed16_16_sqrt((fixed16_16) 0xFFFF0000);
	TestCase_assert(result == FIXED_16_16_NAN, "Expected 0x%X but got 0x%X", FIXED_16_16_NAN, result);
}

static void testFloor() {
	fixed16_16 result;
	
	result = fixed16_16_floor(0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_floor(0x20001);
	TestCase_assert(result == 0x20000, "Expected 0x20000 but got 0x%X", result);
	result = fixed16_16_floor(0x3FFFF);
	TestCase_assert(result == 0x30000, "Expected 0x30000 but got 0x%X", result);
	result = fixed16_16_floor((fixed16_16) 0xFFFF7FFF);
	TestCase_assert(result == (fixed16_16) 0xFFFF0000, "Expected 0xFFFF0000 but got 0x%X", result);
}

static void testCeil() {
	fixed16_16 result;
	
	result = fixed16_16_ceil(0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_ceil(0x20001);
	TestCase_assert(result == 0x30000, "Expected 0x30000 but got 0x%X", result);
	result = fixed16_16_ceil(0x3FFFF);
	TestCase_assert(result == 0x40000, "Expected 0x40000 but got 0x%X", result);
	result = fixed16_16_ceil((fixed16_16) 0xFFFF7FFF);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
}

static void testExp() {
	fixed16_16 result;
	
	result = fixed16_16_exp(0x0);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_exp(0x8000);
	TestCase_assert(result == 0x1A60F, "Expected 0x1A60F but got 0x%X", result);
	result = fixed16_16_exp(0x10000);
	TestCase_assert(result == X_E, "Expected 0x%X but got 0x%X", X_E, result);
	result = fixed16_16_exp((fixed16_16) 0xFFFF0000);
	TestCase_assert(result == 0x5E2E, "Expected 0x5E2E but got 0x%X", result);
	result = fixed16_16_exp(0x20000);
	TestCase_assert(result == 0x7638D, "Expected 0x7638D but got 0x%X", result);
	result = fixed16_16_exp(0xA0000);
	TestCase_assert(result == 0x5609CBA2, "Expected 0x5609CBA2 but got 0x%X", result);
	result = fixed16_16_exp(0xB0000);
	TestCase_assert(result == FIXED_16_16_MAX, "Expected 0x%X but got 0x%X", FIXED_16_16_MAX, result);
}

static void testLog() {
	fixed16_16 result;
	
	result = fixed16_16_log(0x8000);
	TestCase_assert(result == (fixed16_16) 0xFFFF4E8F, "Expected 0xFFFF4E8F but got 0x%X", result);
	result = fixed16_16_log(0x10000);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_log(0x20000);
	TestCase_assert(result == 0xB174, "Expected 0xB174 but got 0x%X", result);
	result = fixed16_16_log(0xA0000);
	TestCase_assert(result == 0x24D78, "Expected 0x24D78 but got 0x%X", result);
	result = fixed16_16_log(0x0);
	TestCase_assert(result == FIXED_16_16_NINF, "Expected 0x%X but got 0x%X", FIXED_16_16_NINF, result);
	result = fixed16_16_log((fixed16_16) 0xFFFF0000);
	TestCase_assert(result == FIXED_16_16_NAN, "Expected 0x%X but got 0x%X", FIXED_16_16_NAN, result);
	result = fixed16_16_log(FIXED_16_16_INF);
	TestCase_assert(result == FIXED_16_16_INF, "Expected 0x%X but got 0x%X", FIXED_16_16_INF, result);
}

static void testPow() {
	fixed16_16 result;
	
	result = fixed16_16_pow(0x10000, 0x10000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_pow(0x18000, 0x10000);
	TestCase_assert(result == 0x18000, "Expected 0x18000 but got 0x%X", result);
	result = fixed16_16_pow(0x10000, 0x18000);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_pow(0x20000, 0x30000);
	TestCase_assert(result == 0x80027, "Expected 0x80027 but got 0x%X", result);
	result = fixed16_16_pow(0x30000, 0x40000);
	TestCase_assert(result == 0x510177, "Expected 0x510177 but got 0x%X", result);
	result = fixed16_16_pow(0x18000, 0x18000);
	TestCase_assert(result == 0x1D64E, "Expected 0x1D64E but got 0x%X", result);
	result = fixed16_16_pow(0x8000, 0x8000);
	TestCase_assert(result == 0xB506, "Expected 0xB506 but got 0x%X", result);
}

#ifndef USE_SIN_LOOKUP_TABLE
#define USE_SIN_LOOKUP_TABLE 1
#endif

static void testSin() {
	fixed16_16 result;
	
#if USE_SIN_LOOKUP_TABLE
	result = fixed16_16_sin(0x0);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_sin(X_PI_2);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_sin(X_PI);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_sin(X_PI_4);
	TestCase_assert(result == 0xB504, "Expected 0xB504 but got 0x%X", result);
	result = fixed16_16_sin(X_PI_2 + X_PI_4);
	TestCase_assert(result == 0xB504, "Expected 0xB504 but got 0x%X", result);
	result = fixed16_16_sin(X_PI + X_PI_4);
	TestCase_assert(result == (fixed16_16) 0xFFFF4AFB, "Expected 0xFFFF4AFB but got 0x%X", result);
	result = fixed16_16_sin(X_PI + X_PI_2 + X_PI_4);
	TestCase_assert(result == (fixed16_16) 0xFFFF4AFB, "Expected 0xFFFF4AFB but got 0x%X", result);
	result = fixed16_16_sin(X_PI + X_PI_2);
	TestCase_assert(result == (fixed16_16) 0xFFFF0000, "Expected 0xFFFF0000 but got 0x%X", result);
	result = fixed16_16_sin(X_PI * 4);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_sin(X_PI * -4);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
#else
	result = fixed16_16_sin(0x0);
	TestCase_assert(result == 0x1, "Expected 0x1 but got 0x%X", result);
	result = fixed16_16_sin(X_PI_2);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_sin(X_PI);
	TestCase_assert(result == 0xD, "Expected 0xD but got 0x%X", result);
	result = fixed16_16_sin(X_PI_4);
	TestCase_assert(result == 0xB505, "Expected 0xB505 but got 0x%X", result);
	result = fixed16_16_sin(X_PI + X_PI_2);
	TestCase_assert(result == (fixed16_16) 0xFFFF0000, "Expected 0xFFFF0000 but got 0x%X", result);
	result = fixed16_16_sin(X_PI * 4);
	TestCase_assert(result == (fixed16_16) 0xFFFFFFDB, "Expected 0xFFFFFFDB but got 0x%X", result);
	result = fixed16_16_sin(X_PI * -4);
	TestCase_assert(result == 0x26, "Expected 0x26 but got 0x%X", result);
#endif
}

static void testCos() {
	fixed16_16 result;
	
#if USE_SIN_LOOKUP_TABLE
	result = fixed16_16_cos(0x0);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_cos(X_PI_2);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_cos(X_PI);
	TestCase_assert(result == (fixed16_16) 0xFFFF0000, "Expected 0xFFFF0000 but got 0x%X", result);
	result = fixed16_16_cos(X_PI_4);
	TestCase_assert(result == 0xB504, "Expected 0xB504 but got 0x%X", result);
	result = fixed16_16_cos(X_PI + X_PI_2);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_cos(X_PI * 4);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_cos(X_PI * -4);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
#else
	result = fixed16_16_cos(0x0);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_cos(X_PI_2);
	TestCase_assert(result == 0xD, "Expected 0xD but got 0x%X", result);
	result = fixed16_16_cos(X_PI);
	TestCase_assert(result == (fixed16_16) 0xFFFF0000, "Expected 0xFFFF0000 but got 0x%X", result);
	result = fixed16_16_cos(X_PI_4);
	TestCase_assert(result == 0xB517, "Expected 0xB517 but got 0x%X", result);
	result = fixed16_16_cos(X_PI + X_PI_2);
	TestCase_assert(result == (fixed16_16) 0xFFFFFFF4, "Expected 0xFFFFFFF4 but got 0x%X", result);
	result = fixed16_16_cos(X_PI * 4);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_cos(X_PI * -4);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
#endif
}

static void testTan() {
	fixed16_16 result;
	
#if USE_SIN_LOOKUP_TABLE
	result = fixed16_16_tan(0x0);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_tan(X_PI / 4);
	TestCase_assert(result == 0x10000, "Expected 0x10000 but got 0x%X", result);
	result = fixed16_16_tan(X_PI / 3);
	TestCase_assert(result == 0x1BB96, "Expected 0x1BB96 but got 0x%X", result);
	result = fixed16_16_tan(X_PI * 3 / 4);
	TestCase_assert(result == (fixed16_16) 0xFFFF0003, "Expected 0xFFFF0003 but got 0x%X", result);
	result = fixed16_16_tan(X_PI * 4);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_tan(X_PI * -4);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
#else
	result = fixed16_16_tan(0x0);
	TestCase_assert(result == 0x1, "Expected 0x1 but got 0x%X", result);
	result = fixed16_16_tan(X_PI / 4);
	TestCase_assert(result == 0xFFE6, "Expected 0xFFE6 but got 0x%X", result);
	result = fixed16_16_tan(X_PI / 3);
	TestCase_assert(result == 0x1BB50, "Expected 0x1BB50 but got 0x%X", result);
	result = fixed16_16_tan(X_PI * 3 / 4);
	TestCase_assert(result == (fixed16_16) 0xFFFEFFE6, "Expected 0xFFFEFFE6 but got 0x%X", result);
	result = fixed16_16_tan(X_PI * 4);
	TestCase_assert(result == (fixed16_16) 0xFFFFFFDB, "Expected 0xFFFFFFDB but got 0x%X", result);
	result = fixed16_16_tan(X_PI * -4);
	TestCase_assert(result == 0x26, "Expected 0x26 but got 0x%X", result);
#endif
}

static void testAsin() {
	fixed16_16 result;
	
	result = fixed16_16_asin(0x0);
	TestCase_assert(result == 0x4, "Expected 0x4 but got 0x%X", result);
	result = fixed16_16_asin(0x10000);
	TestCase_assert(result == X_PI_2, "Expected 0x%X but got 0x%X", X_PI_2, result);
	result = fixed16_16_asin((fixed16_16) 0xFFFF0000);
	TestCase_assert(result == (fixed16_16) 0xFFFE6DE1, "Expected 0x%X but got 0x%X", 0xFFFE6DE1, result);
	result = fixed16_16_asin(0xB504);
	TestCase_assert(result == X_PI_4, "Expected 0x%X but got 0x%X", X_PI_4, result);
	result = fixed16_16_asin(0x61F8);
	TestCase_assert(result == 0x648C, "Expected 0x%X but got 0x%X", 0x648C, result);
	result = fixed16_16_asin(0x20000);
	TestCase_assert(result == FIXED_16_16_NAN, "Expected 0x%X but got 0x%X", FIXED_16_16_NAN, result);
	result = fixed16_16_asin((fixed16_16) 0xFFFE0000);
	TestCase_assert(result == FIXED_16_16_NAN, "Expected 0x%X but got 0x%X", FIXED_16_16_NAN, result);
}

static void testAcos() {
	fixed16_16 result;
	
	result = fixed16_16_acos(0x10000);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_acos(0x0);
	TestCase_assert(result == 0x1921B, "Expected 0x1921B but got 0x%X", result);
	result = fixed16_16_acos((fixed16_16) 0xFFFF0000);
	TestCase_assert(result == 0x3243E, "Expected 0x3243E but got 0x%X", result);
	result = fixed16_16_acos(0xB504);
	TestCase_assert(result == 0xC910, "Expected 0xC910 but got 0x%X", result);
	result = fixed16_16_acos(0xEC83);
	TestCase_assert(result == 0x6489, "Expected 0x6489 but got 0x%X", result);
	result = fixed16_16_acos(0x20000);
	TestCase_assert(result == FIXED_16_16_NAN, "Expected 0x%X but got 0x%X", FIXED_16_16_NAN, result);
	result = fixed16_16_acos((fixed16_16) 0xFFFE0000);
	TestCase_assert(result == FIXED_16_16_NAN, "Expected 0x%X but got 0x%X", FIXED_16_16_NAN, result);
}

static void testAtan() {
	fixed16_16 result;
	
	result = fixed16_16_atan(0x0);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_atan(0x10000);
	TestCase_assert(result == 0xC910, "Expected 0xC910 but got 0x%X", result);
	result = fixed16_16_atan(0x1BB67);
	TestCase_assert(result == X_PI / 3, "Expected 0x%X but got 0x%X", X_PI / 3, result);
	result = fixed16_16_atan((fixed16_16) 0xFFFF0000);
	TestCase_assert(result == (fixed16_16) 0xFFFF36F0, "Expected 0xFFFF36F0 but got 0x%X", result);
}

static void testAtan2() {
	fixed16_16 result;
	
	result = fixed16_16_atan2(0x0, 0x10000);
	TestCase_assert(result == 0x0, "Expected 0x0 but got 0x%X", result);
	result = fixed16_16_atan2(0x10000, 0x0);
	TestCase_assert(result == X_PI_2, "Expected 0x%X but got 0x%X", X_PI_2, result);
	result = fixed16_16_atan2(0x10000, 0x10000);
	TestCase_assert(result == 0xC910, "Expected 0xC910 but got 0x%X", result);
	result = fixed16_16_atan2(0x20000, (fixed16_16) -0x80000);
	TestCase_assert(result == 0x2E588, "Expected 0x2E588 but got 0x%X", result);
}

static void testIsinf() {
	TestCase_assert(!fixed16_16_isinf(0x0), "Expected false but got true");
	TestCase_assert(!fixed16_16_isinf(FIXED_16_16_NAN), "Expected false but got true");
	TestCase_assert(!fixed16_16_isinf(FIXED_16_16_MIN), "Expected false but got true");
	TestCase_assert(!fixed16_16_isinf(FIXED_16_16_MAX), "Expected false but got true");
	TestCase_assert(fixed16_16_isinf(FIXED_16_16_NINF), "Expected true but got false");
	TestCase_assert(fixed16_16_isinf(FIXED_16_16_INF), "Expected true but got false");
}

static void testIsnan() {
	TestCase_assert(!fixed16_16_isnan(0x0), "Expected false but got true");
	TestCase_assert(!fixed16_16_isnan(FIXED_16_16_MIN), "Expected false but got true");
	TestCase_assert(!fixed16_16_isnan(FIXED_16_16_MAX), "Expected false but got true");
	TestCase_assert(!fixed16_16_isnan(FIXED_16_16_NINF), "Expected false but got true");
	TestCase_assert(!fixed16_16_isnan(FIXED_16_16_INF), "Expected false but got true");
	TestCase_assert(fixed16_16_isnan(FIXED_16_16_NAN), "Expected true but got false");
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
           testSqrt,
           testFloor,
           testCeil,
           testExp,
           testLog,
           testPow,
           testSin,
           testCos,
           testTan,
           testAsin,
           testAcos,
           testAtan,
           testAtan2,
           testIsinf,
           testIsnan)
