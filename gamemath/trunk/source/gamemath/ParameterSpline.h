/*
  Copyright (c) 2018 Alex Diener
  
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

#ifndef __ParameterSpline_H__
#define __ParameterSpline_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct ParameterSpline ParameterSpline;

#include "gamemath/Vector2f.h"

struct ParameterSpline_curve {
	float width;
	float originY;
	float endpointY;
	// originHandle and endpointHandle are both in relative space to their respective endpoints, with x axis normalized (divided by width)
	Vector2f originHandle;
	Vector2f endpointHandle;
};

struct ParameterSpline {
	unsigned int curveCount;
	struct ParameterSpline_curve * curves;
	unsigned int curveSampleIterations; // Passed to BezierCurve_sampleYAtX; default 8
	float computedLength; // Read only; do not modify directly
};

ParameterSpline ParameterSpline_init(unsigned int curveCount, const struct ParameterSpline_curve * curves);
void ParameterSpline_dispose(ParameterSpline * spline);

float ParameterSpline_sample(ParameterSpline * spline, float t);
void ParameterSpline_recomputeLength(ParameterSpline * spline); // Called automatically at init; call again if curves have been manually modified

// For use with Interpolation.h. value is multiplied by computedLength before calling ParameterSpline_sample,
// mapping interpolationContext's [0..1] range to the full width of the ParameterSpline.
float ParameterSpline_interpolationCurveFunc(float value, void * context);

#ifdef __cplusplus
}
#endif
#endif
