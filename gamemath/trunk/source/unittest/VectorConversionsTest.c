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

#define assertVector2Equal(vector, expectedX, expectedY, format) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY, "Expected {" format ", " format "} but got {" format ", " format "}", expectedX, expectedY, vector.x, vector.y)
#define assertVector2dEqual(vector, expectedX, expectedY) \
	assertVector2Equal(vector, expectedX, expectedY, "%f")
#define assertVector2fEqual(vector, expectedX, expectedY) \
	assertVector2Equal(vector, expectedX, expectedY, "%f")
#define assertVector2iEqual(vector, expectedX, expectedY) \
	assertVector2Equal(vector, expectedX, expectedY, "%d")
#define assertVector2xEqual(vector, expectedX, expectedY) \
	assertVector2Equal(vector, expectedX, expectedY, "0x%05X")

#define assertVector3Equal(vector, expectedX, expectedY, expectedZ, format) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY && vector.z == expectedZ, "Expected {" format ", " format ", " format "} but got {" format ", " format ", " format "}", expectedX, expectedY, expectedZ, vector.x, vector.y, vector.z)
#define assertVector3dEqual(vector, expectedX, expectedY, expectedZ) \
	assertVector3Equal(vector, expectedX, expectedY, expectedZ, "%f")
#define assertVector3fEqual(vector, expectedX, expectedY, expectedZ) \
	assertVector3Equal(vector, expectedX, expectedY, expectedZ, "%f")
#define assertVector3iEqual(vector, expectedX, expectedY, expectedZ) \
	assertVector3Equal(vector, expectedX, expectedY, expectedZ, "%d")
#define assertVector3xEqual(vector, expectedX, expectedY, expectedZ) \
	assertVector3Equal(vector, expectedX, expectedY, expectedZ, "0x%05X")

#define assertVector4Equal(vector, expectedX, expectedY, expectedZ, expectedW, format) \
	TestCase_assert(vector.x == expectedX && vector.y == expectedY && vector.z == expectedZ && vector.w == expectedW, "Expected {" format ", " format ", " format ", " format "} but got {" format ", " format ", " format ", " format "}", expectedX, expectedY, expectedZ, expectedW, vector.x, vector.y, vector.z, vector.w)
#define assertVector4dEqual(vector, expectedX, expectedY, expectedZ, expectedW) \
	assertVector4Equal(vector, expectedX, expectedY, expectedZ, expectedW, "%f")
#define assertVector4fEqual(vector, expectedX, expectedY, expectedZ, expectedW) \
	assertVector4Equal(vector, expectedX, expectedY, expectedZ, expectedW, "%f")
#define assertVector4iEqual(vector, expectedX, expectedY, expectedZ, expectedW) \
	assertVector4Equal(vector, expectedX, expectedY, expectedZ, expectedW, "%d")
