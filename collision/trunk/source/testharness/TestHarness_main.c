/*
  Copyright (c) 2015 Alex Diener
  
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

#include "collision/CollisionCircle.h"
#include "collision/CollisionRect2D.h"
#include "collision/CollisionResolver.h"
#include "collision/CollisionShared.h"
#include "collision/IntersectionManager.h"

#include "gamemath/Matrix.h"
#include "gamemath/Vector2f.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "shell/Shell.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"

#if defined(STEM_PLATFORM_macosx)
#include "nsopenglshell/NSOpenGLShell.h"
#include "nsopenglshell/NSOpenGLTarget.h"
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
#include "wglshell/WGLShell.h"
#include "wglshell/WGLTarget.h"
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
#include "glxshell/GLXShell.h"
#include "glxshell/GLXTarget.h"
#endif

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Feature wishlist:
// - Demo scenes: Prebuilt setups where collidable objects can be controlled
//  - Platformer controls with gravity
//  - Overhead controls
//  - Bouncing balls
//  - Single-frame and sub-frame simulation (set up lastPosition and position for each object, and step through intersection tests and resolution in detail)
// - Place arbitrary shapes into a simulation and allow them to intercollide
// - Stress test (control number/speed of collisiding objects with performance metrics)

static unsigned int viewWidth, viewHeight;
static float viewRatio;
static IntersectionManager * intersectionManager;
static CollisionResolver * resolver;
static Vector2x dragOrigin;
static bool dragging;
static size_t selectedObjectIndex;

static void loadScene1() {
	if (resolver != NULL) {
		size_t objectIndex;
		
		for (objectIndex = 0; objectIndex < resolver->objectCount; objectIndex++) {
			resolver->objects[objectIndex]->dispose(resolver->objects[objectIndex]);
		}
		CollisionResolver_dispose(resolver);
	}
	resolver = CollisionResolver_create(intersectionManager, false, MAX_SIMULTANEOUS_COLLISIONS_DEFAULT, MAX_ITERATIONS_DEFAULT);
	CollisionResolver_addObject(resolver, (CollisionObject *) CollisionRect2D_create(NULL, NULL, VECTOR2x(0x00000, 0x00000), VECTOR2x(0x10000, 0x10000), false));
	CollisionResolver_addObject(resolver, (CollisionObject *) CollisionRect2D_create(NULL, NULL, VECTOR2x(0x20000, -0x40000), VECTOR2x(0x50000, 0x20000), false));
	CollisionResolver_addObject(resolver, (CollisionObject *) CollisionCircle_create(NULL, NULL, VECTOR2x(-0x30000, 0x00000), 0x10000, false));
	CollisionResolver_addObject(resolver, (CollisionObject *) CollisionCircle_create(NULL, NULL, VECTOR2x(-0x20000, 0x50000), 0x08000, false));
}

#define CIRCLE_TESSELATIONS 64

static void getCollisionObjectVertices2D(struct vertex_p2f_c4f * outVertices, GLuint * outIndexes, size_t * ioVertexCount, size_t * ioIndexCount) {
	size_t objectIndex;
	CollisionObject * object;
	struct vertex_p2f_c4f vertex;
	bool colliding;
	CollisionRecord collision;
	
	for (objectIndex = 0; objectIndex < resolver->objectCount; objectIndex++) {
		object = resolver->objects[objectIndex];
		colliding = CollisionResolver_querySingle(resolver, object, &collision);
		switch (object->shapeType) {
			case COLLISION_SHAPE_RECT_2D: {
				CollisionRect2D * rect = (CollisionRect2D *) object;
				
				if (outVertices != NULL) {
					vertex.color[0] = 0.5f;
					vertex.color[1] = 0.25f;
					vertex.color[2] = 0.0f;
					vertex.color[3] = 1.0f;
					vertex.position[0] = xtof(rect->lastPosition.x);
					vertex.position[1] = xtof(rect->lastPosition.y);
					outVertices[*ioVertexCount + 0] = vertex;
					vertex.position[0] = xtof(rect->lastPosition.x + rect->lastSize.x);
					outVertices[*ioVertexCount + 1] = vertex;
					vertex.position[1] = xtof(rect->lastPosition.y + rect->lastSize.y);
					outVertices[*ioVertexCount + 2] = vertex;
					vertex.position[0] = xtof(rect->lastPosition.x);
					outVertices[*ioVertexCount + 3] = vertex;
					
					if (colliding) {
						vertex.color[0] = 1.0f;
						vertex.color[1] = 0.0f;
						vertex.color[2] = 0.0f;
						vertex.color[3] = 1.0f;
					} else {
						vertex.color[0] = 1.0f;
						vertex.color[1] = 1.0f;
						vertex.color[2] = 0.0f;
						vertex.color[3] = 1.0f;
					}
					vertex.position[0] = xtof(rect->position.x);
					vertex.position[1] = xtof(rect->position.y);
					outVertices[*ioVertexCount + 4] = vertex;
					vertex.position[0] = xtof(rect->position.x + rect->size.x);
					outVertices[*ioVertexCount + 5] = vertex;
					vertex.position[1] = xtof(rect->position.y + rect->size.y);
					outVertices[*ioVertexCount + 6] = vertex;
					vertex.position[0] = xtof(rect->position.x);
					outVertices[*ioVertexCount + 7] = vertex;
				}
				
				if (outIndexes != NULL) {
					unsigned int vertexIndex;
					
					for (vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
						outIndexes[*ioIndexCount + vertexIndex * 2 + 0] = *ioVertexCount + vertexIndex;
						outIndexes[*ioIndexCount + vertexIndex * 2 + 1] = *ioVertexCount + (vertexIndex + 1) % 4;
					}
					for (vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
						outIndexes[*ioIndexCount + vertexIndex * 2 + 8] = *ioVertexCount + 0 + vertexIndex;
						outIndexes[*ioIndexCount + vertexIndex * 2 + 9] = *ioVertexCount + 4 + vertexIndex;
					}
					for (vertexIndex = 0; vertexIndex < 4; vertexIndex++) {
						outIndexes[*ioIndexCount + vertexIndex * 2 + 16] = *ioVertexCount + 4 + vertexIndex;
						outIndexes[*ioIndexCount + vertexIndex * 2 + 17] = *ioVertexCount + 4 + (vertexIndex + 1) % 4;
					}
				}
				
				*ioVertexCount += 8;
				*ioIndexCount += 24;
				break;
			}
			case COLLISION_SHAPE_CIRCLE: {
				CollisionCircle * circle = (CollisionCircle *) object;
				unsigned int tesselationIndex;
				
				if (outVertices != NULL) {
					vertex.color[0] = 0.0f;
					vertex.color[1] = 0.25f;
					vertex.color[2] = 0.5f;
					vertex.color[3] = 1.0f;
					for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
						vertex.position[0] = xtof(circle->lastPosition.x) + xtof(circle->lastRadius) * cos(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						vertex.position[1] = xtof(circle->lastPosition.y) + xtof(circle->lastRadius) * sin(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						outVertices[*ioVertexCount + tesselationIndex] = vertex;
					}
					
					if (colliding) {
						vertex.color[0] = 1.0f;
						vertex.color[1] = 0.0f;
						vertex.color[2] = 0.0f;
						vertex.color[3] = 1.0f;
					} else {
						vertex.color[0] = 0.0f;
						vertex.color[1] = 1.0f;
						vertex.color[2] = 1.0f;
						vertex.color[3] = 1.0f;
					}
					for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
						vertex.position[0] = xtof(circle->position.x) + xtof(circle->radius) * cos(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						vertex.position[1] = xtof(circle->position.y) + xtof(circle->radius) * sin(tesselationIndex * M_PI * 2 / CIRCLE_TESSELATIONS);
						outVertices[*ioVertexCount + CIRCLE_TESSELATIONS + tesselationIndex] = vertex;
					}
				}
				
				if (outIndexes != NULL) {
					float angle;
					int bestEdgeIndex;
					
					for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
						outIndexes[*ioIndexCount + tesselationIndex * 2 + 0] = *ioVertexCount + tesselationIndex;
						outIndexes[*ioIndexCount + tesselationIndex * 2 + 1] = *ioVertexCount + (tesselationIndex + 1) % CIRCLE_TESSELATIONS;
					}
					
					angle = atan2(circle->position.y - circle->lastPosition.y, circle->position.x - circle->lastPosition.x);
					bestEdgeIndex = round((angle + M_PI / 2) * CIRCLE_TESSELATIONS / (M_PI * 2));
					bestEdgeIndex = (bestEdgeIndex % CIRCLE_TESSELATIONS + CIRCLE_TESSELATIONS) % CIRCLE_TESSELATIONS;
					outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 0] = *ioVertexCount + bestEdgeIndex;
					outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 1] = *ioVertexCount + bestEdgeIndex + CIRCLE_TESSELATIONS;
					bestEdgeIndex += CIRCLE_TESSELATIONS / 2;
					bestEdgeIndex %= CIRCLE_TESSELATIONS;
					outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 2] = *ioVertexCount + bestEdgeIndex;
					outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 3] = *ioVertexCount + bestEdgeIndex + CIRCLE_TESSELATIONS;
					
					for (tesselationIndex = 0; tesselationIndex < CIRCLE_TESSELATIONS; tesselationIndex++) {
						outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 4 + tesselationIndex * 2] = *ioVertexCount + CIRCLE_TESSELATIONS + tesselationIndex;
						outIndexes[*ioIndexCount + CIRCLE_TESSELATIONS * 2 + 5 + tesselationIndex * 2] = *ioVertexCount + CIRCLE_TESSELATIONS + (tesselationIndex + 1) % CIRCLE_TESSELATIONS;
					}
				}
				
				*ioVertexCount += CIRCLE_TESSELATIONS * 2;
				*ioIndexCount += CIRCLE_TESSELATIONS * 4 + 4;
				break;
			}
		}
	}
}

static bool Target_draw() {
	static GLuint vertexBufferID, indexBufferID;
	struct vertex_p2f_c4f * vertices;
	size_t vertexCount;
	GLuint * indexes;
	size_t indexCount;
	Matrix matrix;
	
	if (vertexBufferID == 0) {
		glGenBuffersARB(1, &vertexBufferID);
		glGenBuffersARB(1, &indexBufferID);
	}
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	vertexCount = indexCount = 0;
	getCollisionObjectVertices2D(NULL, NULL, &vertexCount, &indexCount);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(struct vertex_p2f_c4f) * vertexCount, NULL, GL_STREAM_DRAW);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, NULL, GL_STREAM_DRAW);
	vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	indexes = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertexCount = indexCount = 0;
	getCollisionObjectVertices2D(vertices, indexes, &vertexCount, &indexCount);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	
	matrix = Matrix_ortho(MATRIX_IDENTITY, -12.0f * viewRatio, 12.0f * viewRatio, -12.0f, 12.0f, -1.0f, 1.0f);
	glLoadMatrixf(matrix.m);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_p2f_c4f), (void *) offsetof(struct vertex_p2f_c4f, position));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_p2f_c4f), (void *) offsetof(struct vertex_p2f_c4f, color));
	glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	return true;
}

static Vector2x transformMousePosition(float x, float y) {
	Vector2x result;
	
	result.x = ftox((x - viewWidth / 2) / viewWidth * 2 * 12 * viewRatio);
	result.y = ftox((viewHeight / 2 - y) / viewHeight * 2 * 12);
	return result;
}

static void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifiers, bool isRepeat) {
	if (keyCode == KEYBOARD_RETURN_OR_ENTER && (modifiers & MODIFIER_ALT_BIT)) {
		if (Shell_isFullScreen()) {
			Shell_exitFullScreen();
		} else {
			Shell_enterFullScreen(Shell_getDisplayIndexFromWindow());
		}
		
	} else if (keyCode == KEYBOARD_TAB && !dragging) {
		selectedObjectIndex++;
		selectedObjectIndex %= resolver->objectCount;
	}
}

static void Target_keyUp(unsigned int keyCode, unsigned int modifiers) {
}

static void Target_mouseDown(unsigned int buttonNumber, float x, float y) {
	CollisionObject * object = resolver->objects[selectedObjectIndex];
	
	dragOrigin = transformMousePosition(x, y);
	switch (object->shapeType) {
		case COLLISION_SHAPE_RECT_2D: {
			CollisionRect2D * rect = (CollisionRect2D *) object;
			dragOrigin.x -= rect->position.x;
			dragOrigin.y -= rect->position.y;
			break;
		}
		case COLLISION_SHAPE_CIRCLE: {
			CollisionCircle * circle = (CollisionCircle *) object;
			dragOrigin.x -= circle->position.x;
			dragOrigin.y -= circle->position.y;
			break;
		}
	}
	dragging = true;
}

static void Target_mouseUp(unsigned int buttonNumber, float x, float y) {
	dragging = false;
}

static void Target_mouseMoved(float x, float y) {
}

static void Target_mouseDragged(unsigned int buttonMask, float x, float y) {
	Vector2x mousePosition = transformMousePosition(x, y);
	CollisionObject * object = resolver->objects[selectedObjectIndex];
	
	switch (object->shapeType) {
		case COLLISION_SHAPE_RECT_2D: {
			CollisionRect2D * rect = (CollisionRect2D *) object;
			rect->position.x = mousePosition.x - dragOrigin.x;
			rect->position.y = mousePosition.y - dragOrigin.y;
			break;
		}
		case COLLISION_SHAPE_CIRCLE: {
			CollisionCircle * circle = (CollisionCircle *) object;
			circle->position.x = mousePosition.x - dragOrigin.x;
			circle->position.y = mousePosition.y - dragOrigin.y;
			break;
		}
	}
	Shell_redisplay();
}

static void Target_resized(unsigned int newWidth, unsigned int newHeight) {
	viewWidth = newWidth;
	viewHeight = newHeight;
	viewRatio = (float) newWidth / newHeight;
	glViewport(0, 0, newWidth, newHeight);
}

static void Target_backgrounded() {
}

static void Target_foregrounded() {
}

static void registerShellCallbacks() {
	Shell_drawFunc(Target_draw);
	Shell_resizeFunc(Target_resized);
	Shell_keyDownFunc(Target_keyDown);
	Shell_keyUpFunc(Target_keyUp);
	Shell_mouseDownFunc(Target_mouseDown);
	Shell_mouseUpFunc(Target_mouseUp);
	Shell_mouseMovedFunc(Target_mouseMoved);
	Shell_mouseDraggedFunc(Target_mouseDragged);
	Shell_backgroundedFunc(Target_backgrounded);
	Shell_foregroundedFunc(Target_foregrounded);
}

#if defined(STEM_PLATFORM_macosx)
void NSOpenGLTarget_configure(int argc, const char ** argv, struct NSOpenGLShellConfiguration * configuration) {
	configuration->fullScreenMenuItem = true;
#elif defined(STEM_PLATFORM_win32) || defined(STEM_PLATFORM_win64)
void WGLTarget_configure(void * instance, void * prevInstance, char * commandLine, int command, int argc, const char ** argv, struct WGLShellConfiguration * configuration) {
#elif defined(STEM_PLATFORM_linux32) || defined(STEM_PLATFORM_linux64)
void GLXTarget_configure(int argc, const char ** argv, struct GLXShellConfiguration * configuration) {
#else
#error Unsupported platform
#endif
	configuration->windowTitle = "Collision Test Harness";
	registerShellCallbacks();
}

void Target_init() {
	intersectionManager = IntersectionManager_createWithStandardHandlers();
	loadScene1();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	Shell_setVSync(false, false);
	Shell_setVSync(false, true);
	Shell_mainLoop();
}
