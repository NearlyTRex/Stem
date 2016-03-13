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
#include "collision/CollisionCapsule.h"
#include "collision/CollisionShared.h"
#include "collision/CollisionSphere.h"
#include "collision/CollisionStaticTrimesh.h"
#include "gamemath/Box6f.h"
#include "gamemath/Matrix4x4f.h"
#include "gamemath/MouseCoordinateTransforms.h"
#include "gamemath/Quaternionf.h"
#include "gamemath/VectorConversions.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include "testharness/SingleFrameScreen3D.h"
#include "testharness/SharedEvents.h"
#include "testharness/TestHarness_globals.h"
#include "utilities/printfFormats.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SUPERCLASS Screen

#define SPHERE_SUBDIVISIONS 16
#define SPHERE_FACE_VERTEX_COUNT ((SPHERE_SUBDIVISIONS + 2) * (SPHERE_SUBDIVISIONS + 2))
#define SPHERE_VERTEX_COUNT (SPHERE_FACE_VERTEX_COUNT * 6)
#define SPHERE_FACE_INDEX_COUNT (6 * (SPHERE_SUBDIVISIONS + 1) * (SPHERE_SUBDIVISIONS + 1))
#define SPHERE_INDEX_COUNT (SPHERE_FACE_INDEX_COUNT * 6)

static struct vertex_p3f_n3f sphereVertexTemplate[SPHERE_VERTEX_COUNT];
static GLuint sphereIndexTemplate[SPHERE_INDEX_COUNT];

// Improvements:
// - Show keyboard and mouse controls
// - Show normals

