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
#include <stdlib.h>

// TODO: Profile this implementation vs. raw math without function calls
Vector2f BezierCurve_sample(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float value) {
	return Vector2f_interpolate(Vector2f_interpolate(Vector2f_interpolate(p0, p1, value), Vector2f_interpolate(p1, p2, value), value), Vector2f_interpolate(Vector2f_interpolate(p1, p2, value), Vector2f_interpolate(p2, p3, value), value), value);
}

float BezierCurve_sampleAtX(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float x, unsigned int iterations) {
	return 0.0f;
}

float BezierCurve_sampleAtY(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float y, unsigned int iterations) {
	return 0.0f;
}

void BezierCurve_getSamples(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, Vector2f * outSamples, unsigned int sampleCount) {
	
}

void BezierCurve_getSamplesWithUniformSpacing(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, Vector2f * outSamples, unsigned int sampleCount, unsigned int iterations) {
	
}

unsigned int BezierCurve_getSamplesWithMaxTurningAngle(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float maxRadians, Vector2f * outSamples, unsigned int sampleMaxCount) {
	return 0;
}
