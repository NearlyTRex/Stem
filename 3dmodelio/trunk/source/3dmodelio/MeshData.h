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

#ifndef __MeshData_H__
#define __MeshData_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MeshData MeshData;

#include "glgraphics/GLIncludes.h"
#include "glgraphics/VertexTypes.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include "stemobject/StemObject.h"
#include "utilities/Atom.h"

#define MESHDATA_FORMAT_VERSION 0
#define MESHDATA_FORMAT_TYPE "mesh"

#define MeshData_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Atom name; \
	const void * vertices; \
	unsigned int vertexCount; \
	bool private_ivar(verticesOwned); \
	const GLuint * indexes; \
	unsigned int indexCount; \
	bool private_ivar(indexesOwned); \
	Atom armatureName; \
	Atom materialName;

stemobject_struct_definition(MeshData)

// vertices and indexes are copied
// If armatureName is NULL, vertexBuffer must be in PTNXC format. If armatureName is non-NULL, vertexBuffer must be in PTNXCBW format.
MeshData * MeshData_create(Atom name, const void * vertices, unsigned int vertexCount, bool takeOwnershipOfVertices, bool copyVertices, const GLuint * indexes, unsigned int indexCount, bool takeOwnershipOfIndexes, bool copyIndexes, Atom armatureName, Atom materialName);
bool MeshData_init(MeshData * self, Atom name, const void * vertices, unsigned int vertexCount, bool takeOwnershipOfVertices, bool copyVertices, const GLuint * indexes, unsigned int indexCount, bool takeOwnershipOfIndexes, bool copyIndexes, Atom armatureName, Atom materialName);
void MeshData_dispose(MeshData * self);

MeshData * MeshData_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool MeshData_loadSerializedData(MeshData * self, compat_type(DeserializationContext *) deserializationContext);
void MeshData_serialize(MeshData * self, compat_type(SerializationContext *) serializationContext);

#ifdef __cplusplus
}
#endif
#endif
