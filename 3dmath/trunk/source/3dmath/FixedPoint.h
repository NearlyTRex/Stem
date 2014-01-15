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

#ifndef __FixedPoint_H__
#define __FixedPoint_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef int32_t fixed16_16;

#define FIXED_16_16_MAX  ((fixed16_16) 0x7FFFFFFE)
#define FIXED_16_16_MIN  ((fixed16_16) 0x80000002)
#define FIXED_16_16_INF  ((fixed16_16) 0x7FFFFFFF)
#define FIXED_16_16_NINF ((fixed16_16) 0x80000000)
#define FIXED_16_16_NAN  ((fixed16_16) 0x80000001)

#define X_PI      0x3243F
#define X_PI_2    0x1921F
#define X_PI_4    0x0C90F
#define X_E       0x2B7E1
#define X_LN2     0x0B172
#define X_SQRT2   0x16A0A
#define X_SQRT1_2 0x0B505

#define xmul(x, y)   fixed16_16_multiply(x, y)
#define xdiv(x, y)   fixed16_16_divide(x, y)
#define xround(x)    fixed16_16_round(x)
#define xfloor(x)    fixed16_16_floor(x)
#define xceil(x)     fixed16_16_ceil(x)
#define xabs(x)      fixed16_16_abs(x)
#define xsqrt(x)     fixed16_16_sqrt(x)
#define xexp(x)      fixed16_16_exp(x)
#define xlog(x)      fixed16_16_log(x)
#define xpow(x, y)   fixed16_16_pow(x, y)
#define xsin(r)      fixed16_16_sin(r)
#define xcos(r)      fixed16_16_cos(r)
#define xtan(r)      fixed16_16_tan(r)
#define xasin(x)     fixed16_16_asin(x)
#define xacos(y)     fixed16_16_acos(y)
#define xatan(y)     fixed16_16_atan(y)
#define xatan2(y, x) fixed16_16_atan2(y, x)
#define xisinf(y)    fixed16_16_isinf(y)
#define xisnan(y)    fixed16_16_isnan(y)

static inline fixed16_16 floatToFixed16_16(float f) {
	return f * 0x10000;
}

static inline float fixed16_16ToFloat(fixed16_16 x) {
	return (int) x / (float) 0x10000;
}

static inline fixed16_16 doubleToFixed16_16(double d) {
	return d * 0x10000;
}

static inline double fixed16_16ToDouble(fixed16_16 x) {
	return (int) x / (double) 0x10000;
}

static inline fixed16_16 fixed16_16_multiply(fixed16_16 lhs, fixed16_16 rhs) {
	return (fixed16_16) ((int64_t) lhs * (int64_t) rhs >> 16);
}

static inline fixed16_16 fixed16_16_divide(fixed16_16 lhs, fixed16_16 rhs) {
	if (rhs == 0) {
		return FIXED_16_16_INF;
	}
	return (fixed16_16) (((int64_t) lhs << 16) / (int64_t) rhs);
}

static inline fixed16_16 fixed16_16_round(fixed16_16 x) {
	return (x + 0x8000) & 0xFFFF0000;
}

static inline fixed16_16 fixed16_16_abs(fixed16_16 x) {
	return x < 0 ? -x : x;
}

static inline fixed16_16 fixed16_16_sqrt(fixed16_16 x) {
	int64_t estimate = 0x10000, lastEstimate = -1;
	int64_t x64;
	
	if (x < 0) {
		return FIXED_16_16_NAN;
	}
	if (x == 0) {
		return 0;
	}
	x64 = x;
	while (estimate != lastEstimate) {
		lastEstimate = estimate;
		estimate = (((x64 << 16) / estimate) + estimate) / 2;
	}
	return (fixed16_16) estimate;
}

static inline fixed16_16 fixed16_16_floor(fixed16_16 x) {
	return x & 0xFFFF0000;
}

static inline fixed16_16 fixed16_16_ceil(fixed16_16 x) {
	return (x + 0xFFFF) & 0xFFFF0000;
}

static inline bool fixed16_16_isinf(fixed16_16 x) {
	return x == FIXED_16_16_NINF || x == FIXED_16_16_INF;
}

static inline bool fixed16_16_isnan(fixed16_16 x) {
	return x == FIXED_16_16_NAN;
}

fixed16_16 fixed16_16_exp(fixed16_16 x);
fixed16_16 fixed16_16_log(fixed16_16 x);
fixed16_16 fixed16_16_pow(fixed16_16 x, fixed16_16 y);
fixed16_16 fixed16_16_sin(fixed16_16 radians);
fixed16_16 fixed16_16_cos(fixed16_16 radians);
fixed16_16 fixed16_16_tan(fixed16_16 radians);
fixed16_16 fixed16_16_asin(fixed16_16 y);
fixed16_16 fixed16_16_acos(fixed16_16 x);
fixed16_16 fixed16_16_atan(fixed16_16 y);
fixed16_16 fixed16_16_atan2(fixed16_16 y, fixed16_16 x);

#ifdef __cplusplus
}
#endif
#endif
