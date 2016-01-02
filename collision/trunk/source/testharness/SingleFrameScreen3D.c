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

#include "collision/CollisionBox3D.h"
#include "collision/CollisionShared.h"
#include "collision/CollisionSphere.h"
#include "gamemath/Matrix4x4f.h"
#include "gamemath/MouseCoordinateTransforms.h"
#include "gamemath/Quaternionf.h"
#include "gamemath/VectorConversions.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "shell/Shell.h"
#include "testharness/SingleFrameScreen3D.h"
#include "testharness/SharedEvents.h"
#include "testharness/TestHarness_globals.h"
#include <stddef.h>
#include <stdlib.h>

#define SUPERCLASS Screen

#define SPHERE_SUBDIVISIONS 16
#define SPHERE_FACE_VERTEX_COUNT ((SPHERE_SUBDIVISIONS + 2) * (SPHERE_SUBDIVISIONS + 2))
#define SPHERE_VERTEX_COUNT (SPHERE_FACE_VERTEX_COUNT * 6)
#define SPHERE_FACE_INDEX_COUNT (6 * (SPHERE_SUBDIVISIONS + 1) * (SPHERE_SUBDIVISIONS + 1))
#define SPHERE_INDEX_COUNT (SPHERE_FACE_INDEX_COUNT * 6)

static struct vertex_p3f_n3f sphereVertexTemplate[SPHERE_VERTEX_COUNT];
static GLuint sphereIndexTemplate[SPHERE_INDEX_COUNT];

static void __attribute__((constructor)) initSphereTemplate() {
	unsigned int columnIndex, rowIndex, faceIndex, indexIndex;
	struct vertex_p3f_n3f vertex;
	Vector3f vector;
	Matrix4x4f faceMatrices[5] = {
		MATRIX4x4f(0.0f, 0.0f, -1.0f, 0.0f,
		           0.0f, 1.0f,  0.0f, 0.0f,
		           1.0f, 0.0f,  0.0f, 0.0f,
		           0.0f, 0.0f,  0.0f, 1.0f),
		MATRIX4x4f(0.0f, 0.0f, 1.0f, 0.0f,
		           0.0f, 1.0f, 0.0f, 0.0f,
		          -1.0f, 0.0f, 0.0f, 0.0f,
		           0.0f, 0.0f, 0.0f, 1.0f),
		MATRIX4x4f(1.0f, 0.0f,  0.0f, 0.0f,
		           0.0f, 0.0f, -1.0f, 0.0f,
		           0.0f, 1.0f,  0.0f, 0.0f,
		           0.0f, 0.0f,  0.0f, 1.0f),
		MATRIX4x4f(1.0f,  0.0f, 0.0f, 0.0f,
		           0.0f,  0.0f, 1.0f, 0.0f,
		           0.0f, -1.0f, 0.0f, 0.0f,
		           0.0f,  0.0f, 0.0f, 1.0f),
		MATRIX4x4f(1.0f,  0.0f,  0.0f, 0.0f,
		           0.0f, -1.0f,  0.0f, 0.0f,
		           0.0f,  0.0f, -1.0f, 0.0f,
		           0.0f,  0.0f,  0.0f, 1.0f)
	};
	
	for (rowIndex = 0; rowIndex < SPHERE_SUBDIVISIONS + 2; rowIndex++) {
		for (columnIndex = 0; columnIndex < SPHERE_SUBDIVISIONS + 2; columnIndex++) {
			vector.x = columnIndex * 2.0f / (SPHERE_SUBDIVISIONS + 1) - 1.0f;
			vector.y = rowIndex * 2.0f / (SPHERE_SUBDIVISIONS + 1) - 1.0f;
			vector.z = 1.0f;
			Vector3f_normalize(&vector);
			vertex.position[0] = vector.x;
			vertex.position[1] = vector.y;
			vertex.position[2] = vector.z;
			vertex.normal[0] = vector.x;
			vertex.normal[1] = vector.y;
			vertex.normal[2] = vector.z;
			sphereVertexTemplate[rowIndex * (SPHERE_SUBDIVISIONS + 2) + columnIndex] = vertex;
		}
	}
	
	for (faceIndex = 1; faceIndex < 6; faceIndex++) {
		for (rowIndex = 0; rowIndex < SPHERE_SUBDIVISIONS + 2; rowIndex++) {
			for (columnIndex = 0; columnIndex < SPHERE_SUBDIVISIONS + 2; columnIndex++) {
				vertex = sphereVertexTemplate[rowIndex * (SPHERE_SUBDIVISIONS + 2) + columnIndex];
				vector = Matrix4x4f_multiplyVector3f(faceMatrices[faceIndex - 1], VECTOR3f(vertex.position[0], vertex.position[1], vertex.position[2]));
				vertex.position[0] = vector.x;
				vertex.position[1] = vector.y;
				vertex.position[2] = vector.z;
				vertex.normal[0] = vector.x;
				vertex.normal[1] = vector.y;
				vertex.normal[2] = vector.z;
				sphereVertexTemplate[faceIndex * SPHERE_FACE_VERTEX_COUNT + rowIndex * (SPHERE_SUBDIVISIONS + 2) + columnIndex] = vertex;
			}
		}
	}
	
	for (faceIndex = 0; faceIndex < 6; faceIndex++) {
		for (rowIndex = 0; rowIndex < SPHERE_SUBDIVISIONS + 1; rowIndex++) {
			for (columnIndex = 0; columnIndex < SPHERE_SUBDIVISIONS + 1; columnIndex++) {
				indexIndex = faceIndex * SPHERE_FACE_INDEX_COUNT + rowIndex * 6 * (SPHERE_SUBDIVISIONS + 1) + columnIndex * 6;
				sphereIndexTemplate[indexIndex + 0] = faceIndex * SPHERE_FACE_VERTEX_COUNT + rowIndex * (SPHERE_SUBDIVISIONS + 2) + columnIndex;
				sphereIndexTemplate[indexIndex + 1] = faceIndex * SPHERE_FACE_VERTEX_COUNT + rowIndex * (SPHERE_SUBDIVISIONS + 2) + columnIndex + 1;
				sphereIndexTemplate[indexIndex + 2] = faceIndex * SPHERE_FACE_VERTEX_COUNT + (rowIndex + 1) * (SPHERE_SUBDIVISIONS + 2) + columnIndex + 1;
				sphereIndexTemplate[indexIndex + 3] = faceIndex * SPHERE_FACE_VERTEX_COUNT + (rowIndex + 1) * (SPHERE_SUBDIVISIONS + 2) + columnIndex + 1;
				sphereIndexTemplate[indexIndex + 4] = faceIndex * SPHERE_FACE_VERTEX_COUNT + (rowIndex + 1) * (SPHERE_SUBDIVISIONS + 2) + columnIndex;
				sphereIndexTemplate[indexIndex + 5] = faceIndex * SPHERE_FACE_VERTEX_COUNT + rowIndex * (SPHERE_SUBDIVISIONS + 2) + columnIndex;
			}
		}
	}
}

