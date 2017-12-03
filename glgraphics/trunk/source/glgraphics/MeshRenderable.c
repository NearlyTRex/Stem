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

#include "glgraphics/MeshRenderable.h"
#include "glgraphics/Renderer.h"
#include <stddef.h>
#include <stdlib.h>
#include <float.h>

#define SUPERCLASS Renderable

MeshRenderable * MeshRenderable_createStatic(struct vertex_p3f_t2f_n3f_c4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount, Material * material) {
	stemobject_create_implementation(MeshRenderable, initStatic, vertices, vertexCount, indexes, indexCount, material)
}

MeshRenderable * MeshRenderable_createAnimated(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount, Material * material, AnimationState * animationState) {
	stemobject_create_implementation(MeshRenderable, initAnimated, vertices, vertexCount, indexes, indexCount, material, animationState)
}

void sharedInit(MeshRenderable * self, void * vertices, size_t verticesSize, GLuint * indexes, unsigned int indexCount, Material * material) {
	call_super(init, self, RENDERABLE_MESH);
	self->dispose = MeshRenderable_dispose;
	self->getBoundingBox = MeshRenderable_getBoundingBox;
	
	glGenVertexArrays(1, &self->vaoID);
	glBindVertexArray(self->vaoID);
	glGenBuffers(1, &self->vertexBufferID);
	glGenBuffers(1, &self->indexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indexes) * indexCount, indexes, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORD);
	glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
	
	self->transform = MATRIX4x4f_IDENTITY;
	self->material = material;
	self->indexCount = indexCount;
	
	self->bounds.left = FLT_MAX;
	self->bounds.right = -FLT_MAX;
	self->bounds.bottom = FLT_MAX;
	self->bounds.top = -FLT_MAX;
	self->bounds.back = FLT_MAX;
	self->bounds.front = -FLT_MAX;
}

bool MeshRenderable_initStatic(MeshRenderable * self, struct vertex_p3f_t2f_n3f_c4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount, Material * material) {
	unsigned int index;
	
	sharedInit(self, vertices, sizeof(*vertices) * vertexCount, indexes, indexCount, material);
	self->hasAnimationData = false;
	self->animationState = NULL;
	
	glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, position));
	glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, texCoords));
	glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, normal));
	glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, color));
	
	for (index = 0; index < indexCount; index++) {
		GLfloat * position = vertices[indexes[index]].position;
		
		if (position[0] < self->bounds.left) {
			self->bounds.left = position[0];
		}
		if (position[0] > self->bounds.right) {
			self->bounds.right = position[0];
		}
		if (position[1] < self->bounds.bottom) {
			self->bounds.bottom = position[1];
		}
		if (position[1] > self->bounds.top) {
			self->bounds.top = position[1];
		}
		if (position[2] < self->bounds.back) {
			self->bounds.back = position[2];
		}
		if (position[2] > self->bounds.front) {
			self->bounds.front = position[2];
		}
	}
	
	return true;
}

bool MeshRenderable_initAnimated(MeshRenderable * self, struct vertex_p3f_t2f_n3f_c4f_b4u_w4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount, Material * material, AnimationState * animationState) {
	unsigned int index;
	
	sharedInit(self, vertices, sizeof(*vertices) * vertexCount, indexes, indexCount, material);
	self->hasAnimationData = true;
	self->animationState = AnimationState_copy(animationState);
	
	glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_ID);
	glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_WEIGHT);
	glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, position));
	glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, texCoords));
	glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, normal));
	glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, color));
	glVertexAttribPointer(VERTEX_ATTRIB_BONE_ID, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, boneIndexes));
	glVertexAttribPointer(VERTEX_ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, boneWeights));
	
	for (index = 0; index < indexCount; index++) {
		GLfloat * position = vertices[indexes[index]].position;
		
		if (position[0] < self->bounds.left) {
			self->bounds.left = position[0];
		}
		if (position[0] > self->bounds.right) {
			self->bounds.right = position[0];
		}
		if (position[1] < self->bounds.bottom) {
			self->bounds.bottom = position[1];
		}
		if (position[1] > self->bounds.top) {
			self->bounds.top = position[1];
		}
		if (position[2] < self->bounds.back) {
			self->bounds.back = position[2];
		}
		if (position[2] > self->bounds.front) {
			self->bounds.front = position[2];
		}
	}
	
	return true;
}

void MeshRenderable_dispose(MeshRenderable * self) {
	glDeleteBuffers(1, &self->vertexBufferID);
	glDeleteBuffers(1, &self->indexBufferID);
	glDeleteVertexArrays(1, &self->vaoID);
	if (self->animationState != NULL) {
		AnimationState_dispose(self->animationState);
	}
	call_super(dispose, self);
}

Box6f MeshRenderable_getBoundingBox(MeshRenderable * self) {
	return self->bounds;
}

