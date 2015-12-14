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

#ifndef __Matrix4x4x_H__
#define __Matrix4x4x_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gamemath/FixedPoint.h"

typedef struct Matrix4x4x Matrix4x4x;

struct Matrix4x4x {
	fixed16_16 m[16];
};

#include "gamemath/Vector2x.h"
#include "gamemath/Vector3x.h"
#include "gamemath/Vector4x.h"

#define MATRIX4x4x_IDENTITY ((Matrix4x4x) {{0x10000, 0x00000, 0x00000, 0x00000, \
                                            0x00000, 0x10000, 0x00000, 0x00000, \
                                            0x00000, 0x00000, 0x10000, 0x00000, \
                                            0x00000, 0x00000, 0x00000, 0x10000}})

void Matrix4x4x_loadIdentity(Matrix4x4x * matrix1);

#define MATRIX4x4x(m0, m4, m8,  m12, \
                   m1, m5, m9,  m13, \
                   m2, m6, m10, m14, \
                   m3, m7, m11, m15) \
        ((Matrix4x4x) {{m0,  m1,  m2,  m3, \
                        m4,  m5,  m6,  m7, \
                        m8,  m9,  m10, m11, \
                        m12, m13, m14, m15}})
Matrix4x4x Matrix4x4x_fromDirectionVectors(Vector3x right, Vector3x up, Vector3x front);

void Matrix4x4x_multiply(Matrix4x4x * matrix1, Matrix4x4x matrix2);
Matrix4x4x Matrix4x4x_multiplied(Matrix4x4x matrix1, Matrix4x4x matrix2);

void Matrix4x4x_translate(Matrix4x4x * matrix1, fixed16_16 x, fixed16_16 y, fixed16_16 z);
Matrix4x4x Matrix4x4x_translated(Matrix4x4x matrix1, fixed16_16 x, fixed16_16 y, fixed16_16 z);

void Matrix4x4x_scale(Matrix4x4x * matrix, fixed16_16 x, fixed16_16 y, fixed16_16 z);
Matrix4x4x Matrix4x4x_scaled(Matrix4x4x matrix, fixed16_16 x, fixed16_16 y, fixed16_16 z);

void Matrix4x4x_rotate(Matrix4x4x * matrix, struct Vector3x axis, fixed16_16 radians);
Matrix4x4x Matrix4x4x_rotated(Matrix4x4x matrix, struct Vector3x axis, fixed16_16 radians);

void Matrix4x4x_shearX(Matrix4x4x * matrix, fixed16_16 y, fixed16_16 z);
Matrix4x4x Matrix4x4x_shearedX(Matrix4x4x matrix, fixed16_16 y, fixed16_16 z);

void Matrix4x4x_shearY(Matrix4x4x * matrix, fixed16_16 x, fixed16_16 z);
Matrix4x4x Matrix4x4x_shearedY(Matrix4x4x matrix, fixed16_16 x, fixed16_16 z);

void Matrix4x4x_shearZ(Matrix4x4x * matrix, fixed16_16 x, fixed16_16 y);
Matrix4x4x Matrix4x4x_shearedZ(Matrix4x4x matrix, fixed16_16 x, fixed16_16 y);

void Matrix4x4x_applyPerspective(Matrix4x4x * matrix, fixed16_16 fovYDegrees, fixed16_16 aspect, fixed16_16 zNear, fixed16_16 zFar);
Matrix4x4x Matrix4x4x_perspective(Matrix4x4x matrix, fixed16_16 fovYDegrees, fixed16_16 aspect, fixed16_16 zNear, fixed16_16 zFar);

void Matrix4x4x_applyOrtho(Matrix4x4x * matrix, fixed16_16 left, fixed16_16 right, fixed16_16 bottom, fixed16_16 top, fixed16_16 zNear, fixed16_16 zFar);
Matrix4x4x Matrix4x4x_ortho(Matrix4x4x matrix, fixed16_16 left, fixed16_16 right, fixed16_16 bottom, fixed16_16 top, fixed16_16 zNear, fixed16_16 zFar);

void Matrix4x4x_transpose(Matrix4x4x * matrix);
Matrix4x4x Matrix4x4x_transposed(Matrix4x4x matrix);

fixed16_16 Matrix4x4x_determinant(Matrix4x4x matrix);

void Matrix4x4x_invert(Matrix4x4x * matrix);
Matrix4x4x Matrix4x4x_inverted(Matrix4x4x matrix);

void Matrix4x4x_interpolate(Matrix4x4x * left, Matrix4x4x right, fixed16_16 value);
Matrix4x4x Matrix4x4x_interpolated(Matrix4x4x left, Matrix4x4x right, fixed16_16 value);

Vector2x Matrix4x4x_multiplyVector2x(Matrix4x4x matrix, Vector2x vector);
Vector3x Matrix4x4x_multiplyVector3x(Matrix4x4x matrix, Vector3x vector);
Vector4x Matrix4x4x_multiplyVector4x(Matrix4x4x matrix, Vector4x vector);

Vector3x Matrix4x4x_multiplyVector3x_rotationOnly(Matrix4x4x matrix, Vector3x vector);

#ifdef __cplusplus
}
#endif
#endif
