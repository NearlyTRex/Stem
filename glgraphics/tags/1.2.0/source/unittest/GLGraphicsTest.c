#include "unittest/TestSuite.h"
#include "glgraphics/GLGraphics.h"

static void testInit() {
	TestCase_assert(GLGraphics_getOpenGLAPIVersion() == GL_API_VERSION_DESKTOP_1, "Uninitialized GLGraphics should return %d from GLGraphics_getOpenGLAPIVersion(), but got %d instead", GL_API_VERSION_DESKTOP_1, GLGraphics_getOpenGLAPIVersion());
	
	// HACK: Can't call GLGraphics_init() when glew is enabled, because it'll call glewInit() with no GL context and crash
#ifdef GLGRAPHICS_NO_GLEW
	GLGraphics_init(GL_API_VERSION_DESKTOP_2);
	TestCase_assert(GLGraphics_getOpenGLAPIVersion() == GL_API_VERSION_DESKTOP_2, "Initialized GLGraphics should return %d from GLGraphics_getOpenGLAPIVersion(), but got %d instead", GL_API_VERSION_DESKTOP_2, GLGraphics_getOpenGLAPIVersion());
	
	// HACK: Shouldn't be calling GLGraphics_init() twice in a single execution
	GLGraphics_init(GL_API_VERSION_DESKTOP_3);
	TestCase_assert(GLGraphics_getOpenGLAPIVersion() == GL_API_VERSION_DESKTOP_3, "Reinitialized GLGraphics should return %d from GLGraphics_getOpenGLAPIVersion(), but got %d instead", GL_API_VERSION_DESKTOP_3, GLGraphics_getOpenGLAPIVersion());
#endif
}

TEST_SUITE(GLGraphicsTest, testInit)
