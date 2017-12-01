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

#include "gamemath/BezierCurve.h"
#include "glgraphics/Animation.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject
#define CURVE_SAMPLE_ITERATIONS 8

Animation * Animation_create(Atom name, Armature * armature, unsigned int keyframeCount, struct AnimationKeyframe * keyframes) {
	stemobject_create_implementation(Animation, init, name, armature, keyframeCount, keyframes)
}

bool Animation_init(Animation * self, Atom name, Armature * armature, unsigned int keyframeCount, struct AnimationKeyframe * keyframes) {
	unsigned int keyframeIndex;
	
	call_super(init, self);
	self->dispose = Animation_dispose;
	self->name = name;
	self->armature = armature;
	assert(keyframeCount > 0);
	self->animationLength = 0.0;
	self->keyframeCount = keyframeCount;
	self->keyframes = malloc(sizeof(*self->keyframes) * keyframeCount);
	for (keyframeIndex = 0; keyframeIndex < keyframeCount; keyframeIndex++) {
		self->animationLength += keyframes[keyframeIndex].interval;
		self->keyframes[keyframeIndex].interval = keyframes[keyframeIndex].interval;
		self->keyframes[keyframeIndex].boneCount = keyframes[keyframeIndex].boneCount;
		self->keyframes[keyframeIndex].bones = malloc(sizeof(*self->keyframes[keyframeIndex].bones) * keyframes[keyframeIndex].boneCount);
		memcpy(self->keyframes[keyframeIndex].bones, keyframes[keyframeIndex].bones, sizeof(*self->keyframes[keyframeIndex].bones) * keyframes[keyframeIndex].boneCount);
	}
	return true;
}

void Animation_dispose(Animation * self) {
	unsigned int keyframeIndex;
	
	for (keyframeIndex = 0; keyframeIndex < self->keyframeCount; keyframeIndex++) {
		free(self->keyframes[keyframeIndex].bones);
	}
	free(self->keyframes);
	call_super(dispose, self);
}

AnimationState * Animation_createAnimationStateAtTime(Animation * self, double animationTime) {
	AnimationState * animationState;
	
	animationState = AnimationState_create(self->armature);
	Animation_poseAnimationStateAtTime(self, animationState, animationTime, 1.0f);
	return animationState;
}