SingleFrameScreen3D * SingleFrameScreen3D_create(ResourceManager * resourceManager) {
	stemobject_create_implementation(SingleFrameScreen3D, init, resourceManager)
}

bool SingleFrameScreen3D_init(SingleFrameScreen3D * self, ResourceManager * resourceManager) {
	call_super(init, self);
	self->dispose = SingleFrameScreen3D_dispose;
	self->activate = SingleFrameScreen3D_activate;
	self->deactivate = SingleFrameScreen3D_deactivate;
	self->intersectionManager = IntersectionManager_createWithStandardHandlers();
	self->lightShader = GLSLShader_createWithFiles("litsurface.vert", "litsurface.frag", NULL);
	return true;
}

void SingleFrameScreen3D_dispose(SingleFrameScreen3D * self) {
	IntersectionManager_dispose(self->intersectionManager);
	GLSLShader_dispose(self->lightShader);
	call_super(dispose, self);
}

static void getSphereVertices(Vector3f position, float radius, Color4f color, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	if (outVertices != NULL) {
		unsigned int vertexIndex;
		struct vertex_p3f_n3f_c4f vertex;
		Vector3f vertexPosition;
		
		vertex.color[0] = color.red;
		vertex.color[1] = color.green;
		vertex.color[2] = color.blue;
		vertex.color[3] = color.alpha;
		for (vertexIndex = 0; vertexIndex < SPHERE_VERTEX_COUNT; vertexIndex++) {
			vertexPosition.x = sphereVertexTemplate[vertexIndex].position[0] + position.x;
			vertexPosition.y = sphereVertexTemplate[vertexIndex].position[1] + position.y;
			vertexPosition.z = sphereVertexTemplate[vertexIndex].position[2] + position.z;
			vertexPosition = Vector3f_multiplyScalar(vertexPosition, radius);
			vertex.position[0] = vertexPosition.x;
			vertex.position[1] = vertexPosition.y;
			vertex.position[2] = vertexPosition.z;
			vertex.normal[0] = sphereVertexTemplate[vertexIndex].normal[0];
			vertex.normal[1] = sphereVertexTemplate[vertexIndex].normal[1];
			vertex.normal[2] = sphereVertexTemplate[vertexIndex].normal[2];
			outVertices[*ioVertexCount + vertexIndex] = vertex;
		}
	}
	if (outIndexes != NULL) {
		unsigned int index;
		
		for (index = 0; index < SPHERE_INDEX_COUNT; index++) {
			outIndexes[*ioIndexCount + index] = sphereIndexTemplate[index] + *ioVertexCount;
		}
	}
	*ioVertexCount += SPHERE_VERTEX_COUNT;
	*ioIndexCount += SPHERE_INDEX_COUNT;
}

