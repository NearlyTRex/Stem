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
#include "gamemath/Quaternionf.h"
#include "gamemath/Vector3f.h"

// TODO: Use AnimationState structs, probably
/*
struct AnimationBoneState {
	unsigned int boneID;
	Vector3f offset;
	Quaternionf rotation;
};

struct AnimationMorphState {
	unsigned int morphID;
	float weight;
};
*/

struct AnimationFrame {
	unsigned int boneStateCount;
	struct AnimationBoneState * boneStates;
	unsigned int morphStateCount;
	struct AnimationMorphState * morphStates;
};

struct AnimationKeyframe {
	double interval;
	struct AnimationFrame frame;
};

#define Animation_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	const char * name; \
	unsigned int keyframeCount; \
	struct AnimationKeyframe * keyframes;

stemobject_struct_definition(Animation)

Animation * Animation_create();
bool Animation_init(Animation * self);
void Animation_dispose(Animation * self);

#ifdef __cplusplus
}
#endif
#endif
