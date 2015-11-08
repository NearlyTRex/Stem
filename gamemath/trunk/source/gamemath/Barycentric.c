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

#include "gamemath/Barycentric.h"

Vector3f cartesianToBarycentric2f(Vector2f position, Vector2f vertex0, Vector2f vertex1, Vector2f vertex2) {
	Vector3f result;
	Vector2f v0 = Vector2f_subtract(vertex1, vertex0);
	Vector2f v1 = Vector2f_subtract(vertex2, vertex0);
	Vector2f v2 = Vector2f_subtract(position, vertex0);
	float denom = Vector2f_cross(v0, v1);
	
	result.y = Vector2f_cross(v2, v1) / denom;
	result.z = Vector2f_cross(v0, v2) / denom;
	result.x = 1.0f - result.y - result.z;
	
	return result;
}

Vector3f cartesianToBarycentric3f(Vector3f position, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2) {
	Vector3f result;
	Vector3f v0 = Vector3f_subtract(vertex1, vertex0);
	Vector3f v1 = Vector3f_subtract(vertex2, vertex0);
	Vector3f v2 = Vector3f_subtract(position, vertex0);
	float d00 = Vector3f_dot(v0, v0);
	float d01 = Vector3f_dot(v0, v1);
	float d11 = Vector3f_dot(v1, v1);
	float d20 = Vector3f_dot(v2, v0);
	float d21 = Vector3f_dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	
	result.y = (d11 * d20 - d01 * d21) / denom;
	result.z = (d00 * d21 - d01 * d20) / denom;
	result.x = 1.0f - result.y - result.z;
	
	return result;
}

Vector3x cartesianToBarycentric2x(Vector2x position, Vector2x vertex0, Vector2x vertex1, Vector2x vertex2) {
	Vector3x result;
	Vector2x v0 = Vector2x_subtract(vertex1, vertex0);
	Vector2x v1 = Vector2x_subtract(vertex2, vertex0);
	Vector2x v2 = Vector2x_subtract(position, vertex0);
	fixed16_16 denom = Vector2x_cross(v0, v1);
	
	result.y = xdiv(Vector2x_cross(v2, v1), denom);
	result.z = xdiv(Vector2x_cross(v0, v2), denom);
	result.x = 0x10000 - result.y - result.z;
	
	return result;
}

Vector3x cartesianToBarycentric3x(Vector3x position, Vector3x vertex0, Vector3x vertex1, Vector3x vertex2) {
	Vector3x result;
	Vector3x v0 = Vector3x_subtract(vertex1, vertex0);
	Vector3x v1 = Vector3x_subtract(vertex2, vertex0);
	Vector3x v2 = Vector3x_subtract(position, vertex0);
	fixed16_16 d00 = Vector3x_dot(v0, v0);
	fixed16_16 d01 = Vector3x_dot(v0, v1);
	fixed16_16 d11 = Vector3x_dot(v1, v1);
	fixed16_16 d20 = Vector3x_dot(v2, v0);
	fixed16_16 d21 = Vector3x_dot(v2, v1);
	fixed16_16 denom = xmul(d00, d11) - xmul(d01, d01);
	
	result.y = xdiv(xmul(d11, d20) - xmul(d01, d21), denom);
	result.z = xdiv(xmul(d00, d21) - xmul(d01, d20), denom);
	result.x = 0x10000 - result.y - result.z;
	
	return result;
}

Vector2f barycentricToCartesian2f(Vector3f position, Vector2f vertex0, Vector2f vertex1, Vector2f vertex2) {
	Vector2f result;
	
	result.x = vertex0.x * position.x + vertex1.x * position.y + vertex2.x * position.z;
	result.y = vertex0.y * position.x + vertex1.y * position.y + vertex2.y * position.z;
	return result;
}

Vector3f barycentricToCartesian3f(Vector3f position, Vector3f vertex0, Vector3f vertex1, Vector3f vertex2) {
	Vector3f result;
	
	result.x = vertex0.x * position.x + vertex1.x * position.y + vertex2.x * position.z;
	result.y = vertex0.y * position.x + vertex1.y * position.y + vertex2.y * position.z;
	result.z = vertex0.z * position.x + vertex1.z * position.y + vertex2.z * position.z;
	return result;
}

Vector2x barycentricToCartesian2x(Vector3x position, Vector2x vertex0, Vector2x vertex1, Vector2x vertex2) {
	Vector2x result;
	
	result.x = xmul(vertex0.x, position.x) + xmul(vertex1.x, position.y) + xmul(vertex2.x, position.z);
	result.y = xmul(vertex0.y, position.x) + xmul(vertex1.y, position.y) + xmul(vertex2.y, position.z);
	return result;
}

Vector3x barycentricToCartesian3x(Vector3x position, Vector3x vertex0, Vector3x vertex1, Vector3x vertex2) {
	Vector3x result;
	
	result.x = xmul(vertex0.x, position.x) + xmul(vertex1.x, position.y) + xmul(vertex2.x, position.z);
	result.y = xmul(vertex0.y, position.x) + xmul(vertex1.y, position.y) + xmul(vertex2.y, position.z);
	result.z = xmul(vertex0.z, position.x) + xmul(vertex1.z, position.y) + xmul(vertex2.z, position.z);
	return result;
}
