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

#ifndef __AnimationState_H__
#define __AnimationState_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct AnimationState AnimationState;

#include "gamemath/Matrix4x4f.h"
#include "gamemath/Quaternionf.h"
#include "gamemath/Vector3f.h"
#include "glgraphics/Armature.h"
#include "stemobject/StemObject.h"

struct AnimationBoneState {
	Vector3f offset;
	Vector3f scale;
	Quaternionf rotation;
};

#define AnimationState_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Armature * armature; \
	struct AnimationBoneState * boneStates; \
	Matrix4x4f * computedBoneTransforms;

stemobject_struct_definition(AnimationState)

AnimationState * AnimationState_create(Armature * armature);
bool AnimationState_init(AnimationState * self, Armature * armature);
void AnimationState_initCopy(AnimationState * self, AnimationState * original);
void AnimationState_dispose(AnimationState * self);
AnimationState * AnimationState_copy(AnimationState * self);

// Resets the offset, scale, and rotation of all bones to the identity. Does not affect computedBoneTransforms.
void AnimationState_resetAllBones(AnimationState * self);

// Computes a matrix for each bone and stores it in computedBoneTransforms. This is not done automatically.
void AnimationState_computeBoneTransforms(AnimationState * self);

// Returns the base position of the specified bone after being transformed by its parent chain.
// AnimationState_computeBoneTransforms must be called before using this function.
Vector3f AnimationState_getBonePosition(AnimationState * self, unsigned int boneID);

// Same as above, but for the endpoint of the bone rather than the base position.
Vector3f AnimationState_getBoneEndpoint(AnimationState * self, unsigned int boneID);

// Adds the transforms in state to the transforms in self
void AnimationState_combine(AnimationState * self, AnimationState * state);

#ifdef __cplusplus
}
#endif
#endif
