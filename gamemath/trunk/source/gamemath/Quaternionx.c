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

#include "gamemath/Quaternionx.h"

#include <math.h>
#include <stdlib.h>

void Quaternionx_loadIdentity(Quaternionx * quaternion) {
	quaternion->x = 0x00000;
	quaternion->y = 0x00000;
	quaternion->z = 0x00000;
	quaternion->w = 0x10000;
}

Quaternionx Quaternionx_fromAxisAngle(Vector3x axis, fixed16_16 radians) {
	Quaternionx quaternion;
	fixed16_16 sinAngle;
	
	radians /= 2;
	if (!Vector3x_normalize(&axis)) {
		quaternion.x = FIXED_16_16_NAN;
		quaternion.y = FIXED_16_16_NAN;
		quaternion.z = FIXED_16_16_NAN;
		quaternion.w = 0x10000;
	} else {
		sinAngle = xsin(radians);
		quaternion.x = xmul(axis.x, sinAngle);
		quaternion.y = xmul(axis.y, sinAngle);
		quaternion.z = xmul(axis.z, sinAngle);
		quaternion.w = xcos(radians);
	}
	
	return quaternion;
}

void Quaternionx_toAxisAngle(Quaternionx quaternion, Vector3x * outAxis, fixed16_16 * outRadians) {
	fixed16_16 sinAngle;
	
	Quaternionx_normalize(&quaternion);
	sinAngle = xsqrt(0x10000 - xmul(quaternion.w, quaternion.w));
	if (xabs(sinAngle) < 0x00040) {
		sinAngle = 0x10000;
	}
	
	if (outAxis != NULL) {
		outAxis->x = xdiv(quaternion.x, sinAngle);
		outAxis->y = xdiv(quaternion.y, sinAngle);
		outAxis->z = xdiv(quaternion.z, sinAngle);
	}
	
	if (outRadians != NULL) {
		*outRadians = xmul(xacos(quaternion.w), 0x20000);
	}
}

Matrix4x4x Quaternionx_toMatrix(Quaternionx quaternion) {
	Matrix4x4x matrix;
	
	matrix.m[0]  = 0x10000 - 2 * (xmul(quaternion.y, quaternion.y) + xmul(quaternion.z, quaternion.z));
	matrix.m[1]  =           2 * (xmul(quaternion.x, quaternion.y) + xmul(quaternion.z, quaternion.w));
	matrix.m[2]  =           2 * (xmul(quaternion.x, quaternion.z) - xmul(quaternion.y, quaternion.w));
	matrix.m[3]  = 0x00000;
	matrix.m[4]  =           2 * (xmul(quaternion.x, quaternion.y) - xmul(quaternion.z, quaternion.w));
	matrix.m[5]  = 0x10000 - 2 * (xmul(quaternion.x, quaternion.x) + xmul(quaternion.z, quaternion.z));
	matrix.m[6]  =           2 * (xmul(quaternion.y, quaternion.z) + xmul(quaternion.x, quaternion.w));
	matrix.m[7]  = 0x00000;
	matrix.m[8]  =           2 * (xmul(quaternion.x, quaternion.z) + xmul(quaternion.y, quaternion.w));
	matrix.m[9]  =           2 * (xmul(quaternion.y, quaternion.z) - xmul(quaternion.x, quaternion.w));
	matrix.m[10] = 0x10000 - 2 * (xmul(quaternion.x, quaternion.x) + xmul(quaternion.y, quaternion.y));
	matrix.m[11] = 0x00000;
	matrix.m[12] = 0x00000;
	matrix.m[13] = 0x00000;
	matrix.m[14] = 0x00000;
	matrix.m[15] = 0x10000;
	
	return matrix;
}

void Quaternionx_normalize(Quaternionx * quaternion) {
	fixed16_16 magnitude;
	
	magnitude = xsqrt(xmul(quaternion->x, quaternion->x) +
	                  xmul(quaternion->y, quaternion->y) +
	                  xmul(quaternion->z, quaternion->z) +
	                  xmul(quaternion->w, quaternion->w));
	quaternion->x = xdiv(quaternion->x, magnitude);
	quaternion->y = xdiv(quaternion->y, magnitude);
	quaternion->z = xdiv(quaternion->z, magnitude);
	quaternion->w = xdiv(quaternion->w, magnitude);
}

Quaternionx Quaternionx_normalized(Quaternionx quaternion) {
	Quaternionx_normalize(&quaternion);
	return quaternion;
}

