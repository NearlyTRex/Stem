unittest provides facilities for building and running unit tests as part of a library or application's build process. Each test suite must use the TEST_SUITE macro to name the suite and list the functions to be invoked when running it. Test case functions should perform assertions with the TestCase_assert macro.

When linking to libstem_unittest.a, the UnitTest_suiteNameList symbol must be defined as follows:

const char * UnitTest_suiteNameList[] = {"name1", "name2", NULL};

name1, name2, etc. are replaced by names of your test suites. Conventionally, this would be the name of the file containing the test suite, without the extension. The last entry must be NULL. As long as this list is defined at link time, unittest will be able to find and invoke all test cases defined by TEST_SUITEs in the test suite files you link to.

template_stemlib's makefile takes care of all of this. Simply list your test suite files in SOURCES_unittest_suites, and as long as their names follow the convention above, an appropriate UnitTest_suiteNameList will be generated and linked automatically.