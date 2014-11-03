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
  
  Alex Diener alex@ludobloom.com
*/

#ifndef __Matrix_H__
#define __Matrix_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Matrix Matrix;

struct Matrix {
	float m[16];
};

#include "gamemath/Vector2f.h"
#include "gamemath/Vector3f.h"
#include "gamemath/Vector4f.h"

#define MATRIX_IDENTITY ((Matrix) {{1.0f, 0.0f, 0.0f, 0.0f, \
                                    0.0f, 1.0f, 0.0f, 0.0f, \
                                    0.0f, 0.0f, 1.0f, 0.0f, \
                                    0.0f, 0.0f, 0.0f, 1.0f}})

void Matrix_loadIdentity(Matrix * matrix1);

#define MATRIX(m0, m4, m8,  m12, \
               m1, m5, m9,  m13, \
               m2, m6, m10, m14, \
               m3, m7, m11, m15) \
        ((Matrix) {{m0,  m1,  m2,  m3, \
                    m4,  m5,  m6,  m7, \
                    m8,  m9,  m10, m11, \
                    m12, m13, m14, m15}})
Matrix Matrix_fromDirectionVectors(Vector3f right, Vector3f up, Vector3f front);

void Matrix_multiply(Matrix * matrix1, Matrix matrix2);
Matrix Matrix_multiplied(Matrix matrix1, Matrix matrix2);

void Matrix_translate(Matrix * matrix1, float x, float y, float z);
Matrix Matrix_translated(Matrix matrix1, float x, float y, float z);

void Matrix_scale(Matrix * matrix, float x, float y, float z);
Matrix Matrix_scaled(Matrix matrix, float x, float y, float z);

void Matrix_rotate(Matrix * matrix, struct Vector3f axis, float radians);
Matrix Matrix_rotated(Matrix matrix, struct Vector3f axis, float radians);

void Matrix_shearX(Matrix * matrix, float y, float z);
Matrix Matrix_shearedX(Matrix matrix, float y, float z);

void Matrix_shearY(Matrix * matrix, float x, float z);
Matrix Matrix_shearedY(Matrix matrix, float x, float z);

void Matrix_shearZ(Matrix * matrix, float x, float y);
Matrix Matrix_shearedZ(Matrix matrix, float x, float y);

void Matrix_applyPerspective(Matrix * matrix, float fovYDegrees, float aspect, float zNear, float zFar);
Matrix Matrix_perspective(Matrix matrix, float fovYDegrees, float aspect, float zNear, float zFar);

void Matrix_applyOrtho(Matrix * matrix, float left, float right, float bottom, float top, float zNear, float zFar);
Matrix Matrix_ortho(Matrix matrix, float left, float right, float bottom, float top, float zNear, float zFar);

void Matrix_transpose(Matrix * matrix);
Matrix Matrix_transposed(Matrix matrix);

float Matrix_determinant(Matrix matrix);

void Matrix_invert(Matrix * matrix);
Matrix Matrix_inverted(Matrix matrix);

void Matrix_interpolate(Matrix * left, Matrix right, float value);
Matrix Matrix_interpolated(Matrix left, Matrix right, float value);

Vector2f Matrix_multiplyVector2f(Matrix matrix, Vector2f vector);
Vector3f Matrix_multiplyVector3f(Matrix matrix, Vector3f vector);
Vector4f Matrix_multiplyVector4f(Matrix matrix, Vector4f vector);

Vector3f Matrix_multiplyVector3f_rotationOnly(Matrix matrix, Vector3f vector);

#ifdef __cplusplus
}
#endif
#endif
