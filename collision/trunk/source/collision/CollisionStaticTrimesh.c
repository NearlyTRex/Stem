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

#include "collision/CollisionStaticTrimesh.h"
#include "collision/CollisionShared.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS CollisionObject

static void sharedInit(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback) {
	call_super(init, self, owner, COLLISION_SHAPE_STATIC_TRIMESH, collisionCallback);
	self->dispose = CollisionStaticTrimesh_dispose;
	self->isStatic = CollisionStaticTrimesh_isStatic;
	self->getCollisionBounds = CollisionStaticTrimesh_getCollisionBounds;
}

#define FORWARD_DOT_MAX 0x0FFF0

static unsigned int computeEdgeInfo(CollisionStaticTrimesh * self, struct trimeshConvexEdge * edges) {
	unsigned int triangleIndex, triangleVertexIndex, edgeIndex = 0;
	Vector3x edgeParallel, edgePerpendicular, normal, connectedNormal;
	
	for (triangleIndex = 0; triangleIndex < self->triangleCount; triangleIndex++) {
		for (triangleVertexIndex = 0; triangleVertexIndex < 3; triangleVertexIndex++) {
			if (self->triangles[triangleIndex].connectedTriangleIndexes[triangleVertexIndex] > triangleIndex && self->triangles[triangleIndex].connectedTriangleIndexes[triangleVertexIndex] != UINT_MAX) {
				normal = self->triangles[triangleIndex].normal;
				connectedNormal = self->triangles[self->triangles[triangleIndex].connectedTriangleIndexes[triangleVertexIndex]].normal;
				edgeParallel = Vector3x_subtract(self->vertices[self->triangles[triangleIndex].vertexIndexes[triangleVertexIndex]].position, self->vertices[self->triangles[triangleIndex].vertexIndexes[(triangleVertexIndex + 1) % 3]].position);
				edgePerpendicular = Vector3x_cross(edgeParallel, normal);
				if (Vector3x_dot(edgePerpendicular, connectedNormal) < 0x00000) {
					if (edges != NULL) {
						Vector3x axis;
						fixed16_16 dot;
						
						self->edges[edgeIndex].vertexIndexes[0] = self->triangles[triangleIndex].vertexIndexes[triangleVertexIndex];
						self->edges[edgeIndex].vertexIndexes[1] = self->triangles[triangleIndex].vertexIndexes[(triangleVertexIndex + 1) % 3];
						self->edges[edgeIndex].normal = Vector3x_add(normal, connectedNormal);
						if (!Vector3x_normalize(&self->edges[edgeIndex].normal)) {
							self->edges[edgeIndex].normal = Vector3x_normalized(edgePerpendicular);
						}
						self->edges[edgeIndex].triangleNormals[0] = normal;
						self->edges[edgeIndex].triangleNormals[1] = connectedNormal;
						
						self->edges[edgeIndex].length = Vector3x_magnitude(edgeParallel);
						edgeParallel = Vector3x_divideScalar(edgeParallel, self->edges[edgeIndex].length);
						dot = Vector3x_dot(edgeParallel, VECTOR3x_FRONT);
						if (dot > FORWARD_DOT_MAX) {
							self->edges[edgeIndex].planarTransform = QUATERNIONx_IDENTITY;
							
						} else if (dot < -FORWARD_DOT_MAX) {
							self->edges[edgeIndex].planarTransform = Quaternionx_fromAxisAngle(VECTOR3x_UP, X_PI);
							
						} else {
							axis = Vector3x_cross(edgeParallel, VECTOR3x_FRONT);
							Vector3x_normalize(&axis);
							self->edges[edgeIndex].planarTransform = Quaternionx_fromAxisAngle(axis, xacos(dot));
						}
					}
					edgeIndex++;
				}
			}
		}
	}
	return edgeIndex;
}

