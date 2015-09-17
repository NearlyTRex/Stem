#include "gamemath/Vector2d.h"
#include "gamemath/Vector2f.h"
#include "gamemath/Vector2i.h"
#include "gamemath/Vector2x.h"
#include "gamemath/Vector3d.h"
#include "gamemath/Vector3f.h"
#include "gamemath/Vector3i.h"
#include "gamemath/Vector3x.h"
#include "gamemath/Vector4d.h"
#include "gamemath/Vector4f.h"
#include "gamemath/Vector4i.h"
#include "gamemath/Vector4x.h"
#include "gamemath/VectorConversions.h"
#include "unittest/TestSuite.h"

static void testTypeConversions() {
	Vector2d vector2d;
	Vector2f vector2f;
	Vector2i vector2i;
	Vector2x vector2x;
	Vector3d vector3d;
	Vector3f vector3f;
	Vector3i vector3i;
	Vector3x vector3x;
	Vector4d vector4d;
	Vector4f vector4f;
	Vector4i vector4i;
	Vector4x vector4x;
	
	vector2d = Vector2f_toVector2d(VECTOR2f(1.25f, 2.0f));
	TestCase_assert(vector2d.x == 1.25 && vector2d.y == 2.0, "Expected {1.25, 2.0} but got {%f, %f}", vector2d.x, vector2d.y);
	vector2d = Vector2i_toVector2d(VECTOR2i(1, 2));
	TestCase_assert(vector2d.x == 1.0 && vector2d.y == 2.0, "Expected {1.0, 2.0} but got {%f, %f}", vector2d.x, vector2d.y);
	vector2d = Vector2x_toVector2d(VECTOR2x(0x14000, 0x20000));
	TestCase_assert(vector2d.x == 1.25 && vector2d.y == 2.0, "Expected {1.25, 2.0} but got {%f, %f}", vector2d.x, vector2d.y);
	vector2d = Vector2f_toVector2d(VECTOR2f(5.0f, 6.0f));
	TestCase_assert(vector2d.x == 5.0 && vector2d.y == 6.0, "Expected {5.0, 6.0} but got {%f, %f}", vector2d.x, vector2d.y);
	vector2d = Vector2i_toVector2d(VECTOR2i(5, 6));
	TestCase_assert(vector2d.x == 5.0 && vector2d.y == 6.0, "Expected {5.0, 6.0} but got {%f, %f}", vector2d.x, vector2d.y);
	vector2d = Vector2x_toVector2d(VECTOR2x(0x50000, 0x60000));
	TestCase_assert(vector2d.x == 5.0 && vector2d.y == 6.0, "Expected {5.0, 6.0} but got {%f, %f}", vector2d.x, vector2d.y);
	
	vector2f = Vector2d_toVector2f(VECTOR2d(1.25, 2.0));
	TestCase_assert(vector2f.x == 1.25f && vector2f.y == 2.0f, "Expected {1.25, 2.0} but got {%f, %f}", vector2f.x, vector2f.y);
	vector2f = Vector2i_toVector2f(VECTOR2i(1, 2));
	TestCase_assert(vector2f.x == 1.0f && vector2f.y == 2.0f, "Expected {1.0, 2.0} but got {%f, %f}", vector2f.x, vector2f.y);
	vector2f = Vector2x_toVector2f(VECTOR2x(0x14000, 0x20000));
	TestCase_assert(vector2f.x == 1.25f && vector2f.y == 2.0f, "Expected {1.25, 2.0} but got {%f, %f}", vector2f.x, vector2f.y);
	vector2f = Vector2d_toVector2f(VECTOR2d(5.0, 6.0));
	TestCase_assert(vector2f.x == 5.0f && vector2f.y == 6.0f, "Expected {5.0, 6.0} but got {%f, %f}", vector2f.x, vector2f.y);
	vector2f = Vector2i_toVector2f(VECTOR2i(5, 6));
	TestCase_assert(vector2f.x == 5.0f && vector2f.y == 6.0f, "Expected {5.0, 6.0} but got {%f, %f}", vector2f.x, vector2f.y);
	vector2f = Vector2x_toVector2f(VECTOR2x(0x50000, 0x60000));
	TestCase_assert(vector2f.x == 5.0f && vector2f.y == 6.0f, "Expected {5.0, 6.0} but got {%f, %f}", vector2f.x, vector2f.y);
	
	vector2i = Vector2d_toVector2i(VECTOR2d(1.25, 2.0));
	TestCase_assert(vector2i.x == 1 && vector2i.y == 2, "Expected {1, 2} but got {%d, %d}", vector2i.x, vector2i.y);
	vector2i = Vector2f_toVector2i(VECTOR2f(1.25f, 2.0f));
	TestCase_assert(vector2i.x == 1 && vector2i.y == 2, "Expected {1, 2} but got {%d, %d}", vector2i.x, vector2i.y);
	vector2i = Vector2x_toVector2i(VECTOR2x(0x14000, 0x20000));
	TestCase_assert(vector2i.x == 1 && vector2i.y == 2, "Expected {1, 2} but got {%d, %d}", vector2i.x, vector2i.y);
	vector2i = Vector2d_toVector2i(VECTOR2d(5.0, 6.0));
	TestCase_assert(vector2i.x == 5 && vector2i.y == 6, "Expected {5, 6} but got {%d, %d}", vector2i.x, vector2i.y);
	vector2i = Vector2f_toVector2i(VECTOR2f(5.0f, 6.0f));
	TestCase_assert(vector2i.x == 5 && vector2i.y == 6, "Expected {5, 6} but got {%d, %d}", vector2i.x, vector2i.y);
	vector2i = Vector2x_toVector2i(VECTOR2x(0x50000, 0x60000));
	TestCase_assert(vector2i.x == 5 && vector2i.y == 6, "Expected {5, 6} but got {%d, %d}", vector2i.x, vector2i.y);
	
	vector2x = Vector2d_toVector2x(VECTOR2d(1.25, 2.0));
	TestCase_assert(vector2x.x == 0x14000 && vector2x.y == 0x20000, "Expected {0x14000, 0x20000} but got {0x%05X, 0x%05X}", vector2x.x, vector2x.y);
	vector2x = Vector2f_toVector2x(VECTOR2f(1.25f, 2.0f));
	TestCase_assert(vector2x.x == 0x14000 && vector2x.y == 0x20000, "Expected {0x14000, 0x20000} but got {0x%05X, 0x%05X}", vector2x.x, vector2x.y);
	vector2x = Vector2i_toVector2x(VECTOR2i(1, 2));
	TestCase_assert(vector2x.x == 0x10000 && vector2x.y == 0x20000, "Expected {0x10000, 0x20000} but got {0x%05X, 0x%05X}", vector2x.x, vector2x.y);
	vector2x = Vector2d_toVector2x(VECTOR2d(5.0, 6.0));
	TestCase_assert(vector2x.x == 0x50000 && vector2x.y == 0x60000, "Expected {0x50000, 0x60000} but got {0x%05X, 0x%05X}", vector2x.x, vector2x.y);
	vector2x = Vector2f_toVector2x(VECTOR2f(5.0f, 6.0f));
	TestCase_assert(vector2x.x == 0x50000 && vector2x.y == 0x60000, "Expected {0x50000, 0x60000} but got {0x%05X, 0x%05X}", vector2x.x, vector2x.y);
	vector2x = Vector2i_toVector2x(VECTOR2i(5, 6));
	TestCase_assert(vector2x.x == 0x50000 && vector2x.y == 0x60000, "Expected {0x50000, 0x60000} but got {0x%05X, 0x%05X}", vector2x.x, vector2x.y);
	
	
	vector3d = Vector3f_toVector3d(VECTOR3f(1.25f, 2.0f, 3.0f));
	TestCase_assert(vector3d.x == 1.25 && vector3d.y == 2.0 && vector3d.z == 3.0, "Expected {1.25, 2.0, 3.0} but got {%f, %f, %f}", vector3d.x, vector3d.y);
	vector3d = Vector3i_toVector3d(VECTOR3i(1, 2, 3));
	TestCase_assert(vector3d.x == 1.0 && vector3d.y == 2.0 && vector3d.z == 3.0, "Expected {1.0, 2.0, 3.0} but got {%f, %f, %f}", vector3d.x, vector3d.y);
	vector3d = Vector3x_toVector3d(VECTOR3x(0x14000, 0x20000, 0x30000));
	TestCase_assert(vector3d.x == 1.25 && vector3d.y == 2.0 && vector3d.z == 3.0, "Expected {1.25, 2.0, 3.0} but got {%f, %f, %f}", vector3d.x, vector3d.y);
	vector3d = Vector3f_toVector3d(VECTOR3f(5.0f, 6.0f, 7.0f));
	TestCase_assert(vector3d.x == 5.0 && vector3d.y == 6.0 && vector3d.z == 7.0, "Expected {5.0, 6.0, 7.0} but got {%f, %f, %f}", vector3d.x, vector3d.y);
	vector3d = Vector3i_toVector3d(VECTOR3i(5, 6, 7));
	TestCase_assert(vector3d.x == 5.0 && vector3d.y == 6.0 && vector3d.z == 7.0, "Expected {5.0, 6.0, 7.0} but got {%f, %f, %f}", vector3d.x, vector3d.y);
	vector3d = Vector3x_toVector3d(VECTOR3x(0x50000, 0x60000, 0x70000));
	TestCase_assert(vector3d.x == 5.0 && vector3d.y == 6.0 && vector3d.z == 7.0, "Expected {5.0, 6.0, 7.0} but got {%f, %f, %f}", vector3d.x, vector3d.y);
	
	vector3f = Vector3d_toVector3f(VECTOR3d(1.25, 2.0, 3.0));
	TestCase_assert(vector3f.x == 1.25f && vector3f.y == 2.0f && vector3f.z == 3.0f, "Expected {1.25, 2.0, 3.0} but got {%f, %f, %f}", vector3f.x, vector3f.y);
	vector3f = Vector3i_toVector3f(VECTOR3i(1, 2, 3));
	TestCase_assert(vector3f.x == 1.0f && vector3f.y == 2.0f && vector3f.z == 3.0f, "Expected {1.0, 2.0, 3.0} but got {%f, %f, %f}", vector3f.x, vector3f.y);
	vector3f = Vector3x_toVector3f(VECTOR3x(0x14000, 0x20000, 0x30000));
	TestCase_assert(vector3f.x == 1.25f && vector3f.y == 2.0f && vector3f.z == 3.0f, "Expected {1.25, 2.0, 3.0} but got {%f, %f, %f}", vector3f.x, vector3f.y);
	vector3f = Vector3d_toVector3f(VECTOR3d(5.0, 6.0, 7.0));
	TestCase_assert(vector3f.x == 5.0f && vector3f.y == 6.0f && vector3f.z == 7.0f, "Expected {5.0, 6.0, 7.0} but got {%f, %f, %f}", vector3f.x, vector3f.y);
	vector3f = Vector3i_toVector3f(VECTOR3i(5, 6, 7));
	TestCase_assert(vector3f.x == 5.0f && vector3f.y == 6.0f && vector3f.z == 7.0f, "Expected {5.0, 6.0, 7.0} but got {%f, %f, %f}", vector3f.x, vector3f.y);
	vector3f = Vector3x_toVector3f(VECTOR3x(0x50000, 0x60000, 0x70000));
	TestCase_assert(vector3f.x == 5.0f && vector3f.y == 6.0f && vector3f.z == 7.0f, "Expected {5.0, 6.0, 7.0} but got {%f, %f, %f}", vector3f.x, vector3f.y);
	
	vector3i = Vector3d_toVector3i(VECTOR3d(1.25, 2.0, 3.0));
	TestCase_assert(vector3i.x == 1 && vector3i.y == 2 && vector3i.z == 3, "Expected {1, 2, 3} but got {%d, %d, %d}", vector3i.x, vector3i.y);
	vector3i = Vector3f_toVector3i(VECTOR3f(1.25f, 2.0f, 3.0f));
	TestCase_assert(vector3i.x == 1 && vector3i.y == 2 && vector3i.z == 3, "Expected {1, 2, 3} but got {%d, %d, %d}", vector3i.x, vector3i.y);
	vector3i = Vector3x_toVector3i(VECTOR3x(0x14000, 0x20000, 0x30000));
	TestCase_assert(vector3i.x == 1 && vector3i.y == 2 && vector3i.z == 3, "Expected {1, 2, 3} but got {%d, %d, %d}", vector3i.x, vector3i.y);
	vector3i = Vector3d_toVector3i(VECTOR3d(5.0, 6.0, 7.0));
	TestCase_assert(vector3i.x == 5 && vector3i.y == 6 && vector3i.z == 7, "Expected {5, 6, 7} but got {%d, %d, %d}", vector3i.x, vector3i.y);
	vector3i = Vector3f_toVector3i(VECTOR3f(5.0f, 6.0f, 7.0f));
	TestCase_assert(vector3i.x == 5 && vector3i.y == 6 && vector3i.z == 7, "Expected {5, 6, 7} but got {%d, %d, %d}", vector3i.x, vector3i.y);
	vector3i = Vector3x_toVector3i(VECTOR3x(0x50000, 0x60000, 0x70000));
	TestCase_assert(vector3i.x == 5 && vector3i.y == 6 && vector3i.z == 7, "Expected {5, 6, 7} but got {%d, %d, %d}", vector3i.x, vector3i.y);
	
	vector3x = Vector3d_toVector3x(VECTOR3d(1.25, 2.0, 3.0));
	TestCase_assert(vector3x.x == 0x14000 && vector3x.y == 0x20000 && vector3x.z == 0x30000, "Expected {0x14000, 0x20000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X}", vector3x.x, vector3x.y);
	vector3x = Vector3f_toVector3x(VECTOR3f(1.25f, 2.0f, 3.0f));
	TestCase_assert(vector3x.x == 0x14000 && vector3x.y == 0x20000 && vector3x.z == 0x30000, "Expected {0x14000, 0x20000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X}", vector3x.x, vector3x.y);
	vector3x = Vector3i_toVector3x(VECTOR3i(1, 2, 3));
	TestCase_assert(vector3x.x == 0x10000 && vector3x.y == 0x20000 && vector3x.z == 0x30000, "Expected {0x10000, 0x20000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X}", vector3x.x, vector3x.y);
	vector3x = Vector3d_toVector3x(VECTOR3d(5.0, 6.0, 7.0));
	TestCase_assert(vector3x.x == 0x50000 && vector3x.y == 0x60000 && vector3x.z == 0x70000, "Expected {0x50000, 0x60000, 0x70000} but got {0x%05X, 0x%05X, 0x%05X}", vector3x.x, vector3x.y);
	vector3x = Vector3f_toVector3x(VECTOR3f(5.0f, 6.0f, 7.0f));
	TestCase_assert(vector3x.x == 0x50000 && vector3x.y == 0x60000 && vector3x.z == 0x70000, "Expected {0x50000, 0x60000, 0x70000} but got {0x%05X, 0x%05X, 0x%05X}", vector3x.x, vector3x.y);
	vector3x = Vector3i_toVector3x(VECTOR3i(5, 6, 7));
	TestCase_assert(vector3x.x == 0x50000 && vector3x.y == 0x60000 && vector3x.z == 0x70000, "Expected {0x50000, 0x60000, 0x70000} but got {0x%05X, 0x%05X, 0x%05X}", vector3x.x, vector3x.y);
	
	
	vector4d = Vector4f_toVector4d(VECTOR4f(1.25f, 2.0f, 3.0f, 4.0f));
	TestCase_assert(vector4d.x == 1.25 && vector4d.y == 2.0 && vector4d.z == 3.0 && vector4d.w == 4.0, "Expected {1.25, 2.0, 3.0, 4.0} but got {%f, %f, %f, %f}", vector4d.x, vector4d.y);
	vector4d = Vector4i_toVector4d(VECTOR4i(1, 2, 3, 4));
	TestCase_assert(vector4d.x == 1.0 && vector4d.y == 2.0 && vector4d.z == 3.0 && vector4d.w == 4.0, "Expected {1.0, 2.0, 3.0, 4.0} but got {%f, %f, %f, %f}", vector4d.x, vector4d.y);
	vector4d = Vector4x_toVector4d(VECTOR4x(0x14000, 0x20000, 0x30000, 0x40000));
	TestCase_assert(vector4d.x == 1.25 && vector4d.y == 2.0 && vector4d.z == 3.0 && vector4d.w == 4.0, "Expected {1.25, 2.0, 3.0, 4.0} but got {%f, %f, %f, %f}", vector4d.x, vector4d.y);
	vector4d = Vector4f_toVector4d(VECTOR4f(5.0f, 6.0f, 7.0f, 8.0f));
	TestCase_assert(vector4d.x == 5.0 && vector4d.y == 6.0 && vector4d.z == 7.0 && vector4d.w == 8.0, "Expected {5.0, 6.0, 7.0, 8.0} but got {%f, %f, %f, %f}", vector4d.x, vector4d.y);
	vector4d = Vector4i_toVector4d(VECTOR4i(5, 6, 7, 8));
	TestCase_assert(vector4d.x == 5.0 && vector4d.y == 6.0 && vector4d.z == 7.0 && vector4d.w == 8.0, "Expected {5.0, 6.0, 7.0, 8.0} but got {%f, %f, %f, %f}", vector4d.x, vector4d.y);
	vector4d = Vector4x_toVector4d(VECTOR4x(0x50000, 0x60000, 0x70000, 0x80000));
	TestCase_assert(vector4d.x == 5.0 && vector4d.y == 6.0 && vector4d.z == 7.0 && vector4d.w == 8.0, "Expected {5.0, 6.0, 7.0, 8.0} but got {%f, %f, %f, %f}", vector4d.x, vector4d.y);
	
	vector4f = Vector4d_toVector4f(VECTOR4d(1.25, 2.0, 3.0, 4.0));
	TestCase_assert(vector4f.x == 1.25f && vector4f.y == 2.0f && vector4f.z == 3.0f && vector4f.w == 4.0f, "Expected {1.25, 2.0, 3.0, 4.0} but got {%f, %f, %f, %f}", vector4f.x, vector4f.y);
	vector4f = Vector4i_toVector4f(VECTOR4i(1, 2, 3, 4));
	TestCase_assert(vector4f.x == 1.0f && vector4f.y == 2.0f && vector4f.z == 3.0f && vector4f.w == 4.0f, "Expected {1.0, 2.0, 3.0, 4.0} but got {%f, %f, %f, %f}", vector4f.x, vector4f.y);
	vector4f = Vector4x_toVector4f(VECTOR4x(0x14000, 0x20000, 0x30000, 0x40000));
	TestCase_assert(vector4f.x == 1.25f && vector4f.y == 2.0f && vector4f.z == 3.0f && vector4f.w == 4.0f, "Expected {1.25, 2.0, 3.0, 4.0} but got {%f, %f, %f, %f}", vector4f.x, vector4f.y);
	vector4f = Vector4d_toVector4f(VECTOR4d(5.0, 6.0, 7.0, 8.0));
	TestCase_assert(vector4f.x == 5.0f && vector4f.y == 6.0f && vector4f.z == 7.0f && vector4f.w == 8.0f, "Expected {5.0, 6.0, 7.0, 8.0} but got {%f, %f, %f, %f}", vector4f.x, vector4f.y);
	vector4f = Vector4i_toVector4f(VECTOR4i(5, 6, 7, 8));
	TestCase_assert(vector4f.x == 5.0f && vector4f.y == 6.0f && vector4f.z == 7.0f && vector4f.w == 8.0f, "Expected {5.0, 6.0, 7.0, 8.0} but got {%f, %f, %f, %f}", vector4f.x, vector4f.y);
	vector4f = Vector4x_toVector4f(VECTOR4x(0x50000, 0x60000, 0x70000, 0x80000));
	TestCase_assert(vector4f.x == 5.0f && vector4f.y == 6.0f && vector4f.z == 7.0f && vector4f.w == 8.0f, "Expected {5.0, 6.0, 7.0, 8.0} but got {%f, %f, %f, %f}", vector4f.x, vector4f.y);
	
	vector4i = Vector4d_toVector4i(VECTOR4d(1.25, 2.0, 3.0, 4.0));
	TestCase_assert(vector4i.x == 1 && vector4i.y == 2 && vector4i.z == 3 && vector4i.w == 4, "Expected {1, 2, 3, 4} but got {%d, %d, %d, %d}", vector4i.x, vector4i.y);
	vector4i = Vector4f_toVector4i(VECTOR4f(1.25f, 2.0f, 3.0f, 4.0f));
	TestCase_assert(vector4i.x == 1 && vector4i.y == 2 && vector4i.z == 3 && vector4i.w == 4, "Expected {1, 2, 3, 4} but got {%d, %d, %d, %d}", vector4i.x, vector4i.y);
	vector4i = Vector4x_toVector4i(VECTOR4x(0x14000, 0x20000, 0x30000, 0x40000));
	TestCase_assert(vector4i.x == 1 && vector4i.y == 2 && vector4i.z == 3 && vector4i.w == 4, "Expected {1, 2, 3, 4} but got {%d, %d, %d, %d}", vector4i.x, vector4i.y);
	vector4i = Vector4d_toVector4i(VECTOR4d(5.0, 6.0, 7.0, 8.0));
	TestCase_assert(vector4i.x == 5 && vector4i.y == 6 && vector4i.z == 7 && vector4i.w == 8, "Expected {5, 6, 7, 8} but got {%d, %d, %d, %d}", vector4i.x, vector4i.y);
	vector4i = Vector4f_toVector4i(VECTOR4f(5.0f, 6.0f, 7.0f, 8.0f));
	TestCase_assert(vector4i.x == 5 && vector4i.y == 6 && vector4i.z == 7 && vector4i.w == 8, "Expected {5, 6, 7, 8} but got {%d, %d, %d, %d}", vector4i.x, vector4i.y);
	vector4i = Vector4x_toVector4i(VECTOR4x(0x50000, 0x60000, 0x70000, 0x80000));
	TestCase_assert(vector4i.x == 5 && vector4i.y == 6 && vector4i.z == 7 && vector4i.w == 8, "Expected {5, 6, 7, 8} but got {%d, %d, %d, %d}", vector4i.x, vector4i.y);
	
	vector4x = Vector4d_toVector4x(VECTOR4d(1.25, 2.0, 3.0, 4.0));
	TestCase_assert(vector4x.x == 0x14000 && vector4x.y == 0x20000 && vector4x.z == 0x30000 && vector4x.w == 0x40000, "Expected {0x14000, 0x20000, 0x30000, 0x40000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}", vector4x.x, vector4x.y);
	vector4x = Vector4f_toVector4x(VECTOR4f(1.25f, 2.0f, 3.0f, 4.0f));
	TestCase_assert(vector4x.x == 0x14000 && vector4x.y == 0x20000 && vector4x.z == 0x30000 && vector4x.w == 0x40000, "Expected {0x14000, 0x20000, 0x30000, 0x40000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}", vector4x.x, vector4x.y);
	vector4x = Vector4i_toVector4x(VECTOR4i(1, 2, 3, 4));
	TestCase_assert(vector4x.x == 0x10000 && vector4x.y == 0x20000 && vector4x.z == 0x30000 && vector4x.w == 0x40000, "Expected {0x10000, 0x20000, 0x30000, 0x40000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}", vector4x.x, vector4x.y);
	vector4x = Vector4d_toVector4x(VECTOR4d(5.0, 6.0, 7.0, 8.0));
	TestCase_assert(vector4x.x == 0x50000 && vector4x.y == 0x60000 && vector4x.z == 0x70000 && vector4x.w == 0x80000, "Expected {0x50000, 0x60000, 0x70000, 0x80000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}", vector4x.x, vector4x.y);
	vector4x = Vector4f_toVector4x(VECTOR4f(5.0f, 6.0f, 7.0f, 8.0f));
	TestCase_assert(vector4x.x == 0x50000 && vector4x.y == 0x60000 && vector4x.z == 0x70000 && vector4x.w == 0x80000, "Expected {0x50000, 0x60000, 0x70000, 0x80000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}", vector4x.x, vector4x.y);
	vector4x = Vector4i_toVector4x(VECTOR4i(5, 6, 7, 8));
	TestCase_assert(vector4x.x == 0x50000 && vector4x.y == 0x60000 && vector4x.z == 0x70000 && vector4x.w == 0x80000, "Expected {0x50000, 0x60000, 0x70000, 0x80000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}", vector4x.x, vector4x.y);
}

TEST_SUITE(VectorConversionsTest,
           testTypeConversions)
