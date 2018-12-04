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
#include <string.h>

#define SUPERCLASS StemObject

VertexBuffer * VertexBuffer_createPTC(const struct vertex_p2f_t2f_c4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferStorage storageType, enum VertexBufferUsageHint usageHint) {
	stemobject_create_implementation(VertexBuffer, initPTC, vertices, vertexCount, indexes, indexCount, storageType, usageHint)
}

VertexBuffer * VertexBuffer_createPTNXC(const struct vertex_p3f_t2f_n3f_x4f_c4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferStorage storageType, enum VertexBufferUsageHint usageHint) {
	stemobject_create_implementation(VertexBuffer, initPTNXC, vertices, vertexCount, indexes, indexCount, storageType, usageHint)
}

VertexBuffer * VertexBuffer_createPTNXCBW(const struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferStorage storageType, enum VertexBufferUsageHint usageHint) {
	stemobject_create_implementation(VertexBuffer, initPTNXCBW, vertices, vertexCount, indexes, indexCount, storageType, usageHint)
}

// TODO: VertexBuffer probably shouldn't calculate its own bounds. Remove this?
static void calculateBounds(VertexBuffer * self, const void * vertices) {
	unsigned int vertexIndex;
	
	if (self->private_ivar(hasZComponent)) {
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
		
	} else {
		Vector2f position;
		
		self->bounds = BOX6f(FLT_MAX, -FLT_MAX, FLT_MAX, -FLT_MAX, 0.0f, 0.0f);
		for (vertexIndex = 0; vertexIndex < self->vertexCount; vertexIndex++) {
			position = *((Vector2f *) (vertices + vertexIndex * self->private_ivar(vertexSize)));
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
		}
	}
}

static void sharedInit(VertexBuffer * self, const void * vertices, size_t vertexSize, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferStorage storageType, enum VertexBufferUsageHint usageHint) {
	call_super(init, self);
	self->dispose = VertexBuffer_dispose;
	
	self->private_ivar(vertexSize) = vertexSize;
	self->private_ivar(mappedVertices) = NULL;
	self->usageHint = usageHint;
	self->storageType = storageType;
	
	if (storageType == VERTEX_BUFFER_STORAGE_CPU_ONLY) {
		self->vaoID = self->vertexBufferID = self->indexBufferID = 0;
	} else {
		glGenVertexArrays(1, &self->vaoID);
		glBindVertexArray(self->vaoID);
		glGenBuffers(1, &self->vertexBufferID);
		glGenBuffers(1, &self->indexBufferID);
	}
	
	self->vertices = self->indexes = NULL;
	
	VertexBuffer_bufferData(self, vertices, vertexCount, indexes, indexCount);
}

bool VertexBuffer_initPTC(VertexBuffer * self, const struct vertex_p2f_t2f_c4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferStorage storageType, enum VertexBufferUsageHint usageHint) {
	self->private_ivar(hasZComponent) = false;
	sharedInit(self, vertices, sizeof(*vertices), vertexCount, indexes, indexCount, storageType, usageHint);
	
	if (storageType != VERTEX_BUFFER_STORAGE_CPU_ONLY) {
		glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
		glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE_COORD);
		glEnableVertexAttribArray(VERTEX_ATTRIB_COLOR);
		glVertexAttribPointer(VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, position));
		glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, texCoords));
		glVertexAttribPointer(VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_p2f_t2f_c4f), (void *) offsetof(struct vertex_p2f_t2f_c4f, color));
	}
	
	return true;
}

bool VertexBuffer_initPTNXC(VertexBuffer * self, const struct vertex_p3f_t2f_n3f_x4f_c4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferStorage storageType, enum VertexBufferUsageHint usageHint) {
	self->private_ivar(hasZComponent) = true;
	sharedInit(self, vertices, sizeof(*vertices), vertexCount, indexes, indexCount, storageType, usageHint);
	
	if (storageType != VERTEX_BUFFER_STORAGE_CPU_ONLY) {
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
	}
	
	return true;
}

bool VertexBuffer_initPTNXCBW(VertexBuffer * self, const struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferStorage storageType, enum VertexBufferUsageHint usageHint) {
	self->private_ivar(hasZComponent) = true;
	sharedInit(self, vertices, sizeof(*vertices), vertexCount, indexes, indexCount, storageType, usageHint);
	
	if (storageType != VERTEX_BUFFER_STORAGE_CPU_ONLY) {
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
	}
	
	return true;
}

void VertexBuffer_dispose(VertexBuffer * self) {
	glDeleteBuffers(1, &self->vertexBufferID);
	glDeleteBuffers(1, &self->indexBufferID);
	glDeleteVertexArrays(1, &self->vaoID);
	free(self->vertices);
	free(self->indexes);
	call_super(dispose, self);
}

