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

#include "gamemath/Matrix4x4x.h"
#include "gamemath/Quaternionx.h"
#include <math.h>

void Matrix4x4x_loadIdentity(Matrix4x4x * matrix) {
	matrix->m[0] = 0x10000;
	matrix->m[1] = 0x00000;
	matrix->m[2] = 0x00000;
	matrix->m[3] = 0x00000;
	matrix->m[4] = 0x00000;
	matrix->m[5] = 0x10000;
	matrix->m[6] = 0x00000;
	matrix->m[7] = 0x00000;
	matrix->m[8] = 0x00000;
	matrix->m[9] = 0x00000;
	matrix->m[10] = 0x10000;
	matrix->m[11] = 0x00000;
	matrix->m[12] = 0x00000;
	matrix->m[13] = 0x00000;
	matrix->m[14] = 0x00000;
	matrix->m[15] = 0x10000;
}

Matrix4x4x Matrix4x4x_fromDirectionVectors(Vector3x right, Vector3x up, Vector3x front) {
	Matrix4x4x matrix;
	
	Matrix4x4x_loadIdentity(&matrix);
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

void Matrix4x4x_multiply(Matrix4x4x * matrix1, Matrix4x4x m2) {
	Matrix4x4x m1, result;
	
	m1 = *matrix1;
	
	result.m[0]  = xmul(m1.m[0], m2.m[0])  + xmul(m1.m[4], m2.m[1])  + xmul(m1.m[8],  m2.m[2])  + xmul(m1.m[12], m2.m[3]);
	result.m[1]  = xmul(m1.m[1], m2.m[0])  + xmul(m1.m[5], m2.m[1])  + xmul(m1.m[9],  m2.m[2])  + xmul(m1.m[13], m2.m[3]);
	result.m[2]  = xmul(m1.m[2], m2.m[0])  + xmul(m1.m[6], m2.m[1])  + xmul(m1.m[10], m2.m[2])  + xmul(m1.m[14], m2.m[3]);
	result.m[3]  = xmul(m1.m[3], m2.m[0])  + xmul(m1.m[7], m2.m[1])  + xmul(m1.m[11], m2.m[2])  + xmul(m1.m[15], m2.m[3]);
	result.m[4]  = xmul(m1.m[0], m2.m[4])  + xmul(m1.m[4], m2.m[5])  + xmul(m1.m[8],  m2.m[6])  + xmul(m1.m[12], m2.m[7]);
	result.m[5]  = xmul(m1.m[1], m2.m[4])  + xmul(m1.m[5], m2.m[5])  + xmul(m1.m[9],  m2.m[6])  + xmul(m1.m[13], m2.m[7]);
	result.m[6]  = xmul(m1.m[2], m2.m[4])  + xmul(m1.m[6], m2.m[5])  + xmul(m1.m[10], m2.m[6])  + xmul(m1.m[14], m2.m[7]);
	result.m[7]  = xmul(m1.m[3], m2.m[4])  + xmul(m1.m[7], m2.m[5])  + xmul(m1.m[11], m2.m[6])  + xmul(m1.m[15], m2.m[7]);
	result.m[8]  = xmul(m1.m[0], m2.m[8])  + xmul(m1.m[4], m2.m[9])  + xmul(m1.m[8],  m2.m[10]) + xmul(m1.m[12], m2.m[11]);
	result.m[9]  = xmul(m1.m[1], m2.m[8])  + xmul(m1.m[5], m2.m[9])  + xmul(m1.m[9],  m2.m[10]) + xmul(m1.m[13], m2.m[11]);
	result.m[10] = xmul(m1.m[2], m2.m[8])  + xmul(m1.m[6], m2.m[9])  + xmul(m1.m[10], m2.m[10]) + xmul(m1.m[14], m2.m[11]);
	result.m[11] = xmul(m1.m[3], m2.m[8])  + xmul(m1.m[7], m2.m[9])  + xmul(m1.m[11], m2.m[10]) + xmul(m1.m[15], m2.m[11]);
	result.m[12] = xmul(m1.m[0], m2.m[12]) + xmul(m1.m[4], m2.m[13]) + xmul(m1.m[8],  m2.m[14]) + xmul(m1.m[12], m2.m[15]);
	result.m[13] = xmul(m1.m[1], m2.m[12]) + xmul(m1.m[5], m2.m[13]) + xmul(m1.m[9],  m2.m[14]) + xmul(m1.m[13], m2.m[15]);
	result.m[14] = xmul(m1.m[2], m2.m[12]) + xmul(m1.m[6], m2.m[13]) + xmul(m1.m[10], m2.m[14]) + xmul(m1.m[14], m2.m[15]);
	result.m[15] = xmul(m1.m[3], m2.m[12]) + xmul(m1.m[7], m2.m[13]) + xmul(m1.m[11], m2.m[14]) + xmul(m1.m[15], m2.m[15]);
	
	*matrix1 = result;
}

Matrix4x4x Matrix4x4x_multiplied(Matrix4x4x matrix1, Matrix4x4x matrix2) {
	Matrix4x4x_multiply(&matrix1, matrix2);
	return matrix1;
}

void Matrix4x4x_translate(Matrix4x4x * matrix, fixed16_16 x, fixed16_16 y, fixed16_16 z) {
	Matrix4x4x translationMatrix;
	
	Matrix4x4x_loadIdentity(&translationMatrix);
	translationMatrix.m[12] = x;
	translationMatrix.m[13] = y;
	translationMatrix.m[14] = z;
	Matrix4x4x_multiply(matrix, translationMatrix);
}

Matrix4x4x Matrix4x4x_translated(Matrix4x4x matrix, fixed16_16 x, fixed16_16 y, fixed16_16 z) {
	Matrix4x4x_translate(&matrix, x, y, z);
	return matrix;
}

void Matrix4x4x_scale(Matrix4x4x * matrix, fixed16_16 x, fixed16_16 y, fixed16_16 z) {
	Matrix4x4x scalingMatrix;
	
	Matrix4x4x_loadIdentity(&scalingMatrix);
	scalingMatrix.m[0] = x;
	scalingMatrix.m[5] = y;
	scalingMatrix.m[10] = z;
	Matrix4x4x_multiply(matrix, scalingMatrix);
}

Matrix4x4x Matrix4x4x_scaled(Matrix4x4x matrix, fixed16_16 x, fixed16_16 y, fixed16_16 z) {
	Matrix4x4x_scale(&matrix, x, y, z);
	return matrix;
}

void Matrix4x4x_rotate(Matrix4x4x * matrix, Vector3x axis, fixed16_16 radians) {
	Matrix4x4x rotationMatrix;
	Quaternionx quaternion;
	
	quaternion = Quaternionx_fromAxisAngle(axis, radians);
	rotationMatrix = Quaternionx_toMatrix(quaternion);
	Matrix4x4x_multiply(matrix, rotationMatrix);
}

Matrix4x4x Matrix4x4x_rotated(Matrix4x4x matrix, Vector3x axis, fixed16_16 radians) {
	Matrix4x4x_rotate(&matrix, axis, radians);
	return matrix;
}

void Matrix4x4x_shearX(Matrix4x4x * matrix, fixed16_16 y, fixed16_16 z) {
	Matrix4x4x shearingMatrix;
	
	Matrix4x4x_loadIdentity(&shearingMatrix);
	shearingMatrix.m[1] = y;
	shearingMatrix.m[2] = z;
	Matrix4x4x_multiply(matrix, shearingMatrix);
}

Matrix4x4x Matrix4x4x_shearedX(Matrix4x4x matrix, fixed16_16 y, fixed16_16 z) {
	Matrix4x4x_shearX(&matrix, y, z);
	return matrix;
}

void Matrix4x4x_shearY(Matrix4x4x * matrix, fixed16_16 x, fixed16_16 z) {
	Matrix4x4x shearingMatrix;
	
	Matrix4x4x_loadIdentity(&shearingMatrix);
	shearingMatrix.m[4] = x;
	shearingMatrix.m[6] = z;
	Matrix4x4x_multiply(matrix, shearingMatrix);
}

Matrix4x4x Matrix4x4x_shearedY(Matrix4x4x matrix, fixed16_16 x, fixed16_16 z) {
	Matrix4x4x_shearY(&matrix, x, z);
	return matrix;
}

void Matrix4x4x_shearZ(Matrix4x4x * matrix, fixed16_16 x, fixed16_16 y) {
	Matrix4x4x shearingMatrix;
	
	Matrix4x4x_loadIdentity(&shearingMatrix);
	shearingMatrix.m[8] = x;
	shearingMatrix.m[9] = y;
	Matrix4x4x_multiply(matrix, shearingMatrix);
}

Matrix4x4x Matrix4x4x_shearedZ(Matrix4x4x matrix, fixed16_16 x, fixed16_16 y) {
	Matrix4x4x_shearZ(&matrix, x, y);
	return matrix;
}

void Matrix4x4x_applyPerspective(Matrix4x4x * matrix, fixed16_16 fovYDegrees, fixed16_16 aspect, fixed16_16 zNear, fixed16_16 zFar) {
	Matrix4x4x perspectiveMatrix;
	fixed16_16 sine, cotangent, deltaZ;
	
	fovYDegrees = xmul(fovYDegrees, X_PI) / 360;
	deltaZ = zFar - zNear;
	sine = xsin(fovYDegrees);
	if (deltaZ == 0x00000 || sine == 0x00000 || aspect == 0x00000) {
		return;
	}
	cotangent = xdiv(xcos(fovYDegrees), sine);
	
	Matrix4x4x_loadIdentity(&perspectiveMatrix);
	perspectiveMatrix.m[0] = xdiv(cotangent, aspect);
	perspectiveMatrix.m[5] = cotangent;
	perspectiveMatrix.m[10] = xdiv(-(zFar + zNear), deltaZ);
	perspectiveMatrix.m[11] = -0x10000;
	perspectiveMatrix.m[14] = xdiv(xmul(xmul(-0x20000, zNear), zFar), deltaZ);
	perspectiveMatrix.m[15] = 0x00000;
	Matrix4x4x_multiply(matrix, perspectiveMatrix);
}

Matrix4x4x Matrix4x4x_perspective(Matrix4x4x matrix, fixed16_16 fovYDegrees, fixed16_16 aspect, fixed16_16 zNear, fixed16_16 zFar) {
	Matrix4x4x_applyPerspective(&matrix, fovYDegrees, aspect, zNear, zFar);
	return matrix;
}

void Matrix4x4x_applyOrtho(Matrix4x4x * matrix, fixed16_16 left, fixed16_16 right, fixed16_16 bottom, fixed16_16 top, fixed16_16 zNear, fixed16_16 zFar) {
	Matrix4x4x orthoMatrix;
	
	Matrix4x4x_loadIdentity(&orthoMatrix);
	orthoMatrix.m[0] = xdiv(0x20000, right - left);
	orthoMatrix.m[5] = xdiv(0x20000, top - bottom);
	orthoMatrix.m[10] = xdiv(-0x20000, zFar - zNear);
	orthoMatrix.m[12] = -xdiv(right + left, right - left);
	orthoMatrix.m[13] = -xdiv(top + bottom, top - bottom);
	orthoMatrix.m[14] = -xdiv(zFar + zNear, zFar - zNear);
	Matrix4x4x_multiply(matrix, orthoMatrix);
}

Matrix4x4x Matrix4x4x_ortho(Matrix4x4x matrix, fixed16_16 left, fixed16_16 right, fixed16_16 bottom, fixed16_16 top, fixed16_16 zNear, fixed16_16 zFar) {
	Matrix4x4x_applyOrtho(&matrix, left, right, bottom, top, zNear, zFar);
	return matrix;
}

void Matrix4x4x_transpose(Matrix4x4x * matrix) {
	*matrix = MATRIX4x4x(matrix->m[0],  matrix->m[1],  matrix->m[2],  matrix->m[3],
	                     matrix->m[4],  matrix->m[5],  matrix->m[6],  matrix->m[7],
	                     matrix->m[8],  matrix->m[9],  matrix->m[10], matrix->m[11],
	                     matrix->m[12], matrix->m[13], matrix->m[14], matrix->m[15]);
}

Matrix4x4x Matrix4x4x_transposed(Matrix4x4x matrix) {
	Matrix4x4x_transpose(&matrix);
	return matrix;
}

static fixed16_16 Matrix4x4x_subdeterminant(Matrix4x4x matrix, int excludeIndex) {
	int index4x4, index3x3;
	fixed16_16 matrix3x3[9];
	
	if (excludeIndex < 0 || excludeIndex > 15) {
		return 0x10000;
	}
	index3x3 = 0;
	for (index4x4 = 0; index4x4 < 16; index4x4++) {
		if (index4x4 / 4 == excludeIndex / 4 ||
		    index4x4 % 4 == excludeIndex % 4) {
			continue;
		}
		matrix3x3[index3x3++] = matrix.m[index4x4];
	}
	
	return xmul(matrix3x3[0], xmul(matrix3x3[4], matrix3x3[8]) - xmul(matrix3x3[5], matrix3x3[7])) -
	       xmul(matrix3x3[3], xmul(matrix3x3[1], matrix3x3[8]) - xmul(matrix3x3[2], matrix3x3[7])) +
	       xmul(matrix3x3[6], xmul(matrix3x3[1], matrix3x3[5]) - xmul(matrix3x3[2], matrix3x3[4]));
}

fixed16_16 Matrix4x4x_determinant(Matrix4x4x matrix) {
	fixed16_16 subdeterminant0, subdeterminant1, subdeterminant2, subdeterminant3;
	
	subdeterminant0 = Matrix4x4x_subdeterminant(matrix, 0);
	subdeterminant1 = Matrix4x4x_subdeterminant(matrix, 4);
	subdeterminant2 = Matrix4x4x_subdeterminant(matrix, 8);
	subdeterminant3 = Matrix4x4x_subdeterminant(matrix, 12);
	
	return xmul(matrix.m[0],   subdeterminant0) +
	       xmul(matrix.m[4],  -subdeterminant1) +
	       xmul(matrix.m[8],   subdeterminant2) +
	       xmul(matrix.m[12], -subdeterminant3);
}

void Matrix4x4x_invert(Matrix4x4x * matrix) {
	fixed16_16 determinant;
	Matrix4x4x result;
	int index, indexTransposed;
	int sign;
	
	determinant = Matrix4x4x_determinant(*matrix);
	for (index = 0; index < 16; index++) {
		sign = 1 - (index % 4 + index / 4) % 2 * 2;
		indexTransposed = index % 4 * 4 + index / 4;
		result.m[indexTransposed] = xdiv(Matrix4x4x_subdeterminant(*matrix, index) * sign, determinant);
	}
	
	*matrix = result;
}

Matrix4x4x Matrix4x4x_inverted(Matrix4x4x matrix) {
	Matrix4x4x_invert(&matrix);
	return matrix;
}

void Matrix4x4x_interpolate(Matrix4x4x * left, Matrix4x4x right, fixed16_16 value) {
	unsigned int index;
	
	for (index = 0; index < 16; index++) {
		left->m[index] = xmul(left->m[index], 0x10000 - value) + xmul(right.m[index], value);
	}
}

Matrix4x4x Matrix4x4x_interpolated(Matrix4x4x left, Matrix4x4x right, fixed16_16 value) {
	Matrix4x4x_interpolate(&left, right, value);
	return left;
}

Vector2x Matrix4x4x_multiplyVector2x(Matrix4x4x matrix, Vector2x vector) {
	Vector2x result;
	
	result.x = xmul(matrix.m[0], vector.x) + xmul(matrix.m[4], vector.y) + matrix.m[12];
	result.y = xmul(matrix.m[1], vector.x) + xmul(matrix.m[5], vector.y) + matrix.m[13];
	return result;
}

Vector3x Matrix4x4x_multiplyVector3x(Matrix4x4x matrix, Vector3x vector) {
	Vector3x result;
	
	result.x = xmul(matrix.m[0], vector.x) + xmul(matrix.m[4], vector.y) + xmul(matrix.m[8],  vector.z) + matrix.m[12];
	result.y = xmul(matrix.m[1], vector.x) + xmul(matrix.m[5], vector.y) + xmul(matrix.m[9],  vector.z) + matrix.m[13];
	result.z = xmul(matrix.m[2], vector.x) + xmul(matrix.m[6], vector.y) + xmul(matrix.m[10], vector.z) + matrix.m[14];
	return result;
}

Vector4x Matrix4x4x_multiplyVector4x(Matrix4x4x matrix, Vector4x vector) {
	Vector4x result;
	
	result.x = xmul(matrix.m[0], vector.x) + xmul(matrix.m[4], vector.y) + xmul(matrix.m[8],  vector.z) + xmul(matrix.m[12], vector.w);
	result.y = xmul(matrix.m[1], vector.x) + xmul(matrix.m[5], vector.y) + xmul(matrix.m[9],  vector.z) + xmul(matrix.m[13], vector.w);
	result.z = xmul(matrix.m[2], vector.x) + xmul(matrix.m[6], vector.y) + xmul(matrix.m[10], vector.z) + xmul(matrix.m[14], vector.w);
	result.w = xmul(matrix.m[3], vector.x) + xmul(matrix.m[7], vector.y) + xmul(matrix.m[11], vector.z) + xmul(matrix.m[15], vector.w);
	return result;
}

Vector3x Matrix4x4x_multiplyVector3x_rotationOnly(Matrix4x4x matrix, Vector3x vector) {
	Vector3x result;
	
	result.x = xmul(matrix.m[0], vector.x) + xmul(matrix.m[4], vector.y) + xmul(matrix.m[8],  vector.z);
	result.y = xmul(matrix.m[1], vector.x) + xmul(matrix.m[5], vector.y) + xmul(matrix.m[9],  vector.z);
	result.z = xmul(matrix.m[2], vector.x) + xmul(matrix.m[6], vector.y) + xmul(matrix.m[10], vector.z);
	return result;
}
