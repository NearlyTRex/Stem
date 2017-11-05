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

#ifndef __OrbitCameraController_H__
#define __OrbitCameraController_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct OrbitCameraController OrbitCameraController;

#include "gamemath/Matrix4x4f.h"
#include "gamemath/Quaternionf.h"
#include "gamemath/Vector3f.h"
#include "stemobject/StemObject.h"

#define OrbitCameraController_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Vector3f cameraFocus; \
	Quaternionf cameraDirection; \
	float cameraDistance; \
	float minDistance; \
	float maxDistance; \
	float rotateSensitivity; \
	float offsetSensitivity; \
	float zoomSensitivity;

stemobject_struct_definition(OrbitCameraController)

OrbitCameraController * OrbitCameraController_create();
bool OrbitCameraController_init(OrbitCameraController * self);
void OrbitCameraController_dispose(OrbitCameraController * self);

void OrbitCameraController_rotate(OrbitCameraController * self, float offsetX, float offsetY);
void OrbitCameraController_offset(OrbitCameraController * self, float offsetX, float offsetY, float offsetZ);
void OrbitCameraController_zoom(OrbitCameraController * self, float offsetY);
Vector3f OrbitCameraController_getPosition(OrbitCameraController * self);
Matrix4x4f OrbitCameraController_getMatrix(OrbitCameraController * self);

#ifdef __cplusplus
}
#endif
#endif
