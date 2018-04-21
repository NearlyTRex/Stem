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

#ifndef __Armature_H__
#define __Armature_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Armature Armature;

#include "stemobject/StemObject.h"
#include "gamemath/Quaternionf.h"
#include "gamemath/Vector3f.h"
#include "glgraphics/VertexBuffer.h"
#include "utilities/Atom.h"
#include <limits.h>

#define BONE_INDEX_NOT_FOUND UINT_MAX

struct ArmatureBone {
	Atom boneID;
	unsigned int parentIndex;
	Vector3f position;
	Vector3f endpoint;
	// Animation quaternions are relative to this. Absolute for relaxed pose.
	Quaternionf baseOrientation;
};

#define Armature_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Atom name; \
	unsigned int boneCount; \
	struct ArmatureBone * bones;

stemobject_struct_definition(Armature)

// bones is copied
// In a correctly formed armature, all parent indexes must be less than the index of the bone referencing them.
Armature * Armature_create(Atom name, unsigned int boneCount, struct ArmatureBone * bones);
bool Armature_init(Armature * self, Atom name, unsigned int boneCount, struct ArmatureBone * bones);
void Armature_dispose(Armature * self);
unsigned int Armature_boneIndexForID(Armature * self, Atom boneID);

VertexBuffer * Armature_createDebugVertexBuffer(Armature * self);

#ifdef __cplusplus
}
#endif
#endif
