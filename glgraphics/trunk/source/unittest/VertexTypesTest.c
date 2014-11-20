#include "unittest/TestSuite.h"
#include "glgraphics/VertexTypes.h"
#include <math.h>

#define EPSILON 0.01f

static void testColor4f() {
	Color4f color;
	
	color = COLOR4f(0.0f, 0.25f, 0.5f, 0.75f);
	TestCase_assert(color.red == 0.0f, "Expected 0.0 but got %f", color.red);
	TestCase_assert(color.green == 0.25f, "Expected 0.25 but got %f", color.green);
	TestCase_assert(color.blue == 0.5f, "Expected 0.5 but got %f", color.blue);
	TestCase_assert(color.alpha == 0.75f, "Expected 0.75 but got %f", color.alpha);
	
	color = COLOR4f(1.0f, 0.375f, 0.25f, 0.0f);
	TestCase_assert(color.red == 1.0f, "Expected 1.0 but got %f", color.red);
	TestCase_assert(color.green == 0.375f, "Expected 0.375 but got %f", color.green);
	TestCase_assert(color.blue == 0.25f, "Expected 0.25 but got %f", color.blue);
	TestCase_assert(color.alpha == 0.0f, "Expected 0.0 but got %f", color.alpha);
	
	color = Color4f_fromRGBA8888(0x00, 0x3F, 0x7F, 0xBF);
	TestCase_assert(color.red == 0.0f, "Expected 0.0 but got %f", color.red);
	TestCase_assert(fabs(color.green - 0.25f) < EPSILON, "Expected ~0.25 but got %f", color.green);
	TestCase_assert(fabs(color.blue - 0.5f) < EPSILON, "Expected ~0.5 but got %f", color.blue);
	TestCase_assert(fabs(color.alpha - 0.75f) < EPSILON, "Expected ~0.75 but got %f", color.alpha);
	
	color = Color4f_fromRGBA8888(0xFF, 0x5F, 0x3F, 0x00);
	TestCase_assert(color.red == 1.0f, "Expected 1.0 but got %f", color.red);
	TestCase_assert(fabs(color.green - 0.375f) < EPSILON, "Expected ~0.375 but got %f", color.green);
	TestCase_assert(fabs(color.blue - 0.25f) < EPSILON, "Expected ~0.25 but got %f", color.blue);
	TestCase_assert(color.alpha == 0.0f, "Expected 0.0 but got %f", color.alpha);
	
	color = Color4f_fromRGBA32(0x003F7FBF);
	TestCase_assert(color.red == 0.0f, "Expected 0.0 but got %f", color.red);
	TestCase_assert(fabs(color.green - 0.25f) < EPSILON, "Expected ~0.25 but got %f", color.green);
	TestCase_assert(fabs(color.blue - 0.5f) < EPSILON, "Expected ~0.5 but got %f", color.blue);
	TestCase_assert(fabs(color.alpha - 0.75f) < EPSILON, "Expected ~0.75 but got %f", color.alpha);
	
	color = Color4f_fromRGBA32(0xFF5F3F00);
	TestCase_assert(color.red == 1.0f, "Expected 1.0 but got %f", color.red);
	TestCase_assert(fabs(color.green - 0.375f) < EPSILON, "Expected ~0.375 but got %f", color.green);
	TestCase_assert(fabs(color.blue - 0.25f) < EPSILON, "Expected ~0.25 but got %f", color.blue);
	TestCase_assert(color.alpha == 0.0f, "Expected 0.0 but got %f", color.alpha);
}

TEST_SUITE(VertexTypesTest, testColor4f)