static bool Animation_findBoneKeyframes(Animation * self, unsigned int boneIndex, double animationTime, unsigned int * outKeyframeIndexLeft, unsigned int * outBoneKeyframeIndexLeft, unsigned int * outKeyframeIndexRight, unsigned int * outBoneKeyframeIndexRight, float * outKeyframeWeight) {
	double keyframeTime = 0.0, keyframeTimeLeft = 0.0, keyframeTimeRight = 0.0;
	unsigned int keyframeIndex, keyframeIndexLeft = 0, keyframeIndexRight = 0;
	unsigned int boneKeyframeIndex, boneKeyframeIndexLeft = BONE_INDEX_NOT_FOUND, boneKeyframeIndexRight = BONE_INDEX_NOT_FOUND;
	
	// Search for keyframe with an entry for this bone closest to animationTime
	for (keyframeIndex = 0; keyframeIndex < self->keyframeCount && keyframeTime < animationTime; keyframeIndex++) {
		for (boneKeyframeIndex = 0; boneKeyframeIndex < self->keyframes[keyframeIndex].boneCount; boneKeyframeIndex++) {
			if (self->keyframes[keyframeIndex].bones[boneKeyframeIndex].boneIndex == boneIndex) {
				keyframeIndexLeft = keyframeIndex;
				keyframeTimeLeft = keyframeTime;
				boneKeyframeIndexLeft = boneKeyframeIndex;
				break;
			}
		}
		keyframeTime += self->keyframes[keyframeIndex].interval;
	}
	if (boneKeyframeIndexLeft == BONE_INDEX_NOT_FOUND) {
		// If the bone isn't specified prior to animationTime, find the latest specification past it, if any
		for (; keyframeIndex < self->keyframeCount; keyframeIndex++) {
			for (boneKeyframeIndex = 0; boneKeyframeIndex < self->keyframes[keyframeIndex].boneCount; boneKeyframeIndex++) {
				if (self->keyframes[keyframeIndex].bones[boneKeyframeIndex].boneIndex == boneIndex) {
					keyframeIndexLeft = keyframeIndex;
					keyframeTimeLeft = keyframeTime;
					boneKeyframeIndexLeft = boneKeyframeIndex;
					break;
				}
			}
			keyframeTime += self->keyframes[keyframeIndex].interval;
		}
	}
	if (boneKeyframeIndexLeft == BONE_INDEX_NOT_FOUND) {
		// This bone isn't specified in this animation
		return false;
	}
	
	// Search for the next appearance of this bone in a keyframe
	keyframeTime = keyframeTimeLeft + self->keyframes[keyframeIndexLeft].interval;
	for (keyframeIndex = keyframeIndexLeft + 1; keyframeIndex < self->keyframeCount; keyframeIndex++) {
		for (boneKeyframeIndex = 0; boneKeyframeIndex < self->keyframes[keyframeIndex].boneCount; boneKeyframeIndex++) {
			if (self->keyframes[keyframeIndex].bones[boneKeyframeIndex].boneIndex == boneIndex) {
				keyframeIndexRight = keyframeIndex;
				keyframeTimeRight = keyframeTime;
				boneKeyframeIndexRight = boneKeyframeIndex;
				break;
			}
		}
		keyframeTime += self->keyframes[keyframeIndex].interval;
		if (boneKeyframeIndexRight != BONE_INDEX_NOT_FOUND) {
			break;
		}
	}
	if (boneKeyframeIndexRight == BONE_INDEX_NOT_FOUND) {
		// Wrap search around to the beginning if necessary
		for (keyframeIndex = 0; keyframeIndex < keyframeIndexLeft; keyframeIndex++) {
			for (boneKeyframeIndex = 0; boneKeyframeIndex < self->keyframes[keyframeIndex].boneCount; boneKeyframeIndex++) {
				if (self->keyframes[keyframeIndex].bones[boneKeyframeIndex].boneIndex == boneIndex) {
					keyframeIndexRight = keyframeIndex;
					keyframeTimeRight = keyframeTime;
					boneKeyframeIndexRight = boneKeyframeIndex;
					break;
				}
			}
			keyframeTime += self->keyframes[keyframeIndex].interval;
			if (boneKeyframeIndexRight != BONE_INDEX_NOT_FOUND) {
				break;
			}
		}
	}
	if (boneKeyframeIndexRight == BONE_INDEX_NOT_FOUND) {
		keyframeIndexRight = keyframeIndexLeft;
		boneKeyframeIndexRight = boneKeyframeIndexLeft;
		keyframeTimeRight = keyframeTimeLeft;
	}
	
	*outKeyframeIndexLeft = keyframeIndexLeft;
	*outBoneKeyframeIndexLeft = boneKeyframeIndexLeft;
	*outKeyframeIndexRight = keyframeIndexRight;
	*outBoneKeyframeIndexRight = boneKeyframeIndexRight;
	// TODO: There may be degenerate cases in here. Shouldn't left and right be swapped if animationTime is outside them?
	if (keyframeTimeLeft == keyframeTimeRight) {
		*outKeyframeWeight = 0.0f;
	} else if (keyframeTimeLeft < keyframeTimeRight) {
		if (animationTime < keyframeTimeLeft) {
			// Degenerate
			*outKeyframeWeight = 0.0f;
		} else if (animationTime < keyframeTimeRight) {
			*outKeyframeWeight = (animationTime - keyframeTimeLeft) / (keyframeTimeRight - keyframeTimeLeft);
		} else {
			// Degenerate
			*outKeyframeWeight = 1.0f;
		}
	} else {
		if (animationTime < keyframeTimeRight) {
			*outKeyframeWeight = (animationTime - (keyframeTimeLeft - self->animationLength)) / (keyframeTimeRight - (keyframeTimeLeft - self->animationLength));
		} else if (animationTime < keyframeTimeLeft) {
			// Degenerate
			*outKeyframeWeight = 0.0f;
		} else {
			*outKeyframeWeight = (animationTime - keyframeTimeLeft) / ((keyframeTimeRight + self->animationLength) - keyframeTimeLeft);
		}
	}
	return true;
}

