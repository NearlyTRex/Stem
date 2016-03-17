/*
  Copyright (c) 2016 Alex Diener
  
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

#include "gamemath/Matrix4x4f.h"
#include "gamemath/Quaternionf.h"
#include "gamemath/Vector3f.h"
#include "gamemath/VectorConversions.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include "testharness/Obj3DModelIO.h"
#include "testharness/SharedEvents.h"
#include "testharness/TestHarness_globals.h"
#include "testharness/TrimeshViewerScreen.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define SUPERCLASS Screen

TrimeshViewerScreen * TrimeshViewerScreen_create(ResourceManager * resourceManager) {
	stemobject_create_implementation(TrimeshViewerScreen, init, resourceManager)
}

bool TrimeshViewerScreen_init(TrimeshViewerScreen * self, ResourceManager * resourceManager) {
	call_super(init, self);
	self->dispose = TrimeshViewerScreen_dispose;
	self->activate = TrimeshViewerScreen_activate;
	self->deactivate = TrimeshViewerScreen_deactivate;
	glGenBuffers(1, &self->vertexBuffer);
	glGenBuffers(1, &self->indexBuffer);
	self->resourceManager = resourceManager;
	self->lightShader = GLSLShader_createWithFiles("litsurface.vert", "litsurface.frag", NULL);
	return true;
}

void TrimeshViewerScreen_dispose(TrimeshViewerScreen * self) {
	if (self->trimesh != NULL) {
		CollisionStaticTrimesh_dispose(self->trimesh);
		self->trimesh = NULL;
	}
	glDeleteBuffers(1, &self->vertexBuffer);
	glDeleteBuffers(1, &self->indexBuffer);
	GLSLShader_dispose(self->lightShader);
	call_super(dispose, self);
}

static void getTrimeshTriangleVertices(CollisionStaticTrimesh * trimesh, struct vertex_p3f_n3f * vertices, GLuint * indexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int triangleIndex;
	struct vertex_p3f_n3f vertex;
	Vector3f normal;
	Vector3f position;
	
	if (vertices != NULL) {
		for (triangleIndex = 0; triangleIndex < trimesh->triangleCount; triangleIndex++) {
			normal = Vector3x_toVector3f(trimesh->triangles[triangleIndex].normal);
			vertex.normal[0] = normal.x;
			vertex.normal[1] = normal.y;
			vertex.normal[2] = normal.z;
			position = Vector3x_toVector3f(trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[0]].position);
			vertex.position[0] = position.x;
			vertex.position[1] = position.y;
			vertex.position[2] = position.z;
			vertices[*ioVertexCount + triangleIndex * 3 + 0] = vertex;
			position = Vector3x_toVector3f(trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[1]].position);
			vertex.position[0] = position.x;
			vertex.position[1] = position.y;
			vertex.position[2] = position.z;
			vertices[*ioVertexCount + triangleIndex * 3 + 1] = vertex;
			position = Vector3x_toVector3f(trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[2]].position);
			vertex.position[0] = position.x;
			vertex.position[1] = position.y;
			vertex.position[2] = position.z;
			vertices[*ioVertexCount + triangleIndex * 3 + 2] = vertex;
		}
	}
	if (indexes != NULL) {
		for (triangleIndex = 0; triangleIndex < trimesh->triangleCount; triangleIndex++) {
			indexes[*ioIndexCount + triangleIndex * 3 + 0] = *ioVertexCount + triangleIndex * 3 + 0;
			indexes[*ioIndexCount + triangleIndex * 3 + 1] = *ioVertexCount + triangleIndex * 3 + 1;
			indexes[*ioIndexCount + triangleIndex * 3 + 2] = *ioVertexCount + triangleIndex * 3 + 2;
		}
	}
	*ioVertexCount += trimesh->triangleCount * 3;
	*ioIndexCount += trimesh->triangleCount * 3;
}

#define NORMAL_SCALE 0.25f

static void getTrimeshNormalVertices(CollisionStaticTrimesh * trimesh, struct vertex_p3f_n3f * vertices, GLuint * indexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int vertexIndex;
	struct vertex_p3f_n3f vertex;
	Vector3f normal;
	Vector3f position;
	
	if (vertices != NULL) {
		for (vertexIndex = 0; vertexIndex < trimesh->vertexCount; vertexIndex++) {
			normal = Vector3x_toVector3f(trimesh->vertices[vertexIndex].normal);
			vertex.normal[0] = normal.x;
			vertex.normal[1] = normal.y;
			vertex.normal[2] = normal.z;
			position = Vector3x_toVector3f(trimesh->vertices[vertexIndex].position);
			vertex.position[0] = position.x;
			vertex.position[1] = position.y;
			vertex.position[2] = position.z;
			vertices[*ioVertexCount + vertexIndex * 2 + 0] = vertex;
			vertex.position[0] += normal.x * NORMAL_SCALE;
			vertex.position[1] += normal.y * NORMAL_SCALE;
			vertex.position[2] += normal.z * NORMAL_SCALE;
			vertices[*ioVertexCount + vertexIndex * 2 + 1] = vertex;
		}
	}
	if (indexes != NULL) {
		for (vertexIndex = 0; vertexIndex < trimesh->vertexCount; vertexIndex++) {
			indexes[*ioIndexCount + vertexIndex * 2 + 0] = *ioVertexCount + vertexIndex * 2 + 0;
			indexes[*ioIndexCount + vertexIndex * 2 + 1] = *ioVertexCount + vertexIndex * 2 + 1;
		}
	}
	*ioVertexCount += trimesh->vertexCount * 2;
	*ioIndexCount += trimesh->vertexCount * 2;
}

static void initVBOWithTrimesh(TrimeshViewerScreen * self, CollisionStaticTrimesh * trimesh) {
	struct vertex_p3f_n3f * vertices;
	GLuint * indexes;
	unsigned int vertexCount, indexCount;
	
	glBindBufferARB(GL_ARRAY_BUFFER, self->vertexBuffer);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, self->indexBuffer);
	vertexCount = indexCount = 0;
	getTrimeshTriangleVertices(trimesh, NULL, NULL, &vertexCount, &indexCount);
	self->indexCountTriangle = indexCount;
	getTrimeshNormalVertices(trimesh, NULL, NULL, &vertexCount, &indexCount);
	self->indexCountNormal = indexCount - self->indexCountTriangle;
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(struct vertex_p3f_n3f) * vertexCount, NULL, GL_STATIC_DRAW);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, NULL, GL_STATIC_DRAW);
	vertices = glMapBufferARB(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	indexes = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertexCount = indexCount = 0;
	getTrimeshTriangleVertices(trimesh, vertices, indexes, &vertexCount, &indexCount);
	getTrimeshNormalVertices(trimesh, vertices, indexes, &vertexCount, &indexCount);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
}

static void loadTrimeshPreset1(TrimeshViewerScreen * self) {
	CollisionStaticTrimesh * trimesh;
	Vector3x vertices[] = {{0x01000, 0x01000, 0x01000}, {0x20000, 0x00000, 0x00000}, {0x00000, 0x00000, 0x20000},
	                       {0x01000, 0x01000, 0x01000}, {0x00000, 0x00000, 0x20000}, {0x00000, 0x20000, 0x00000},
	                       {0x01000, 0x01000, 0x01000}, {0x00000, 0x20000, 0x00000}, {0x20000, 0x00000, 0x00000},
	                       {0x00000, 0x20000, 0x00000}, {0x00000, 0x00000, 0x20000}, {0x20000, 0x00000, 0x00000}};
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, vertices, 12);
	initVBOWithTrimesh(self, trimesh);
	CollisionStaticTrimesh_dispose(trimesh);
	Shell_redisplay();
}

static void loadTrimeshPreset2(TrimeshViewerScreen * self) {
	CollisionStaticTrimesh * trimesh;
	Vector3x vertices[] = {{0x00000, 0x00000, 0x00000}, {0x10000, 0x00000, 0x00000}, {0x00000, 0x10000, 0x00000},
	                       {0x00000, 0x10000, 0x00000}, {0x10000, 0x00000, 0x00000}, {0x10000, 0x10000, -0x10000},
	                       {0x10000, 0x00000, 0x00000}, {0x00000, 0x00000, 0x00000}, {0x10000, 0x10000, -0x10000}};
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, vertices, 9);
	initVBOWithTrimesh(self, trimesh);
	CollisionStaticTrimesh_dispose(trimesh);
	Shell_redisplay();
}

static bool draw(Atom eventID, void * eventData, void * context) {
	TrimeshViewerScreen * self = context;
	Matrix4x4f matrix;
	Vector3f cameraPosition;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	matrix = Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, 70.0f, g_viewRatio, 0.5f, 800.0f);
	Matrix4x4f_translate(&matrix, 0.0f, 0.0f, -xtof(self->cameraDistance));
	Matrix4x4f_multiply(&matrix, Quaternionf_toMatrix(Quaternionx_toQuaternionf(self->cameraDirection)));
	Matrix4x4f_translate(&matrix, xtof(self->cameraFocus.x), xtof(self->cameraFocus.y), xtof(self->cameraFocus.z));
	glLoadMatrixf(matrix.m);
	
	GLSLShader_activate(self->lightShader);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "light0Position"), 0.0f, 8.0f, 8.0f);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "light0Color"), 1.0f, 1.0f, 1.0f);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "light1Position"), -1.0f, -2.0f, -8.0f);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "light1Color"), 0.8f, 0.8f, 0.8f);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "ambientColor"), 0.2f, 0.2f, 0.105f);
	glUniform1f(GLSLShader_getUniformLocation(self->lightShader, "specularIntensity"), 0.875f);
	glUniform1f(GLSLShader_getUniformLocation(self->lightShader, "shininess"), 32.0f);
	cameraPosition = Vector3f_add(Quaternionf_multiplyVector3f(Quaternionx_toQuaternionf(self->cameraDirection), VECTOR3f(0.0f, 0.0f, -xtof(self->cameraDistance))), Vector3x_toVector3f(self->cameraFocus));
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "cameraPosition"), cameraPosition.x, cameraPosition.y, -cameraPosition.z);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER, self->vertexBuffer);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, self->indexBuffer);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex_p3f_n3f), (void *) offsetof(struct vertex_p3f_n3f, position));
	glNormalPointer(GL_FLOAT, sizeof(struct vertex_p3f_n3f), (void *) offsetof(struct vertex_p3f_n3f, normal));
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDrawElements(GL_TRIANGLES, self->indexCountTriangle, GL_UNSIGNED_INT, 0);
	GLSLShader_deactivate(self->lightShader);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINES, self->indexCountNormal, GL_UNSIGNED_INT, (void *) (ptrdiff_t) (self->indexCountTriangle * sizeof(GLuint)));
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_CULL_FACE);
	
	return true;
}

static bool keyDown(Atom eventID, void * eventData, void * context) {
	TrimeshViewerScreen * self = context;
	struct keyEvent * event = eventData;
	
	switch (event->keyCode) {
		case KEYBOARD_O: {
			char filePath[PATH_MAX];
			if (Shell_openFileDialog(NULL, filePath, PATH_MAX)) {
				CollisionStaticTrimesh * trimesh = loadObjStaticModelFile(filePath);
				if (trimesh != NULL) {
					initVBOWithTrimesh(self, trimesh);
					CollisionStaticTrimesh_dispose(trimesh);
					Shell_redisplay();
				}
			}
			break;
		}
		case KEYBOARD_1:
			loadTrimeshPreset1(self);
			break;
			
		case KEYBOARD_2:
			loadTrimeshPreset2(self);
			break;
	}
	return false;
}

static bool mouseDown(Atom eventID, void * eventData, void * context) {
	Shell_setMouseDeltaMode(true);
	Shell_redisplay();
	return true;
}

static bool mouseUp(Atom eventID, void * eventData, void * context) {
	Shell_setMouseDeltaMode(false);
	Shell_redisplay();
	return true;
}

#define CAMERA_LOOK_SENSITIVITY 0x00100
#define CAMERA_ZOOM_SENSITIVITY 0x00060
#define CAMERA_DRAG_SENSITIVITY 0x00060

static bool mouseDragged(Atom eventID, void * eventData, void * context) {
	TrimeshViewerScreen * self = context;
	struct mouseEvent * event = eventData;
	Vector3x offset;
	
	if (g_shiftKeyDown) {
		// Pan
		if (g_controlKeyDown) {
			offset.x = 0x00000;
			offset.y = 0x00000;
			offset.z = -xmul(ftox(event->position.y), xmul(CAMERA_DRAG_SENSITIVITY, self->cameraDistance));
		} else {
			offset.x = xmul(ftox(event->position.x), xmul(CAMERA_DRAG_SENSITIVITY, self->cameraDistance));
			offset.y = -xmul(ftox(event->position.y), xmul(CAMERA_DRAG_SENSITIVITY, self->cameraDistance));
			offset.z = 0x00000;
		}
		offset = Quaternionx_multiplyVector3x(Quaternionx_inverted(self->cameraDirection), offset);
		self->cameraFocus = Vector3x_add(self->cameraFocus, offset);
		Shell_redisplay();
		
	} else if (g_controlKeyDown) {
		// Zoom
		self->cameraDistance += xmul(ftox(event->position.y), xmul(CAMERA_ZOOM_SENSITIVITY, self->cameraDistance));
		if (self->cameraDistance < 0x10000) {
			self->cameraDistance = 0x10000;
		}
		Shell_redisplay();
		
	} else {
		// Rotate
		Quaternionx_rotate(&self->cameraDirection, VECTOR3x_UP, xmul(ftox(event->position.x), CAMERA_LOOK_SENSITIVITY));
		Quaternionx_rotate(&self->cameraDirection, Quaternionx_multiplyVector3x(Quaternionx_inverted(self->cameraDirection), VECTOR3x_RIGHT), xmul(ftox(event->position.y), CAMERA_LOOK_SENSITIVITY));
		Shell_redisplay();
	}
	
	return true;
}

static bool resized(Atom eventID, void * eventData, void * context) {
	Shell_redisplay();
	return true;
}

void TrimeshViewerScreen_activate(TrimeshViewerScreen * self) {
	loadTrimeshPreset1(self);
	self->trimesh = NULL;
	self->cameraFocus = VECTOR3x_ZERO;
	self->cameraDirection = QUATERNIONx_IDENTITY;
	self->cameraDistance = 0xF0000;
	
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), mouseDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), mouseUp, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), mouseDragged, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
	
	Shell_redisplay();
}

void TrimeshViewerScreen_deactivate(TrimeshViewerScreen * self) {
	if (self->trimesh != NULL) {
		CollisionStaticTrimesh_dispose(self->trimesh);
		self->trimesh = NULL;
	}
	
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), mouseDown, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), mouseUp, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), mouseDragged, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
}
