/*
  Copyright (c) 2010 Alex Diener
  
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

#ifndef __GLSL_SHADER_H__
#define __GLSL_SHADER_H__

typedef struct GLSLShader GLSLShader;

#include "glgraphics/GLIncludes.h"
#include "stemobject/StemObject.h"
#include <stdarg.h>
#include <stdlib.h>

#define GLSLShader_structContents \
	StemObject_structContents \
	\
	GLuint program; \
	\
	GLint (* getUniformLocation)(compat_type(GLSLShader *) self, const char * uniformName); \
	void (* activate)(compat_type(GLSLShader *) self); \
	void (* deactivate)(compat_type(GLSLShader *) self);

struct GLSLShader {
	GLSLShader_structContents
};

// Additional args are pairs of attribute names (const char *) and locations (GLuint) to bind to, terminated by NULL
GLSLShader * GLSLShader_create(const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, ...) __attribute__((sentinel));
GLSLShader * GLSLShader_vcreate(const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, va_list args);
void GLSLShader_init(compat_type(GLSLShader *) selfPtr, const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, ...) __attribute__((sentinel));
void GLSLShader_vinit(compat_type(GLSLShader *) selfPtr, const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, va_list args);

void GLSLShader_dispose(compat_type(GLSLShader *) selfPtr);
GLint GLSLShader_getUniformLocation(compat_type(GLSLShader *) selfPtr, const char * uniformName);
void GLSLShader_activate(compat_type(GLSLShader *) selfPtr);
void GLSLShader_deactivate(compat_type(GLSLShader *) selfPtr);

#endif
