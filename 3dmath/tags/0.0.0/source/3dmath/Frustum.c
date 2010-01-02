#include "3dmath/Frustum.h"

#include "3dmath/Matrix.h"

Frustum Frustum_fromMatrix(Matrix matrix) {
	Frustum frustum;
	
	frustum.left.x = matrix.m[ 3] + matrix.m[ 0];
	frustum.left.y = matrix.m[ 7] + matrix.m[ 4];
	frustum.left.z = matrix.m[11] + matrix.m[ 8];
	frustum.left.w = matrix.m[15] + matrix.m[12];
	frustum.left = Vector4_divideScalar(frustum.left, Vector3_magnitude(Vector3_init(frustum.left.x, frustum.left.y, frustum.left.z)));
	
	frustum.right.x = matrix.m[ 3] - matrix.m[ 0];
	frustum.right.y = matrix.m[ 7] - matrix.m[ 4];
	frustum.right.z = matrix.m[11] - matrix.m[ 8];
	frustum.right.w = matrix.m[15] - matrix.m[12];
	frustum.right = Vector4_divideScalar(frustum.right, Vector3_magnitude(Vector3_init(frustum.right.x, frustum.right.y, frustum.right.z)));
	
	frustum.bottom.x = matrix.m[ 3] + matrix.m[ 1];
	frustum.bottom.y = matrix.m[ 7] + matrix.m[ 5];
	frustum.bottom.z = matrix.m[11] + matrix.m[ 9];
	frustum.bottom.w = matrix.m[15] + matrix.m[13];
	frustum.bottom = Vector4_divideScalar(frustum.bottom, Vector3_magnitude(Vector3_init(frustum.bottom.x, frustum.bottom.y, frustum.bottom.z)));
	
	frustum.top.x = matrix.m[ 3] - matrix.m[ 1];
	frustum.top.y = matrix.m[ 7] - matrix.m[ 5];
	frustum.top.z = matrix.m[11] - matrix.m[ 9];
	frustum.top.w = matrix.m[15] - matrix.m[13];
	frustum.top = Vector4_divideScalar(frustum.top, Vector3_magnitude(Vector3_init(frustum.top.x, frustum.top.y, frustum.top.z)));
	
	frustum.back.x = matrix.m[ 3] + matrix.m[ 2];
	frustum.back.y = matrix.m[ 7] + matrix.m[ 6];
	frustum.back.z = matrix.m[11] + matrix.m[10];
	frustum.back.w = matrix.m[15] + matrix.m[14];
	frustum.back = Vector4_divideScalar(frustum.back, Vector3_magnitude(Vector3_init(frustum.back.x, frustum.back.y, frustum.back.z)));
	
	frustum.front.x = matrix.m[ 3] - matrix.m[ 2];
	frustum.front.y = matrix.m[ 7] - matrix.m[ 6];
	frustum.front.z = matrix.m[11] - matrix.m[10];
	frustum.front.w = matrix.m[15] - matrix.m[14];
	frustum.front = Vector4_divideScalar(frustum.front, Vector3_magnitude(Vector3_init(frustum.front.x, frustum.front.y, frustum.front.z)));
	
	return frustum;
}

bool Frustum_containsSphere(Frustum frustum, Vector3 spherePosition, float sphereRadius) {
	Vector4 spherePosition4;
	
	spherePosition4 = Vector4_init(spherePosition.x, spherePosition.y, spherePosition.z, 1.0f);
	if (Vector4_dot(spherePosition4, frustum.left)   < -sphereRadius) return false;
	if (Vector4_dot(spherePosition4, frustum.right)  < -sphereRadius) return false;
	if (Vector4_dot(spherePosition4, frustum.bottom) < -sphereRadius) return false;
	if (Vector4_dot(spherePosition4, frustum.top)    < -sphereRadius) return false;
	if (Vector4_dot(spherePosition4, frustum.front)  < -sphereRadius) return false;
	if (Vector4_dot(spherePosition4, frustum.back)   < -sphereRadius) return false;
	return true;
}

float Frustum_sphereDistanceFromNearClippingPlane(Frustum frustum, Vector3 spherePosition, float sphereRadius) {
	Vector4 spherePosition4;
	float distance;
	
	spherePosition4 = Vector4_init(spherePosition.x, spherePosition.y, spherePosition.z, 1.0f);
	if (Vector4_dot(spherePosition4, frustum.left)   < -sphereRadius) return -1.0f;
	if (Vector4_dot(spherePosition4, frustum.right)  < -sphereRadius) return -1.0f;
	if (Vector4_dot(spherePosition4, frustum.bottom) < -sphereRadius) return -1.0f;
	if (Vector4_dot(spherePosition4, frustum.top)    < -sphereRadius) return -1.0f;
	if (Vector4_dot(spherePosition4, frustum.back)   < -sphereRadius) return -1.0f;
	
	distance = Vector4_dot(spherePosition4, frustum.front);
	return distance - sphereRadius;
}
