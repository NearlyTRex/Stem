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

#ifndef __AnimationIO_H__
#define __AnimationIO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "glgraphics/Animation.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"

#define ANIMATION_FORMAT_VERSION 0
#define ANIMATION_FORMAT_TYPE "animation"

Animation * Animation_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool Animation_loadSerializedData(Animation * self, compat_type(DeserializationContext *) deserializationContext);
void Animation_serialize(Animation * self, compat_type(SerializationContext *) serializationContext);

#ifdef __cplusplus
}
#endif
#endif
