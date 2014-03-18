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

#include "glgraphics/Camera.h"

#include <stdio.h>
#include <stdlib.h>

#define SUPERCLASS StemObject

Camera * Camera_create() {
	stemobject_create_implementation(Camera, init)
}

bool Camera_init(Camera * self) {
	call_super(init, self);
	
	self->orientation = QUATERNION_IDENTITY;
	self->position = VECTOR3f_ZERO;
	
	self->dispose = Camera_dispose;
	self->getMatrix = Camera_getMatrix;
	return true;
}

void Camera_dispose(Camera * self) {
	call_super(dispose, self);
}

Matrix Camera_getMatrix(Camera * self) {
	Matrix matrix;
	
	matrix = Quaternion_toMatrix(Quaternion_inverted(self->orientation));
	Matrix_translate(&matrix, -self->position.x, -self->position.y, -self->position.z);
	return matrix;
}