static inline float curvedKeyframeInterpolationValue(Vector2f leftHandle, Vector2f rightHandle, float value) {
	if (leftHandle.x == 0.0f && leftHandle.y == 0.0f && rightHandle.x == 1.0f && rightHandle.y == 1.0f) {
		return value;
	}
	return BezierCurve_sampleYAtX(VECTOR2f(0.0f, 0.0f), leftHandle, rightHandle, VECTOR2f(1.0f, 1.0f), value, CURVE_SAMPLE_ITERATIONS);
}

void Animation_poseAnimationStateAtTime(Animation * self, AnimationState * animationState, double animationTime, float weight) {
	unsigned int boneIndex;
	unsigned int keyframeIndexLeft = 0, keyframeIndexRight = 0;
	unsigned int boneKeyframeIndexLeft, boneKeyframeIndexRight;
	float keyframeWeight;
	struct AnimationBoneState boneState;
	
	assert(self->armature->boneCount == animationState->armature->boneCount);
	
	animationTime = fmod(animationTime, self->animationLength);
	if (animationTime < 0.0) {
		animationTime += self->animationLength;
	}
	
	for (boneIndex = 0; boneIndex < self->armature->boneCount; boneIndex++) {
		if (Animation_findBoneKeyframes(self, boneIndex, animationTime, &keyframeIndexLeft, &boneKeyframeIndexLeft, &keyframeIndexRight, &boneKeyframeIndexRight, &keyframeWeight)) {
			// Interpolate bone based on keyframes
			struct AnimationBoneKeyframe boneKeyframeLeft = self->keyframes[keyframeIndexLeft].bones[boneKeyframeIndexLeft];
			struct AnimationBoneKeyframe boneKeyframeRight = self->keyframes[keyframeIndexRight].bones[boneKeyframeIndexRight];
			
			boneState.offset   = Vector3f_interpolate(boneKeyframeLeft.offset,   boneKeyframeRight.offset,   curvedKeyframeInterpolationValue(boneKeyframeLeft.outgoingOffsetBezierHandle,   boneKeyframeRight.incomingOffsetBezierHandle,   keyframeWeight));
			boneState.scale    = Vector3f_interpolate(boneKeyframeLeft.scale,    boneKeyframeRight.scale,    curvedKeyframeInterpolationValue(boneKeyframeLeft.outgoingScaleBezierHandle,    boneKeyframeRight.incomingScaleBezierHandle,    keyframeWeight));
			boneState.rotation =    Quaternionf_slerp(boneKeyframeLeft.rotation, boneKeyframeRight.rotation, curvedKeyframeInterpolationValue(boneKeyframeLeft.outgoingRotationBezierHandle, boneKeyframeRight.incomingRotationBezierHandle, keyframeWeight));
			
			animationState->boneStates[boneIndex].offset   = Vector3f_add(animationState->boneStates[boneIndex].offset, Vector3f_multiplyScalar(boneState.offset, weight));
			animationState->boneStates[boneIndex].scale    = Vector3f_multiplyComponents(animationState->boneStates[boneIndex].scale, Vector3f_interpolate(VECTOR3f(1.0f, 1.0f, 1.0f), boneState.scale, weight));
			Quaternionf_multiply(&animationState->boneStates[boneIndex].rotation, Quaternionf_slerp(QUATERNIONf_IDENTITY, boneState.rotation, weight));
		}
	}
}
