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

#include "3dmodelio/MeshData.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

MeshData * MeshData_create(Atom name, const void * vertices, unsigned int vertexCount, bool takeOwnershipOfVertices, bool copyVertices, const GLuint * indexes, unsigned int indexCount, bool takeOwnershipOfIndexes, bool copyIndexes, Atom armatureName, Atom materialName) {
	stemobject_create_implementation(MeshData, init, name, vertices, vertexCount, takeOwnershipOfVertices, copyVertices, indexes, indexCount, takeOwnershipOfIndexes, copyIndexes, armatureName, materialName)
}

bool MeshData_init(MeshData * self, Atom name, const void * vertices, unsigned int vertexCount, bool takeOwnershipOfVertices, bool copyVertices, const GLuint * indexes, unsigned int indexCount, bool takeOwnershipOfIndexes, bool copyIndexes, Atom armatureName, Atom materialName) {
	size_t vertexSize;
	
	call_super(init, self);
	self->dispose = MeshData_dispose;
	self->name = name;
	if (armatureName == NULL) {
		vertexSize = sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f);
	} else {
		vertexSize = sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f);
	}
	if (copyVertices) {
		self->vertices = malloc(vertexSize * vertexCount);
		memcpy((void *) self->vertices, vertices, vertexSize * vertexCount);
	} else {
		self->vertices = vertices;
	}
	self->private_ivar(verticesOwned) = takeOwnershipOfVertices;
	self->vertexCount = vertexCount;
	if (copyIndexes) {
		self->indexes = malloc(sizeof(*indexes) * indexCount);
		memcpy((GLuint *) self->indexes, indexes, sizeof(*indexes) * indexCount);
	} else {
		self->indexes = indexes;
	}
	self->indexCount = indexCount;
	self->private_ivar(indexesOwned) = takeOwnershipOfIndexes;
	self->armatureName = armatureName;
	self->materialName = materialName;
	return true;
}

void MeshData_dispose(MeshData * self) {
	if (self->private_ivar(verticesOwned)) {
		free((void *) self->vertices);
	}
	if (self->private_ivar(indexesOwned)) {
		free((GLuint *) self->indexes);
	}
	call_super(dispose, self);
}

MeshData * MeshData_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(MeshData, deserializationContext, loadSerializedData)
}

bool MeshData_loadSerializedData(MeshData * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * formatType, * nameString, * armatureNameString, * materialNameString;
	size_t verticesSize, indexesSize;
	unsigned int vertexCount;
	const void * vertices;
	const GLuint * indexes;
	
	context->beginStructure(context, MESHDATA_FORMAT_TYPE);
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status != SERIALIZATION_ERROR_OK || formatVersion > MESHDATA_FORMAT_VERSION) {
		return false;
	}
	formatType = context->readString(context, "format_type");
	if (context->status != SERIALIZATION_ERROR_OK || strcmp(formatType, MESHDATA_FORMAT_TYPE)) {
		return false;
	}
	
	nameString = context->readStringNullable(context, "name");
	armatureNameString = context->readStringNullable(context, "armature");
	materialNameString = context->readStringNullable(context, "material");
	vertices = context->readBlob(context, "vertices", &verticesSize);
	indexes = context->readBlob(context, "indexes", &indexesSize);
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	
	if (armatureNameString == NULL) {
		vertexCount = verticesSize / sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f);
	} else {
		vertexCount = verticesSize / sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f);
	}
	MeshData_init(self, Atom_fromString(nameString), vertices, vertexCount, true, true, indexes, indexesSize / sizeof(GLuint), true, true, Atom_fromString(armatureNameString), Atom_fromString(materialNameString));
	return true;
}

void MeshData_serialize(MeshData * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	size_t vertexSize;
	
	context->beginStructure(context, MESHDATA_FORMAT_TYPE);
	context->writeUInt16(context, "format_version", MESHDATA_FORMAT_VERSION);
	context->writeString(context, "format_type", MESHDATA_FORMAT_TYPE);
	context->writeStringNullable(context, "name", self->name);
	context->writeStringNullable(context, "armature", self->armatureName);
	context->writeStringNullable(context, "material", self->materialName);
	if (self->armatureName == NULL) {
		vertexSize = sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f);
	} else {
		vertexSize = sizeof(struct vertex_p3f_t2f_n3f_x4f_c4f_b4f_w4f);
	}
	context->writeBlob(context, "vertices", self->vertices, vertexSize * self->vertexCount);
	context->writeBlob(context, "indexes", self->indexes, sizeof(GLuint) * self->indexCount);
	context->endStructure(context);
}