static void __attribute__((constructor)) initSphereTemplate() {
	unsigned int columnIndex, rowIndex, faceIndex, indexIndex;
	struct vertex_p3f_n3f vertex;
	Vector3f vector;
	Matrix4x4f faceMatrices[5] = {
		MATRIX4x4f(0.0f, 0.0f, -1.0f, 0.0f,
		           0.0f, 1.0f,  0.0f, 0.0f,
		           1.0f, 0.0f,  0.0f, 0.0f,
		           0.0f, 0.0f,  0.0f, 1.0f), // Left
		MATRIX4x4f(-1.0f, 0.0f,  0.0f, 0.0f,
		           0.0f,  1.0f,  0.0f, 0.0f,
		           0.0f,  0.0f, -1.0f, 0.0f,
		           0.0f,  0.0f,  0.0f, 1.0f), // Back
		MATRIX4x4f(0.0f, 0.0f, 1.0f, 0.0f,
		           0.0f, 1.0f, 0.0f, 0.0f,
		          -1.0f, 0.0f, 0.0f, 0.0f,
		           0.0f, 0.0f, 0.0f, 1.0f), // Right
		MATRIX4x4f(1.0f, 0.0f,  0.0f, 0.0f,
		           0.0f, 0.0f, -1.0f, 0.0f,
		           0.0f, 1.0f,  0.0f, 0.0f,
		           0.0f, 0.0f,  0.0f, 1.0f), // Bottom
		MATRIX4x4f(1.0f,  0.0f, 0.0f, 0.0f,
		           0.0f,  0.0f, 1.0f, 0.0f,
		           0.0f, -1.0f, 0.0f, 0.0f,
		           0.0f,  0.0f, 0.0f, 1.0f) // Top
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
	self->resourceManager = resourceManager;
	self->lightShader = GLSLShader_createWithFiles("litsurface.vert", "litsurface.frag", NULL);
	self->font = ResourceManager_referenceResource(self->resourceManager, "bitmap_font", "verdana_font.json");
	return true;
}

void SingleFrameScreen3D_dispose(SingleFrameScreen3D * self) {
	IntersectionManager_dispose(self->intersectionManager);
	GLSLShader_dispose(self->lightShader);
	ResourceManager_releaseResource(self->resourceManager, "bitmap_font", "verdana_font.json");
	call_super(dispose, self);
}

static void updateCollisions(SingleFrameScreen3D * self) {
	unsigned int collisionIndex;
	
	for (collisionIndex = 0; collisionIndex < OBJECT_COUNT_3D; collisionIndex++) {
		if (!CollisionResolver_querySingle(self->resolver, self->resolver->objects[collisionIndex], &self->collisions[collisionIndex])) {
			self->collisions[collisionIndex].time = -1;
		}
	}
}

#define COLOR_BOX_LAST_POSITION                COLOR4f(0.5f, 0.25f, 0.0f, 1.0f)
#define COLOR_BOX_LAST_POSITION_HIGHLIGHT      COLOR4f(0.75f, 0.625f, 0.125f, 1.0f)
#define COLOR_BOX_POSITION                     COLOR4f(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BOX_POSITION_HIGHLIGHT           COLOR4f(1.0f, 1.0f, 0.875f, 1.0f)
#define COLOR_BOX_POSITION_COLLIDING           COLOR4f(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_BOX_POSITION_COLLIDING_HIGHLIGHT COLOR4f(1.0f, 0.75f, 0.75f, 1.0f)

#define COLOR_SPHERE_LAST_POSITION                COLOR4f(0.0f, 0.25f, 0.5f, 1.0f)
#define COLOR_SPHERE_LAST_POSITION_HIGHLIGHT      COLOR4f(0.125f, 0.625f, 0.75f, 1.0f)
#define COLOR_SPHERE_POSITION                     COLOR4f(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_SPHERE_POSITION_HIGHLIGHT           COLOR4f(0.875f, 1.0f, 1.0f, 1.0f)
#define COLOR_SPHERE_POSITION_COLLIDING           COLOR4f(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_SPHERE_POSITION_COLLIDING_HIGHLIGHT COLOR4f(1.0f, 0.75f, 0.75f, 1.0f)

#define COLOR_CAPSULE_LAST_POSITION                COLOR4f(0.25f, 0.0f, 0.5f, 1.0f)
#define COLOR_CAPSULE_LAST_POSITION_HIGHLIGHT      COLOR4f(0.5f, 0.25f, 0.75f, 1.0f)
#define COLOR_CAPSULE_POSITION                     COLOR4f(0.5f, 0.0f, 1.0f, 1.0f)
#define COLOR_CAPSULE_POSITION_HIGHLIGHT           COLOR4f(0.75f, 0.5f, 1.0f, 1.0f)
#define COLOR_CAPSULE_POSITION_COLLIDING           COLOR4f(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_CAPSULE_POSITION_COLLIDING_HIGHLIGHT COLOR4f(1.0f, 0.75f, 0.75f, 1.0f)

#define COLOR_TRIMESH COLOR4f(0.0f, 1.0f, 0.0f, 1.0f)

#define SWEEP_ALPHA 0.5f

static void setVertexColor(struct vertex_p3f_n3f_c4f * vertex, Color4f color) {
	vertex->color[0] = color.red;
	vertex->color[1] = color.green;
	vertex->color[2] = color.blue;
	vertex->color[3] = color.alpha;
}

static void getBoxVertices(Vector3f position, Vector3f size, Color4f color, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	if (outVertices != NULL) {
		struct vertex_p3f_n3f_c4f vertex;
		
		setVertexColor(&vertex, color);
		vertex.normal[0] = -1.0f;
		vertex.normal[1] = 0.0f;
		vertex.normal[2] = 0.0f;
		vertex.position[0] = position.x;
		vertex.position[1] = position.y;
		vertex.position[2] = position.z;
		outVertices[*ioVertexCount + 0] = vertex;
		vertex.position[2] = position.z + size.z;
		outVertices[*ioVertexCount + 1] = vertex;
		vertex.position[1] = position.y + size.y;
		outVertices[*ioVertexCount + 2] = vertex;
		vertex.position[2] = position.z;
		outVertices[*ioVertexCount + 3] = vertex;
		
		vertex.normal[0] = 1.0f;
		vertex.normal[1] = 0.0f;
		vertex.normal[2] = 0.0f;
		vertex.position[0] = position.x + size.x;
		vertex.position[1] = position.y;
		vertex.position[2] = position.z + size.z;
		outVertices[*ioVertexCount + 4] = vertex;
		vertex.position[2] = position.z;
		outVertices[*ioVertexCount + 5] = vertex;
		vertex.position[1] = position.y + size.y;
		outVertices[*ioVertexCount + 6] = vertex;
		vertex.position[2] = position.z + size.z;
		outVertices[*ioVertexCount + 7] = vertex;
		
		vertex.normal[0] = 0.0f;
		vertex.normal[1] = -1.0f;
		vertex.normal[2] = 0.0f;
		vertex.position[0] = position.x;
		vertex.position[1] = position.y;
		vertex.position[2] = position.z;
		outVertices[*ioVertexCount + 8] = vertex;
		vertex.position[0] = position.x + size.x;
		outVertices[*ioVertexCount + 9] = vertex;
		vertex.position[2] = position.z + size.z;
		outVertices[*ioVertexCount + 10] = vertex;
		vertex.position[0] = position.x;
		outVertices[*ioVertexCount + 11] = vertex;
		
		vertex.normal[0] = 0.0f;
		vertex.normal[1] = 1.0f;
		vertex.normal[2] = 0.0f;
		vertex.position[0] = position.x;
		vertex.position[1] = position.y + size.y;
		vertex.position[2] = position.z + size.z;
		outVertices[*ioVertexCount + 12] = vertex;
		vertex.position[0] = position.x + size.x;
		outVertices[*ioVertexCount + 13] = vertex;
		vertex.position[2] = position.z;
		outVertices[*ioVertexCount + 14] = vertex;
		vertex.position[0] = position.x;
		outVertices[*ioVertexCount + 15] = vertex;
		
		vertex.normal[0] = 0.0f;
		vertex.normal[1] = 0.0f;
		vertex.normal[2] = -1.0f;
		vertex.position[0] = position.x + size.x;
		vertex.position[1] = position.y;
		vertex.position[2] = position.z;
		outVertices[*ioVertexCount + 16] = vertex;
		vertex.position[0] = position.x;
		outVertices[*ioVertexCount + 17] = vertex;
		vertex.position[1] = position.y + size.y;
		outVertices[*ioVertexCount + 18] = vertex;
		vertex.position[0] = position.x + size.x;
		outVertices[*ioVertexCount + 19] = vertex;
		
		vertex.normal[0] = 0.0f;
		vertex.normal[1] = 0.0f;
		vertex.normal[2] = 1.0f;
		vertex.position[0] = position.x;
		vertex.position[1] = position.y;
		vertex.position[2] = position.z + size.z;
		outVertices[*ioVertexCount + 20] = vertex;
		vertex.position[0] = position.x + size.x;
		outVertices[*ioVertexCount + 21] = vertex;
		vertex.position[1] = position.y + size.y;
		outVertices[*ioVertexCount + 22] = vertex;
		vertex.position[0] = position.x;
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
}

static void getSphereVertices(Vector3f position, float radius, Color4f color, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	if (outVertices != NULL) {
		unsigned int vertexIndex;
		struct vertex_p3f_n3f_c4f vertex;
		Vector3f vertexPosition;
		
		setVertexColor(&vertex, color);
		for (vertexIndex = 0; vertexIndex < SPHERE_VERTEX_COUNT; vertexIndex++) {
			vertexPosition.x = sphereVertexTemplate[vertexIndex].position[0];
			vertexPosition.y = sphereVertexTemplate[vertexIndex].position[1];
			vertexPosition.z = sphereVertexTemplate[vertexIndex].position[2];
			vertexPosition = Vector3f_multiplyScalar(vertexPosition, radius);
			vertex.position[0] = vertexPosition.x + position.x;
			vertex.position[1] = vertexPosition.y + position.y;
			vertex.position[2] = vertexPosition.z + position.z;
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

static void getCapsuleVertices(Vector3f position, float radius, float cylinderHeight, Color4f color, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	if (outVertices != NULL) {
		unsigned int vertexIndex;
		struct vertex_p3f_n3f_c4f vertex;
		Vector3f vertexPosition;
		
		setVertexColor(&vertex, color);
		for (vertexIndex = 0; vertexIndex < SPHERE_VERTEX_COUNT; vertexIndex++) {
			vertexPosition.x = sphereVertexTemplate[vertexIndex].position[0];
			vertexPosition.y = sphereVertexTemplate[vertexIndex].position[1];
			vertexPosition.z = sphereVertexTemplate[vertexIndex].position[2];
			vertexPosition = Vector3f_multiplyScalar(vertexPosition, radius);
			if (vertexPosition.y > 0.0f) {
				vertexPosition.y += cylinderHeight;
			}
			vertexPosition.y += radius;
			vertex.position[0] = vertexPosition.x + position.x;
			vertex.position[1] = vertexPosition.y + position.y;
			vertex.position[2] = vertexPosition.z + position.z;
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

static void getTrimeshVertices(CollisionStaticTrimesh * trimesh, Color4f color, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int triangleIndex, vertexIndex;
	
	if (outVertices != NULL) {
		struct vertex_p3f_n3f_c4f vertex;
		
		setVertexColor(&vertex, color);
		for (triangleIndex = 0; triangleIndex < trimesh->triangleCount; triangleIndex++) {
			vertex.normal[0] = trimesh->triangles[triangleIndex].normal.x;
			vertex.normal[1] = trimesh->triangles[triangleIndex].normal.y;
			vertex.normal[2] = trimesh->triangles[triangleIndex].normal.z;
			for (vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
				vertex.position[0] = xtof(trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[vertexIndex]].position.x);
				vertex.position[1] = xtof(trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[vertexIndex]].position.y);
				vertex.position[2] = xtof(trimesh->vertices[trimesh->triangles[triangleIndex].vertexIndexes[vertexIndex]].position.z);
				outVertices[*ioVertexCount + triangleIndex * 3 + vertexIndex] = vertex;
			}
		}
	}
	
	if (outIndexes != NULL) {
		for (triangleIndex = 0; triangleIndex < trimesh->triangleCount; triangleIndex++) {
			for (vertexIndex = 0; vertexIndex < 3; vertexIndex++) {
				outIndexes[*ioIndexCount + triangleIndex * 3 + vertexIndex] = *ioVertexCount + triangleIndex * 3 + vertexIndex;
			}
		}
	}
	
	*ioVertexCount += trimesh->triangleCount * 3;
	*ioIndexCount += trimesh->triangleCount * 3;
}

static void getOpaqueCollisionObjectVertices(SingleFrameScreen3D * self, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	size_t objectIndex;
	CollisionObject * object;
	Color4f color;
	
	for (objectIndex = 0; objectIndex < self->resolver->objectCount; objectIndex++) {
		object = self->resolver->objects[objectIndex];
		switch (object->shapeType) {
			case COLLISION_SHAPE_BOX_3D: {
				CollisionBox3D * box = (CollisionBox3D *) object;
				
				if (objectIndex == self->selectedObjectIndex) {
					color = COLOR_BOX_LAST_POSITION_HIGHLIGHT;
				} else {
					color = COLOR_BOX_LAST_POSITION;
				}
				getBoxVertices(Vector3x_toVector3f(box->lastPosition), Vector3x_toVector3f(box->lastSize), color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				
				if (self->collisions[objectIndex].time > -1) {
					Vector3x collidingPosition = Vector3x_interpolate(box->lastPosition, box->position, self->collisions[objectIndex].time);
					Vector3x collidingSize = Vector3x_interpolate(box->lastSize, box->size, self->collisions[objectIndex].time);
					
					if (objectIndex == self->selectedObjectIndex) {
						color = COLOR_BOX_POSITION_HIGHLIGHT;
					} else {
						color = COLOR_BOX_POSITION;
					}
					getBoxVertices(Vector3x_toVector3f(collidingPosition), Vector3x_toVector3f(collidingSize), color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				}
				
				if (objectIndex == self->selectedObjectIndex) {
					if (self->collisions[objectIndex].time > -1) {
						color = COLOR_BOX_POSITION_COLLIDING_HIGHLIGHT;
					} else {
						color = COLOR_BOX_POSITION_HIGHLIGHT;
					}
				} else if (self->collisions[objectIndex].time > -1) {
					color = COLOR_BOX_POSITION_COLLIDING;
				} else {
					color = COLOR_BOX_POSITION;
				}
				getBoxVertices(Vector3x_toVector3f(box->position), Vector3x_toVector3f(box->size), color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				break;
			}
			case COLLISION_SHAPE_SPHERE: {
				CollisionSphere * sphere = (CollisionSphere *) object;
				
				if (objectIndex == self->selectedObjectIndex) {
					color = COLOR_SPHERE_LAST_POSITION_HIGHLIGHT;
				} else {
					color = COLOR_SPHERE_LAST_POSITION;
				}
				getSphereVertices(Vector3x_toVector3f(sphere->lastPosition), xtof(sphere->radius), color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				
				if (self->collisions[objectIndex].time > -1) {
					Vector3x collidingPosition = Vector3x_interpolate(sphere->lastPosition, sphere->position, self->collisions[objectIndex].time);
					
					if (objectIndex == self->selectedObjectIndex) {
						color = COLOR_SPHERE_POSITION_HIGHLIGHT;
					} else {
						color = COLOR_SPHERE_POSITION;
					}
					getSphereVertices(Vector3x_toVector3f(collidingPosition), xtof(sphere->radius), color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				}
				
				if (objectIndex == self->selectedObjectIndex) {
					if (self->collisions[objectIndex].time > -1) {
						color = COLOR_SPHERE_POSITION_COLLIDING_HIGHLIGHT;
					} else {
						color = COLOR_SPHERE_POSITION_HIGHLIGHT;
					}
				} else if (self->collisions[objectIndex].time > -1) {
					color = COLOR_SPHERE_POSITION_COLLIDING;
				} else {
					color = COLOR_SPHERE_POSITION;
				}
				getSphereVertices(Vector3x_toVector3f(sphere->position), xtof(sphere->radius), color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				break;
			}
			case COLLISION_SHAPE_CAPSULE: {
				CollisionCapsule * capsule = (CollisionCapsule *) object;
				
				if (objectIndex == self->selectedObjectIndex) {
					color = COLOR_CAPSULE_LAST_POSITION_HIGHLIGHT;
				} else {
					color = COLOR_CAPSULE_LAST_POSITION;
				}
				getCapsuleVertices(Vector3x_toVector3f(capsule->lastPosition), xtof(capsule->radius), xtof(capsule->cylinderHeight), color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				
				if (self->collisions[objectIndex].time > -1) {
					Vector3x collidingPosition = Vector3x_interpolate(capsule->lastPosition, capsule->position, self->collisions[objectIndex].time);
					
					if (objectIndex == self->selectedObjectIndex) {
						color = COLOR_CAPSULE_POSITION_HIGHLIGHT;
					} else {
						color = COLOR_CAPSULE_POSITION;
					}
					getCapsuleVertices(Vector3x_toVector3f(collidingPosition), xtof(capsule->radius), xtof(capsule->cylinderHeight), color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				}
				
				if (objectIndex == self->selectedObjectIndex) {
					if (self->collisions[objectIndex].time > -1) {
						color = COLOR_CAPSULE_POSITION_COLLIDING_HIGHLIGHT;
					} else {
						color = COLOR_CAPSULE_POSITION_HIGHLIGHT;
					}
				} else if (self->collisions[objectIndex].time > -1) {
					color = COLOR_CAPSULE_POSITION_COLLIDING;
				} else {
					color = COLOR_CAPSULE_POSITION;
				}
				getCapsuleVertices(Vector3x_toVector3f(capsule->position), xtof(capsule->radius), xtof(capsule->cylinderHeight), color, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				break;
			}
			case COLLISION_SHAPE_STATIC_TRIMESH:
				getTrimeshVertices((CollisionStaticTrimesh *) object, COLOR_TRIMESH, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				break;
		}
	}
}

static void getBoxSweepQuadVertices(Vector3f vertex0, Vector3f vertex1, Vector3f vertex2, Vector3f vertex3, Color4f colorStart, Color4f colorEnd, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	if (outVertices != NULL) {
		struct vertex_p3f_n3f_c4f vertex;
		Vector3f normal;
		
		setVertexColor(&vertex, colorStart);
		normal = Vector3f_normalized(Vector3f_cross(Vector3f_subtract(vertex1, vertex0), Vector3f_subtract(vertex3, vertex0)));
		vertex.normal[0] = normal.x;
		vertex.normal[1] = normal.y;
		vertex.normal[2] = normal.z;
		vertex.position[0] = vertex0.x;
		vertex.position[1] = vertex0.y;
		vertex.position[2] = vertex0.z;
		outVertices[*ioVertexCount + 0] = vertex;
		
		vertex.position[0] = vertex1.x;
		vertex.position[1] = vertex1.y;
		vertex.position[2] = vertex1.z;
		outVertices[*ioVertexCount + 1] = vertex;
		
		setVertexColor(&vertex, colorEnd);
		vertex.position[0] = vertex2.x;
		vertex.position[1] = vertex2.y;
		vertex.position[2] = vertex2.z;
		outVertices[*ioVertexCount + 2] = vertex;
		
		vertex.position[0] = vertex3.x;
		vertex.position[1] = vertex3.y;
		vertex.position[2] = vertex3.z;
		outVertices[*ioVertexCount + 3] = vertex;
	}
	if (outIndexes != NULL) {
		outIndexes[*ioIndexCount + 0] = *ioVertexCount + 0;
		outIndexes[*ioIndexCount + 1] = *ioVertexCount + 1;
		outIndexes[*ioIndexCount + 2] = *ioVertexCount + 2;
		outIndexes[*ioIndexCount + 3] = *ioVertexCount + 2;
		outIndexes[*ioIndexCount + 4] = *ioVertexCount + 3;
		outIndexes[*ioIndexCount + 5] = *ioVertexCount + 0;
	}
	*ioVertexCount += 4;
	*ioIndexCount += 6;
}

static void getBoxSweepVertices(Vector3f lastPosition, Vector3f lastSize, Vector3f position, Vector3f size, Color4f colorStart, Color4f colorEnd, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	Box6f lastBounds, bounds;
	
	if (lastPosition.x == position.x && lastPosition.y == position.y && lastPosition.z == position.z &&
	    lastSize.x == size.x && lastSize.y == size.y && lastSize.z == size.z) {
		return;
	}
	
	lastBounds = Box6f_fromPositionAndSize(lastPosition, lastSize);
	bounds = Box6f_fromPositionAndSize(position, size);
	
	if (bounds.left <= lastBounds.left) {
		if (bounds.bottom >= lastBounds.bottom) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.front),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.top <= lastBounds.top) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.front),
			                        VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.back),
			                        VECTOR3f(bounds.left, bounds.top, bounds.back),
			                        VECTOR3f(bounds.left, bounds.top, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	} else if (bounds.left >= lastBounds.left) {
		if (bounds.top >= lastBounds.top) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.back),
			                        VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.front),
			                        VECTOR3f(bounds.left, bounds.top, bounds.front),
			                        VECTOR3f(bounds.left, bounds.top, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.bottom <= lastBounds.bottom) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.back),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	}
	if (bounds.right >= lastBounds.right) {
		if (bounds.bottom >= lastBounds.bottom) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.back),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.top <= lastBounds.top) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.back),
			                        VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.front),
			                        VECTOR3f(bounds.right, bounds.top, bounds.front),
			                        VECTOR3f(bounds.right, bounds.top, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	} else if (bounds.right <= lastBounds.right) {
		if (bounds.top >= lastBounds.top) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.front),
			                        VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.back),
			                        VECTOR3f(bounds.right, bounds.top, bounds.back),
			                        VECTOR3f(bounds.right, bounds.top, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.bottom <= lastBounds.bottom) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.front),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	}
	
	if (bounds.back <= lastBounds.back) {
		if (bounds.bottom >= lastBounds.bottom) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.back),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.top <= lastBounds.top) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.back),
			                        VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.back),
			                        VECTOR3f(bounds.right, bounds.top, bounds.back),
			                        VECTOR3f(bounds.left, bounds.top, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	} else if (bounds.back >= lastBounds.back) {
		if (bounds.top >= lastBounds.top) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.back),
			                        VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.back),
			                        VECTOR3f(bounds.left, bounds.top, bounds.back),
			                        VECTOR3f(bounds.right, bounds.top, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.bottom <= lastBounds.bottom) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.back),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	}
	if (bounds.front >= lastBounds.front) {
		if (bounds.bottom >= lastBounds.bottom) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.front),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.top <= lastBounds.top) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.front),
			                        VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.front),
			                        VECTOR3f(bounds.left, bounds.top, bounds.front),
			                        VECTOR3f(bounds.right, bounds.top, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	} else if (bounds.front <= lastBounds.front) {
		if (bounds.top >= lastBounds.top) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.front),
			                        VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.front),
			                        VECTOR3f(bounds.right, bounds.top, bounds.front),
			                        VECTOR3f(bounds.left, bounds.top, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.bottom <= lastBounds.bottom) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.front),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	}
	
	if (bounds.back <= lastBounds.back) {
		if (bounds.left >= lastBounds.left) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.back),
			                        VECTOR3f(bounds.left, bounds.top, bounds.back),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.right <= lastBounds.right) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.back),
			                        VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.back),
			                        VECTOR3f(bounds.right, bounds.top, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	} else if (bounds.back >= lastBounds.back) {
		if (bounds.right >= lastBounds.right) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.back),
			                        VECTOR3f(bounds.right, bounds.top, bounds.back),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.left <= lastBounds.left) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.back),
			                        VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.back),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.back),
			                        VECTOR3f(bounds.left, bounds.top, bounds.back),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	}
	if (bounds.front >= lastBounds.front) {
		if (bounds.left >= lastBounds.left) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.front),
			                        VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.front),
			                        VECTOR3f(bounds.left, bounds.top, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.right <= lastBounds.right) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.front),
			                        VECTOR3f(bounds.right, bounds.top, bounds.front),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	} else if (bounds.front <= lastBounds.front) {
		if (bounds.right >= lastBounds.right) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.right, lastBounds.top, lastBounds.front),
			                        VECTOR3f(lastBounds.right, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(bounds.right, bounds.bottom, bounds.front),
			                        VECTOR3f(bounds.right, bounds.top, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		} else if (bounds.left <= lastBounds.left) {
			getBoxSweepQuadVertices(VECTOR3f(lastBounds.left, lastBounds.bottom, lastBounds.front),
			                        VECTOR3f(lastBounds.left, lastBounds.top, lastBounds.front),
			                        VECTOR3f(bounds.left, bounds.top, bounds.front),
			                        VECTOR3f(bounds.left, bounds.bottom, bounds.front),
			                        colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
		}
	}
}

