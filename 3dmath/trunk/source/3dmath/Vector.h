/*
  Copyright (c) 2012 Alex Diener
  
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

#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct Vector2 Vector2;
typedef struct Vector3 Vector3;
typedef struct Vector4 Vector4;

struct Vector2 {
	float x;
	float y;
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};

#define VECTOR2_ZERO ((Vector2) {0.0f, 0.0f})
#define VECTOR3_ZERO ((Vector3) {0.0f, 0.0f, 0.0f})
#define VECTOR4_ZERO ((Vector4) {0.0f, 0.0f, 0.0f, 0.0f})
#define VECTOR3_LEFT  ((Vector3) {-1.0f,  0.0f,  0.0f})
#define VECTOR3_RIGHT ((Vector3) { 1.0f,  0.0f,  0.0f})
#define VECTOR3_DOWN  ((Vector3) { 0.0f, -1.0f,  0.0f})
#define VECTOR3_UP    ((Vector3) { 0.0f,  1.0f,  0.0f})
#define VECTOR3_BACK  ((Vector3) { 0.0f,  0.0f, -1.0f})
#define VECTOR3_FRONT ((Vector3) { 0.0f,  0.0f,  1.0f})

Vector2 Vector2_init(float x, float y);
Vector3 Vector3_init(float x, float y, float z);
Vector4 Vector4_init(float x, float y, float z, float w);

void Vector2_normalize(Vector2 * vector);
void Vector3_normalize(Vector3 * vector);
void Vector4_normalize(Vector4 * vector);
Vector2 Vector2_normalized(Vector2 vector);
Vector3 Vector3_normalized(Vector3 vector);
Vector4 Vector4_normalized(Vector4 vector);

float Vector2_magnitude(Vector2 vector);
float Vector3_magnitude(Vector3 vector);
float Vector4_magnitude(Vector4 vector);
float Vector2_magnitudeSquared(Vector2 vector);
float Vector3_magnitudeSquared(Vector3 vector);
float Vector4_magnitudeSquared(Vector4 vector);

Vector2 Vector2_add(Vector2 vector1, Vector2 vector2);
Vector3 Vector3_add(Vector3 vector1, Vector3 vector2);
Vector4 Vector4_add(Vector4 vector1, Vector4 vector2);
Vector2 Vector2_subtract(Vector2 vector1, Vector2 vector2);
Vector3 Vector3_subtract(Vector3 vector1, Vector3 vector2);
Vector4 Vector4_subtract(Vector4 vector1, Vector4 vector2);
Vector2 Vector2_multiplyScalar(Vector2 vector, float scalar);
Vector3 Vector3_multiplyScalar(Vector3 vector, float scalar);
Vector4 Vector4_multiplyScalar(Vector4 vector, float scalar);
Vector2 Vector2_divideScalar(Vector2 vector, float scalar);
Vector3 Vector3_divideScalar(Vector3 vector, float scalar);
Vector4 Vector4_divideScalar(Vector4 vector, float scalar);

Vector2 Vector2_interpolate(Vector2 left, Vector2 right, float value);
Vector3 Vector3_interpolate(Vector3 left, Vector3 right, float value);
Vector4 Vector4_interpolate(Vector4 left, Vector4 right, float value);

Vector2 Vector2_reflect(Vector2 vector, Vector2 normal);
Vector3 Vector3_reflect(Vector3 vector, Vector3 normal);
Vector4 Vector4_reflect(Vector4 vector, Vector4 normal);

Vector2 Vector2_rotate(Vector2 vector, float radians);

float Vector2_dot(Vector2 vector1, Vector2 vector2);
float Vector3_dot(Vector3 vector1, Vector3 vector2);
float Vector4_dot(Vector4 vector1, Vector4 vector2);
float Vector2_cross(Vector2 vector1, Vector2 vector2);
Vector3 Vector3_cross(Vector3 vector1, Vector3 vector2);

#endif
