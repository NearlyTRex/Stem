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

#include "gamemath/Quaternionf.h"

#include <math.h>
#include <stdlib.h>

void Quaternionf_loadIdentity(Quaternionf * quaternion) {
	quaternion->x = 0.0f;
	quaternion->y = 0.0f;
	quaternion->z = 0.0f;
	quaternion->w = 1.0f;
}

Quaternionf Quaternionf_fromAxisAngle(Vector3f axis, float radians) {
	Quaternionf quaternion;
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

void Quaternionf_toAxisAngle(Quaternionf quaternion, Vector3f * outAxis, float * outRadians) {
	float sinAngle;
	
	Quaternionf_normalize(&quaternion);
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

Matrix4x4f Quaternionf_toMatrix(Quaternionf quaternion) {
	Matrix4x4f matrix;
	
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

void Quaternionf_normalize(Quaternionf * quaternion) {
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

Quaternionf Quaternionf_normalized(Quaternionf quaternion) {
	Quaternionf_normalize(&quaternion);
	return quaternion;
}

void Quaternionf_multiply(Quaternionf * quaternion1, Quaternionf quaternion2) {
	Quaternionf result;
	
	result.x = quaternion1->w * quaternion2.x + quaternion1->x * quaternion2.w + quaternion1->y * quaternion2.z - quaternion1->z * quaternion2.y;
	result.y = quaternion1->w * quaternion2.y - quaternion1->x * quaternion2.z + quaternion1->y * quaternion2.w + quaternion1->z * quaternion2.x;
	result.z = quaternion1->w * quaternion2.z + quaternion1->x * quaternion2.y - quaternion1->y * quaternion2.x + quaternion1->z * quaternion2.w;
	result.w = quaternion1->w * quaternion2.w - quaternion1->x * quaternion2.x - quaternion1->y * quaternion2.y - quaternion1->z * quaternion2.z;
	*quaternion1 = result;
}

Quaternionf Quaternionf_multiplied(Quaternionf quaternion1, Quaternionf quaternion2) {
	Quaternionf_multiply(&quaternion1, quaternion2);
	return quaternion1;
}

#define SLERP_TO_LERP_SWITCH_THRESHOLD 0.01f

Quaternionf Quaternionf_slerp(Quaternionf left, Quaternionf right, float value) {
	float leftWeight, rightWeight, difference;
	Quaternionf result;
	
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
	Quaternionf_normalize(&result);
	
	return result;
}

void Quaternionf_rotate(Quaternionf * quaternion, Vector3f axis, float radians) {
	Quaternionf rotationQuaternion;
	
	rotationQuaternion = Quaternionf_fromAxisAngle(axis, radians);
	Quaternionf_multiply(quaternion, rotationQuaternion);
}

Quaternionf Quaternionf_rotated(Quaternionf quaternion, Vector3f axis, float radians) {
	Quaternionf_rotate(&quaternion, axis, radians);
	return quaternion;
}

void Quaternionf_invert(Quaternionf * quaternion) {
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

Quaternionf Quaternionf_inverted(Quaternionf quaternion) {
	Quaternionf_invert(&quaternion);
	return quaternion;
}

Vector3f Quaternionf_multiplyVector3f(Quaternionf quaternion, Vector3f vector) {
	Quaternionf vectorQuaternion, inverseQuaternion, result;
	
	vectorQuaternion = QUATERNIONf(vector.x, vector.y, vector.z, 0.0f);
	inverseQuaternion = Quaternionf_inverted(quaternion);
	result = Quaternionf_multiplied(quaternion, Quaternionf_multiplied(vectorQuaternion, inverseQuaternion));
	return VECTOR3f(result.x, result.y, result.z);
}

Vector4f Quaternionf_multiplyVector4f(Quaternionf quaternion, Vector4f vector) {
	Vector3f vector3;
	
	vector3 = Quaternionf_multiplyVector3f(quaternion, VECTOR3f(vector.x, vector.y, vector.z));
	return VECTOR4f(vector3.x, vector3.y, vector3.z, vector.w);
}
