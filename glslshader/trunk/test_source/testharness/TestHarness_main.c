#include "shell/Shell.h"
#include "shell/Target.h"
#include "glgraphics/GLGraphics.h"
#include "glslshader/GLSLShader.h"
#include "utilities/IOUtilities.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#if TARGET_OPENGL_ES
#include "eaglshell/EAGLShell.h"
#include "eaglshell/EAGLTarget.h"
#else
#include "glutshell/GLUTTarget.h"
#endif

static GLSLShader * shader1, * shader2;
static GLuint vboID, eboID;

struct vertex_p2f_c4f {
	GLfloat position[2];
	GLfloat color[4];
};

void Target_init() {
	char * vshaderSource, * fshaderSource;
	size_t vshaderLength, fshaderLength;
	struct vertex_p2f_c4f vertices[3] = {
		{{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{{1.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
	};
	GLshort indexes[3] = {0, 1, 2};
	
#ifndef GLGRAPHICS_NO_GLEW
	if (!GLEW_ARB_shader_objects ||
	    !GLEW_ARB_vertex_shader ||
	    !GLEW_ARB_fragment_shader ||
	    !GLEW_ARB_shading_language_100) {
		fprintf(stderr, "GLSL is not supported on this system. Test harness cannot run.\n");
		exit(EXIT_FAILURE);
	}
#endif
	
	chdir(Shell_getResourcePath());
	
	vshaderSource = readFileSimple("shader1.vert", &vshaderLength);
	fshaderSource = readFileSimple("shader1.frag", &fshaderLength);
	shader1 = GLSLShader_create(vshaderSource, vshaderLength, fshaderSource, fshaderLength, "vertexPosition", 0, NULL);
	free(vshaderSource);
	free(fshaderSource);
	
	vshaderSource = readFileSimple("shader2.vert", &vshaderLength);
	fshaderSource = readFileSimple("shader2.frag", &fshaderLength);
	shader2 = GLSLShader_create(vshaderSource, vshaderLength, fshaderSource, fshaderLength, "vertexPosition", 0, "colorAttribute", 1, NULL);
	free(vshaderSource);
	free(fshaderSource);
	
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct vertex_p2f_c4f) * 3, vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * 3, indexes, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p2f_c4f), (void *) offsetof(struct vertex_p2f_c4f, position));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p2f_c4f), (void *) offsetof(struct vertex_p2f_c4f, color));
	glEnableVertexAttribArray(0);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	Shell_mainLoop();
}

bool Target_draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	shader1->activate(shader1);
	glUniform4f(shader1->getUniformLocation(shader1, "colorUniform"), 1.0f, 1.0f, 1.0f, 1.0f);
	glDisableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);
	shader1->deactivate(shader1);
	
	shader2->activate(shader2);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, NULL);
	shader2->deactivate(shader2);
	
	return true;
}

void Target_resized(unsigned int newWidth, unsigned int newHeight) {
}

void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags) {
}

void Target_keyUp(unsigned int charCode, unsigned int modifierFlags) {
}

void Target_keyModifiersChanged(unsigned int modifierFlags) {
}

void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
}

void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
}

void Target_mouseMoved(float x, float y) {
}

void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
}

#if TARGET_OPENGL_ES
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration) {
	configuration->preferredOpenGLAPIVersion = EAGLShellOpenGLVersion_ES2;
}

void EAGLTarget_openURL(const char * url) {
}

void EAGLTarget_touchesCancelled(unsigned int buttonMask) {
}

void EAGLTarget_accelerometer(double x, double y, double z) {
}
#else
void GLUTTarget_configure(int argc, const char ** argv, struct GLUTShellConfiguration * configuration) {
	configuration->windowTitle = "GLSLShader Test Harness";
}
#endif
