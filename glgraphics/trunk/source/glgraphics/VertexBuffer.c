/*
  Copyright (c) 2018 Alex Diener
  
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

#include "glgraphics/Renderer.h"
#include "glgraphics/VertexBuffer.h"
#include <stddef.h>
#include <stdlib.h>

#define SUPERCLASS StemObject

VertexBuffer * VertexBuffer_createPTNC(struct vertex_p3f_t2f_n3f_c4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount) {
	stemobject_create_implementation(VertexBuffer, initPTNC, vertices, vertexCount, indexes, indexCount)
}

VertexBuffer * VertexBuffer_createPTNCBW(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount) {
	stemobject_create_implementation(VertexBuffer, initPTNCBW, vertices, vertexCount, indexes, indexCount)
}

static void sharedInit(VertexBuffer * self, void * vertices, size_t verticesSize, GLuint * indexes, unsigned int indexCount) {
	call_super(init, self);
	self->dispose = VertexBuffer_dispose;
	
	glGenVertexArrays(1, &self->vaoID);
	glBindVertexArray(self->vaoID);
	glGenBuffers(1, &self->vertexBufferID);
	glGenBuffers(1, &self->indexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indexes) * indexCount, indexes, GL_STATIC_DRAW);
	
	self->indexCount = indexCount;
}

bool VertexBuffer_initPTNC(VertexBuffer * self, struct vertex_p3f_t2f_n3f_c4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount) {
	sharedInit(self, vertices, sizeof(*vertices) * vertexCount, indexes, indexCount);
	
	glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORD);
	glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, position));
	glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, texCoords));
	glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, normal));
	glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f, color));
	glBindVertexArray(0);
	
	return true;
}

bool VertexBuffer_initPTNCBW(VertexBuffer * self, struct vertex_p3f_t2f_n3f_c4f_b4u_w4f * vertices, unsigned int vertexCount, GLuint * indexes, unsigned int indexCount) {
	sharedInit(self, vertices, sizeof(*vertices) * vertexCount, indexes, indexCount);
	
	glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORD);
	glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
	glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_ID);
	glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_WEIGHT);
	glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, position));
	glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, texCoords));
	glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, normal));
	glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, color));
	glVertexAttribPointer(VERTEX_ATTRIB_BONE_ID, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, boneIndexes));
	glVertexAttribPointer(VERTEX_ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_c4f_b4u_w4f, boneWeights));
	glBindVertexArray(0);
	
	return true;
}

void VertexBuffer_dispose(VertexBuffer * self) {
	glDeleteBuffers(1, &self->vertexBufferID);
	glDeleteBuffers(1, &self->indexBufferID);
	glDeleteVertexArrays(1, &self->vaoID);
	call_super(dispose, self);
}
