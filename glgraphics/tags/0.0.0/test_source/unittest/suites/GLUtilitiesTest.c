#include "unittest/framework/TestSuite.h"
#include "glgraphics/GLUtilities.h"

static void testStringToGLConstant() {
	bool success;
	GLenum result;
	
	success = stringToGLConstant("This is most certainly not an OpenGL constant.", &result);
	TestCase_assert(!success, "Expected false but got true");
	
#define assertStringToGLConstant(constant) \
	success = stringToGLConstant(#constant, &result); \
	TestCase_assert(success, "Expected true but got false"); \
	TestCase_assert(result == constant, "Expected %d but got %d", (int) constant, (int) result);
	
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

TEST_SUITE(GLUtilitiesTest, testStringToGLConstant)
