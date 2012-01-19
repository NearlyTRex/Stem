#include "unittest/framework/TestSuite.h"
#include "binaryserialization/BinaryDeserializationContext.h"

static void failTest() {
	TestCase_assert(false, "Serialization tests not implemented");
}

TEST_SUITE(BinarySerializationContextTest,
           failTest)
