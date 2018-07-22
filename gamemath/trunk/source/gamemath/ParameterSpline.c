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

#include "gamemath/ParameterSpline.h"
#include "gamemath/BezierCurve.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ParameterSpline ParameterSpline_init(unsigned int curveCount, const struct ParameterSpline_curve * curves) {
	ParameterSpline spline;
	
	assert(curveCount > 0);
	spline.curveCount = curveCount;
	spline.curves = malloc(sizeof(*spline.curves) * curveCount);
	memcpy(spline.curves, curves, sizeof(*spline.curves) * curveCount);
	
	spline.curveSampleIterations = 8;
	ParameterSpline_recomputeLength(&spline);
	
	return spline;
}

void ParameterSpline_dispose(ParameterSpline * spline) {
	free(spline->curves);
}

float ParameterSpline_sample(ParameterSpline * spline, float t) {
	unsigned int curveIndex;
	
	if (t <= 0.0f) {
		return spline->curves[0].originY;
	}
	if (t >= spline->computedLength) {
		return spline->curves[spline->curveCount - 1].endpointY;
	}
	for (curveIndex = 0; curveIndex < spline->curveCount; curveIndex++) {
		if (spline->curves[curveIndex].width >= t) {
			struct ParameterSpline_curve * curve = spline->curves + curveIndex;
			Vector2f origin, originHandle, endpointHandle, endpoint;
			
			origin = VECTOR2f(0.0f, curve->originY);
			originHandle = VECTOR2f(curve->originHandle.x * curve->width, curve->originHandle.y + curve->originY);
			endpointHandle = VECTOR2f(curve->endpointHandle.x * curve->width + curve->width, curve->endpointHandle.y + curve->endpointY);
			endpoint = VECTOR2f(curve->width, curve->endpointY);
			return BezierCurve_sampleYAtX(origin, originHandle, endpointHandle, endpoint, t, spline->curveSampleIterations);
		}
		
		t -= spline->curves[curveIndex].width;
	}
	return spline->curves[spline->curveCount - 1].endpointY;
}

void ParameterSpline_recomputeLength(ParameterSpline * spline) {
	unsigned int curveIndex;
	
	spline->computedLength = 0.0f;
	for (curveIndex = 0; curveIndex < spline->curveCount; curveIndex++) {
		spline->computedLength += spline->curves[curveIndex].width;
	}
}

float ParameterSpine_interpolationCurveFunc(float value, void * context) {
	ParameterSpline * spline = context;
	return ParameterSpline_sample(spline, value * spline->computedLength);
}