static void computeGeometryInfo(CollisionStaticTrimesh * self) {
	unsigned int triangleIndex, triangleIndex2, vertexIndex, triangleVertexIndex, triangleVertexIndex2;
	Vector3x v01, v02, normal, v1, v2, trianglePerpendicular;
	
	for (triangleIndex = 0; triangleIndex < self->triangleCount; triangleIndex++) {
		v01 = Vector3x_subtract(self->vertices[self->triangles[triangleIndex].vertexIndexes[0]].position, self->vertices[self->triangles[triangleIndex].vertexIndexes[1]].position);
		v02 = Vector3x_subtract(self->vertices[self->triangles[triangleIndex].vertexIndexes[0]].position, self->vertices[self->triangles[triangleIndex].vertexIndexes[2]].position);
		self->triangles[triangleIndex].normal = Vector3x_normalized(Vector3x_cross(v01, v02));
		
		self->triangles[triangleIndex].connectedTriangleIndexes[0] = UINT_MAX;
		self->triangles[triangleIndex].connectedTriangleIndexes[1] = UINT_MAX;
		self->triangles[triangleIndex].connectedTriangleIndexes[2] = UINT_MAX;
	}
	
	for (triangleIndex = 0; triangleIndex < self->triangleCount; triangleIndex++) {
		for (triangleVertexIndex = 0; triangleVertexIndex < 3; triangleVertexIndex++) {
			if (self->triangles[triangleIndex].connectedTriangleIndexes[triangleVertexIndex] != UINT_MAX) {
				continue;
			}
			for (triangleIndex2 = triangleIndex + 1; triangleIndex2 < self->triangleCount; triangleIndex2++) {
				for (triangleVertexIndex2 = 0; triangleVertexIndex2 < 3; triangleVertexIndex2++) {
					if (self->triangles[triangleIndex].vertexIndexes[triangleVertexIndex] == self->triangles[triangleIndex2].vertexIndexes[triangleVertexIndex2] &&
					    self->triangles[triangleIndex].vertexIndexes[(triangleVertexIndex + 1) % 3] == self->triangles[triangleIndex2].vertexIndexes[(triangleVertexIndex2 + 2) % 3]) {
						self->triangles[triangleIndex].connectedTriangleIndexes[triangleVertexIndex] = triangleIndex2;
						self->triangles[triangleIndex2].connectedTriangleIndexes[(triangleVertexIndex2 + 2) % 3] = triangleIndex;
						break;
					}
				}
				if (self->triangles[triangleIndex].connectedTriangleIndexes[triangleVertexIndex] != UINT_MAX) {
					break;
				}
			}
		}
	}
	
	for (vertexIndex = 0; vertexIndex < self->vertexCount; vertexIndex++) {
		normal = VECTOR3x_ZERO;
		// TODO: Simple average of triangle normals is inaccurate for vertex normal. Triangle normals need to be weighted. (How?)
		for (triangleIndex = 0; triangleIndex < self->triangleCount; triangleIndex++) {
			for (triangleVertexIndex = 0; triangleVertexIndex < 3; triangleVertexIndex++) {
				if (self->triangles[triangleIndex].vertexIndexes[triangleVertexIndex] == vertexIndex) {
					normal = Vector3x_add(normal, self->triangles[triangleIndex].normal);
					break;
				}
			}
		}
		self->vertices[vertexIndex].normal = Vector3x_normalized(normal);
		
		self->vertices[vertexIndex].convex = false;
		for (triangleIndex = 0; triangleIndex < self->triangleCount; triangleIndex++) {
			for (triangleVertexIndex = 0; triangleVertexIndex < 3; triangleVertexIndex++) {
				if (self->triangles[triangleIndex].vertexIndexes[triangleVertexIndex] == vertexIndex) {
					v1 = self->vertices[self->triangles[triangleIndex].vertexIndexes[(triangleVertexIndex + 1) % 3]].position;
					v2 = self->vertices[self->triangles[triangleIndex].vertexIndexes[(triangleVertexIndex + 2) % 3]].position;
					trianglePerpendicular = Vector3x_subtract(self->vertices[vertexIndex].position, Vector3x_multiplyScalar(Vector3x_add(v1, v2), 0x08000));
					if (Vector3x_dot(self->vertices[vertexIndex].normal, trianglePerpendicular) > 0x00000) {
						self->vertices[vertexIndex].convex = true;
					}
				}
			}
		}
	}
	
	self->edgeCount = computeEdgeInfo(self, NULL);
	self->edges = malloc(sizeof(struct trimeshConvexEdge) * self->edgeCount);
	computeEdgeInfo(self, self->edges);
	
	self->bounds = BOX6x(0, 0, 0, 0, 0, 0);
	if (self->vertexCount > 0) {
		self->bounds.left = self->bounds.right = self->vertices[0].position.x;
		self->bounds.bottom = self->bounds.top = self->vertices[0].position.y;
		self->bounds.back = self->bounds.front = self->vertices[0].position.z;
	}
	for (vertexIndex = 1; vertexIndex < self->vertexCount; vertexIndex++) {
		if (self->vertices[vertexIndex].position.x < self->bounds.left) {
			self->bounds.left = self->vertices[vertexIndex].position.x;
		} else if (self->vertices[vertexIndex].position.x > self->bounds.right) {
			self->bounds.right = self->vertices[vertexIndex].position.x;
		}
		if (self->vertices[vertexIndex].position.y < self->bounds.bottom) {
			self->bounds.bottom = self->vertices[vertexIndex].position.y;
		} else if (self->vertices[vertexIndex].position.y > self->bounds.top) {
			self->bounds.top = self->vertices[vertexIndex].position.y;
		}
		if (self->vertices[vertexIndex].position.z < self->bounds.back) {
			self->bounds.back = self->vertices[vertexIndex].position.z;
		} else if (self->vertices[vertexIndex].position.z > self->bounds.front) {
			self->bounds.front = self->vertices[vertexIndex].position.z;
		}
	}
}

