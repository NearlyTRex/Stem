#include "unittest/TestSuite.h"
#include "glgraphics/GLUtilities.h"

static void testStringToGLConstant() {
	bool success;
	GLenum result;
	
	success = stringToGLConstant("This is most certainly not an OpenGL constant.", &result);
	TestCase_assert(!success, "Expected false but got true");
	
#define assertStringToGLConstant(constant) \
	success = stringToGLConstant(#constant, &result); \
	TestCase_assert(success, "Expected true for %s but got false", #constant); \
	TestCase_assert(result == constant, "Expected %d for %s but got %d", (int) constant, #constant, (int) result);
	
	assertStringToGLConstant(GL_LUMINANCE);
	assertStringToGLConstant(GL_LUMINANCE_ALPHA);
	assertStringToGLConstant(GL_RGB);
	assertStringToGLConstant(GL_RGBA);
	
	assertStringToGLConstant(GL_NEAREST);
	assertStringToGLConstant(GL_LINEAR);
	assertStringToGLConstant(GL_NEAREST_MIPMAP_NEAREST);
	assertStringToGLConstant(GL_LINEAR_MIPMAP_NEAREST);
	assertStringToGLConstant(GL_NEAREST_MIPMAP_LINEAR);
	assertStringToGLConstant(GL_LINEAR_MIPMAP_LINEAR);
	
	assertStringToGLConstant(GL_CLAMP_TO_EDGE);
	assertStringToGLConstant(GL_REPEAT);
	
	assertStringToGLConstant(GL_POINTS);
	assertStringToGLConstant(GL_LINE_STRIP);
	assertStringToGLConstant(GL_LINE_LOOP);
	assertStringToGLConstant(GL_LINES);
	assertStringToGLConstant(GL_TRIANGLE_STRIP);
	assertStringToGLConstant(GL_TRIANGLE_FAN);
	assertStringToGLConstant(GL_TRIANGLES);
	
#if !TARGET_OPENGL_ES
	assertStringToGLConstant(GL_ALPHA4);
	assertStringToGLConstant(GL_ALPHA8);
	assertStringToGLConstant(GL_ALPHA12);
	assertStringToGLConstant(GL_ALPHA16);
	assertStringToGLConstant(GL_LUMINANCE4);
	assertStringToGLConstant(GL_LUMINANCE8);
	assertStringToGLConstant(GL_LUMINANCE12);
	assertStringToGLConstant(GL_LUMINANCE16);
	assertStringToGLConstant(GL_LUMINANCE4_ALPHA4);
	assertStringToGLConstant(GL_LUMINANCE6_ALPHA2);
	assertStringToGLConstant(GL_LUMINANCE8_ALPHA8);
	assertStringToGLConstant(GL_LUMINANCE12_ALPHA4);
	assertStringToGLConstant(GL_LUMINANCE12_ALPHA12);
	assertStringToGLConstant(GL_LUMINANCE16_ALPHA16);
	assertStringToGLConstant(GL_INTENSITY);
	assertStringToGLConstant(GL_INTENSITY4);
	assertStringToGLConstant(GL_INTENSITY8);
	assertStringToGLConstant(GL_INTENSITY12);
	assertStringToGLConstant(GL_INTENSITY16);
	assertStringToGLConstant(GL_R3_G3_B2);
	assertStringToGLConstant(GL_RGB4);
	assertStringToGLConstant(GL_RGB5);
	assertStringToGLConstant(GL_RGB8);
	assertStringToGLConstant(GL_RGB10);
	assertStringToGLConstant(GL_RGB12);
	assertStringToGLConstant(GL_RGB16);
	assertStringToGLConstant(GL_RGBA2);
	assertStringToGLConstant(GL_RGBA4);
	assertStringToGLConstant(GL_RGB5_A1);
	assertStringToGLConstant(GL_RGBA8);
	assertStringToGLConstant(GL_RGB10_A2);
	assertStringToGLConstant(GL_RGBA12);
	assertStringToGLConstant(GL_RGBA16);
	
	assertStringToGLConstant(GL_CLAMP);
	
	assertStringToGLConstant(GL_QUAD_STRIP);
	assertStringToGLConstant(GL_QUADS);
	assertStringToGLConstant(GL_POLYGON);
#endif
}

static void testGLConstantToString() {
	const char * string;
	
	string = glConstantToString(-1); // Hope this doesn't become a valid GLenum value in the future!
	TestCase_assert(string == NULL, "Expected NULL but got %s", string);
	
#define assertGLConstantToString(constant) \
	string = glConstantToString(constant); \
	TestCase_assert(string != NULL, "Returned string for %s was unexpectedly NULL", #constant); \
	TestCase_assert(!strcmp(#constant, string), "Expected %s but got %s", #constant, string);
	
	assertGLConstantToString(GL_LUMINANCE);
	assertGLConstantToString(GL_LUMINANCE_ALPHA);
	assertGLConstantToString(GL_RGB);
	assertGLConstantToString(GL_RGBA);
	
	assertGLConstantToString(GL_NEAREST);
	assertGLConstantToString(GL_LINEAR);
	assertGLConstantToString(GL_NEAREST_MIPMAP_NEAREST);
	assertGLConstantToString(GL_LINEAR_MIPMAP_NEAREST);
	assertGLConstantToString(GL_NEAREST_MIPMAP_LINEAR);
	assertGLConstantToString(GL_LINEAR_MIPMAP_LINEAR);
	
	assertGLConstantToString(GL_CLAMP_TO_EDGE);
	assertGLConstantToString(GL_REPEAT);
	
	assertGLConstantToString(GL_POINTS);
	assertGLConstantToString(GL_LINE_STRIP);
	assertGLConstantToString(GL_LINE_LOOP);
	assertGLConstantToString(GL_LINES);
	assertGLConstantToString(GL_TRIANGLE_STRIP);
	assertGLConstantToString(GL_TRIANGLE_FAN);
	assertGLConstantToString(GL_TRIANGLES);
	
#if !TARGET_OPENGL_ES
	assertGLConstantToString(GL_ALPHA4);
	assertGLConstantToString(GL_ALPHA8);
	assertGLConstantToString(GL_ALPHA12);
	assertGLConstantToString(GL_ALPHA16);
	assertGLConstantToString(GL_LUMINANCE4);
	assertGLConstantToString(GL_LUMINANCE8);
	assertGLConstantToString(GL_LUMINANCE12);
	assertGLConstantToString(GL_LUMINANCE16);
	assertGLConstantToString(GL_LUMINANCE4_ALPHA4);
	assertGLConstantToString(GL_LUMINANCE6_ALPHA2);
	assertGLConstantToString(GL_LUMINANCE8_ALPHA8);
	assertGLConstantToString(GL_LUMINANCE12_ALPHA4);
	assertGLConstantToString(GL_LUMINANCE12_ALPHA12);
	assertGLConstantToString(GL_LUMINANCE16_ALPHA16);
	assertGLConstantToString(GL_INTENSITY);
	assertGLConstantToString(GL_INTENSITY4);
	assertGLConstantToString(GL_INTENSITY8);
	assertGLConstantToString(GL_INTENSITY12);
	assertGLConstantToString(GL_INTENSITY16);
	assertGLConstantToString(GL_R3_G3_B2);
	assertGLConstantToString(GL_RGB4);
	assertGLConstantToString(GL_RGB5);
	assertGLConstantToString(GL_RGB8);
	assertGLConstantToString(GL_RGB10);
	assertGLConstantToString(GL_RGB12);
	assertGLConstantToString(GL_RGB16);
	assertGLConstantToString(GL_RGBA2);
	assertGLConstantToString(GL_RGBA4);
	assertGLConstantToString(GL_RGB5_A1);
	assertGLConstantToString(GL_RGBA8);
	assertGLConstantToString(GL_RGB10_A2);
	assertGLConstantToString(GL_RGBA12);
	assertGLConstantToString(GL_RGBA16);
	
	assertGLConstantToString(GL_CLAMP);
	
	assertGLConstantToString(GL_QUAD_STRIP);
	assertGLConstantToString(GL_QUADS);
	assertGLConstantToString(GL_POLYGON);
#endif
}

TEST_SUITE(GLUtilitiesTest, testStringToGLConstant, testGLConstantToString)
