#include "unittest/framework/TestSuite.h"
#include "stemobject/StemObject.h" 

static void testInit() {
	StemObject * objectPtr;
	StemObject objectStruct;
	
	objectPtr = StemObject_create();
	TestCase_assert(objectPtr->allocated, "Expected true but got false");
	TestCase_assert(objectPtr->dispose != NULL, "Method unexpectedly NULL");
	objectPtr->dispose(objectPtr);
	
	StemObject_init(&objectStruct);
	TestCase_assert(!objectStruct.allocated, "Expected false but got true");
	TestCase_assert(objectStruct.dispose != NULL, "Method unexpectedly NULL");
	objectStruct.dispose(&objectStruct);
}

TEST_SUITE(StemObjectTest, testInit)
