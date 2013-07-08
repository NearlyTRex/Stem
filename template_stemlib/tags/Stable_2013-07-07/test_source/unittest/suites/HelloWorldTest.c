#include <string.h>
#include "template_stemlib/HelloWorld.h"
#include "unittest/framework/TestSuite.h"

static void testHelloWorld() {
	const char * hello;
	
	hello = helloWorld();
	TestCase_assert(!strcmp("Hello, world!", hello), "Unexpected result \"%s\" from call to helloWorld (expected \"Hello, world!\")", hello);
}

TEST_SUITE(HelloWorldTest, testHelloWorld)
