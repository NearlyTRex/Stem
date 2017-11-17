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

#ifndef __BezierCurve_H__
#define __BezierCurve_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gamemath/Vector2f.h"

// p0 and p3 are endpoints; p1 is p0's control point, p2 is p3's control point
Vector2f BezierCurve_sample(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float value);

// Finds the x value of the curve at the specified y coordinate, if possible. Results may be inaccurate for self-overlapping curves.
// iterations specifies how many subdivisions to use when searching; a higher value will take longer but return a more accurate value.
float BezierCurve_sampleXAtY(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float y, unsigned int iterations);

// Same as above, with x and y axes reversed.
float BezierCurve_sampleYAtX(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, float x, unsigned int iterations);

// Fills outSamples with coordinates along the curve, by sampling linearly.
void BezierCurve_getSamples(Vector2f p0, Vector2f p1, Vector2f p2, Vector2f p3, Vector2f * outSamples, unsigned int sampleCount);

#ifdef __cplusplus
}
#endif
#endif