#define SWEEP_CYLINDER_SUBDIVISIONS 32

static void getSphereSweepVertices(Vector3f lastPosition, Vector3f position, float radius, Color4f colorStart, Color4f colorEnd, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	if (lastPosition.x == position.x && lastPosition.y == position.y && lastPosition.z == position.z) {
		return;
	}
	
	if (outVertices != NULL) {
		Vector3f direction, radialPosition;
		float length, dot;
		Quaternionf orientation;
		unsigned int vertexIndex;
		struct vertex_p3f_n3f_c4f vertex;
		
		direction = Vector3f_subtract(position, lastPosition);
		length = Vector3f_magnitude(direction);
		direction = Vector3f_divideScalar(direction, length);
		dot = Vector3f_dot(direction, VECTOR3f_FRONT);
		if (dot > 0.9999f) {
			orientation = QUATERNIONf_IDENTITY;
			
		} else if (dot < -0.9999f) {
			orientation = Quaternionf_fromAxisAngle(VECTOR3f_UP, M_PI);
			
		} else {
			Vector3f axis = Vector3f_cross(direction, VECTOR3f_FRONT);
			Vector3f_normalize(&axis);
			orientation = Quaternionf_fromAxisAngle(axis, -acos(dot));
		}
		
		radius /= cos(0.5f * M_PI * 2 / SWEEP_CYLINDER_SUBDIVISIONS);
		
		for (vertexIndex = 0; vertexIndex < SWEEP_CYLINDER_SUBDIVISIONS; vertexIndex++) {
			radialPosition = VECTOR3f(cos(vertexIndex * M_PI * 2 / SWEEP_CYLINDER_SUBDIVISIONS), sin(vertexIndex * M_PI * 2 / SWEEP_CYLINDER_SUBDIVISIONS), 0.0f);
			radialPosition = Quaternionf_multiplyVector3f(orientation, radialPosition);
			vertex.normal[0] = radialPosition.x;
			vertex.normal[1] = radialPosition.y;
			vertex.normal[2] = radialPosition.z;
			vertex.position[0] = radialPosition.x * radius + lastPosition.x;
			vertex.position[1] = radialPosition.y * radius + lastPosition.y;
			vertex.position[2] = radialPosition.z * radius + lastPosition.z;
			setVertexColor(&vertex, colorStart);
			outVertices[*ioVertexCount + vertexIndex] = vertex;
			
			vertex.position[0] = radialPosition.x * radius + position.x;
			vertex.position[1] = radialPosition.y * radius + position.y;
			vertex.position[2] = radialPosition.z * radius + position.z;
			setVertexColor(&vertex, colorEnd);
			outVertices[*ioVertexCount + SWEEP_CYLINDER_SUBDIVISIONS + vertexIndex] = vertex;
		}
	}
	
	if (outIndexes != NULL) {
		unsigned int vertexIndex;
		
		for (vertexIndex = 0; vertexIndex < SWEEP_CYLINDER_SUBDIVISIONS; vertexIndex++) {
			outIndexes[*ioIndexCount + vertexIndex * 6 + 0] = *ioVertexCount + vertexIndex;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 1] = *ioVertexCount + (vertexIndex + 1) % SWEEP_CYLINDER_SUBDIVISIONS;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 2] = *ioVertexCount + (vertexIndex + 1) % SWEEP_CYLINDER_SUBDIVISIONS + SWEEP_CYLINDER_SUBDIVISIONS;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 3] = *ioVertexCount + (vertexIndex + 1) % SWEEP_CYLINDER_SUBDIVISIONS + SWEEP_CYLINDER_SUBDIVISIONS;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 4] = *ioVertexCount + vertexIndex + SWEEP_CYLINDER_SUBDIVISIONS;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 5] = *ioVertexCount + vertexIndex;
		}
	}
	*ioVertexCount += SWEEP_CYLINDER_SUBDIVISIONS * 2;
	*ioIndexCount += SWEEP_CYLINDER_SUBDIVISIONS * 6;
}

