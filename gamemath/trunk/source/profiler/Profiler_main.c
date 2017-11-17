#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gamemath/BezierCurve.h"
#include "gamemath/FixedPoint.h"

#if defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#elif defined(__linux)
#include <time.h>
#elif defined(WIN32)
#include <windows.h>
#endif

static double getCurrentTime() {
#if defined(__APPLE__)
	static mach_timebase_info_data_t timebaseInfo;
	
	if (timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}
	return mach_absolute_time() * (double) timebaseInfo.numer / timebaseInfo.denom * 0.000000001;
	
#elif defined(WIN32)
	static LARGE_INTEGER frequency;
	LARGE_INTEGER currentTime;
	
	if (frequency.QuadPart == 0) {
		QueryPerformanceFrequency(&frequency);
	}
	QueryPerformanceCounter(&currentTime);
	
	return (double) currentTime.QuadPart / frequency.QuadPart;
	
#elif defined(__linux)
	struct timespec currentTime;
	
	clock_gettime(CLOCK_MONOTONIC, &currentTime);
	return currentTime.tv_sec + currentTime.tv_nsec * 0.000000001;
#endif
}

int main(int argc, char ** argv) {
	unsigned long testCount = 2000000, testIndex;
	double startTime, timeSys, timeFixed;
	volatile float resultf;
	volatile fixed16_16 resultx;
	volatile Vector2f resultv2;
	Vector2f p0 = {0.0f, 0.0f}, p1 = {1.0f, 0.0f}, p2 = {0.0f, 1.0f}, p3 = {1.0f, 1.0f};
	
	if (argc > 1) {
		if (!sscanf(argv[1], "%lu", &testCount)) {
			fprintf(stderr, "Couldn't parse \"%s\" as unsigned int\n", argv[1]);
		}
	}
	
#define RUN_TEST(TEST_CODE, INTERVAL_VAR) \
	startTime = getCurrentTime(); \
	for (testIndex = 0; testIndex < testCount; testIndex++) { \
		TEST_CODE; \
	} \
	INTERVAL_VAR = getCurrentTime() - startTime; \
	printf("Performing \"" #TEST_CODE "\" %lu times took %f seconds\n", testCount, INTERVAL_VAR);
	
#define RUN_TWO_TESTS(TEST_CODE_SYS, TEST_CODE_FIXED) \
	RUN_TEST(TEST_CODE_SYS, timeSys) \
	RUN_TEST(TEST_CODE_FIXED, timeFixed) \
	printf("Ratio (fixed / system): %f\n\n", timeFixed / timeSys);
	
	RUN_TWO_TESTS(resultf = 0.125f * 13.0f, resultx = xmul(0x2000, 0xD0000))
	RUN_TWO_TESTS(resultf = 7.0f / 13.0f, resultx = xdiv(0x70000, 0xD0000))
	RUN_TWO_TESTS(resultf = sqrtf(5.432f), resultx = xsqrt(0x56430))
	RUN_TWO_TESTS(resultf = expf(1.5f), resultx = xexp(0x18000))
	RUN_TWO_TESTS(resultf = logf(1.5f), resultx = xlog(0x18000))
	RUN_TWO_TESTS(resultf = powf(1.5f, 1.5f), resultx = xpow(0x18000, 0x18000))
	RUN_TWO_TESTS(resultf = sinf(1.0f), resultx = xsin(0x10000))
	RUN_TWO_TESTS(resultf = cosf(1.0f), resultx = xcos(0x10000))
	RUN_TWO_TESTS(resultf = tanf(1.0f), resultx = xtan(0x10000))
	RUN_TWO_TESTS(resultf = asinf(0.5f), resultx = xasin(0x8000))
	RUN_TWO_TESTS(resultf = acosf(0.5f), resultx = xacos(0x8000))
	RUN_TWO_TESTS(resultf = atanf(0.5f), resultx = xatan(0x8000))
	RUN_TWO_TESTS(resultf = atan2f(0.5f, 1.0f), resultx = xatan2(0x8000, 0x10000))
	
	RUN_TEST(resultv2 = BezierCurve_sample(p0, p1, p2, p3, 0.5f), timeSys);
	
	return EXIT_SUCCESS;
}
