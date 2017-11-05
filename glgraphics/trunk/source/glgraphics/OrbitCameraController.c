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

#include "glgraphics/OrbitCameraController.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

OrbitCameraController * OrbitCameraController_create() {
	stemobject_create_implementation(OrbitCameraController, init)
}

bool OrbitCameraController_init(OrbitCameraController * self) {
	call_super(init, self);
	self->dispose = OrbitCameraController_dispose;
	self->cameraFocus = VECTOR3f_ZERO;
	self->cameraDirection = QUATERNIONf_IDENTITY;
	self->cameraDistance = 5.0f;
	self->minDistance = 1.0f;
	self->maxDistance = 100.0f;
	self->rotateSensitivity = 0.0078125f;
	self->offsetSensitivity = 0.001953125f;
	self->zoomSensitivity = 0.001953125f;
	return true;
}

void OrbitCameraController_dispose(OrbitCameraController * self) {
	call_super(dispose, self);
}

void OrbitCameraController_rotate(OrbitCameraController * self, float offsetX, float offsetY) {
	Quaternionf_rotate(&self->cameraDirection, VECTOR3f_UP, offsetX * self->rotateSensitivity);
	Quaternionf_rotate(&self->cameraDirection, Quaternionf_multiplyVector3f(Quaternionf_inverted(self->cameraDirection), VECTOR3f_RIGHT), offsetY * self->rotateSensitivity);
}

void OrbitCameraController_offset(OrbitCameraController * self, float offsetX, float offsetY, float offsetZ) {
	Vector3f offset;
	
	offset.x = offsetX * self->offsetSensitivity * self->cameraDistance;
	offset.y = offsetY * self->offsetSensitivity * self->cameraDistance;
	offset.z = offsetZ * self->offsetSensitivity * self->cameraDistance;
	offset = Quaternionf_multiplyVector3f(Quaternionf_inverted(self->cameraDirection), offset);
	self->cameraFocus = Vector3f_add(self->cameraFocus, offset);
}

void OrbitCameraController_zoom(OrbitCameraController * self, float offsetY) {
	self->cameraDistance += offsetY * self->zoomSensitivity * self->cameraDistance;
	if (self->cameraDistance < self->minDistance) {
		self->cameraDistance = self->minDistance;
	} else if (self->cameraDistance > self->maxDistance) {
		self->cameraDistance = self->maxDistance;
	}
}

Vector3f OrbitCameraController_getPosition(OrbitCameraController * self) {
	return Vector3f_add(Quaternionf_multiplyVector3f(self->cameraDirection, VECTOR3f(0.0f, 0.0f, -self->cameraDistance)), self->cameraFocus);
}

Matrix4x4f OrbitCameraController_getMatrix(OrbitCameraController * self) {
	Matrix4x4f matrix = MATRIX4x4f_IDENTITY;
	
	Matrix4x4f_translate(&matrix, 0.0f, 0.0f, -self->cameraDistance);
	Matrix4x4f_multiply(&matrix, Quaternionf_toMatrix(self->cameraDirection));
	Matrix4x4f_translate(&matrix, self->cameraFocus.x, self->cameraFocus.y, self->cameraFocus.z);
	return matrix;
}