static void getCollisionObjectVertices(SingleFrameScreen3D * self, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	size_t objectIndex;
	CollisionObject * object;
	struct vertex_p3f_n3f_c4f vertex;
	
	for (objectIndex = 0; objectIndex < self->resolver->objectCount; objectIndex++) {
		object = self->resolver->objects[objectIndex];
		switch (object->shapeType) {
			case COLLISION_SHAPE_BOX_3D: {
				CollisionBox3D * box = (CollisionBox3D *) object;
				
				if (outVertices != NULL) {
					vertex.color[0] = 1.0f;
					vertex.color[1] = 1.0f;
					vertex.color[2] = 0.0f;
					vertex.color[3] = 1.0f;
					vertex.normal[0] = -1.0f;
					vertex.normal[1] = 0.0f;
					vertex.normal[2] = 0.0f;
					vertex.position[0] = xtof(box->position.x);
					vertex.position[1] = xtof(box->position.y);
					vertex.position[2] = xtof(box->position.z);
					outVertices[*ioVertexCount + 0] = vertex;
					vertex.position[2] = xtof(box->position.z + box->size.z);
					outVertices[*ioVertexCount + 1] = vertex;
					vertex.position[1] = xtof(box->position.y + box->size.y);
					outVertices[*ioVertexCount + 2] = vertex;
					vertex.position[2] = xtof(box->position.z);
					outVertices[*ioVertexCount + 3] = vertex;
					
					vertex.normal[0] = 1.0f;
					vertex.normal[1] = 0.0f;
					vertex.normal[2] = 0.0f;
					vertex.position[0] = xtof(box->position.x + box->size.x);
					vertex.position[1] = xtof(box->position.y);
					vertex.position[2] = xtof(box->position.z + box->size.z);
					outVertices[*ioVertexCount + 4] = vertex;
					vertex.position[2] = xtof(box->position.z);
					outVertices[*ioVertexCount + 5] = vertex;
					vertex.position[1] = xtof(box->position.y + box->size.y);
					outVertices[*ioVertexCount + 6] = vertex;
					vertex.position[2] = xtof(box->position.z + box->size.z);
					outVertices[*ioVertexCount + 7] = vertex;
					
					vertex.normal[0] = 0.0f;
					vertex.normal[1] = -1.0f;
					vertex.normal[2] = 0.0f;
					vertex.position[0] = xtof(box->position.x);
					vertex.position[1] = xtof(box->position.y);
					vertex.position[2] = xtof(box->position.z);
					outVertices[*ioVertexCount + 8] = vertex;
					vertex.position[0] = xtof(box->position.x + box->size.x);
					outVertices[*ioVertexCount + 9] = vertex;
					vertex.position[2] = xtof(box->position.z + box->size.z);
					outVertices[*ioVertexCount + 10] = vertex;
					vertex.position[0] = xtof(box->position.x);
					outVertices[*ioVertexCount + 11] = vertex;
					
					vertex.normal[0] = 0.0f;
					vertex.normal[1] = 1.0f;
					vertex.normal[2] = 0.0f;
					vertex.position[0] = xtof(box->position.x);
					vertex.position[1] = xtof(box->position.y + box->size.y);
					vertex.position[2] = xtof(box->position.z + box->size.z);
					outVertices[*ioVertexCount + 12] = vertex;
					vertex.position[0] = xtof(box->position.x + box->size.x);
					outVertices[*ioVertexCount + 13] = vertex;
					vertex.position[2] = xtof(box->position.z);
					outVertices[*ioVertexCount + 14] = vertex;
					vertex.position[0] = xtof(box->position.x);
					outVertices[*ioVertexCount + 15] = vertex;
					
					vertex.normal[0] = 0.0f;
					vertex.normal[1] = 0.0f;
					vertex.normal[2] = -1.0f;
					vertex.position[0] = xtof(box->position.x + box->size.x);
					vertex.position[1] = xtof(box->position.y);
					vertex.position[2] = xtof(box->position.z);
					outVertices[*ioVertexCount + 16] = vertex;
					vertex.position[0] = xtof(box->position.x);
					outVertices[*ioVertexCount + 17] = vertex;
					vertex.position[1] = xtof(box->position.y + box->size.y);
					outVertices[*ioVertexCount + 18] = vertex;
					vertex.position[0] = xtof(box->position.x + box->size.x);
					outVertices[*ioVertexCount + 19] = vertex;
					
					vertex.normal[0] = 0.0f;
					vertex.normal[1] = 0.0f;
					vertex.normal[2] = 1.0f;
					vertex.position[0] = xtof(box->position.x);
					vertex.position[1] = xtof(box->position.y);
					vertex.position[2] = xtof(box->position.z + box->size.z);
					outVertices[*ioVertexCount + 20] = vertex;
					vertex.position[0] = xtof(box->position.x + box->size.x);
					outVertices[*ioVertexCount + 21] = vertex;
					vertex.position[1] = xtof(box->position.y + box->size.y);
					outVertices[*ioVertexCount + 22] = vertex;
					vertex.position[0] = xtof(box->position.x);
					outVertices[*ioVertexCount + 23] = vertex;
				}
				if (outIndexes != NULL) {
					unsigned int faceIndex;
					
					for (faceIndex = 0; faceIndex < 6; faceIndex++) {
						outIndexes[*ioIndexCount + faceIndex * 6 + 0] = *ioVertexCount + faceIndex * 4 + 0;
						outIndexes[*ioIndexCount + faceIndex * 6 + 1] = *ioVertexCount + faceIndex * 4 + 1;
						outIndexes[*ioIndexCount + faceIndex * 6 + 2] = *ioVertexCount + faceIndex * 4 + 2;
						outIndexes[*ioIndexCount + faceIndex * 6 + 3] = *ioVertexCount + faceIndex * 4 + 2;
						outIndexes[*ioIndexCount + faceIndex * 6 + 4] = *ioVertexCount + faceIndex * 4 + 3;
						outIndexes[*ioIndexCount + faceIndex * 6 + 5] = *ioVertexCount + faceIndex * 4 + 0;
					}
				}
				*ioVertexCount += 24;
				*ioIndexCount += 36;
				
				break;
			}
			case COLLISION_SHAPE_SPHERE: {
				CollisionSphere * sphere = (CollisionSphere *) object;
				
				getSphereVertices(Vector3x_toVector3f(sphere->position), xtof(sphere->radius), COLOR4f(0.0f, 1.0f, 1.0f, 1.0f), outVertices, outIndexes, ioVertexCount, ioIndexCount);
				break;
			}
		}
	}
}

