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
#include <float.h>
#include <stddef.h>
#include <stdio.h>

#define SUPERCLASS StemObject

VertexBuffer * VertexBuffer_createPTNXC(const struct vertex_p3f_t2f_n3f_x4f_c4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferUsageHint usageHint) {
	stemobject_create_implementation(VertexBuffer, initPTNXC, vertices, vertexCount, indexes, indexCount, usageHint)
}

VertexBuffer * VertexBuffer_createPTNXCBW(const struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferUsageHint usageHint) {
	stemobject_create_implementation(VertexBuffer, initPTNXCBW, vertices, vertexCount, indexes, indexCount, usageHint)
}

static void calculateBounds(VertexBuffer * self, const void * vertices) {
	unsigned int vertexIndex;
	Vector3f position;
	
	self->bounds = BOX6f(FLT_MAX, -FLT_MAX, FLT_MAX, -FLT_MAX, FLT_MAX, -FLT_MAX);
	for (vertexIndex = 0; vertexIndex < self->vertexCount; vertexIndex++) {
		position = *((Vector3f *) (vertices + vertexIndex * self->private_ivar(vertexSize)));
		if (position.x < self->bounds.left) {
			self->bounds.left = position.x;
		}
		if (position.x > self->bounds.right) {
			self->bounds.right = position.x;
		}
		if (position.y < self->bounds.bottom) {
			self->bounds.bottom = position.y;
		}
		if (position.y > self->bounds.top) {
			self->bounds.top = position.y;
		}
		if (position.z < self->bounds.back) {
			self->bounds.back = position.z;
		}
		if (position.z > self->bounds.front) {
			self->bounds.front = position.z;
		}
	}
}

static void sharedInit(VertexBuffer * self, const void * vertices, size_t vertexSize, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferUsageHint usageHint) {
	call_super(init, self);
	self->dispose = VertexBuffer_dispose;
	
	glGenVertexArrays(1, &self->vaoID);
	glBindVertexArray(self->vaoID);
	glGenBuffers(1, &self->vertexBufferID);
	glGenBuffers(1, &self->indexBufferID);
	
	self->private_ivar(vertexSize) = vertexSize;
	self->private_ivar(mappedVertices) = NULL;
	self->usageHint = usageHint;
	VertexBuffer_bufferData(self, vertices, vertexCount, indexes, indexCount);
}

bool VertexBuffer_initPTNXC(VertexBuffer * self, const struct vertex_p3f_t2f_n3f_x4f_c4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferUsageHint usageHint) {
	sharedInit(self, vertices, sizeof(*vertices), vertexCount, indexes, indexCount, usageHint);
	
	glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORD);
	glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTRIB_TANGENT);
	glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
	glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f, position));
	glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f, texCoords));
	glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f, normal));
	glVertexAttribPointer(VERTEX_ATTRIB_TANGENT, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f, tangent));
	glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f, color));
	glBindVertexArray(0);
	
	return true;
}

bool VertexBuffer_initPTNXCBW(VertexBuffer * self, const struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferUsageHint usageHint) {
	sharedInit(self, vertices, sizeof(*vertices), vertexCount, indexes, indexCount, usageHint);
	
	glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORD);
	glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTRIB_TANGENT);
	glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
	glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_ID);
	glEnableVertexAttribArray(VERTEX_ATTRIB_BONE_WEIGHT);
	glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f, position));
	glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f, texCoords));
	glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f, normal));
	glVertexAttribPointer(VERTEX_ATTRIB_TANGENT, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f, tangent));
	glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f, color));
	glVertexAttribPointer(VERTEX_ATTRIB_BONE_ID, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f, boneIndexes));
	glVertexAttribPointer(VERTEX_ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f), (void *) offsetof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f, boneWeights));
	glBindVertexArray(0);
	
	return true;
}