#define assertVector4xEqual(vector, expectedX, expectedY, expectedZ, expectedW) \
	assertVector4Equal(vector, expectedX, expectedY, expectedZ, expectedW, "0x%05X")

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
	assertVector2dEqual(vector2d, 1.25, 2.0);
	vector2d = Vector2i_toVector2d(VECTOR2i(1, 2));
	assertVector2dEqual(vector2d, 1.0, 2.0);
	vector2d = Vector2x_toVector2d(VECTOR2x(0x14000, 0x20000));
	assertVector2dEqual(vector2d, 1.25, 2.0);
	vector2d = Vector2f_toVector2d(VECTOR2f(5.0f, 6.0f));
	assertVector2dEqual(vector2d, 5.0, 6.0);
	vector2d = Vector2i_toVector2d(VECTOR2i(5, 6));
	assertVector2dEqual(vector2d, 5.0, 6.0);
	vector2d = Vector2x_toVector2d(VECTOR2x(0x50000, 0x60000));
	assertVector2dEqual(vector2d, 5.0, 6.0);
	
	vector2f = Vector2d_toVector2f(VECTOR2d(1.25, 2.0));
	assertVector2fEqual(vector2f, 1.25f, 2.0f);
	vector2f = Vector2i_toVector2f(VECTOR2i(1, 2));
	assertVector2fEqual(vector2f, 1.0f, 2.0f);
	vector2f = Vector2x_toVector2f(VECTOR2x(0x14000, 0x20000));
	assertVector2fEqual(vector2f, 1.25f, 2.0f);
	vector2f = Vector2d_toVector2f(VECTOR2d(5.0, 6.0));
	assertVector2fEqual(vector2f, 5.0f, 6.0f);
	vector2f = Vector2i_toVector2f(VECTOR2i(5, 6));
	assertVector2fEqual(vector2f, 5.0f, 6.0f);
	vector2f = Vector2x_toVector2f(VECTOR2x(0x50000, 0x60000));
	assertVector2fEqual(vector2f, 5.0f, 6.0f);
	
	vector2i = Vector2d_toVector2i(VECTOR2d(1.25, 2.0));
	assertVector2iEqual(vector2i, 1, 2);
	vector2i = Vector2f_toVector2i(VECTOR2f(1.25f, 2.0f));
	assertVector2iEqual(vector2i, 1, 2);
	vector2i = Vector2x_toVector2i(VECTOR2x(0x14000, 0x20000));
	assertVector2iEqual(vector2i, 1, 2);
	vector2i = Vector2d_toVector2i(VECTOR2d(5.0, 6.0));
	assertVector2iEqual(vector2i, 5, 6);
	vector2i = Vector2f_toVector2i(VECTOR2f(5.0f, 6.0f));
	assertVector2iEqual(vector2i, 5, 6);
	vector2i = Vector2x_toVector2i(VECTOR2x(0x50000, 0x60000));
	assertVector2iEqual(vector2i, 5, 6);
	
	vector2x = Vector2d_toVector2x(VECTOR2d(1.25, 2.0));
	assertVector2xEqual(vector2x, 0x14000, 0x20000);
	vector2x = Vector2f_toVector2x(VECTOR2f(1.25f, 2.0f));
	assertVector2xEqual(vector2x, 0x14000, 0x20000);
	vector2x = Vector2i_toVector2x(VECTOR2i(1, 2));
	assertVector2xEqual(vector2x, 0x10000, 0x20000);
	vector2x = Vector2d_toVector2x(VECTOR2d(5.0, 6.0));
	assertVector2xEqual(vector2x, 0x50000, 0x60000);
	vector2x = Vector2f_toVector2x(VECTOR2f(5.0f, 6.0f));
	assertVector2xEqual(vector2x, 0x50000, 0x60000);
	vector2x = Vector2i_toVector2x(VECTOR2i(5, 6));
	assertVector2xEqual(vector2x, 0x50000, 0x60000);
	
	
	vector3d = Vector3f_toVector3d(VECTOR3f(1.25f, 2.0f, 3.0f));
	assertVector3dEqual(vector3d, 1.25, 2.0, 3.0);
	vector3d = Vector3i_toVector3d(VECTOR3i(1, 2, 3));
	assertVector3dEqual(vector3d, 1.0, 2.0, 3.0);
	vector3d = Vector3x_toVector3d(VECTOR3x(0x14000, 0x20000, 0x30000));
	assertVector3dEqual(vector3d, 1.25, 2.0, 3.0);
	vector3d = Vector3f_toVector3d(VECTOR3f(5.0f, 6.0f, 7.0f));
	assertVector3dEqual(vector3d, 5.0, 6.0, 7.0);
	vector3d = Vector3i_toVector3d(VECTOR3i(5, 6, 7));
	assertVector3dEqual(vector3d, 5.0, 6.0, 7.0);
	vector3d = Vector3x_toVector3d(VECTOR3x(0x50000, 0x60000, 0x70000));
	assertVector3dEqual(vector3d, 5.0, 6.0, 7.0);
	
	vector3f = Vector3d_toVector3f(VECTOR3d(1.25, 2.0, 3.0));
	assertVector3fEqual(vector3f, 1.25f, 2.0f, 3.0f);
	vector3f = Vector3i_toVector3f(VECTOR3i(1, 2, 3));
	assertVector3fEqual(vector3f, 1.0f, 2.0f, 3.0f);
	vector3f = Vector3x_toVector3f(VECTOR3x(0x14000, 0x20000, 0x30000));
	assertVector3fEqual(vector3f, 1.25f, 2.0f, 3.0f);
	vector3f = Vector3d_toVector3f(VECTOR3d(5.0, 6.0, 7.0));
	assertVector3fEqual(vector3f, 5.0f, 6.0f, 7.0f);
	vector3f = Vector3i_toVector3f(VECTOR3i(5, 6, 7));
	assertVector3fEqual(vector3f, 5.0f, 6.0f, 7.0f);
	vector3f = Vector3x_toVector3f(VECTOR3x(0x50000, 0x60000, 0x70000));
	assertVector3fEqual(vector3f, 5.0f, 6.0f, 7.0f);
	
	vector3i = Vector3d_toVector3i(VECTOR3d(1.25, 2.0, 3.0));
	assertVector3iEqual(vector3i, 1, 2, 3);
	vector3i = Vector3f_toVector3i(VECTOR3f(1.25f, 2.0f, 3.0f));
	assertVector3iEqual(vector3i, 1, 2, 3);
	vector3i = Vector3x_toVector3i(VECTOR3x(0x14000, 0x20000, 0x30000));
	assertVector3iEqual(vector3i, 1, 2, 3);
	vector3i = Vector3d_toVector3i(VECTOR3d(5.0, 6.0, 7.0));
	assertVector3iEqual(vector3i, 5, 6, 7);
	vector3i = Vector3f_toVector3i(VECTOR3f(5.0f, 6.0f, 7.0f));
	assertVector3iEqual(vector3i, 5, 6, 7);
	vector3i = Vector3x_toVector3i(VECTOR3x(0x50000, 0x60000, 0x70000));
	assertVector3iEqual(vector3i, 5, 6, 7);
	
	vector3x = Vector3d_toVector3x(VECTOR3d(1.25, 2.0, 3.0));
	assertVector3xEqual(vector3x, 0x14000, 0x20000, 0x30000);
	vector3x = Vector3f_toVector3x(VECTOR3f(1.25f, 2.0f, 3.0f));
	assertVector3xEqual(vector3x, 0x14000, 0x20000, 0x30000);
	vector3x = Vector3i_toVector3x(VECTOR3i(1, 2, 3));
	assertVector3xEqual(vector3x, 0x10000, 0x20000, 0x30000);
	vector3x = Vector3d_toVector3x(VECTOR3d(5.0, 6.0, 7.0));
	assertVector3xEqual(vector3x, 0x50000, 0x60000, 0x70000);
	vector3x = Vector3f_toVector3x(VECTOR3f(5.0f, 6.0f, 7.0f));
	assertVector3xEqual(vector3x, 0x50000, 0x60000, 0x70000);
	vector3x = Vector3i_toVector3x(VECTOR3i(5, 6, 7));
	assertVector3xEqual(vector3x, 0x50000, 0x60000, 0x70000);
	
	
	vector4d = Vector4f_toVector4d(VECTOR4f(1.25f, 2.0f, 3.0f, 4.0f));
	assertVector4dEqual(vector4d, 1.25, 2.0, 3.0, 4.0);
	vector4d = Vector4i_toVector4d(VECTOR4i(1, 2, 3, 4));
	assertVector4dEqual(vector4d, 1.0, 2.0, 3.0, 4.0);
	vector4d = Vector4x_toVector4d(VECTOR4x(0x14000, 0x20000, 0x30000, 0x40000));
	assertVector4dEqual(vector4d, 1.25, 2.0, 3.0, 4.0);
	vector4d = Vector4f_toVector4d(VECTOR4f(5.0f, 6.0f, 7.0f, 8.0f));
	assertVector4dEqual(vector4d, 5.0, 6.0, 7.0, 8.0);
	vector4d = Vector4i_toVector4d(VECTOR4i(5, 6, 7, 8));
	assertVector4dEqual(vector4d, 5.0, 6.0, 7.0, 8.0);
	vector4d = Vector4x_toVector4d(VECTOR4x(0x50000, 0x60000, 0x70000, 0x80000));
	assertVector4dEqual(vector4d, 5.0, 6.0, 7.0, 8.0);
	
	vector4f = Vector4d_toVector4f(VECTOR4d(1.25, 2.0, 3.0, 4.0));
	assertVector4fEqual(vector4f, 1.25f, 2.0f, 3.0f, 4.0f);
	vector4f = Vector4i_toVector4f(VECTOR4i(1, 2, 3, 4));
	assertVector4fEqual(vector4f, 1.0f, 2.0f, 3.0f, 4.0f);
	vector4f = Vector4x_toVector4f(VECTOR4x(0x14000, 0x20000, 0x30000, 0x40000));
	assertVector4fEqual(vector4f, 1.25f, 2.0f, 3.0f, 4.0f);
	vector4f = Vector4d_toVector4f(VECTOR4d(5.0, 6.0, 7.0, 8.0));
	assertVector4fEqual(vector4f, 5.0f, 6.0f, 7.0f, 8.0f);
	vector4f = Vector4i_toVector4f(VECTOR4i(5, 6, 7, 8));
	assertVector4fEqual(vector4f, 5.0f, 6.0f, 7.0f, 8.0f);
	vector4f = Vector4x_toVector4f(VECTOR4x(0x50000, 0x60000, 0x70000, 0x80000));
	assertVector4fEqual(vector4f, 5.0f, 6.0f, 7.0f, 8.0f);
	
	vector4i = Vector4d_toVector4i(VECTOR4d(1.25, 2.0, 3.0, 4.0));
	assertVector4iEqual(vector4i, 1, 2, 3, 4);
	vector4i = Vector4f_toVector4i(VECTOR4f(1.25f, 2.0f, 3.0f, 4.0f));
	assertVector4iEqual(vector4i, 1, 2, 3, 4);
	vector4i = Vector4x_toVector4i(VECTOR4x(0x14000, 0x20000, 0x30000, 0x40000));
	assertVector4iEqual(vector4i, 1, 2, 3, 4);
	vector4i = Vector4d_toVector4i(VECTOR4d(5.0, 6.0, 7.0, 8.0));
	assertVector4iEqual(vector4i, 5, 6, 7, 8);
	vector4i = Vector4f_toVector4i(VECTOR4f(5.0f, 6.0f, 7.0f, 8.0f));
	assertVector4iEqual(vector4i, 5, 6, 7, 8);
	vector4i = Vector4x_toVector4i(VECTOR4x(0x50000, 0x60000, 0x70000, 0x80000));
	assertVector4iEqual(vector4i, 5, 6, 7, 8);
	
	vector4x = Vector4d_toVector4x(VECTOR4d(1.25, 2.0, 3.0, 4.0));
	assertVector4xEqual(vector4x, 0x14000, 0x20000, 0x30000, 0x40000);
	vector4x = Vector4f_toVector4x(VECTOR4f(1.25f, 2.0f, 3.0f, 4.0f));
	assertVector4xEqual(vector4x, 0x14000, 0x20000, 0x30000, 0x40000);
	vector4x = Vector4i_toVector4x(VECTOR4i(1, 2, 3, 4));
	assertVector4xEqual(vector4x, 0x10000, 0x20000, 0x30000, 0x40000);
	vector4x = Vector4d_toVector4x(VECTOR4d(5.0, 6.0, 7.0, 8.0));
	assertVector4xEqual(vector4x, 0x50000, 0x60000, 0x70000, 0x80000);
	vector4x = Vector4f_toVector4x(VECTOR4f(5.0f, 6.0f, 7.0f, 8.0f));
	assertVector4xEqual(vector4x, 0x50000, 0x60000, 0x70000, 0x80000);
	vector4x = Vector4i_toVector4x(VECTOR4i(5, 6, 7, 8));
	assertVector4xEqual(vector4x, 0x50000, 0x60000, 0x70000, 0x80000);
}

