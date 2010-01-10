#ifndef __CAMERA_H__
#define __CAMERA_H__

typedef struct Camera Camera;

#include "3dmath/Quaternion.h"
#include "3dmath/Vector.h"

#define Camera_structContents \
	\
	Quaternion orientation; \
	Vector3 position; \
	\
	void (* dispose)(void * self); \
	void (* apply)(void * self);

struct Camera {
	Camera_structContents
};

Camera * Camera_create();
void Camera_init(Camera * self);

void Camera_dispose(void * selfPtr);
void Camera_apply(void * selfPtr);

#endif
