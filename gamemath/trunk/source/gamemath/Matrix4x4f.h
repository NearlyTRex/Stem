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

#ifndef __Matrix4x4f_H__
#define __Matrix4x4f_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Matrix4x4f Matrix4x4f;

struct Matrix4x4f {
	float m[16];
};

#include "gamemath/Vector2f.h"
#include "gamemath/Vector3f.h"
#include "gamemath/Vector4f.h"

#define MATRIX4x4f_IDENTITY ((Matrix4x4f) {{1.0f, 0.0f, 0.0f, 0.0f, \
                                            0.0f, 1.0f, 0.0f, 0.0f, \
                                            0.0f, 0.0f, 1.0f, 0.0f, \
                                            0.0f, 0.0f, 0.0f, 1.0f}})

void Matrix4x4f_loadIdentity(Matrix4x4f * matrix1);

#define MATRIX4x4f(m0, m4, m8,  m12, \
                   m1, m5, m9,  m13, \
                   m2, m6, m10, m14, \
                   m3, m7, m11, m15) \
        ((Matrix4x4f) {{m0,  m1,  m2,  m3, \
                        m4,  m5,  m6,  m7, \
                        m8,  m9,  m10, m11, \
                        m12, m13, m14, m15}})
Matrix4x4f Matrix4x4f_fromDirectionVectors(Vector3f right, Vector3f up, Vector3f front);

void Matrix4x4f_multiply(Matrix4x4f * matrix1, Matrix4x4f matrix2);
Matrix4x4f Matrix4x4f_multiplied(Matrix4x4f matrix1, Matrix4x4f matrix2);

void Matrix4x4f_translate(Matrix4x4f * matrix1, float x, float y, float z);
Matrix4x4f Matrix4x4f_translated(Matrix4x4f matrix1, float x, float y, float z);

void Matrix4x4f_scale(Matrix4x4f * matrix, float x, float y, float z);
Matrix4x4f Matrix4x4f_scaled(Matrix4x4f matrix, float x, float y, float z);

void Matrix4x4f_rotate(Matrix4x4f * matrix, struct Vector3f axis, float radians);
Matrix4x4f Matrix4x4f_rotated(Matrix4x4f matrix, struct Vector3f axis, float radians);

void Matrix4x4f_shearX(Matrix4x4f * matrix, float y, float z);
Matrix4x4f Matrix4x4f_shearedX(Matrix4x4f matrix, float y, float z);

void Matrix4x4f_shearY(Matrix4x4f * matrix, float x, float z);
Matrix4x4f Matrix4x4f_shearedY(Matrix4x4f matrix, float x, float z);

void Matrix4x4f_shearZ(Matrix4x4f * matrix, float x, float y);
Matrix4x4f Matrix4x4f_shearedZ(Matrix4x4f matrix, float x, float y);

void Matrix4x4f_applyPerspective(Matrix4x4f * matrix, float fovYDegrees, float aspect, float zNear, float zFar);
Matrix4x4f Matrix4x4f_perspective(Matrix4x4f matrix, float fovYDegrees, float aspect, float zNear, float zFar);

void Matrix4x4f_applyOrtho(Matrix4x4f * matrix, float left, float right, float bottom, float top, float zNear, float zFar);
Matrix4x4f Matrix4x4f_ortho(Matrix4x4f matrix, float left, float right, float bottom, float top, float zNear, float zFar);

void Matrix4x4f_transpose(Matrix4x4f * matrix);
Matrix4x4f Matrix4x4f_transposed(Matrix4x4f matrix);

float Matrix4x4f_determinant(Matrix4x4f matrix);

void Matrix4x4f_invert(Matrix4x4f * matrix);
Matrix4x4f Matrix4x4f_inverted(Matrix4x4f matrix);

void Matrix4x4f_interpolate(Matrix4x4f * left, Matrix4x4f right, float value);
Matrix4x4f Matrix4x4f_interpolated(Matrix4x4f left, Matrix4x4f right, float value);

Vector2f Matrix4x4f_multiplyVector2f(Matrix4x4f matrix, Vector2f vector);
Vector3f Matrix4x4f_multiplyVector3f(Matrix4x4f matrix, Vector3f vector);
Vector4f Matrix4x4f_multiplyVector4f(Matrix4x4f matrix, Vector4f vector);

Vector3f Matrix4x4f_multiplyVector3f_rotationOnly(Matrix4x4f matrix, Vector3f vector);

#ifdef __cplusplus
}
#endif
#endif
