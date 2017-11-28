/*
  Copyright (c) 2017 Alex Diener
  
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
#include "glgraphics/Armature.h"
#include "glgraphics/MeshRenderable.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

Armature * Armature_create(unsigned int boneCount, struct ArmatureBone * bones) {
	stemobject_create_implementation(Armature, init, boneCount, bones)
}

bool Armature_init(Armature * self, unsigned int boneCount, struct ArmatureBone * bones) {
	call_super(init, self);
	self->dispose = Armature_dispose;
	self->boneCount = boneCount;
	self->bones = malloc(sizeof(*self->bones) * boneCount);
	memcpy(self->bones, bones, sizeof(*self->bones) * boneCount);
	
#ifndef NDEBUG
	unsigned int boneIndex;
	
	for (boneIndex = 0; boneIndex < boneCount; boneIndex++) {
		if (bones[boneIndex].parentIndex != BONE_INDEX_NOT_FOUND) {
			assert(bones[boneIndex].parentIndex < boneIndex);
		}
	}
#endif
	
	return true;
}

void Armature_dispose(Armature * self) {
	free(self->bones);
	call_super(dispose, self);
}

unsigned int Armature_boneIndexForID(Armature * self, Atom boneID) {
	unsigned int boneIndex;
	
	for (boneIndex = 0; boneIndex < self->boneCount; boneIndex++) {
		if (self->bones[boneIndex].boneID == boneID) {
			return boneIndex;
		}
	}
	return BONE_INDEX_NOT_FOUND;
}

MeshRenderable * Armature_createDebugMesh(Armature * self) {
	struct vertex_p3f_t2f_n3f_c4f_b4u_w4f boneVertices[] = {
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {-0.7071067811865476f, -0.7071067811865476f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-0.1f, 0.1f, 0.1f}, {0.0f, 0.0f}, {-0.7071067811865476f, -0.7071067811865476f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, {-0.7071067811865476f, -0.7071067811865476f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -0.7071067811865476f, -0.7071067811865476f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, {0.0f, -0.7071067811865476f, -0.7071067811865476f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, {0.0f, -0.7071067811865476f, -0.7071067811865476f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.7071067811865476f, -0.7071067811865476f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, {0.7071067811865476f, -0.7071067811865476f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{0.1f, 0.1f, 0.1f}, {0.0f, 0.0f}, {0.7071067811865476f, -0.7071067811865476f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -0.7071067811865476f, 0.7071067811865476f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{0.1f, 0.1f, 0.1f}, {0.0f, 0.0f}, {0.0f, -0.7071067811865476f, 0.7071067811865476f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-0.1f, 0.1f, 0.1f}, {0.0f, 0.0f}, {0.0f, -0.7071067811865476f, 0.7071067811865476f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {-0.9938837346736188f, -0.11043152607484653f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, {-0.9938837346736188f, -0.11043152607484653f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-0.1f, 0.1f, 0.1f}, {0.0f, 0.0f}, {-0.9938837346736188f, -0.11043152607484653f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -0.11043152607484653f, -0.9938837346736188f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, {0.0f, -0.11043152607484653f, -0.9938837346736188f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, {0.0f, -0.11043152607484653f, -0.9938837346736188f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.9938837346736188f, -0.11043152607484653f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{0.1f, 0.1f, 0.1f}, {0.0f, 0.0f}, {0.9938837346736188f, -0.11043152607484653f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, {0.9938837346736188f, -0.11043152607484653f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		
		{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -0.11043152607484653f, 0.9938837346736188f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{-0.1f, 0.1f, 0.1f}, {0.0f, 0.0f}, {0.0f, -0.11043152607484653f, 0.9938837346736188f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}},
		{{0.1f, 0.1f, 0.1f}, {0.0f, 0.0f}, {0.0f, -0.11043152607484653f, 0.9938837346736188f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f}}
	};
	GLuint boneIndexes[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23
	};
	struct vertex_p3f_t2f_n3f_c4f_b4u_w4f * vertices;
	GLuint * indexes;
	unsigned int boneIndex, vertexIndex, indexIndex;
	struct vertex_p3f_t2f_n3f_c4f_b4u_w4f vertex;
	MeshRenderable * mesh;
	static Material * boneMaterial;
	AnimationState * animationState;
	size_t boneVertexCount, boneIndexCount;
	Matrix4x4f matrix;
	Vector3f axis, boneVector, position, normal;
	float boneDotUp, angle, boneLength;
	
	vertices = malloc(sizeof(boneVertices) * self->boneCount);
	indexes = malloc(sizeof(boneIndexes) * self->boneCount);
	boneVertexCount = sizeof(boneVertices) / sizeof(boneVertices[0]);
	boneIndexCount = sizeof(boneIndexes) / sizeof(boneIndexes[0]);
	for (boneIndex = 0; boneIndex < self->boneCount; boneIndex++) {
		boneVector = Vector3f_subtract(self->bones[boneIndex].endpoint, self->bones[boneIndex].position);
		boneLength = Vector3f_magnitude(boneVector);
		Vector3f_normalize(&boneVector);
		boneDotUp = Vector3f_dot(boneVector, VECTOR3f_UP);
		if (boneDotUp < -0.9999f) {
			axis = VECTOR3f_RIGHT;
			angle = M_PI;
		} else if (boneDotUp > 0.9999f) {
			axis = VECTOR3f_RIGHT;
			angle = 0.0f;
		} else {
			axis = Vector3f_normalized(Vector3f_cross(boneVector, VECTOR3f_UP));
			angle = -acosf(boneDotUp);
		}
		
		matrix = MATRIX4x4f_IDENTITY;
		Matrix4x4f_translate(&matrix, self->bones[boneIndex].position.x, self->bones[boneIndex].position.y, self->bones[boneIndex].position.z);
		Matrix4x4f_scale(&matrix, boneLength, boneLength, boneLength);
		Matrix4x4f_rotate(&matrix, axis, angle);
		
		for (vertexIndex = 0; vertexIndex < boneVertexCount; vertexIndex++) {
			vertex = boneVertices[vertexIndex];
			vertex.boneIndexes[0] = boneIndex;
			position = Matrix4x4f_multiplyVector3f(matrix, VECTOR3f(vertex.position[0], vertex.position[1], vertex.position[2]));
			normal = Matrix4x4f_multiplyVector3f_rotationOnly(matrix, VECTOR3f(vertex.normal[0], vertex.normal[1], vertex.normal[2]));
			vertex.position[0] = position.x;
			vertex.position[1] = position.y;
			vertex.position[2] = position.z;
			vertex.normal[0] = normal.x;
			vertex.normal[1] = normal.y;
			vertex.normal[2] = normal.z;
			vertices[boneIndex * boneVertexCount + vertexIndex] = vertex;
		}
		for (indexIndex = 0; indexIndex < boneIndexCount; indexIndex++) {
			indexes[boneIndex * boneIndexCount + indexIndex] = boneIndexes[indexIndex] + boneIndex * boneIndexCount;
		}
	}
	
	if (boneMaterial == NULL) {
		unsigned char boneColor[4] = {0xFF, 0xFF, 0xEF, 0xFF};
		boneMaterial = Material_create();
		Material_setColorTexture(boneMaterial, false, 1, 1, 4, boneColor);
	}
	animationState = AnimationState_create(self);
	mesh = MeshRenderable_createAnimated(vertices, sizeof(boneVertices) / sizeof(boneVertices[0]) * self->boneCount, indexes, sizeof(boneIndexes) / sizeof(boneIndexes[0]) * self->boneCount, boneMaterial, animationState);
	AnimationState_dispose(animationState);
	free(vertices);
	free(indexes);
	
	return mesh;
}