void VertexBuffer_bufferData(VertexBuffer * self, const void * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount) {
	if (self->storageType != VERTEX_BUFFER_STORAGE_CPU_ONLY) {
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
	}
	
	if (self->storageType != VERTEX_BUFFER_STORAGE_GPU_ONLY) {
		free(self->vertices);
		free(self->indexes);
		self->vertices = malloc(self->private_ivar(vertexSize) * vertexCount);
		self->indexes = malloc(sizeof(*indexes) * indexCount);
		if (vertices != NULL) {
			memcpy(self->vertices, vertices, self->private_ivar(vertexSize) * vertexCount);
		}
		if (indexes != NULL) {
			memcpy(self->indexes, vertices, sizeof(*indexes) * indexCount);
		}
	}
	
	self->vertexCount = vertexCount;
	self->indexCount = indexCount;
	if (vertices != NULL) {
		calculateBounds(self, vertices);
	}
}

void * VertexBuffer_mapData(VertexBuffer * self, enum VertexBufferDataType dataType, enum VertexBufferAccessMode accessMode) {
	void * mappedData;
	
	if (self->storageType == VERTEX_BUFFER_STORAGE_CPU_ONLY) {
		switch (dataType) {
			case VERTEX_BUFFER_DATA_TYPE_VERTEX:
				mappedData = self->vertices;
				break;
			case VERTEX_BUFFER_DATA_TYPE_INDEX:
				mappedData = self->indexes;
				break;
			default:
				fprintf(stderr, "Warning: Unknown data type %d specified to VertexBuffer_mapData; returning NULL\n", dataType);
				return NULL;
		}
		
	} else {
		GLenum glBufferType;
		GLenum glAccessMode;
	
		switch (dataType) {
			case VERTEX_BUFFER_DATA_TYPE_VERTEX:
				glBufferType = GL_ARRAY_BUFFER;
				glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferID);
				break;
			case VERTEX_BUFFER_DATA_TYPE_INDEX:
				glBufferType = GL_ELEMENT_ARRAY_BUFFER;
				glBindBuffer(GL_ARRAY_BUFFER, self->indexBufferID);
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
	}
	
	if (accessMode == VERTEX_BUFFER_ACCESS_MODE_WRITE_ONLY || accessMode == VERTEX_BUFFER_ACCESS_MODE_READ_WRITE) {
		if (dataType == VERTEX_BUFFER_DATA_TYPE_VERTEX) {
			self->private_ivar(mappedVertices) = mappedData;
		} else {
			self->private_ivar(mappedIndexes) = mappedData;
		}
	}
	return mappedData;
}

void VertexBuffer_unmapData(VertexBuffer * self, enum VertexBufferDataType dataType) {
	if (self->storageType != VERTEX_BUFFER_STORAGE_CPU_ONLY) {
		GLenum glBufferType;
		
		switch (dataType) {
			case VERTEX_BUFFER_DATA_TYPE_VERTEX:
				glBufferType = GL_ARRAY_BUFFER;
				glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferID);
				if (self->private_ivar(mappedVertices) != NULL) {
					calculateBounds(self, self->private_ivar(mappedVertices));
					if (self->storageType == VERTEX_BUFFER_STORAGE_CPU_AND_GPU) {
						memcpy(self->vertices, self->private_ivar(mappedVertices), self->private_ivar(vertexSize) * self->vertexCount);
					}
					self->private_ivar(mappedVertices) = NULL;
				}
				break;
			case VERTEX_BUFFER_DATA_TYPE_INDEX:
				glBufferType = GL_ELEMENT_ARRAY_BUFFER;
				glBindBuffer(GL_ARRAY_BUFFER, self->indexBufferID);
				if (self->private_ivar(mappedIndexes) != NULL) {
					if (self->storageType == VERTEX_BUFFER_STORAGE_CPU_AND_GPU) {
						memcpy(self->indexes, self->private_ivar(mappedIndexes), sizeof(*self->indexes) * self->indexCount);
					}
					self->private_ivar(mappedIndexes) = NULL;
				}
				break;
			default:
				fprintf(stderr, "Warning: Unknown data type %d specified to VertexBuffer_unmapData; doing nothing\n", dataType);
				return;
		}
		glUnmapBuffer(glBufferType);
	}
}

void VertexBuffer_getVertices(VertexBuffer * self, void * outVertices, GLuint * outIndexes, unsigned int * ioVertexCount, unsigned int * ioIndexCount) {
	unsigned int vertexCount = 0, indexCount = 0;
	
	if (self->storageType == VERTEX_BUFFER_STORAGE_GPU_ONLY) {
		return;
	}
	
	if (ioVertexCount != NULL) {
		vertexCount = *ioVertexCount;
	}
	if (ioIndexCount != NULL) {
		indexCount = *ioIndexCount;
	}
	if (outVertices != NULL) {
		memcpy(outVertices + (vertexCount * self->private_ivar(vertexSize)), self->vertices, self->private_ivar(vertexSize) * self->vertexCount);
	}
	if (outIndexes != NULL) {
		memcpy(outIndexes + indexCount, self->indexes, sizeof(*self->indexes) * self->indexCount);
	}
	if (ioVertexCount != NULL) {
		*ioVertexCount = vertexCount;
	}
	if (ioIndexCount != NULL) {
		*ioIndexCount = indexCount;
	}
}
