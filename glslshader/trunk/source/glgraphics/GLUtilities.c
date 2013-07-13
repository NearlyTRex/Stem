/*
  Copyright (c) 2013 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener adiener@sacredsoftware.net
*/

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

#define translateEnum(expectedEnum) \
	case expectedEnum: \
		return #expectedEnum;

const char * glConstantToString(GLenum constant) {
	switch (constant) {
		translateEnum(GL_ALPHA);
		translateEnum(GL_LUMINANCE);
		translateEnum(GL_LUMINANCE_ALPHA);
		translateEnum(GL_RGB);
		translateEnum(GL_RGBA);
		
		translateEnum(GL_NEAREST);
		translateEnum(GL_LINEAR);
		translateEnum(GL_NEAREST_MIPMAP_NEAREST);
		translateEnum(GL_LINEAR_MIPMAP_NEAREST);
		translateEnum(GL_NEAREST_MIPMAP_LINEAR);
		translateEnum(GL_LINEAR_MIPMAP_LINEAR);
		
		translateEnum(GL_CLAMP_TO_EDGE);
		translateEnum(GL_REPEAT);
		
		translateEnum(GL_POINTS);
		translateEnum(GL_LINE_STRIP);
		translateEnum(GL_LINE_LOOP);
		translateEnum(GL_LINES);
		translateEnum(GL_TRIANGLE_STRIP);
		translateEnum(GL_TRIANGLE_FAN);
		translateEnum(GL_TRIANGLES);
		
#if !TARGET_OPENGL_ES
		translateEnum(GL_ALPHA4);
		translateEnum(GL_ALPHA8);
		translateEnum(GL_ALPHA12);
		translateEnum(GL_ALPHA16);
		translateEnum(GL_LUMINANCE4);
		translateEnum(GL_LUMINANCE8);
		translateEnum(GL_LUMINANCE12);
		translateEnum(GL_LUMINANCE16);
		translateEnum(GL_LUMINANCE4_ALPHA4);
		translateEnum(GL_LUMINANCE6_ALPHA2);
		translateEnum(GL_LUMINANCE8_ALPHA8);
		translateEnum(GL_LUMINANCE12_ALPHA4);
		translateEnum(GL_LUMINANCE12_ALPHA12);
		translateEnum(GL_LUMINANCE16_ALPHA16);
		translateEnum(GL_INTENSITY);
		translateEnum(GL_INTENSITY4);
		translateEnum(GL_INTENSITY8);
		translateEnum(GL_INTENSITY12);
		translateEnum(GL_INTENSITY16);
		translateEnum(GL_R3_G3_B2);
		translateEnum(GL_RGB4);
		translateEnum(GL_RGB5);
		translateEnum(GL_RGB8);
		translateEnum(GL_RGB10);
		translateEnum(GL_RGB12);
		translateEnum(GL_RGB16);
		translateEnum(GL_RGBA2);
		translateEnum(GL_RGBA4);
		translateEnum(GL_RGB5_A1);
		translateEnum(GL_RGBA8);
		translateEnum(GL_RGB10_A2);
		translateEnum(GL_RGBA12);
		translateEnum(GL_RGBA16);
		
		translateEnum(GL_CLAMP);
		
		translateEnum(GL_QUAD_STRIP);
		translateEnum(GL_QUADS);
		translateEnum(GL_POLYGON);
#endif
		default:
			return NULL;
	}
}