void Quaternionx_multiply(Quaternionx * quaternion1, Quaternionx quaternion2) {
	Quaternionx result;
	
	result.x = xmul(quaternion1->w, quaternion2.x) + xmul(quaternion1->x, quaternion2.w) + xmul(quaternion1->y, quaternion2.z) - xmul(quaternion1->z, quaternion2.y);
	result.y = xmul(quaternion1->w, quaternion2.y) - xmul(quaternion1->x, quaternion2.z) + xmul(quaternion1->y, quaternion2.w) + xmul(quaternion1->z, quaternion2.x);
	result.z = xmul(quaternion1->w, quaternion2.z) + xmul(quaternion1->x, quaternion2.y) - xmul(quaternion1->y, quaternion2.x) + xmul(quaternion1->z, quaternion2.w);
	result.w = xmul(quaternion1->w, quaternion2.w) - xmul(quaternion1->x, quaternion2.x) - xmul(quaternion1->y, quaternion2.y) - xmul(quaternion1->z, quaternion2.z);
	*quaternion1 = result;
}

Quaternionx Quaternionx_multiplied(Quaternionx quaternion1, Quaternionx quaternion2) {
	Quaternionx_multiply(&quaternion1, quaternion2);
	return quaternion1;
}

#define SLERP_TO_LERP_SWITCH_THRESHOLD 0x00100

Quaternionx Quaternionx_slerp(Quaternionx left, Quaternionx right, fixed16_16 value) {
	fixed16_16 leftWeight, rightWeight, difference;
	Quaternionx result;
	
	difference = xmul(left.x, right.x) + xmul(left.y, right.y) + xmul(left.z, right.z) + xmul(left.w, right.w);
	if (0x10000 - xabs(difference) > SLERP_TO_LERP_SWITCH_THRESHOLD) {
		fixed16_16 theta, oneOverSinTheta;
		
		theta = xcos(xabs(difference));
		oneOverSinTheta = xdiv(0x10000, xsin(theta));
		leftWeight = xmul(xsin(xmul(theta, 0x10000 - value)), oneOverSinTheta);
		rightWeight = xmul(xsin(xmul(theta, value)), oneOverSinTheta);
		if (difference < 0x00000) {
			leftWeight = -leftWeight;
		}
	} else {
		leftWeight = 0x10000 - value;
		rightWeight = value;
	}
	result.x = xmul(left.x, leftWeight) + xmul(right.x, rightWeight);
	result.y = xmul(left.y, leftWeight) + xmul(right.y, rightWeight);
	result.z = xmul(left.z, leftWeight) + xmul(right.z, rightWeight);
	result.w = xmul(left.w, leftWeight) + xmul(right.w, rightWeight);
	Quaternionx_normalize(&result);
	
	return result;
}

void Quaternionx_rotate(Quaternionx * quaternion, Vector3x axis, fixed16_16 radians) {
	Quaternionx rotationQuaternion;
	
	rotationQuaternion = Quaternionx_fromAxisAngle(axis, radians);
	Quaternionx_multiply(quaternion, rotationQuaternion);
}

Quaternionx Quaternionx_rotated(Quaternionx quaternion, Vector3x axis, fixed16_16 radians) {
	Quaternionx_rotate(&quaternion, axis, radians);
	return quaternion;
}

void Quaternionx_invert(Quaternionx * quaternion) {
	fixed16_16 length;
	
	length = xdiv(0x10000, xmul(quaternion->x, quaternion->x) +
	                       xmul(quaternion->y, quaternion->y) +
	                       xmul(quaternion->z, quaternion->z) +
	                       xmul(quaternion->w, quaternion->w));
	quaternion->x = xmul(quaternion->x, -length);
	quaternion->y = xmul(quaternion->y, -length);
	quaternion->z = xmul(quaternion->z, -length);
	quaternion->w = xmul(quaternion->w, length);
}

Quaternionx Quaternionx_inverted(Quaternionx quaternion) {
	Quaternionx_invert(&quaternion);
	return quaternion;
}

Vector3x Quaternionx_multiplyVector3x(Quaternionx quaternion, Vector3x vector) {
	Quaternionx vectorQuaternion, inverseQuaternion, result;
	
	vectorQuaternion = QUATERNIONx(vector.x, vector.y, vector.z, 0x00000);
	inverseQuaternion = Quaternionx_inverted(quaternion);
	result = Quaternionx_multiplied(quaternion, Quaternionx_multiplied(vectorQuaternion, inverseQuaternion));
	return VECTOR3x(result.x, result.y, result.z);
}

Vector4x Quaternionx_multiplyVector4x(Quaternionx quaternion, Vector4x vector) {
	Vector3x vector3;
	
	vector3 = Quaternionx_multiplyVector3x(quaternion, VECTOR3x(vector.x, vector.y, vector.z));
	return VECTOR4x(vector3.x, vector3.y, vector3.z, vector.w);
}
