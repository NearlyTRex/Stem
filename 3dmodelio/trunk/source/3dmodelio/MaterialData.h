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

#ifndef __MaterialData_H__
#define __MaterialData_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MaterialData MaterialData;

#include "glgraphics/Material.h"
#include "glgraphics/VertexTypes.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include "stemobject/StemObject.h"
#include "utilities/Atom.h"

#define MATERIALDATA_FORMAT_VERSION 0
#define MATERIALDATA_FORMAT_TYPE "material"

#define MaterialData_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Atom name; \
	Color4f color; \
	float specularity; \
	float shininess; \
	float emissiveness; \
	Atom colorMapName; \
	bool magnifyColorMapNearest; \
	Atom normalMapName; \
	bool magnifyNormalMapNearest;

stemobject_struct_definition(MaterialData)

MaterialData * MaterialData_create(Atom name, Color4f color, float specularity, float shininess, float emissiveness, Atom colorMapName, bool magnifyColorMapNearest, Atom normalMapName, bool magnifyNormalMapNearest);
bool MaterialData_init(MaterialData * self, Atom name, Color4f color, float specularity, float shininess, float emissiveness, Atom colorMapName, bool magnifyColorMapNearest, Atom normalMapName, bool magnifyNormalMapNearest);
void MaterialData_dispose(MaterialData * self);

MaterialData * MaterialData_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool MaterialData_loadSerializedData(MaterialData * self, compat_type(DeserializationContext *) deserializationContext);
void MaterialData_serialize(MaterialData * self, compat_type(SerializationContext *) serializationContext);

// Creates a Material object initialized with this MaterialData's color, specularity, shininess, and emissiveness.
// Does not call Material_setTexture(); the caller must fetch the texture images themselves and call this function
// to fully initialize the returned Material.
Material * MaterialData_createMaterial(MaterialData * self);

#ifdef __cplusplus
}
#endif
#endif
