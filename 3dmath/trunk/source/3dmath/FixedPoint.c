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
  
  Alex Diener adiener@sacredsoftware.net
*/

#include "3dmath/FixedPoint.h"
#include "3dmath/Vector2x.h"
#include "3dmath/Vector3x.h"
#include "3dmath/Vector4x.h"

#ifndef USE_SIN_LOOKUP_TABLE
#define USE_SIN_LOOKUP_TABLE 1
#endif

#if USE_SIN_LOOKUP_TABLE
#include "3dmath/FixedPoint_sinLookup.h"
#endif

fixed16_16 fixed16_16_pow(fixed16_16 x, fixed16_16 y) {
	return 0;
}

#if USE_SIN_LOOKUP_TABLE
static fixed16_16 lookUpSinValue(unsigned int index) {
	int sign = 1;
	
	if (index >= SIN_LOOKUP_COUNT * 2) {
		sign = -1;
		index %= SIN_LOOKUP_COUNT * 2;
	}
	if (index >= SIN_LOOKUP_COUNT) {
		return fixed16_16_sinLookup[SIN_LOOKUP_COUNT * 2 - index - 1] * sign;
	}
	return fixed16_16_sinLookup[index] * sign;
}
#endif

#include <stdio.h>

static fixed16_16 fixed16_16_sinInternal(fixed16_16 radians, bool isCos) {
#if USE_SIN_LOOKUP_TABLE
	fixed16_16 ratio, scaledRadians;
	unsigned int indexLow, indexHigh;
	fixed16_16 valueLow, valueHigh;
	
	if (isCos) {
		radians += X_PI - X_HALF_PI;
	}
	radians %= X_2_PI;
	if (radians < 0) {
		radians += X_2_PI;
	}
	
	scaledRadians = (int64_t) radians * (SIN_LOOKUP_COUNT * 4 << 16) / X_2_PI;
	indexLow = floorx(scaledRadians) >> 16;
	indexHigh = ceilx(scaledRadians) >> 16;
	valueLow = lookUpSinValue(indexLow);
	valueHigh = lookUpSinValue(indexHigh);
	ratio = scaledRadians & 0xFFFF;
	if (indexLow % (SIN_LOOKUP_COUNT * 2) >= SIN_LOOKUP_COUNT) {
		ratio = 0x10000 - ratio;
	}
	return valueLow + xmul(valueHigh - valueLow, ratio);
	
#else
	// See http://http.developer.nvidia.com/Cg/sin.html
	Vector4x c0 = VECTOR4x(     0x0,    0x8000,   0x10000,      0x0);
	Vector4x c1 = VECTOR4x(  0x4000,  -0x90000,    0xC000,   0x28BE);
	Vector4x c2 = VECTOR4x(0x18FB15, -0x18FB15, -0x3C2553, 0x3C2553);
	Vector4x c3 = VECTOR4x(0x55742B, -0x55742B, -0x40F079, 0x40F079);
	Vector4x c4 = VECTOR4x(0x13BD3C, -0x13BD3C,  -0x10000,  0x10000);
	Vector3x r0 = VECTOR3x_ZERO;
	Vector3x r1 = VECTOR3x_ZERO;
	Vector3x r2 = VECTOR3x_ZERO;
	
	r1.x = xmul(c1.w, radians) - (isCos ? 0 : c1.x);
	r1.y = r1.x - floorx(r1.x);
	r2.x = r1.y < c1.x ? 0x10000 : 0x0;
	r2.y = r1.y >= c1.y ? 0x10000 : 0x0;
	r2.z = r1.y >= c1.z ? 0x10000 : 0x0;
	r2.y = Vector3x_dot(r2, VECTOR3x(c4.z, c4.w, c4.z));
	r0 = VECTOR3x(c0.x - r1.y, c0.y - r1.y, c0.z - r1.y);
	r0 = VECTOR3x(xmul(r0.x, r0.x), xmul(r0.y, r0.y), xmul(r0.z, r0.z));
	
	r1 = VECTOR3x(xmul(c2.x, r0.x) + c2.z, xmul(c2.y, r0.y) + c2.w, xmul(c2.x, r0.z) + c2.z);
	r1 = VECTOR3x(xmul(r1.x, r0.x) + c3.x, xmul(r1.y, r0.y) + c3.y, xmul(r1.z, r0.z) + c3.x);
	r1 = VECTOR3x(xmul(r1.x, r0.x) + c3.z, xmul(r1.y, r0.y) + c3.w, xmul(r1.z, r0.z) + c3.z);
	r1 = VECTOR3x(xmul(r1.x, r0.x) + c4.x, xmul(r1.y, r0.y) + c4.y, xmul(r1.z, r0.z) + c4.x);
	r1 = VECTOR3x(xmul(r1.x, r0.x) + c4.z, xmul(r1.y, r0.y) + c4.w, xmul(r1.z, r0.z) + c4.z);
	
	return Vector3x_dot(r1, VECTOR3x(-r2.x, -r2.y, -r2.z));
#endif
}

