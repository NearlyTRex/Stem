#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct Matrix Matrix;

struct Matrix {
	float m[16];
};

#include "3dmath/Vector.h"

void Matrix_loadIdentity(Matrix * matrix1);
Matrix Matrix_identity();

Matrix Matrix_init(float m0, float m4, float m8,  float m12,
                   float m1, float m5, float m9,  float m13,
                   float m2, float m6, float m10, float m14,
                   float m3, float m7, float m11, float m15);
Matrix Matrix_fromDirectionVectors(Vector3 right, Vector3 up, Vector3 front);

void Matrix_multiply(Matrix * matrix1, Matrix matrix2);
Matrix Matrix_multiplied(Matrix matrix1, Matrix matrix2);

void Matrix_translate(Matrix * matrix1, float x, float y, float z);
Matrix Matrix_translated(Matrix matrix1, float x, float y, float z);

void Matrix_scale(Matrix * matrix, float x, float y, float z);
Matrix Matrix_scaled(Matrix matrix, float x, float y, float z);

void Matrix_rotate(Matrix * matrix, struct Vector3 axis, float angle);
Matrix Matrix_rotated(Matrix matrix, struct Vector3 axis, float angle);

void Matrix_shearX(Matrix * matrix, float y, float z);
Matrix Matrix_shearedX(Matrix matrix, float y, float z);

void Matrix_shearY(Matrix * matrix, float x, float z);
Matrix Matrix_shearedY(Matrix matrix, float x, float z);

void Matrix_shearZ(Matrix * matrix, float x, float y);
Matrix Matrix_shearedZ(Matrix matrix, float x, float y);

void Matrix_applyPerspective(Matrix * matrix, float fovY, float aspect, float zNear, float zFar);
Matrix Matrix_perspective(Matrix matrix, float fovY, float aspect, float zNear, float zFar);

void Matrix_transpose(Matrix * matrix);
Matrix Matrix_transposed(Matrix matrix);

float Matrix_determinant(Matrix matrix);

void Matrix_invert(Matrix * matrix);
Matrix Matrix_inverted(Matrix matrix);

Vector2 Matrix_multiplyVector2(Matrix matrix, Vector2 vector);
Vector3 Matrix_multiplyVector3(Matrix matrix, Vector3 vector);
Vector4 Matrix_multiplyVector4(Matrix matrix, Vector4 vector);

Vector3 Matrix_multiplyVector3_rotationOnly(Matrix matrix, Vector3 vector);

#endif
