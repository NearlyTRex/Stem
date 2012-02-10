/*
  Copyright (c) 2012 Alex Diener
  
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

#define SUPERCLASS StemObject

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

void GLSLShader_init(GLSLShader * self, const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, ...) {
	va_list args;
	
	va_start(args, fshaderLength);
	self = GLSLShader_vcreate(vshaderSource, vshaderLength, fshaderSource, fshaderLength, args);
	va_end(args);
}

void GLSLShader_vinit(GLSLShader * self, const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, va_list args) {
	GLint shaderLength;
	GLuint vertexShader, fragmentShader;
	const char * attribName;
	GLuint attribLocation;
#ifdef DEBUG
	GLint logLength;
#endif
	
	call_super(init, self);
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
		if (logLength > 0) { // Surprisingly enough, this is not always true! In the glgraphics test harness, Windows gives me 1 the first time, and 0 the second...
			fprintf(stderr, "Vertex shader compile log:\n%s\n", log);
		}
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
		if (logLength > 0) {
			fprintf(stderr, "Fragment shader compile log:\n%s\n", log);
		}
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
		if (logLength > 0) {
			fprintf(stderr, "Program link log:\n%s\n", log);
		}
		free(log);
	}
	
	glValidateProgram(self->program);
	glGetProgramiv(self->program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar * log = malloc(logLength);
		glGetProgramInfoLog(self->program, logLength, &logLength, log);
		if (logLength > 0) {
			fprintf(stderr, "Program validation log:\n%s\n", log);
		}
		free(log);
	}
#endif
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void GLSLShader_dispose(GLSLShader * self) {
	glDeleteProgram(self->program);
	call_super(dispose, self);
}

GLint GLSLShader_getUniformLocation(GLSLShader * self, const char * uniformName) {
	return glGetUniformLocation(self->program, uniformName);
}

void GLSLShader_activate(GLSLShader * self) {
	glUseProgram(self->program);
}

void GLSLShader_deactivate(GLSLShader * self) {
	glUseProgram(0);
}
