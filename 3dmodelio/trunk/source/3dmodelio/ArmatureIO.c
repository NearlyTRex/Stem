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

#include "3dmodelio/ArmatureIO.h"
#include <string.h>

Armature * Armature_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(Armature, deserializationContext, loadSerializedData)
}

bool Armature_loadSerializedData(Armature * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * formatType, * nameString, * boneIDString, * parentString;
	unsigned int boneCount, boneIndex, boneIndex2;
	struct ArmatureBone * bones;
	bool success;
	
	context->beginStructure(context, "armature");
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status != SERIALIZATION_ERROR_OK || formatVersion > ARMATURE_SERIALIZATION_FORMAT_VERSION) {
		return false;
	}
	formatType = context->readString(context, "format_type");
	if (context->status != SERIALIZATION_ERROR_OK || strcmp(formatType, ARMATURE_SERIALIZATION_FORMAT_TYPE)) {
		return false;
	}
	
	nameString = context->readStringNullable(context, "name");
	boneCount = context->beginDictionary(context, "bones");
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	bones = malloc(sizeof(*bones) * boneCount);
	for (boneIndex = 0; boneIndex < boneCount; boneIndex++) {
		boneIDString = context->readNextDictionaryKey(context);
		context->beginStructure(context, boneIDString);
		parentString = context->readStringNullable(context, "parent");
		if (parentString == NULL) {
			bones[boneIndex].parentIndex = BONE_INDEX_NOT_FOUND;
		} else {
			for (boneIndex2 = 0; boneIndex2 < boneIndex; boneIndex2++) {
				if (!strcmp(bones[bones[boneIndex].parentIndex].boneID, parentString)) {
					bones[boneIndex].parentIndex = boneIndex2;
					break;
				}
			}
			if (boneIndex2 == boneIndex) {
				free(bones);
				// TODO: Report error?
				return false;
			}
		}
		context->beginStructure(context, "position");
		bones[boneIndex].position.x = context->readFloat(context, "x");
		bones[boneIndex].position.y = context->readFloat(context, "y");
		bones[boneIndex].position.z = context->readFloat(context, "z");
		context->endStructure(context);
		context->beginStructure(context, "endpoint");
		bones[boneIndex].endpoint.x = context->readFloat(context, "x");
		bones[boneIndex].endpoint.y = context->readFloat(context, "y");
		bones[boneIndex].endpoint.z = context->readFloat(context, "z");
		context->endStructure(context);
		bones[boneIndex].roll = context->readFloat(context, "roll");
		context->endStructure(context);
		if (context->status != SERIALIZATION_ERROR_OK) {
			break;
		}
		bones[boneIndex].boneID = Atom_fromString(boneIDString);
	}
	context->endDictionary(context);
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		free(bones);
		return false;
	}
	
	success = Armature_init(self, Atom_fromString(nameString), boneCount, bones);
	free(bones);
	return success;
}

void Armature_serialize(Armature * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	unsigned int boneIndex;
	
	context->beginStructure(context, "armature");
	context->writeUInt16(context, "format_version", ARMATURE_SERIALIZATION_FORMAT_VERSION);
	context->writeString(context, "format_type", ARMATURE_SERIALIZATION_FORMAT_TYPE);
	context->writeStringNullable(context, "name", self->name);
	context->beginDictionary(context, "bones");
	for (boneIndex = 0; boneIndex < self->boneCount; boneIndex++) {
		context->beginStructure(context, self->bones[boneIndex].boneID);
		context->writeStringNullable(context, "parent", self->bones[boneIndex].parentIndex == BONE_INDEX_NOT_FOUND ? NULL : self->bones[self->bones[boneIndex].parentIndex].boneID);
		context->beginStructure(context, "position");
		context->writeFloat(context, "x", self->bones[boneIndex].position.x);
		context->writeFloat(context, "y", self->bones[boneIndex].position.y);
		context->writeFloat(context, "z", self->bones[boneIndex].position.z);
		context->endStructure(context);
		context->beginStructure(context, "endpoint");
		context->writeFloat(context, "x", self->bones[boneIndex].endpoint.x);
		context->writeFloat(context, "y", self->bones[boneIndex].endpoint.y);
		context->writeFloat(context, "z", self->bones[boneIndex].endpoint.z);
		context->endStructure(context);
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
}
