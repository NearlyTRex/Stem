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

#ifndef __SceneLayout_H__
#define __SceneLayout_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SceneLayout SceneLayout;

#include "gamemath/Quaternionf.h"
#include "gamemath/Vector3f.h"
#include "glgraphics/VertexTypes.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include "stemobject/StemObject.h"
#include "utilities/Atom.h"

enum SceneLightType {
	SceneLightType_point,
	SceneLightType_directional
};

struct SceneLight {
	Atom name;
	enum SceneLightType type;
	Vector3f vector;
	Color3f color;
	float intensity;
	float falloff;
};

struct SceneMesh {
	Atom meshName;
	Vector3f offset;
	Vector3f scale;
	Quaternionf rotation;
};

#define SCENELAYOUT_SERIALIZATION_FORMAT_VERSION 0
#define SCENELAYOUT_SERIALIZATION_FORMAT_TYPE "scene"

#define SceneLayout_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Atom name; \
	unsigned int meshCount; \
	struct SceneMesh * meshes; \
	unsigned int lightCount; \
	struct SceneLight * lights;

stemobject_struct_definition(SceneLayout)

// meshes and lights are copied
SceneLayout * SceneLayout_create(Atom name, unsigned int meshCount, struct SceneMesh * meshes, unsigned int lightCount, struct SceneLight * lights);
bool SceneLayout_init(SceneLayout * self, Atom name, unsigned int meshCount, struct SceneMesh * meshes, unsigned int lightCount, struct SceneLight * lights);
void SceneLayout_dispose(SceneLayout * self);

SceneLayout * SceneLayout_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool SceneLayout_loadSerializedData(SceneLayout * self, compat_type(DeserializationContext *) deserializationContext);
void SceneLayout_serialize(SceneLayout * self, compat_type(SerializationContext *) serializationContext);

#ifdef __cplusplus
}
#endif
#endif
