#include "unittest/TestSuite.h"
#include "stemobject/StemObject.h" 

static void testInit() {
	StemObject objectStruct;
	bool success;
	
	success = StemObject_init(&objectStruct);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(!objectStruct.protected_ivar(allocated), "Expected false but got true");
	TestCase_assert(objectStruct.dispose != NULL, "Method unexpectedly NULL");
	objectStruct.dispose(&objectStruct);
}

TEST_SUITE(StemObjectTest, testInit)
