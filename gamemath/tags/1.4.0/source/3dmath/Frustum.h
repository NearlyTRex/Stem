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