static void getCapsuleSweepVertices(Vector3f lastPosition, Vector3f position, float radius, float cylinderHeight, Color4f colorStart, Color4f colorEnd, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	if (lastPosition.x == position.x && lastPosition.y == position.y && lastPosition.z == position.z) {
		return;
	}
	
	if (outVertices != NULL) {
		Vector3f direction, radialPosition;
		float length, dot;
		Quaternionf orientation;
		unsigned int vertexIndex;
		struct vertex_p3f_n3f_c4f vertex;
		float yOffset;
		
		direction = Vector3f_subtract(position, lastPosition);
		length = Vector3f_magnitude(direction);
		direction = Vector3f_divideScalar(direction, length);
		dot = Vector3f_dot(direction, VECTOR3f_FRONT);
		if (dot > 0.9999f) {
			orientation = QUATERNIONf_IDENTITY;
			
		} else if (dot < -0.9999f) {
			orientation = Quaternionf_fromAxisAngle(VECTOR3f_UP, M_PI);
			
		} else {
			Vector3f axis = Vector3f_cross(direction, VECTOR3f_FRONT);
			Vector3f_normalize(&axis);
			orientation = Quaternionf_fromAxisAngle(axis, -acos(dot));
		}
		
		radius /= cos(0.5f * M_PI * 2 / SWEEP_CYLINDER_SUBDIVISIONS);
		
		for (vertexIndex = 0; vertexIndex < SWEEP_CYLINDER_SUBDIVISIONS; vertexIndex++) {
			radialPosition = VECTOR3f(cos(vertexIndex * M_PI * 2 / SWEEP_CYLINDER_SUBDIVISIONS), sin(vertexIndex * M_PI * 2 / SWEEP_CYLINDER_SUBDIVISIONS), 0.0f);
			radialPosition = Quaternionf_multiplyVector3f(orientation, radialPosition);
			vertex.normal[0] = radialPosition.x;
			vertex.normal[1] = radialPosition.y;
			vertex.normal[2] = radialPosition.z;
			yOffset = radius;
			if (radialPosition.y > 0.0f) {
				yOffset += cylinderHeight;
			}
			vertex.position[0] = radialPosition.x * radius + lastPosition.x;
			vertex.position[1] = radialPosition.y * radius + lastPosition.y + yOffset;
			vertex.position[2] = radialPosition.z * radius + lastPosition.z;
			setVertexColor(&vertex, colorStart);
			outVertices[*ioVertexCount + vertexIndex] = vertex;
			
			vertex.position[0] = radialPosition.x * radius + position.x;
			vertex.position[1] = radialPosition.y * radius + position.y + yOffset;
			vertex.position[2] = radialPosition.z * radius + position.z;
			setVertexColor(&vertex, colorEnd);
			outVertices[*ioVertexCount + SWEEP_CYLINDER_SUBDIVISIONS + vertexIndex] = vertex;
		}
	}
	
	if (outIndexes != NULL) {
		unsigned int vertexIndex;
		
		for (vertexIndex = 0; vertexIndex < SWEEP_CYLINDER_SUBDIVISIONS; vertexIndex++) {
			outIndexes[*ioIndexCount + vertexIndex * 6 + 0] = *ioVertexCount + vertexIndex;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 1] = *ioVertexCount + (vertexIndex + 1) % SWEEP_CYLINDER_SUBDIVISIONS;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 2] = *ioVertexCount + (vertexIndex + 1) % SWEEP_CYLINDER_SUBDIVISIONS + SWEEP_CYLINDER_SUBDIVISIONS;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 3] = *ioVertexCount + (vertexIndex + 1) % SWEEP_CYLINDER_SUBDIVISIONS + SWEEP_CYLINDER_SUBDIVISIONS;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 4] = *ioVertexCount + vertexIndex + SWEEP_CYLINDER_SUBDIVISIONS;
			outIndexes[*ioIndexCount + vertexIndex * 6 + 5] = *ioVertexCount + vertexIndex;
		}
	}
	*ioVertexCount += SWEEP_CYLINDER_SUBDIVISIONS * 2;
	*ioIndexCount += SWEEP_CYLINDER_SUBDIVISIONS * 6;
}