static bool draw(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen3D * self = context;
	static GLuint vertexBufferID, indexBufferID;
	struct vertex_p3f_n3f_c4f * vertices;
	unsigned int vertexCount;
	GLuint * indexes;
	unsigned int indexCount;
	Matrix4x4f matrix;
	
	if (vertexBufferID == 0) {
		glGenBuffersARB(1, &vertexBufferID);
		glGenBuffersARB(1, &indexBufferID);
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	vertexCount = indexCount = 0;
	getCollisionObjectVertices(self, NULL, NULL, &vertexCount, &indexCount);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(struct vertex_p3f_n3f_c4f) * vertexCount, NULL, GL_STREAM_DRAW);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, NULL, GL_STREAM_DRAW);
	vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	indexes = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertexCount = indexCount = 0;
	getCollisionObjectVertices(self, vertices, indexes, &vertexCount, &indexCount);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	
	matrix = Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, 70.0f, g_viewRatio, 0.5f, 800.0f);
	Matrix4x4f_translate(&matrix, 0.0f, 0.0f, -xtof(self->cameraDistance));
	Matrix4x4f_multiply(&matrix, Quaternionf_toMatrix(Quaternionx_toQuaternionf(self->cameraDirection)));
	Matrix4x4f_translate(&matrix, -xtof(self->cameraFocus.x), -xtof(self->cameraFocus.y), -xtof(self->cameraFocus.z));
	glLoadMatrixf(matrix.m);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	GLSLShader_activate(self->lightShader);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "light0"), 0.0f, 5.0f, 10.0f);
	glUniform1f(GLSLShader_getUniformLocation(self->lightShader, "ambient"), 0.1f);
	
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex_p3f_n3f_c4f), (void *) offsetof(struct vertex_p3f_n3f_c4f, position));
	glNormalPointer(GL_FLOAT, sizeof(struct vertex_p3f_n3f_c4f), (void *) offsetof(struct vertex_p3f_n3f_c4f, normal));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_p3f_n3f_c4f), (void *) offsetof(struct vertex_p3f_n3f_c4f, color));
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	
	GLSLShader_deactivate(self->lightShader);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	return true;
}

