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

#include "3dmodelio/SceneLayout.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

SceneLayout * SceneLayout_create(Atom name, unsigned int meshCount, struct SceneMesh * meshes, unsigned int lightCount, struct SceneLight * lights) {
	stemobject_create_implementation(SceneLayout, init, name, meshCount, meshes, lightCount, lights)
}

static void sharedInit(SceneLayout * self, Atom name) {
	call_super(init, self);
	self->dispose = SceneLayout_dispose;
	self->name = name;
}

bool SceneLayout_init(SceneLayout * self, Atom name, unsigned int meshCount, struct SceneMesh * meshes, unsigned int lightCount, struct SceneLight * lights) {
	sharedInit(self, name);
	self->meshCount = meshCount;
	if (meshCount == 0) {
		self->meshes = NULL;
	} else {
		self->meshes = malloc(sizeof(*meshes) * meshCount);
		memcpy(self->meshes, meshes, sizeof(*meshes) * meshCount);
	}
	self->lightCount = lightCount;
	if (lightCount == 0) {
		self->lights = NULL;
	} else {
		self->lights = malloc(sizeof(*lights) * lightCount);
		memcpy(self->lights, lights, sizeof(*lights) * lightCount);
	}
	return true;
}

void SceneLayout_dispose(SceneLayout * self) {
	free(self->meshes);
	free(self->lights);
	call_super(dispose, self);
}

SceneLayout * SceneLayout_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(SceneLayout, deserializationContext, loadSerializedData)
}

bool SceneLayout_loadSerializedData(SceneLayout * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * formatType, * nameString, * meshNameString, * lightNameString;
	unsigned int meshCount, meshIndex, lightCount, lightIndex;
	struct SceneMesh * meshes;
	struct SceneLight * lights;
	
	context->beginStructure(context, "scene");
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status != SERIALIZATION_ERROR_OK || formatVersion > SCENELAYOUT_SERIALIZATION_FORMAT_VERSION) {
		return false;
	}
	formatType = context->readString(context, "format_type");
	if (context->status != SERIALIZATION_ERROR_OK || strcmp(formatType, SCENELAYOUT_SERIALIZATION_FORMAT_TYPE)) {
		return false;
	}
	
	nameString = context->readStringNullable(context, "name");
	meshCount = context->beginDictionary(context, "meshes");
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	meshes = malloc(sizeof(*meshes) * meshCount);
	for (meshIndex = 0; meshIndex < meshCount; meshIndex++) {
		meshNameString = context->readNextDictionaryKey(context);
		context->beginStructure(context, meshNameString);
		context->beginStructure(context, "offset");
		meshes[meshIndex].offset.x = context->readFloat(context, "x");
		meshes[meshIndex].offset.y = context->readFloat(context, "y");
		meshes[meshIndex].offset.z = context->readFloat(context, "z");
		context->endStructure(context);
		context->beginStructure(context, "scale");
		meshes[meshIndex].scale.x = context->readFloat(context, "x");
		meshes[meshIndex].scale.y = context->readFloat(context, "y");
		meshes[meshIndex].scale.z = context->readFloat(context, "z");
		context->endStructure(context);
		context->beginStructure(context, "rotation");
		meshes[meshIndex].rotation.x = context->readFloat(context, "x");
		meshes[meshIndex].rotation.y = context->readFloat(context, "y");
		meshes[meshIndex].rotation.z = context->readFloat(context, "z");
		meshes[meshIndex].rotation.w = context->readFloat(context, "w");
		context->endStructure(context);
		context->endStructure(context);
		if (context->status != SERIALIZATION_ERROR_OK) {
			break;
		}
		meshes[meshIndex].meshName = Atom_fromString(meshNameString);
	}
	context->endDictionary(context);
	if (context->status != SERIALIZATION_ERROR_OK) {
		free(meshes);
		return false;
	}
	
	lightCount = context->beginDictionary(context, "lights");
	if (context->status != SERIALIZATION_ERROR_OK) {
		free(meshes);
		return false;
	}
	lights = malloc(sizeof(*lights) * lightCount);
	for (lightIndex = 0; lightIndex < lightCount; lightIndex++) {
		lightNameString = context->readNextDictionaryKey(context);
		context->beginStructure(context, lightNameString);
		lights[lightIndex].type = context->readEnumeration(context, "type", "POINT", SceneLightType_point, "DIRECTIONAL", SceneLightType_directional, NULL);
		context->beginStructure(context, lights[lightIndex].type == SceneLightType_point ? "position" : "direction");
		lights[lightIndex].vector.x = context->readFloat(context, "x");
		lights[lightIndex].vector.y = context->readFloat(context, "y");
		lights[lightIndex].vector.z = context->readFloat(context, "z");
		context->endStructure(context);
		context->beginStructure(context, "color");
		lights[lightIndex].color.red = context->readFloat(context, "red");
		lights[lightIndex].color.green = context->readFloat(context, "green");
		lights[lightIndex].color.blue = context->readFloat(context, "blue");
		context->endStructure(context);
		lights[lightIndex].intensity = context->readFloat(context, "intensity");
		if (lights[lightIndex].type == SceneLightType_point) {
			lights[lightIndex].falloff = context->readFloat(context, "falloff");
		} else {
			lights[lightIndex].falloff = 0.0f;
		}
		context->endStructure(context);
		if (context->status != SERIALIZATION_ERROR_OK) {
			break;
		}
		lights[lightIndex].name = Atom_fromString(lightNameString);
	}
	
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		free(meshes);
		free(lights);
		return false;
	}
	
	sharedInit(self, Atom_fromString(nameString));
	self->meshCount = meshCount;
	self->meshes = meshes;
	self->lightCount = lightCount;
	self->lights = lights;
	
	return true;
}

