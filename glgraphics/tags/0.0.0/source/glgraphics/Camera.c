#include "glgraphics/Camera.h"

#include <stdio.h>
#include <stdlib.h>

#include "3dmath/Matrix.h"
#include "glgraphics/GLIncludes.h"

Camera * Camera_create() {
	Camera * self;
	
	self = malloc(sizeof(Camera));
	Camera_init(self);
	return self;
}

void Camera_init(Camera * self) {
	self->orientation = Quaternion_identity();
	self->position = Vector3_zero();
	
	self->dispose = Camera_dispose;
	self->apply = Camera_apply;
}

void Camera_dispose(void * selfPtr) {
	free(selfPtr);
}

void Camera_apply(void * selfPtr) {
	Camera * self = selfPtr;
	Matrix matrix;
	
	matrix = Quaternion_toMatrix(Quaternion_inverted(self->orientation));
	glMultMatrixf(matrix.m);
	glTranslatef(-self->position.x, -self->position.y, -self->position.z);
}