fixed16_16 fixed16_16_sin(fixed16_16 radians) {
	return fixed16_16_sinInternal(radians, false);
}

fixed16_16 fixed16_16_cos(fixed16_16 radians) {
	return fixed16_16_sinInternal(radians, true);
}

fixed16_16 fixed16_16_tan(fixed16_16 radians) {
	return xdiv(sinx(radians), cosx(radians));
}

fixed16_16 fixed16_16_asin(fixed16_16 y) {
	// See http://http.developer.nvidia.com/Cg/asin.html
	fixed16_16 negate, result;
	
	if (y < (fixed16_16) 0xFFFF0000 || y > 0x10000) {
		return FIXED_16_16_NAN;
	}
	
	negate = y < 0 ? 0x20000 : 0;
	y = xabs(y);
	result = -0x4CB;
	result = xmul(result, y);
	result += 0x1303;
	result = xmul(result, y);
	result -= 0x364D;
	result = xmul(result, y);
	result += 0x1921B;
	result = X_HALF_PI - xmul(sqrtx(0x10000 - y), result);
	return result - xmul(negate, result);
}

fixed16_16 fixed16_16_acos(fixed16_16 x) {
	// See http://http.developer.nvidia.com/Cg/acos.html
	fixed16_16 negate, result;
	
	if (x < (fixed16_16) 0xFFFF0000 || x > 0x10000) {
		return FIXED_16_16_NAN;
	}
	
	negate = x < 0 ? 0x20000 : 0;
	x = xabs(x);
	result = -0x4CB;
	result = xmul(result, x);
	result += 0x1303;
	result = xmul(result, x);
	result -= 0x364D;
	result = xmul(result, x);
	result += 0x1921B;
	result = xmul(result, sqrtx(0x10000 - x));
	result -= xmul(negate, result);
	return xmul(negate, X_HALF_PI) + result;
}

fixed16_16 fixed16_16_atan(fixed16_16 y) {
	return fixed16_16_atan2(y, 0x10000);
}

fixed16_16 fixed16_16_atan2(fixed16_16 y, fixed16_16 x) {
	// See http://http.developer.nvidia.com/Cg/atan2.html
	fixed16_16 t0, t1, t3, t4;
	
	t3 = xabs(x);
	t1 = xabs(y);
	t0 = t3 > t1 ? t3 : t1;
	t1 = t3 < t1 ? t3 : t1;
	t3 = xdiv(0x10000, t0);
	t3 = xmul(t1, t3);
	
	t4 = xmul(t3, t3);
	t0 = -0x373;
	t0 = xmul(t0, t4) + 0xEB7;
	t0 = xmul(t0, t4) - 0x1F0A;
	t0 = xmul(t0, t4) + 0x3215;
	t0 = xmul(t0, t4) - 0x553F;
	t0 = xmul(t0, t4) + 0x10000;
	t3 = xmul(t3, t0);
	
	if (xabs(y) > xabs(x)) {
		t3 = X_HALF_PI - t3;
	}
	if (x < 0) {
		t3 = X_PI - t3;
	}
	if (y < 0) {
		t3 = -t3;
	}
	
	return t3;
}
