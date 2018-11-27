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

#include "3dmodelio/MaterialData.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

MaterialData * MaterialData_create(Atom name, Color4f color, float specularity, float shininess, float emissiveness, Atom colorMapName, bool magnifyColorMapNearest, Atom normalMapName, bool magnifyNormalMapNearest) {
	stemobject_create_implementation(MaterialData, init, name, color, specularity, shininess, emissiveness, colorMapName, magnifyColorMapNearest, normalMapName, magnifyNormalMapNearest)
}

static void sharedInit(MaterialData * self) {
	call_super(init, self);
	self->dispose = MaterialData_dispose;
}

bool MaterialData_init(MaterialData * self, Atom name, Color4f color, float specularity, float shininess, float emissiveness, Atom colorMapName, bool magnifyColorMapNearest, Atom normalMapName, bool magnifyNormalMapNearest) {
	sharedInit(self);
	self->name = name;
	self->color = color;
	self->specularity = specularity;
	self->shininess = shininess;
	self->emissiveness = emissiveness;
	self->colorMapName = colorMapName;
	self->magnifyColorMapNearest = magnifyColorMapNearest;
	self->normalMapName = normalMapName;
	self->magnifyNormalMapNearest = magnifyNormalMapNearest;
	return true;
}

void MaterialData_dispose(MaterialData * self) {
	call_super(dispose, self);
}

MaterialData * MaterialData_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(MaterialData, deserializationContext, loadSerializedData)
}

bool MaterialData_loadSerializedData(MaterialData * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	const char * formatType, * nameString, * colorMapNameString = NULL, * normalMapNameString = NULL;
	
	context->beginStructure(context, MATERIALDATA_FORMAT_TYPE);
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status != SERIALIZATION_ERROR_OK || formatVersion > MATERIALDATA_FORMAT_VERSION) {
		return false;
	}
	formatType = context->readString(context, "format_type");
	if (context->status != SERIALIZATION_ERROR_OK || strcmp(formatType, MATERIALDATA_FORMAT_TYPE)) {
		return false;
	}
	
	nameString = context->readStringNullable(context, "name");
	context->beginStructure(context, "color");
	self->color.red = context->readFloat(context, "red");
	self->color.green = context->readFloat(context, "green");
	self->color.blue = context->readFloat(context, "blue");
	self->color.alpha = context->readFloat(context, "alpha");
	context->endStructure(context);
	self->specularity = context->readFloat(context, "specularity");
	self->shininess = context->readFloat(context, "shininess");
	self->emissiveness = context->readFloat(context, "emissiveness");
	
	context->beginDictionary(context, "textures");
	if (context->hasDictionaryKey(context, "color_map")) {
		context->beginStructure(context, "color_map");
		colorMapNameString = context->readString(context, "name");
		self->magnifyColorMapNearest = context->readBoolean(context, "magnify_nearest");
		context->endStructure(context);
	}
	if (context->hasDictionaryKey(context, "normal_map")) {
		context->beginStructure(context, "normal_map");
		normalMapNameString = context->readString(context, "name");
		self->magnifyNormalMapNearest = context->readBoolean(context, "magnify_nearest");
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		return false;
	}
	
	sharedInit(self);
	self->name = Atom_fromString(nameString);
	self->colorMapName = Atom_fromString(colorMapNameString);
	self->normalMapName = Atom_fromString(normalMapNameString);
	
	return true;
}

void MaterialData_serialize(MaterialData * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	
	context->beginStructure(context, MATERIALDATA_FORMAT_TYPE);
	context->writeUInt16(context, "format_version", MATERIALDATA_FORMAT_VERSION);
	context->writeString(context, "format_type", MATERIALDATA_FORMAT_TYPE);
	context->writeStringNullable(context, "name", self->name);
	context->beginStructure(context, "color");
	context->writeFloat(context, "red", self->color.red);
	context->writeFloat(context, "green", self->color.green);
	context->writeFloat(context, "blue", self->color.blue);
	context->writeFloat(context, "alpha", self->color.alpha);
	context->endStructure(context);
	context->writeFloat(context, "specularity", self->specularity);
	context->writeFloat(context, "shininess", self->shininess);
	context->writeFloat(context, "emissiveness", self->emissiveness);
	context->beginDictionary(context, "textures");
	if (self->colorMapName != NULL) {
		context->beginStructure(context, "color_map");
		context->writeString(context, "name", self->colorMapName);
		context->writeBoolean(context, "magnify_nearest", self->magnifyColorMapNearest);
		context->endStructure(context);
	}
	if (self->normalMapName != NULL) {
		context->beginStructure(context, "normal_map");
		context->writeString(context, "name", self->normalMapName);
		context->writeBoolean(context, "magnify_nearest", self->magnifyNormalMapNearest);
		context->endStructure(context);
	}
	context->endDictionary(context);
	context->endStructure(context);
}

Material * MaterialData_createMaterial(MaterialData * self) {
	return Material_create(self->color, self->specularity, self->shininess, self->emissiveness);
}