static void testAxisRearrangement() {
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
	
	vector2d = Vector2d_yx(VECTOR2d(1.0, 2.0));
	assertVector2dEqual(vector2d, 2.0, 1.0);
	
	vector2f = Vector2f_yx(VECTOR2f(1.0f, 2.0f));
	assertVector2fEqual(vector2f, 2.0f, 1.0f);
	
	vector2i = Vector2i_yx(VECTOR2i(1, 2));
	assertVector2iEqual(vector2i, 2, 1);
	
	vector2x = Vector2x_yx(VECTOR2x(0x10000, 0x20000));
	assertVector2xEqual(vector2x, 0x20000, 0x10000);
	
	
	vector3d = Vector3d_xzy(VECTOR3d(1.0, 2.0, 3.0));
	assertVector3dEqual(vector3d, 1.0, 3.0, 2.0);
	vector3d = Vector3d_yxz(VECTOR3d(1.0, 2.0, 3.0));
	assertVector3dEqual(vector3d, 2.0, 1.0, 3.0);
	vector3d = Vector3d_yzx(VECTOR3d(1.0, 2.0, 3.0));
	assertVector3dEqual(vector3d, 2.0, 3.0, 1.0);
	vector3d = Vector3d_zxy(VECTOR3d(1.0, 2.0, 3.0));
	assertVector3dEqual(vector3d, 3.0, 1.0, 2.0);
	vector3d = Vector3d_zyx(VECTOR3d(1.0, 2.0, 3.0));
	assertVector3dEqual(vector3d, 3.0, 2.0, 1.0);
	
	vector3f = Vector3f_xzy(VECTOR3f(1.0f, 2.0f, 3.0f));
	assertVector3fEqual(vector3f, 1.0f, 3.0f, 2.0f);
	vector3f = Vector3f_yxz(VECTOR3f(1.0f, 2.0f, 3.0f));
	assertVector3fEqual(vector3f, 2.0f, 1.0f, 3.0f);
	vector3f = Vector3f_yzx(VECTOR3f(1.0f, 2.0f, 3.0f));
	assertVector3fEqual(vector3f, 2.0f, 3.0f, 1.0f);
	vector3f = Vector3f_zxy(VECTOR3f(1.0f, 2.0f, 3.0f));
	assertVector3fEqual(vector3f, 3.0f, 1.0f, 2.0f);
	vector3f = Vector3f_zyx(VECTOR3f(1.0f, 2.0f, 3.0f));
	assertVector3fEqual(vector3f, 3.0f, 2.0f, 1.0f);
	
	vector3i = Vector3i_xzy(VECTOR3i(1, 2, 3));
	assertVector3iEqual(vector3i, 1, 3, 2);
	vector3i = Vector3i_yxz(VECTOR3i(1, 2, 3));
	assertVector3iEqual(vector3i, 2, 1, 3);
	vector3i = Vector3i_yzx(VECTOR3i(1, 2, 3));
	assertVector3iEqual(vector3i, 2, 3, 1);
	vector3i = Vector3i_zxy(VECTOR3i(1, 2, 3));
	assertVector3iEqual(vector3i, 3, 1, 2);
	vector3i = Vector3i_zyx(VECTOR3i(1, 2, 3));
	assertVector3iEqual(vector3i, 3, 2, 1);
	
	vector3x = Vector3x_xzy(VECTOR3x(0x10000, 0x20000, 0x30000));
	assertVector3xEqual(vector3x, 0x10000, 0x30000, 0x20000);
	vector3x = Vector3x_yxz(VECTOR3x(0x10000, 0x20000, 0x30000));
	assertVector3xEqual(vector3x, 0x20000, 0x10000, 0x30000);
	vector3x = Vector3x_yzx(VECTOR3x(0x10000, 0x20000, 0x30000));
	assertVector3xEqual(vector3x, 0x20000, 0x30000, 0x10000);
	vector3x = Vector3x_zxy(VECTOR3x(0x10000, 0x20000, 0x30000));
	assertVector3xEqual(vector3x, 0x30000, 0x10000, 0x20000);
	vector3x = Vector3x_zyx(VECTOR3x(0x10000, 0x20000, 0x30000));
	assertVector3xEqual(vector3x, 0x30000, 0x20000, 0x10000);
	
	
	vector4d = Vector4d_xywz(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 1.0, 2.0, 4.0, 3.0);
	vector4d = Vector4d_xwyz(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 1.0, 4.0, 2.0, 3.0);
	vector4d = Vector4d_xwzy(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 1.0, 4.0, 3.0, 2.0);
	vector4d = Vector4d_xzyw(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 1.0, 3.0, 2.0, 4.0);
	vector4d = Vector4d_xzwy(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 1.0, 3.0, 4.0, 2.0);
	vector4d = Vector4d_yxzw(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 2.0, 1.0, 3.0, 4.0);
	vector4d = Vector4d_yxwz(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 2.0, 1.0, 4.0, 3.0);
	vector4d = Vector4d_ywxz(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 2.0, 4.0, 1.0, 3.0);
	vector4d = Vector4d_ywzx(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 2.0, 4.0, 3.0, 1.0);
	vector4d = Vector4d_yzxw(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 2.0, 3.0, 1.0, 4.0);
	vector4d = Vector4d_yzwx(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 2.0, 3.0, 4.0, 1.0);
	vector4d = Vector4d_zxyw(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 3.0, 1.0, 2.0, 4.0);
	vector4d = Vector4d_zxwy(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 3.0, 1.0, 4.0, 2.0);
	vector4d = Vector4d_zwxy(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 3.0, 4.0, 1.0, 2.0);
	vector4d = Vector4d_zwyx(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 3.0, 4.0, 2.0, 1.0);
	vector4d = Vector4d_zyxw(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 3.0, 2.0, 1.0, 4.0);
	vector4d = Vector4d_zywx(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 3.0, 2.0, 4.0, 1.0);
	vector4d = Vector4d_wxyz(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 4.0, 1.0, 2.0, 3.0);
	vector4d = Vector4d_wxzy(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 4.0, 1.0, 3.0, 2.0);
	vector4d = Vector4d_wzxy(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 4.0, 3.0, 1.0, 2.0);
	vector4d = Vector4d_wzyx(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 4.0, 3.0, 2.0, 1.0);
	vector4d = Vector4d_wyxz(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 4.0, 2.0, 1.0, 3.0);
	vector4d = Vector4d_wyzx(VECTOR4d(1.0, 2.0, 3.0, 4.0));
	assertVector4dEqual(vector4d, 4.0, 2.0, 3.0, 1.0);
	
	vector4f = Vector4f_xywz(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 1.0f, 2.0f, 4.0f, 3.0f);
	vector4f = Vector4f_xwyz(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 1.0f, 4.0f, 2.0f, 3.0f);
	vector4f = Vector4f_xwzy(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 1.0f, 4.0f, 3.0f, 2.0f);
	vector4f = Vector4f_xzyw(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 1.0f, 3.0f, 2.0f, 4.0f);
	vector4f = Vector4f_xzwy(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 1.0f, 3.0f, 4.0f, 2.0f);
	vector4f = Vector4f_yxzw(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 2.0f, 1.0f, 3.0f, 4.0f);
	vector4f = Vector4f_yxwz(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 2.0f, 1.0f, 4.0f, 3.0f);
	vector4f = Vector4f_ywxz(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 2.0f, 4.0f, 1.0f, 3.0f);
	vector4f = Vector4f_ywzx(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 2.0f, 4.0f, 3.0f, 1.0f);
	vector4f = Vector4f_yzxw(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 2.0f, 3.0f, 1.0f, 4.0f);
	vector4f = Vector4f_yzwx(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 2.0f, 3.0f, 4.0f, 1.0f);
	vector4f = Vector4f_zxyw(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 3.0f, 1.0f, 2.0f, 4.0f);
	vector4f = Vector4f_zxwy(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 3.0f, 1.0f, 4.0f, 2.0f);
	vector4f = Vector4f_zwxy(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 3.0f, 4.0f, 1.0f, 2.0f);
	vector4f = Vector4f_zwyx(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 3.0f, 4.0f, 2.0f, 1.0f);
	vector4f = Vector4f_zyxw(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 3.0f, 2.0f, 1.0f, 4.0f);
	vector4f = Vector4f_zywx(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 3.0f, 2.0f, 4.0f, 1.0f);
	vector4f = Vector4f_wxyz(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 4.0f, 1.0f, 2.0f, 3.0f);
	vector4f = Vector4f_wxzy(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 4.0f, 1.0f, 3.0f, 2.0f);
	vector4f = Vector4f_wzxy(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 4.0f, 3.0f, 1.0f, 2.0f);
	vector4f = Vector4f_wzyx(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 4.0f, 3.0f, 2.0f, 1.0f);
	vector4f = Vector4f_wyxz(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 4.0f, 2.0f, 1.0f, 3.0f);
	vector4f = Vector4f_wyzx(VECTOR4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertVector4fEqual(vector4f, 4.0f, 2.0f, 3.0f, 1.0f);
	
	vector4i = Vector4i_xywz(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 1, 2, 4, 3);
	vector4i = Vector4i_xwyz(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 1, 4, 2, 3);
	vector4i = Vector4i_xwzy(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 1, 4, 3, 2);
	vector4i = Vector4i_xzyw(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 1, 3, 2, 4);
	vector4i = Vector4i_xzwy(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 1, 3, 4, 2);
	vector4i = Vector4i_yxzw(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 2, 1, 3, 4);
	vector4i = Vector4i_yxwz(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 2, 1, 4, 3);
	vector4i = Vector4i_ywxz(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 2, 4, 1, 3);
	vector4i = Vector4i_ywzx(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 2, 4, 3, 1);
	vector4i = Vector4i_yzxw(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 2, 3, 1, 4);
	vector4i = Vector4i_yzwx(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 2, 3, 4, 1);
	vector4i = Vector4i_zxyw(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 3, 1, 2, 4);
	vector4i = Vector4i_zxwy(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 3, 1, 4, 2);
	vector4i = Vector4i_zwxy(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 3, 4, 1, 2);
	vector4i = Vector4i_zwyx(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 3, 4, 2, 1);
	vector4i = Vector4i_zyxw(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 3, 2, 1, 4);
	vector4i = Vector4i_zywx(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 3, 2, 4, 1);
	vector4i = Vector4i_wxyz(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 4, 1, 2, 3);
	vector4i = Vector4i_wxzy(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 4, 1, 3, 2);
	vector4i = Vector4i_wzxy(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 4, 3, 1, 2);
	vector4i = Vector4i_wzyx(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 4, 3, 2, 1);
	vector4i = Vector4i_wyxz(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 4, 2, 1, 3);
	vector4i = Vector4i_wyzx(VECTOR4i(1, 2, 3, 4));
	assertVector4iEqual(vector4i, 4, 2, 3, 1);
	
	vector4x = Vector4x_xywz(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x10000, 0x20000, 0x40000, 0x30000);
	vector4x = Vector4x_xwyz(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x10000, 0x40000, 0x20000, 0x30000);
	vector4x = Vector4x_xwzy(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x10000, 0x40000, 0x30000, 0x20000);
	vector4x = Vector4x_xzyw(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x10000, 0x30000, 0x20000, 0x40000);
	vector4x = Vector4x_xzwy(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x10000, 0x30000, 0x40000, 0x20000);
	vector4x = Vector4x_yxzw(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x20000, 0x10000, 0x30000, 0x40000);
	vector4x = Vector4x_yxwz(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x20000, 0x10000, 0x40000, 0x30000);
	vector4x = Vector4x_ywxz(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x20000, 0x40000, 0x10000, 0x30000);
	vector4x = Vector4x_ywzx(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x20000, 0x40000, 0x30000, 0x10000);
	vector4x = Vector4x_yzxw(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x20000, 0x30000, 0x10000, 0x40000);
	vector4x = Vector4x_yzwx(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x20000, 0x30000, 0x40000, 0x10000);
	vector4x = Vector4x_zxyw(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x30000, 0x10000, 0x20000, 0x40000);
	vector4x = Vector4x_zxwy(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x30000, 0x10000, 0x40000, 0x20000);
	vector4x = Vector4x_zwxy(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x30000, 0x40000, 0x10000, 0x20000);
	vector4x = Vector4x_zwyx(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x30000, 0x40000, 0x20000, 0x10000);
	vector4x = Vector4x_zyxw(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x30000, 0x20000, 0x10000, 0x40000);
	vector4x = Vector4x_zywx(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x30000, 0x20000, 0x40000, 0x10000);
	vector4x = Vector4x_wxyz(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x40000, 0x10000, 0x20000, 0x30000);
	vector4x = Vector4x_wxzy(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x40000, 0x10000, 0x30000, 0x20000);
	vector4x = Vector4x_wzxy(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x40000, 0x30000, 0x10000, 0x20000);
	vector4x = Vector4x_wzyx(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x40000, 0x30000, 0x20000, 0x10000);
	vector4x = Vector4x_wyxz(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x40000, 0x20000, 0x10000, 0x30000);
	vector4x = Vector4x_wyzx(VECTOR4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertVector4xEqual(vector4x, 0x40000, 0x20000, 0x30000, 0x10000);
}

static void testAxisSelection() {
	TestCase_assert(false, "Unimplemented");
}

TEST_SUITE(VectorConversionsTest,
           testTypeConversions,
           testAxisRearrangement,
           testAxisSelection)
