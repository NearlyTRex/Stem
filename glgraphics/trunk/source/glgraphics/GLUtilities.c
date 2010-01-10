#include "glgraphics/GLUtilities.h"

#include <string.h>

#define translateString(expectedString) \
	if (!strcmp(string, #expectedString)) { \
		*outResult = expectedString; \
		return true; \
	}

bool stringToGLConstant(const char * string, GLenum * outResult) {
	translateString(GL_ALPHA);
	translateString(GL_LUMINANCE);
	translateString(GL_LUMINANCE_ALPHA);
	translateString(GL_RGB);
	translateString(GL_RGBA);
	
	translateString(GL_NEAREST);
	translateString(GL_LINEAR);
	translateString(GL_NEAREST_MIPMAP_NEAREST);
	translateString(GL_LINEAR_MIPMAP_NEAREST);
	translateString(GL_NEAREST_MIPMAP_LINEAR);
	translateString(GL_LINEAR_MIPMAP_LINEAR);
	
	translateString(GL_CLAMP_TO_EDGE);
	translateString(GL_REPEAT);
	
	translateString(GL_POINTS);
	translateString(GL_LINE_STRIP);
	translateString(GL_LINE_LOOP);
	translateString(GL_LINES);
	translateString(GL_TRIANGLE_STRIP);
	translateString(GL_TRIANGLE_FAN);
	translateString(GL_TRIANGLES);
	
#if !TARGET_OPENGL_ES
	translateString(GL_ALPHA4);
	translateString(GL_ALPHA8);
	translateString(GL_ALPHA12);
	translateString(GL_ALPHA16);
	translateString(GL_LUMINANCE4);
	translateString(GL_LUMINANCE8);
	translateString(GL_LUMINANCE12);
	translateString(GL_LUMINANCE16);
	translateString(GL_LUMINANCE4_ALPHA4);
	translateString(GL_LUMINANCE6_ALPHA2);
	translateString(GL_LUMINANCE8_ALPHA8);
	translateString(GL_LUMINANCE12_ALPHA4);
	translateString(GL_LUMINANCE12_ALPHA12);
	translateString(GL_LUMINANCE16_ALPHA16);
	translateString(GL_INTENSITY);
	translateString(GL_INTENSITY4);
	translateString(GL_INTENSITY8);
	translateString(GL_INTENSITY12);
	translateString(GL_INTENSITY16);
	translateString(GL_R3_G3_B2);
	translateString(GL_RGB4);
	translateString(GL_RGB5);
	translateString(GL_RGB8);
	translateString(GL_RGB10);
	translateString(GL_RGB12);
	translateString(GL_RGB16);
	translateString(GL_RGBA2);
	translateString(GL_RGBA4);
	translateString(GL_RGB5_A1);
	translateString(GL_RGBA8);
	translateString(GL_RGB10_A2);
	translateString(GL_RGBA12);
	translateString(GL_RGBA16);
	
	translateString(GL_CLAMP);
	
	translateString(GL_QUAD_STRIP);
	translateString(GL_QUADS);
	translateString(GL_POLYGON);
#endif
	
	return false;
}
