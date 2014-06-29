/*
  Copyright (c) 2014 Alex Diener
  
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

#ifndef __Camera_H__
#define __Camera_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Camera Camera;

#include "3dmath/Quaternion.h"
#include "3dmath/Matrix.h"
#include "3dmath/Vector3f.h"
#include "stemobject/StemObject.h"

#define Camera_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Quaternion orientation; \
	Vector3f position; \
	\
	Matrix (* getMatrix)(self_type * self);

stemobject_struct_definition(Camera)

Camera * Camera_create();
bool Camera_init(Camera * self);

void Camera_dispose(Camera * self);
Matrix Camera_getMatrix(Camera * self);

#ifdef __cplusplus
}
#endif
#endif
