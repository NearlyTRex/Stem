/*
  Copyright (c) 2010 Alex Diener
  
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

#include "glgraphics/Camera.h"

#include <stdio.h>
#include <stdlib.h>

Camera * Camera_create() {
	stemobject_create_implementation(Camera, init)
}

void Camera_init(compat_type(Camera *) selfPtr) {
	Camera * self = selfPtr;
	
	StemObject_init(self);
	
	self->orientation = Quaternion_identity();
	self->position = Vector3_zero();
	
	self->dispose = Camera_dispose;
	self->getMatrix = Camera_getMatrix;
}

void Camera_dispose(compat_type(Camera *) selfPtr) {
	StemObject_dispose(selfPtr);
}

Matrix Camera_getMatrix(compat_type(Camera *) selfPtr) {
	Camera * self = selfPtr;
	Matrix matrix;
	
	matrix = Quaternion_toMatrix(Quaternion_inverted(self->orientation));
	Matrix_translate(&matrix, -self->position.x, -self->position.y, -self->position.z);
	return matrix;
}
