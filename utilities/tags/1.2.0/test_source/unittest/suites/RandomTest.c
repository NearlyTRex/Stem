#include "unittest/framework/TestSuite.h"
#include "utilities/Random.h"
#include <math.h>

static void testRandom() {
	unsigned int uirandResult;
	int irandResult;
	float ufrandResult, frandResult;
	
	sdrand(0);
	uirandResult = uirand();
	TestCase_assert(uirandResult == 2147418111u, "Expected 2147418111 but got %u\n", uirandResult);
	uirandResult = uirand();
	TestCase_assert(uirandResult == 2147352573u, "Expected 2147352573 but got %u\n", uirandResult);
	
	sdrand(0);
	irandResult = irand();
	TestCase_assert(irandResult == -65537, "Expected -65537 but got %d\n", irandResult);
	irandResult = irand();
	TestCase_assert(irandResult == -131075, "Expected -131075 but got %d\n", irandResult);
	
	sdrand(0);
	ufrandResult = ufrand(1.0f);
	TestCase_assert(fabs(ufrandResult - 0.999969f) < 0.00001f, "Expected 0.999969 but got %f\n", ufrandResult);
	ufrandResult = ufrand(1.0f);
	TestCase_assert(fabs(ufrandResult - 0.999939f) < 0.00001f, "Expected 0.999939 but got %f\n", ufrandResult);
	
	sdrand(0);
	frandResult = frand(1.0f);
	TestCase_assert(fabs(frandResult - -0.000031f) < 0.00001f, "Expected -0.000031 but got %f\n", frandResult);
	frandResult = frand(1.0f);
	TestCase_assert(fabs(frandResult - -0.000061f) < 0.00001f, "Expected -0.000061 but got %f\n", frandResult);
	
	sdrand(1000);
	uirandResult = uirand();
	TestCase_assert(uirandResult == 2081883111u, "Expected 2081883111 but got %u\n", uirandResult);
	uirandResult = uirand();
	TestCase_assert(uirandResult == 2147353574u, "Expected 2147353574 but got %u\n", uirandResult);
	
	sdrand(1000);
	irandResult = irand();
	TestCase_assert(irandResult == -65600537, "Expected -65600537 but got %d\n", irandResult);
	irandResult = irand();
	TestCase_assert(irandResult == -130074, "Expected -131075 but got %d\n", irandResult);
	
	sdrand(1000);
	ufrandResult = ufrand(2.0f);
	TestCase_assert(fabs(ufrandResult - 1.938905f) < 0.00001f, "Expected 1.938905 but got %f\n", ufrandResult);
	ufrandResult = ufrand(2.0f);
	TestCase_assert(fabs(ufrandResult - 1.999879f) < 0.00001f, "Expected 1.999879 but got %f\n", ufrandResult);
	
	sdrand(1000);
	frandResult = frand(2.0f);
	TestCase_assert(fabs(frandResult - -0.061095f) < 0.00001f, "Expected -0.061095 but got %f\n", frandResult);
	frandResult = frand(2.0f);
	TestCase_assert(fabs(frandResult - -0.000121f) < 0.00001f, "Expected -0.000121 but got %f\n", frandResult);
}

TEST_SUITE(RandomTest, testRandom)
