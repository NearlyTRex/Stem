/*
  Copyright (c) 2017 Alex Diener
  
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

#include "gamemath/BezierCurve.h"
#include <math.h>
#include <stdlib.h>

Vector2f BezierCurve_sample(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float value) {
	Vector2f midp0p1, midp1p2, midp2p3, midMid1, midMid2, result;
	
	midp0p1.x = p0.x + (p1.x - p0.x) * value;
	midp0p1.y = p0.y + (p1.y - p0.y) * value;
	midp1p2.x = p1.x + (p2.x - p1.x) * value;
	midp1p2.y = p1.y + (p2.y - p1.y) * value;
	midp2p3.x = p2.x + (p3.x - p2.x) * value;
	midp2p3.y = p2.y + (p3.y - p2.y) * value;
	midMid1.x = midp0p1.x + (midp1p2.x - midp0p1.x) * value;
	midMid1.y = midp0p1.y + (midp1p2.y - midp0p1.y) * value;
	midMid2.x = midp1p2.x + (midp2p3.x - midp1p2.x) * value;
	midMid2.y = midp1p2.y + (midp2p3.y - midp1p2.y) * value;
	result.x = midMid1.x + (midMid2.x - midMid1.x) * value;
	result.y = midMid1.y + (midMid2.y - midMid1.y) * value;
	return result;
	
	// Slow
	//return Vector2f_interpolate(Vector2f_interpolate(Vector2f_interpolate(p0, p1, value), Vector2f_interpolate(p1, p2, value), value), Vector2f_interpolate(Vector2f_interpolate(p1, p2, value), Vector2f_interpolate(p2, p3, value), value), value);
}

float BezierCurve_sampleXAtY(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float y, unsigned int iterations) {
	float guess, guessAdjust;
	Vector2f leftSample, rightSample, newSample;
	unsigned int iteration;
	
	if (p0.y < p3.y) {
		if (y < p0.y) {
			return p0.x;
		}
		if (y > p3.y) {
			return p3.x;
		}
	} else {
		if (y < p3.y) {
			return p3.x;
		}
		if (y > p0.y) {
			return p0.x;
		}
	}
	
	guess = 0.5f;
	guessAdjust = 0.25f;
	leftSample = p0;
	rightSample = p3;
	for (iteration = 0; iteration < iterations; iteration++) {
		newSample = BezierCurve_sample(p0, p1, p2, p3, guess);
		if ((newSample.y < y) == (p0.y < p3.y)) {
			guess += guessAdjust;
			guessAdjust *= 0.5f;
			leftSample = newSample;
		} else {
			guess -= guessAdjust;
			guessAdjust *= 0.5f;
			rightSample = newSample;
		}
	}
	
	if (fabsf(leftSample.y - rightSample.y) < 0.00001f) {
		return leftSample.x;
	}
	return leftSample.x + (rightSample.x - leftSample.x) * (y - leftSample.y) / (rightSample.y - leftSample.y);
}

float BezierCurve_sampleYAtX(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float x, unsigned int iterations) {
	return BezierCurve_sampleXAtY(VECTOR2f(p0.y, p0.x), VECTOR2f(p1.y, p1.x), VECTOR2f(p2.y, p2.x), VECTOR2f(p3.y, p3.x), x, iterations);
}

void BezierCurve_getSamples(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, Vector2f * outSamples, unsigned int sampleCount) {
	unsigned int sampleIndex;
	
	for (sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++) {
		outSamples[sampleIndex] = BezierCurve_sample(p0, p1, p2, p3, sampleIndex / (float) (sampleCount - 1));
	}
}
