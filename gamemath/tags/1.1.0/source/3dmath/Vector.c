/*
  Copyright (c) 2010 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#include "3dmath/Vector.h"

#include <math.h>

Vector2 Vector2_init(float x, float y) {
	Vector2 vector;
	
	vector.x = x;
	vector.y = y;
	return vector;
}

Vector3 Vector3_init(float x, float y, float z) {
	Vector3 vector;
	
	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}

Vector4 Vector4_init(float x, float y, float z, float w) {
	Vector4 vector;
	
	vector.x = x;
	vector.y = y;
	vector.z = z;
	vector.w = w;
	return vector;
}

Vector2 Vector2_zero() {
	return Vector2_init(0.0f, 0.0f);
}

Vector3 Vector3_zero() {
	return Vector3_init(0.0f, 0.0f, 0.0f);
}

Vector4 Vector4_zero() {
	return Vector4_init(0.0f, 0.0f, 0.0f, 0.0f);
}

Vector3 Vector3_left() {
	return Vector3_init(-1.0f, 0.0f, 0.0f);
}

Vector3 Vector3_right() {
	return Vector3_init(1.0f, 0.0f, 0.0f);
}

Vector3 Vector3_down() {
	return Vector3_init(0.0f, -1.0f, 0.0f);
}

Vector3 Vector3_up() {
	return Vector3_init(0.0f, 1.0f, 0.0f);
}

Vector3 Vector3_back() {
	return Vector3_init(0.0f, 0.0f, -1.0f);
}

Vector3 Vector3_front() {
	return Vector3_init(0.0f, 0.0f, 1.0f);
}

void Vector2_normalize(Vector2 * vector) {
	float magnitude;
	
	magnitude = sqrtf(vector->x * vector->x + vector->y * vector->y);
	vector->x /= magnitude;
	vector->y /= magnitude;
}

void Vector3_normalize(Vector3 * vector) {
	float magnitude;
	
	magnitude = sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
	vector->x /= magnitude;
	vector->y /= magnitude;
	vector->z /= magnitude;
}

void Vector4_normalize(Vector4 * vector) {
	float magnitude;
	
	magnitude = sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z + vector->w * vector->w);
	vector->x /= magnitude;
	vector->y /= magnitude;
	vector->z /= magnitude;
	vector->w /= magnitude;
}

Vector2 Vector2_normalized(Vector2 vector) {
	Vector2_normalize(&vector);
	return vector;
}

Vector3 Vector3_normalized(Vector3 vector) {
	Vector3_normalize(&vector);
	return vector;
}

Vector4 Vector4_normalized(Vector4 vector) {
	Vector4_normalize(&vector);
	return vector;
}

float Vector2_magnitude(Vector2 vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

float Vector3_magnitude(Vector3 vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

float Vector4_magnitude(Vector4 vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w);
}

float Vector2_magnitudeSquared(Vector2 vector) {
	return vector.x * vector.x + vector.y * vector.y;
}

float Vector3_magnitudeSquared(Vector3 vector) {
	return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

float Vector4_magnitudeSquared(Vector4 vector) {
	return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
}

Vector2 Vector2_add(Vector2 vector1, Vector2 vector2) {
	return Vector2_init(vector1.x + vector2.x, vector1.y + vector2.y);
}

Vector3 Vector3_add(Vector3 vector1, Vector3 vector2) {
	return Vector3_init(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z);
}

Vector4 Vector4_add(Vector4 vector1, Vector4 vector2) {
	return Vector4_init(vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z, vector1.w + vector2.w);
}

Vector2 Vector2_subtract(Vector2 vector1, Vector2 vector2) {
	return Vector2_init(vector1.x - vector2.x, vector1.y - vector2.y);
}

Vector3 Vector3_subtract(Vector3 vector1, Vector3 vector2) {
	return Vector3_init(vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z);
}

Vector4 Vector4_subtract(Vector4 vector1, Vector4 vector2) {
	return Vector4_init(vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z, vector1.w - vector2.w);
}

Vector2 Vector2_multiplyScalar(Vector2 vector, float scalar) {
	return Vector2_init(vector.x * scalar, vector.y * scalar);
}

Vector3 Vector3_multiplyScalar(Vector3 vector, float scalar) {
	return Vector3_init(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

Vector4 Vector4_multiplyScalar(Vector4 vector, float scalar) {
	return Vector4_init(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
}

Vector2 Vector2_divideScalar(Vector2 vector, float scalar) {
	return Vector2_init(vector.x / scalar, vector.y / scalar);
}

Vector3 Vector3_divideScalar(Vector3 vector, float scalar) {
	return Vector3_init(vector.x / scalar, vector.y / scalar, vector.z / scalar);
}

Vector4 Vector4_divideScalar(Vector4 vector, float scalar) {
	return Vector4_init(vector.x / scalar, vector.y / scalar, vector.z / scalar, vector.w / scalar);
}

Vector2 Vector2_interpolate(Vector2 left, Vector2 right, float phase) {
	return Vector2_init(left.x * (1.0f - phase) + right.x * phase,
	                    left.y * (1.0f - phase) + right.y * phase);
}

Vector3 Vector3_interpolate(Vector3 left, Vector3 right, float phase) {
	return Vector3_init(left.x * (1.0f - phase) + right.x * phase,
	                    left.y * (1.0f - phase) + right.y * phase,
	                    left.z * (1.0f - phase) + right.z * phase);
}

Vector4 Vector4_interpolate(Vector4 left, Vector4 right, float phase) {
	return Vector4_init(left.x * (1.0f - phase) + right.x * phase,
	                    left.y * (1.0f - phase) + right.y * phase,
	                    left.z * (1.0f - phase) + right.z * phase,
	                    left.w * (1.0f - phase) + right.w * phase);
}

Vector2 Vector2_reflect(Vector2 vector, Vector2 normal) {
	Vector2 result;
	float dot;
	
	dot = Vector2_dot(vector, normal);
	result.x = 2 * dot * normal.x - vector.x;
	result.y = 2 * dot * normal.y - vector.y;
	return result;
}

Vector3 Vector3_reflect(Vector3 vector, Vector3 normal) {
	Vector3 result;
	float dot;
	
	dot = Vector3_dot(vector, normal);
	result.x = 2 * dot * normal.x - vector.x;
	result.y = 2 * dot * normal.y - vector.y;
	result.z = 2 * dot * normal.z - vector.z;
	return result;
}

Vector4 Vector4_reflect(Vector4 vector, Vector4 normal) {
	Vector4 result;
	float dot;
	
	dot = Vector4_dot(vector, normal);
	result.x = 2 * dot * normal.x - vector.x;
	result.y = 2 * dot * normal.y - vector.y;
	result.z = 2 * dot * normal.z - vector.z;
	result.w = 2 * dot * normal.w - vector.w;
	return result;
}

float Vector2_dot(Vector2 vector1, Vector2 vector2) {
	return vector1.x * vector2.x + vector1.y * vector2.y;
}

float Vector3_dot(Vector3 vector1, Vector3 vector2) {
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

float Vector4_dot(Vector4 vector1, Vector4 vector2) {
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z + vector1.w * vector2.w;
}

float Vector2_cross(Vector2 vector1, Vector2 vector2) {
	return vector1.x * vector2.y - vector1.y * vector2.x;
}

Vector3 Vector3_cross(Vector3 vector1, Vector3 vector2) {
	Vector3 result;
	
	result.x = vector1.y * vector2.z - vector1.z * vector2.y;
	result.y = vector1.z * vector2.x - vector1.x * vector2.z;
	result.z = vector1.x * vector2.y - vector1.y * vector2.x;
	return result;
}
