#include <stdlib.h>

#include "unittest/framework/TestSuite.h"
#include "unittest/suites/StubShellTest.h"

TestSuite ** getTestSuites() {
	static TestSuite * testSuites[2];
	
	testSuites[0] = StubShellTest_suite();
	testSuites[1] = NULL;
	
	return testSuites;
}
