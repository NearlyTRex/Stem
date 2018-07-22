/*
  Copyright (c) 2018 Alex Diener
  
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

#include "gamemath/Matrix4x4f.h"
#include "gamemath/Quaternionf.h"
#include <math.h>

void Matrix4x4f_loadIdentity(Matrix4x4f * matrix) {
	matrix->m[0] = 1.0f;
	matrix->m[1] = 0.0f;
	matrix->m[2] = 0.0f;
	matrix->m[3] = 0.0f;
	matrix->m[4] = 0.0f;
	matrix->m[5] = 1.0f;
	matrix->m[6] = 0.0f;
	matrix->m[7] = 0.0f;
	matrix->m[8] = 0.0f;
	matrix->m[9] = 0.0f;
	matrix->m[10] = 1.0f;
	matrix->m[11] = 0.0f;
	matrix->m[12] = 0.0f;
	matrix->m[13] = 0.0f;
	matrix->m[14] = 0.0f;
	matrix->m[15] = 1.0f;
}

Matrix4x4f Matrix4x4f_fromDirectionVectors(Vector3f right, Vector3f up, Vector3f front) {
	Matrix4x4f matrix = MATRIX4x4f_IDENTITY;
	
	matrix.m[0]  = right.x;
	matrix.m[1]  = right.y;
	matrix.m[2]  = right.z;
	matrix.m[4]  = up.x;
	matrix.m[5]  = up.y;
	matrix.m[6]  = up.z;
	matrix.m[8]  = front.x;
	matrix.m[9]  = front.y;
	matrix.m[10] = front.z;
	return matrix;
}

Matrix4x4f Matrix4x4f_multiplied(Matrix4x4f m1, Matrix4x4f m2) {
	Matrix4x4f result;
	
	result.m[0]  = m1.m[0] * m2.m[0]  + m1.m[4] * m2.m[1]  + m1.m[8]  * m2.m[2]  + m1.m[12] * m2.m[3];
	result.m[1]  = m1.m[1] * m2.m[0]  + m1.m[5] * m2.m[1]  + m1.m[9]  * m2.m[2]  + m1.m[13] * m2.m[3];
	result.m[2]  = m1.m[2] * m2.m[0]  + m1.m[6] * m2.m[1]  + m1.m[10] * m2.m[2]  + m1.m[14] * m2.m[3];
	result.m[3]  = m1.m[3] * m2.m[0]  + m1.m[7] * m2.m[1]  + m1.m[11] * m2.m[2]  + m1.m[15] * m2.m[3];
	result.m[4]  = m1.m[0] * m2.m[4]  + m1.m[4] * m2.m[5]  + m1.m[8]  * m2.m[6]  + m1.m[12] * m2.m[7];
	result.m[5]  = m1.m[1] * m2.m[4]  + m1.m[5] * m2.m[5]  + m1.m[9]  * m2.m[6]  + m1.m[13] * m2.m[7];
	result.m[6]  = m1.m[2] * m2.m[4]  + m1.m[6] * m2.m[5]  + m1.m[10] * m2.m[6]  + m1.m[14] * m2.m[7];
	result.m[7]  = m1.m[3] * m2.m[4]  + m1.m[7] * m2.m[5]  + m1.m[11] * m2.m[6]  + m1.m[15] * m2.m[7];
	result.m[8]  = m1.m[0] * m2.m[8]  + m1.m[4] * m2.m[9]  + m1.m[8]  * m2.m[10] + m1.m[12] * m2.m[11];
	result.m[9]  = m1.m[1] * m2.m[8]  + m1.m[5] * m2.m[9]  + m1.m[9]  * m2.m[10] + m1.m[13] * m2.m[11];
	result.m[10] = m1.m[2] * m2.m[8]  + m1.m[6] * m2.m[9]  + m1.m[10] * m2.m[10] + m1.m[14] * m2.m[11];
	result.m[11] = m1.m[3] * m2.m[8]  + m1.m[7] * m2.m[9]  + m1.m[11] * m2.m[10] + m1.m[15] * m2.m[11];
	result.m[12] = m1.m[0] * m2.m[12] + m1.m[4] * m2.m[13] + m1.m[8]  * m2.m[14] + m1.m[12] * m2.m[15];
	result.m[13] = m1.m[1] * m2.m[12] + m1.m[5] * m2.m[13] + m1.m[9]  * m2.m[14] + m1.m[13] * m2.m[15];
	result.m[14] = m1.m[2] * m2.m[12] + m1.m[6] * m2.m[13] + m1.m[10] * m2.m[14] + m1.m[14] * m2.m[15];
	result.m[15] = m1.m[3] * m2.m[12] + m1.m[7] * m2.m[13] + m1.m[11] * m2.m[14] + m1.m[15] * m2.m[15];
	
	return result;
}

void Matrix4x4f_multiply(Matrix4x4f * matrix1, Matrix4x4f matrix2) {
	*matrix1 = Matrix4x4f_multiplied(*matrix1, matrix2);
}

void Matrix4x4f_leftMultiply(Matrix4x4f * matrix1, Matrix4x4f matrix2) {
	*matrix1 = Matrix4x4f_multiplied(matrix2, *matrix1);
}

Matrix4x4f Matrix4x4f_translationMatrix(float x, float y, float z) {
	Matrix4x4f translationMatrix = MATRIX4x4f_IDENTITY;
	
	translationMatrix.m[12] = x;
	translationMatrix.m[13] = y;
	translationMatrix.m[14] = z;
	return translationMatrix;
}

Matrix4x4f Matrix4x4f_scaleMatrix(float x, float y, float z) {
	Matrix4x4f scaleMatrix = MATRIX4x4f_IDENTITY;
	
	scaleMatrix.m[0] = x;
	scaleMatrix.m[5] = y;
	scaleMatrix.m[10] = z;
	return scaleMatrix;
}

Matrix4x4f Matrix4x4f_rotationMatrix(struct Vector3f axis, float radians) {
	return Quaternionf_toMatrix(Quaternionf_fromAxisAngle(axis, radians));
}

Matrix4x4f Matrix4x4f_shearXMatrix(float y, float z) {
	Matrix4x4f shearMatrix = MATRIX4x4f_IDENTITY;
	
	shearMatrix.m[1] = y;
	shearMatrix.m[2] = z;
	return shearMatrix;
}

Matrix4x4f Matrix4x4f_shearYMatrix(float x, float z) {
	Matrix4x4f shearMatrix = MATRIX4x4f_IDENTITY;
	
	shearMatrix.m[4] = x;
	shearMatrix.m[6] = z;
	return shearMatrix;
}

Matrix4x4f Matrix4x4f_shearZMatrix(float x, float y) {
	Matrix4x4f shearMatrix = MATRIX4x4f_IDENTITY;
	
	shearMatrix.m[8] = x;
	shearMatrix.m[9] = y;
	return shearMatrix;
}

Matrix4x4f Matrix4x4f_perspectiveMatrix(float fovYDegrees, float aspect, float zNear, float zFar) {
	Matrix4x4f perspectiveMatrix = MATRIX4x4f_IDENTITY;
	float sine, cotangent, deltaZ;
	
	fovYDegrees = fovYDegrees * (float) M_PI / 360.0f;
	deltaZ = zFar - zNear;
	sine = sinf(fovYDegrees);
	if (deltaZ != 0.0f && sine != 0.0f && aspect != 0.0f) {
		cotangent = cos(fovYDegrees) / sine;
	
		perspectiveMatrix.m[0] = cotangent / aspect;
		perspectiveMatrix.m[5] = cotangent;
		perspectiveMatrix.m[10] = -(zFar + zNear) / deltaZ;
		perspectiveMatrix.m[11] = -1.0f;
		perspectiveMatrix.m[14] = -2.0f * zNear * zFar / deltaZ;
		perspectiveMatrix.m[15] = 0.0f;
	}
	return perspectiveMatrix;
}

Matrix4x4f Matrix4x4f_orthoMatrix(float left, float right, float bottom, float top, float zNear, float zFar) {
	Matrix4x4f orthoMatrix = MATRIX4x4f_IDENTITY;
	
	orthoMatrix.m[0] = 2.0f / (right - left);
	orthoMatrix.m[5] = 2.0f / (top - bottom);
	orthoMatrix.m[10] = -2.0f / (zFar - zNear);
	orthoMatrix.m[12] = -((right + left) / (right - left));
	orthoMatrix.m[13] = -((top + bottom) / (top - bottom));
	orthoMatrix.m[14] = -((zFar + zNear) / (zFar - zNear));
	return orthoMatrix;
}

void Matrix4x4f_translate(Matrix4x4f * matrix, float x, float y, float z) {
	*matrix = Matrix4x4f_multiplied(*matrix, Matrix4x4f_translationMatrix(x, y, z));
}

void Matrix4x4f_scale(Matrix4x4f * matrix, float x, float y, float z) {
	*matrix = Matrix4x4f_multiplied(*matrix, Matrix4x4f_scaleMatrix(x, y, z));
}

void Matrix4x4f_rotate(Matrix4x4f * matrix, Vector3f axis, float radians) {
	*matrix = Matrix4x4f_multiplied(*matrix, Matrix4x4f_rotationMatrix(axis, radians));
}

void Matrix4x4f_shearX(Matrix4x4f * matrix, float y, float z) {
	*matrix = Matrix4x4f_multiplied(*matrix, Matrix4x4f_shearXMatrix(y, z));
}

void Matrix4x4f_shearY(Matrix4x4f * matrix, float x, float z) {
	*matrix = Matrix4x4f_multiplied(*matrix, Matrix4x4f_shearYMatrix(x, z));
}

void Matrix4x4f_shearZ(Matrix4x4f * matrix, float x, float y) {
	*matrix = Matrix4x4f_multiplied(*matrix, Matrix4x4f_shearZMatrix(x, y));
}

void Matrix4x4f_applyPerspective(Matrix4x4f * matrix, float fovYDegrees, float aspect, float zNear, float zFar) {
	*matrix = Matrix4x4f_multiplied(*matrix, Matrix4x4f_perspectiveMatrix(fovYDegrees, aspect, zNear, zFar));
}

void Matrix4x4f_applyOrtho(Matrix4x4f * matrix, float left, float right, float bottom, float top, float zNear, float zFar) {
	*matrix = Matrix4x4f_multiplied(*matrix, Matrix4x4f_orthoMatrix(left, right, bottom, top, zNear, zFar));
}

Matrix4x4f Matrix4x4f_translated(Matrix4x4f matrix, float x, float y, float z) {
	return Matrix4x4f_multiplied(matrix, Matrix4x4f_translationMatrix(x, y, z));
}

Matrix4x4f Matrix4x4f_scaled(Matrix4x4f matrix, float x, float y, float z) {
	return Matrix4x4f_multiplied(matrix, Matrix4x4f_scaleMatrix(x, y, z));
}

Matrix4x4f Matrix4x4f_rotated(Matrix4x4f matrix, Vector3f axis, float radians) {
	return Matrix4x4f_multiplied(matrix, Matrix4x4f_rotationMatrix(axis, radians));
}

Matrix4x4f Matrix4x4f_shearedX(Matrix4x4f matrix, float y, float z) {
	return Matrix4x4f_multiplied(matrix, Matrix4x4f_shearXMatrix(y, z));
}

Matrix4x4f Matrix4x4f_shearedY(Matrix4x4f matrix, float x, float z) {
	return Matrix4x4f_multiplied(matrix, Matrix4x4f_shearYMatrix(x, z));
}

Matrix4x4f Matrix4x4f_shearedZ(Matrix4x4f matrix, float x, float y) {
	return Matrix4x4f_multiplied(matrix, Matrix4x4f_shearZMatrix(x, y));
}

Matrix4x4f Matrix4x4f_perspective(Matrix4x4f matrix, float fovYDegrees, float aspect, float zNear, float zFar) {
	return Matrix4x4f_multiplied(matrix, Matrix4x4f_perspectiveMatrix(fovYDegrees, aspect, zNear, zFar));
}

Matrix4x4f Matrix4x4f_ortho(Matrix4x4f matrix, float left, float right, float bottom, float top, float zNear, float zFar) {
	return Matrix4x4f_multiplied(matrix, Matrix4x4f_orthoMatrix(left, right, bottom, top, zNear, zFar));
}

Matrix4x4f Matrix4x4f_transposed(Matrix4x4f matrix) {
	return MATRIX4x4f(matrix.m[0],  matrix.m[1],  matrix.m[2],  matrix.m[3],
	                  matrix.m[4],  matrix.m[5],  matrix.m[6],  matrix.m[7],
	                  matrix.m[8],  matrix.m[9],  matrix.m[10], matrix.m[11],
	                  matrix.m[12], matrix.m[13], matrix.m[14], matrix.m[15]);
}

void Matrix4x4f_transpose(Matrix4x4f * matrix) {
	*matrix = Matrix4x4f_transposed(*matrix);
}

static float Matrix4x4f_subdeterminant(Matrix4x4f matrix, int excludeIndex) {
	int index4x4, index3x3;
	float matrix3x3[9];
	
	if (excludeIndex < 0 || excludeIndex > 15) {
		return 1.0f;
	}
	index3x3 = 0;
	for (index4x4 = 0; index4x4 < 16; index4x4++) {
		if (index4x4 / 4 == excludeIndex / 4 ||
		    index4x4 % 4 == excludeIndex % 4) {
			continue;
		}
		matrix3x3[index3x3++] = matrix.m[index4x4];
	}
	
	return matrix3x3[0] * (matrix3x3[4] * matrix3x3[8] - matrix3x3[5] * matrix3x3[7]) -
	       matrix3x3[3] * (matrix3x3[1] * matrix3x3[8] - matrix3x3[2] * matrix3x3[7]) +
	       matrix3x3[6] * (matrix3x3[1] * matrix3x3[5] - matrix3x3[2] * matrix3x3[4]);
}

float Matrix4x4f_determinant(Matrix4x4f matrix) {
	float subdeterminant0, subdeterminant1, subdeterminant2, subdeterminant3;
	
	subdeterminant0 = Matrix4x4f_subdeterminant(matrix, 0);
	subdeterminant1 = Matrix4x4f_subdeterminant(matrix, 4);
	subdeterminant2 = Matrix4x4f_subdeterminant(matrix, 8);
	subdeterminant3 = Matrix4x4f_subdeterminant(matrix, 12);
	
	return matrix.m[0]  *  subdeterminant0 +
	       matrix.m[4]  * -subdeterminant1 +
	       matrix.m[8]  *  subdeterminant2 +
	       matrix.m[12] * -subdeterminant3;
}

Matrix4x4f Matrix4x4f_inverted(Matrix4x4f matrix) {
	float determinant;
	Matrix4x4f result;
	int index, indexTransposed;
	int sign;
	
	determinant = Matrix4x4f_determinant(matrix);
	for (index = 0; index < 16; index++) {
		sign = 1 - (index % 4 + index / 4) % 2 * 2;
		indexTransposed = index % 4 * 4 + index / 4;
		result.m[indexTransposed] = Matrix4x4f_subdeterminant(matrix, index) * sign / determinant;
	}
	return result;
}

void Matrix4x4f_invert(Matrix4x4f * matrix) {
	*matrix = Matrix4x4f_inverted(*matrix);
}

Matrix4x4f Matrix4x4f_interpolated(Matrix4x4f left, Matrix4x4f right, float value) {
	unsigned int index;
	
	for (index = 0; index < 16; index++) {
		left.m[index] = left.m[index] * (1.0f - value) + right.m[index] * value;
	}
	return left;
}

void Matrix4x4f_interpolate(Matrix4x4f * left, Matrix4x4f right, float value) {
	*left = Matrix4x4f_interpolated(*left, right, value);
}

Vector2f Matrix4x4f_multiplyVector2f(Matrix4x4f matrix, Vector2f vector) {
	Vector2f result;
	
	result.x = matrix.m[0] * vector.x + matrix.m[4] * vector.y + matrix.m[12];
	result.y = matrix.m[1] * vector.x + matrix.m[5] * vector.y + matrix.m[13];
	return result;
}

Vector3f Matrix4x4f_multiplyVector3f(Matrix4x4f matrix, Vector3f vector) {
	Vector3f result;
	
	result.x = matrix.m[0] * vector.x + matrix.m[4] * vector.y + matrix.m[8]  * vector.z + matrix.m[12];
	result.y = matrix.m[1] * vector.x + matrix.m[5] * vector.y + matrix.m[9]  * vector.z + matrix.m[13];
	result.z = matrix.m[2] * vector.x + matrix.m[6] * vector.y + matrix.m[10] * vector.z + matrix.m[14];
	return result;
}

Vector4f Matrix4x4f_multiplyVector4f(Matrix4x4f matrix, Vector4f vector) {
	Vector4f result;
	
	result.x = matrix.m[0] * vector.x + matrix.m[4] * vector.y + matrix.m[8]  * vector.z + matrix.m[12] * vector.w;
	result.y = matrix.m[1] * vector.x + matrix.m[5] * vector.y + matrix.m[9]  * vector.z + matrix.m[13] * vector.w;
	result.z = matrix.m[2] * vector.x + matrix.m[6] * vector.y + matrix.m[10] * vector.z + matrix.m[14] * vector.w;
	result.w = matrix.m[3] * vector.x + matrix.m[7] * vector.y + matrix.m[11] * vector.z + matrix.m[15] * vector.w;
	return result;
}

Vector3f Matrix4x4f_multiplyVector3f_rotationOnly(Matrix4x4f matrix, Vector3f vector) {
	Vector3f result;
	
	result.x = matrix.m[0] * vector.x + matrix.m[4] * vector.y + matrix.m[8]  * vector.z;
	result.y = matrix.m[1] * vector.x + matrix.m[5] * vector.y + matrix.m[9]  * vector.z;
	result.z = matrix.m[2] * vector.x + matrix.m[6] * vector.y + matrix.m[10] * vector.z;
	return result;
}
