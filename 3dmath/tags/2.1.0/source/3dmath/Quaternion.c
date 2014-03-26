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

#include "3dmath/Quaternion.h"

#include <math.h>
#include <stdlib.h>

void Quaternion_loadIdentity(Quaternion * quaternion) {
	quaternion->x = 0.0f;
	quaternion->y = 0.0f;
	quaternion->z = 0.0f;
	quaternion->w = 1.0f;
}

Quaternion Quaternion_fromAxisAngle(Vector3f axis, float radians) {
	Quaternion quaternion;
	float sinAngle;
	
	radians *= 0.5f;
	if (!Vector3f_normalize(&axis)) {
		quaternion.x = NAN;
		quaternion.y = NAN;
		quaternion.z = NAN;
		quaternion.w = 1.0f;
	} else {
		sinAngle = sin(radians);
		quaternion.x = axis.x * sinAngle;
		quaternion.y = axis.y * sinAngle;
		quaternion.z = axis.z * sinAngle;
		quaternion.w = cos(radians);
	}
	
	return quaternion;
}

void Quaternion_toAxisAngle(Quaternion quaternion, Vector3f * outAxis, float * outRadians) {
	float sinAngle;
	
	Quaternion_normalize(&quaternion);
	sinAngle = sqrtf(1.0f - quaternion.w * quaternion.w);
	if (fabsf(sinAngle) < 0.0005f) {
		sinAngle = 1.0f;
	}
	
	if (outAxis != NULL) {
		outAxis->x = quaternion.x / sinAngle;
		outAxis->y = quaternion.y / sinAngle;
		outAxis->z = quaternion.z / sinAngle;
	}
	
	if (outRadians != NULL) {
		*outRadians = acos(quaternion.w) * 2.0f;
	}
}

Matrix Quaternion_toMatrix(Quaternion quaternion) {
	Matrix matrix;
	
	matrix.m[0]  = 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	matrix.m[1]  =        2.0f * (quaternion.x * quaternion.y + quaternion.z * quaternion.w);
	matrix.m[2]  =        2.0f * (quaternion.x * quaternion.z - quaternion.y * quaternion.w);
	matrix.m[3]  = 0.0f;
	matrix.m[4]  =        2.0f * (quaternion.x * quaternion.y - quaternion.z * quaternion.w);
	matrix.m[5]  = 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z);
	matrix.m[6]  =        2.0f * (quaternion.y * quaternion.z + quaternion.x * quaternion.w);
	matrix.m[7]  = 0.0f;
	matrix.m[8]  =        2.0f * (quaternion.x * quaternion.z + quaternion.y * quaternion.w);
	matrix.m[9]  =        2.0f * (quaternion.y * quaternion.z - quaternion.x * quaternion.w);
	matrix.m[10] = 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
	matrix.m[11] = 0.0f;
	matrix.m[12] = 0.0f;
	matrix.m[13] = 0.0f;
	matrix.m[14] = 0.0f;
	matrix.m[15] = 1.0f;
	
	return matrix;
}

void Quaternion_normalize(Quaternion * quaternion) {
	float magnitude;
	
	magnitude = sqrtf(quaternion->x * quaternion->x +
	                  quaternion->y * quaternion->y +
	                  quaternion->z * quaternion->z +
	                  quaternion->w * quaternion->w);
	quaternion->x /= magnitude;
	quaternion->y /= magnitude;
	quaternion->z /= magnitude;
	quaternion->w /= magnitude;
}

Quaternion Quaternion_normalized(Quaternion quaternion) {
	Quaternion_normalize(&quaternion);
	return quaternion;
}

void Quaternion_multiply(Quaternion * quaternion1, Quaternion quaternion2) {
	Quaternion result;
	
	result.x = quaternion1->w * quaternion2.x + quaternion1->x * quaternion2.w + quaternion1->y * quaternion2.z - quaternion1->z * quaternion2.y;
	result.y = quaternion1->w * quaternion2.y - quaternion1->x * quaternion2.z + quaternion1->y * quaternion2.w + quaternion1->z * quaternion2.x;
	result.z = quaternion1->w * quaternion2.z + quaternion1->x * quaternion2.y - quaternion1->y * quaternion2.x + quaternion1->z * quaternion2.w;
	result.w = quaternion1->w * quaternion2.w - quaternion1->x * quaternion2.x - quaternion1->y * quaternion2.y - quaternion1->z * quaternion2.z;
	*quaternion1 = result;
}

Quaternion Quaternion_multiplied(Quaternion quaternion1, Quaternion quaternion2) {
	Quaternion_multiply(&quaternion1, quaternion2);
	return quaternion1;
}

#define SLERP_TO_LERP_SWITCH_THRESHOLD 0.01f

Quaternion Quaternion_slerp(Quaternion left, Quaternion right, float value) {
	float leftWeight, rightWeight, difference;
	Quaternion result;
	
	difference = left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
	if (1.0f - fabs(difference) > SLERP_TO_LERP_SWITCH_THRESHOLD) {
		float theta, oneOverSinTheta;
		
		theta = acos(fabs(difference));
		oneOverSinTheta = 1.0f / sin(theta);
		leftWeight = sin(theta * (1.0f - value)) * oneOverSinTheta;
		rightWeight = sin(theta * value) * oneOverSinTheta;
		if (difference < 0.0f) {
			leftWeight = -leftWeight;
		}
	} else {
		leftWeight = 1.0f - value;
		rightWeight = value;
	}
	result.x = left.x * leftWeight + right.x * rightWeight;
	result.y = left.y * leftWeight + right.y * rightWeight;
	result.z = left.z * leftWeight + right.z * rightWeight;
	result.w = left.w * leftWeight + right.w * rightWeight;
	Quaternion_normalize(&result);
	
	return result;
}

void Quaternion_rotate(Quaternion * quaternion, Vector3f axis, float radians) {
	Quaternion rotationQuaternion;
	
	rotationQuaternion = Quaternion_fromAxisAngle(axis, radians);
	Quaternion_multiply(quaternion, rotationQuaternion);
}

Quaternion Quaternion_rotated(Quaternion quaternion, Vector3f axis, float radians) {
	Quaternion_rotate(&quaternion, axis, radians);
	return quaternion;
}

void Quaternion_invert(Quaternion * quaternion) {
	float length;
	
	length = 1.0f / (quaternion->x * quaternion->x +
	                 quaternion->y * quaternion->y +
	                 quaternion->z * quaternion->z +
	                 quaternion->w * quaternion->w);
	quaternion->x *= -length;
	quaternion->y *= -length;
	quaternion->z *= -length;
	quaternion->w *= length;
}

Quaternion Quaternion_inverted(Quaternion quaternion) {
	Quaternion_invert(&quaternion);
	return quaternion;
}

Vector3f Quaternion_multiplyVector3f(Quaternion quaternion, Vector3f vector) {
	Quaternion vectorQuaternion, inverseQuaternion, result;
	
	vectorQuaternion = QUATERNION(vector.x, vector.y, vector.z, 0.0f);
	inverseQuaternion = Quaternion_inverted(quaternion);
	result = Quaternion_multiplied(quaternion, Quaternion_multiplied(vectorQuaternion, inverseQuaternion));
	return VECTOR3f(result.x, result.y, result.z);
}

Vector4f Quaternion_multiplyVector4f(Quaternion quaternion, Vector4f vector) {
	Vector3f vector3;
	
	vector3 = Quaternion_multiplyVector3f(quaternion, VECTOR3f(vector.x, vector.y, vector.z));
	return VECTOR4f(vector3.x, vector3.y, vector3.z, vector.w);
}