static void getTranslucentCollisionObjectVertices(SingleFrameScreen3D * self, struct vertex_p3f_n3f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	size_t objectIndex;
	CollisionObject * object;
	Color4f colorStart, colorEnd;
	
	if (!self->drawSweeps) {
		return;
	}
	for (objectIndex = 0; objectIndex < self->resolver->objectCount; objectIndex++) {
		object = self->resolver->objects[objectIndex];
		switch (object->shapeType) {
			case COLLISION_SHAPE_BOX_3D: {
				CollisionBox3D * box = (CollisionBox3D *) object;
				
				if (objectIndex == self->selectedObjectIndex) {
					colorStart = COLOR_BOX_LAST_POSITION_HIGHLIGHT;
					if (self->collisions[objectIndex].time > -1) {
						colorEnd = COLOR_BOX_POSITION_COLLIDING_HIGHLIGHT;
					} else {
						colorEnd = COLOR_BOX_POSITION_HIGHLIGHT;
					}
					
				} else {
					colorStart = COLOR_BOX_LAST_POSITION;
					if (self->collisions[objectIndex].time > -1) {
						colorEnd = COLOR_BOX_POSITION_COLLIDING;
					} else {
						colorEnd = COLOR_BOX_POSITION;
					}
				}
				colorStart.alpha = SWEEP_ALPHA;
				colorEnd.alpha = SWEEP_ALPHA;
				getBoxSweepVertices(Vector3x_toVector3f(box->lastPosition), Vector3x_toVector3f(box->lastSize), Vector3x_toVector3f(box->position), Vector3x_toVector3f(box->size), colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				break;
			}
			case COLLISION_SHAPE_SPHERE: {
				CollisionSphere * sphere = (CollisionSphere *) object;
				
				if (objectIndex == self->selectedObjectIndex) {
					colorStart = COLOR_SPHERE_LAST_POSITION_HIGHLIGHT;
					if (self->collisions[objectIndex].time > -1) {
						colorEnd = COLOR_SPHERE_POSITION_COLLIDING_HIGHLIGHT;
					} else {
						colorEnd = COLOR_SPHERE_POSITION_HIGHLIGHT;
					}
				} else {
					colorStart = COLOR_SPHERE_LAST_POSITION;
					if (self->collisions[objectIndex].time > -1) {
						colorEnd = COLOR_SPHERE_POSITION_COLLIDING;
					} else {
						colorEnd = COLOR_SPHERE_POSITION;
					}
				}
				colorStart.alpha = SWEEP_ALPHA;
				colorEnd.alpha = SWEEP_ALPHA;
				getSphereSweepVertices(Vector3x_toVector3f(sphere->lastPosition), Vector3x_toVector3f(sphere->position), xtof(sphere->radius), colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				break;
			}
			case COLLISION_SHAPE_CAPSULE: {
				CollisionCapsule * capsule = (CollisionCapsule *) object;
				
				if (objectIndex == self->selectedObjectIndex) {
					colorStart = COLOR_CAPSULE_LAST_POSITION_HIGHLIGHT;
					if (self->collisions[objectIndex].time > -1) {
						colorEnd = COLOR_CAPSULE_POSITION_COLLIDING_HIGHLIGHT;
					} else {
						colorEnd = COLOR_CAPSULE_POSITION_HIGHLIGHT;
					}
				} else {
					colorStart = COLOR_CAPSULE_LAST_POSITION;
					if (self->collisions[objectIndex].time > -1) {
						colorEnd = COLOR_CAPSULE_POSITION_COLLIDING;
					} else {
						colorEnd = COLOR_CAPSULE_POSITION;
					}
				}
				colorStart.alpha = SWEEP_ALPHA;
				colorEnd.alpha = SWEEP_ALPHA;
				getCapsuleSweepVertices(Vector3x_toVector3f(capsule->lastPosition), Vector3x_toVector3f(capsule->position), xtof(capsule->radius), xtof(capsule->cylinderHeight), colorStart, colorEnd, outVertices, outIndexes, ioVertexCount, ioIndexCount);
				break;
			}
			case COLLISION_SHAPE_STATIC_TRIMESH:
				break;
		}
	}
}

static void getTextVertices(SingleFrameScreen3D * self, struct vertex_p2f_t2f_c4f * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
}

static bool draw(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen3D * self = context;
	static GLuint vertexBufferID, indexBufferID;
	struct vertex_p3f_n3f_c4f * vertices;
	struct vertex_p2f_t2f_c4f * textVertices;
	unsigned int vertexCount;
	GLuint * indexes;
	unsigned int indexCount, indexCountOpaque;
	Matrix4x4f matrix;
	Vector3f cameraPosition;
	
	if (vertexBufferID == 0) {
		glGenBuffersARB(1, &vertexBufferID);
		glGenBuffersARB(1, &indexBufferID);
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	
#pragma mark 3D geometry
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	vertexCount = indexCount = 0;
	getOpaqueCollisionObjectVertices(self, NULL, NULL, &vertexCount, &indexCount);
	indexCountOpaque = indexCount;
	getTranslucentCollisionObjectVertices(self, NULL, NULL, &vertexCount, &indexCount);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(struct vertex_p3f_n3f_c4f) * vertexCount, NULL, GL_STREAM_DRAW);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, NULL, GL_STREAM_DRAW);
	vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	indexes = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertexCount = indexCount = 0;
	getOpaqueCollisionObjectVertices(self, vertices, indexes, &vertexCount, &indexCount);
	getTranslucentCollisionObjectVertices(self, vertices, indexes, &vertexCount, &indexCount);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	
	matrix = Matrix4x4f_perspective(MATRIX4x4f_IDENTITY, 70.0f, g_viewRatio, 0.5f, 800.0f);
	Matrix4x4f_translate(&matrix, 0.0f, 0.0f, -xtof(self->cameraDistance));
	Matrix4x4f_multiply(&matrix, Quaternionf_toMatrix(Quaternionx_toQuaternionf(self->cameraDirection)));
	Matrix4x4f_translate(&matrix, xtof(self->cameraFocus.x), xtof(self->cameraFocus.y), xtof(self->cameraFocus.z));
	glLoadMatrixf(matrix.m);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	GLSLShader_activate(self->lightShader);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "light0Position"), 0.0f, 8.0f, 8.0f);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "light0Color"), 1.0f, 1.0f, 0.95f);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "light1Position"), -1.0f, -2.0f, -8.0f);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "light1Color"), 0.8f, 0.8f, 0.8f);
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "ambientColor"), 0.1f, 0.1f, 0.105f);
	glUniform1f(GLSLShader_getUniformLocation(self->lightShader, "specularIntensity"), 0.875f);
	glUniform1f(GLSLShader_getUniformLocation(self->lightShader, "shininess"), 32.0f);
	cameraPosition = Vector3f_add(Quaternionf_multiplyVector3f(Quaternionx_toQuaternionf(self->cameraDirection), VECTOR3f(0.0f, 0.0f, -xtof(self->cameraDistance))), Vector3x_toVector3f(self->cameraFocus));
	glUniform3f(GLSLShader_getUniformLocation(self->lightShader, "cameraPosition"), cameraPosition.x, cameraPosition.y, -cameraPosition.z);
	
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex_p3f_n3f_c4f), (void *) offsetof(struct vertex_p3f_n3f_c4f, position));
	glNormalPointer(GL_FLOAT, sizeof(struct vertex_p3f_n3f_c4f), (void *) offsetof(struct vertex_p3f_n3f_c4f, normal));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_p3f_n3f_c4f), (void *) offsetof(struct vertex_p3f_n3f_c4f, color));
	glDrawElements(GL_TRIANGLES, indexCountOpaque, GL_UNSIGNED_INT, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void *) (ptrdiff_t) ((indexCount - indexCountOpaque) * sizeof(GLuint)));
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	
	GLSLShader_deactivate(self->lightShader);
	glDisableClientState(GL_NORMAL_ARRAY);
	
