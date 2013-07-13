/*
  Copyright (c) 2013 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __CAMERA_H__
#define __CAMERA_H__

typedef struct Camera Camera;

#include "3dmath/Quaternion.h"
#include "3dmath/Matrix.h"
#include "3dmath/Vector.h"
#include "stemobject/StemObject.h"

#define Camera_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Quaternion orientation; \
	Vector3 position; \
	\
	Matrix (* getMatrix)(self_type * self);

stemobject_struct_definition(Camera)

Camera * Camera_create();
void Camera_init(Camera * self);

void Camera_dispose(Camera * self);
Matrix Camera_getMatrix(Camera * self);

#endif