void SceneLayout_serialize(SceneLayout * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	unsigned int meshIndex, lightIndex;
	
	context->beginStructure(context, "scene");
	context->writeUInt16(context, "format_version", SCENELAYOUT_SERIALIZATION_FORMAT_VERSION);
	context->writeString(context, "format_type", SCENELAYOUT_SERIALIZATION_FORMAT_TYPE);
	context->writeStringNullable(context, "name", self->name);
	context->beginDictionary(context, "meshes");
	for (meshIndex = 0; meshIndex < self->meshCount; meshIndex++) {
		context->beginStructure(context, self->meshes[meshIndex].meshName);
		context->beginStructure(context, "offset");
		context->writeFloat(context, "x", self->meshes[meshIndex].offset.x);
		context->writeFloat(context, "y", self->meshes[meshIndex].offset.y);
		context->writeFloat(context, "z", self->meshes[meshIndex].offset.z);
		context->endStructure(context);
		context->beginStructure(context, "scale");
		context->writeFloat(context, "x", self->meshes[meshIndex].scale.x);
		context->writeFloat(context, "y", self->meshes[meshIndex].scale.y);
		context->writeFloat(context, "z", self->meshes[meshIndex].scale.z);
		context->endStructure(context);
		context->beginStructure(context, "rotation");
		context->writeFloat(context, "x", self->meshes[meshIndex].rotation.x);
		context->writeFloat(context, "y", self->meshes[meshIndex].rotation.y);
		context->writeFloat(context, "z", self->meshes[meshIndex].rotation.z);
		context->writeFloat(context, "w", self->meshes[meshIndex].rotation.w);
		context->endStructure(context);
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->beginDictionary(context, "lights");
	for (lightIndex = 0; lightIndex < self->lightCount; lightIndex++) {
		context->beginStructure(context, self->lights[lightIndex].name);
		context->writeEnumeration(context, "type", self->lights[lightIndex].type, "POINT", SceneLightType_point, "DIRECTIONAL", SceneLightType_directional, NULL);
		context->beginStructure(context, self->lights[lightIndex].type == SceneLightType_point ? "position" : "direction");
		context->writeFloat(context, "x", self->lights[lightIndex].vector.x);
		context->writeFloat(context, "y", self->lights[lightIndex].vector.y);
		context->writeFloat(context, "z", self->lights[lightIndex].vector.z);
		context->endStructure(context);
		context->beginStructure(context, "color");
		context->writeFloat(context, "red", self->lights[lightIndex].color.red);
		context->writeFloat(context, "green", self->lights[lightIndex].color.green);
		context->writeFloat(context, "blue", self->lights[lightIndex].color.blue);
		context->endStructure(context);
		context->writeFloat(context, "intensity", self->lights[lightIndex].intensity);
		if (self->lights[lightIndex].type == SceneLightType_point) {
			context->writeFloat(context, "falloff", self->lights[lightIndex].falloff);
		}
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
}
