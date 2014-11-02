#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include <stdbool.h>

typedef struct Frustum Frustum;

#include "Vector.h"

struct Frustum {
	Vector4 left;
	Vector4 right;
	Vector4 bottom;
	Vector4 top;
	Vector4 front;
	Vector4 back;
};

struct Matrix;

Frustum Frustum_fromMatrix(struct Matrix matrix);
bool Frustum_containsSphere(Frustum frustum, Vector3 spherePosition, float sphereRadius);
float Frustum_sphereDistanceFromNearClippingPlane(Frustum frustum, Vector3 spherePosition, float sphereRadius);

#endif
