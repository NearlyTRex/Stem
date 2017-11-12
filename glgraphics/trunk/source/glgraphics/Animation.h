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

#ifndef __Animation_H__
#define __Animation_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Animation Animation;

#include "stemobject/StemObject.h"
#include "glgraphics/AnimationState.h"

// TODO: Bezier handles for individual components?
// Bezier handles exist in a coordinate space where {0, 0} is the left keyframe, and {1, 1} is the right keyframe.
// The input linear weight value is taken as the x coordinate, and the y value at that location is the output curved weight.
// Curves that go outside the range of 0-1 on the x axis are clipped. Curves that have control points passing each other
// will have their control points clipped to ensure that the curve cannot overlap itself on the time (x) axis.
struct AnimationBoneKeyframe {
	unsigned int boneIndex;
	Vector3f offset;
	Vector2f incomingOffsetBezierHandle;
	Vector2f outgoingOffsetBezierHandle;
	Vector3f scale;
	Vector2f incomingScaleBezierHandle;
	Vector2f outgoingScaleBezierHandle;
	Quaternionf rotation;
	Vector2f incomingRotationBezierHandle;
	Vector2f outgoingRotationBezierHandle;
};

struct AnimationKeyframe {
	double interval;
	unsigned int boneCount;
	struct AnimationBoneKeyframe * bones;
};

#define Animation_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Atom name; \
	Armature * armature; \
	double animationLength; \
	unsigned int keyframeCount; \
	struct AnimationKeyframe * keyframes;

stemobject_struct_definition(Animation)

// keyframes is copied; caller retains ownership of their copy. armature is referenced and not copied.
Animation * Animation_create(Atom name, Armature * armature, unsigned int keyframeCount, struct AnimationKeyframe * keyframes);
bool Animation_init(Animation * self, Atom name, Armature * armature, unsigned int keyframeCount, struct AnimationKeyframe * keyframes);
void Animation_dispose(Animation * self);
AnimationState * Animation_createAnimationStateAtTime(Animation * self, double animationTime);
void Animation_setAnimationStateAtTime(Animation * self, AnimationState * animationState, double animationTime);

#ifdef __cplusplus
}
#endif
#endif
