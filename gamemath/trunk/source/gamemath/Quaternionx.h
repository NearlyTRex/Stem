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

#ifndef __Quaternionx_H__
#define __Quaternionx_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gamemath/FixedPoint.h"

typedef struct Quaternionx Quaternionx;

struct Quaternionx {
	fixed16_16 x;
	fixed16_16 y;
	fixed16_16 z;
	fixed16_16 w;
};

#include "gamemath/Matrix4x4x.h"
#include "gamemath/Vector3x.h"
#include "gamemath/Vector4x.h"

#define QUATERNIONx_IDENTITY ((Quaternionx) {0x00000, 0x00000, 0x00000, 0x10000})
#define QUATERNIONx(x, y, z, w) ((Quaternionx) {x, y, z, w})

void Quaternionx_loadIdentity(Quaternionx * quaternion);

Quaternionx Quaternionx_fromAxisAngle(Vector3x axis, fixed16_16 radians);
void Quaternionx_toAxisAngle(Quaternionx quaternion, Vector3x * outAxis, fixed16_16 * outRadians);
struct Matrix4x4x Quaternionx_toMatrix(Quaternionx quaternion);

void Quaternionx_normalize(Quaternionx * quaternion);
Quaternionx Quaternionx_normalized(Quaternionx quaternion);

void Quaternionx_multiply(Quaternionx * quaternion1, Quaternionx quaternion2);
Quaternionx Quaternionx_multiplied(Quaternionx quaternion1, Quaternionx quaternion2);
Quaternionx Quaternionx_slerp(Quaternionx left, Quaternionx right, fixed16_16 value);

void Quaternionx_rotate(Quaternionx * quaternion, Vector3x axis, fixed16_16 radians);
Quaternionx Quaternionx_rotated(Quaternionx quaternion, Vector3x axis, fixed16_16 radians);

void Quaternionx_invert(Quaternionx * quaternion);
Quaternionx Quaternionx_inverted(Quaternionx quaternion);

Vector3x Quaternionx_multiplyVector3x(Quaternionx quaternion, Vector3x vector);
Vector4x Quaternionx_multiplyVector4x(Quaternionx quaternion, Vector4x vector);

#ifdef __cplusplus
}
#endif
#endif
