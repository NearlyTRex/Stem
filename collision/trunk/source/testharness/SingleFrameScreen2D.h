/*
  Copyright (c) 2015 Alex Diener
  
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

#ifndef __SingleFrameScreen2D_H__
#define __SingleFrameScreen2D_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SingleFrameScreen2D SingleFrameScreen2D;

#include "collision/CollisionResolver.h"
#include "collision/IntersectionManager.h"
#include "gamemath/Vector2x.h"
#include "resourcemanager/ResourceManager.h"
#include "screenmanager/Screen.h"

#define SingleFrameScreen2D_structContents(self_type) \
	Screen_structContents(self_type) \
	\
	IntersectionManager * intersectionManager; \
	CollisionResolver * resolver; \
	Vector2x dragOrigin; \
	bool dragging; \
	bool draggingLastPosition; \
	bool draggingBoth; \
	bool draggingSize; \
	size_t selectedObjectIndex; \
	Vector2x dragStartPosition; \
	Vector2x dragStartLastPosition; \
	Vector2x dragStartSize; \
	Vector2x dragStartLastSize; \
	fixed16_16 dragStartRadius;

stemobject_struct_definition(SingleFrameScreen2D)

SingleFrameScreen2D * SingleFrameScreen2D_create(ResourceManager * resourceManager);
bool SingleFrameScreen2D_init(SingleFrameScreen2D * self, ResourceManager * resourceManager);
void SingleFrameScreen2D_dispose(SingleFrameScreen2D * self);

void SingleFrameScreen2D_activate(SingleFrameScreen2D * self);
void SingleFrameScreen2D_deactivate(SingleFrameScreen2D * self);

#ifdef __cplusplus
}
#endif
#endif
