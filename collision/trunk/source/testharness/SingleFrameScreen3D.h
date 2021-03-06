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

#ifndef __SingleFrameScreen3D_H__
#define __SingleFrameScreen3D_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SingleFrameScreen3D SingleFrameScreen3D;

#include "collision/CollisionResolver.h"
#include "collision/IntersectionManager.h"
#include "gamemath/Quaternionx.h"
#include "gamemath/Vector3x.h"
#include "glbitmapfont/GLBitmapFont.h"
#include "glslshader/GLSLShader.h"
#include "resourcemanager/ResourceManager.h"
#include "screenmanager/Screen.h"

#define OBJECT_COUNT_3D 8

#define SingleFrameScreen3D_structContents(self_type) \
	Screen_structContents(self_type) \
	\
	IntersectionManager * intersectionManager; \
	CollisionResolver * resolver; \
	ResourceManager * resourceManager; \
	GLSLShader * lightShader; \
	GLBitmapFont * font; \
	bool draggingCamera; \
	size_t selectedObjectIndex; \
	Vector3x cameraFocus; \
	Quaternionx cameraDirection; \
	fixed16_16 cameraDistance; \
	CollisionRecord collisions[OBJECT_COUNT_3D]; \
	bool drawSweeps; \
	bool wireframe;

stemobject_struct_definition(SingleFrameScreen3D)

SingleFrameScreen3D * SingleFrameScreen3D_create(ResourceManager * resourceManager);
bool SingleFrameScreen3D_init(SingleFrameScreen3D * self, ResourceManager * resourceManager);
void SingleFrameScreen3D_dispose(SingleFrameScreen3D * self);

void SingleFrameScreen3D_activate(SingleFrameScreen3D * self);
void SingleFrameScreen3D_deactivate(SingleFrameScreen3D * self);

#ifdef __cplusplus
}
#endif
#endif
