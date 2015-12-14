/*
  Copyright (c) 2015 Alex Diener
  
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

#ifndef __Quaternionf_H__
#define __Quaternionf_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Quaternionf Quaternionf;

struct Quaternionf {
	float x;
	float y;
	float z;
	float w;
};

#include "gamemath/Matrix4x4f.h"
#include "gamemath/Vector3f.h"
#include "gamemath/Vector4f.h"

#define QUATERNIONf_IDENTITY ((Quaternionf) {0.0f, 0.0f, 0.0f, 1.0f})
#define QUATERNIONf(x, y, z, w) ((Quaternionf) {x, y, z, w})

void Quaternionf_loadIdentity(Quaternionf * quaternion);

Quaternionf Quaternionf_fromAxisAngle(Vector3f axis, float radians);
void Quaternionf_toAxisAngle(Quaternionf quaternion, Vector3f * outAxis, float * outRadians);
struct Matrix4x4f Quaternionf_toMatrix(Quaternionf quaternion);

void Quaternionf_normalize(Quaternionf * quaternion);
Quaternionf Quaternionf_normalized(Quaternionf quaternion);

void Quaternionf_multiply(Quaternionf * quaternion1, Quaternionf quaternion2);
Quaternionf Quaternionf_multiplied(Quaternionf quaternion1, Quaternionf quaternion2);
Quaternionf Quaternionf_slerp(Quaternionf left, Quaternionf right, float value);

void Quaternionf_rotate(Quaternionf * quaternion, Vector3f axis, float radians);
Quaternionf Quaternionf_rotated(Quaternionf quaternion, Vector3f axis, float radians);

void Quaternionf_invert(Quaternionf * quaternion);
Quaternionf Quaternionf_inverted(Quaternionf quaternion);

Vector3f Quaternionf_multiplyVector3f(Quaternionf quaternion, Vector3f vector);
Vector4f Quaternionf_multiplyVector4f(Quaternionf quaternion, Vector4f vector);

#ifdef __cplusplus
}
#endif
#endif
