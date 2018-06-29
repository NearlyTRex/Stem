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

#ifndef __VertexBuffer_H__
#define __VertexBuffer_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct VertexBuffer VertexBuffer;

enum VertexBufferAccessMode {
	VERTEX_BUFFER_ACCESS_MODE_READ_ONLY,
	VERTEX_BUFFER_ACCESS_MODE_WRITE_ONLY,
	VERTEX_BUFFER_ACCESS_MODE_READ_WRITE
};

enum VertexBufferUsageHint {
	VERTEX_BUFFER_USAGE_STATIC,
	VERTEX_BUFFER_USAGE_STREAM,
	VERTEX_BUFFER_USAGE_DYNAMIC
};

enum VertexBufferDataType {
	VERTEX_BUFFER_DATA_TYPE_VERTEX,
	VERTEX_BUFFER_DATA_TYPE_INDEX
};

#include "gamemath/Box6f.h"
#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "stemobject/StemObject.h"
#include <stdlib.h>

#define VertexBuffer_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	GLuint vaoID; \
	GLuint vertexBufferID; \
	GLuint indexBufferID; \
	unsigned int vertexCount; \
	unsigned int indexCount; \
	Box6f bounds; \
	enum VertexBufferUsageHint usageHint; \
	size_t private_ivar(vertexSize);

stemobject_struct_definition(VertexBuffer)

VertexBuffer * VertexBuffer_createPTNXC(const struct vertex_p3f_t2f_n3f_x4f_c4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferUsageHint usageHint);
VertexBuffer * VertexBuffer_createPTNXCBW(const struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferUsageHint usageHint);
bool VertexBuffer_initPTNXC(VertexBuffer * self, const struct vertex_p3f_t2f_n3f_x4f_c4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferUsageHint usageHint);
bool VertexBuffer_initPTNXCBW(VertexBuffer * self, const struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount, enum VertexBufferUsageHint usageHint);
void VertexBuffer_dispose(VertexBuffer * self);

// vertices must be of the same type this VertexBuffer was initialized with
void VertexBuffer_bufferData(VertexBuffer * self, const void * vertices, unsigned int vertexCount, const GLuint * indexes, unsigned int indexCount);
void * VertexBuffer_mapData(VertexBuffer * self, enum VertexBufferDataType dataType, enum VertexBufferAccessMode accessMode);
void VertexBuffer_unmapData(VertexBuffer * self, enum VertexBufferDataType dataType);

#ifdef __cplusplus
}
#endif
#endif
