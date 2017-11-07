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

#include "glgraphics/Animation.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

Animation * Animation_create(Atom name, Armature * armature, unsigned int keyframeCount, struct AnimationKeyframe * keyframes) {
	stemobject_create_implementation(Animation, init, name, armature, keyframeCount, keyframes)
}

bool Animation_init(Animation * self, Atom name, Armature * armature, unsigned int keyframeCount, struct AnimationKeyframe * keyframes) {
	unsigned int keyframeIndex;
	
	call_super(init, self);
	self->dispose = Animation_dispose;
	self->name = name;
	self->armature = armature;
	self->keyframeCount = keyframeCount;
	self->keyframes = malloc(sizeof(*self->keyframes) * keyframeCount);
	for (keyframeIndex = 0; keyframeIndex < keyframeCount; keyframeIndex++) {
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
	Animation_setAnimationStateAtTime(self, animationState, animationTime);
	return animationState;
}

void Animation_setAnimationStateAtTime(Animation * self, AnimationState * animationState, double animationTime) {
	// magic happens here
}