#pragma mark Text
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	matrix = Matrix4x4f_ortho(MATRIX4x4f_IDENTITY, -12.0f * g_viewRatio, 12.0f * g_viewRatio, -12.0f, 12.0f, -1.0f, 1.0f);
	glLoadMatrixf(matrix.m);
	
	vertexCount = indexCount = 0;
	getTextVertices(self, NULL, NULL, &vertexCount, &indexCount);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(struct vertex_p2f_t2f_c4f) * vertexCount, NULL, GL_STREAM_DRAW);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, NULL, GL_STREAM_DRAW);
	textVertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	indexes = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	vertexCount = indexCount = 0;
	getTextVertices(self, textVertices, indexes, &vertexCount, &indexCount);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glVertexPointer(2, GL_FLOAT, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, position));
	glColorPointer(4, GL_FLOAT, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, color));
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, texCoords));
	GLTexture_activate(self->font->atlas->texture);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	GLTexture_deactivate(self->font->atlas->texture);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_CULL_FACE);
	
	return true;
}

static bool keyDown(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen3D * self = context;
	struct keyEvent * event = eventData;
	
	switch (event->keyCode) {
		case KEYBOARD_TAB: {
			size_t selectedObjectIndex = self->selectedObjectIndex;
			
			do {
				if (event->modifiers & MODIFIER_SHIFT_BIT) {
					selectedObjectIndex += self->resolver->objectCount - 1;
				} else {
					selectedObjectIndex++;
				}
				selectedObjectIndex %= self->resolver->objectCount;
			} while (self->resolver->objects[selectedObjectIndex]->shapeType == COLLISION_SHAPE_STATIC_TRIMESH && selectedObjectIndex != self->selectedObjectIndex);
			
			self->selectedObjectIndex = selectedObjectIndex;
			Shell_redisplay();
			break;
		}
		case KEYBOARD_I:
		case KEYBOARD_J:
		case KEYBOARD_K:
		case KEYBOARD_L:
		case KEYBOARD_U:
		case KEYBOARD_M:
			if (self->resolver->objects[self->selectedObjectIndex]->shapeType == COLLISION_SHAPE_BOX_3D) {
				CollisionBox3D * box = (CollisionBox3D *) self->resolver->objects[self->selectedObjectIndex];
				CollisionBox3D_setSolidity(box,
				                           event->keyCode == KEYBOARD_J ? !box->solidLeft   : box->solidLeft,
				                           event->keyCode == KEYBOARD_L ? !box->solidRight  : box->solidRight,
				                           event->keyCode == KEYBOARD_K ? !box->solidBottom : box->solidBottom,
				                           event->keyCode == KEYBOARD_I ? !box->solidTop    : box->solidTop,
				                           event->keyCode == KEYBOARD_U ? !box->solidBack   : box->solidBack,
				                           event->keyCode == KEYBOARD_M ? !box->solidFront  : box->solidFront);
				Shell_redisplay();
			}
			break;
			
		case KEYBOARD_S:
			self->drawSweeps = !self->drawSweeps;
			Shell_redisplay();
			break;
			
		case KEYBOARD_W:
			self->wireframe = !self->wireframe;
			glPolygonMode(GL_FRONT, self->wireframe ? GL_LINE : GL_FILL);
			Shell_redisplay();
			break;
			
		case KEYBOARD_P: {
			if (self->collisions[self->selectedObjectIndex].time == -1) {
				printf("No collision\n");
			} else {
				printf("Collision at 0x%05X; normal = {0x%05X, 0x%05X, 0x%05X}\n", self->collisions[self->selectedObjectIndex].time, self->collisions[self->selectedObjectIndex].normal.x, self->collisions[self->selectedObjectIndex].normal.y, self->collisions[self->selectedObjectIndex].normal.z);
			}
			CollisionObject * object = self->resolver->objects[self->selectedObjectIndex];
			switch (object->shapeType) {
				case COLLISION_SHAPE_BOX_3D: {
					CollisionBox3D * box = (CollisionBox3D *) object;
					printf("  lastPosition: {0x%05X, 0x%05X, 0x%05X}\n", box->lastPosition.x, box->lastPosition.y, box->lastPosition.z);
					printf("  lastSize: {0x%05X, 0x%05X, 0x%05X}\n", box->lastSize.x, box->lastSize.y, box->lastSize.z);
					printf("  position: {0x%05X, 0x%05X, 0x%05X}\n", box->position.x, box->position.y, box->position.z);
					printf("  size: {0x%05X, 0x%05X, 0x%05X}\n", box->size.x, box->size.y, box->size.z);
					break;
				}
				case COLLISION_SHAPE_SPHERE: {
					CollisionSphere * sphere = (CollisionSphere *) object;
					printf("  lastPosition: {0x%05X, 0x%05X, 0x%05X}\n", sphere->lastPosition.x, sphere->lastPosition.y, sphere->lastPosition.z);
					printf("  position: {0x%05X, 0x%05X, 0x%05X}\n", sphere->position.x, sphere->position.y, sphere->position.z);
					printf("  radius: 0x%05X\n", sphere->radius);
					break;
				}
				case COLLISION_SHAPE_CAPSULE: {
					CollisionCapsule * capsule = (CollisionCapsule *) object;
					printf("  lastPosition: {0x%05X, 0x%05X, 0x%05X}\n", capsule->lastPosition.x, capsule->lastPosition.y, capsule->lastPosition.z);
					printf("  position: {0x%05X, 0x%05X, 0x%05X}\n", capsule->position.x, capsule->position.y, capsule->position.z);
					printf("  radius: 0x%05X\n", capsule->radius);
					printf("  cylinderHeight: 0x%05X\n", capsule->cylinderHeight);
					break;
				}
			}
			break;
		}
		case KEYBOARD_D: {
			CollisionCapsule * capsule = (CollisionCapsule *) self->resolver->objects[4];
			capsule->lastPosition = VECTOR3x(0x04185, 0x03851, 0x0EF4C);
			capsule->position = VECTOR3x(0xFFFFFF80, 0x1C842, 0xFFFFF985);
			capsule->radius = 0x01DB3;
			capsule->cylinderHeight = 0x03F25;
			updateCollisions(self);
			Shell_redisplay();
			break;
		}
	}
	return true;
}