void VertexBuffer_dispose(VertexBuffer * self) {
	glDeleteBuffers(1, &self->vertexBufferID);
	glDeleteBuffers(1, &self->indexBufferID);
	glDeleteVertexArrays(1, &self->vaoID);
	call_super(dispose, self);
}

void VertexBuffer_bufferData(VertexBuffer * self, const void * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount) {
	GLenum usageHint;
	
	switch (self->usageHint) {
		case VERTEX_BUFFER_USAGE_STATIC:
			usageHint = GL_STATIC_DRAW;
			break;
		case VERTEX_BUFFER_USAGE_STREAM:
			usageHint = GL_STREAM_DRAW;
			break;
		case VERTEX_BUFFER_USAGE_DYNAMIC:
			usageHint = GL_DYNAMIC_DRAW;
			break;
		default:
			fprintf(stderr, "Warning: Unknown usage hint %d specified to VertexBuffer_bufferData; defaulting to VERTEX_BUFFER_USAGE_STATIC\n", self->usageHint);
			usageHint = GL_STATIC_DRAW;
			break;
	}
	glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indexBufferID);
	glBufferData(GL_ARRAY_BUFFER, self->private_ivar(vertexSize) * vertexCount, vertices, usageHint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indexes) * indexCount, indexes, usageHint);
	self->vertexCount = vertexCount;
	self->indexCount = indexCount;
	calculateBounds(self, vertices);
}

void * VertexBuffer_mapData(VertexBuffer * self, enum VertexBufferDataType dataType, enum VertexBufferAccessMode accessMode) {
	GLenum glBufferType;
	GLenum glAccessMode;
	void * mappedData;
	
	switch (dataType) {
		case VERTEX_BUFFER_DATA_TYPE_VERTEX:
			glBufferType = GL_ARRAY_BUFFER;
			break;
		case VERTEX_BUFFER_DATA_TYPE_INDEX:
			glBufferType = GL_ELEMENT_ARRAY_BUFFER;
			break;
		default:
			fprintf(stderr, "Warning: Unknown data type %d specified to VertexBuffer_mapData; returning NULL\n", dataType);
			return NULL;
	}
	switch (accessMode) {
		case VERTEX_BUFFER_ACCESS_MODE_READ_ONLY:
			glAccessMode = GL_READ_ONLY;
			break;
		case VERTEX_BUFFER_ACCESS_MODE_WRITE_ONLY:
			glAccessMode = GL_WRITE_ONLY;
			break;
		case VERTEX_BUFFER_ACCESS_MODE_READ_WRITE:
			glAccessMode = GL_READ_WRITE;
			break;
		default:
			fprintf(stderr, "Warning: Unknown access mode %d specified to VertexBuffer_mapData; returning NULL\n", accessMode);
			return NULL;
	}
	
	mappedData = glMapBuffer(glBufferType, glAccessMode);
	if (dataType == VERTEX_BUFFER_DATA_TYPE_VERTEX && (accessMode == VERTEX_BUFFER_ACCESS_MODE_WRITE_ONLY || accessMode == VERTEX_BUFFER_ACCESS_MODE_READ_WRITE)) {
		self->private_ivar(mappedVertices) = mappedData;
	}
	return mappedData;
}

void VertexBuffer_unmapData(VertexBuffer * self, enum VertexBufferDataType dataType) {
	GLenum glBufferType;
	
	switch (dataType) {
		case VERTEX_BUFFER_DATA_TYPE_VERTEX:
			glBufferType = GL_ARRAY_BUFFER;
			if (self->private_ivar(mappedVertices) != NULL) {
				calculateBounds(self, self->private_ivar(mappedVertices));
				self->private_ivar(mappedVertices) = NULL;
			}
			break;
		case VERTEX_BUFFER_DATA_TYPE_INDEX:
			glBufferType = GL_ELEMENT_ARRAY_BUFFER;
			break;
		default:
			fprintf(stderr, "Warning: Unknown data type %d specified to VertexBuffer_unmapData; doing nothing\n", dataType);
			return;
	}
	
	glUnmapBuffer(glBufferType);
}
