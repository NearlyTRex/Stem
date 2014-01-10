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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __Vector_legacy_H__
#define __Vector_legacy_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "3dmath/Vector2f.h"
#include "3dmath/Vector3f.h"
#include "3dmath/Vector4f.h"

#define Vector2 Vector2f
#define Vector3 Vector3f
#define Vector4 Vector4f

#define VECTOR2_ZERO VECTOR2f_ZERO
#define VECTOR2_LEFT  VECTOR2f_LEFT
#define VECTOR2_RIGHT VECTOR2f_RIGHT
#define VECTOR2_DOWN  VECTOR2f_DOWN
#define VECTOR2_UP    VECTOR2f_UP

#define VECTOR3_ZERO VECTOR3f_ZERO
#define VECTOR3_LEFT  VECTOR3f_LEFT
#define VECTOR3_RIGHT VECTOR3f_RIGHT
#define VECTOR3_DOWN  VECTOR3f_DOWN
#define VECTOR3_UP    VECTOR3f_UP
#define VECTOR3_BACK  VECTOR3f_BACK
#define VECTOR3_FRONT VECTOR3f_FRONT

#define VECTOR4_ZERO VECTOR4f_ZERO
#define VECTOR4_LEFT  VECTOR4f_LEFT
#define VECTOR4_RIGHT VECTOR4f_RIGHT
#define VECTOR4_DOWN  VECTOR4f_DOWN
#define VECTOR4_UP    VECTOR4f_UP
#define VECTOR4_BACK  VECTOR4f_BACK
#define VECTOR4_FRONT VECTOR4f_FRONT
#define VECTOR4_KATA  VECTOR4f_KATA
#define VECTOR4_ANA   VECTOR4f_ANA

#define Vector2_init(x, y) VECTOR2f(x, y)
#define Vector3_init(x, y, z) VECTOR3f(x, y, z)
#define Vector4_init(x, y, z, w) VECTOR4f(x, y, z, w)

#define Vector2_normalize(vector) Vector2f_normalize(vector)
#define Vector3_normalize(vector) Vector3f_normalize(vector)
#define Vector4_normalize(vector) Vector4f_normalize(vector)
#define Vector2_normalized(vector) Vector2f_normalized(vector)
#define Vector3_normalized(vector) Vector3f_normalized(vector)
#define Vector4_normalized(vector) Vector4f_normalized(vector)

#define Vector2_scaleTo(vector, magnitude) Vector2f_scaleTo(vector, magnitude)
#define Vector3_scaleTo(vector, magnitude) Vector3f_scaleTo(vector, magnitude)
#define Vector4_scaleTo(vector, magnitude) Vector4f_scaleTo(vector, magnitude)
#define Vector2_scaledTo(vector, magnitude) Vector2f_scaledTo(vector, magnitude)
#define Vector3_scaledTo(vector, magnitude) Vector3f_scaledTo(vector, magnitude)
#define Vector4_scaledTo(vector, magnitude) Vector4f_scaledTo(vector, magnitude)

#define Vector2_magnitude(vector) Vector2f_magnitude(vector)
#define Vector3_magnitude(vector) Vector3f_magnitude(vector)
#define Vector4_magnitude(vector) Vector4f_magnitude(vector)
#define Vector2_magnitudeSquared(vector) Vector2f_magnitudeSquared(vector)
#define Vector3_magnitudeSquared(vector) Vector3f_magnitudeSquared(vector)
#define Vector4_magnitudeSquared(vector) Vector4f_magnitudeSquared(vector)

#define Vector2_add(vector1, vector2) Vector2f_add(vector1, vector2)
#define Vector3_add(vector1, vector2) Vector3f_add(vector1, vector2)
#define Vector4_add(vector1, vector2) Vector4f_add(vector1, vector2)
#define Vector2_subtract(vector1, vector2) Vector2f_subtract(vector1, vector2)
#define Vector3_subtract(vector1, vector2) Vector3f_subtract(vector1, vector2)
#define Vector4_subtract(vector1, vector2) Vector4f_subtract(vector1, vector2)
#define Vector2_multiplyScalar(vector, scalar) Vector2f_multiplyScalar(vector, scalar)
#define Vector3_multiplyScalar(vector, scalar) Vector3f_multiplyScalar(vector, scalar)
#define Vector4_multiplyScalar(vector, scalar) Vector4f_multiplyScalar(vector, scalar)
#define Vector2_divideScalar(vector, scalar) Vector2f_divideScalar(vector, scalar)
#define Vector3_divideScalar(vector, scalar) Vector3f_divideScalar(vector, scalar)
#define Vector4_divideScalar(vector, scalar) Vector4f_divideScalar(vector, scalar)

#define Vector2_interpolate(left, right, value) Vector2f_interpolate(left, right, value)
#define Vector3_interpolate(left, right, value) Vector3f_interpolate(left, right, value)
#define Vector4_interpolate(left, right, value) Vector4f_interpolate(left, right, value)

#define Vector2_reflect(vector, normal) Vector2f_reflect(vector, normal)
#define Vector3_reflect(vector, normal) Vector3f_reflect(vector, normal)
#define Vector4_reflect(vector, normal) Vector4f_reflect(vector, normal)

#define Vector2_rotate(vector, radians) Vector2f_rotate(vector, radians)

#define Vector2_dot(vector1, vector2) Vector2f_dot(vector1, vector2)
#define Vector3_dot(vector1, vector2) Vector3f_dot(vector1, vector2)
#define Vector4_dot(vector1, vector2) Vector4f_dot(vector1, vector2)
#define Vector2_cross(vector1, vector2) Vector2f_cross(vector1, vector2)
#define Vector3_cross(vector1, vector2) Vector3f_cross(vector1, vector2)

#ifdef __cplusplus
}
#endif
#endif
