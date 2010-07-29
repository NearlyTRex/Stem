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

#include "glgraphics/GLSLShader.h"
#include <stdio.h>

GLSLShader * GLSLShader_create(const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, ...) {
	GLSLShader * self;
	va_list args;
	
	va_start(args, fshaderLength);
	self = GLSLShader_vcreate(vshaderSource, vshaderLength, fshaderSource, fshaderLength, args);
	va_end(args);
	return self;
}

GLSLShader * GLSLShader_vcreate(const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, va_list args) {
	stemobject_create_implementation(GLSLShader, vinit, vshaderSource, vshaderLength, fshaderSource, fshaderLength, args)
}

void GLSLShader_init(compat_type(GLSLShader *) selfPtr, const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, ...) {
	GLSLShader * self = selfPtr;
	va_list args;
	
	va_start(args, fshaderLength);
	self = GLSLShader_vcreate(vshaderSource, vshaderLength, fshaderSource, fshaderLength, args);
	va_end(args);
}

void GLSLShader_vinit(compat_type(GLSLShader *) selfPtr, const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, va_list args) {
	GLSLShader * self = selfPtr;
	GLint shaderLength;
	GLuint vertexShader, fragmentShader;
	const char * attribName;
	GLuint attribLocation;
#ifdef DEBUG
	GLint logLength;
#endif
	
	StemObject_init(self);
	self->dispose = GLSLShader_dispose;
	self->getUniformLocation = GLSLShader_getUniformLocation;
	self->activate = GLSLShader_activate;
	self->deactivate = GLSLShader_deactivate;
	
	self->program = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	shaderLength = vshaderLength;
	glShaderSource(vertexShader, 1, (const GLchar **) &vshaderSource, &shaderLength);
	glCompileShader(vertexShader);
#ifdef DEBUG
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar * log = malloc(logLength);
		glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
		fprintf(stderr, "Vertex shader compile log:\n%s\n", log);
		free(log);
	}
#endif
	
	shaderLength = fshaderLength;
	glShaderSource(fragmentShader, 1, (const GLchar **) &fshaderSource, &shaderLength);
	glCompileShader(fragmentShader);
#ifdef DEBUG
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar * log = malloc(logLength);
		glGetShaderInfoLog(fragmentShader, logLength, &logLength, log);
		fprintf(stderr, "Fragment shader compile log:\n%s\n", log);
		free(log);
	}
#endif
	
	glAttachShader(self->program, vertexShader);
	glAttachShader(self->program, fragmentShader);
	
	for (attribName = va_arg(args, const char *); attribName != NULL; attribName = va_arg(args, const char *)) {
		attribLocation = va_arg(args, GLuint);
		glBindAttribLocation(self->program, attribLocation, attribName);
	}
	
	glLinkProgram(self->program);
#ifdef DEBUG
	glGetProgramiv(self->program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar * log = malloc(logLength);
		glGetProgramInfoLog(self->program, logLength, &logLength, log);
		fprintf(stderr, "Program link log:\n%s\n", log);
		free(log);
	}
	
	glValidateProgram(self->program);
	glGetProgramiv(self->program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar * log = malloc(logLength);
		glGetProgramInfoLog(self->program, logLength, &logLength, log);
		fprintf(stderr, "Program validation log:\n%s\n", log);
		free(log);
	}
#endif
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void GLSLShader_dispose(compat_type(GLSLShader *) selfPtr) {
	GLSLShader * self = selfPtr;
	
	glDeleteProgram(self->program);
	StemObject_dispose(selfPtr);
}

GLint GLSLShader_getUniformLocation(compat_type(GLSLShader *) selfPtr, const char * uniformName) {
	GLSLShader * self = selfPtr;
	
	return glGetUniformLocation(self->program, uniformName);
}

void GLSLShader_activate(compat_type(GLSLShader *) selfPtr) {
	GLSLShader * self = selfPtr;
	
	glUseProgram(self->program);
}

void GLSLShader_deactivate(compat_type(GLSLShader *) selfPtr) {
	glUseProgram(0);
}