static bool keyDown(Atom eventID, void * eventData, void * context) {
	return true;
}

static bool mouseDown(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen3D * self = context;
	
	if (g_controlKeyDown) {
		self->dragMode = DRAG_CAMERA_DISTANCE;
	} else if (g_shiftKeyDown) {
		self->dragMode = DRAG_CAMERA_FOCUS;
	} else {
		self->dragMode = DRAG_CAMERA_DIRECTION;
	}
	Shell_setMouseDeltaMode(true);
	return true;
}

static bool mouseUp(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen3D * self = context;
	
	self->dragMode = DRAG_NONE;
	Shell_setMouseDeltaMode(false);
	return true;
}

#define CAMERA_LOOK_SENSITIVITY 0x00100
#define CAMERA_ZOOM_SENSITIVITY 0x00060
#define CAMERA_DRAG_SENSITIVITY 0x00060

static bool mouseDragged(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen3D * self = context;
	struct mouseEvent * event = eventData;
	
	switch (self->dragMode) {
		case DRAG_NONE:
			break;
			
		case DRAG_OBJECT_POSITION:
		case DRAG_OBJECT_LAST_POSITION:
		case DRAG_OBJECT_BOTH_POSITIONS:
		case DRAG_OBJECT_SIZE:
		case DRAG_OBJECT_LAST_SIZE:
		case DRAG_OBJECT_BOTH_SIZES:
			break;
			
		case DRAG_CAMERA_DIRECTION:
			Quaternionx_rotate(&self->cameraDirection, VECTOR3x_UP, xmul(ftox(event->position.x), CAMERA_LOOK_SENSITIVITY));
			Quaternionx_rotate(&self->cameraDirection, Quaternionx_multiplyVector3x(Quaternionx_inverted(self->cameraDirection), VECTOR3x_RIGHT), xmul(ftox(event->position.y), CAMERA_LOOK_SENSITIVITY));
			Shell_redisplay();
			break;
			
		case DRAG_CAMERA_FOCUS: {
			Vector3x offset;
			
			offset.x = xmul(ftox(event->position.x), xmul(CAMERA_DRAG_SENSITIVITY, self->cameraDistance));
			offset.y = -xmul(ftox(event->position.y), xmul(CAMERA_DRAG_SENSITIVITY, self->cameraDistance));
			offset.z = 0x00000;
			offset = Quaternionx_multiplyVector3x(Quaternionx_inverted(self->cameraDirection), offset);
			self->cameraFocus = Vector3x_subtract(self->cameraFocus, offset);
			Shell_redisplay();
			break;
		}
		case DRAG_CAMERA_DISTANCE:
			self->cameraDistance += xmul(ftox(event->position.y), xmul(CAMERA_ZOOM_SENSITIVITY, self->cameraDistance));
			if (self->cameraDistance < 0x10000) {
				self->cameraDistance = 0x10000;
			}
			Shell_redisplay();
			break;
	}
	return true;
}

static bool resized(Atom eventID, void * eventData, void * context) {
	Shell_redisplay();
	return true;
}

void SingleFrameScreen3D_activate(SingleFrameScreen3D * self) {
	self->resolver = CollisionResolver_create(self->intersectionManager, false);
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionBox3D_create(NULL, NULL, VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), 0x00000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionBox3D_create(NULL, NULL, VECTOR3x(0x20000, -0x40000, 0x00000), VECTOR3x(0x50000, 0x20000, 0x30000), 0x00000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionSphere_create(NULL, NULL, VECTOR3x(-0x30000, 0x00000, 0x00000), 0x10000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionSphere_create(NULL, NULL, VECTOR3x(-0x20000, 0x50000, 0x00000), 0x08000));
	
	self->dragMode = DRAG_NONE;
	self->selectedObjectIndex = 0;
	self->cameraFocus = VECTOR3x_ZERO;
	self->cameraDirection = QUATERNIONx_IDENTITY;
	self->cameraDistance = 0xF0000;
	
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), mouseDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), mouseUp, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), mouseDragged, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	Shell_redisplay();
}

void SingleFrameScreen3D_deactivate(SingleFrameScreen3D * self) {
	size_t objectIndex;
	
	for (objectIndex = 0; objectIndex < self->resolver->objectCount; objectIndex++) {
		self->resolver->objects[objectIndex]->dispose(self->resolver->objects[objectIndex]);
	}
	CollisionResolver_dispose(self->resolver);
	
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), mouseDown, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), mouseUp, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), mouseDragged, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_unregisterForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
	
	glDisable(GL_DEPTH_TEST);
}
