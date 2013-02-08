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

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

typedef struct Quaternion Quaternion;

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

#include "3dmath/Matrix.h"
#include "3dmath/Vector.h"

#define QUATERNION_IDENTITY ((Quaternion) {0.0f, 0.0f, 0.0f, 1.0f})

void Quaternion_loadIdentity(Quaternion * quaternion);
Quaternion Quaternion_init(float x, float y, float z, float w);

Quaternion Quaternion_fromAxisAngle(Vector3 axis, float radians);
void Quaternion_toAxisAngle(Quaternion quaternion, Vector3 * outAxis, float * outRadians);
struct Matrix Quaternion_toMatrix(Quaternion quaternion);

void Quaternion_normalize(Quaternion * quaternion);
Quaternion Quaternion_normalized(Quaternion quaternion);

void Quaternion_multiply(Quaternion * quaternion1, Quaternion quaternion2);
Quaternion Quaternion_multiplied(Quaternion quaternion1, Quaternion quaternion2);
Quaternion Quaternion_slerp(Quaternion left, Quaternion right, float value);

void Quaternion_rotate(Quaternion * quaternion, Vector3 axis, float radians);
Quaternion Quaternion_rotated(Quaternion quaternion, Vector3 axis, float radians);

void Quaternion_invert(Quaternion * quaternion);
Quaternion Quaternion_inverted(Quaternion quaternion);

Vector3 Quaternion_multiplyVector3(Quaternion quaternion, Vector3 vector);
Vector4 Quaternion_multiplyVector4(Quaternion quaternion, Vector4 vector);

#endif
