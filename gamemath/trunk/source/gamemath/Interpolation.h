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

#ifndef __Interpolation_H__
#define __Interpolation_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Usage: Initialize an interpolationContext struct with interpolationContextInit(), call one or more of the
//        update methods described below on it, and read its currentValue field to retrieve curved results
//        between startValue and endValue. To determine whether an interpolation has finished, check if
//        currentProgress >= 1.0f.
// 
// - If you wish to use an absolute clock measurement such as Shell_getTime() to update interpolation, first
//   call interpolationStart() to set the initial time value, then call interpolationSetCurrentTime() to
//   advance interpolation to the appropriate point relative to the initial time value.
// 
// - If you wish to use a relative clock measurement to update interpolation, calling interpolationStart() is
//   unnecessary. Instead of calling interpolationSetCurrentTime(), call interpolationAdvance() with the
//   interval since initialization or the last call to interpolationAdvance().
// 
// - If you wish to manually set overall animation progress, call interpolationSetProgress(). currentValue
//   will be updated accordingly.

struct interpolationContext {
	double interval;
	double startTime;
	double currentTime;
	float (* curveFunc)(float value);
	float startValue;
	float endValue;
	float currentValue; // startValue .. endValue, curved
	float currentProgress; // 0.0f .. 1.0f, uncurved
	bool allowExtrapolation;
};

/* Returns a struct interpolationContext initialized to interpolate from startValue to endValue, curved by
   curveFunc (or linearly if curveFunc is NULL) in the amount of time specified by interval. Interval must
   be a nonzero positive number; results are undefined for zero or negative numbers.
   
   If allowExtrapolation is false, any updates that would cause currentProgress to be assigned a value
   outside the 0..1 range will be clamped. Note that this only affects updates to currentProgress, not
   currentValue; if the curve function returns a value outside the 0..1 range, currentValue will be
   extrapolated even if allowExtrapolation is set to false.
   
   If allowExtrapolation is true, currentProgress will not be restricted to 0..1 if the context is updated
   beyond the specified time interval (in either direction) with interpolationAdvance() or
   interpolationSetCurrentTime(), or if interpolationSetProgress() is called with a value outside 0..1.
   
   Curve functions are called with values of currentProgress whenever currentValue is being recalculated.
   The return value of the curve function specifies the interpolation point between startValue and endValue
   for the argument passed to it. In order for currentValue to start at startValue and end at endValue, any
   curve function you implement musyt return 0 for an input of 0 and return 1 for an input of 1. Returning
   other values for these inputs will cause currentValue to start and/or end at a different value than the
   specified startValue and endValue. */
struct interpolationContext interpolationContextInit(float startValue, float endValue, double interval, bool allowExtrapolation, float (* curveFunc)(float value));

/* Initializes the context's startTime field and resets progress to 0. startTime must be set (either by
   calling this function or manually) in order for interpolationSetCurrentTime() to work correctly.
   If this interpolation will only be updated by interpolationAdvance() or interpolationSetProgress(),
   calling interpolationStart() is unnecessary. */
void interpolationStart(struct interpolationContext * context, double startTime);

/* Advances the interpolation by the specified time interval, updating currentProgress and currentValue. */
void interpolationAdvance(struct interpolationContext * context, double timeDelta);

/* Advances the interpolation to the specified time measured relative to the interpolation's startTime,
   updating currentProgress and currentValue. */
void interpolationSetCurrentTime(struct interpolationContext * context, double currentTime);

/* Updates currentProgress to the specified value, updating both currentValue and currentTime. */
void interpolationSetProgress(struct interpolationContext * context, float progress);

float InterpolationCurveFunc_linear(float value);
float InterpolationCurveFunc_easeInSin(float value);
float InterpolationCurveFunc_easeOutSin(float value);
float InterpolationCurveFunc_easeInOutSin(float value);

#ifdef __cplusplus
}
#endif
#endif
