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

#include "glgraphics/AnimationState.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

AnimationState * AnimationState_create(Armature * armature) {
	stemobject_create_implementation(AnimationState, init, armature)
}

static void sharedInit(AnimationState * self) {
	call_super(init, self);
	self->dispose = AnimationState_dispose;
}

bool AnimationState_init(AnimationState * self, Armature * armature) {
	unsigned int boneIndex;
	
	self->armature = armature;
	self->boneStates = malloc(sizeof(*self->boneStates) * armature->boneCount);
	self->computedBoneTransforms = malloc(sizeof(*self->computedBoneTransforms) * self->armature->boneCount);
	for (boneIndex = 0; boneIndex < armature->boneCount; boneIndex++) {
		self->boneStates[boneIndex].offset = VECTOR3f_ZERO;
		self->boneStates[boneIndex].scale = VECTOR3f(1.0f, 1.0f, 1.0f);
		self->boneStates[boneIndex].rotation = QUATERNIONf_IDENTITY;
		self->computedBoneTransforms[boneIndex] = MATRIX4x4f_IDENTITY;
	}
	return true;
}

void AnimationState_initCopy(AnimationState * self, AnimationState * original) {
	sharedInit(self);
	self->armature = original->armature;
	self->boneStates = malloc(sizeof(*self->boneStates) * self->armature->boneCount);
	self->computedBoneTransforms = malloc(sizeof(*self->computedBoneTransforms) * self->armature->boneCount);
	memcpy(self->boneStates, original->boneStates, sizeof(*self->boneStates) * self->armature->boneCount);
	memcpy(self->computedBoneTransforms, original->computedBoneTransforms, sizeof(*self->computedBoneTransforms) * self->armature->boneCount);
}

void AnimationState_dispose(AnimationState * self) {
	free(self->boneStates);
	free(self->computedBoneTransforms);
	call_super(dispose, self);
}

AnimationState * AnimationState_copy(AnimationState * self) {
	stemobject_copy_implementation(AnimationState, initCopy)
}

void AnimationState_computeBoneTransforms(AnimationState * self) {
	unsigned int boneIndex;
	Matrix4x4f matrix;
	
	for (boneIndex = 0; boneIndex < self->armature->boneCount; boneIndex++) {
		if (self->armature->bones[boneIndex].parentIndex == BONE_INDEX_NOT_FOUND) {
			matrix = MATRIX4x4f_IDENTITY;
		} else {
			matrix = self->computedBoneTransforms[self->armature->bones[boneIndex].parentIndex];
		}
		Matrix4x4f_scale(&matrix, self->boneStates[boneIndex].scale.x, self->boneStates[boneIndex].scale.y, self->boneStates[boneIndex].scale.z);
		Matrix4x4f_translate(&matrix, self->boneStates[boneIndex].offset.x, self->boneStates[boneIndex].offset.y, self->boneStates[boneIndex].offset.z);
		Matrix4x4f_translate(&matrix, self->armature->bones[boneIndex].position.x, self->armature->bones[boneIndex].position.y, self->armature->bones[boneIndex].position.z);
		Matrix4x4f_multiply(&matrix, Quaternionf_toMatrix(self->boneStates[boneIndex].rotation));
		Matrix4x4f_translate(&matrix, -self->armature->bones[boneIndex].position.x, -self->armature->bones[boneIndex].position.y, -self->armature->bones[boneIndex].position.z);
		self->computedBoneTransforms[boneIndex] = matrix;
	}
}

Vector3f AnimationState_getBonePosition(AnimationState * self, unsigned int boneID) {
	assert(boneID < self->armature->boneCount);
	return Matrix4x4f_multiplyVector3f(self->computedBoneTransforms[boneID], self->armature->bones[boneID].position);
}

Vector3f AnimationState_getBoneEndpoint(AnimationState * self, unsigned int boneID) {
	assert(boneID < self->armature->boneCount);
	return Matrix4x4f_multiplyVector3f(self->computedBoneTransforms[boneID], self->armature->bones[boneID].endpoint);
}
