#include "unittest/framework/TestSuite.h"
#include "utilities/Ranrot.h"
#include <math.h>

static void testStaticRandom() {
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
	
	sdrand(0);
	stirrand(1);
	uirandResult = uirand();
	TestCase_assert(uirandResult == 2147352573u, "Expected 2147352573 but got %u\n", uirandResult);
	
	sdrand(0);
	stirrand(2);
	uirandResult = uirand();
	TestCase_assert(uirandResult == 2147090425u, "Expected 2147090425 but got %u\n", uirandResult);
}

static void testInstancedRandom() {
	Ranrot ranrot, * ranrotPtr;
	unsigned int uirandResult;
	int irandResult;
	float ufrandResult, frandResult;
	
	memset(&ranrot, 0, sizeof(ranrot));
	Ranrot_init(&ranrot);
	TestCase_assert(ranrot.dispose == Ranrot_dispose, "Expected %p but got %p", Ranrot_dispose, ranrot.dispose);
	TestCase_assert(ranrot.sdrand == Ranrot_sdrand, "Expected %p but got %p", Ranrot_sdrand, ranrot.sdrand);
	TestCase_assert(ranrot.stirrand == Ranrot_stirrand, "Expected %p but got %p", Ranrot_stirrand, ranrot.stirrand);
	TestCase_assert(ranrot.uirand == Ranrot_uirand, "Expected %p but got %p", Ranrot_uirand, ranrot.uirand);
	TestCase_assert(ranrot.irand == Ranrot_irand, "Expected %p but got %p", Ranrot_irand, ranrot.irand);
	TestCase_assert(ranrot.ufrand == Ranrot_ufrand, "Expected %p but got %p", Ranrot_ufrand, ranrot.ufrand);
	TestCase_assert(ranrot.frand == Ranrot_frand, "Expected %p but got %p", Ranrot_frand, ranrot.frand);
	
	ranrotPtr = Ranrot_create();
	TestCase_assert(ranrotPtr != NULL, "Expected non-NULL but got NULL");
	if (ranrotPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(ranrotPtr->dispose == Ranrot_dispose, "Expected %p but got %p", Ranrot_dispose, ranrotPtr->dispose);
	TestCase_assert(ranrotPtr->sdrand == Ranrot_sdrand, "Expected %p but got %p", Ranrot_sdrand, ranrotPtr->sdrand);
	TestCase_assert(ranrotPtr->stirrand == Ranrot_stirrand, "Expected %p but got %p", Ranrot_stirrand, ranrotPtr->stirrand);
	TestCase_assert(ranrotPtr->uirand == Ranrot_uirand, "Expected %p but got %p", Ranrot_uirand, ranrotPtr->uirand);
	TestCase_assert(ranrotPtr->irand == Ranrot_irand, "Expected %p but got %p", Ranrot_irand, ranrotPtr->irand);
	TestCase_assert(ranrotPtr->ufrand == Ranrot_ufrand, "Expected %p but got %p", Ranrot_ufrand, ranrotPtr->ufrand);
	TestCase_assert(ranrotPtr->frand == Ranrot_frand, "Expected %p but got %p", Ranrot_frand, ranrotPtr->frand);
	
	ranrot.sdrand(&ranrot, 0);
	ranrotPtr->sdrand(ranrotPtr, 0);
	
	uirandResult = ranrot.uirand(&ranrot);
	TestCase_assert(uirandResult == 2147418111u, "Expected 2147418111 but got %u\n", uirandResult);
	uirandResult = ranrot.uirand(&ranrot);
	TestCase_assert(uirandResult == 2147352573u, "Expected 2147352573 but got %u\n", uirandResult);
	
	uirandResult = ranrotPtr->uirand(ranrotPtr);
	TestCase_assert(uirandResult == 2147418111u, "Expected 2147418111 but got %u\n", uirandResult);
	uirandResult = ranrotPtr->uirand(ranrotPtr);
	TestCase_assert(uirandResult == 2147352573u, "Expected 2147352573 but got %u\n", uirandResult);
	
	ranrot.sdrand(&ranrot, 0);
	ranrotPtr->sdrand(ranrotPtr, 0);
	
	irandResult = ranrot.irand(&ranrot);
	TestCase_assert(irandResult == -65537, "Expected -65537 but got %d\n", irandResult);
	irandResult = ranrot.irand(&ranrot);
	TestCase_assert(irandResult == -131075, "Expected -131075 but got %d\n", irandResult);
	
	irandResult = ranrotPtr->irand(ranrotPtr);
	TestCase_assert(irandResult == -65537, "Expected -65537 but got %d\n", irandResult);
	irandResult = ranrotPtr->irand(ranrotPtr);
	TestCase_assert(irandResult == -131075, "Expected -131075 but got %d\n", irandResult);
	
	ranrot.sdrand(&ranrot, 0);
	ranrotPtr->sdrand(ranrotPtr, 0);
	
	ufrandResult = ranrot.ufrand(&ranrot, 1.0f);
	TestCase_assert(fabs(ufrandResult - 0.999969f) < 0.00001f, "Expected 0.999969 but got %f\n", ufrandResult);
	ufrandResult = ranrot.ufrand(&ranrot, 1.0f);
	TestCase_assert(fabs(ufrandResult - 0.999939f) < 0.00001f, "Expected 0.999939 but got %f\n", ufrandResult);
	
	ufrandResult = ranrotPtr->ufrand(ranrotPtr, 1.0f);
	TestCase_assert(fabs(ufrandResult - 0.999969f) < 0.00001f, "Expected 0.999969 but got %f\n", ufrandResult);
	ufrandResult = ranrotPtr->ufrand(ranrotPtr, 1.0f);
	TestCase_assert(fabs(ufrandResult - 0.999939f) < 0.00001f, "Expected 0.999939 but got %f\n", ufrandResult);
	
	ranrot.sdrand(&ranrot, 0);
	ranrotPtr->sdrand(ranrotPtr, 0);
	
	frandResult = ranrot.frand(&ranrot, 1.0f);
	TestCase_assert(fabs(frandResult - -0.000031f) < 0.00001f, "Expected -0.000031 but got %f\n", frandResult);
	frandResult = ranrot.frand(&ranrot, 1.0f);
	TestCase_assert(fabs(frandResult - -0.000061f) < 0.00001f, "Expected -0.000061 but got %f\n", frandResult);
	
	frandResult = ranrotPtr->frand(ranrotPtr, 1.0f);
	TestCase_assert(fabs(frandResult - -0.000031f) < 0.00001f, "Expected -0.000031 but got %f\n", frandResult);
	frandResult = ranrotPtr->frand(ranrotPtr, 1.0f);
	TestCase_assert(fabs(frandResult - -0.000061f) < 0.00001f, "Expected -0.000061 but got %f\n", frandResult);
	
	ranrot.sdrand(&ranrot, 1000);
	ranrotPtr->sdrand(ranrotPtr, 1000);
	
	uirandResult = ranrot.uirand(&ranrot);
	TestCase_assert(uirandResult == 2081883111u, "Expected 2081883111 but got %u\n", uirandResult);
	uirandResult = ranrot.uirand(&ranrot);
	TestCase_assert(uirandResult == 2147353574u, "Expected 2147353574 but got %u\n", uirandResult);
	
	uirandResult = ranrotPtr->uirand(ranrotPtr);
	TestCase_assert(uirandResult == 2081883111u, "Expected 2081883111 but got %u\n", uirandResult);
	uirandResult = ranrotPtr->uirand(ranrotPtr);
	TestCase_assert(uirandResult == 2147353574u, "Expected 2147353574 but got %u\n", uirandResult);
	
	ranrot.sdrand(&ranrot, 1000);
	ranrotPtr->sdrand(ranrotPtr, 1000);
	
	irandResult = ranrot.irand(&ranrot);
	TestCase_assert(irandResult == -65600537, "Expected -65600537 but got %d\n", irandResult);
	irandResult = ranrot.irand(&ranrot);
	TestCase_assert(irandResult == -130074, "Expected -131075 but got %d\n", irandResult);
	
	irandResult = ranrotPtr->irand(ranrotPtr);
	TestCase_assert(irandResult == -65600537, "Expected -65600537 but got %d\n", irandResult);
	irandResult = ranrotPtr->irand(ranrotPtr);
	TestCase_assert(irandResult == -130074, "Expected -131075 but got %d\n", irandResult);
	
	ranrot.sdrand(&ranrot, 1000);
	ranrotPtr->sdrand(ranrotPtr, 1000);
	
	ufrandResult = ranrot.ufrand(&ranrot, 2.0f);
	TestCase_assert(fabs(ufrandResult - 1.938905f) < 0.00001f, "Expected 1.938905 but got %f\n", ufrandResult);
	ufrandResult = ranrot.ufrand(&ranrot, 2.0f);
	TestCase_assert(fabs(ufrandResult - 1.999879f) < 0.00001f, "Expected 1.999879 but got %f\n", ufrandResult);
	
	ufrandResult = ranrotPtr->ufrand(ranrotPtr, 2.0f);
	TestCase_assert(fabs(ufrandResult - 1.938905f) < 0.00001f, "Expected 1.938905 but got %f\n", ufrandResult);
	ufrandResult = ranrotPtr->ufrand(ranrotPtr, 2.0f);
	TestCase_assert(fabs(ufrandResult - 1.999879f) < 0.00001f, "Expected 1.999879 but got %f\n", ufrandResult);
	
	ranrot.sdrand(&ranrot, 1000);
	ranrotPtr->sdrand(ranrotPtr, 1000);
	
	frandResult = ranrot.frand(&ranrot, 2.0f);
	TestCase_assert(fabs(frandResult - -0.061095f) < 0.00001f, "Expected -0.061095 but got %f\n", frandResult);
	frandResult = ranrot.frand(&ranrot, 2.0f);
	TestCase_assert(fabs(frandResult - -0.000121f) < 0.00001f, "Expected -0.000121 but got %f\n", frandResult);
	
	frandResult = ranrotPtr->frand(ranrotPtr, 2.0f);
	TestCase_assert(fabs(frandResult - -0.061095f) < 0.00001f, "Expected -0.061095 but got %f\n", frandResult);
	frandResult = ranrotPtr->frand(ranrotPtr, 2.0f);
	TestCase_assert(fabs(frandResult - -0.000121f) < 0.00001f, "Expected -0.000121 but got %f\n", frandResult);
	
	ranrot.sdrand(&ranrot, 0);
	ranrot.stirrand(&ranrot, 1);
	ranrotPtr->sdrand(ranrotPtr, 0);
	ranrotPtr->stirrand(ranrotPtr, 1);
	uirandResult = ranrot.uirand(&ranrot);
	TestCase_assert(uirandResult == 2147352573u, "Expected 2147352573 but got %u\n", uirandResult);
	uirandResult = ranrotPtr->uirand(ranrotPtr);
	TestCase_assert(uirandResult == 2147352573u, "Expected 2147352573 but got %u\n", uirandResult);
	
	ranrot.sdrand(&ranrot, 0);
	ranrot.stirrand(&ranrot, 2);
	ranrotPtr->sdrand(ranrotPtr, 0);
	ranrotPtr->stirrand(ranrotPtr, 2);
	uirandResult = ranrot.uirand(&ranrot);
	TestCase_assert(uirandResult == 2147090425u, "Expected 2147090425 but got %u\n", uirandResult);
	uirandResult = ranrotPtr->uirand(ranrotPtr);
	TestCase_assert(uirandResult == 2147090425u, "Expected 2147090425 but got %u\n", uirandResult);
}

TEST_SUITE(RanrotTest,
           testStaticRandom,
           testInstancedRandom)
