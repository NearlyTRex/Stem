/*
  Copyright (c) 2015 Alex Diener
  
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

#include "gamemath/Interpolation.h"
#include <math.h>
#include <stdlib.h>

struct interpolationContext interpolationContextInit(float startValue, float endValue, double interval, bool allowExtrapolation, float (* curveFunc)(float value, void * curveFuncContext), void * curveFuncContext) {
	struct interpolationContext context;
	
	context.startValue = startValue;
	context.endValue = endValue;
	context.allowExtrapolation = allowExtrapolation;
	context.interval = interval;
	context.startTime = 0.0;
	context.curveFunc = curveFunc;
	context.curveFuncContext = curveFuncContext;
	interpolationSetProgress(&context, 0.0f);
	
	return context;
}

void interpolationStart(struct interpolationContext * context, double startTime) {
	context->startTime = context->currentTime = startTime;
	interpolationSetProgress(context, 0.0f);
}

void interpolationAdvance(struct interpolationContext * context, double timeDelta) {
	interpolationSetCurrentTime(context, context->currentTime + timeDelta);
}

void interpolationSetCurrentTime(struct interpolationContext * context, double currentTime) {
	interpolationSetProgress(context, (currentTime - context->startTime) / context->interval);
	// If allowExtrapolation is false, interpolationSetProgress may have been clamped and set currentTime to the wrong value
	context->currentTime = currentTime;
}

void interpolationSetProgress(struct interpolationContext * context, float progress) {
	float curvedProgress;
	
	if (!context->allowExtrapolation) {
		if (progress < 0.0f) {
			progress = 0.0f;
		} else if (progress > 1.0f) {
			progress = 1.0f;
		}
	}
	context->currentProgress = progress;
	if (context->curveFunc == NULL) {
		curvedProgress = progress;
	} else {
		curvedProgress = context->curveFunc(progress, context->curveFuncContext);
	}
	context->currentValue = context->startValue + (context->endValue - context->startValue) * curvedProgress;
	context->currentTime = context->startTime + context->interval * progress;
}

float InterpolationCurveFunc_linear(float value, void * context) {
	return value;
}

float InterpolationCurveFunc_easeInSin(float value, void * context) {
	return 1.0f - cosf(value * M_PI * 0.5f);
}

float InterpolationCurveFunc_easeOutSin(float value, void * context) {
	return sinf(value * M_PI * 0.5f);
}

float InterpolationCurveFunc_easeInOutSin(float value, void * context) {
	return 0.5f * sinf(M_PI * (value - 0.5f)) + 0.5f;
}
