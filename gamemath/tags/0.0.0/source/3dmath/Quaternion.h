#ifndef __QUATERNION_H__
#define __QUATERNION_H__

typedef struct Quaternion Quaternion;

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

#include "3dmath/Matrix.h"
#include "3dmath/Vector.h"

void Quaternion_loadIdentity(Quaternion * quaternion);
Quaternion Quaternion_identity();
Quaternion Quaternion_init(float x, float y, float z, float w);

Quaternion Quaternion_fromVector(Vector3 vector);
struct Vector3 Quaternion_toVector(Quaternion quaternion);
Quaternion Quaternion_fromAxisAngle(Vector3 axis, float angle);
void Quaternion_toAxisAngle(Quaternion quaternion, Vector3 * axis, float * angle);
struct Matrix Quaternion_toMatrix(Quaternion quaternion);

void Quaternion_normalize(Quaternion * quaternion);
Quaternion Quaternion_normalized(Quaternion quaternion);

void Quaternion_multiply(Quaternion * quaternion1, Quaternion quaternion2);
Quaternion Quaternion_multiplied(Quaternion quaternion1, Quaternion quaternion2);
Quaternion Quaternion_slerp(Quaternion start, Quaternion end, float alpha);

void Quaternion_rotate(Quaternion * quaternion, Vector3 axis, float angle);
Quaternion Quaternion_rotated(Quaternion quaternion, Vector3 axis, float angle);

void Quaternion_invert(Quaternion * quaternion);
Quaternion Quaternion_inverted(Quaternion quaternion);

Vector3 Quaternion_multiplyVector3(Quaternion quaternion, Vector3 vector);
Vector4 Quaternion_multiplyVector4(Quaternion quaternion, Vector4 vector);

#endif