#define DOUBLE_CLICK_INTERVAL 0.25
#define DOUBLE_CLICK_MAX_DISTANCE 4.0f

static bool mouseDown(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen3D * self = context;
	struct mouseEvent * event = eventData;
	static double lastClickTime;
	static Vector2f lastClickPosition;
	double clickTime = Shell_getCurrentTime();
	
	if (clickTime - lastClickTime < DOUBLE_CLICK_INTERVAL && fabs(event->position.x - lastClickPosition.x) <= DOUBLE_CLICK_MAX_DISTANCE && fabs(event->position.y - lastClickPosition.y) <= DOUBLE_CLICK_MAX_DISTANCE) {
		CollisionObject * object = self->resolver->objects[self->selectedObjectIndex];
		switch (object->shapeType) {
			case COLLISION_SHAPE_BOX_3D: {
				CollisionBox3D * box = (CollisionBox3D *) object;
				box->position = box->lastPosition;
				box->size = box->lastSize;
				break;
			}
			case COLLISION_SHAPE_SPHERE: {
				CollisionSphere * sphere = (CollisionSphere *) object;
				sphere->position = sphere->lastPosition;
				break;
			}
			case COLLISION_SHAPE_CAPSULE: {
				CollisionCapsule * capsule = (CollisionCapsule *) object;
				capsule->position = capsule->lastPosition;
				break;
			}
		}
		Shell_redisplay();
		
	} else {
		self->draggingCamera = g_altKeyDown;
		Shell_setMouseDeltaMode(true);
	}
	Shell_redisplay();
	
	lastClickTime = clickTime;
	lastClickPosition = VECTOR2f(event->position.x, event->position.y);
	
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
#define OBJECT_DRAG_SENSITIVITY 0x00060

static bool mouseDragged(Atom eventID, void * eventData, void * context) {
	SingleFrameScreen3D * self = context;
	struct mouseEvent * event = eventData;
	Vector3x offset;
	CollisionObject * object = self->resolver->objects[self->selectedObjectIndex];
	
	if (self->draggingCamera) {
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
		
	} else {
		if (g_controlKeyDown) {
			offset.x = 0x00000;
			offset.y = 0x00000;
			offset.z = xmul(ftox(event->position.y), xmul(OBJECT_DRAG_SENSITIVITY, self->cameraDistance));
		} else {
			offset.x = xmul(ftox(event->position.x), xmul(OBJECT_DRAG_SENSITIVITY, self->cameraDistance));
			offset.y = -xmul(ftox(event->position.y), xmul(OBJECT_DRAG_SENSITIVITY, self->cameraDistance));
			offset.z = 0x00000;
		}
		offset = Quaternionx_multiplyVector3x(Quaternionx_inverted(self->cameraDirection), offset);
		switch (object->shapeType) {
			case COLLISION_SHAPE_BOX_3D: {
				CollisionBox3D * box = (CollisionBox3D *) object;
				if (g_altKeyDown) {
					if (g_shiftKeyDown) {
						box->lastSize = Vector3x_add(box->lastSize, offset);
					} else {
						box->lastPosition = Vector3x_add(box->lastPosition, offset);
					}
				} else if (g_shiftKeyDown) {
					box->size = Vector3x_add(box->size, offset);
					if (g_spacebarDown) {
						box->lastSize = Vector3x_add(box->lastSize, offset);
					}
				} else {
					box->position = Vector3x_add(box->position, offset);
					if (g_spacebarDown) {
						box->lastPosition = Vector3x_add(box->lastPosition, offset);
					}
				}
				break;
			}
			case COLLISION_SHAPE_SPHERE: {
				CollisionSphere * sphere = (CollisionSphere *) object;
				if (g_shiftKeyDown) {
					sphere->radius += offset.y;
				} else if (g_altKeyDown) {
					sphere->lastPosition = Vector3x_add(sphere->lastPosition, offset);
				} else {
					sphere->position = Vector3x_add(sphere->position, offset);
					if (g_spacebarDown) {
						sphere->lastPosition = Vector3x_add(sphere->lastPosition, offset);
					}
				}
				break;
			}
			case COLLISION_SHAPE_CAPSULE: {
				CollisionCapsule * capsule = (CollisionCapsule *) object;
				if (g_shiftKeyDown) {
					if (g_altKeyDown) {
						capsule->cylinderHeight += offset.y;
						if (capsule->cylinderHeight < 0x00000) {
							capsule->cylinderHeight = 0x00000;
						}
					} else {
						capsule->radius += offset.y;
					}
				} else if (g_altKeyDown) {
					capsule->lastPosition = Vector3x_add(capsule->lastPosition, offset);
				} else {
					capsule->position = Vector3x_add(capsule->position, offset);
					if (g_spacebarDown) {
						capsule->lastPosition = Vector3x_add(capsule->lastPosition, offset);
					}
				}
				break;
			}
		}
		updateCollisions(self);
		Shell_redisplay();
	}
	
	return true;
}

static bool resized(Atom eventID, void * eventData, void * context) {
	Shell_redisplay();
	return true;
}

void SingleFrameScreen3D_activate(SingleFrameScreen3D * self) {
	Vector3x vertices[] = {
		{-0x20000, -0x20000, -0x60000},
		{ 0x20000, -0x20000, -0x20000},
		{ 0x20000,  0x20000, -0x60000},
		{ 0x20000, -0x20000, -0x60000}
	};
	unsigned int indexes[] = {
		0, 1, 2,
		0, 3, 1,
		0, 2, 3,
		1, 3, 2
	};
	/*
	Vector3x vertices2[] = {{-0x80000, -0x60000, 0x00000}, {-0x60000, -0x60000, 0x00000}, {-0x80000, -0x60000, 0x20000},
	                        {-0x80000, -0x60000, 0x00000}, {-0x80000, -0x60000, 0x20000}, {-0x80000, -0x40000, 0x00000},
	                        {-0x80000, -0x60000, 0x00000}, {-0x80000, -0x40000, 0x00000}, {-0x60000, -0x60000, 0x00000},
	                        {-0x80000, -0x40000, 0x00000}, {-0x80000, -0x60000, 0x20000}, {-0x60000, -0x60000, 0x00000}};
	*/
	Vector3x vertices2[] = {{0x01000, 0x01000, 0x01000}, {0x20000, 0x00000, 0x00000}, {0x00000, 0x00000, 0x20000},
	                        {0x01000, 0x01000, 0x01000}, {0x00000, 0x00000, 0x20000}, {0x00000, 0x20000, 0x00000},
	                        {0x01000, 0x01000, 0x01000}, {0x00000, 0x20000, 0x00000}, {0x20000, 0x00000, 0x00000},
	                        {0x00000, 0x20000, 0x00000}, {0x00000, 0x00000, 0x20000}, {0x20000, 0x00000, 0x00000}};
	
	self->resolver = CollisionResolver_create(self->intersectionManager, false);
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionBox3D_create(NULL, NULL, VECTOR3x(0x30000, 0x10000, 0x00000), VECTOR3x(0x10000, 0x10000, 0x10000), 0x00000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionBox3D_create(NULL, NULL, VECTOR3x(0x20000, -0x40000, 0x00000), VECTOR3x(0x50000, 0x20000, 0x30000), 0x00000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionSphere_create(NULL, NULL, VECTOR3x(-0x30000, 0x00000, 0x00000), 0x10000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionSphere_create(NULL, NULL, VECTOR3x(-0x20000, 0x50000, 0x00000), 0x08000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionCapsule_create(NULL, NULL, VECTOR3x(0x00000, -0x50000, 0x20000), 0x08000, 0x14000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionCapsule_create(NULL, NULL, VECTOR3x(0x20000, 0x60000, 0x20000), 0x0C000, 0x10000));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionStaticTrimesh_createWithIndexes(NULL, NULL, vertices, indexes, sizeof(indexes) / sizeof(unsigned int)));
	CollisionResolver_addObject(self->resolver, (CollisionObject *) CollisionStaticTrimesh_create(NULL, NULL, vertices2, 12));
	if (self->resolver->objectCount > OBJECT_COUNT_3D) {
		fprintf(stderr, "OBJECT_COUNT_3D needs to be updated (currently %u; " SIZE_T_FORMAT " objects added)\n", OBJECT_COUNT_3D, self->resolver->objectCount);
		abort();
	}
	
	self->selectedObjectIndex = 0;
	self->cameraFocus = VECTOR3x_ZERO;
	self->cameraDirection = QUATERNIONx_IDENTITY;
	self->cameraDistance = 0xF0000;
	self->drawSweeps = true;
	updateCollisions(self);
	
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_KEY_DOWN), keyDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DOWN), mouseDown, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_UP), mouseUp, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_MOUSE_DRAGGED), mouseDragged, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_RESIZED), resized, self);
	EventDispatcher_registerForEvent(self->screenManager->eventDispatcher, ATOM(EVENT_DRAW), draw, self);
	
	Shell_redisplay();
}

void SingleFrameScreen3D_deactivate(SingleFrameScreen3D * self) {
	size_t objectIndex;
	
	glPolygonMode(GL_FRONT, GL_FILL);
	
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
}
