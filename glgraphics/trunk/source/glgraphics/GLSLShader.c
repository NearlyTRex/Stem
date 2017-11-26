/*
  Copyright (c) 2014 Alex Diener
  
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
  
  Alex Diener alex@ludobloom.com
*/

#include "glgraphics/GLSLShader.h"
#include "utilities/IOUtilities.h"
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

GLSLShader * GLSLShader_createWithFiles(const char * vshaderFilePath, const char * fshaderFilePath, ...) {
	GLSLShader * self;
	va_list args;
	
	va_start(args, fshaderFilePath);
	self = GLSLShader_vcreateWithFiles(vshaderFilePath, fshaderFilePath, args);
	va_end(args);
	return self;
}

GLSLShader * GLSLShader_vcreateWithFiles(const char * vshaderFilePath, const char * fshaderFilePath, va_list args) {
	stemobject_create_implementation(GLSLShader, vinitWithFiles, vshaderFilePath, fshaderFilePath, args)
}

bool GLSLShader_init(GLSLShader * self, const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, ...) {
	va_list args;
	bool success;
	
	va_start(args, fshaderLength);
	success = GLSLShader_vinit(self, vshaderSource, vshaderLength, fshaderSource, fshaderLength, args);
	va_end(args);
	return success;
}

bool GLSLShader_vinit(GLSLShader * self, const char * vshaderSource, size_t vshaderLength, const char * fshaderSource, size_t fshaderLength, va_list args) {
	GLint shaderLength;
	GLuint vertexShader, fragmentShader;
	const char * attribName;
	GLuint attribLocation;
#ifdef DEBUG
	GLint logLength;
#endif
	
	call_super(init, self);
	self->dispose = GLSLShader_dispose;
	
	self->programID = glCreateProgram();
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
	
	glAttachShader(self->programID, vertexShader);
	glAttachShader(self->programID, fragmentShader);
	
	for (attribName = va_arg(args, const char *); attribName != NULL; attribName = va_arg(args, const char *)) {
		attribLocation = va_arg(args, GLuint);
		glBindAttribLocation(self->programID, attribLocation, attribName);
	}
	
	glLinkProgram(self->programID);
#ifdef DEBUG
	glGetProgramiv(self->programID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar * log = malloc(logLength);
		glGetProgramInfoLog(self->programID, logLength, &logLength, log);
		if (logLength > 0) {
			fprintf(stderr, "Program link log:\n%s\n", log);
		}
		free(log);
	}
#endif
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return true;
}

bool GLSLShader_initWithFiles(GLSLShader * self, const char * vshaderFilePath, const char * fshaderFilePath, ...) {
	va_list args;
	bool success;
	
	va_start(args, fshaderFilePath);
	success = GLSLShader_vinitWithFiles(self, vshaderFilePath, fshaderFilePath, args);
	va_end(args);
	return success;
}

bool GLSLShader_vinitWithFiles(GLSLShader * self, const char * vshaderFilePath, const char * fshaderFilePath, va_list args) {
	void * vshader, * fshader;
	size_t vshaderLength, fshaderLength;
	bool success;
	
	vshader = readFileSimple(vshaderFilePath, &vshaderLength);
	fshader = readFileSimple(fshaderFilePath, &fshaderLength);
	success = GLSLShader_vinit(self, vshader, vshaderLength, fshader, fshaderLength, args);
	free(vshader);
	free(fshader);
	
	return success;
}

void GLSLShader_dispose(GLSLShader * self) {
	glDeleteProgram(self->programID);
	call_super(dispose, self);
}

GLint GLSLShader_getUniformLocation(GLSLShader * self, const char * uniformName) {
	return glGetUniformLocation(self->programID, uniformName);
}

bool GLSLShader_validate(GLSLShader * self) {
	GLint logLength;
	
	glValidateProgram(self->programID);
	glGetProgramiv(self->programID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar * log = malloc(logLength);
		glGetProgramInfoLog(self->programID, logLength, &logLength, log);
		if (logLength > 0) {
			fprintf(stderr, "Program validation log:\n%s\n", log);
		}
		free(log);
		return false;
	}
	return true;
}
