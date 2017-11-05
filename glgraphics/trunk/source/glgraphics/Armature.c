/*
  Copyright (c) 2017 Alex Diener
  
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

#include "glgraphics/Armature.h"
#include <stdlib.h>
#include <string.h>
#ifdef DEBUG
#include <assert.h>
#endif

#define SUPERCLASS StemObject

Armature * Armature_create(unsigned int boneCount, struct ArmatureBone * bones) {
	stemobject_create_implementation(Armature, init, boneCount, bones)
}

bool Armature_init(Armature * self, unsigned int boneCount, struct ArmatureBone * bones) {
	call_super(init, self);
	self->dispose = Armature_dispose;
	self->boneCount = boneCount;
	self->bones = malloc(sizeof(*self->bones) * boneCount);
	memcpy(self->bones, bones, sizeof(*self->bones) * boneCount);
	
#ifdef DEBUG
	unsigned int boneIndex;
	
	for (boneIndex = 0; boneIndex < boneCount; boneIndex++) {
		if (bones[boneIndex].parentIndex != BONE_INDEX_NOT_FOUND) {
			assert(bones[boneIndex].parentIndex < boneIndex);
		}
	}
#endif
	
	return true;
}

void Armature_dispose(Armature * self) {
	free(self->bones);
	call_super(dispose, self);
}

unsigned int Armature_boneIndexForID(Armature * self, Atom boneID) {
	unsigned int boneIndex;
	
	for (boneIndex = 0; boneIndex < self->boneCount; boneIndex++) {
		if (self->bones[boneIndex].boneID == boneID) {
			return boneIndex;
		}
	}
	return BONE_INDEX_NOT_FOUND;
}