static unsigned int uniqVertices(Vector3x * vertices, unsigned int vertexCount, unsigned int * indexes, unsigned int indexCount) {
	unsigned int index, vertexIndex, vertexIndex2;
	
	for (vertexIndex = 1; vertexIndex < vertexCount; vertexIndex++) {
		for (vertexIndex2 = 0; vertexIndex2 < vertexIndex; vertexIndex2++) {
			if (vertices[vertexIndex2].x == vertices[vertexIndex].x &&
			    vertices[vertexIndex2].y == vertices[vertexIndex].y &&
			    vertices[vertexIndex2].z == vertices[vertexIndex].z) {
				
				for (index = 0; index < indexCount; index++) {
					if (indexes[index] == vertexIndex) {
						indexes[index] = vertexIndex2;
						
					} else if (indexes[index] > vertexIndex) {
						indexes[index]--;
					}
				}
				
				vertexCount--;
				for (vertexIndex2 = vertexIndex; vertexIndex2 < vertexCount; vertexIndex2++) {
					vertices[vertexIndex2] = vertices[vertexIndex2 + 1];
				}
				
				vertexIndex--;
				break;
			}
		}
	}
	return vertexCount;
}

static void initVertexAndTriangleData(CollisionStaticTrimesh * self, const Vector3x * vertices, unsigned int vertexCount, const unsigned int * indexes, unsigned int indexCount) {
	unsigned int vertexIndex, triangleIndex;
	
	self->vertexCount = vertexCount;
	self->vertices = malloc(sizeof(struct trimeshVertex) * vertexCount);
	for (vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
		self->vertices[vertexIndex].position = vertices[vertexIndex];
	}
	
	self->triangleCount = indexCount / 3;
	self->triangles = malloc(sizeof(struct trimeshTriangle) * self->triangleCount);
	for (triangleIndex = 0; triangleIndex < self->triangleCount; triangleIndex++) {
		self->triangles[triangleIndex].vertexIndexes[0] = indexes[triangleIndex * 3];
		self->triangles[triangleIndex].vertexIndexes[1] = indexes[triangleIndex * 3 + 1];
		self->triangles[triangleIndex].vertexIndexes[2] = indexes[triangleIndex * 3 + 2];
	}
}

CollisionStaticTrimesh * CollisionStaticTrimesh_create(void * owner, CollisionCallback collisionCallback, const Vector3x * vertices, unsigned int vertexCount) {
	stemobject_create_implementation(CollisionStaticTrimesh, init, owner, collisionCallback, vertices, vertexCount)
}

bool CollisionStaticTrimesh_init(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback, const Vector3x * vertices, unsigned int vertexCount) {
	unsigned int index, indexCount;
	unsigned int * indexes;
	Vector3x * verticesCopy;
	
	sharedInit(self, owner, collisionCallback);
	
	indexes = malloc(sizeof(unsigned int) * vertexCount);
	for (index = 0; index < vertexCount; index++) {
		indexes[index] = index;
	}
	
	indexCount = vertexCount;
	verticesCopy = malloc(sizeof(Vector3x) * vertexCount);
	memcpy(verticesCopy, vertices, sizeof(Vector3x) * vertexCount);
	vertexCount = uniqVertices(verticesCopy, vertexCount, indexes, indexCount);
	initVertexAndTriangleData(self, verticesCopy, vertexCount, indexes, indexCount);
	free(verticesCopy);
	computeGeometryInfo(self);
	
	return true;
}

CollisionStaticTrimesh * CollisionStaticTrimesh_createWithIndexes(void * owner, CollisionCallback collisionCallback, const Vector3x * vertices, const unsigned int * indexes, unsigned int indexCount) {
	stemobject_create_implementation(CollisionStaticTrimesh, initWithIndexes, owner, collisionCallback, vertices, indexes, indexCount)
}

bool CollisionStaticTrimesh_initWithIndexes(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback, const Vector3x * vertices, const unsigned int * indexes, unsigned int indexCount) {
	unsigned int vertexCount, index;
	unsigned int * indexesCopy;
	Vector3x * verticesCopy;
	
	sharedInit(self, owner, collisionCallback);
	vertexCount = 0;
	for (index = 0; index < indexCount; index++) {
		if (indexes[index] + 1 > vertexCount) {
			vertexCount = indexes[index] + 1;
		}
	}
	
	verticesCopy = malloc(sizeof(Vector3x) * vertexCount);
	memcpy(verticesCopy, vertices, sizeof(Vector3x) * vertexCount);
	indexesCopy = malloc(sizeof(unsigned int) * indexCount);
	memcpy(indexesCopy, indexes, sizeof(unsigned int) * indexCount);
	vertexCount = uniqVertices(verticesCopy, vertexCount, indexesCopy, indexCount);
	initVertexAndTriangleData(self, verticesCopy, vertexCount, indexesCopy, indexCount);
	free(verticesCopy);
	free(indexesCopy);
	computeGeometryInfo(self);
	return true;
}

void CollisionStaticTrimesh_dispose(CollisionStaticTrimesh * self) {
	free(self->vertices);
	free(self->edges);
	free(self->triangles);
	call_super(dispose, self);
}

bool CollisionStaticTrimesh_isStatic(CollisionStaticTrimesh * self) {
	return true;
}

Box6x CollisionStaticTrimesh_getCollisionBounds(CollisionStaticTrimesh * self) {
	return self->bounds;
}
