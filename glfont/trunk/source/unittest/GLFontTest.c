#include "unittest/TestSuite.h"
#include "glfont/GLFont.h"

static void testInit() {
	GLFont font;
	
	memset(&font, 0xFF, sizeof(GLFont));
	GLFont_init(&font);
	TestCase_assert(font.dispose == GLFont_dispose, "Expected %p but got %p", GLFont_dispose, font.dispose);
	TestCase_assert(font.measureString == NULL, "Expected NULL but got %p", font.measureString);
	TestCase_assert(font.indexAtWidth == NULL, "Expected NULL but got %p", font.indexAtWidth);
	TestCase_assert(font.drawString == NULL, "Expected NULL but got %p", font.drawString);
	font.dispose(&font);
}

TEST_SUITE(GLFontTest, testInit)
